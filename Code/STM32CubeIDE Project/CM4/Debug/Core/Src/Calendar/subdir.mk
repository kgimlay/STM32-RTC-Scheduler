################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Calendar/calendar.c \
../Core/Src/Calendar/rtc_calendar_api.c 

OBJS += \
./Core/Src/Calendar/calendar.o \
./Core/Src/Calendar/rtc_calendar_api.o 

C_DEPS += \
./Core/Src/Calendar/calendar.d \
./Core/Src/Calendar/rtc_calendar_api.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Calendar/%.o Core/Src/Calendar/%.su Core/Src/Calendar/%.cyclo: ../Core/Src/Calendar/%.c Core/Src/Calendar/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core/Inc/Calendar -I../Core/Inc/Serial_Communication -I../Core/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Calendar

clean-Core-2f-Src-2f-Calendar:
	-$(RM) ./Core/Src/Calendar/calendar.cyclo ./Core/Src/Calendar/calendar.d ./Core/Src/Calendar/calendar.o ./Core/Src/Calendar/calendar.su ./Core/Src/Calendar/rtc_calendar_api.cyclo ./Core/Src/Calendar/rtc_calendar_api.d ./Core/Src/Calendar/rtc_calendar_api.o ./Core/Src/Calendar/rtc_calendar_api.su

.PHONY: clean-Core-2f-Src-2f-Calendar

