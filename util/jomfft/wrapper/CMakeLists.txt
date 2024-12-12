cmake_minimum_required(VERSION 3.21)

target_sources(${UTIL_OBJLIB} PRIVATE
ComplexDft.c
ComplexDftSizes.c
DftSizeUtil.c
RealDft.c
RealDftSizes.c
ComplexDft.h
ComplexDftSizes.h
DftSizeUtil.h
RealDft.h
RealDftSizes.h
Wrapper.h
)