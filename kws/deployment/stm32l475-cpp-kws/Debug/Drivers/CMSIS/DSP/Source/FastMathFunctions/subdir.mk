################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctionsF16.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_divide_q15.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_divide_q31.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f16.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f64.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vinverse_f16.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f16.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f64.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_q15.c \
../Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_q31.c 

C_DEPS += \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctionsF16.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_divide_q15.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_divide_q31.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f16.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f64.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vinverse_f16.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f16.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f64.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_q15.d \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_q31.d 

OBJS += \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/FastMathFunctionsF16.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_f32.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q15.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_cos_q31.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_divide_q15.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_divide_q31.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_f32.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q15.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sin_q31.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q15.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_sqrt_q31.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f16.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f32.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vexp_f64.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vinverse_f16.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f16.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f32.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_f64.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_q15.o \
./Drivers/CMSIS/DSP/Source/FastMathFunctions/arm_vlog_q31.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Source/FastMathFunctions/%.o: ../Drivers/CMSIS/DSP/Source/FastMathFunctions/%.c Drivers/CMSIS/DSP/Source/FastMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/CMSIS/DSP/Include -I../Drivers/CMSIS/NN/Include -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

