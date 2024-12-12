#include "StrideBatchDft_B9Test.h"

#include <jomfft/cdft/core/impl/StrideBatchDft_B9.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_B9_1Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B9_1), true, 9, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_B9_2Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B9_2), true, 9, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_B9_4Test)() {
  NS(CDftCore_StrideBatchDftFunctionTest)
  (NS(CDftCore_StrideBatchDft_B9_4), true, 9, 8);
}
#endif
