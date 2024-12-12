#include "Dft.h"

#include "LargeDft.h"
#include "NestedDft.h"
#include "SmallDft.h"

#define DFT_SIXSTEP_MIN_SIZE (8 * C_L3_CACHE_SIZE)

#define NS_(n) NS(CDftSquare_##n)

bool NS_(Dft_CanSplit)(const struct jomfftPrimeFactors *dft_factors,
                       int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  if (dft_size >= DFT_SIXSTEP_MIN_SIZE / num_threads &&
      NS_(LargeDft_CanSplit)(dft_factors)) {
    return true;
  }
  return NS_(NestedDft_CanSplit)(dft_factors);
}

struct NS(ComplexFunction) *NS_(Dft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long x_stride, Long y_stride,
    bool inplace, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  if (NS_(SmallDft_CanSplit)(dft_factors)) {
    return NS_(SmallDft_new)(data, backward, dft_factors, x_stride, y_stride,
                             inplace);
  }
  if (dft_size >= DFT_SIXSTEP_MIN_SIZE / num_threads &&
      NS_(LargeDft_CanSplit)(dft_factors)) {
    return NS_(LargeDft_new)(data, backward, dft_factors, x_stride, y_stride,
                             inplace, num_threads);
  }
  return NS_(NestedDft_new)(data, backward, dft_factors, x_stride, y_stride,
                            inplace, num_threads);
}
