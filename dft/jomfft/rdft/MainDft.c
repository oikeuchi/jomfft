#include "MainDft.h"

#include "../cdft/MainDft.h"
#include "../model/ComplexToRealFunctionLoop.h"
#include "../model/ComplexToRealFunctionPair.h"
#include "../model/RealToComplexFunctionLoop.h"
#include "../model/RealToComplexFunctionPair.h"
#include "../model/ThreadingUtil.h"
#include "BatchDft.h"
#include "Dft.h"
#include <jomfft/util/LongUtil.h>

#define NS_(n) NS(RDft_##n)

static struct NS(RealToComplexFunction) *NS_(MainBatchDft_F_new_)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    const Long *data_sizes, const Long *x_strides, const Long *y_strides,
    bool inplace, int num_threads) {
  assert(dimension >= 1);
  if (dimension == 1) {
    return NS_(BatchDft_F_new)(data, dft_sizes[0], data_sizes[1], x_strides[0],
                               x_strides[1], y_strides[0], y_strides[1],
                               inplace, num_threads);
  } else {
    const Long batch_size_0 = data_sizes[0];
    const Long batch_size_1 =
        jomfftLong_Product(dimension - 2, data_sizes + 1) *
        data_sizes[dimension];
    const Long dft_size_1 = dft_sizes[dimension - 1];
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split)
    (num_threads, batch_size_0, batch_size_1, dft_size_1, &num_threads_0,
     &num_threads_1);
    return NS(RealToComplexFunctionPair_1_new)(
        NS(RealToComplexFunctionLoop_new_)(
            NS_(MainBatchDft_F_new_)(data, dimension - 1, dft_sizes + 1,
                                     data_sizes + 1, x_strides + 1,
                                     y_strides + 1, inplace, num_threads_1),
            data_sizes[0], x_strides[0], y_strides[0], num_threads_0),
        NS(CDft_BatchDftLoop_new)(data, false, dft_sizes[0], y_strides[0],
                                  dimension, data_sizes + 1, y_strides + 1,
                                  num_threads));
  }
}

static struct NS(RealToComplexFunction) *NS_(MainDft_F_new_)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    const Long *data_sizes, const Long *x_strides, const Long *y_strides,
    bool inplace, int num_threads) {
  assert(dimension >= 1);
  if (dimension == 1) {
    return NS_(Dft_F_new)(data, dft_sizes[0], x_strides[0], y_strides[0],
                          inplace, num_threads);
  } else if (dimension == 2) {
    return NS(RealToComplexFunctionPair_1_new)(
        NS_(BatchDft_F_new)(data, dft_sizes[1], data_sizes[0], x_strides[1],
                            x_strides[0], y_strides[1], y_strides[0], inplace,
                            num_threads),
        NS(CDft_BatchDftLoop_new)(data, false, dft_sizes[0], y_strides[0],
                                  dimension - 1, data_sizes + 1, y_strides + 1,
                                  num_threads));
  } else {
    const Long batch_size_0 = data_sizes[0];
    const Long batch_size_1 = jomfftLong_Product(dimension - 2, data_sizes + 1);
    const Long dft_size_1 = dft_sizes[dimension - 1];
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split)
    (num_threads, batch_size_0, batch_size_1, dft_size_1, &num_threads_0,
     &num_threads_1);
    return NS(RealToComplexFunctionPair_1_new)(
        NS(RealToComplexFunctionLoop_new_)(
            NS_(MainDft_F_new_)(data, dimension - 1, dft_sizes + 1,
                                data_sizes + 1, x_strides + 1, y_strides + 1,
                                inplace, num_threads_1),
            data_sizes[0], x_strides[0], y_strides[0], num_threads_0),
        NS(CDft_BatchDftLoop_new)(data, false, dft_sizes[0], y_strides[0],
                                  dimension - 1, data_sizes + 1, y_strides + 1,
                                  num_threads));
  }
}

