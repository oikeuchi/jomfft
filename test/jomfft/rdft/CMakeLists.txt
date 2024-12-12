cmake_minimum_required(VERSION 3.21)

add_subdirectory(core)
add_subdirectory(naive)
add_subdirectory(cooleyturkey)
add_subdirectory(bluestein)
add_subdirectory(square)
add_subdirectory(singleton)

target_sources(${TEST} PRIVATE
BatchDftTest.c
BatchDftTest_.c
DftTest.c
DftTest_.c
InnerBatchDftTest.c
OuterBatchDftTest.c
RegisterTest.c
TransposeDftTest.c
BatchDftTest.h
BatchDftTest_.h
DftTest.h
DftTest_.h
InnerBatchDftTest.h
OuterBatchDftTest.h
RegisterTest.h
TransposeDftTest.h
)