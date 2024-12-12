cmake_minimum_required(VERSION 3.21)

add_subdirectory(template)
add_subdirectory(impl)

target_sources(${OBJLIB} PRIVATE
EvenSizeStrideBatchDft.c
StrideBatchDft.c
EvenSizeStrideBatchDft.h
Singleton.h
StrideBatchDft.h
)