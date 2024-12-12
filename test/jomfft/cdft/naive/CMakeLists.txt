cmake_minimum_required(VERSION 3.21)

target_sources(${TEST} PRIVATE
BatchDft.c
BatchMultiply.c
Dft.c
TwiddleDft.c
BatchDft.h
BatchMultiply.h
Dft.h
Naive.h
TwiddleDft.h
)