#include "PairSquareTwiddleDft_F32Test.h"

#include <jomfft/cdft/core/impl/PairSquareTwiddleDft_F32.h>

#include "../PairSquareTwiddleDftFunctionTest.h"

void NS(CDftCore_PairSquareTwiddleDft_F32Test)() {
  NS(CDftCore_PairSquareTwiddleDftFunctionTest)
  (NS(CDftCore_PairSquareTwiddleDft_F32), false, 32);
}
