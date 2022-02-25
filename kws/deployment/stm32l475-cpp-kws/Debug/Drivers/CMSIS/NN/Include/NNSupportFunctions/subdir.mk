################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_accumulate_q7_to_q15.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_add_q7.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_kernel_s16.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mult_q15.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mult_q7.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nntables.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_no_shift.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_reordered_with_offset.c \
../Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_with_offset.c 

C_DEPS += \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_accumulate_q7_to_q15.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_add_q7.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_kernel_s16.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mult_q15.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mult_q7.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nntables.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_no_shift.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_reordered_with_offset.d \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_with_offset.d 

OBJS += \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_accumulate_q7_to_q15.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_add_q7.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_padded_s8.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_depthwise_conv_nt_t_s8.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_core_1x_s8.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_core_4x_s8.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mul_kernel_s16.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mat_mult_nt_t_s8.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mult_q15.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_mult_q7.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_s16.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_s8.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nn_vec_mat_mult_t_svdf_s8.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_nntables.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_no_shift.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_reordered_with_offset.o \
./Drivers/CMSIS/NN/Include/NNSupportFunctions/arm_q7_to_q15_with_offset.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Include/NNSupportFunctions/%.o: ../Drivers/CMSIS/NN/Include/NNSupportFunctions/%.c Drivers/CMSIS/NN/Include/NNSupportFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I"C:/Users/Windows/STM32CubeIDE/workspace_1.7.0/stm32l475-cpp-kws/Drivers/CMSIS/NN/Include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

