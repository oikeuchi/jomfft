#include "SmallDftTest.h"

#include <jomfft/cdft/square/Dft.h>

#include "../DftTest_.h"

static struct NS(ComplexFunction) *
CDftSquare_DftFactory_create(struct NS(CDft_DftFactory) *p,
                             struct NS(DataFactory) *data, bool backward,
                             Long dft_size, Long x_stride, Long y_stride,
                             bool inplace, int num_threads) {
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(CDftSquare_Dft_new)(data, backward, &f, x_stride, y_stride, inplace,
                                num_threads);
}

void NS(CDftSquare_DftTest)() {
  struct NS(CDft_DftFactory) *factory =
      NS(CDft_DftFactory_new)(CDftSquare_DftFactory_create, NULL, NULL);
  NS(CDft_DftTest_)(factory, 128);
  NS(CDft_DftTest_)(factory, 256);
  NS(CDft_DftFactory_delete)(factory);
}
