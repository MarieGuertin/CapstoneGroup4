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
