#include "StrideBatchDft_B1Test.h"

#include <jomfft/cdft/singleton/impl/StrideBatchDft_B1.h>

#include "../StrideBatchDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftSingleton_StrideBatchDft_B1_1Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_B1_1), true, 13, 8);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(CDftSingleton_StrideBatchDft_B1_2Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_B1_2), true, 13, 8);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(CDftSingleton_StrideBatchDft_B1_4Test)() {
  NS(CDftSingleton_StrideBatchDftFunctionTest)
  (NS(CDftSingleton_StrideBatchDft_B1_4), true, 13, 8);
}
#endif
