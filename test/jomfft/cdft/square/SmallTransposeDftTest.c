#include "SmallTransposeDftTest.h"

#include <jomfft/cdft/square/SmallTransposeDft.h>
#include <jomfft/util/IntUtil.h>
#include <jomfft/util/random/ComplexTensor.h>

#include "../TransposeDftTest.h"

#define SZ(a) (sizeof(a) / sizeof(int))

static void SmallTransposeDftTest(bool backward, int square_dft_size,
                                  int inner_dft_size, Long batch_size) {
  CAPTURE_INT(square_dft_size);
  CAPTURE_INT(inner_dft_size);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  const Long dft_size = square_dft_size * square_dft_size * inner_dft_size;
  struct NS(ComplexFunction) *func = NS(CDftSquare_SmallTransposeDft_new_)(
      data, backward, square_dft_size, inner_dft_size, batch_size, batch_size,
      1, dft_size);
  NS(CDft_TransposeDft_0Test)(func, backward, dft_size, batch_size);
  NS(ComplexFunction_delete)(func);
  NS(DataFactory_delete)(data);
}

void NS(CDftSquare_SmallTransposeDftTest)() {
  SmallTransposeDftTest(false, 4, 8, 2);
  SmallTransposeDftTest(true, 8, 4, 4);
}
