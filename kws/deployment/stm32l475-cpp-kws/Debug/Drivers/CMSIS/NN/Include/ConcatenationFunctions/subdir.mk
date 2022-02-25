################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_w.c \
../Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_x.c \
../Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_y.c \
../Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_z.c 

C_DEPS += \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_w.d \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_x.d \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_y.d \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_z.d 

OBJS += \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_w.o \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_x.o \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_y.o \
./Drivers/CMSIS/NN/Include/ConcatenationFunctions/arm_concatenation_s8_z.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Include/ConcatenationFunctions/%.o: ../Drivers/CMSIS/NN/Include/ConcatenationFunctions/%.c Drivers/CMSIS/NN/Include/ConcatenationFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I"C:/Users/Windows/STM32CubeIDE/workspace_1.7.0/stm32l475-cpp-kws/Drivers/CMSIS/NN/Include" -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

