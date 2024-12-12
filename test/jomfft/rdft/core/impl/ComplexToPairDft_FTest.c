#include "ComplexToPairDft_FTest.h"

#include <jomfft/rdft/core/impl/ComplexToPairDft_F.h>

#include "../ComplexToPairDftFunctionTest.h"

#if C_SIMD_SIZE_S >= 1
void NS(RDftCore_ComplexToPairDft_F_1Test)() {
  NS(RDftCore_ComplexToPairDftFunction_FTest)
  (NS(RDftCore_ComplexToPairDft_F_1), 17);
}
#endif

#if C_SIMD_SIZE_S >= 2
void NS(RDftCore_ComplexToPairDft_F_2Test)() {
  NS(RDftCore_ComplexToPairDftFunction_FTest)
  (NS(RDftCore_ComplexToPairDft_F_2), 17);
}
#endif

#if C_SIMD_SIZE_S >= 4
void NS(RDftCore_ComplexToPairDft_F_4Test)() {
  NS(RDftCore_ComplexToPairDftFunction_FTest)
  (NS(RDftCore_ComplexToPairDft_F_4), 17);
}
#endif
