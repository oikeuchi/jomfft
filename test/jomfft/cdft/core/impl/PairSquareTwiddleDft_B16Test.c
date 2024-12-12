#include "PairSquareTwiddleDft_B16Test.h"

#include <jomfft/cdft/core/impl/PairSquareTwiddleDft_B16.h>

#include "../PairSquareTwiddleDftFunctionTest.h"

void NS(CDftCore_PairSquareTwiddleDft_B16Test)() {
  NS(CDftCore_PairSquareTwiddleDftFunctionTest)
  (NS(CDftCore_PairSquareTwiddleDft_B16), true, 16);
}
