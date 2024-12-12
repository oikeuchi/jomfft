cmake_minimum_required(VERSION 3.21)

target_sources(${OBJLIB} PRIVATE
BatchDft.c
Dft.c
EvenSizeBatchDft.c
EvenSizeDft.c
BatchDft.h
CooleyTurkey.h
Dft.h
EvenSizeBatchDft.h
EvenSizeDft.h
)