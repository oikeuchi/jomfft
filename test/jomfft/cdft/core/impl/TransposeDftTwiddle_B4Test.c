#include "TransposeDftTwiddle_B4Test.h"

#include <jomfft/cdft/core/impl/TransposeDftTwiddle_B4.h>

#include "../TransposeDftTwiddleFunctionTest.h"

void NS(CDftCore_TransposeDftTwiddle_B4Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_B4), true, 4, 4);
}
