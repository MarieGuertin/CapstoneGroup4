################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Tests/UnitTest/TestCases/test_arm_depthwise_conv_s8/TestRunner/test_arm_depthwise_conv_s8_runner.c 

C_DEPS += \
./Drivers/CMSIS/NN/Tests/UnitTest/TestCases/test_arm_depthwise_conv_s8/TestRunner/test_arm_depthwise_conv_s8_runner.d 

OBJS += \
./Drivers/CMSIS/NN/Tests/UnitTest/TestCases/test_arm_depthwise_conv_s8/TestRunner/test_arm_depthwise_conv_s8_runner.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Tests/UnitTest/TestCases/test_arm_depthwise_conv_s8/TestRunner/%.o: ../Drivers/CMSIS/NN/Tests/UnitTest/TestCases/test_arm_depthwise_conv_s8/TestRunner/%.c Drivers/CMSIS/NN/Tests/UnitTest/TestCases/test_arm_depthwise_conv_s8/TestRunner/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

