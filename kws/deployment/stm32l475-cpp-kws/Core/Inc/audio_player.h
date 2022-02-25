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

// DAC Parameters
#define DAC_DATA_WIDTH ((uint32_t) 2) // bytes
#define DAC_MAX ((uint32_t)4095) // unsigned 12-bit max
#define DAC_MIN = ((uint32_t)0) // unsigned 12-bit min

// DFSDM --> DAC
#define DFSDM_TO_DAC_SCALE_FACTOR ((float)(DAC_MAX/(DFSDM_MAX + (-DFSDM_MIN))))
#define DFSDM_TO_DAC_BIAS ((float)DFSDM_TO_DAC_SCALE_FACTOR*(-DFSDM_MIN))

// WAVE --> DAC
#define WAVE_TO_DAC_SCALE_FACTOR ((float)(DAC_MAX/(WAVE_MAX + (-WAVE_MIN))))
#define WAVE_TO_DAC_BIAS ((float)WAVE_TO_DAC_SCALE_FACTOR*(-WAVE_MIN))

class AudioPlayer {
private:
	uint16_t *dac_buffer;
	uint16_t *conversion_buffer;

public:
	AudioPlayer(DAC_HandleTypeDef *hdac);
	~AudioPlayer();
	void play_audio(WaveData * data);
	void update_dac_buffer(uint32_t offset, uint32_t data_length);
	WaveData *cur_data;
	uint8_t dac_stop_flag;
	DAC_HandleTypeDef *hdac;
	uint32_t played_samples;
	uint32_t converted_samples;
};


#endif /* INC_AUDIO_PLAYER_H_ */
