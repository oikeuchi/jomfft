#pragma once

#include "../../../Test.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_B0_1Test)();
#endif
#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_B0_2Test)();
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_B0_4Test)();
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_B0_8Test)();
#endif
