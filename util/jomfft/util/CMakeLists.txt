cmake_minimum_required(VERSION 3.21)

add_subdirectory(template)
add_subdirectory(random)

target_sources(${UTIL_OBJLIB} PRIVATE
ComplexTensor.c
ComplexUtil.c
RealTensor.c
RealUtil.c
TensorShape.c
TensorUtil.c
Variadic.c
ComplexTensor.h
ComplexUtil.h
GetOpt.h
RealTensor.h
RealUtil.h
TensorShape.h
TensorUtil.h
Util_.h
Variadic.h
)