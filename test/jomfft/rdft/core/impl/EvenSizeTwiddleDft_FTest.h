#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE >= 1
void NS(RDftCore_EvenSizeTwiddleDft_F_1Test)();
#endif
#if C_SIMD_SIZE >= 2
void NS(RDftCore_EvenSizeTwiddleDft_F_2Test)();
#endif
#if C_SIMD_SIZE >= 4
void NS(RDftCore_EvenSizeTwiddleDft_F_4Test)();
#endif
#if C_SIMD_SIZE >= 8
void NS(RDftCore_EvenSizeTwiddleDft_F_8Test)();
#endif
