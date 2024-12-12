cmake_minimum_required(VERSION 3.21)

add_subdirectory(constants)

target_sources(${COMMON_OBJLIB} PRIVATE
PrimeFactors.c
IntegerMath.h
PrimeFactors.h
)