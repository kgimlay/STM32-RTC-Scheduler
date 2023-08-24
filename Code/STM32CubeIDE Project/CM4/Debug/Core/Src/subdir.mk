################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/calendar.c \
../Core/Src/com_datalink_layer.c \
../Core/Src/com_session_layer.c \
../Core/Src/com_transport_layer.c \
../Core/Src/led_debug.c \
../Core/Src/main.c \
../Core/Src/rtc_alarm_control.c \
../Core/Src/stm32wlxx_hal_msp.c \
../Core/Src/stm32wlxx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/uart_basic_com.c 

OBJS += \
./Core/Src/calendar.o \
./Core/Src/com_datalink_layer.o \
./Core/Src/com_session_layer.o \
./Core/Src/com_transport_layer.o \
./Core/Src/led_debug.o \
./Core/Src/main.o \
./Core/Src/rtc_alarm_control.o \
./Core/Src/stm32wlxx_hal_msp.o \
./Core/Src/stm32wlxx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/uart_basic_com.o 

C_DEPS += \
./Core/Src/calendar.d \
./Core/Src/com_datalink_layer.d \
./Core/Src/com_session_layer.d \
./Core/Src/com_transport_layer.d \
./Core/Src/led_debug.d \
./Core/Src/main.d \
./Core/Src/rtc_alarm_control.d \
./Core/Src/stm32wlxx_hal_msp.d \
./Core/Src/stm32wlxx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/uart_basic_com.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/calendar.cyclo ./Core/Src/calendar.d ./Core/Src/calendar.o ./Core/Src/calendar.su ./Core/Src/com_datalink_layer.cyclo ./Core/Src/com_datalink_layer.d ./Core/Src/com_datalink_layer.o ./Core/Src/com_datalink_layer.su ./Core/Src/com_session_layer.cyclo ./Core/Src/com_session_layer.d ./Core/Src/com_session_layer.o ./Core/Src/com_session_layer.su ./Core/Src/com_transport_layer.cyclo ./Core/Src/com_transport_layer.d ./Core/Src/com_transport_layer.o ./Core/Src/com_transport_layer.su ./Core/Src/led_debug.cyclo ./Core/Src/led_debug.d ./Core/Src/led_debug.o ./Core/Src/led_debug.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/rtc_alarm_control.cyclo ./Core/Src/rtc_alarm_control.d ./Core/Src/rtc_alarm_control.o ./Core/Src/rtc_alarm_control.su ./Core/Src/stm32wlxx_hal_msp.cyclo ./Core/Src/stm32wlxx_hal_msp.d ./Core/Src/stm32wlxx_hal_msp.o ./Core/Src/stm32wlxx_hal_msp.su ./Core/Src/stm32wlxx_it.cyclo ./Core/Src/stm32wlxx_it.d ./Core/Src/stm32wlxx_it.o ./Core/Src/stm32wlxx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/uart_basic_com.cyclo ./Core/Src/uart_basic_com.d ./Core/Src/uart_basic_com.o ./Core/Src/uart_basic_com.su

.PHONY: clean-Core-2f-Src

