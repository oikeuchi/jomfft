cmake_minimum_required(VERSION 3.21)

add_subdirectory(impl)

target_sources(${TEST} PRIVATE
InnerBatchDftFunctionTest.c
InnerBatchDftTest.c
RegisterTest.c
StrideBatchDftFunctionTest.c
StrideBatchDftTest.c
InnerBatchDftFunctionTest.h
InnerBatchDftTest.h
RegisterTest.h
StrideBatchDftFunctionTest.h
StrideBatchDftTest.h
)