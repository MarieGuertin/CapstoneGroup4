/*
 * audio_recorder.cpp
 *
 *  Created on: Dec. 26, 2021
 *      Author: chrismartel
 */

#include "audio_recorder.h"
#include "main.h"
#include "low_power.h"
#include "qspi_handler.h"
#include <stdlib.h>
#include <stdint.h>
#include<stdio.h>

WaveData::WaveData(uint32_t qspi_address)
: qspi_address(qspi_address)
{
	num_of_samples = 0;
}

// constructor
AudioRecorder::AudioRecorder(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
: hdfsdm_filter(hdfsdm_filter)
{
	dfsdm_stop_flag = 1;
}

// destructor
AudioRecorder::~AudioRecorder() {
}

// record audio
WaveData * AudioRecorder::record_audio(uint32_t qspi_address) {
	// create audio record instance
	cur_data = new WaveData(qspi_address);

	// start DFSDM
	dfsdm_buffer = (int32_t*) calloc(RECORD_BUFFER_LENGTH, DFSDM_DATA_WIDTH);
	dfsdm_stop_flag = 0;
	if (HAL_DFSDM_FilterRegularStart_DMA(hdfsdm_filter, dfsdm_buffer,RECORD_BUFFER_LENGTH) == HAL_ERROR) {
		Error_Handler();
	}
	// Sleep while waiting for DFSDM conversion
	if(LOW_POWER_MODE){
		enter_sleep_mode();
		HAL_ResumeTick();
	}
	while(!dfsdm_stop_flag);
	delete [] dfsdm_buffer;
	return cur_data;
}

// update buffer. Method called by dfsdm callbacks functions.
void AudioRecorder::update_dfsdm_buffer(uint32_t offset, uint32_t size) {
	uint32_t remaining_samples = MAX_RECORD_LENGTH - cur_data->num_of_samples;
	uint32_t remaining_size = remaining_samples * DFSDM_DATA_WIDTH;
	uint32_t write_size = size;
	if (write_size >= remaining_size) {
		write_size = remaining_size;
	}

	// write half of buffer
	qspi_write((uint8_t*)(dfsdm_buffer + offset), cur_data->qspi_address + (cur_data->num_of_samples * DFSDM_DATA_WIDTH), write_size);
	cur_data->num_of_samples += (write_size / DFSDM_DATA_WIDTH);
}

// print recorded data in 16-bit signed format
void AudioRecorder::print_data(WaveData * data) {
	// flash address pointers
	uint32_t printed_samples = 0;

	// buffer
	int32_t *buffer = (int32_t *) calloc(PRINT_BUFFER_LENGTH, DFSDM_DATA_WIDTH);

	// navigate through all DFSDM audio memory on flash
	while (printed_samples < data->num_of_samples) {
		uint32_t remaining_samples = data->num_of_samples - printed_samples;
		uint32_t read_samples = remaining_samples < PRINT_BUFFER_LENGTH ? remaining_samples : PRINT_BUFFER_LENGTH;

		// read from flash
		qspi_read((uint8_t*)buffer, data->qspi_address + printed_samples*DFSDM_DATA_WIDTH, read_samples*DFSDM_DATA_WIDTH);

		// print to terminal
		for (uint32_t i = 0; i < PRINT_BUFFER_LENGTH; i++) {
			char wav_str[10];
			// print 16-bit format
			sprintf(wav_str, "%d", (int)(buffer[i] >> 16));
			print(wav_str);
			print(",");
		}

		printed_samples += read_samples;
	}
	print("\r\n");
	// free up memory
	delete [] buffer;
}
