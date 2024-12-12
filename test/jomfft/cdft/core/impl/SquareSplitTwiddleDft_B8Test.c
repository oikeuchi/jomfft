#include "SquareSplitTwiddleDft_B8Test.h"

#include <jomfft/cdft/core/impl/SquareSplitTwiddleDft_B8.h>

#include "../SquareSplitTwiddleDftFunctionTest.h"

void NS(CDftCore_SquareSplitTwiddleDft_B8Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest)
  (NS(CDftCore_SquareSplitTwiddleDft_B8), true, 8, 4);
}
