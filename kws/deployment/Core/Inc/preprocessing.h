/*
 * mfcc.h
 *
 *  Created on: Dec. 22, 2021
 *      Author: chrismartel
 */

#ifndef INC_PREPROCESSING_H_
#define INC_PREPROCESSING_H_

#define SAMP_FREQ 16000
#define MFCC_DEC_BITS 1
#define FRAME_SHIFT_MS 20
#define FRAME_SHIFT ((int16_t)(SAMP_FREQ * 0.001 * FRAME_SHIFT_MS))
#define NUM_FRAMES 49
#define NUM_MFCC_COEFFS 10
#define MFCC_BUFFER_SIZE (NUM_FRAMES*NUM_MFCC_COEFFS)
#define FRAME_LEN_MS 40
#define FRAME_LEN ((int16_t)(SAMP_FREQ * 0.001 * FRAME_LEN_MS))


void compute_mfcc_coefficients(q7_t *mfcc_out, uint32_t audio_start_address, uint32_t num_frames, uint32_t mfcc_num_features, uint32_t frame_len, uint32_t mfcc_num_dec_bits);
#endif /* INC_PREPROCESSING_H_ */