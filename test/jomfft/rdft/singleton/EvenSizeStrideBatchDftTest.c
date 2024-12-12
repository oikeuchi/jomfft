#include "EvenSizeStrideBatchDftTest.h"

#include <jomfft/rdft/singleton/EvenSizeStrideBatchDft.h>

#include "../BatchDftTest_.h"

static struct NS(RealToComplexFunction) *
RDftSingleton_EvenSizeStrideBatchDftFactory_F_create(
    struct NS(RDft_BatchDftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  (void)inplace;
  return NS(RDftSingleton_EvenSizeStrideBatchDft_F_new)(
      data, (int)dft_size / 2, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, num_threads);
}

void NS(RDftSingleton_EvenSizeStrideBatchDft_FTest)() {
  struct NS(RDft_BatchDftFactory_F) *factory = NS(RDft_BatchDftFactory_F_new)(
      RDftSingleton_EvenSizeStrideBatchDftFactory_F_create, NULL, NULL);
  NS(RDft_BatchDft_FTest_)(factory, 22);
  NS(RDft_BatchDft_FTest_)(factory, 24);
  NS(RDft_BatchDft_FTest_)(factory, 26);
  NS(RDft_BatchDft_FTest_)(factory, 134);
  NS(RDft_BatchDftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftSingleton_EvenSizeStrideBatchDftFactory_B_create(
    struct NS(RDft_BatchDftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  (void)inplace;
  return NS(RDftSingleton_EvenSizeStrideBatchDft_B_new)(
      data, (int)dft_size / 2, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, num_threads);
}

void NS(RDftSingleton_EvenSizeStrideBatchDft_BTest)() {
  struct NS(RDft_BatchDftFactory_B) *factory = NS(RDft_BatchDftFactory_B_new)(
      RDftSingleton_EvenSizeStrideBatchDftFactory_B_create, NULL, NULL);
  NS(RDft_BatchDft_BTest_)(factory, 22);
  NS(RDft_BatchDft_BTest_)(factory, 24);
  NS(RDft_BatchDft_BTest_)(factory, 26);
  NS(RDft_BatchDft_BTest_)(factory, 134);
  NS(RDft_BatchDftFactory_B_delete)(factory);
}
