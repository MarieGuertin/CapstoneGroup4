/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "low_power.h"
#include "qspi_handler.h"
#include "audio_recorder.h"
#include "audio_player.h"
#include "arm_math.h"
#include "ML-KWS-for-MCU/MFCC/mfcc.h"
#include "kws.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// X-CUBE-AI includes
#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "cnn_model_data.h"
#include "cnn_model.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ARM_MATH_CM4
#define ITM_Port32(n) (*((volatile unsigned long *) (0xE0000000+4*n)))

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;
DAC_HandleTypeDef hdac1;
DMA_HandleTypeDef hdma_dac_ch1;

DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel2;
DMA_HandleTypeDef hdma_dfsdm1_flt0;

QSPI_HandleTypeDef hqspi;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
/* USER CODE BEGIN PV */
// State Machine
// SETUP: erasing Quad-SPI Flash
// READY: waits for the user to push the button to start recording
// RECORDING: records data from the DFSDM, waits for second push or timeout to stop
// NN: preprocess the audio data and forward into the neural network
// DAC_TEST: testing audio signal through the dac output
enum MAIN_STATE {READY, SETUP, RECORDING, NN, AUDIO_TEST, MFCC_TEST};
volatile enum MAIN_STATE main_state;

// text buffer
char uart_buffer[100] = "";

AudioRecorder *audio_recorder;
WaveData *wave_data;

AudioPlayer *audio_player;
//DACData *dac_data;

