#include "InnerBatchDft_B8Test.h"

#include <jomfft/cdft/core/impl/InnerBatchDft_B8.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_InnerBatchDft_B8_1Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B8_1), true, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_InnerBatchDft_B8_2Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B8_2), true, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_InnerBatchDft_B8_4Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B8_4), true, 8, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_InnerBatchDft_B8_8Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B8_8), true, 8, 8);
}
#endif
