#include "StrideBatchDft_F1Test.h"

#include <jomfft/cdft/singleton/impl/StrideBatchDft_F1.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftSingleton_StrideBatchDft_F1_1Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_F1_1), false, 13, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftSingleton_StrideBatchDft_F1_2Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_F1_2), false, 13, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftSingleton_StrideBatchDft_F1_4Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_F1_4), false, 13, 8);
}
#endif
