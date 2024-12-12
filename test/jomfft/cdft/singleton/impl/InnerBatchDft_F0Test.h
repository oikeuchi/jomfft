#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE >= 1
void NS(CDftSingleton_InnerBatchDft_F0_1Test)();
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftSingleton_InnerBatchDft_F0_2Test)();
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftSingleton_InnerBatchDft_F0_4Test)();
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftSingleton_InnerBatchDft_F0_8Test)();
#endif
