################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/SD2_board.c \
../source/main.c \
../source/mcp2515.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/spi_can.c 

C_DEPS += \
./source/SD2_board.d \
./source/main.d \
./source/mcp2515.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/spi_can.d 

OBJS += \
./source/SD2_board.o \
./source/main.o \
./source/mcp2515.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/spi_can.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXC444VLH -DCPU_MCXC444VLH_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\board" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\source" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\drivers" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\CMSIS" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\CMSIS\m-profile" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities\debug_console\config" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\device" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\device\periph2" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities\debug_console" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\component\serial_manager" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\component\lists" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities\str" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\component\uart" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/SD2_board.d ./source/SD2_board.o ./source/main.d ./source/main.o ./source/mcp2515.d ./source/mcp2515.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/spi_can.d ./source/spi_can.o

.PHONY: clean-source

