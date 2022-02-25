################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/ML-KWS-for-MCU/NN/DS_CNN/ds_cnn.cpp 

OBJS += \
./Core/Src/ML-KWS-for-MCU/NN/DS_CNN/ds_cnn.o 

CPP_DEPS += \
./Core/Src/ML-KWS-for-MCU/NN/DS_CNN/ds_cnn.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/ML-KWS-for-MCU/NN/DS_CNN/%.o: ../Core/Src/ML-KWS-for-MCU/NN/DS_CNN/%.cpp Core/Src/ML-KWS-for-MCU/NN/DS_CNN/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DDEBUG -DARM_MATH_CM7 -DUSE_HAL_DRIVER -DSTM32F769xx -c -I../Core/Inc -I../Drivers/CMSIS/DSP/Include -I../Drivers/CMSIS/NN/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

