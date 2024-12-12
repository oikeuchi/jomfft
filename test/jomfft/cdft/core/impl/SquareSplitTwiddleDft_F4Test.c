#include "SquareSplitTwiddleDft_F4Test.h"

#include <jomfft/cdft/core/impl/SquareSplitTwiddleDft_F4.h>

#include "../SquareSplitTwiddleDftFunctionTest.h"

void NS(CDftCore_SquareSplitTwiddleDft_F4Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest)
  (NS(CDftCore_SquareSplitTwiddleDft_F4), false, 4, 4);
}
