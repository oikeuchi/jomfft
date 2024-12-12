cmake_minimum_required(VERSION 3.21)

add_subdirectory(template)
add_subdirectory(impl)

target_sources(${OBJLIB} PRIVATE
BatchMultiply.c
InnerBatchDft.c
PairSquareTwiddleDft.c
SquareSplitTwiddleDft.c
StrideBatchDft.c
TransposeDftTwiddle.c
TwiddleDft.c
BatchMultiply.h
Core.h
InnerBatchDft.h
PairSquareTwiddleDft.h
SquareSplitTwiddleDft.h
StrideBatchDft.h
TransposeDftTwiddle.h
TwiddleDft.h
)