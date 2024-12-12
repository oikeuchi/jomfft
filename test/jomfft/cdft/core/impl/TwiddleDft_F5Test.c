#include "TwiddleDft_F5Test.h"

#include <jomfft/cdft/core/impl/TwiddleDft_F5.h>

#include "../TwiddleDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_F5_1Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_F5_1), false, 5, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_F5_2Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_F5_2), false, 5, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_F5_4Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_F5_4), false, 5, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_F5_8Test)() {
  NS(CDftCore_TwiddleDftFunctionTest)
  (NS(CDftCore_TwiddleDft_F5_8), false, 5, 8);
}
#endif
