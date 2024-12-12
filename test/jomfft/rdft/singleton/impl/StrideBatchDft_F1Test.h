#pragma once

#include "../../../Test.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_F1_1Test)();
#endif
#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_F1_2Test)();
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_F1_4Test)();
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_F1_8Test)();
#endif
