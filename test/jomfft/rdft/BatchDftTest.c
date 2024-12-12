#include "BatchDftTest.h"

#include <jomfft/rdft/BatchDft.h>

#include "BatchDftTest_.h"

#define NS_(n) NS(RDft_##n)

static struct NS(RealToComplexFunction) *BatchDftFactory_F_create(
    struct NS_(BatchDftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  (void)p;
  return NS_(BatchDft_F_new)(data, dft_size, batch_size, x_stride_0, x_stride_1,
                             y_stride_0, y_stride_1, inplace, num_threads);
}

void NS_(BatchDft_FTest)() {
  struct NS_(BatchDftFactory_F) *factory =
      NS_(BatchDftFactory_F_new)(BatchDftFactory_F_create, NULL, NULL);
  NS_(BatchDft_FTest_)(factory, 8);
  NS_(BatchDft_FTest_)(factory, 31);
  NS_(BatchDft_FTest_)(factory, 256);
  NS_(BatchDftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *BatchDftFactory_B_create(
    struct NS_(BatchDftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  (void)p;
  return NS_(BatchDft_B_new)(data, dft_size, batch_size, x_stride_0, x_stride_1,
                             y_stride_0, y_stride_1, inplace, num_threads);
}

void NS_(BatchDft_BTest)() {
  struct NS_(BatchDftFactory_B) *factory =
      NS_(BatchDftFactory_B_new)(BatchDftFactory_B_create, NULL, NULL);
  NS_(BatchDft_BTest_)(factory, 8);
  NS_(BatchDft_BTest_)(factory, 31);
  NS_(BatchDft_BTest_)(factory, 256);
  NS_(BatchDftFactory_B_delete)(factory);
}
