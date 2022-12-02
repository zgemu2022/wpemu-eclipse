################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main_pro/TaskMain.cpp \
../src/main_pro/program.cpp 

OBJS += \
./src/main_pro/TaskMain.o \
./src/main_pro/program.o 

CPP_DEPS += \
./src/main_pro/TaskMain.d \
./src/main_pro/program.d 


# Each subdirectory must supply rules for building sources it contributes
src/main_pro/%.o: ../src/main_pro/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"/home/cz/STM32CubeIDE/wpemu/emu-app/src/common" -I"/home/cz/STM32CubeIDE/wpemu/emu-app/src/config_firm" -I"/home/cz/STM32CubeIDE/wpemu/emu-app/src/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


