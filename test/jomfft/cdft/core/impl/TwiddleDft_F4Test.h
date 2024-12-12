#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_F4_1Test)();
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_F4_2Test)();
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_F4_4Test)();
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_F4_8Test)();
#endif
