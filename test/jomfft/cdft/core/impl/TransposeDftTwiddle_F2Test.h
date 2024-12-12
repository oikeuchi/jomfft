#pragma once

#include "../../../Test.h"

void NS(CDftCore_TransposeDftTwiddle_F2Test)();
#if C_SIMD_SIZE >= 4
void NS(CDftCore_TransposeDftTwiddle_F2_4Test)();
#endif
