################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/kevinimlay/Developer/Graduate\ Research/STM32\ RTC\ Scheduler/Code/STM32CubeIDE\ Project/STM32\ RTC\ Scheduler/Common/System/system_stm32wlxx.c 

OBJS += \
./Common/System/system_stm32wlxx.o 

C_DEPS += \
./Common/System/system_stm32wlxx.d 


# Each subdirectory must supply rules for building sources it contributes
Common/System/system_stm32wlxx.o: /Users/kevinimlay/Developer/Graduate\ Research/STM32\ RTC\ Scheduler/Code/STM32CubeIDE\ Project/STM32\ RTC\ Scheduler/Common/System/system_stm32wlxx.c Common/System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DCORE_CM0PLUS -DSTM32WL55xx -DUSE_HAL_DRIVER -c -I../Core/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Common-2f-System

clean-Common-2f-System:
	-$(RM) ./Common/System/system_stm32wlxx.cyclo ./Common/System/system_stm32wlxx.d ./Common/System/system_stm32wlxx.o ./Common/System/system_stm32wlxx.su

.PHONY: clean-Common-2f-System

