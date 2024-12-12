#include "EvenSizeDftTest.h"

#include <jomfft/rdft/square/EvenSizeBatchDft.h>

#include "../BatchDftTest_.h"

static struct NS(RealToComplexFunction) *
RDftSquare_EvenSizeBatchDftFactory_F_create(
    struct NS(RDft_BatchDftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size / 2);
  return NS(RDftSquare_EvenSizeBatchDft_F_new)(
      data, &f, batch_size, x_stride_0, x_stride_1, y_stride_0, y_stride_1,
      inplace, num_threads);
}

void NS(RDftSquare_EvenSizeBatchDft_FTest)() {
  struct NS(RDft_BatchDftFactory_F) *factory = NS(RDft_BatchDftFactory_F_new)(
      RDftSquare_EvenSizeBatchDftFactory_F_create, NULL, NULL);
  NS(RDft_BatchDft_FTest_)(factory, 256);
  NS(RDft_BatchDftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftSquare_EvenSizeBatchDftFactory_B_create(
    struct NS(RDft_BatchDftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size / 2);
  return NS(RDftSquare_EvenSizeBatchDft_B_new)(
      data, &f, batch_size, x_stride_0, x_stride_1, y_stride_0, y_stride_1,
      inplace, num_threads);
}

void NS(RDftSquare_EvenSizeBatchDft_BTest)() {
  struct NS(RDft_BatchDftFactory_B) *factory = NS(RDft_BatchDftFactory_B_new)(
      RDftSquare_EvenSizeBatchDftFactory_B_create, NULL, NULL);
  NS(RDft_BatchDft_BTest_)(factory, 256);
  NS(RDft_BatchDftFactory_B_delete)(factory);
}