// Flags
uint8_t LOW_POWER_MODE = 1;
uint8_t DEBUG_MODE = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC1_Init(void);
static void MX_DFSDM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_QUADSPI_Init();
  MX_DMA_Init();
  MX_DAC1_Init();
  MX_DFSDM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
  qspi_init();
  HAL_TIM_Base_Start_IT(&htim2);
  main_state = SETUP;

  char output_class[12][8] = {"Silence", "Unknown","yes","no","up","down","left","right","on","off","stop","go"};

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
  	switch(main_state) {
  	case SETUP:
  	{
  		ITM_Port32(31) = 1;
  		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

  		// To indicate to user, don't do nothing when red light
  		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
  		qspi_erase_blocks(WAVE_DATA_QSPI_ADDRESS, 6);

  		ITM_Port32(31) = 2;
  		print("Press the blue button and say a keyword\r\n");
  		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
  	  	main_state = READY;
  		break;
  	}
  	case RECORDING:
  	{
  		ITM_Port32(31) = 3;
  		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
  		audio_recorder = new AudioRecorder(&hdfsdm1_filter0);
  		wave_data = audio_recorder->record_audio(WAVE_DATA_QSPI_ADDRESS);

  		ITM_Port32(31) = 4;
  		HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
  //		main_state = NN;
  		audio_recorder->~AudioRecorder();
  		main_state = AUDIO_TEST;
  		break;
  	}
  	case NN:
  	{
  		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);

  		// input buffer
  		int16_t *audio_buffer = new int16_t[(RECORDING_WINDOW_LENGTH+1)*FRAME_SHIFT];

  		// mfcc coefficients
  		q7_t *mfcc_out = (q7_t*) calloc(NUM_FRAMES * NUM_MFCC_COEFFS, sizeof(q7_t));

  		// output buffer
  		q7_t *predictions = (q7_t*) calloc(NUM_PREDICTIONS * NUM_OUTPUT_CLASSES, sizeof(q7_t));

  		// average predictions
  		q7_t *average = new q7_t[NUM_OUTPUT_CLASSES];

  		uint32_t pred_index;

  		q7_t *mfcc_head;
  		//DS_CNN *ds_cnn = new DS_CNN();
  		MFCC *mfcc = new MFCC(NUM_MFCC_COEFFS, FRAME_LEN, MFCC_DEC_BITS);

  		////////////////////////
  		// Build NN layers with pretrained weights
		ai_error ai_err;
		ai_i32 nbatch;

		// Chunk of memory used to hold intermediate values for the NN
		AI_ALIGNED(4) ai_u8 *activations = calloc(AI_NETWORK_DATA_ACTIVATIONS_SIZE, sizeof(ai_u8));//[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

		// Buffers used to store input and output tensors
		AI_ALIGNED(4) ai_i8 *in_data = calloc(AI_NETWORK_IN_1_SIZE_BYTES, sizeof(ai_i8));
		AI_ALIGNED(4) ai_i8 *out_data = calloc(AI_NETWORK_OUT_1_SIZE_BYTES, sizeof(ai_i8));

		// Pointer to our model
		ai_handle cnn_model = AI_HANDLE_NULL;

		// Initialize wrapper structs that hold pointers to data and info about
		// data (tensor height, width, channels)
		ai_buffer ai_input = AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_FLOAT, 49, 10, 1, 1, NULL);
		ai_buffer ai_output = AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_FLOAT, 1, 1, 12, 1, NULL);

		// Set working memory and get weights/biases from model
		ai_network_params ai_params = AI_NETWORK_PARAMS_INIT(
				AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()),
				AI_NETWORK_DATA_ACTIVATIONS(activations)
		);

		// Set pointers wrapper structs to our data buffers
		ai_input.n_batches = 1;
		ai_input.data = AI_HANDLE_PTR(in_data);
		ai_output.n_batches = 1;
		ai_output.data = AI_HANDLE_PTR(out_data);

		ITM_Port32(31) = 7;
		// create instance of neural network
		ai_err = ai_network_create(&cnn_model, AI_NETWORK_DATA_CONFIG);
		if (ai_err.type != AI_ERROR_NONE)
		{
			// error creating model
			Error_Handler();
		}

		if(!ai_network_init(cnn_model, &ai_params))
		{
			ai_err = ai_network_get_error(cnn_model);
			Error_Handler();
		}

		// read MFCC matrix from Q-SPI Flash
		ai_float mfcc_buff[NUM_FRAMES][MFCC_NUM_COEFFICIENTS];
		if (BSP_QSPI_Read((uint8_t *) mfcc_buff, MFCC_QSPI_ADDRESS, sizeof(mfcc_buff)) != QSPI_OK)
			Error_Handler();

		// Fill input buffer
		for (uint32_t i = 0; i < AI_NETWORK_IN_1_SIZE; i++) {
			uint32_t row_index = (uint32_t) (i / MFCC_NUM_COEFFICIENTS);
			uint32_t col_index = i % MFCC_NUM_COEFFICIENTS;
			((ai_float*)in_data)[i] = mfcc_buff[row_index][col_index];
		}

		// forward MFCC matrix to the NN
		nbatch = ai_network_run(cnn_model, &ai_input, &ai_output);
		if (nbatch != 1) {
			// could not run inference
			Error_Handler();
		}

		// Read predicted output
		for(uint32_t i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {
			y_pred[i] = ((float*)out_data)[i];
		}

		// Get label index with highest prediction
		uint32_t pred_index = 0;
		for(uint32_t i = 0; i < AI_NETWORK_OUT_1_SIZE; i++){
			if(y_pred[i]>y_pred[pred_index])
				pred_index = i;
		}

		// free allocated memory
		free(activations);
		free(in_data);
		free(out_data);
		// destroy model and free allocated memory
		ai_network_destroy(cnn_model);


		///////////

  		bool keyword_detected = false;

  		q7_t *average_window_head = predictions;
  		for (uint32_t i = 0; i < NUM_PREDICTIONS; i ++) {
  				qspi_read((uint8_t*)audio_buffer, WAVE_DATA_QSPI_ADDRESS + (i * RECORDING_WINDOW_LENGTH * FRAME_SHIFT * WAVE_DATA_WIDTH), RECORDING_WINDOW_SIZE);
  				// move old data to the left
  				arm_copy_q7(mfcc_out + (RECORDING_WINDOW_LENGTH * NUM_MFCC_COEFFS), mfcc_out, (NUM_FRAMES - RECORDING_WINDOW_LENGTH) * NUM_MFCC_COEFFS * sizeof(q7_t));
  				mfcc_head = mfcc_out + ((NUM_FRAMES-RECORDING_WINDOW_LENGTH) * NUM_MFCC_COEFFS);
  				for (uint32_t j = 0; j < RECORDING_WINDOW_LENGTH; j ++) {
  					mfcc->mfcc_compute(audio_buffer + (j * FRAME_SHIFT), mfcc_head);
  					mfcc_head += NUM_MFCC_COEFFS;
  				}
  				q7_t* nn_out = predictions + (i * NUM_OUTPUT_CLASSES);
  				//ds_cnn->run_nn(mfcc_out, nn_out);
  //				arm_softmax_q7(nn_out,NUM_OUTPUT_CLASSES,nn_out);

  				// get prediction for each recording window
  				if (DEBUG_MODE) {
  					pred_index = get_top_class(nn_out);
  					sprintf(uart_buffer, "Prediction: \"%s\" score: %d\r\n", output_class[pred_index], nn_out[pred_index]);
  					print(uart_buffer);
  				}

  				// increment average window pointer
  				if (i >= AVERAGE_WINDOW_LENGTH) {
  					average_window_head += NUM_OUTPUT_CLASSES;
  				}
  				average_predictions(average, average_window_head, AVERAGE_WINDOW_LENGTH, NUM_OUTPUT_CLASSES);
  				pred_index = get_top_class(average);

  				if (DEBUG_MODE) {
  					sprintf(uart_buffer, "Average: \"%s\" score: %d\r\n", output_class[pred_index], average[pred_index]);
  					print(uart_buffer);
  				}

  				if (average[pred_index] / 128.0 * 100 > DETECTION_THRESHOLD) {
  					sprintf(uart_buffer, "Keyword Detected: \"%s\"\r\n", output_class[pred_index]);
  					print(uart_buffer);
  					keyword_detected = true;
  					break;
  				}
  		}

  		if (!keyword_detected) {
  			print("No keyword detected.");
  		}

  		mfcc->~MFCC();
  		//ds_cnn->~DS_CNN();
  		delete [] mfcc_out;
  		delete [] predictions;
  		delete [] average;
  		delete [] audio_buffer;
  		main_state = SETUP;
  		break;
  	}
  	case AUDIO_TEST:
  	{
  		audio_player = new AudioPlayer(&hdac1);
  		audio_player->play_audio(wave_data);

  //		audio_recorder->print_data(wave_data);
  		main_state = NN;
  //		main_state = SETUP;
  		audio_player->~AudioPlayer();

  		break;
  	}
  	case READY:
  	{
  		if (LOW_POWER_MODE)
  			enter_sleep_mode();
  		break;
  	}
  	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */
  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief DFSDM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DFSDM1_Init(void)
{

  /* USER CODE BEGIN DFSDM1_Init 0 */

  /* USER CODE END DFSDM1_Init 0 */

  /* USER CODE BEGIN DFSDM1_Init 1 */

  /* USER CODE END DFSDM1_Init 1 */
  hdfsdm1_filter0.Instance = DFSDM1_Filter0;
  hdfsdm1_filter0.Init.RegularParam.Trigger = DFSDM_FILTER_SW_TRIGGER;
  hdfsdm1_filter0.Init.RegularParam.FastMode = ENABLE;
  hdfsdm1_filter0.Init.RegularParam.DmaMode = ENABLE;
  hdfsdm1_filter0.Init.FilterParam.SincOrder = DFSDM_FILTER_SINC3_ORDER;
  hdfsdm1_filter0.Init.FilterParam.Oversampling = 200;
  hdfsdm1_filter0.Init.FilterParam.IntOversampling = 1;
  if (HAL_DFSDM_FilterInit(&hdfsdm1_filter0) != HAL_OK)
  {
    Error_Handler();
  }
  hdfsdm1_channel2.Instance = DFSDM1_Channel2;
  hdfsdm1_channel2.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel2.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel2.Init.OutputClock.Divider = 25;
  hdfsdm1_channel2.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel2.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel2.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel2.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel2.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel2.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel2.Init.Awd.Oversampling = 1;
  hdfsdm1_channel2.Init.Offset = 0;
  hdfsdm1_channel2.Init.RightBitShift = 0x00;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter0, DFSDM_CHANNEL_2, DFSDM_CONTINUOUS_CONV_ON) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DFSDM1_Init 2 */

  /* USER CODE END DFSDM1_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 5000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : RED_LED_Pin */
  GPIO_InitStruct.Pin = RED_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RED_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GREEN_LED_Pin */
  GPIO_InitStruct.Pin = GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GREEN_LED_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
