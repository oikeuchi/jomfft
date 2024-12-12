cmake_minimum_required(VERSION 3.21)

target_sources(${UTIL_OBJLIB} PRIVATE
ComplexDft.c
HandleUtil.c
RealDft.c
ComplexDft.h
HandleUtil.h
Mkl.h
RealDft.h
)