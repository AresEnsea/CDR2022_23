################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../GUI\ files/font12.c \
../GUI\ files/font16.c \
../GUI\ files/font20.c \
../GUI\ files/font24.c \
../GUI\ files/font8.c \
../GUI\ files/ft5336.c \
../GUI\ files/stm32746g_discovery.c \
../GUI\ files/stm32746g_discovery_lcd.c \
../GUI\ files/stm32746g_discovery_sdram.c \
../GUI\ files/stm32746g_discovery_ts.c 

OBJS += \
./GUI\ files/font12.o \
./GUI\ files/font16.o \
./GUI\ files/font20.o \
./GUI\ files/font24.o \
./GUI\ files/font8.o \
./GUI\ files/ft5336.o \
./GUI\ files/stm32746g_discovery.o \
./GUI\ files/stm32746g_discovery_lcd.o \
./GUI\ files/stm32746g_discovery_sdram.o \
./GUI\ files/stm32746g_discovery_ts.o 

C_DEPS += \
./GUI\ files/font12.d \
./GUI\ files/font16.d \
./GUI\ files/font20.d \
./GUI\ files/font24.d \
./GUI\ files/font8.d \
./GUI\ files/ft5336.d \
./GUI\ files/stm32746g_discovery.d \
./GUI\ files/stm32746g_discovery_lcd.d \
./GUI\ files/stm32746g_discovery_sdram.d \
./GUI\ files/stm32746g_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
GUI\ files/font12.o: ../GUI\ files/font12.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/font12.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/font16.o: ../GUI\ files/font16.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/font16.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/font20.o: ../GUI\ files/font20.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/font20.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/font24.o: ../GUI\ files/font24.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/font24.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/font8.o: ../GUI\ files/font8.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/font8.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/ft5336.o: ../GUI\ files/ft5336.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/ft5336.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/stm32746g_discovery.o: ../GUI\ files/stm32746g_discovery.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/stm32746g_discovery.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/stm32746g_discovery_lcd.o: ../GUI\ files/stm32746g_discovery_lcd.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/stm32746g_discovery_lcd.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/stm32746g_discovery_sdram.o: ../GUI\ files/stm32746g_discovery_sdram.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/stm32746g_discovery_sdram.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
GUI\ files/stm32746g_discovery_ts.o: ../GUI\ files/stm32746g_discovery_ts.c GUI\ files/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F746xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I"../GUI files" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"GUI files/stm32746g_discovery_ts.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-GUI-20-files

clean-GUI-20-files:
	-$(RM) ./GUI\ files/font12.d ./GUI\ files/font12.o ./GUI\ files/font12.su ./GUI\ files/font16.d ./GUI\ files/font16.o ./GUI\ files/font16.su ./GUI\ files/font20.d ./GUI\ files/font20.o ./GUI\ files/font20.su ./GUI\ files/font24.d ./GUI\ files/font24.o ./GUI\ files/font24.su ./GUI\ files/font8.d ./GUI\ files/font8.o ./GUI\ files/font8.su ./GUI\ files/ft5336.d ./GUI\ files/ft5336.o ./GUI\ files/ft5336.su ./GUI\ files/stm32746g_discovery.d ./GUI\ files/stm32746g_discovery.o ./GUI\ files/stm32746g_discovery.su ./GUI\ files/stm32746g_discovery_lcd.d ./GUI\ files/stm32746g_discovery_lcd.o ./GUI\ files/stm32746g_discovery_lcd.su ./GUI\ files/stm32746g_discovery_sdram.d ./GUI\ files/stm32746g_discovery_sdram.o ./GUI\ files/stm32746g_discovery_sdram.su ./GUI\ files/stm32746g_discovery_ts.d ./GUI\ files/stm32746g_discovery_ts.o ./GUI\ files/stm32746g_discovery_ts.su

.PHONY: clean-GUI-20-files

