#include "BatchDft.h"

#include <jomfft/math/IntegerMath.h>

#include "../../model/ComplexFunctionLoop.h"
#include "Dft.h"
#include "LargeBatchDft.h"
#include "NestedBatchDft.h"
#include "SmallBatchDft.h"

#define DFT_SIXSTEP_MIN_SIZE (8 * C_L3_CACHE_SIZE)

#define NS_(n) NS(CDftSquare_##n)

bool NS_(BatchDft_CanSplit)(const struct jomfftPrimeFactors *dft_factors,
                            int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  if (dft_size >= DFT_SIXSTEP_MIN_SIZE / num_threads &&
      NS_(LargeBatchDft_CanSplit)(dft_factors)) {
    return true;
  }
  return NS_(NestedBatchDft_CanSplit)(dft_factors);
}

struct NS(ComplexFunction) *NS_(BatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  if (NS_(SmallBatchDft_CanSplit)(dft_factors)) {
    return NS_(SmallBatchDft_new)(data, backward, dft_factors, batch_size,
                                  x_stride_0, x_stride_1, y_stride_0,
                                  y_stride_1, inplace, num_threads);
  }
  if (dft_size >= DFT_SIXSTEP_MIN_SIZE / num_threads &&
      NS_(LargeBatchDft_CanSplit)(dft_factors)) {
    return NS_(LargeBatchDft_new)(data, backward, dft_factors, batch_size,
                                  x_stride_0, x_stride_1, y_stride_0,
                                  y_stride_1, inplace, num_threads);
  }
  return NS_(NestedBatchDft_new)(data, backward, dft_factors, batch_size,
                                 x_stride_0, x_stride_1, y_stride_0, y_stride_1,
                                 inplace, num_threads);
}
