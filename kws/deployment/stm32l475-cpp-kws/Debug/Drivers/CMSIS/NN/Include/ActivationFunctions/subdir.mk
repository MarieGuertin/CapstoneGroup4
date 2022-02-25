################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Include/ActivationFunctions/arm_nn_activations_q15.c \
../Drivers/CMSIS/NN/Include/ActivationFunctions/arm_nn_activations_q7.c \
../Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu6_s8.c \
../Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu_q15.c \
../Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu_q7.c 

C_DEPS += \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_nn_activations_q15.d \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_nn_activations_q7.d \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu6_s8.d \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu_q15.d \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu_q7.d 

OBJS += \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_nn_activations_q15.o \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_nn_activations_q7.o \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu6_s8.o \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu_q15.o \
./Drivers/CMSIS/NN/Include/ActivationFunctions/arm_relu_q7.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Include/ActivationFunctions/%.o: ../Drivers/CMSIS/NN/Include/ActivationFunctions/%.c Drivers/CMSIS/NN/Include/ActivationFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I"C:/Users/Windows/STM32CubeIDE/workspace_1.7.0/stm32l475-cpp-kws/Drivers/CMSIS/NN/Include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

