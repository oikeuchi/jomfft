#include "MainDft.h"

#include <jomfft/util/LongUtil.h>

#include "../model/ComplexFunctionLoop.h"
#include "../model/ComplexFunctionPair.h"
#include "../model/ThreadingUtil.h"
#include "BatchDft.h"
#include "Dft.h"

#define NS_(n) NS(CDft_##n)

struct NS(ComplexFunction) *NS_(BatchDftLoop_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long x_distance,
    Long dimension, const Long *sizes, const Long *x_strides, int num_threads) {
  if (dimension == 1) {
    return NS_(BatchDft_new)(data, backward, dft_size, sizes[0], x_distance,
                             x_strides[0], x_distance, x_strides[0], true,
                             num_threads);
  } else {
    bool is_consecutive = true;
    Long stride = 1;
    {
      for (int i = dimension - 1; is_consecutive && i >= 0; --i) {
        is_consecutive = is_consecutive && x_strides[i] == stride;
        stride *= sizes[i];
      }
    }
    if (is_consecutive) {
      return NS_(BatchDft_new)(data, backward, dft_size, stride, x_distance, 1,
                               x_distance, 1, true, num_threads);
    } else {
      const Long batch_size_0 = sizes[0];
      const Long batch_size_1 = jomfftLong_Product(dimension - 1, sizes + 1);
      int num_threads_0 = 1;
      int num_threads_1 = 1;
      NS(Threading_Split)(num_threads, batch_size_0, batch_size_1, dft_size,
                          &num_threads_0, &num_threads_1);
      return NS(ComplexFunctionLoop_new_)(
          NS_(BatchDftLoop_new)(data, backward, dft_size, x_distance,
                                dimension - 1, sizes + 1, x_strides + 1,
                                num_threads_1),
          sizes[0], x_strides[0], x_strides[0], num_threads_0);
    }
  }
}

static struct NS(ComplexFunction) *NS_(MainBatchDft_new_)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const Long *dft_sizes, const Long *x_strides, const Long *y_strides,
    bool inplace, int num_threads) {
  assert(dimension >= 1);
  if (dimension == 1) {
    return NS_(BatchDft_new)(data, backward, dft_sizes[0], dft_sizes[1],
                             x_strides[0], x_strides[1], y_strides[0],
                             y_strides[1], inplace, num_threads);
  } else {
    const Long batch_size_0 = dft_sizes[0];
    const Long batch_size_1 =
        jomfftLong_Product(dimension - 2, dft_sizes + 1) * dft_sizes[dimension];
    const Long dft_size_1 = dft_sizes[dimension - 1];
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split)(num_threads, batch_size_0, batch_size_1, dft_size_1,
                        &num_threads_0, &num_threads_1);
    return NS(ComplexFunctionPair_1_new)(
        NS(ComplexFunctionLoop_new_)(
            NS_(MainBatchDft_new_)(data, backward, dimension - 1, dft_sizes + 1,
                                   x_strides + 1, y_strides + 1, inplace,
                                   num_threads_1),
            dft_sizes[0], x_strides[0], y_strides[0], num_threads_0),
        NS_(BatchDftLoop_new)(data, backward, dft_sizes[0], y_strides[0],
                              dimension, dft_sizes + 1, y_strides + 1,
                              num_threads));
  }
}

