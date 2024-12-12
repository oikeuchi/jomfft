#include "SquareSplitTwiddleDft_F8Test.h"

#include <jomfft/cdft/core/impl/SquareSplitTwiddleDft_F8.h>

#include "../SquareSplitTwiddleDftFunctionTest.h"

void NS(CDftCore_SquareSplitTwiddleDft_F8Test)() {
  NS(CDftCore_SquareSplitTwiddleDftFunctionTest)
  (NS(CDftCore_SquareSplitTwiddleDft_F8), false, 8, 4);
}
