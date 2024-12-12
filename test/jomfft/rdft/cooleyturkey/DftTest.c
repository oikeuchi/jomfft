#include "DftTest.h"

#include <jomfft/rdft/cooleyturkey/Dft.h>

#include "../DftTest_.h"

static struct NS(RealToComplexFunction) *RDftCooleyTurkey_DftFactory_F_create(
    struct NS(RDft_DftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long x_stride, Long y_stride, bool inplace,
    int num_threads) {
  (void)p;
  (void)inplace;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(RDftCooleyTurkey_Dft_F_new)(data, &f, x_stride, y_stride,
                                        num_threads);
}

void NS(RDftCooleyTurkey_Dft_FTest)() {
  struct NS(RDft_DftFactory_F) *factory = NS(RDft_DftFactory_F_new)(
      RDftCooleyTurkey_DftFactory_F_create, NULL, NULL);
  NS(RDft_Dft_FTest_)(factory, 45);
  NS(RDft_Dft_FTest_)(factory, 64);
  NS(RDft_DftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *RDftCooleyTurkey_DftFactory_B_create(
    struct NS(RDft_DftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long x_stride, Long y_stride, bool inplace,
    int num_threads) {
  (void)p;
  (void)inplace;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(RDftCooleyTurkey_Dft_B_new)(data, &f, x_stride, y_stride,
                                        num_threads);
}

void NS(RDftCooleyTurkey_Dft_BTest)() {
  struct NS(RDft_DftFactory_B) *factory = NS(RDft_DftFactory_B_new)(
      RDftCooleyTurkey_DftFactory_B_create, NULL, NULL);
  NS(RDft_Dft_BTest_)(factory, 45);
  NS(RDft_Dft_BTest_)(factory, 64);
  NS(RDft_DftFactory_B_delete)(factory);
}
