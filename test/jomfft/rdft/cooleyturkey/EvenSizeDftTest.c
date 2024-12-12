#include "EvenSizeDftTest.h"

#include <jomfft/rdft/cooleyturkey/EvenSizeDft.h>

#include "../DftTest_.h"

static struct NS(RealToComplexFunction) *
RDftCooleyTurkey_EvenSizeDftFactory_F_create(struct NS(RDft_DftFactory_F) *p,
                                             struct NS(DataFactory) *data,
                                             Long dft_size, Long x_stride,
                                             Long y_stride, bool inplace,
                                             int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size / 2);
  return NS(RDftCooleyTurkey_EvenSizeDft_F_new)(data, &f, x_stride, y_stride,
                                                inplace, num_threads);
}

void NS(RDftCooleyTurkey_EvenSizeDft_FTest)() {
  struct NS(RDft_DftFactory_F) *factory = NS(RDft_DftFactory_F_new)(
      RDftCooleyTurkey_EvenSizeDftFactory_F_create, NULL, NULL);
  NS(RDft_Dft_FTest_)(factory, 128);
  NS(RDft_DftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
RDftCooleyTurkey_EvenSizeDftFactory_B_create(struct NS(RDft_DftFactory_B) *p,
                                             struct NS(DataFactory) *data,
                                             Long dft_size, Long x_stride,
                                             Long y_stride, bool inplace,
                                             int num_threads) {
  assert(dft_size % 2 == 0);
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size / 2);
  return NS(RDftCooleyTurkey_EvenSizeDft_B_new)(data, &f, x_stride, y_stride,
                                                inplace, num_threads);
}

void NS(RDftCooleyTurkey_EvenSizeDft_BTest)() {
  struct NS(RDft_DftFactory_B) *factory = NS(RDft_DftFactory_B_new)(
      RDftCooleyTurkey_EvenSizeDftFactory_B_create, NULL, NULL);
  NS(RDft_Dft_BTest_)(factory, 128);
  NS(RDft_DftFactory_B_delete)(factory);
}
