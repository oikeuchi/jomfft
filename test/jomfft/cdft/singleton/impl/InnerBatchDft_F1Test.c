#include "InnerBatchDft_F1Test.h"

#include <jomfft/cdft/singleton/impl/InnerBatchDft_F1.h>

#include "../InnerBatchDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(CDftSingleton_InnerBatchDft_F1_1Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F1_1), false, 13, 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(CDftSingleton_InnerBatchDft_F1_2Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F1_2), false, 13, 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(CDftSingleton_InnerBatchDft_F1_4Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F1_4), false, 13, 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(CDftSingleton_InnerBatchDft_F1_8Test)() {
  NS(CDftSingleton_InnerBatchDftFunctionTest)
  (NS(CDftSingleton_InnerBatchDft_F1_8), false, 13, 8);
}
#endif
