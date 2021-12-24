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

void compute_mfcc_coefficients(q7_t *mfcc_out, uint32_t audio_start_address, uint32_t num_frames, uint32_t frame_len, uint32_t frame_shift, uint32_t mfcc_num_features, uint32_t mfcc_num_dec_bits) {

	MFCC *mfcc = new MFCC(mfcc_num_features, frame_len, mfcc_num_dec_bits);

	int32_t *audio_buffer_int32 = (int32_t*) calloc(frame_len, sizeof(int32_t));
	int16_t *mfcc_in = (int16_t*) calloc(frame_len, sizeof(int16_t));
//	int32_t audio_buffer_int32[frame_len];
//	int16_t mfcc_in[frame_len];

	uint32_t cur_qspi_address = audio_start_address;
	for (uint32_t i=0; i < num_frames; i ++) {
		qspi_read((uint8_t*)audio_buffer_int32, cur_qspi_address, frame_len * sizeof(int32_t));
		for (uint32_t j=0; j < frame_len; j++) {
			mfcc_in[j] = (int16_t)(audio_buffer_int32[j] >> 8);
		}
		mfcc->mfcc_compute(mfcc_in, mfcc_out + i * mfcc_num_features);

		cur_qspi_address += frame_shift * sizeof(int32_t);
	}
	mfcc->~MFCC();
	free(mfcc_in);
	free(audio_buffer_int32);
}

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
