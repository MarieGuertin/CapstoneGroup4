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
#include <cstring>

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
void AudioPlayer::play_audio(WaveData * data) {
	cur_data = data;
	played_samples = 0;
	converted_samples = 0;

	conversion_buffer = (uint32_t *) calloc(PLAY_BUFFER_LENGTH, DAC_DATA_WIDTH);
	dac_buffer = (uint32_t *) calloc(PLAY_BUFFER_LENGTH, DAC_DATA_WIDTH);


	// initial full read
	update_dac_buffer(0, PLAY_BUFFER_LENGTH);

	// Start DAC in circular mode
	dac_stop_flag = 0;
	if (HAL_DAC_Start_DMA(hdac, DAC_CHANNEL_1, dac_buffer, PLAY_BUFFER_LENGTH, DAC_ALIGN_12B_R) == HAL_ERROR) {
		Error_Handler();
	}
	while(!dac_stop_flag);
	delete [] dac_buffer;
	delete [] conversion_buffer;
}

// update buffer. Method called by dac callbacks functions.
void AudioPlayer::update_dac_buffer(uint32_t offset, uint32_t data_length) {
	uint32_t remaining_samples = MAX_RECORD_LENGTH - converted_samples;
	uint32_t read_length = data_length;
	if (read_length >= remaining_samples) {
		read_length = remaining_samples;
	}
	if (read_length > 0) {
		qspi_read((uint8_t*)conversion_buffer, cur_data->qspi_address + converted_samples*DFSDM_DATA_WIDTH, read_length * DFSDM_DATA_WIDTH);
		for (uint32_t i = 0; i < read_length; i++) {
			conversion_buffer[i] = (uint32_t)(((int32_t)conversion_buffer[i] >> 8) * DFSDM_TO_DAC_SCALE_FACTOR + DFSDM_TO_DAC_BIAS);
		}
		converted_samples += read_length;
		memcpy(dac_buffer + offset, conversion_buffer, read_length * DAC_DATA_WIDTH);
	}
}


