################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.c 

CPP_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.cpp 

OBJS += \
./.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.o 

C_DEPS += \
./.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.d 

CPP_DEPS += \
./.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/spec.d 


# Each subdirectory must supply rules for building sources it contributes
.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/%.o: ../.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/%.o: ../.metadata/.plugins/org.eclipse.cdt.managedbuilder.core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"/home/cz/STM32CubeIDE/wpemu/emu-app/src/common" -I"/home/cz/STM32CubeIDE/wpemu/emu-app/src/config_firm" -I"/home/cz/STM32CubeIDE/wpemu/emu-app/src/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