struct NS(RealToComplexFunction) *NS_(MainDft_F_new)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    Long batch_size, const Long *x_strides, Long x_distance,
    const Long *y_strides, Long y_distance, bool inplace, int num_threads) {
  assert(dimension >= 1);
  assert(batch_size >= 1);
  struct NS(RealToComplexFunction) *out = NULL;
  Long *data_sizes = ARRAY_NEW(Long, dimension);
  ARRAY_COPY(Long, dimension, dft_sizes, data_sizes);
  data_sizes[dimension - 1] = CE_SIZE(dft_sizes[dimension - 1]);
  if (batch_size == 1) {
    out = NS_(MainDft_F_new_)(data, dimension, dft_sizes, data_sizes, x_strides,
                              y_strides, inplace, num_threads);
  } else {
    if (dimension == 1) {
      out = NS_(BatchDft_F_new)(data, dft_sizes[0], batch_size, x_strides[0],
                                x_distance, y_strides[0], y_distance, inplace,
                                num_threads);
    } else {
      if (x_distance == 1 && y_distance == 1) {
        Long *data_sizes_ = ARRAY_NEW(Long, dimension + 1);
        Long *x_strides_ = ARRAY_NEW(Long, dimension + 1);
        Long *y_strides_ = ARRAY_NEW(Long, dimension + 1);
        ARRAY_COPY(Long, dimension, data_sizes, data_sizes_);
        ARRAY_COPY(Long, dimension, x_strides, x_strides_);
        ARRAY_COPY(Long, dimension, y_strides, y_strides_);
        data_sizes_[dimension] = batch_size;
        x_strides_[dimension] = x_distance;
        y_strides_[dimension] = y_distance;
        out = NS_(MainBatchDft_F_new_)(data, dimension, dft_sizes, data_sizes_,
                                       x_strides_, y_strides_, inplace,
                                       num_threads);
        ARRAY_DELETE(data_sizes_);
        ARRAY_DELETE(x_strides_);
        ARRAY_DELETE(y_strides_);
      } else {
        const Long batch_size_0 = batch_size;
        const Long batch_size_1 = jomfftLong_Product(dimension - 1, data_sizes);
        const Long dft_size_1 = dft_sizes[dimension - 1];
        int num_threads_0 = 1;
        int num_threads_1 = 1;
        NS(Threading_Split)
        (num_threads, batch_size_0, batch_size_1, dft_size_1, &num_threads_0,
         &num_threads_1);
        out = NS(RealToComplexFunctionLoop_new_)(
            NS_(MainDft_F_new_)(data, dimension, dft_sizes, data_sizes,
                                x_strides, y_strides, inplace, num_threads_1),
            batch_size, x_distance, y_distance, num_threads_0);
      }
    }
  }
  ARRAY_DELETE(data_sizes);
  return out;
}

static struct NS(ComplexToRealFunction) *NS_(MainBatchDft_B_new_)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    const Long *data_sizes, const Long *x_strides, const Long *y_strides,
    bool inplace, int num_threads) {
  assert(dimension >= 1);
  if (dimension == 1) {
    return NS_(BatchDft_B_new)(data, dft_sizes[0], data_sizes[1], x_strides[0],
                               x_strides[1], y_strides[0], y_strides[1],
                               inplace, num_threads);
  } else {
    const Long batch_size_0 = data_sizes[0];
    const Long batch_size_1 =
        jomfftLong_Product(dimension - 2, data_sizes + 1) *
        data_sizes[dimension];
    const Long dft_size_1 = dft_sizes[dimension - 1];
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split)
    (num_threads, batch_size_0, batch_size_1, dft_size_1, &num_threads_0,
     &num_threads_1);
    return NS(ComplexToRealFunctionPair_0_new)(
        NS(CDft_BatchDftLoop_new)(data, true, dft_sizes[0], x_strides[0],
                                  dimension, data_sizes + 1, x_strides + 1,
                                  num_threads),
        NS(ComplexToRealFunctionLoop_new_)(
            NS_(MainBatchDft_B_new_)(data, dimension - 1, dft_sizes + 1,
                                     data_sizes + 1, x_strides + 1,
                                     y_strides + 1, inplace, num_threads_1),
            data_sizes[0], x_strides[0], y_strides[0], num_threads_0));
  }
}

