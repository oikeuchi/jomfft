cmake_minimum_required(VERSION 3.21)

target_sources(${OBJLIB} PRIVATE
BluesteinDftData.c
BluesteinDftDataList.c
DataFactory.c
LargeTwiddleMatrix.c
LargeTwiddleMatrixList.c
TwiddleMatrix.c
TwiddleMatrixList.c
TwiddleVector.c
TwiddleVectorList.c
BluesteinDftData.h
BluesteinDftDataList.h
Data.h
DataFactory.h
LargeTwiddleMatrix.h
LargeTwiddleMatrixList.h
TwiddleMatrix.h
TwiddleMatrixList.h
TwiddleVector.h
TwiddleVectorList.h
)