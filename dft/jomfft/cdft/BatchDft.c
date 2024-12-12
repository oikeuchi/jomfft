#include "BatchDft.h"

#include "InnerBatchDft.h"
#include "StrideBatchDft.h"

#define NS_(n) NS(CDft_##n)

struct NS(ComplexFunction) *NS_(BatchDft_new)(struct NS(DataFactory) *data,
                                              bool backward, Long dft_size,
                                              Long batch_size, Long x_stride_0,
                                              Long x_stride_1, Long y_stride_0,
                                              Long y_stride_1, bool inplace,
                                              int num_threads) {
  if (x_stride_1 == 1 && y_stride_1 == 1) {
    return NS_(InnerBatchDft_new)(data, backward, dft_size, batch_size,
                                  x_stride_0, y_stride_0, inplace, num_threads);
  } else {
    return NS_(StrideBatchDft_new)(data, backward, dft_size, batch_size,
                                   x_stride_0, x_stride_1, y_stride_0,
                                   y_stride_1, inplace, num_threads);
  }
}
