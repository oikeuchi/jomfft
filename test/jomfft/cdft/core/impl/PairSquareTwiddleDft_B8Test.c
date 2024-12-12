#include "PairSquareTwiddleDft_B8Test.h"

#include <jomfft/cdft/core/impl/PairSquareTwiddleDft_B8.h>

#include "../PairSquareTwiddleDftFunctionTest.h"

void NS(CDftCore_PairSquareTwiddleDft_B8Test)() {
  NS(CDftCore_PairSquareTwiddleDftFunctionTest)
  (NS(CDftCore_PairSquareTwiddleDft_B8), true, 8);
}
