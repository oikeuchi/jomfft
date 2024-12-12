#include "TwiddleDft_B2Test.h"

#include <jomfft/cdft/core/impl/TwiddleDft_B2.h>

#include "../TwiddleDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_B2_1Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B2_1), true, 2, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_B2_2Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B2_2), true, 2, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_B2_4Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B2_4), true, 2, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_B2_8Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B2_8), true, 2, 8);
}
#endif
