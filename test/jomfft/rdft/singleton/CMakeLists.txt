cmake_minimum_required(VERSION 3.21)

add_subdirectory(impl)

target_sources(${TEST} PRIVATE
EvenSizeStrideBatchDftTest.c
RegisterTest.c
StrideBatchDftFunctionTest.c
StrideBatchDftTest.c
EvenSizeStrideBatchDftTest.h
RegisterTest.h
StrideBatchDftFunctionTest.h
StrideBatchDftTest.h
)