#include "DftTest.h"

#include <jomfft/cdft/cooleyturkey/Dft.h>
#include <jomfft/util/IntUtil.h>

#include "../DftTest_.h"

#define SZ(a) (sizeof(a) / sizeof(int))

static void Dft_1Test__(bool backward, Long dimension, const int *sizes,
                        int num_threads) {
  CAPTURE_INTARRAY(dimension, sizes);
  CAPTURE_INT(num_threads);
  const Long dft_size = jomfftInt_ProductLong(dimension, sizes);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *func = NS(CDftCooleyTurkey_Dft_new_1)(
      data, backward, dimension, sizes, 1, num_threads);
  NS(CDft_DftTest__)(func, backward, dft_size, 1, 1, false);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

static void Dft_1Test_(bool backward, Long dimension, const int *sizes) {
  Dft_1Test__(backward, dimension, sizes, 1);
}

void NS(CDftCooleyTurkey_Dft_1Test)() {
  const int sizes0[] = {2, 4};
  const int sizes1[] = {2, 4, 2};
  Dft_1Test_(false, SZ(sizes0), sizes0);
  Dft_1Test_(true, SZ(sizes1), sizes1);
#if USE_OPENMP
  Dft_1Test__(false, SZ(sizes0), sizes0, 2);
  Dft_1Test__(true, SZ(sizes1), sizes1, omp_get_max_threads());
#endif
}

static struct NS(ComplexFunction) *
CDftCooleyTurkey_DftFactory_create(struct NS(CDft_DftFactory) *p,
                                   struct NS(DataFactory) *data, bool backward,
                                   Long dft_size, Long x_stride, Long y_stride,
                                   bool inplace, int num_threads) {
  (void)p;
  (void)inplace;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(dft_size);
  return NS(CDftCooleyTurkey_Dft_new)(data, backward, &f, x_stride, y_stride,
                                      inplace, num_threads);
}

void NS(CDftCooleyTurkey_DftTest)() {
  struct NS(CDft_DftFactory) *factory =
      NS(CDft_DftFactory_new)(CDftCooleyTurkey_DftFactory_create, NULL, NULL);
  NS(CDft_DftTest_)(factory, 8 * 8);
  NS(CDft_DftTest_)(factory, 9 * 5);
  NS(CDft_DftTest_)(factory, 8 * 3 * 5 * 7);
  NS(CDft_DftFactory_delete)(factory);
}
