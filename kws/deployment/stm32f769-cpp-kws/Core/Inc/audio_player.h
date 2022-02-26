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
#define SAI_DATA_WIDTH ((uint32_t) 2) // bytes
#define SAI_MAX ((uint32_t)4095) // unsigned 12-bit max
#define SAI_MIN = ((uint32_t)0) // unsigned 12-bit min

// DFSDM --> SAI
#define DFSDM_TO_SAI_SCALE_FACTOR ((float)(SAI_MAX/(DFSDM_MAX + (-DFSDM_MIN))))
#define DFSDM_TO_SAI_BIAS ((float)DFSDM_TO_SAI_SCALE_FACTOR*(-DFSDM_MIN))

// WAVE --> SAI
#define WAVE_TO_SAI_SCALE_FACTOR ((float)(SAI_MAX/(WAVE_MAX + (-WAVE_MIN))))
#define WAVE_TO_SAI_BIAS ((float)WAVE_TO_SAI_SCALE_FACTOR*(-WAVE_MIN))

class AudioPlayer {
private:
	uint16_t *sai_buffer;
	uint16_t *conversion_buffer;

public:
	AudioPlayer(SAI_HandleTypeDef *hsai);
	~AudioPlayer();
	void play_audio(WaveData * data);
	void update_sai_buffer(uint32_t offset, uint32_t data_length);
	WaveData *cur_data;
	uint8_t sai_stop_flag;
	SAI_HandleTypeDef *hsai;
	uint32_t played_samples;
	uint32_t converted_samples;
};


#endif /* INC_AUDIO_PLAYER_H_ */
