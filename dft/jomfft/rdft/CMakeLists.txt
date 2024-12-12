cmake_minimum_required(VERSION 3.21)

add_subdirectory(core)
add_subdirectory(cooleyturkey)
add_subdirectory(bluestein)
add_subdirectory(square)
add_subdirectory(singleton)

target_sources(${OBJLIB} PRIVATE
BatchDft.c
Dft.c
EvenSizeBatch.c
EvenSizeDft.c
MainDft.c
BatchDft.h
Dft.h
EvenSizeBatch.h
EvenSizeDft.h
MainDft.h
RDft.h
)