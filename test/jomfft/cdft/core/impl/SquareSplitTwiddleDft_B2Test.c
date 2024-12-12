#include "SquareSplitTwiddleDft_B2Test.h"

#include <jomfft/cdft/core/impl/SquareSplitTwiddleDft_B2.h>

#include "../SquareSplitTwiddleDftFunctionTest.h"

void NS(CDftCore_SquareSplitTwiddleDft_B2Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest)
  (NS(CDftCore_SquareSplitTwiddleDft_B2), true, 2, 4);
}

#if C_SIMD_SIZE >= 4
void NS(CDftCore_SquareSplitTwiddleDft_B2_4Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest_)
  (NS(CDftCore_SquareSplitTwiddleDft_B2_4), true, 2, 4, 4);
}
#endif
