cmake_minimum_required(VERSION 3.21)

add_subdirectory(util)
add_subdirectory(wrapper)
add_subdirectory(mkl)
add_subdirectory(fftw)

target_sources(${UTIL_OBJLIB} PRIVATE
Util.h
)