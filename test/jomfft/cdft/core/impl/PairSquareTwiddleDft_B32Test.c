#include "PairSquareTwiddleDft_B32Test.h"

#include <jomfft/cdft/core/impl/PairSquareTwiddleDft_B32.h>

#include "../PairSquareTwiddleDftFunctionTest.h"

void NS(CDftCore_PairSquareTwiddleDft_B32Test)() {
  NS(CDftCore_PairSquareTwiddleDftFunctionTest)
  (NS(CDftCore_PairSquareTwiddleDft_B32), true, 32);
}
