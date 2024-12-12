cmake_minimum_required(VERSION 3.21)

target_sources(${TEST} PRIVATE
BatchDftTest.c
DftTest.c
LargeBatchDftTest.c
LargeDftTest.c
NestedBatchDftTest.c
NestedDftTest.c
PairTwiddleTransposeTest.c
RegisterTest.c
SmallBatchDftTest.c
SmallDftTest.c
SmallTransposeDftTest.c
TwiddleTransposeTest.c
BatchDftTest.h
DftTest.h
LargeBatchDftTest.h
LargeDftTest.h
NestedBatchDftTest.h
NestedDftTest.h
PairTwiddleTransposeTest.h
RegisterTest.h
SmallBatchDftTest.h
SmallDftTest.h
SmallTransposeDftTest.h
TwiddleTransposeTest.h
)