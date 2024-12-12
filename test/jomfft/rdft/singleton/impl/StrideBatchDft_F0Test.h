#pragma once

#include "../../../Test.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_F0_1Test)();
#endif
#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_F0_2Test)();
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_F0_4Test)();
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_F0_8Test)();
#endif
