cmake_minimum_required(VERSION 3.21)

add_subdirectory(template)

target_sources(${OBJLIB} PRIVATE
ComplexCopy.c
ComplexTranspose.c
ConjugateEvenCopy.c
RealCopy.c
SplitComplexCopy.c
ComplexCopy.h
ComplexTranspose.h
ConjugateEvenCopy.h
RealCopy.h
SplitComplexCopy.h
Util.h
)