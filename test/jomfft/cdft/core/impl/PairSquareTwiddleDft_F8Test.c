#include "PairSquareTwiddleDft_F8Test.h"

#include <jomfft/cdft/core/impl/PairSquareTwiddleDft_F8.h>

#include "../PairSquareTwiddleDftFunctionTest.h"

void NS(CDftCore_PairSquareTwiddleDft_F8Test)() {
  NS(CDftCore_PairSquareTwiddleDftFunctionTest)
  (NS(CDftCore_PairSquareTwiddleDft_F8), false, 8);
}
