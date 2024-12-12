#include "TwiddleDft_B64Test.h"

#include <jomfft/cdft/core/impl/TwiddleDft_B64.h>

#include "../TwiddleDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_B64_1Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B64_1), true, 64, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_B64_2Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B64_2), true, 64, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_B64_4Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B64_4), true, 64, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_B64_8Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B64_8), true, 64, 8);
}
#endif
