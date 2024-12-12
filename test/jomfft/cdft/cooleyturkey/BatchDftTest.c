#include "BatchDftTest.h"

#include <jomfft/cdft/cooleyturkey/BatchDft.h>

#include "../BatchDftTest_.h"

static struct NS(ComplexFunction) *CDftCooleyTurkey_BatchDftFactory_create(
    struct NS(CDft_BatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, Long dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, bool inplace,
    int num_threads) {
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(CDftCooleyTurkey_BatchDft_new)(data, backward, &f, batch_size,
                                           x_stride_0, x_stride_1, y_stride_0,
                                           y_stride_1, inplace, num_threads);
}

void NS(CDftCooleyTurkey_BatchDftTest)() {
  struct NS(CDft_BatchDftFactory) *factory = NS(CDft_BatchDftFactory_new)(
      CDftCooleyTurkey_BatchDftFactory_create, NULL, NULL);
  NS(CDft_BatchDftTest_)(factory, 8 * 8);
  NS(CDft_BatchDftTest_)(factory, 9 * 5);
  NS(CDft_BatchDftTest_)(factory, 4 * 3 * 5 * 7);
  NS(CDft_BatchDftFactory_delete)(factory);
}
