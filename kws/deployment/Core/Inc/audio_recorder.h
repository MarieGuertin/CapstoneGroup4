/*
 * audio_recorder.h
 *
 *  Created on: Dec. 26, 2021
 *      Author: chrismartel
 */

#ifndef INC_AUDIO_RECORDER_H_
#define INC_AUDIO_RECORDER_H_

#include <stdint.h>
#include "main.h"

#define RECORD_SAMPLING_RATE ((uint32_t ) 16000) //Hz
#define RECORD_BUFFER_LENGTH ((uint32_t)8000)
#define RECORD_HALF_BUFFER_LENGTH ((uint32_t) RECORD_BUFFER_LENGTH / 2)
#define PRINT_BUFFER_LENGTH ((uint32_t)8000)

#define MAX_RECORD_LENGTH_S ((uint32_t) 1)
#define MAX_RECORD_LENGTH ((uint32_t) RECORD_SAMPLING_RATE * MAX_RECORD_LENGTH_S)

// DFSDM Parameters
#define DFSDM_DATA_WIDTH ((uint32_t) 4) // bytes
#define DFSDM_MIN ((float)-8388608) // signed 24-bit int min
#define DFSDM_MAX ((float)8388608) // signed 24-bit int max

#define WAVE_DATA_QSPI_ADDRESS (0xA0000)

class WaveData {
public:
	uint32_t num_of_samples;
	uint32_t qspi_address;
	WaveData(uint32_t qspi_address);
	~WaveData();
};

class AudioRecorder {
private:
	int32_t *dfsdm_buffer;

public:
	AudioRecorder(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);
	~AudioRecorder();
	WaveData * record_audio(uint32_t qspi_address);
	void update_dfsdm_buffer(uint32_t offset, uint32_t size);
	void print_data(WaveData *data);
	WaveData *cur_data;
	uint8_t dfsdm_stop_flag;
	DFSDM_Filter_HandleTypeDef *hdfsdm_filter;
};




#endif /* INC_AUDIO_RECORDER_H_ */
