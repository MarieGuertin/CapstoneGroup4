################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1_x_n_s8.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1x1_s8_fast.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_basic.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_fast.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_basic.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_fast.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_fast_s16.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_s16.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_s8.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_wrapper_s16.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_wrapper_s8.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_s8.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_s8_opt.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16_reordered.c \
../Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_s8.c 

C_DEPS += \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1_x_n_s8.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1x1_s8_fast.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_basic.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_fast.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_basic.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_fast.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_fast_s16.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_s16.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_s8.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_wrapper_s16.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_wrapper_s8.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_s8.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_s8_opt.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16_reordered.d \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_s8.d 

OBJS += \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1_x_n_s8.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1x1_HWC_q7_fast_nonsquare.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_1x1_s8_fast.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_basic.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_fast.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q15_fast_nonsquare.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_RGB.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_basic.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_basic_nonsquare.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_fast.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_HWC_q7_fast_nonsquare.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_fast_s16.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_s16.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_s8.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_wrapper_s16.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_convolve_wrapper_s8.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_3x3_s8.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_s8.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_s8_opt.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_u8_basic_ver1.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_conv_wrapper_s8.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_depthwise_separable_conv_HWC_q7_nonsquare.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_depthwise_conv_s8_core.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_q7_q15_reordered.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_kernel_s8_s16_reordered.o \
./Drivers/CMSIS/NN/Include/ConvolutionFunctions/arm_nn_mat_mult_s8.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Include/ConvolutionFunctions/%.o: ../Drivers/CMSIS/NN/Include/ConvolutionFunctions/%.c Drivers/CMSIS/NN/Include/ConvolutionFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I"C:/Users/Windows/STM32CubeIDE/workspace_1.7.0/stm32l475-cpp-kws/Drivers/CMSIS/NN/Include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

