#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_F32_1Test)();
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_F32_2Test)();
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_F32_4Test)();
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_F32_8Test)();
#endif
