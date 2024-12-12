#include "EvenSizeDftTest.h"

#include <jomfft/rdft/bluestein/EvenSizeDft.h>

#include "../DftTest_.h"

static struct NS(RealToComplexFunction) *
RDftBluestein_EvenSizeDftFactory_F_create(struct NS(RDft_DftFactory_F) *p,
                                          struct NS(DataFactory) *data,
                                          Long dft_size, Long x_stride,
                                          Long y_stride, bool inplace,
                                          int num_threads) {
  (void)p;
  (void)inplace;
  return NS(RDftBluestein_EvenSizeDft_F_new)(data, dft_size / 2, x_stride,
                                             y_stride, num_threads);
}

void NS(RDftBluestein_EvenSizeDft_FTest)() {
  struct NS(RDft_DftFactory_F) *factory = NS(RDft_DftFactory_F_new)(
      RDftBluestein_EvenSizeDftFactory_F_create, NULL, NULL);
  NS(RDft_Dft_FTest_)(factory, 62);
  NS(RDft_DftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftBluestein_EvenSizeDftFactory_B_create(struct NS(RDft_DftFactory_B) *p,
                                          struct NS(DataFactory) *data,
                                          Long dft_size, Long x_stride,
                                          Long y_stride, bool inplace,
                                          int num_threads) {
  (void)p;
  (void)inplace;
  return NS(RDftBluestein_EvenSizeDft_B_new)(data, dft_size / 2, x_stride,
                                             y_stride, num_threads);
}

void NS(RDftBluestein_EvenSizeDft_BTest)() {
  struct NS(RDft_DftFactory_B) *factory = NS(RDft_DftFactory_B_new)(
      RDftBluestein_EvenSizeDftFactory_B_create, NULL, NULL);
  NS(RDft_Dft_BTest_)(factory, 62);
  NS(RDft_DftFactory_B_delete)(factory);
}
