#include "EvenSizeStrideBatchDftTest.h"

#include <jomfft/rdft/core/EvenSizeStrideBatchDft.h>

#include "../BatchDftTest_.h"

static struct NS(RealToComplexFunction) *
RDftCore_EvenSizeStrideBatchDftFactory_F_create(
    struct NS(RDft_BatchDftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  (void)inplace;
  return NS(RDftCore_EvenSizeStrideBatchDft_F_new)(
      data, (int)dft_size / 2, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, num_threads);
}

void NS(RDftCore_EvenSizeStrideBatchDft_FTest)() {
  struct NS(RDft_BatchDftFactory_F) *factory = NS(RDft_BatchDftFactory_F_new)(
      RDftCore_EvenSizeStrideBatchDftFactory_F_create, NULL, NULL);
  NS(RDft_BatchDft_FTest_)(factory, 4);
  NS(RDft_BatchDft_FTest_)(factory, 8);
  NS(RDft_BatchDft_FTest_)(factory, 16);
  NS(RDft_BatchDftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftCore_EvenSizeStrideBatchDftFactory_B_create(
    struct NS(RDft_BatchDftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  (void)inplace;
  return NS(RDftCore_EvenSizeStrideBatchDft_B_new)(
      data, (int)dft_size / 2, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, num_threads);
}

void NS(RDftCore_EvenSizeStrideBatchDft_BTest)() {
  struct NS(RDft_BatchDftFactory_B) *factory = NS(RDft_BatchDftFactory_B_new)(
      RDftCore_EvenSizeStrideBatchDftFactory_B_create, NULL, NULL);
  NS(RDft_BatchDft_BTest_)(factory, 4);
  NS(RDft_BatchDft_BTest_)(factory, 8);
  NS(RDft_BatchDft_BTest_)(factory, 16);
  NS(RDft_BatchDftFactory_B_delete)(factory);
}
