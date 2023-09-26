################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Mode_Timer/Src/mode_timer.c 

OBJS += \
./Modules/Mode_Timer/Src/mode_timer.o 

C_DEPS += \
./Modules/Mode_Timer/Src/mode_timer.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/Mode_Timer/Src/%.o Modules/Mode_Timer/Src/%.su Modules/Mode_Timer/Src/%.cyclo: ../Modules/Mode_Timer/Src/%.c Modules/Mode_Timer/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Modules/Calendar/Inc -I../Modules/Mode_Timer/Inc -I../Modules/LED_Debug/Inc -I../Modules/Serial_Communication/Inc -I../Core/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Modules-2f-Mode_Timer-2f-Src

clean-Modules-2f-Mode_Timer-2f-Src:
	-$(RM) ./Modules/Mode_Timer/Src/mode_timer.cyclo ./Modules/Mode_Timer/Src/mode_timer.d ./Modules/Mode_Timer/Src/mode_timer.o ./Modules/Mode_Timer/Src/mode_timer.su

.PHONY: clean-Modules-2f-Mode_Timer-2f-Src

