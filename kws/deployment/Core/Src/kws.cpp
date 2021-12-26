/*
 * kws.cpp
 *
 *  Created on: Dec. 24, 2021
 *      Author: chrismartel
 */

#include "kws.h"
#include "main.h"
#include "ML-KWS-for-MCU/MFCC/mfcc.h"
#include "qspi_handler.h"
#include "audio_recording.h"

uint32_t num_output_classes = 12;


uint32_t get_top_class(q7_t* nn_output) {
	uint32_t max_index = 0;
	int32_t max_value = -128;
	for (uint32_t i = 0; i < num_output_classes; i++) {
		if (nn_output[i] > max_value) {
			max_index = i;
			max_value = nn_output[i];
		}
	}
	return max_index;
}
