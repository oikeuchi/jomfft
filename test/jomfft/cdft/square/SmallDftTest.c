#include "SmallDftTest.h"

#include <jomfft/cdft/square/SmallDft.h>

#include "../DftTest_.h"

static void SmallDft_1Test_(bool backward, int square_dft_size,
                            int inner_dft_size) {
  CAPTURE_BOOL(backward);
  CAPTURE_INT(square_dft_size);
  CAPTURE_INT(inner_dft_size);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS(CDftSquare_SmallDft_new_1)(
      data, backward, square_dft_size, inner_dft_size);
  const Long dft_size = square_dft_size * square_dft_size * inner_dft_size;
  NS(CDft_DftTest__)(func, backward, dft_size, 1, 1, true);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

void NS(CDftSquare_SmallDft_1Test)() {
  SmallDft_1Test_(false, 8, 2);
  SmallDft_1Test_(true, 16, 1);
}

static struct NS(ComplexFunction) *
CDftSquare_SmallDftFactory_create(struct NS(CDft_DftFactory) *p,
                                  struct NS(DataFactory) *data, bool backward,
                                  Long dft_size, Long x_stride, Long y_stride,
                                  bool inplace, int num_threads) {
  (void)p;
  (void)num_threads;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(CDftSquare_SmallDft_new)(data, backward, &f, x_stride, y_stride,
                                     inplace);
}

void NS(CDftSquare_SmallDftTest)() {
  struct NS(CDft_DftFactory) *factory =
      NS(CDft_DftFactory_new)(CDftSquare_SmallDftFactory_create, NULL, NULL);
  NS(CDft_DftTest_)(factory, 128);
  NS(CDft_DftTest_)(factory, 256);
  NS(CDft_DftFactory_delete)(factory);
}