static struct NS(ComplexFunction) *NS_(MainDft_new_)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const Long *dft_sizes, const Long *x_strides, const Long *y_strides,
    bool inplace, int num_threads) {
  assert(dimension >= 1);
  if (dimension == 1) {
    return NS_(Dft_new)(data, backward, dft_sizes[0], x_strides[0],
                        y_strides[0], inplace, num_threads);
  } else if (dimension == 2) {
    return NS(ComplexFunctionPair_1_new)(
        NS_(BatchDft_new)(data, backward, dft_sizes[1], dft_sizes[0],
                          x_strides[1], x_strides[0], y_strides[1],
                          y_strides[0], inplace, num_threads),
        NS_(BatchDft_new)(data, backward, dft_sizes[0], dft_sizes[1],
                          y_strides[0], y_strides[1], y_strides[0],
                          y_strides[1], true, num_threads));
  } else {
    const Long batch_size_0 = dft_sizes[0];
    const Long batch_size_1 = jomfftLong_Product(dimension - 2, dft_sizes + 1);
    const Long dft_size_1 = dft_sizes[dimension - 1];
    int num_threads_0 = 1;
    int num_threads_1 = 1;
    NS(Threading_Split)(num_threads, batch_size_0, batch_size_1, dft_size_1,
                        &num_threads_0, &num_threads_1);
    return NS(ComplexFunctionPair_1_new)(
        NS(ComplexFunctionLoop_new_)(
            NS_(MainDft_new_)(data, backward, dimension - 1, dft_sizes + 1,
                              x_strides + 1, y_strides + 1, inplace,
                              num_threads_1),
            dft_sizes[0], x_strides[0], y_strides[0], num_threads_0),
        NS_(BatchDftLoop_new)(data, backward, dft_sizes[0], y_strides[0],
                              dimension - 1, dft_sizes + 1, y_strides + 1,
                              num_threads));
  }
}

struct NS(ComplexFunction) *NS_(MainDft_new)(
    struct NS(DataFactory) *data, bool backward, Long dimension,
    const Long *dft_sizes, Long batch_size, const Long *x_strides,
    Long x_distance, const Long *y_strides, Long y_distance, bool inplace,
    int num_threads) {
  assert(dimension >= 1);
  assert(batch_size >= 1);
  struct NS(ComplexFunction) *out = NULL;
  if (batch_size == 1) {
    out = NS_(MainDft_new_)(data, backward, dimension, dft_sizes, x_strides,
                            y_strides, inplace, num_threads);
  } else {
    if (dimension == 1) {
      out = NS_(BatchDft_new)(data, backward, dft_sizes[0], batch_size,
                              x_strides[0], x_distance, y_strides[0],
                              y_distance, inplace, num_threads);
    } else {
      if (x_distance == 1 && y_distance == 1) {
        Long *dft_sizes_ = ARRAY_NEW(Long, dimension + 1);
        Long *x_strides_ = ARRAY_NEW(Long, dimension + 1);
        Long *y_strides_ = ARRAY_NEW(Long, dimension + 1);
        ARRAY_COPY(Long, dimension, dft_sizes, dft_sizes_);
        ARRAY_COPY(Long, dimension, x_strides, x_strides_);
        ARRAY_COPY(Long, dimension, y_strides, y_strides_);
        dft_sizes_[dimension] = batch_size;
        x_strides_[dimension] = x_distance;
        y_strides_[dimension] = y_distance;
        out = NS_(MainBatchDft_new_)(data, backward, dimension, dft_sizes_,
                                     x_strides_, y_strides_, inplace,
                                     num_threads);
        ARRAY_DELETE(dft_sizes_);
        ARRAY_DELETE(x_strides_);
        ARRAY_DELETE(y_strides_);
      } else {
        const Long batch_size_0 = batch_size;
        const Long batch_size_1 = jomfftLong_Product(dimension - 1, dft_sizes);
        const Long dft_size_1 = dft_sizes[dimension - 1];
        int num_threads_0 = 1;
        int num_threads_1 = 1;
        NS(Threading_Split)(num_threads, batch_size_0, batch_size_1, dft_size_1,
                            &num_threads_0, &num_threads_1);
        out = NS(ComplexFunctionLoop_new_)(
            NS_(MainDft_new_)(data, backward, dimension, dft_sizes, x_strides,
                              y_strides, inplace, num_threads_1),
            batch_size, x_distance, y_distance, num_threads_0);
      }
    }
  }
  return out;
}
