cmake_minimum_required(VERSION 3.21)

target_sources(${OBJLIB} PRIVATE
Common.h
Dft_B0.h
Dft_B1.h
Dft_F0.h
Dft_F1.h
StrideBatchDft_B.h
StrideBatchDft_F.h
)