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

int32_t *dfsdm_buffer_ptr;
int32_t *dfsdm_buffer_half_ptr;

// DAC
uint32_t *dac_buffer_ptr;
uint32_t *dac_buffer_half_ptr;

uint8_t dac_stop_flag = 1;
uint8_t dfsdm_stop_flag = 1;

uint32_t dac_address_checkpoint;
uint32_t dfsdm_address_checkpoint;


void update_dfsdm_buffer(DFSDM_Filter_HandleTypeDef *hdfsdm_filter, int32_t *dfsdm_buffer_ptr, uint32_t size) {
	uint32_t remaining_size = DFSDM_AUDIO_SIZE - (dfsdm_address_checkpoint - DFSDM_START_QSPI_ADDRESS);
	uint32_t write_size = size;
	if (write_size >= remaining_size) {
		write_size = remaining_size;
	}

	// write half of buffer
	qspi_write((uint8_t*)dfsdm_buffer_ptr, dfsdm_address_checkpoint, write_size);
}

void update_dac_buffer(DAC_HandleTypeDef *hdac, uint32_t *dac_buffer_ptr, uint32_t size) {
	uint32_t remaining_size =  DAC_START_QSPI_ADDRESS + DAC_AUDIO_SIZE - dac_address_checkpoint;
	uint32_t read_size = remaining_size < size ? remaining_size : size;

	// read from flash
	qspi_read((uint8_t*) dac_buffer_ptr, dac_address_checkpoint, read_size);
}

// play collected audio samples on DAC
void play_audio(DAC_HandleTypeDef *hdac) {
	dac_address_checkpoint = DAC_START_QSPI_ADDRESS;

	dac_buffer_ptr = (uint32_t *) calloc(DAC_BUFFER_LENGTH, sizeof(uint32_t));
	dac_buffer_half_ptr = dac_buffer_ptr + (DAC_BUFFER_LENGTH / 2);

	// initial full read
	update_dac_buffer(hdac, dac_buffer_ptr, DAC_BUFFER_SIZE);

	// Start DAC in circular mode
	dac_stop_flag = 0;
	HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, dac_buffer_ptr, DAC_BUFFER_LENGTH, DAC_ALIGN_12B_R);
	while(!dac_stop_flag);
	free(dac_buffer_ptr);
}

void record_audio(DFSDM_Filter_HandleTypeDef *hdfsdm_filter) {
	dfsdm_address_checkpoint = DFSDM_START_QSPI_ADDRESS;
	dfsdm_buffer_ptr = (int32_t *) calloc(DFSDM_BUFFER_LENGTH, sizeof(uint32_t));
	dfsdm_buffer_half_ptr = dfsdm_buffer_ptr + (DFSDM_BUFFER_LENGTH/2);

	dfsdm_stop_flag = 0;
	HAL_DFSDM_FilterRegularStart_DMA(hdfsdm_filter, dfsdm_buffer_ptr,DFSDM_BUFFER_LENGTH);
	if(LOW_POWER_MODE){
		enter_sleep_mode();
		HAL_ResumeTick();
	}
	while(!dfsdm_stop_flag);
	free(dfsdm_buffer_ptr);
}

void print_dfsdm_data() {
	// flash address pointers
	uint32_t cur_dfsdm_audio_qspi_address = DFSDM_START_QSPI_ADDRESS;

	// buffer
	int32_t *buffer_arr = (int32_t *) calloc(DFSDM_BUFFER_LENGTH, sizeof(int32_t));

	// navigate through all DFSDM audio memory on flash
	while (cur_dfsdm_audio_qspi_address < DFSDM_START_QSPI_ADDRESS + DFSDM_AUDIO_SIZE) {
		uint32_t remaining_size = DFSDM_START_QSPI_ADDRESS + DFSDM_AUDIO_SIZE - cur_dfsdm_audio_qspi_address;
		uint32_t size = remaining_size < DFSDM_BUFFER_SIZE ? remaining_size : DFSDM_BUFFER_SIZE;

		// read from flash
		qspi_read((uint8_t*)buffer_arr, cur_dfsdm_audio_qspi_address, size);

		// print to terminal
		for (uint32_t i = 0; i < DFSDM_BUFFER_LENGTH; i++) {
			char dfsdm_value_str[10];
			sprintf(dfsdm_value_str, "%d", (buffer_arr[i] >> 16));
			print(dfsdm_value_str);
			print(",");
		}

		cur_dfsdm_audio_qspi_address += size;
	}
	print("\r\n");
	// free up memory
	free(buffer_arr);
}

void convert_from_dfsdm_to_dac_range() {
	// flash address pointers
	uint32_t cur_dfsdm_audio_qspi_address = DFSDM_START_QSPI_ADDRESS;
	uint32_t cur_dac_audio_qspi_address = DAC_START_QSPI_ADDRESS;

	// buffer
	int32_t *buffer_arr = (int32_t *) calloc(DAC_BUFFER_LENGTH, sizeof(int32_t));

	// navigate through all DFSDM audio memory on flash
	while (cur_dfsdm_audio_qspi_address < DFSDM_START_QSPI_ADDRESS + DFSDM_AUDIO_SIZE) {
		uint32_t remaining_size = DFSDM_START_QSPI_ADDRESS + DFSDM_AUDIO_SIZE - cur_dfsdm_audio_qspi_address;
		uint32_t size = remaining_size < DAC_BUFFER_SIZE ? remaining_size : DAC_BUFFER_SIZE;

		// read from flash
		qspi_read((uint8_t*)buffer_arr, cur_dfsdm_audio_qspi_address, size);

		// convert to DAC range
		// must shift right 8 bits since data is on 24 MSB
		for (uint32_t i = 0; i < DAC_BUFFER_LENGTH; i++) {
			buffer_arr[i] = (buffer_arr[i] >> 8) * DFSDM_TO_DAC_SCALE_FACTOR + DFSDM_TO_DAC_BIAS;
		}

		// write converted data to flash
		qspi_write((uint8_t*)buffer_arr, cur_dac_audio_qspi_address, size);

		cur_dfsdm_audio_qspi_address += size;
		cur_dac_audio_qspi_address += size;
	}
	// free up memory
	free(buffer_arr);
}



