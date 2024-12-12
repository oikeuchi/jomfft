#pragma once

#include "../../../Test.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_B3_1Test)();
#endif
#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_B3_2Test)();
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_B3_4Test)();
#endif
#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_B3_8Test)();
#endif
