#include "StrideBatchDft_F1Test.h"

#include <jomfft/rdft/singleton/impl/StrideBatchDft_F1.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_F1_1Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F1_1), 13, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_F1_2Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F1_2), 13, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_F1_4Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F1_4), 13, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftSingleton_StrideBatchDft_F1_8Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_FTest)
  (NS(RDftSingleton_StrideBatchDft_F1_4), 13, 8);
}
#endif
