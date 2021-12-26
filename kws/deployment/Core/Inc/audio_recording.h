/*
 * qspi_controller.h
 *
 *  Created on: Nov. 7, 2021
 *      Author: Christian Martel
 */
#ifndef INC_AUDIO_RECORDING_H_
#define INC_AUDIO_RECORDING_H_

// Audio
#define RECORDING_LENGTH_S ((uint32_t) 1)
#define SAMPLING_RATE ((uint32_t ) 16000)
#define AUDIO_LENGTH ((uint32_t) SAMPLING_RATE * RECORDING_LENGTH_S)

// Buffer
#define DEFAULT_BUFFER_LENGTH ((uint32_t) 8000)

// DFSDM
#define DFSDM_DATA_WIDTH ((uint32_t) 4) // word
#define DFSDM_BUFFER_LENGTH ((uint32_t)8000)
#define DFSDM_AUDIO_SIZE ((uint32_t)SAMPLING_RATE*DFSDM_DATA_WIDTH*RECORDING_LENGTH_S) // sampling rate * size of one sample (bytes) * number of seconds
#define DFSDM_MIN ((float)-8388608)
#define DFSDM_MAX ((float)8388608)
#define DFSDM_AUDIO_QSPI_ADDRESS (0xA0000)

// DAC
#define DAC_DATA_WIDTH ((uint32_t) 4) // word
#define DAC_BUFFER_LENGTH ((uint32_t)8000)
#define DAC_MAX ((uint32_t)4095)
#define DAC_MIN = ((uint32_t)0)
#define DAC_AUDIO_QSPI_ADDRESS (DFSDM_AUDIO_QSPI_ADDRESS+DFSDM_AUDIO_SIZE)
#define DAC_AUDIO_SIZE ((uint32_t)SAMPLING_RATE*DAC_DATA_WIDTH*RECORDING_LENGTH_S)

// DFSDM --> DAC
#define DFSDM_TO_DAC_SCALE_FACTOR ((float)(DAC_MAX/(DFSDM_MAX + (-DFSDM_MIN))))
#define DFSDM_TO_DAC_BIAS ((float)DFSDM_TO_DAC_SCALE_FACTOR*(-DFSDM_MIN))

// DFSDM buffer pointers
extern int32_t *dfsdm_buffer_ptr;
extern int32_t *dfsdm_buffer_half_ptr;
extern uint8_t	dfsdm_stop_flag;
extern uint32_t recorded_size;

// DAC buffer pointers
extern uint32_t *dac_buffer_ptr;
extern uint32_t *dac_buffer_half_ptr;
extern uint8_t dac_stop_flag;
extern uint32_t played_size;

// function declarations
void play_audio(DAC_HandleTypeDef *hdac);
void record_audio(DFSDM_Filter_HandleTypeDef *hdfsdm_filter);
void update_dfsdm_buffer(DFSDM_Filter_HandleTypeDef *hdfsdm_filter, int32_t *dfsdm_buffer_ptr, uint32_t size);
void update_dac_buffer(DAC_HandleTypeDef *hdac, uint32_t *dac_buffer_ptr, uint32_t size);
void convert_from_dfsdm_to_dac_range();
void init_qspi();
void print_dfsdm_data();
#endif /* INC_AUDIO_RECORDING_H_ */
