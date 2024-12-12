#include "EvenSizeTwiddleDft_BTest.h"

#include <jomfft/rdft/core/impl/EvenSizeTwiddleDft_B.h>

#include "../EvenSizeTwiddleDftFunctionTest.h"

#if C_SIMD_SIZE >= 1
void NS(RDftCore_EvenSizeTwiddleDft_B_1Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_BTest)
  (NS(RDftCore_EvenSizeTwiddleDft_B_1), 8);
}
#endif

#if C_SIMD_SIZE >= 2
void NS(RDftCore_EvenSizeTwiddleDft_B_2Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_BTest)
  (NS(RDftCore_EvenSizeTwiddleDft_B_2), 8);
}
#endif

#if C_SIMD_SIZE >= 4
void NS(RDftCore_EvenSizeTwiddleDft_B_4Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_BTest)
  (NS(RDftCore_EvenSizeTwiddleDft_B_4), 8);
}
#endif

#if C_SIMD_SIZE >= 8
void NS(RDftCore_EvenSizeTwiddleDft_B_8Test)() {
  NS(RDftCore_EvenSizeTwiddleDftFunction_BTest)
  (NS(RDftCore_EvenSizeTwiddleDft_B_8), 8);
}
#endif
