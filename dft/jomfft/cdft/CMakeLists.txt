cmake_minimum_required(VERSION 3.21)

add_subdirectory(core)
add_subdirectory(cooleyturkey)
add_subdirectory(bluestein)
add_subdirectory(square)
add_subdirectory(singleton)

target_sources(${OBJLIB} PRIVATE
BatchDft.c
BatchUtil.c
Dft.c
InnerBatchDft.c
MainDft.c
StrideBatchDft.c
BatchDft.h
BatchUtil.h
CDft.h
Dft.h
InnerBatchDft.h
MainDft.h
StrideBatchDft.h
)