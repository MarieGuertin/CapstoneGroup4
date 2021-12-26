/*
 * kws.h
 *
 *  Created on: Dec. 24, 2021
 *      Author: chrismartel
 */

#ifndef INC_KWS_H_
#define INC_KWS_H_

#include "arm_nnfunctions.h"

#define SAMP_FREQ 16000
#define MFCC_DEC_BITS 1
#define FRAME_SHIFT_MS 20
#define FRAME_SHIFT ((int16_t)(SAMP_FREQ * 0.001 * FRAME_SHIFT_MS))
#define NUM_FRAMES 49
#define NUM_MFCC_COEFFS 10
#define MFCC_BUFFER_SIZE (NUM_FRAMES*NUM_MFCC_COEFFS)
#define FRAME_LEN_MS 40
#define FRAME_LEN ((int16_t)(SAMP_FREQ * 0.001 * FRAME_LEN_MS))
#define RECORDING_WINDOW_LENGTH ((uint32_t)3)
#define NUM_PREDICTIONS ((uint32_t) NUM_FRAMES - (RECORDING_WINDOW_LENGTH-1))
#define NUM_OUTPUT_CLASSES ((uint32_t) 12)

void compute_mfcc_coefficients(q7_t *mfcc_out, uint32_t audio_start_address, uint32_t num_frames, uint32_t frame_len, uint32_t frame_shift, uint32_t mfcc_num_features, uint32_t mfcc_num_dec_bits);
uint32_t get_top_class(q7_t* nn_output);


#endif /* INC_KWS_H_ */
