#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_B8_1Test)();
#endif
#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_B8_2Test)();
#endif
#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_B8_4Test)();
#endif
