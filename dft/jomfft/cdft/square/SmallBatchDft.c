#include "SmallBatchDft.h"

#include "../../model/ComplexCopy.h"
#include "../../model/ComplexFunctionBuffered.h"
#include "../../model/ComplexFunctionBufferedLoop.h"
#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../../model/ComplexFunctionPairW.h"
#include "../../model/ComplexFunctionWLoop.h"
#include "../../model/ThreadingUtil.h"
#include "../BatchUtil.h"
#include "../cooleyturkey/BatchDft.h"
#include "SmallDft.h"
#include "SmallTransposeDft.h"

#define NS_(n) NS(CDftSquare_##n)

bool NS_(SmallBatchDft_CanSplit)(const struct jomfftPrimeFactors *dft_factors) {
  return NS_(SmallDft_CanSplit)(dft_factors);
}

static struct NS(ComplexFunction) *NS_(SmallOuterBatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_1, Long y_stride_1, int num_threads) {
  return NS(ComplexFunctionLoop_new_)(
      NS_(SmallDft_new)(data, backward, dft_factors, 1, 1, true), batch_size,
      x_stride_1, y_stride_1, num_threads);
}

static struct NS(ComplexFunction) *NS_(SmallBatchDft_new_0)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(ComplexFunctionLoop_new_)(
      NS(ComplexFunctionPair_0_new)(
          NS_(SmallDft_new)(data, backward, dft_factors, 1, 1, true),
          NS(Complex_Copy_new)(dft_size, 1, y_stride_0)),
      batch_size, x_stride_1, y_stride_1, num_threads);
}

static struct NS(ComplexFunction) *NS_(SmallBatchDft_new_1)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_1, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long chunk_size =
      NS(CDft_BatchChunkSize)(dft_size, batch_size, x_stride_0, 1);
  if (chunk_size > 1 && num_threads <= batch_size / chunk_size) {
    if (batch_size <= chunk_size) {
      return NS_(SmallTransposeDft_new)(data, backward, dft_factors, batch_size,
                                        x_stride_0, x_stride_1, y_stride_1);
    } else {
      const Long residual = batch_size % chunk_size;
      if (residual == 0) {
        return NS(ComplexFunctionLoop_new_)(
            NS_(SmallTransposeDft_new)(data, backward, dft_factors, chunk_size,
                                       x_stride_0, x_stride_1, y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      } else {
        return NS(ComplexFunctionLoopR_new_)(
            NS_(SmallTransposeDft_new)(data, backward, dft_factors, chunk_size,
                                       x_stride_0, x_stride_1, y_stride_1),
            NS_(SmallTransposeDft_new)(data, backward, dft_factors, residual,
                                       x_stride_0, x_stride_1, y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      }
    }
  } else {
    return NS(CDftCooleyTurkey_BatchDft_new)(data, backward, dft_factors,
                                             batch_size, x_stride_0, x_stride_1,
                                             1, y_stride_1, false, num_threads);
  }
}

static struct NS(ComplexFunctionW) *NS_(SmallBatchDft_newW_2)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long dft_size,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1) {
  return NS(ComplexFunctionPairW_new)(
      NS_(SmallTransposeDft_new)(data, backward, dft_factors, batch_size,
                                 x_stride_0, x_stride_1, dft_size),
      NS(Complex_Copy2D_new)(dft_size, batch_size, 1, dft_size, y_stride_0,
                             y_stride_1),
      dft_size * batch_size * sizeof(Complex));
}

static struct NS(ComplexFunction) *NS_(SmallBatchDft_new_2)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long chunk_size =
      NS(CDft_BatchChunkSize)(dft_size, batch_size, x_stride_0, y_stride_0);
  if (chunk_size > 1 && num_threads <= batch_size / chunk_size) {
    if (batch_size <= chunk_size) {
      return NS(ComplexFunctionBuffered_new)(NS_(SmallBatchDft_newW_2)(
          data, backward, dft_factors, dft_size, batch_size, x_stride_0,
          x_stride_1, y_stride_0, y_stride_1));
    } else {
      const Long residual = batch_size % chunk_size;
      if (residual == 0) {
        return NS(ComplexFunctionBufferedLoop_new_)(
            NS_(SmallBatchDft_newW_2)(data, backward, dft_factors, dft_size,
                                      chunk_size, x_stride_0, x_stride_1,
                                      y_stride_0, y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      } else {
        return NS(ComplexFunctionBufferedLoopR_new_)(
            NS_(SmallBatchDft_newW_2)(data, backward, dft_factors, dft_size,
                                      chunk_size, x_stride_0, x_stride_1,
                                      y_stride_0, y_stride_1),
            NS_(SmallBatchDft_newW_2)(data, backward, dft_factors, dft_size,
                                      residual, x_stride_0, x_stride_1,
                                      y_stride_0, y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      }
    }
  } else {
    return NS(CDftCooleyTurkey_BatchDft_new)(
        data, backward, dft_factors, batch_size, x_stride_0, x_stride_1,
        y_stride_0, y_stride_1, false, num_threads);
  }
}

struct NS(ComplexFunction) *NS_(SmallTransposeBatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  assert(NS_(SmallTransposeDft_CanSplit)(dft_factors));
  if (!inplace && y_stride_0 == 1) {
    return NS_(SmallBatchDft_new_1)(data, backward, dft_factors, batch_size,
                                    x_stride_0, x_stride_1, y_stride_1,
                                    num_threads);
  } else {
    return NS_(SmallBatchDft_new_2)(data, backward, dft_factors, batch_size,
                                    x_stride_0, x_stride_1, y_stride_0,
                                    y_stride_1, num_threads);
  }
}

struct NS(ComplexFunction) *NS_(SmallBatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  if (batch_size == 1) {
    return NS_(SmallDft_new)(data, backward, dft_factors, x_stride_0,
                             y_stride_0, inplace);
  } else if (inplace && x_stride_0 == 1 && y_stride_0 == 1) {
    return NS_(SmallOuterBatchDft_new)(data, backward, dft_factors, batch_size,
                                       x_stride_1, y_stride_1, num_threads);
  } else if (!inplace && x_stride_0 == 1 && y_stride_0 != 1) {
    return NS_(SmallBatchDft_new_0)(data, backward, dft_factors, batch_size,
                                    x_stride_1, y_stride_0, y_stride_1,
                                    num_threads);
  } else {
    if (NS_(SmallTransposeDft_CanSplit)(dft_factors)) {
      return NS_(SmallTransposeBatchDft_new)(
          data, backward, dft_factors, batch_size, x_stride_0, x_stride_1,
          y_stride_0, y_stride_1, inplace, num_threads);
    } else {
      return NS(CDftCooleyTurkey_BatchDft_new)(
          data, backward, dft_factors, batch_size, x_stride_0, x_stride_1,
          y_stride_0, y_stride_1, inplace, num_threads);
    }
  }
}
