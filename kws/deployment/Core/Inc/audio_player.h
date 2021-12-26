/*
 * audio_player.h
 *
 *  Created on: Dec. 26, 2021
 *      Author: chrismartel
 */

#ifndef INC_AUDIO_PLAYER_H_
#define INC_AUDIO_PLAYER_H_

#include <stdint.h>
#include "main.h"
#include "audio_recorder.h"

#define PLAY_SAMPLING_RATE ((uint32_t ) 16000) //Hz
#define PLAY_BUFFER_LENGTH ((uint32_t)8000)
#define PLAY_HALF_BUFFER_LENGTH ((uint32_t) PLAY_BUFFER_LENGTH / 2)
#define CONVERT_BUFFER_LENGTH ((uint32_t) 8000)

// DAC Parameters
#define DAC_DATA_WIDTH ((uint32_t) 4) // bytes
#define DAC_MAX ((uint32_t)4095) // unsigned 12-bit max
#define DAC_MIN = ((uint32_t)0) // unsigned 12-bit min
#define DAC_AUDIO_QSPI_ADDRESS

// DFSDM --> DAC
#define DFSDM_TO_DAC_SCALE_FACTOR ((float)(DAC_MAX/(DFSDM_MAX + (-DFSDM_MIN))))
#define DFSDM_TO_DAC_BIAS ((float)DFSDM_TO_DAC_SCALE_FACTOR*(-DFSDM_MIN))


class DACData {
public:
	uint32_t num_of_samples;
	uint32_t qspi_address;
	uint32_t played_samples;
	DACData(uint32_t qspi_address);
	~DACData();
};

class AudioPlayer {
private:
	uint32_t *dac_buffer;
	int32_t *dfsdm_buffer;

public:
	AudioPlayer(DAC_HandleTypeDef *hdac);
	~AudioPlayer();
	void play_audio(DACData * data);
	void update_dac_buffer(uint32_t offset, uint32_t size);
	DACData * create_dac_data(DFSDMData *dfsdm_data);
	DACData *cur_data;
	uint8_t dac_stop_flag;
	DAC_HandleTypeDef *hdac;
};


#endif /* INC_AUDIO_PLAYER_H_ */
