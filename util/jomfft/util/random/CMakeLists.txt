cmake_minimum_required(VERSION 3.21)

target_sources(${UTIL_OBJLIB} PRIVATE
ComplexTensor.c
ConjugateEvenTensor.c
Random.c
RealTensor.c
ComplexTensor.h
ConjugateEvenTensor.h
Random.h
RealTensor.h
)