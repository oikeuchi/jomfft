#include "BatchDftTest.h"

#include <jomfft/cdft/bluestein/BatchDft.h>

#include "../BatchDftTest_.h"

static struct NS(ComplexFunction) *CDftBluestein_BatchDftFactory_create(
    struct NS(CDft_BatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, Long dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, bool inplace,
    int num_threads) {
  (void)p;
  (void)inplace;
  return NS(CDftBluestein_BatchDft_new)(data, backward, dft_size, batch_size,
                                        x_stride_0, x_stride_1, y_stride_0,
                                        y_stride_1, num_threads);
}

void NS(CDftBluestein_BatchDftTest)() {
  struct NS(CDft_BatchDftFactory) *factory = NS(CDft_BatchDftFactory_new)(
      CDftBluestein_BatchDftFactory_create, NULL, NULL);
  NS(CDft_BatchDftTest_)(factory, 7);
  NS(CDft_BatchDftTest_)(factory, 31);
  NS(CDft_BatchDftFactory_delete)(factory);
}
