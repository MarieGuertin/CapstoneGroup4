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
#define RECORDING_WINDOW_SIZE ((uint32_t) (RECORDING_WINDOW_LENGTH + 1)*FRAME_SHIFT*WAVE_DATA_WIDTH)
#define NUM_OUTPUT_CLASSES ((uint32_t) 12)
#define NUM_PREDICTIONS ((uint32_t) (NUM_FRAMES / RECORDING_WINDOW_LENGTH))
#define SILENCE_INDEX ((uint32_t) 0)
#define UNKNOWN_INDEX ((uint32_t) 1)


// TODO: tweak these parameters to find optimal combination
#define RECORDING_WINDOW_LENGTH ((uint32_t)5)
#define DETECTION_THRESHOLD ((uint32_t) 55)
#define AVERAGE_WINDOW_LENGTH ((uint32_t) 2)

void compute_mfcc_coefficients(q7_t *mfcc_out, uint32_t audio_start_address, uint32_t num_frames, uint32_t frame_len, uint32_t frame_shift, uint32_t mfcc_num_features, uint32_t mfcc_num_dec_bits);
void print_mfcc(q7_t *mfcc_out);
void average_predictions(q7_t *average, q7_t* predictions, uint32_t num_predictions, uint32_t num_output_classes);
uint32_t get_top_class(q7_t* nn_output);


#endif /* INC_KWS_H_ */
