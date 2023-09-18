################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Calendar/Src/calendar.c \
../Modules/Calendar/Src/rtc_calendar_control.c 

OBJS += \
./Modules/Calendar/Src/calendar.o \
./Modules/Calendar/Src/rtc_calendar_control.o 

C_DEPS += \
./Modules/Calendar/Src/calendar.d \
./Modules/Calendar/Src/rtc_calendar_control.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/Calendar/Src/%.o Modules/Calendar/Src/%.su Modules/Calendar/Src/%.cyclo: ../Modules/Calendar/Src/%.c Modules/Calendar/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Modules/Calendar/Inc -I../Modules/LED_Debug/Inc -I../Modules/Serial_Communication/Inc -I../Core/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Modules-2f-Calendar-2f-Src

clean-Modules-2f-Calendar-2f-Src:
	-$(RM) ./Modules/Calendar/Src/calendar.cyclo ./Modules/Calendar/Src/calendar.d ./Modules/Calendar/Src/calendar.o ./Modules/Calendar/Src/calendar.su ./Modules/Calendar/Src/rtc_calendar_control.cyclo ./Modules/Calendar/Src/rtc_calendar_control.d ./Modules/Calendar/Src/rtc_calendar_control.o ./Modules/Calendar/Src/rtc_calendar_control.su

.PHONY: clean-Modules-2f-Calendar-2f-Src

