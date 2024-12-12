#include "TransposeDftTwiddle_B8Test.h"

#include <jomfft/cdft/core/impl/TransposeDftTwiddle_B8.h>

#include "../TransposeDftTwiddleFunctionTest.h"

void NS(CDftCore_TransposeDftTwiddle_B8Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_B8), true, 8, 4);
}
