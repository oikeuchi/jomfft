#include "InnerBatchDft_B1Test.h"

#include <jomfft/cdft/singleton/impl/InnerBatchDft_B1.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftSingleton_InnerBatchDft_B1_1Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B1_1), true, 13, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftSingleton_InnerBatchDft_B1_2Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B1_2), true, 13, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftSingleton_InnerBatchDft_B1_4Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B1_4), true, 13, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftSingleton_InnerBatchDft_B1_8Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_B1_8), true, 13, 8);
}
#endif
