#include "InnerBatchDft_B16Test.h"

#include <jomfft/cdft/core/impl/InnerBatchDft_B16.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_InnerBatchDft_B16_1Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B16_1), true, 16, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_InnerBatchDft_B16_2Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B16_2), true, 16, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_InnerBatchDft_B16_4Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B16_4), true, 16, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_InnerBatchDft_B16_8Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B16_8), true, 16, 8);
}
#endif
