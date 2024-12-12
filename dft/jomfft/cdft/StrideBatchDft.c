#include "StrideBatchDft.h"

#include <jomfft/math/IntegerMath.h>

#include "bluestein/BatchDft.h"
#include "cooleyturkey/BatchDft.h"
#include "core/StrideBatchDft.h"
#include "singleton/StrideBatchDft.h"
#include "square/BatchDft.h"

#define NS_(n) NS(CDft_##n)

struct NS(ComplexFunction) *NS_(StrideBatchDft_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("CDftSingleton_StrideBatchDft(%s,%d,%ld;%d)\n", BOOL_STR(backward),
             (int)dft_size, batch_size, num_threads);
#endif
      return NS(CDftSingleton_StrideBatchDft_new_)(
          data, backward, (int)dft_size, batch_size, x_stride_0, x_stride_1,
          y_stride_0, y_stride_1, num_threads);
    } else {
      return NS(CDftBluestein_BatchDft_new)(
          data, backward, dft_size, batch_size, x_stride_0, x_stride_1,
          y_stride_0, y_stride_1, num_threads);
    }
  }
  if (dft_size < (Long)INT_MAX &&
      NS(CDftCore_StrideBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("CDftCore_StrideBatchDft(%s,%d,%ld;%d)\n", BOOL_STR(backward),
           (int)dft_size, batch_size, num_threads);
#endif
    return NS(CDftCore_StrideBatchDft_new_)(backward, (int)dft_size, batch_size,
                                            x_stride_0, x_stride_1, y_stride_0,
                                            y_stride_1, num_threads);
  }
  if (NS(CDftSquare_BatchDft_CanSplit)(&f, num_threads)) {
    return NS(CDftSquare_BatchDft_new)(data, backward, &f, batch_size,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1, inplace, num_threads);
  }
  return NS(CDftCooleyTurkey_BatchDft_new)(data, backward, &f, batch_size,
                                           x_stride_0, x_stride_1, y_stride_0,
                                           y_stride_1, inplace, num_threads);
}
