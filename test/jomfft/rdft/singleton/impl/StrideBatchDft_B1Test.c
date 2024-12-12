#include "StrideBatchDft_B1Test.h"

#include <jomfft/rdft/singleton/impl/StrideBatchDft_B1.h>

#include "../StrideBatchDftFunctionTest.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_B1_1Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B1_1), 13, 8);
}
#endif

#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_B1_2Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B1_2), 13, 8);
}
#endif

#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_B1_4Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B1_4), 13, 8);
}
#endif

#if R_SIMD_SIZE_S >= 8
void NS(RDftSingleton_StrideBatchDft_B1_8Test)() {
  NS(RDftSingleton_StrideBatchDftFunction_BTest)
  (NS(RDftSingleton_StrideBatchDft_B1_4), 13, 8);
}
#endif
