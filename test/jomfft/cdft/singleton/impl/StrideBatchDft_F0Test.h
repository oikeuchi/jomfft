#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftSingleton_StrideBatchDft_F0_1Test)();
#endif
#if C_SIMD_SIZE_S >= 2
void NS(CDftSingleton_StrideBatchDft_F0_2Test)();
#endif
#if C_SIMD_SIZE_S >= 4
void NS(CDftSingleton_StrideBatchDft_F0_4Test)();
#endif
