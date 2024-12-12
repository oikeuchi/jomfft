#include "DftTest.h"

#include <jomfft/cdft/bluestein/Dft.h>

#include "../DftTest_.h"

static struct NS(ComplexFunction) *
CDftBluestein_DftFactory_create(struct NS(CDft_DftFactory) *p,
                                struct NS(DataFactory) *data, bool backward,
                                Long dft_size, Long x_stride, Long y_stride,
                                bool inplace, int num_threads) {
  (void)p;
  (void)inplace;
  return NS(CDftBluestein_Dft_new)(data, backward, dft_size, x_stride, y_stride,
                                   num_threads);
}

void NS(CDftBluestein_DftTest)() {
  struct NS(CDft_DftFactory) *factory =
      NS(CDft_DftFactory_new)(CDftBluestein_DftFactory_create, NULL, NULL);
  NS(CDft_DftTest_)(factory, 7);
  NS(CDft_DftTest_)(factory, 31);
  NS(CDft_DftTest_)(factory, 127);
  NS(CDft_DftFactory_delete)(factory);
}
