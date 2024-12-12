#include "EvenSizeBatchDft.h"

#include "../../model/ComplexToRealFunctionBufferedLoop.h"
#include "../../model/ComplexToRealFunctionLoop.h"
#include "../../model/RealToComplexFunctionBufferedLoop.h"
#include "../../model/RealToComplexFunctionLoop.h"
#include "../../model/ThreadingUtil.h"
#include "EvenSizeDft.h"

#define NS_(n) NS(RDftSquare_##n)

bool NS_(EvenSizeBatchDft_CanSplit)(
    const struct jomfftPrimeFactors *dft_factors, int num_threads) {
  return NS_(EvenSizeDft_CanSplit)(dft_factors, num_threads);
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeOuterBatchDft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_1, Long y_stride_1, bool inplace,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)
  (num_threads, batch_size, 1, dft_size, &num_threads_0, &num_threads_1, true);
  return NS(RealToComplexFunctionLoop_new_)(
      NS_(EvenSizeDft_F_new)(data, dft_factors, 1, 1, inplace, num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeBatchDft_F_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_1,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)
  (num_threads, batch_size, 1, dft_size, &num_threads_0, &num_threads_1, true);
  return NS(RealToComplexFunctionLoop_new_)(
      NS_(EvenSizeDft_F_new)(data, dft_factors, x_stride_0, 1, false,
                             num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeBatchDft_F_new_2)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)
  (num_threads, batch_size, 1, dft_size, &num_threads_0, &num_threads_1, true);
  return NS(RealToComplexFunctionBufferedLoop_new_)(
      NS_(EvenSizeDft_F_newW)(data, dft_factors, x_stride_0, y_stride_0,
                              num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

struct NS(RealToComplexFunction) *NS_(EvenSizeBatchDft_F_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, bool inplace, int num_threads) {
  if (batch_size == 1) {
    return NS_(EvenSizeDft_F_new)(data, dft_factors, x_stride_0, y_stride_0,
                                  inplace, num_threads);
  } else if (x_stride_0 == 1 && y_stride_0 == 1) {
    return NS_(EvenSizeOuterBatchDft_F_new)(data, dft_factors, batch_size,
                                            x_stride_1, y_stride_1, inplace,
                                            num_threads);
  } else if (!inplace && y_stride_0 == 1) {
    return NS_(EvenSizeBatchDft_F_new_1)(data, dft_factors, batch_size,
                                         x_stride_0, x_stride_1, y_stride_1,
                                         num_threads);
  } else {
    return NS_(EvenSizeBatchDft_F_new_2)(data, dft_factors, batch_size,
                                         x_stride_0, x_stride_1, y_stride_0,
                                         y_stride_1, num_threads);
  }
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeOuterBatchDft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_1, Long y_stride_1, bool inplace,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)
  (num_threads, batch_size, 1, dft_size, &num_threads_0, &num_threads_1, true);
  return NS(ComplexToRealFunctionLoop_new_)(
      NS_(EvenSizeDft_B_new)(data, dft_factors, 1, 1, inplace, num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeBatchDft_B_new_1)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)
  (num_threads, batch_size, 1, dft_size, &num_threads_0, &num_threads_1, true);
  return NS(ComplexToRealFunctionLoop_new_)(
      NS_(EvenSizeDft_B_new)(data, dft_factors, 1, y_stride_0, false,
                             num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeBatchDft_B_new_2)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
  const Long dft_size = jomfftPrimeFactors_productLong(dft_factors);
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)
  (num_threads, batch_size, 1, dft_size, &num_threads_0, &num_threads_1, true);
  return NS(ComplexToRealFunctionBufferedLoop_new_)(
      NS_(EvenSizeDft_B_newW)(data, dft_factors, x_stride_0, y_stride_0,
                              num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeBatchDft_B_new)(
    struct NS(DataFactory) *data, const struct jomfftPrimeFactors *dft_factors,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, bool inplace, int num_threads) {
  if (batch_size == 1) {
    return NS_(EvenSizeDft_B_new)(data, dft_factors, x_stride_0, y_stride_0,
                                  inplace, num_threads);
  } else if (x_stride_0 == 1 && y_stride_0 == 1) {
    return NS_(EvenSizeOuterBatchDft_B_new)(data, dft_factors, batch_size,
                                            x_stride_1, y_stride_1, inplace,
                                            num_threads);
  } else if (!inplace && x_stride_0 == 1) {
    return NS_(EvenSizeBatchDft_B_new_1)(data, dft_factors, batch_size,
                                         x_stride_1, y_stride_0, y_stride_1,
                                         num_threads);
  } else {
    return NS_(EvenSizeBatchDft_B_new_2)(data, dft_factors, batch_size,
                                         x_stride_0, x_stride_1, y_stride_0,
                                         y_stride_1, num_threads);
  }
}
