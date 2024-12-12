#include "DftTest.h"

#include <jomfft/rdft/bluestein/Dft.h>

#include "../DftTest_.h"

static struct NS(RealToComplexFunction) *
RDftBluestein_DftFactory_F_create(struct NS(RDft_DftFactory_F) *p,
                                  struct NS(DataFactory) *data, Long dft_size,
                                  Long x_stride, Long y_stride, bool inplace,
                                  int num_threads) {
  (void)p;
  (void)inplace;
  return NS(RDftBluestein_Dft_F_new)(data, dft_size, x_stride, y_stride,
                                     num_threads);
}

void NS(RDftBluestein_Dft_FTest)() {
  struct NS(RDft_DftFactory_F) *factory =
      NS(RDft_DftFactory_F_new)(RDftBluestein_DftFactory_F_create, NULL, NULL);
  NS(RDft_Dft_FTest_)(factory, 61);
  NS(RDft_DftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftBluestein_DftFactory_B_create(struct NS(RDft_DftFactory_B) *p,
                                  struct NS(DataFactory) *data, Long dft_size,
                                  Long x_stride, Long y_stride, bool inplace,
                                  int num_threads) {
  (void)p;
  (void)inplace;
  return NS(RDftBluestein_Dft_B_new)(data, dft_size, x_stride, y_stride,
                                     num_threads);
}

void NS(RDftBluestein_Dft_BTest)() {
  struct NS(RDft_DftFactory_B) *factory =
      NS(RDft_DftFactory_B_new)(RDftBluestein_DftFactory_B_create, NULL, NULL);
  NS(RDft_Dft_BTest_)(factory, 61);
  NS(RDft_DftFactory_B_delete)(factory);
}
