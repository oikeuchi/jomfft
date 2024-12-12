#include "ComplexToPairDft_BTest.h"

#include <jomfft/rdft/core/impl/ComplexToPairDft_B.h>

#include "../ComplexToPairDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(RDftCore_ComplexToPairDft_B_1Test)() {
  NS(RDftCore_ComplexToPairDftFunction_BTest)
  (NS(RDftCore_ComplexToPairDft_B_1), 17);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(RDftCore_ComplexToPairDft_B_2Test)() {
  NS(RDftCore_ComplexToPairDftFunction_BTest)
  (NS(RDftCore_ComplexToPairDft_B_2), 17);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(RDftCore_ComplexToPairDft_B_4Test)() {
  NS(RDftCore_ComplexToPairDftFunction_BTest)
  (NS(RDftCore_ComplexToPairDft_B_4), 17);
}
#endif
