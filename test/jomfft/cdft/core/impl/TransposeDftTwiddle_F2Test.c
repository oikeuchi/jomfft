#include "TransposeDftTwiddle_F2Test.h"

#include <jomfft/cdft/core/impl/TransposeDftTwiddle_F2.h>

#include "../TransposeDftTwiddleFunctionTest.h"

void NS(CDftCore_TransposeDftTwiddle_F2Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_F2), false, 2, 4);
}

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TransposeDftTwiddle_F2_4Test)() {
  NS(CDftCore_TransposeDftTwiddleFunctionTest)
  (NS(CDftCore_TransposeDftTwiddle_F2_4), false, 2, 4);
}
#endif
