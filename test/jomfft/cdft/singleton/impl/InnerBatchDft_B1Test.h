#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE >= 1
void NS(CDftSingleton_InnerBatchDft_B1_1Test)();
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftSingleton_InnerBatchDft_B1_2Test)();
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftSingleton_InnerBatchDft_B1_4Test)();
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftSingleton_InnerBatchDft_B1_8Test)();
#endif
