################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IndexGenerator.cpp \
../src/Main.cpp 

OBJS += \
./src/IndexGenerator.o \
./src/Main.o 

CPP_DEPS += \
./src/IndexGenerator.d \
./src/Main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/SC-IndexGenerator/src" -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/SC-IndexGenerator/include" -I/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/Utils/arboretum/src/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


