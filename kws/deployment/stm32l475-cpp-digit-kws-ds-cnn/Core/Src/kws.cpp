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
#include <stdio.h>


uint32_t num_output_classes = 12;


uint32_t get_top_class(float* nn_output) {
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

void average_predictions(float *average_out, float* average_window_head, uint32_t num_predictions, uint32_t num_output_classes) {
	float *sum = new float[num_output_classes];
	for (uint32_t i = 0; i < num_output_classes; i++) {
		sum[i] = 0;
		for (uint32_t j = 0; j < num_predictions; j++) {
			sum[i] += average_window_head[j * num_output_classes + i];
		}
		average_out[i] = sum[i] / (float) num_predictions;
	}
	delete [] sum;
}

void print_mfcc(q7_t *mfcc_out) {
	print("\nMFCC:\r\n");
	for (uint32_t i=0; i < MFCC_BUFFER_SIZE; i++) {
		char mfcc_coeff_str[10];
		sprintf(mfcc_coeff_str, "%d", (int8_t)(mfcc_out[i]));
		print(mfcc_coeff_str);
		if ((i+1) % NUM_MFCC_COEFFS == 0)
			print("\r\n");
		else
			print(",");
	}
	print("\r\n");
}
