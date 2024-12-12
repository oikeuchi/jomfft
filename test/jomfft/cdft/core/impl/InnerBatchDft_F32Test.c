#include "InnerBatchDft_F32Test.h"

#include <jomfft/cdft/core/impl/InnerBatchDft_F32.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_InnerBatchDft_F32_1Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F32_1), false, 32, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftCore_InnerBatchDft_F32_2Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F32_2), false, 32, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftCore_InnerBatchDft_F32_4Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F32_4), false, 32, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftCore_InnerBatchDft_F32_8Test)() {
  NS(CDftCore_InnerBatchDftFunctionTest)
  (NS(CDftCore_InnerBatchDft_F32_8), false, 32, 8);
}
#endif
