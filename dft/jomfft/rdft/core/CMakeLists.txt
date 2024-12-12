cmake_minimum_required(VERSION 3.21)

add_subdirectory(template)
add_subdirectory(impl)

target_sources(${OBJLIB} PRIVATE
ComplexToPairDft.c
EvenSizeStrideBatchDft.c
EvenSizeTwiddleDft.c
StrideBatchDft.c
ComplexToPairDft.h
Core.h
EvenSizeStrideBatchDft.h
EvenSizeTwiddleDft.h
StrideBatchDft.h
)