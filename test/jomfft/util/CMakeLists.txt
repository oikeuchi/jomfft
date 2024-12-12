cmake_minimum_required(VERSION 3.21)

target_sources(${TEST} PRIVATE
ComplexTensorTest.c
ComplexTransposeTest.c
RealTensorTest.c
RegisterTest.c
TensorShapeTest.c
ComplexTensorTest.h
ComplexTransposeTest.h
RealTensorTest.h
RegisterTest.h
TensorShapeTest.h
)