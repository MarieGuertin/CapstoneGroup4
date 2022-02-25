################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ML-KWS-for-MCU/local_NN/arm_avepool_q7_HWC_nonsquare.c 

C_DEPS += \
./Core/Src/ML-KWS-for-MCU/local_NN/arm_avepool_q7_HWC_nonsquare.d 

OBJS += \
./Core/Src/ML-KWS-for-MCU/local_NN/arm_avepool_q7_HWC_nonsquare.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/ML-KWS-for-MCU/local_NN/%.o: ../Core/Src/ML-KWS-for-MCU/local_NN/%.c Core/Src/ML-KWS-for-MCU/local_NN/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM7 -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Core/Inc -I../Drivers/CMSIS/DSP/Include -I../Drivers/CMSIS/NN/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

