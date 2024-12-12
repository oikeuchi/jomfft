#include "LargeDftTest.h"

#include <jomfft/cdft/square/LargeDft.h>

#include "../DftTest_.h"

static void LargeDft_1Test__(bool backward, Long square_dft_size,
                             int inner_dft_size, bool inplace,
                             int num_threads) {
  CAPTURE_INT(square_dft_size);
  CAPTURE_INT(inner_dft_size);
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func =
      NS(CDftSquare_LargeDft_new_1)(data, backward, square_dft_size,
                                    inner_dft_size, 1, 1, inplace, num_threads);
  const Long dft_size = square_dft_size * square_dft_size * inner_dft_size;
  NS(CDft_DftTest__)(func, backward, dft_size, 1, 1, inplace);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void LargeDft_1Test_(bool backward, Long square_dft_size,
                            int inner_dft_size, bool inplace) {
  LargeDft_1Test__(backward, square_dft_size, inner_dft_size, inplace, 1);
}

void NS(CDftSquare_LargeDft_1Test)() {
  LargeDft_1Test_(false, 8, 2, true);
  LargeDft_1Test_(true, 8, 1, false);
#if USE_OPENMP
  LargeDft_1Test__(false, 16, 2, true, 2);
  LargeDft_1Test__(true, 16, 1, false, omp_get_max_threads());
#endif
}

static struct NS(ComplexFunction) *
CDftSquare_LargeDftFactory_create(struct NS(CDft_DftFactory) *p,
                                  struct NS(DataFactory) *data, bool backward,
                                  Long dft_size, Long x_stride, Long y_stride,
                                  bool inplace, int num_threads) {
  (void)p;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(CDftSquare_LargeDft_new)(data, backward, &f, x_stride, y_stride,
                                     inplace, num_threads);
}

void NS(CDftSquare_LargeDftTest)() {
  struct NS(CDft_DftFactory) *factory =
      NS(CDft_DftFactory_new)(CDftSquare_LargeDftFactory_create, NULL, NULL);
  NS(CDft_DftTest_)(factory, 128);
  NS(CDft_DftTest_)(factory, 256);
  NS(CDft_DftFactory_delete)(factory);
}
