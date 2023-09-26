################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Modules/Serial_Communication/Src/desktop_app_commands.c \
../Modules/Serial_Communication/Src/desktop_app_session.c \
../Modules/Serial_Communication/Src/uart_packet_helpers.c \
../Modules/Serial_Communication/Src/uart_transport_layer.c 

OBJS += \
./Modules/Serial_Communication/Src/desktop_app_commands.o \
./Modules/Serial_Communication/Src/desktop_app_session.o \
./Modules/Serial_Communication/Src/uart_packet_helpers.o \
./Modules/Serial_Communication/Src/uart_transport_layer.o 

C_DEPS += \
./Modules/Serial_Communication/Src/desktop_app_commands.d \
./Modules/Serial_Communication/Src/desktop_app_session.d \
./Modules/Serial_Communication/Src/uart_packet_helpers.d \
./Modules/Serial_Communication/Src/uart_transport_layer.d 


# Each subdirectory must supply rules for building sources it contributes
Modules/Serial_Communication/Src/%.o Modules/Serial_Communication/Src/%.su Modules/Serial_Communication/Src/%.cyclo: ../Modules/Serial_Communication/Src/%.c Modules/Serial_Communication/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Modules/Calendar/Inc -I../Modules/Mode_Timer/Inc -I../Modules/LED_Debug/Inc -I../Modules/Serial_Communication/Inc -I../Core/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Modules-2f-Serial_Communication-2f-Src

clean-Modules-2f-Serial_Communication-2f-Src:
	-$(RM) ./Modules/Serial_Communication/Src/desktop_app_commands.cyclo ./Modules/Serial_Communication/Src/desktop_app_commands.d ./Modules/Serial_Communication/Src/desktop_app_commands.o ./Modules/Serial_Communication/Src/desktop_app_commands.su ./Modules/Serial_Communication/Src/desktop_app_session.cyclo ./Modules/Serial_Communication/Src/desktop_app_session.d ./Modules/Serial_Communication/Src/desktop_app_session.o ./Modules/Serial_Communication/Src/desktop_app_session.su ./Modules/Serial_Communication/Src/uart_packet_helpers.cyclo ./Modules/Serial_Communication/Src/uart_packet_helpers.d ./Modules/Serial_Communication/Src/uart_packet_helpers.o ./Modules/Serial_Communication/Src/uart_packet_helpers.su ./Modules/Serial_Communication/Src/uart_transport_layer.cyclo ./Modules/Serial_Communication/Src/uart_transport_layer.d ./Modules/Serial_Communication/Src/uart_transport_layer.o ./Modules/Serial_Communication/Src/uart_transport_layer.su

.PHONY: clean-Modules-2f-Serial_Communication-2f-Src