// callback function of GPIO interrupts
void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
{
	switch(GPIO_Pin) {
	case BUTTON_Pin:
		switch(main_state) {
		case READY:
			main_state = RECORDING;
			break;
		case RECORDING:
//			dfsdm_stop_flag = 1;
			audio_recorder->dfsdm_stop_flag = 1;
			break;
		case SETUP:
		case MFCC_TEST:
		case AUDIO_TEST:
		case NN:
			break;
		}
		break;
	}
}

// DAC Circular DMA callback functions
void HAL_DAC_ConvHalfCpltCallbackCh1 (DAC_HandleTypeDef * hdac) {
	if (hdac->Instance == DAC1) {
		audio_player->played_samples += PLAY_HALF_BUFFER_LENGTH;
		if (audio_player->played_samples >= MAX_RECORD_LENGTH) {
			if (HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_1) == HAL_ERROR) {
				Error_Handler();
			}
			audio_player->dac_stop_flag = 1;
		}
		else {
			audio_player->update_dac_buffer(0, PLAY_HALF_BUFFER_LENGTH);
		}
	}
}

void HAL_DAC_ConvCpltCallbackCh1 (DAC_HandleTypeDef * hdac) {
	audio_player->played_samples += PLAY_HALF_BUFFER_LENGTH;
	if (hdac->Instance == DAC1) {
		if (audio_player->played_samples >= MAX_RECORD_LENGTH) {
			if (HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_1) == HAL_ERROR) {
				Error_Handler();
			}
			audio_player->dac_stop_flag = 1;
		}
		else {
			audio_player->update_dac_buffer(PLAY_HALF_BUFFER_LENGTH, PLAY_HALF_BUFFER_LENGTH);
		}
	}
}

