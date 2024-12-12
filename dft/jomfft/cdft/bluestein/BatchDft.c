#include "BatchDft.h"

#include "../../data/BluesteinDftData.h"
#include "../../model/ComplexFunctionBufferedLoop.h"
#include "../../model/ComplexFunctionW.h"
#include "../../model/ThreadingUtil.h"
#include "Dft.h"

#define NS_(n) NS(CDftBluestein_##n)

struct NS(ComplexFunction) *NS_(BatchDft_new)(struct NS(DataFactory) *data,
                                              bool backward, Long dft_size,
                                              Long batch_size, Long x_stride_0,
                                              Long x_stride_1, Long y_stride_0,
                                              Long y_stride_1,
                                              int num_threads) {
  int num_threads_0 = 1;
  int num_threads_1 = 1;
  NS(Threading_Split)(num_threads, batch_size, 1, 2 * dft_size, &num_threads_0,
                      &num_threads_1);
  return NS(ComplexFunctionBufferedLoop_new_)(
      NS_(Dft_newW)(data, backward, dft_size, x_stride_0, y_stride_0,
                    num_threads_1),
      batch_size, x_stride_1, y_stride_1, num_threads_0);
}
