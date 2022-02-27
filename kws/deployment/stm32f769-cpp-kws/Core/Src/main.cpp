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
#include "ML-KWS-for-MCU/NN/DS_CNN/ds_cnn.h"
#include "ML-KWS-for-MCU/MFCC/mfcc.h"
#include "kws.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ITM_Port32(n) (*((volatile unsigned long *) (0xE0000000+4*n)))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DFSDM_Filter_HandleTypeDef hdfsdm1_filter0;
DFSDM_Channel_HandleTypeDef hdfsdm1_channel5;
DMA_HandleTypeDef hdma_dfsdm1_flt0;

QSPI_HandleTypeDef hqspi;

SAI_HandleTypeDef hsai_BlockA1;
DMA_HandleTypeDef hdma_sai1_a;

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
uint8_t LOW_POWER_MODE = 0;
uint8_t DEBUG_MODE = 1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DFSDM1_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SAI1_Init(void);
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
  MX_DMA_Init();
  MX_DFSDM1_Init();
  MX_QUADSPI_Init();
  MX_USART1_UART_Init();
  MX_SAI1_Init();
  /* USER CODE BEGIN 2 */

  qspi_init();
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
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
		qspi_erase_blocks(WAVE_DATA_QSPI_ADDRESS, 6);
		HAL_Delay(2000);

		ITM_Port32(31) = 2;
		print("Press the blue button and say a keyword\r\n");
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
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
		HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);

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
		DS_CNN *ds_cnn = new DS_CNN();
		MFCC *mfcc = new MFCC(NUM_MFCC_COEFFS, FRAME_LEN, MFCC_DEC_BITS);

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
				ds_cnn->run_nn(mfcc_out, nn_out);
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
		ds_cnn->~DS_CNN();
		delete [] mfcc_out;
		delete [] predictions;
		delete [] average;
		delete [] audio_buffer;
		main_state = SETUP;
		break;
	}
	case AUDIO_TEST:
	{
		audio_player = new AudioPlayer(&hsai_BlockA1);
		audio_player->play_audio(wave_data);

		audio_recorder->print_data(wave_data);
		main_state = NN;
//		main_state = SETUP;
		audio_player->~AudioPlayer();

		break;
	}
	case READY:
	{
		BLUE_BUTTON_WAIT();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_DFSDM1|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_SAI1;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 6;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLSAI;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
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
  hdfsdm1_filter0.Init.FilterParam.Oversampling = 100;
  hdfsdm1_filter0.Init.FilterParam.IntOversampling = 1;
  HAL_DFSDM_FilterInit(&hdfsdm1_filter0);
  hdfsdm1_channel5.Instance = DFSDM1_Channel5;
  hdfsdm1_channel5.Init.OutputClock.Activation = ENABLE;
  hdfsdm1_channel5.Init.OutputClock.Selection = DFSDM_CHANNEL_OUTPUT_CLOCK_SYSTEM;
  hdfsdm1_channel5.Init.OutputClock.Divider = 68;
  hdfsdm1_channel5.Init.Input.Multiplexer = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  hdfsdm1_channel5.Init.Input.DataPacking = DFSDM_CHANNEL_STANDARD_MODE;
  hdfsdm1_channel5.Init.Input.Pins = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  hdfsdm1_channel5.Init.SerialInterface.Type = DFSDM_CHANNEL_SPI_RISING;
  hdfsdm1_channel5.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  hdfsdm1_channel5.Init.Awd.FilterOrder = DFSDM_CHANNEL_FASTSINC_ORDER;
  hdfsdm1_channel5.Init.Awd.Oversampling = 1;
  hdfsdm1_channel5.Init.Offset = 0;
  hdfsdm1_channel5.Init.RightBitShift = 0x00;
  if (HAL_DFSDM_ChannelInit(&hdfsdm1_channel5) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_DFSDM_FilterConfigRegChannel(&hdfsdm1_filter0, DFSDM_CHANNEL_5, DFSDM_CONTINUOUS_CONV_ON);
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
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief SAI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_16K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

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
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOJ, RED_LED_Pin|GREEN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RED_LED_Pin GREEN_LED_Pin */
  GPIO_InitStruct.Pin = RED_LED_Pin|GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  /*Configure GPIO pin : BLUE_BUTTON_Pin */
  GPIO_InitStruct.Pin = BLUE_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BLUE_BUTTON_GPIO_Port, &GPIO_InitStruct);
}
/* USER CODE BEGIN 4 */
//void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
//{
//	switch(GPIO_Pin) {
//	case BLUE_BUTTON_Pin:
//		switch(main_state) {
//		case READY:
//			main_state = RECORDING;
//			break;
//		case RECORDING:
////			dfsdm_stop_flag = 1;
//			audio_recorder->dfsdm_stop_flag = 1;
//			break;
//		case SETUP:
//		case MFCC_TEST:
//		case AUDIO_TEST:
//		case NN:
//			break;
//		}
//		break;
//	}
//}
void BLUE_BUTTON_WAIT()
{
	while(HAL_GPIO_ReadPin(BLUE_BUTTON_GPIO_Port, BLUE_BUTTON_Pin)==GPIO_PIN_RESET);
	switch(main_state) {
	case READY:
		main_state = RECORDING;
		break;
	case RECORDING:
		audio_recorder->dfsdm_stop_flag = 1;
		break;
	case SETUP:
	case MFCC_TEST:
	case AUDIO_TEST:
	case NN:
		break;
	}
}

// SAI Circular DMA callback functions
void HAL_SAI_TxHalfCpltCallback (SAI_HandleTypeDef *hsai) {
	if (hsai->Instance == SAI1_Block_A) {
		audio_player->played_samples += PLAY_HALF_BUFFER_LENGTH;
		if (audio_player->played_samples >= MAX_RECORD_LENGTH) {
			if (HAL_SAI_DMAStop(hsai) == HAL_ERROR) {
				Error_Handler();
			}
			audio_player->sai_stop_flag = 1;
		}
		else {
			audio_player->update_sai_buffer(0, PLAY_HALF_BUFFER_LENGTH);
		}
	}
}

void HAL_SAI_TxCpltCallback (SAI_HandleTypeDef *hsai) {
	audio_player->played_samples += PLAY_HALF_BUFFER_LENGTH;
	if (hsai->Instance == SAI1_Block_A) {
		if (audio_player->played_samples >= MAX_RECORD_LENGTH) {
			if (HAL_SAI_DMAStop(hsai) == HAL_ERROR) {
				Error_Handler();
			}
			audio_player->sai_stop_flag = 1;
		}
		else {
			audio_player->update_sai_buffer(PLAY_HALF_BUFFER_LENGTH, PLAY_HALF_BUFFER_LENGTH);
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
  * @note   This function is called  when TIM13 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM13) {
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
