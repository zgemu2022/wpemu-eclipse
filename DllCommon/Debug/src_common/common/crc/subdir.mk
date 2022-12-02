################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src_common/common/crc/crc16_check.c \
../src_common/common/crc/crc_cal_by_bit.c \
../src_common/common/crc/crc_cal_by_byte.c \
../src_common/common/crc/crc_cal_by_halfbyte.c 

OBJS += \
./src_common/common/crc/crc16_check.o \
./src_common/common/crc/crc_cal_by_bit.o \
./src_common/common/crc/crc_cal_by_byte.o \
./src_common/common/crc/crc_cal_by_halfbyte.o 

C_DEPS += \
./src_common/common/crc/crc16_check.d \
./src_common/common/crc/crc_cal_by_bit.d \
./src_common/common/crc/crc_cal_by_byte.d \
./src_common/common/crc/crc_cal_by_halfbyte.d 


# Each subdirectory must supply rules for building sources it contributes
src_common/common/crc/%.o: ../src_common/common/crc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


