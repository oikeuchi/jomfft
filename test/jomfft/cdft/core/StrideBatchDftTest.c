#include "StrideBatchDftTest.h"

#include <jomfft/cdft/core/StrideBatchDft.h>
#include <jomfft/data/DataFactory.h>

#include "../BatchDftTest_.h"
#include "../OuterBatchDftTest.h"

static void CDftCore_StrideBatchDft_1Test__(bool backward, int dft_size,
                                            Long batch_size, Long stride,
                                            int num_threads) {
  const Long stride_1 = dft_size * stride;
  struct NS(ComplexFunction) *p =
      NS(CDftCore_StrideBatchDft_new_)(backward, dft_size, batch_size, stride,
                                       stride_1, stride, stride_1, num_threads);
  NS(CDft_OuterBatchDftTest)(p, backward, dft_size, batch_size, stride, stride,
                             false);
  NS(ComplexFunction_delete)(p);
}

static void CDftCore_StrideBatchDft_1Test_(bool backward, int dft_size,
                                           Long batch_size, Long stride) {
  CDftCore_StrideBatchDft_1Test__(backward, dft_size, batch_size, stride, 1);
}

void NS(CDftCore_StrideBatchDft_1Test)() {
  CDftCore_StrideBatchDft_1Test_(false, 2, 1, 1);
  CDftCore_StrideBatchDft_1Test_(false, 4, 2, 2);
  CDftCore_StrideBatchDft_1Test_(true, 8, 3, 1);
  CDftCore_StrideBatchDft_1Test_(true, 16, 4, 2);
  CDftCore_StrideBatchDft_1Test_(false, 2, 5, 1);
  CDftCore_StrideBatchDft_1Test_(false, 2, 6, 1);
  CDftCore_StrideBatchDft_1Test_(false, 2, 7, 1);
  CDftCore_StrideBatchDft_1Test_(false, 2, 8, 1);
  CDftCore_StrideBatchDft_1Test_(false, 2, 9, 1);
  CDftCore_StrideBatchDft_1Test_(false, 2, 10, 1);
#if USE_OPENMP
  CDftCore_StrideBatchDft_1Test__(false, 4, 2, 1, 2);
  CDftCore_StrideBatchDft_1Test__(false, 4, 16, 2, 2);
  CDftCore_StrideBatchDft_1Test__(false, 4, 15, 1, 2);
  CDftCore_StrideBatchDft_1Test__(true, 4, 3, 1, 2);
  CDftCore_StrideBatchDft_1Test__(true, 4, 16, 2, 2);
  CDftCore_StrideBatchDft_1Test__(true, 4, 15, 1, 2);
#endif
}

static struct NS(ComplexFunction) *CDftCore_StrideBatchDftFactory_create(
    struct NS(CDft_BatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, Long dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, bool inplace,
    int num_threads) {
  (void)p;
  (void)data;
  (void)inplace;
  return NS(CDftCore_StrideBatchDft_new_)(backward, (int)dft_size, batch_size,
                                          x_stride_0, x_stride_1, y_stride_0,
                                          y_stride_1, num_threads);
}

void NS(CDftCore_StrideBatchDftTest)() {
  struct NS(CDft_BatchDftFactory) *factory = NS(CDft_BatchDftFactory_new)(
      CDftCore_StrideBatchDftFactory_create, NULL, NULL);
  NS(CDft_BatchDftTest_)(factory, 2);
  NS(CDft_BatchDftTest_)(factory, 4);
  NS(CDft_BatchDftTest_)(factory, 8);
  NS(CDft_BatchDftFactory_delete)(factory);
}
