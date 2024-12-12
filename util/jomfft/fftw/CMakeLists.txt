cmake_minimum_required(VERSION 3.21)

target_sources(${UTIL_OBJLIB} PRIVATE
ComplexDft.c
Plan.c
RealDft.c
ComplexDft.h
Fftw.h
Plan.h
RealDft.h
)