cmake_minimum_required(VERSION 3.21)

target_sources(${OBJLIB} PRIVATE
BatchDft.c
Dft.c
TransposeDft.c
BatchDft.h
CooleyTurkey.h
Dft.h
TransposeDft.h
)