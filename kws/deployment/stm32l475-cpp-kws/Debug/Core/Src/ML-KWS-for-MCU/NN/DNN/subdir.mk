################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/ML-KWS-for-MCU/NN/DNN/dnn.cpp 

OBJS += \
./Core/Src/ML-KWS-for-MCU/NN/DNN/dnn.o 

CPP_DEPS += \
./Core/Src/ML-KWS-for-MCU/NN/DNN/dnn.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/ML-KWS-for-MCU/NN/DNN/%.o: ../Core/Src/ML-KWS-for-MCU/NN/DNN/%.cpp Core/Src/ML-KWS-for-MCU/NN/DNN/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/CMSIS/NN/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

