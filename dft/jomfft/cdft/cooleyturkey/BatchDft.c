#include "BatchDft.h"

#include "../../model/ComplexCopy.h"
#include "../../model/ComplexFunctionBuffered.h"
#include "../../model/ComplexFunctionBufferedLoop.h"
#include "../../model/ComplexFunctionLoop.h"
#include "../../model/ComplexFunctionPair.h"
#include "../../model/ComplexFunctionPairW.h"
#include "../../model/ComplexFunctionWLoop.h"
#include "../../model/ThreadingUtil.h"
#include "../BatchUtil.h"
#include "Dft.h"
#include "TransposeDft.h"

#define NS_(n) NS(CDftCooleyTurkey_##n)

static struct NS(ComplexFunction) *NS_(BatchDft_new_1_)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_1, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)(num_threads, batch_size, 1, dft_size, &num_threads_0,
                       &num_threads_1, true);
  return NS(ComplexFunctionLoop_new_)(
      NS_(Dft_new)(data, backward, dft_factors, x_stride_0, 1, false,
                   num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

static struct NS(ComplexFunction) *NS_(BatchDft_new_1)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_1, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long chunk_size =
      NS(CDft_BatchChunkSize)(dft_size, batch_size, x_stride_0, 1);
  if (chunk_size > 1 && num_threads <= batch_size / chunk_size) {
    if (batch_size <= chunk_size) {
      return NS_(TransposeDft_new)(data, backward, dft_factors, batch_size,
                                   x_stride_0, x_stride_1, y_stride_1);
    } else {
      const Long residual = batch_size % chunk_size;
      if (residual == 0) {
        return NS(ComplexFunctionLoop_new_)(
            NS_(TransposeDft_new)(data, backward, dft_factors, chunk_size,
                                  x_stride_0, x_stride_1, y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      } else {
        return NS(ComplexFunctionLoopR_new_)(
            NS_(TransposeDft_new)(data, backward, dft_factors, chunk_size,
                                  x_stride_0, x_stride_1, y_stride_1),
            NS_(TransposeDft_new)(data, backward, dft_factors, residual,
                                  x_stride_0, x_stride_1, y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      }
    }
  } else {
    return NS_(BatchDft_new_1_)(data, backward, dft_factors, batch_size,
                                x_stride_0, x_stride_1, y_stride_1,
                                num_threads);
  }
}

static struct NS(ComplexFunction) *NS_(BatchDft_new_2_)(
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
          NS_(Dft_new)(data, backward, dft_factors, x_stride_0, 1, false,
                       num_threads_1),
          NS(Complex_Copy_new)(dft_size, 1, y_stride_0),
          dft_size * sizeof(Complex)),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

static struct NS(ComplexFunctionW) *NS_(BatchDft_newW_2)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long dft_size,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1) {
  return NS(ComplexFunctionPairW_new)(
      NS_(TransposeDft_new)(data, backward, dft_factors, batch_size, x_stride_0,
                            x_stride_1, dft_size),
      NS(Complex_Copy2D_new)(dft_size, batch_size, 1, dft_size, y_stride_0,
                             y_stride_1),
      dft_size * batch_size * sizeof(Complex));
}

static struct NS(ComplexFunction) *NS_(BatchDft_new_2)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  const Long chunk_size =
      NS(CDft_BatchChunkSize)(dft_size, batch_size, x_stride_0, y_stride_0);
  if (chunk_size > 1 && num_threads <= batch_size / chunk_size) {
    if (batch_size <= chunk_size) {
      return NS(ComplexFunctionBuffered_new)(NS_(BatchDft_newW_2)(
          data, backward, dft_factors, dft_size, batch_size, x_stride_0,
          x_stride_1, y_stride_0, y_stride_1));
    } else {
      const Long residual = batch_size % chunk_size;
      if (residual == 0) {
        return NS(ComplexFunctionBufferedLoop_new_)(
            NS_(BatchDft_newW_2)(data, backward, dft_factors, dft_size,
                                 chunk_size, x_stride_0, x_stride_1, y_stride_0,
                                 y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      } else {
        return NS(ComplexFunctionBufferedLoopR_new_)(
            NS_(BatchDft_newW_2)(data, backward, dft_factors, dft_size,
                                 chunk_size, x_stride_0, x_stride_1, y_stride_0,
                                 y_stride_1),
            NS_(BatchDft_newW_2)(data, backward, dft_factors, dft_size,
                                 residual, x_stride_0, x_stride_1, y_stride_0,
                                 y_stride_1),
            batch_size / chunk_size, x_stride_1 * chunk_size,
            y_stride_1 * chunk_size, num_threads);
      }
    }
  } else {
    return NS_(BatchDft_new_2_)(data, backward, dft_factors, batch_size,
                                x_stride_0, x_stride_1, y_stride_0, y_stride_1,
                                num_threads);
  }
}

struct NS(ComplexFunction) *NS_(BatchDft_new)(
    struct NS(DataFactory) *data, bool backward,
    const struct jomfftPrimeFactors *dft_factors, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  if (batch_size == 1) {
    return NS_(Dft_new)(data, backward, dft_factors, x_stride_0, y_stride_0,
                        inplace, num_threads);
  } else {
    if (!inplace && y_stride_0 == 1) {
      return NS_(BatchDft_new_1)(data, backward, dft_factors, batch_size,
                                 x_stride_0, x_stride_1, y_stride_1,
                                 num_threads);
    } else {
      return NS_(BatchDft_new_2)(data, backward, dft_factors, batch_size,
                                 x_stride_0, x_stride_1, y_stride_0, y_stride_1,
                                 num_threads);
    }
  }
}

bool NS_(UseChunkedBatch)(const struct jomfftPrimeFactors *dft_factors,
                          Long batch_size, Long x_stride_0, Long y_stride_0) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  return NS(CDft_BatchChunkSize)(dft_size, batch_size, x_stride_0, y_stride_0) >
         1;
}
