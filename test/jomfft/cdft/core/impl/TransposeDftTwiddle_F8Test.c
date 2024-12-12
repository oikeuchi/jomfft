#include "TransposeDftTwiddle_F8Test.h"

#include <jomfft/cdft/core/impl/TransposeDftTwiddle_F8.h>

#include "../TransposeDftTwiddleFunctionTest.h"

void NS(CDftCore_TransposeDftTwiddle_F8Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_F8), false, 8, 4);
}
