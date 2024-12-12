cmake_minimum_required(VERSION 3.21)

add_subdirectory(template)
add_subdirectory(impl)

target_sources(${OBJLIB} PRIVATE
BatchDft.c
Dft.c
LargeBatchDft.c
LargeDft.c
NestedBatchDft.c
NestedDft.c
PairTwiddleTranspose.c
SmallBatchDft.c
SmallDft.c
SmallTransposeDft.c
TwiddleTranspose.c
BatchDft.h
BlockSize.h
Dft.h
LargeBatchDft.h
LargeDft.h
NestedBatchDft.h
NestedDft.h
PairTwiddleTranspose.h
SmallBatchDft.h
SmallDft.h
SmallTransposeDft.h
Square.h
TwiddleTranspose.h
)