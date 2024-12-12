cmake_minimum_required(VERSION 3.21)

target_sources(${TEST} PRIVATE
InnerBatchDft_B0Test.c
InnerBatchDft_B1Test.c
InnerBatchDft_F0Test.c
InnerBatchDft_F1Test.c
RegisterTest.c
StrideBatchDft_B0Test.c
StrideBatchDft_B1Test.c
StrideBatchDft_F0Test.c
StrideBatchDft_F1Test.c
InnerBatchDft_B0Test.h
InnerBatchDft_B1Test.h
InnerBatchDft_F0Test.h
InnerBatchDft_F1Test.h
RegisterTest.h
StrideBatchDft_B0Test.h
StrideBatchDft_B1Test.h
StrideBatchDft_F0Test.h
StrideBatchDft_F1Test.h
)