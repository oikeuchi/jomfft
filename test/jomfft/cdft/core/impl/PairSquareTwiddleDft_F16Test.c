#include "PairSquareTwiddleDft_F16Test.h"

#include <jomfft/cdft/core/impl/PairSquareTwiddleDft_F16.h>

#include "../PairSquareTwiddleDftFunctionTest.h"

void NS(CDftCore_PairSquareTwiddleDft_F16Test)() {
  NS(CDftCore_PairSquareTwiddleDftFunctionTest)
  (NS(CDftCore_PairSquareTwiddleDft_F16), false, 16);
}
