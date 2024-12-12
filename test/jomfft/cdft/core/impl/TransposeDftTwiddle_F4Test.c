#include "TransposeDftTwiddle_F4Test.h"

#include <jomfft/cdft/core/impl/TransposeDftTwiddle_F4.h>

#include "../TransposeDftTwiddleFunctionTest.h"

void NS(CDftCore_TransposeDftTwiddle_F4Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_F4), false, 4, 4);
}
