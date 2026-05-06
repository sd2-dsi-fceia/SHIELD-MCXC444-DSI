################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/debug_console/fsl_debug_console.c 

C_DEPS += \
./utilities/debug_console/fsl_debug_console.d 

OBJS += \
./utilities/debug_console/fsl_debug_console.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/debug_console/%.o: ../utilities/debug_console/%.c utilities/debug_console/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MCXC444VLH -DCPU_MCXC444VLH_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\board" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\source" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\drivers" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\CMSIS" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\CMSIS\m-profile" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities\debug_console\config" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\device" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\device\periph2" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities\debug_console" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\component\serial_manager" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\component\lists" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\utilities\str" -I"D:\aguat\Documents\GitHub\SD2_MCXC444_CAN\component\uart" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities-2f-debug_console

clean-utilities-2f-debug_console:
	-$(RM) ./utilities/debug_console/fsl_debug_console.d ./utilities/debug_console/fsl_debug_console.o

.PHONY: clean-utilities-2f-debug_console

