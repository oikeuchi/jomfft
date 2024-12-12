#include "EvenSizeDft.h"

#include "../../cdft/cooleyturkey/Dft.h"
#include "../../model/ComplexCopy.h"
#include "../../model/ComplexToRealFunctionBuffered.h"
#include "../../model/ComplexToRealFunctionPairW.h"
#include "../../model/ComplexToRealFunctionTripleW.h"
#include "../../model/RealCopy.h"
#include "../../model/RealToComplexFunctionBuffered.h"
#include "../../model/RealToComplexFunctionPairW.h"
#include "../../model/RealToComplexFunctionTripleW.h"
#include "../EvenSizeDft.h"

#define NS_(n) NS(RDftCooleyTurkey_##n)

struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(RDft_EvenSizeDft_F_new)(
      data,
      NS(CDftCooleyTurkey_Dft_new)(data, false, dft_factors, 1, 1, false,
                                   num_threads),
      dft_size, num_threads);
}

struct NS(RealToComplexFunctionW) *NS_(EvenSizeDft_F_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long x_size = 2 * dft_size;
  const Long y_size = CE_SIZE(2 * dft_size);
  if (y_stride == 1) {
    return NS(RealToComplexFunctionPairW_0_new)(
        NS(Real_Copy_new_)(x_size, x_stride, 1, num_threads),
        NS_(EvenSizeDft_F_new_1)(data, dft_factors, num_threads),
        x_size * sizeof(Real));
  } else {
    return NS(RealToComplexFunctionTripleDW_1_new)(
        NS(Real_Copy_new_)(x_size, x_stride, 1, num_threads),
        NS_(EvenSizeDft_F_new_1)(data, dft_factors, num_threads),
        NS(Complex_Copy_new_)(y_size, 1, y_stride, num_threads),
        x_size * sizeof(Real), y_size * sizeof(Complex));
  }
}

struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  if (!inplace && x_stride == 1 && y_stride == 1) {
    return NS_(EvenSizeDft_F_new_1)(data, dft_factors, num_threads);
  } else {
    return NS(RealToComplexFunctionBuffered_new)(NS_(EvenSizeDft_F_newW)(
        data, dft_factors, x_stride, y_stride, num_threads));
  }
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(RDft_EvenSizeDft_B_new)(
      data,
      NS(CDftCooleyTurkey_Dft_new)(data, true, dft_factors, 1, 1, false,
                                   num_threads),
      dft_size, num_threads);
}

struct NS(ComplexToRealFunctionW) *NS_(EvenSizeDft_B_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long x_size = CE_SIZE(2 * dft_size);
  const Long y_size = 2 * dft_size;
  if (x_stride == 1) {
    return NS(ComplexToRealFunctionPairW_1_new)(
        NS_(EvenSizeDft_B_new_1)(data, dft_factors, num_threads),
        NS(Real_Copy_new_)(y_size, 1, y_stride, num_threads),
        y_size * sizeof(Real));
  } else {
    return NS(ComplexToRealFunctionTripleDW_1_new)(
        NS(Complex_Copy_new_)(x_size, x_stride, 1, num_threads),
        NS_(EvenSizeDft_B_new_1)(data, dft_factors, num_threads),
        NS(Real_Copy_new_)(y_size, 1, y_stride, num_threads),
        x_size * sizeof(Complex), y_size * sizeof(Real));
  }
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  if (!inplace && x_stride == 1 && y_stride == 1) {
    return NS_(EvenSizeDft_B_new_1)(data, dft_factors, num_threads);
  } else {
    return NS(ComplexToRealFunctionBuffered_new)(NS_(EvenSizeDft_B_newW)(
        data, dft_factors, x_stride, y_stride, num_threads));
  }
}
