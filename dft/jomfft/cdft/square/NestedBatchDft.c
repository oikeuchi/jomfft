#include "NestedBatchDft.h"

#include "../../model/ComplexCopy.h"
#include "../../model/ComplexFunctionBuffered.h"
#include "../../model/ComplexFunctionBufferedLoop.h"
#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../../model/ComplexFunctionPairW.h"
#include "../../model/ThreadingUtil.h"
#include "../cooleyturkey/BatchDft.h"
#include "NestedDft.h"
#include "SmallBatchDft.h"
#include "SmallTransposeDft.h"

#define NS_(n) NS(CDftSquare_##n)

bool NS_(NestedBatchDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors) {
  return NS_(NestedDft_CanSplit)(dft_factors);
}

static struct NS(ComplexFunction) *NS_(NestedBatchDft_new_1)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_1, bool inplace,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)(num_threads, batch_size, 1, dft_size, &num_threads_0,
                       &num_threads_1, true);
  return NS(ComplexFunctionLoop_new_)(
      NS_(NestedDft_new)(data, backward, dft_factors, x_stride_0, 1, inplace,
                         num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

static struct NS(ComplexFunction) *NS_(NestedBatchDft_new_2)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)(num_threads, batch_size, 1, dft_size, &num_threads_0,
                       &num_threads_1, true);
  return NS(ComplexFunctionBufferedLoop_new_)(
      NS(ComplexFunctionPairW_new)(
          NS_(NestedDft_new)(data, backward, dft_factors, x_stride_0, 1, false,
                             num_threads_1),
          NS(Complex_Copy_new_)(dft_size, 1, y_stride_0, num_threads_1),
          dft_size * sizeof(Complex)),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

struct NS(ComplexFunction) *NS_(NestedBatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  if (batch_size == 1) {
    return NS_(NestedDft_new)(data, backward, dft_factors, x_stride_0,
                              y_stride_0, inplace, num_threads);
  } else {
    if ((x_stride_0 != 1 || y_stride_0 != 1) &&
        NS(CDftCooleyTurkey_UseChunkedBatch)(dft_factors, batch_size,
                                             x_stride_0, 1)) {
      if (NS_(SmallTransposeDft_CanSplit)(dft_factors)) {
        return NS_(SmallTransposeBatchDft_new)(
            data, backward, dft_factors, batch_size, x_stride_0, x_stride_1,
            y_stride_0, y_stride_1, inplace, num_threads);
      } else {
        return NS(CDftCooleyTurkey_BatchDft_new)(
            data, backward, dft_factors, batch_size, x_stride_0, x_stride_1,
            y_stride_0, y_stride_1, inplace, num_threads);
      }
    } else {
      if (y_stride_0 == 1) {
        return NS_(NestedBatchDft_new_1)(data, backward, dft_factors,
                                         batch_size, x_stride_0, x_stride_1,
                                         y_stride_1, inplace, num_threads);
      } else {
        return NS_(NestedBatchDft_new_2)(data, backward, dft_factors,
                                         batch_size, x_stride_0, x_stride_1,
                                         y_stride_0, y_stride_1, num_threads);
      }
    }
  }
}
