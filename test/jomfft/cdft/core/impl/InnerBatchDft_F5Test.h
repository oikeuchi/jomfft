#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_InnerBatchDft_F5_1Test)();
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftCore_InnerBatchDft_F5_2Test)();
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftCore_InnerBatchDft_F5_4Test)();
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftCore_InnerBatchDft_F5_8Test)();
#endif
