#include "BatchDftTest.h"

#include <jomfft/rdft/cooleyturkey/BatchDft.h>

#include "../BatchDftTest_.h"

static struct NS(RealToComplexFunction) *
RDftCooleyTurkey_BatchDftFactory_F_create(struct NS(RDft_BatchDftFactory_F) *p,
                                          struct NS(DataFactory) *data,
                                          Long dft_size, Long batch_size,
                                          Long x_stride_0, Long x_stride_1,
                                          Long y_stride_0, Long y_stride_1,
                                          bool inplace, int num_threads) {
  (void)p;
  (void)inplace;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(RDftCooleyTurkey_BatchDft_F_new)(data, &f, batch_size, x_stride_0,
                                             x_stride_1, y_stride_0, y_stride_1,
                                             num_threads);
}

void NS(RDftCooleyTurkey_BatchDft_FTest)() {
  struct NS(RDft_BatchDftFactory_F) *factory = NS(RDft_BatchDftFactory_F_new)(
      RDftCooleyTurkey_BatchDftFactory_F_create, NULL, NULL);
  NS(RDft_BatchDft_FTest_)(factory, 16);
  NS(RDft_BatchDftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftCooleyTurkey_BatchDftFactory_B_create(struct NS(RDft_BatchDftFactory_B) *p,
                                          struct NS(DataFactory) *data,
                                          Long dft_size, Long batch_size,
                                          Long x_stride_0, Long x_stride_1,
                                          Long y_stride_0, Long y_stride_1,
                                          bool inplace, int num_threads) {
  (void)p;
  (void)inplace;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(RDftCooleyTurkey_BatchDft_B_new)(data, &f, batch_size, x_stride_0,
                                             x_stride_1, y_stride_0, y_stride_1,
                                             num_threads);
}

void NS(RDftCooleyTurkey_BatchDft_BTest)() {
  struct NS(RDft_BatchDftFactory_B) *factory = NS(RDft_BatchDftFactory_B_new)(
      RDftCooleyTurkey_BatchDftFactory_B_create, NULL, NULL);
  NS(RDft_BatchDft_BTest_)(factory, 16);
  NS(RDft_BatchDftFactory_B_delete)(factory);
}
