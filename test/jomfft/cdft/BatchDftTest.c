#include "BatchDftTest.h"

#include <jomfft/cdft/BatchDft.h>

#include "BatchDftTest_.h"

#define NS_(n) NS(CDft_##n)

static struct NS(ComplexFunction) *BatchDftFactory_create(
    struct NS_(BatchDftFactory) *p, struct NS(DataFactory) *data, bool backward,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  (void)p;
  return NS_(BatchDft_new)(data, backward, dft_size, batch_size, x_stride_0,
                           x_stride_1, y_stride_0, y_stride_1, inplace,
                           num_threads);
}

void NS_(BatchDftTest)() {
  struct NS_(BatchDftFactory) *factory =
      NS_(BatchDftFactory_new)(BatchDftFactory_create, NULL, NULL);
  NS_(BatchDftTest_)(factory, 4);
  NS_(BatchDftTest_)(factory, 31);
  NS_(BatchDftTest_)(factory, 256);
  NS_(BatchDftFactory_delete)(factory);
}
