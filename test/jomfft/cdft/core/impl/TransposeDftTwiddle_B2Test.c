#include "TransposeDftTwiddle_B2Test.h"

#include <jomfft/cdft/core/impl/TransposeDftTwiddle_B2.h>

#include "../TransposeDftTwiddleFunctionTest.h"

void NS(CDftCore_TransposeDftTwiddle_B2Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_B2), true, 2, 4);
}

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TransposeDftTwiddle_B2_4Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_B2_4), true, 2, 4);
}
#endif
