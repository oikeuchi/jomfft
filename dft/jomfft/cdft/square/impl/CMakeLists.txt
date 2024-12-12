cmake_minimum_required(VERSION 3.21)

target_sources(${OBJLIB} PRIVATE
PairTwiddleTranspose_B.c
PairTwiddleTranspose_F.c
TwiddleTranspose_B.c
TwiddleTranspose_F.c
PairTwiddleTranspose_B.h
PairTwiddleTranspose_F.h
TwiddleTranspose_B.h
TwiddleTranspose_F.h
)