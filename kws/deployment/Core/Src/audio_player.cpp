/*
 * audio_player.cpp
 *
 *  Created on: Dec. 26, 2021
 *      Author: chrismartel
 */

#include "audio_player.h"
#include "audio_recorder.h"
#include "main.h"
#include "low_power.h"
#include "qspi_handler.h"
#include <stdlib.h>
#include <stdint.h>
#include<stdio.h>

DACData::DACData(uint32_t qspi_address)
: qspi_address(qspi_address)
{
	num_of_samples = 0;
	played_samples = 0;
}

// constructor
AudioPlayer::AudioPlayer(DAC_HandleTypeDef *hdac)
: hdac(hdac)
{
	dac_stop_flag = 1;
}

// destructor
AudioPlayer::~AudioPlayer() {
}

// record audio
void AudioPlayer::play_audio(DACData * data) {
	cur_data = data;
	cur_data->played_samples = 0;

	dac_buffer = (uint32_t *) calloc(PLAY_BUFFER_LENGTH, DAC_DATA_WIDTH);

	// initial full read
	update_dac_buffer(0, PLAY_BUFFER_LENGTH * DAC_DATA_WIDTH);

	// Start DAC in circular mode
	dac_stop_flag = 0;
	if (HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, dac_buffer, PLAY_BUFFER_LENGTH, DAC_ALIGN_12B_R) == HAL_ERROR) {
		Error_Handler();
	}
	while(!dac_stop_flag);
	delete [] dac_buffer;
}

// update buffer. Method called by dac callbacks functions.
void AudioPlayer::update_dac_buffer(uint32_t offset, uint32_t size) {
	uint32_t remaining_samples = MAX_RECORD_LENGTH - cur_data->played_samples;
	uint32_t remaining_size = remaining_samples * DAC_DATA_WIDTH;
	uint32_t read_size = size;
	if (read_size >= remaining_size) {
		read_size = remaining_size;
	}

	// read from flash
	qspi_read((uint8_t*) dac_buffer, cur_data->qspi_address + cur_data->played_samples * DAC_DATA_WIDTH, read_size);
	cur_data->played_samples += (read_size / DAC_DATA_WIDTH);
}

// print recorded data in 16-bit signed format
DACData * AudioPlayer::create_dac_data(DFSDMData *dfsdm_data) {
	// DAC data is written next to dfsdm recorded data
	DACData *dac_data = new DACData(dfsdm_data->qspi_address + dfsdm_data->num_of_samples * DFSDM_DATA_WIDTH);

	// buffers
	dfsdm_buffer = (int32_t *) calloc(CONVERT_BUFFER_LENGTH, DFSDM_DATA_WIDTH);
	dac_buffer = (uint32_t *) calloc(CONVERT_BUFFER_LENGTH, DAC_DATA_WIDTH);

	// navigate through all DFSDM audio memory on flash
	while (dac_data->num_of_samples < dfsdm_data->num_of_samples) {
		uint32_t remaining_samples = dfsdm_data->num_of_samples - dac_data->num_of_samples;
		uint32_t n = remaining_samples < CONVERT_BUFFER_LENGTH ? remaining_samples : CONVERT_BUFFER_LENGTH;

		// read from flash
		qspi_read((uint8_t*)dfsdm_buffer, dfsdm_data->qspi_address + dac_data->num_of_samples*DFSDM_DATA_WIDTH, n * DFSDM_DATA_WIDTH);

		// convert to DAC range
		// must shift right 8 bits since data is on 24 MSB
		for (uint32_t i = 0; i < CONVERT_BUFFER_LENGTH; i++) {
			dac_buffer[i] = (uint32_t)((dfsdm_buffer[i] >> 8) * DFSDM_TO_DAC_SCALE_FACTOR + DFSDM_TO_DAC_BIAS);
		}

		// write converted data to flash
		qspi_write((uint8_t*)dac_buffer, dac_data->qspi_address + dac_data->num_of_samples*DAC_DATA_WIDTH, n*DAC_DATA_WIDTH);

		dac_data->num_of_samples += n;
	}
	// free up memory
	delete [] dfsdm_buffer;
	delete [] dac_buffer;
	return dac_data;
}
