#include "SquareSplitTwiddleDft_F2Test.h"

#include <jomfft/cdft/core/impl/SquareSplitTwiddleDft_F2.h>

#include "../SquareSplitTwiddleDftFunctionTest.h"

void NS(CDftCore_SquareSplitTwiddleDft_F2Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest)
  (NS(CDftCore_SquareSplitTwiddleDft_F2), false, 2, 4);
}

#if C_SIMD_SIZE >= 4
void NS(CDftCore_SquareSplitTwiddleDft_F2_4Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest_)
  (NS(CDftCore_SquareSplitTwiddleDft_F2_4), false, 2, 4, 4);
}
#endif
