#include "EvenSizeDft.h"

#include "../../cdft/square/Dft.h"
#include "../../model/ComplexCopy.h"
#include "../../model/ComplexToRealFunctionBuffered.h"
#include "../../model/ComplexToRealFunctionPair.h"
#include "../../model/ComplexToRealFunctionTripleW.h"
#include "../../model/RealCopy.h"
#include "../../model/RealToComplexFunctionBuffered.h"
#include "../../model/RealToComplexFunctionPair.h"
#include "../../model/RealToComplexFunctionTripleW.h"
#include "../EvenSizeDft.h"

#define NS_(n) NS(RDftSquare_##n)

bool NS_(EvenSizeDft_CanSplit)(const struct jomfftPrimeFactors *dft_factors,
                               int num_threads) {
  return NS(CDftSquare_Dft_CanSplit)(dft_factors, num_threads);
}

struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    bool inplace, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(RDft_EvenSizeDft_F_new)(
      data,
      NS(CDftSquare_Dft_new)(data, false, dft_factors, 1, 1, inplace,
                             num_threads),
      dft_size, num_threads);
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new_1_)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long x_size = 2 * dft_size;
  return NS(RealToComplexFunctionPair_C1_new)(
      NS(Real_Copy_new)(x_size, x_stride, 1),
      NS_(EvenSizeDft_F_new_1)(data, dft_factors, true, num_threads));
}

struct NS(RealToComplexFunctionW) *NS_(EvenSizeDft_F_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long x_size = 2 * dft_size;
  const Long y_size = CE_SIZE(2 * dft_size);
  return NS(RealToComplexFunctionTripleW_new)(
      NS(Real_Copy_new)(x_size, x_stride, 1),
      NS_(EvenSizeDft_F_new_1)(data, dft_factors, true, num_threads),
      NS(Complex_Copy_new)(y_size, 1, y_stride), y_size * sizeof(Complex));
}

struct NS(RealToComplexFunction) *NS_(EvenSizeDft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  if (x_stride == 1 && y_stride == 1) {
    return NS_(EvenSizeDft_F_new_1)(data, dft_factors, inplace, num_threads);
  } else if (!inplace && y_stride == 1) {
    return NS_(EvenSizeDft_F_new_1_)(data, dft_factors, x_stride, num_threads);
  } else {
    return NS(RealToComplexFunctionBuffered_new)(NS_(EvenSizeDft_F_newW)(
        data, dft_factors, x_stride, y_stride, num_threads));
  }
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    bool inplace, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(RDft_EvenSizeDft_B_new)(
      data,
      NS(CDftSquare_Dft_new)(data, true, dft_factors, 1, 1, inplace,
                             num_threads),
      dft_size, num_threads);
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new_1_)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long y_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long y_size = 2 * dft_size;
  return NS(ComplexToRealFunctionPair_C0_new)(
      NS_(EvenSizeDft_B_new_1)(data, dft_factors, true, num_threads),
      NS(Real_Copy_new)(y_size, 1, y_stride));
}

struct NS(ComplexToRealFunctionW) *NS_(EvenSizeDft_B_newW)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long x_size = CE_SIZE(2 * dft_size);
  const Long y_size = 2 * dft_size;
  return NS(ComplexToRealFunctionTripleW_new)(
      NS(Complex_Copy_new)(x_size, x_stride, 1),
      NS_(EvenSizeDft_B_new_1)(data, dft_factors, true, num_threads),
      NS(Real_Copy_new)(y_size, 1, y_stride), x_size * sizeof(Complex));
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeDft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  if (x_stride == 1 && y_stride == 1) {
    return NS_(EvenSizeDft_B_new_1)(data, dft_factors, inplace, num_threads);
  } else if (!inplace && x_stride == 1) {
    return NS_(EvenSizeDft_B_new_1_)(data, dft_factors, y_stride, num_threads);
  } else {
    return NS(ComplexToRealFunctionBuffered_new)(NS_(EvenSizeDft_B_newW)(
        data, dft_factors, x_stride, y_stride, num_threads));
  }
}
