################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../include/sfp/simple_file_parser.cc 

CC_DEPS += \
./include/sfp/simple_file_parser.d 

OBJS += \
./include/sfp/simple_file_parser.o 


# Each subdirectory must supply rules for building sources it contributes
include/sfp/%.o: ../include/sfp/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/SC-IndexGenerator/src" -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/SC-IndexGenerator/include" -I/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/Utils/arboretum/src/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


