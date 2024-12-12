#include "Dft.h"
#include "../../cdft/cooleyturkey/Dft.h"
#include "../../model/ComplexCopy.h"
#include "../../model/ComplexToRealFunctionBuffered.h"
#include "../../model/ComplexToRealFunctionTripleW.h"
#include "../../model/ConjugateEvenCopy.h"
#include "../../model/RealCopy.h"
#include "../../model/RealToComplexFunctionBuffered.h"
#include "../../model/RealToComplexFunctionTripleW.h"
#include "../../model/SplitComplexCopy.h"

#define NS_(n) NS(RDftCooleyTurkey_##n)

struct NS(RealToComplexFunctionW) *NS_(Dft_F_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(RealToComplexFunctionTripleDW_0_new)(
      NS(SplitComplex_CopyReal_F_new)(dft_size, x_stride, 1, num_threads),
      NS(CDftCooleyTurkey_Dft_new)(data, false, dft_factors, 1, 1, false,
                                   num_threads),
      NS(Complex_Copy_new_)(CE_SIZE(dft_size), 1, y_stride, num_threads),
      dft_size * sizeof(Complex));
}

struct NS(ComplexToRealFunctionW) *NS_(Dft_B_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(ComplexToRealFunctionTripleDW_2_new)(
      NS(Complex_CopyCE_new)(dft_size, x_stride, 1, num_threads),
      NS(CDftCooleyTurkey_Dft_new)(data, true, dft_factors, 1, 1, false,
                                   num_threads),
      NS(SplitComplex_CopyReal_B_new)(dft_size, 1, y_stride, num_threads),
      dft_size * sizeof(Complex));
}

struct NS(RealToComplexFunction) *NS_(Dft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  struct NS(RealToComplexFunction) *out = NS(RealToComplexFunctionBuffered_new)(
      NS_(Dft_F_newW)(data, dft_factors, x_stride, y_stride, num_threads));
  return out;
}

struct NS(ComplexToRealFunction) *NS_(Dft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  struct NS(ComplexToRealFunction) *out = NS(ComplexToRealFunctionBuffered_new)(
      NS_(Dft_B_newW)(data, dft_factors, x_stride, y_stride, num_threads));
  return out;
}
