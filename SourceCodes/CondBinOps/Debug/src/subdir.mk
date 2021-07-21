################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Expression.cpp \
../src/Generics.cpp \
../src/Main.cpp 

OBJS += \
./src/Expression.o \
./src/Generics.o \
./src/Main.o 

CPP_DEPS += \
./src/Expression.d \
./src/Generics.d \
./src/Main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/CondBinOps/src" -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/CondBinOps/include" -I/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/Utils/arboretum/src/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


