cmake_minimum_required(VERSION 3.21)

target_sources(${TEST} PRIVATE
BatchDftTest.c
DftTest.c
EvenSizeBatchDftTest.c
EvenSizeDftTest.c
RegisterTest.c
BatchDftTest.h
DftTest.h
EvenSizeBatchDftTest.h
EvenSizeDftTest.h
RegisterTest.h
)