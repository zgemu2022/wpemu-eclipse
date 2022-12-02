################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_common/base_protocol_104/standard104.cpp \
../src_common/base_protocol_104/yl_socket.cpp 

OBJS += \
./src_common/base_protocol_104/standard104.o \
./src_common/base_protocol_104/yl_socket.o 

CPP_DEPS += \
./src_common/base_protocol_104/standard104.d \
./src_common/base_protocol_104/yl_socket.d 


# Each subdirectory must supply rules for building sources it contributes
src_common/base_protocol_104/%.o: ../src_common/base_protocol_104/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I"/home/cz/STM32CubeIDE/wpemu/DllCommon/src_common/base_protocol_104" -I"/home/cz/STM32CubeIDE/wpemu/DllCommon/src_common/common" -I"/home/cz/STM32CubeIDE/wpemu/DllCommon/src_common/json" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


