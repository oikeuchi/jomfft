#include "InnerBatchDft_B3Test.h"

#include <jomfft/cdft/core/impl/InnerBatchDft_B3.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_InnerBatchDft_B3_1Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B3_1), true, 3, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_InnerBatchDft_B3_2Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B3_2), true, 3, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_InnerBatchDft_B3_4Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B3_4), true, 3, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_InnerBatchDft_B3_8Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_B3_8), true, 3, 8);
}
#endif