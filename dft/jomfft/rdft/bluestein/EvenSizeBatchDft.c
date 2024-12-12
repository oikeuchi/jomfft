#include "EvenSizeBatchDft.h"

#include "../../model/ComplexToRealFunctionBufferedLoop.h"
#include "../../model/RealToComplexFunctionBufferedLoop.h"
#include "../../model/ThreadingUtil.h"
#include "EvenSizeDft.h"

#define NS_(n) NS(RDftBluestein_##n)

struct NS(RealToComplexFunction) *NS_(EvenSizeBatchDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)(num_threads, batch_size, 1, dft_size, &num_threads_0,
                       &num_threads_1, true);
  return NS(RealToComplexFunctionBufferedLoop_new_)(
      NS_(EvenSizeDft_F_newW)(data, dft_size, x_stride_0, y_stride_0,
                              num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeBatchDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split_)(num_threads, batch_size, 1, dft_size, &num_threads_0,
                       &num_threads_1, true);
  return NS(ComplexToRealFunctionBufferedLoop_new_)(
      NS_(EvenSizeDft_B_newW)(data, dft_size, x_stride_0, y_stride_0,
                              num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}
