
/**
  ******************************************************************************
  * @file    cnn_model_data.h
  * @author  AST Embedded Analytics Research Platform
  * @date    Tue Mar 22 17:09:46 2022
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef CNN_MODEL_DATA_H
#define CNN_MODEL_DATA_H
#pragma once

#include "cnn_model_config.h"
#include "ai_platform.h"

#define AI_CNN_MODEL_DATA_CONFIG               (NULL)


#define AI_CNN_MODEL_DATA_ACTIVATIONS_SIZES \
  { 31960, }
#define AI_CNN_MODEL_DATA_ACTIVATIONS_SIZE     (31960)
#define AI_CNN_MODEL_DATA_ACTIVATIONS_COUNT    (1)
#define AI_CNN_MODEL_DATA_ACTIVATION_1_SIZE    (31960)



#define AI_CNN_MODEL_DATA_WEIGHTS_SIZES \
  { 277416, }
#define AI_CNN_MODEL_DATA_WEIGHTS_SIZE         (277416)
#define AI_CNN_MODEL_DATA_WEIGHTS_COUNT        (1)
#define AI_CNN_MODEL_DATA_WEIGHT_1_SIZE        (277416)



AI_DEPRECATED
#define AI_CNN_MODEL_DATA_ACTIVATIONS(ptr_)  \
  ai_cnn_model_data_activations_buffer_get(AI_HANDLE_PTR(ptr_))

AI_DEPRECATED
#define AI_CNN_MODEL_DATA_WEIGHTS(ptr_)  \
  ai_cnn_model_data_weights_buffer_get(AI_HANDLE_PTR(ptr_))


AI_API_DECLARE_BEGIN

/*!
 * @brief Get network activations buffer initialized struct.
 * @ingroup cnn_model_data
 * @param[in] ptr a pointer to the activations array storage area
 * @return an ai_buffer initialized struct
 */
AI_DEPRECATED
AI_API_ENTRY
ai_buffer ai_cnn_model_data_activations_buffer_get(const ai_handle ptr);

/*!
 * @brief Get network weights buffer initialized struct.
 * @ingroup cnn_model_data
 * @param[in] ptr a pointer to the weights array storage area
 * @return an ai_buffer initialized struct
 */
AI_DEPRECATED
AI_API_ENTRY
ai_buffer ai_cnn_model_data_weights_buffer_get(const ai_handle ptr);

/*!
 * @brief Get network weights array pointer as a handle ptr.
 * @ingroup cnn_model_data
 * @return a ai_handle pointer to the weights array
 */
AI_DEPRECATED
AI_API_ENTRY
ai_handle ai_cnn_model_data_weights_get(void);


/*!
 * @brief Get network params configuration data structure.
 * @ingroup cnn_model_data
 * @return true if a valid configuration is present, false otherwise
 */
AI_API_ENTRY
ai_bool ai_cnn_model_data_params_get(ai_network_params* params);


AI_API_DECLARE_END

#endif /* CNN_MODEL_DATA_H */

