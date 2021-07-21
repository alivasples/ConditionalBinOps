################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../include/arboretum/CStorage.cpp \
../include/arboretum/stCellId.cpp \
../include/arboretum/stCompress.cpp \
../include/arboretum/stCountingTree.cpp \
../include/arboretum/stDBMNode.cpp \
../include/arboretum/stDFNode.cpp \
../include/arboretum/stDiskPageManager.cpp \
../include/arboretum/stDummyNode.cpp \
../include/arboretum/stGHNode.cpp \
../include/arboretum/stGnuplot.cpp \
../include/arboretum/stGnuplot3D.cpp \
../include/arboretum/stLevelDiskAccess.cpp \
../include/arboretum/stListPriorityQueue.cpp \
../include/arboretum/stMMNode.cpp \
../include/arboretum/stMNode.cpp \
../include/arboretum/stMemoryPageManager.cpp \
../include/arboretum/stPage.cpp \
../include/arboretum/stPlainDiskPageManager.cpp \
../include/arboretum/stPointSet.cpp \
../include/arboretum/stResult.cpp \
../include/arboretum/stSeqNode.cpp \
../include/arboretum/stSlimNode.cpp \
../include/arboretum/stStructUtils.cpp \
../include/arboretum/stTreeInformation.cpp \
../include/arboretum/stUtil.cpp \
../include/arboretum/stVPNode.cpp 

OBJS += \
./include/arboretum/CStorage.o \
./include/arboretum/stCellId.o \
./include/arboretum/stCompress.o \
./include/arboretum/stCountingTree.o \
./include/arboretum/stDBMNode.o \
./include/arboretum/stDFNode.o \
./include/arboretum/stDiskPageManager.o \
./include/arboretum/stDummyNode.o \
./include/arboretum/stGHNode.o \
./include/arboretum/stGnuplot.o \
./include/arboretum/stGnuplot3D.o \
./include/arboretum/stLevelDiskAccess.o \
./include/arboretum/stListPriorityQueue.o \
./include/arboretum/stMMNode.o \
./include/arboretum/stMNode.o \
./include/arboretum/stMemoryPageManager.o \
./include/arboretum/stPage.o \
./include/arboretum/stPlainDiskPageManager.o \
./include/arboretum/stPointSet.o \
./include/arboretum/stResult.o \
./include/arboretum/stSeqNode.o \
./include/arboretum/stSlimNode.o \
./include/arboretum/stStructUtils.o \
./include/arboretum/stTreeInformation.o \
./include/arboretum/stUtil.o \
./include/arboretum/stVPNode.o 

CPP_DEPS += \
./include/arboretum/CStorage.d \
./include/arboretum/stCellId.d \
./include/arboretum/stCompress.d \
./include/arboretum/stCountingTree.d \
./include/arboretum/stDBMNode.d \
./include/arboretum/stDFNode.d \
./include/arboretum/stDiskPageManager.d \
./include/arboretum/stDummyNode.d \
./include/arboretum/stGHNode.d \
./include/arboretum/stGnuplot.d \
./include/arboretum/stGnuplot3D.d \
./include/arboretum/stLevelDiskAccess.d \
./include/arboretum/stListPriorityQueue.d \
./include/arboretum/stMMNode.d \
./include/arboretum/stMNode.d \
./include/arboretum/stMemoryPageManager.d \
./include/arboretum/stPage.d \
./include/arboretum/stPlainDiskPageManager.d \
./include/arboretum/stPointSet.d \
./include/arboretum/stResult.d \
./include/arboretum/stSeqNode.d \
./include/arboretum/stSlimNode.d \
./include/arboretum/stStructUtils.d \
./include/arboretum/stTreeInformation.d \
./include/arboretum/stUtil.d \
./include/arboretum/stVPNode.d 


# Each subdirectory must supply rules for building sources it contributes
include/arboretum/%.o: ../include/arboretum/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/CondBinOps/src" -I"/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/CondBinOps/include" -I/home/alivasples/Documentos/PROJECTS/ConditionalBinOps/SourceCodes/Utils/arboretum/src/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


