/*
 * audio_recording.c
 *
 *  Created on: Nov. 7, 2021
 *      Author: Christian Martel
 */
#include "main.h"
#include "qspi_handler.h"
#include "audio_recording.h"
#include "low_power.h"
#include <stdlib.h>
#include <stdint.h>
#include<stdio.h>

//int32_t *dfsdm_buffer_ptr;
//int32_t *dfsdm_buffer_half_ptr;
//uint32_t recorded_size;
//uint8_t dfsdm_stop_flag = 1;

// DAC
uint32_t *dac_buffer_ptr;
uint32_t *dac_buffer_half_ptr;
uint32_t played_size;
uint8_t dac_stop_flag = 1;



//void update_dfsdm_buffer(DFSDM_Filter_HandleTypeDef *hdfsdm_filter, int32_t *dfsdm_buffer_ptr, uint32_t size) {
//	uint32_t remaining_size = DFSDM_AUDIO_SIZE - recorded_size;
//	uint32_t write_size = size;
//	if (write_size >= remaining_size) {
//		write_size = remaining_size;
//	}
//
//	// write half of buffer
//	qspi_write((uint8_t*)dfsdm_buffer_ptr, DFSDM_AUDIO_QSPI_ADDRESS + recorded_size, write_size);
//	recorded_size += write_size;
//}

void update_dac_buffer(DAC_HandleTypeDef *hdac, uint32_t *dac_buffer_ptr, uint32_t size) {
	uint32_t remaining_size =  DAC_AUDIO_SIZE - played_size;
	uint32_t read_size = remaining_size < size ? remaining_size : size;

	// read from flash
	qspi_read((uint8_t*) dac_buffer_ptr, DAC_AUDIO_QSPI_ADDRESS + played_size, read_size);
	played_size += read_size;
}

// play collected audio samples on DAC
void play_audio(DAC_HandleTypeDef *hdac) {
	played_size = 0;
	dac_buffer_ptr = (uint32_t *) calloc(DAC_BUFFER_LENGTH, DAC_DATA_WIDTH);
	dac_buffer_half_ptr = dac_buffer_ptr + (DAC_BUFFER_LENGTH / 2);

	// initial full read
	update_dac_buffer(hdac, dac_buffer_ptr, DAC_BUFFER_LENGTH * DAC_DATA_WIDTH);

	// Start DAC in circular mode
	dac_stop_flag = 0;
	if (HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, dac_buffer_ptr, DAC_BUFFER_LENGTH, DAC_ALIGN_12B_R) == HAL_ERROR) {
		Error_Handler();
	}
	while(!dac_stop_flag);
	free(dac_buffer_ptr);
}

//void record_audio(DFSDM_Filter_HandleTypeDef *hdfsdm_filter) {
//	recorded_size = 0;
//	dfsdm_buffer_ptr = (int32_t *) calloc(DFSDM_BUFFER_LENGTH, DFSDM_DATA_WIDTH);
//	dfsdm_buffer_half_ptr = dfsdm_buffer_ptr + (DFSDM_BUFFER_LENGTH/2);
//
//	dfsdm_stop_flag = 0;
//	if (HAL_DFSDM_FilterRegularStart_DMA(hdfsdm_filter, dfsdm_buffer_ptr,DFSDM_BUFFER_LENGTH) == HAL_ERROR) {
//		Error_Handler();
//	}
//	if(LOW_POWER_MODE){
//		enter_sleep_mode();
//		HAL_ResumeTick();
//	}
//	while(!dfsdm_stop_flag);
//	free(dfsdm_buffer_ptr);
//}
//
//void print_dfsdm_data(uint32_t dfsdm_audio_qspi_address, uint32_t audio_length) {
//	// flash address pointers
//	uint32_t printed_samples = 0;
//
//	// buffer
//	int32_t *buffer = (int32_t *) calloc(DEFAULT_BUFFER_LENGTH, DFSDM_DATA_WIDTH);
//
//	// navigate through all DFSDM audio memory on flash
//	while (printed_samples < audio_length) {
//		uint32_t remaining_samples = audio_length - printed_samples;
//		uint32_t num_of_samples = remaining_samples < DEFAULT_BUFFER_LENGTH ? remaining_samples : DEFAULT_BUFFER_LENGTH;
//
//		// read from flash
//		qspi_read((uint8_t*)buffer, dfsdm_audio_qspi_address + printed_samples*DFSDM_DATA_WIDTH, num_of_samples*DFSDM_DATA_WIDTH);
//
//		// print to terminal
//		for (uint32_t i = 0; i < DEFAULT_BUFFER_LENGTH; i++) {
//			char wav_str[10];
//			// print 16-bit format
//			sprintf(wav_str, "%d", (int)(buffer[i] >> 16));
//			print(wav_str);
//			print(",");
//		}
//
//		printed_samples += num_of_samples;
//	}
//	print("\r\n");
//	// free up memory
//	free(buffer);
//}

void convert_from_dfsdm_to_dac_range(uint32_t dfsdm_audio_qspi_address, uint32_t dac_audio_qspi_address, uint32_t audio_length) {
	// flash address pointers
	uint32_t converted_samples = 0;

	// buffer
	int32_t *dfsdm_buffer = (int32_t *) calloc(DEFAULT_BUFFER_LENGTH, DFSDM_DATA_WIDTH);
	uint32_t *dac_buffer = (uint32_t *) calloc(DEFAULT_BUFFER_LENGTH, DAC_DATA_WIDTH);

	// navigate through all DFSDM audio memory on flash
	while (converted_samples < audio_length) {
		uint32_t remaining_samples = audio_length- converted_samples;
		uint32_t num_of_samples = remaining_samples < DEFAULT_BUFFER_LENGTH ? remaining_samples : DEFAULT_BUFFER_LENGTH;

		// read from flash
		qspi_read((uint8_t*)dfsdm_buffer, dfsdm_audio_qspi_address+converted_samples*DFSDM_DATA_WIDTH, num_of_samples*DFSDM_DATA_WIDTH);

		// convert to DAC range
		// must shift right 8 bits since data is on 24 MSB
		for (uint32_t i = 0; i < DEFAULT_BUFFER_LENGTH; i++) {
			dac_buffer[i] = (uint32_t)((dfsdm_buffer[i] >> 8) * DFSDM_TO_DAC_SCALE_FACTOR + DFSDM_TO_DAC_BIAS);
		}

		// write converted data to flash
		qspi_write((uint8_t*)dac_buffer, dac_audio_qspi_address+converted_samples*DAC_DATA_WIDTH, num_of_samples*DAC_DATA_WIDTH);

		converted_samples += num_of_samples;
	}
	// free up memory
	free(dfsdm_buffer);
	free(dac_buffer);
}


