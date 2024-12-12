#include "StrideBatchDftTest.h"

#include <jomfft/cdft/singleton/StrideBatchDft.h>

#include "../BatchDftTest_.h"
#include "../OuterBatchDftTest.h"

static void CDftSingleton_StrideBatchDftTest__(bool backward, int dft_size,
                                               Long batch_size, Long stride,
                                               int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long stride_1 = dft_size * stride;
  struct NS(ComplexFunction) *p = NS(CDftSingleton_StrideBatchDft_new_)(
      data, backward, dft_size, batch_size, stride, stride_1, stride, stride_1,
      num_threads);
  NS(CDft_OuterBatchDftTest)(p, backward, dft_size, batch_size, stride, stride,
                             false);
  NS(ComplexFunction_delete)(p);
  NS(DataFactory_delete)(data);
}

static void CDftSingleton_StrideBatchDftTest_(bool backward, int dft_size,
                                              Long batch_size, Long stride) {
  CDftSingleton_StrideBatchDftTest__(backward, dft_size, batch_size, stride, 1);
}

void NS(CDftSingleton_StrideBatchDft_1Test)() {
  CDftSingleton_StrideBatchDftTest_(false, 13, 1, 1);
  CDftSingleton_StrideBatchDftTest_(false, 28, 2, 2);
  CDftSingleton_StrideBatchDftTest_(true, 31, 3, 1);
  CDftSingleton_StrideBatchDftTest_(true, 9, 4, 2);
  CDftSingleton_StrideBatchDftTest_(false, 12, 5, 1);
#if USE_OPENMP
  CDftSingleton_StrideBatchDftTest__(false, 9, 4, 1, 2);
  CDftSingleton_StrideBatchDftTest__(true, 9, 5, 1, 2);
  CDftSingleton_StrideBatchDftTest__(false, 12, 8, 2, 2);
  CDftSingleton_StrideBatchDftTest__(true, 12, 5, 2, 2);
#endif
}

static struct NS(ComplexFunction) *CDftSingleton_StrideBatchDftFactory_create(
    struct NS(CDft_BatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, Long dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, bool inplace,
    int num_threads) {
  (void)p;
  (void)inplace;
  return NS(CDftSingleton_StrideBatchDft_new_)(
      data, backward, (int)dft_size, batch_size, x_stride_0, x_stride_1,
      y_stride_0, y_stride_1, num_threads);
}

void NS(CDftSingleton_StrideBatchDftTest)() {
  struct NS(CDft_BatchDftFactory) *factory = NS(CDft_BatchDftFactory_new)(
      CDftSingleton_StrideBatchDftFactory_create, NULL, NULL);
  NS(CDft_BatchDftTest_)(factory, 17);
  NS(CDft_BatchDftTest_)(factory, 18);
  NS(CDft_BatchDftFactory_delete)(factory);
}
