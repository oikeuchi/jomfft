#include "SquareSplitTwiddleDft_B4Test.h"

#include <jomfft/cdft/core/impl/SquareSplitTwiddleDft_B4.h>

#include "../SquareSplitTwiddleDftFunctionTest.h"

void NS(CDftCore_SquareSplitTwiddleDft_B4Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest)
  (NS(CDftCore_SquareSplitTwiddleDft_B4), true, 4, 4);
}
