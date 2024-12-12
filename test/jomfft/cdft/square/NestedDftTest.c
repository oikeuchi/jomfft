#include "NestedDftTest.h"

#include <jomfft/cdft/square/NestedDft.h>

#include "../DftTest_.h"

static void NestedDft_1Test__(bool backward, int square_dft_size,
                              int inner_dft_size, int num_threads) {
  CAPTURE_INT(square_dft_size);
  CAPTURE_INT(inner_dft_size);
  CAPTURE_INT(num_threads);
  Long n = inner_dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  assert(n == 1);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS(CDftSquare_NestedDft_new_1)(
      data, backward, square_dft_size, &f, 1, num_threads);
  const Long dft_size = square_dft_size * square_dft_size * inner_dft_size;
  NS(CDft_DftTest__)(func, backward, dft_size, 1, 1, true);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void NestedDft_1Test_(bool backward, int square_dft_size,
                             int inner_dft_size) {
  NestedDft_1Test__(backward, square_dft_size, inner_dft_size, 1);
}

void NS(CDftSquare_NestedDft_1Test)() {
  NestedDft_1Test_(false, 4, 16);
  NestedDft_1Test_(true, 8, 8);
#if USE_OPENMP
  NestedDft_1Test__(false, 8, 8, 2);
  NestedDft_1Test__(true, 2, 256, omp_get_max_threads());
#endif
}

static struct NS(ComplexFunction) *
CDftSquare_NestedDftFactory_create(struct NS(CDft_DftFactory) *p,
                                   struct NS(DataFactory) *data, bool backward,
                                   Long dft_size, Long x_stride, Long y_stride,
                                   bool inplace, int num_threads) {
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(CDftSquare_NestedDft_new)(data, backward, &f, x_stride, y_stride,
                                      inplace, num_threads);
}

void NS(CDftSquare_NestedDftTest)() {
  struct NS(CDft_DftFactory) *factory =
      NS(CDft_DftFactory_new)(CDftSquare_NestedDftFactory_create, NULL, NULL);
  NS(CDft_DftTest_)(factory, 128);
  NS(CDft_DftTest_)(factory, 256);
  NS(CDft_DftFactory_delete)(factory);
}
