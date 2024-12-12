#include "TwiddleDft_B9Test.h"

#include <jomfft/cdft/core/impl/TwiddleDft_B9.h>

#include "../TwiddleDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_B9_1Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B9_1), true, 9, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_B9_2Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B9_2), true, 9, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_B9_4Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B9_4), true, 9, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_B9_8Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_B9_8), true, 9, 8);
}
#endif
