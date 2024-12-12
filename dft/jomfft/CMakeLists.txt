cmake_minimum_required(VERSION 3.21)

add_subdirectory(util)
add_subdirectory(model)
add_subdirectory(rdft)
add_subdirectory(cdft)
add_subdirectory(simd)
add_subdirectory(data)
add_subdirectory(math)

target_sources(${OBJLIB} PRIVATE
Dft.c
Dft.h
)