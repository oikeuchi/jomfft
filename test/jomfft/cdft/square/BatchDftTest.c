#include "BatchDftTest.h"

#include <jomfft/cdft/square/BatchDft.h>

#include "../BatchDftTest_.h"

static struct NS(ComplexFunction) *CDftSquare_BatchDftFactory_create(
    struct NS(CDft_BatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, Long dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, bool inplace,
    int num_threads) {
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(CDftSquare_BatchDft_new)(data, backward, &f, batch_size, x_stride_0,
                                     x_stride_1, y_stride_0, y_stride_1,
                                     inplace, num_threads);
}

void NS(CDftSquare_BatchDftTest)() {
  struct NS(CDft_BatchDftFactory) *factory = NS(CDft_BatchDftFactory_new)(
      CDftSquare_BatchDftFactory_create, NULL, NULL);
  NS(CDft_BatchDftTest_)(factory, 128);
  NS(CDft_BatchDftTest_)(factory, 256);
  NS(CDft_BatchDftFactory_delete)(factory);
}
