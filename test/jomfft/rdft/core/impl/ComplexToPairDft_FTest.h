#pragma once

#include "../../../Test.h"

#if C_SIMD_SIZE_S >= 1
void NS(RDftCore_ComplexToPairDft_F_1Test)();
#endif
#if C_SIMD_SIZE_S >= 2
void NS(RDftCore_ComplexToPairDft_F_2Test)();
#endif
#if C_SIMD_SIZE_S >= 4
void NS(RDftCore_ComplexToPairDft_F_4Test)();
#endif
