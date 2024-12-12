#include "TransposeDftTest.h"

#include <jomfft/cdft/cooleyturkey/TransposeDft.h>
#include <jomfft/util/IntUtil.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../TransposeDftTest.h"

#define SZ(a) (sizeof(a) / sizeof(int))

static void TransposeDftTest(bool backward, Long dimension, const int *sizes,
                             Long batch_size) {
  CAPTURE_INTARRAY(dimension, sizes);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long dft_size = jomfftInt_ProductLong(dimension, sizes);
  struct NS(ComplexFunction) *func = NS(CDftCooleyTurkey_TransposeDft_new_)(
      data, backward, dimension, sizes, batch_size, batch_size, 1, dft_size);
  NS(CDft_TransposeDft_0Test)(func, backward, dft_size, batch_size);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

void NS(CDftCooleyTurkey_TransposeDftTest)() {
  const int sizes0[] = {2, 4};
  const int sizes1[] = {2, 4, 2};
  TransposeDftTest(false, SZ(sizes0), sizes0, 2);
  TransposeDftTest(true, SZ(sizes1), sizes1, 3);
}
