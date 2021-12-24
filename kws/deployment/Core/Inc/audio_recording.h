/*
 * qspi_controller.h
 *
 *  Created on: Nov. 7, 2021
 *      Author: Christian Martel
 */
#ifndef INC_AUDIO_RECORDING_H_
#define INC_AUDIO_RECORDING_H_

// DFSDM
#define DFSDM_BUFFER_LENGTH ((uint32_t)8000)
#define DFSDM_BUFFER_SIZE ((uint32_t) DFSDM_BUFFER_LENGTH * 4)
#define DFSDM_BUFFER_HALFSIZE ((uint32_t)DFSDM_BUFFER_SIZE/2)
#define DFSDM_SAMPLING_RATE ((uint32_t)16000)
#define DFSDM_AUDIO_SIZE ((uint32_t)DFSDM_SAMPLING_RATE*4*1) // sampling rate * size of one sample (bytes) * number of seconds
#define DFSDM_MIN ((float)-8388608)
#define DFSDM_MAX ((float)8388608)
#define DFSDM_START_QSPI_ADDRESS (0xA0000)

// DAC
#define DAC_BUFFER_LENGTH ((uint32_t)8000)
#define DAC_BUFFER_SIZE ((uint32_t )DAC_BUFFER_LENGTH * 4)
#define DAC_MAX ((uint32_t)4095)
#define DAC_MIN = ((uint32_t)0)
#define DAC_START_QSPI_ADDRESS (DFSDM_START_QSPI_ADDRESS+DFSDM_AUDIO_SIZE)
#define DAC_AUDIO_SIZE ((uint32_t)DFSDM_AUDIO_SIZE)

// DFSDM --> DAC
#define DFSDM_TO_DAC_SCALE_FACTOR ((float)0.001)
#define DFSDM_TO_DAC_BIAS ((float)5000.5)

// DFSDM buffer pointers
extern int32_t *dfsdm_buffer_ptr;
extern int32_t *dfsdm_buffer_half_ptr;
extern uint8_t	dfsdm_stop_flag;
extern uint32_t dfsdm_address_checkpoint;

// DAC buffer pointers
extern uint32_t *dac_buffer_ptr;
extern uint32_t *dac_buffer_half_ptr;
extern uint8_t dac_stop_flag;
extern uint32_t dac_address_checkpoint;

// function declarations
void play_audio(DAC_HandleTypeDef *hdac);
void record_audio(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);
void update_dfsdm_buffer(DFSDM_Filter_HandleTypeDef *hdfsdm_filter, int32_t *dfsdm_buffer_ptr, uint32_t size);
void update_dac_buffer(DAC_HandleTypeDef *hdac, uint32_t *dac_buffer_ptr, uint32_t size);
void convert_from_dfsdm_to_dac_range();
void init_qspi();
void print_dfsdm_data();
#endif /* INC_AUDIO_RECORDING_H_ */

