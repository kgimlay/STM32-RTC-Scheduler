################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Serial_Communication/com_datalink_layer.c \
../Core/Src/Serial_Communication/com_presentation_layer.c \
../Core/Src/Serial_Communication/com_session_layer.c \
../Core/Src/Serial_Communication/com_transport_layer.c 

OBJS += \
./Core/Src/Serial_Communication/com_datalink_layer.o \
./Core/Src/Serial_Communication/com_presentation_layer.o \
./Core/Src/Serial_Communication/com_session_layer.o \
./Core/Src/Serial_Communication/com_transport_layer.o 

C_DEPS += \
./Core/Src/Serial_Communication/com_datalink_layer.d \
./Core/Src/Serial_Communication/com_presentation_layer.d \
./Core/Src/Serial_Communication/com_session_layer.d \
./Core/Src/Serial_Communication/com_transport_layer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Serial_Communication/%.o Core/Src/Serial_Communication/%.su Core/Src/Serial_Communication/%.cyclo: ../Core/Src/Serial_Communication/%.c Core/Src/Serial_Communication/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WL55xx -c -I../Core/Inc/Calendar -I../Core/Inc/Serial_Communication -I../Core/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Serial_Communication

clean-Core-2f-Src-2f-Serial_Communication:
	-$(RM) ./Core/Src/Serial_Communication/com_datalink_layer.cyclo ./Core/Src/Serial_Communication/com_datalink_layer.d ./Core/Src/Serial_Communication/com_datalink_layer.o ./Core/Src/Serial_Communication/com_datalink_layer.su ./Core/Src/Serial_Communication/com_presentation_layer.cyclo ./Core/Src/Serial_Communication/com_presentation_layer.d ./Core/Src/Serial_Communication/com_presentation_layer.o ./Core/Src/Serial_Communication/com_presentation_layer.su ./Core/Src/Serial_Communication/com_session_layer.cyclo ./Core/Src/Serial_Communication/com_session_layer.d ./Core/Src/Serial_Communication/com_session_layer.o ./Core/Src/Serial_Communication/com_session_layer.su ./Core/Src/Serial_Communication/com_transport_layer.cyclo ./Core/Src/Serial_Communication/com_transport_layer.d ./Core/Src/Serial_Communication/com_transport_layer.o ./Core/Src/Serial_Communication/com_transport_layer.su

.PHONY: clean-Core-2f-Src-2f-Serial_Communication

