#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_B5_1Test)();
#endif
#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_B5_2Test)();
#endif
#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_B5_4Test)();
#endif