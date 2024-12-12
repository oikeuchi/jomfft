#include "TwiddleDft_B6Test.h"

#include <jomfft/cdft/core/impl/TwiddleDft_B6.h>

#include "../TwiddleDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_B6_1Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B6_1), true, 6, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_B6_2Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B6_2), true, 6, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_B6_4Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B6_4), true, 6, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_B6_8Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B6_8), true, 6, 8);
}
#endif
