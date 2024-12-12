cmake_minimum_required(VERSION 3.21)

add_subdirectory(arm)
add_subdirectory(x64)

target_sources(${OBJLIB} PRIVATE
ComplexCommon.h
Complex_1.h
Complex_V.h
RealCommon.h
Real_1.h
Real_2.h
Real_V.h
Simd.h
SplitComplex.h
)