// DFSDM Circular DMA Callback Functions
void HAL_DFSDM_FilterRegConvHalfCpltCallback (DFSDM_Filter_HandleTypeDef *hdfsdm_filter) {
	if (hdfsdm_filter == &hdfsdm1_filter0) {
		audio_recorder->update_wave_buffer(0, RECORD_HALF_BUFFER_LENGTH);
		if (audio_recorder->cur_data->num_of_samples >= MAX_RECORD_LENGTH) {
			audio_recorder->dfsdm_stop_flag = 1;
			if (HAL_DFSDM_FilterRegularStop_DMA(hdfsdm_filter) == HAL_ERROR) {
				Error_Handler();
			}
		}
	}
}

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter) {
	if (hdfsdm_filter == &hdfsdm1_filter0) {
		audio_recorder->update_wave_buffer(RECORD_HALF_BUFFER_LENGTH, RECORD_HALF_BUFFER_LENGTH);
		if (audio_recorder->cur_data->num_of_samples >= MAX_RECORD_LENGTH) {
			audio_recorder->dfsdm_stop_flag = 1;
			if (HAL_DFSDM_FilterRegularStop_DMA(hdfsdm_filter) == HAL_ERROR) {
				Error_Handler();
			}
		}

	}
}

void print(const char* txt) {
	strcpy(uart_buffer, txt);
  	HAL_UART_Transmit(&huart1, (uint8_t *)uart_buffer, strlen(txt), 10);
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
		__BKPT();
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