static struct NS(ComplexToRealFunction) *NS_(MainDft_B_new_)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    const Long *data_sizes, const Long *x_strides, const Long *y_strides,
    bool inplace, int num_threads) {
  assert(dimension >= 1);
  if (dimension == 1) {
    return NS_(Dft_B_new)(data, dft_sizes[0], x_strides[0], y_strides[0],
                          inplace, num_threads);
  } else if (dimension == 2) {
    return NS(ComplexToRealFunctionPair_0_new)(
        NS(CDft_BatchDftLoop_new)(data, true, dft_sizes[0], x_strides[0],
                                  dimension - 1, data_sizes + 1, x_strides + 1,
                                  num_threads),
        NS_(BatchDft_B_new)(data, dft_sizes[1], data_sizes[0], x_strides[1],
                            x_strides[0], y_strides[1], y_strides[0], inplace,
                            num_threads));
  } else {
    const Long batch_size_0 = data_sizes[0];
    const Long batch_size_1 = jomfftLong_Product(dimension - 2, data_sizes + 1);
    const Long dft_size_1 = dft_sizes[dimension - 1];
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split)
    (num_threads, batch_size_0, batch_size_1, dft_size_1, &num_threads_0,
     &num_threads_1);
    return NS(ComplexToRealFunctionPair_0_new)(
        NS(CDft_BatchDftLoop_new)(data, true, dft_sizes[0], x_strides[0],
                                  dimension - 1, data_sizes + 1, x_strides + 1,
                                  num_threads),
        NS(ComplexToRealFunctionLoop_new_)(
            NS_(MainDft_B_new_)(data, dimension - 1, dft_sizes + 1,
                                data_sizes + 1, x_strides + 1, y_strides + 1,
                                inplace, num_threads_1),
            data_sizes[0], x_strides[0], y_strides[0], num_threads_0));
  }
}

struct NS(ComplexToRealFunction) *NS_(MainDft_B_new)(
    struct NS(DataFactory) *data, Long dimension, const Long *dft_sizes,
    Long batch_size, const Long *x_strides, Long x_distance,
    const Long *y_strides, Long y_distance, bool inplace, int num_threads) {
  assert(dimension >= 1);
  assert(batch_size >= 1);
  struct NS(ComplexToRealFunction) *out = NULL;
  Long *data_sizes = ARRAY_NEW(Long, dimension);
  ARRAY_COPY(Long, dimension, dft_sizes, data_sizes);
  data_sizes[dimension - 1] = CE_SIZE(dft_sizes[dimension - 1]);
  if (batch_size == 1) {
    out = NS_(MainDft_B_new_)(data, dimension, dft_sizes, data_sizes, x_strides,
                              y_strides, inplace, num_threads);
  } else {
    if (dimension == 1) {
      out = NS_(BatchDft_B_new)(data, dft_sizes[0], batch_size, x_strides[0],
                                x_distance, y_strides[0], y_distance, inplace,
                                num_threads);
    } else {
      if (x_distance == 1 && y_distance == 1) {
        Long *data_sizes_ = ARRAY_NEW(Long, dimension + 1);
        Long *x_strides_ = ARRAY_NEW(Long, dimension + 1);
        Long *y_strides_ = ARRAY_NEW(Long, dimension + 1);
        ARRAY_COPY(Long, dimension, data_sizes, data_sizes_);
        ARRAY_COPY(Long, dimension, x_strides, x_strides_);
        ARRAY_COPY(Long, dimension, y_strides, y_strides_);
        data_sizes_[dimension] = batch_size;
        x_strides_[dimension] = x_distance;
        y_strides_[dimension] = y_distance;
        out = NS_(MainBatchDft_B_new_)(data, dimension, dft_sizes, data_sizes_,
                                       x_strides_, y_strides_, inplace,
                                       num_threads);
        ARRAY_DELETE(data_sizes_);
        ARRAY_DELETE(x_strides_);
        ARRAY_DELETE(y_strides_);
      } else {
        const Long batch_size_0 = batch_size;
        const Long batch_size_1 = jomfftLong_Product(dimension - 1, data_sizes);
        const Long dft_size_1 = dft_sizes[dimension - 1];
        int num_threads_0 = 1;
        int num_threads_1 = 1;
        NS(Threading_Split)
        (num_threads, batch_size_0, batch_size_1, dft_size_1, &num_threads_0,
         &num_threads_1);
        out = NS(ComplexToRealFunctionLoop_new_)(
            NS_(MainDft_B_new_)(data, dimension, dft_sizes, data_sizes,
                                x_strides, y_strides, inplace, num_threads),
            batch_size, x_distance, y_distance, num_threads);
      }
    }
  }
  ARRAY_DELETE(data_sizes);
  return out;
}
