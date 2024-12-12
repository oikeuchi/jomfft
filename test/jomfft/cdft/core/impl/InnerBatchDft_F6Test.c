#include "InnerBatchDft_F6Test.h"

#include <jomfft/cdft/core/impl/InnerBatchDft_F6.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_InnerBatchDft_F6_1Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F6_1), false, 6, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_InnerBatchDft_F6_2Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F6_2), false, 6, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_InnerBatchDft_F6_4Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F6_4), false, 6, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_InnerBatchDft_F6_8Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F6_8), false, 6, 8);
}
#endif
