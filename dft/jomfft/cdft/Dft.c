#include "Dft.h"

#include "bluestein/Dft.h"
#include "cooleyturkey/Dft.h"
#include "core/InnerBatchDft.h"
#include "singleton/InnerBatchDft.h"
#include "square/Dft.h"

#define NS_(n) NS(CDft_##n)

struct NS(ComplexFunction) *NS_(Dft_new)(struct NS(DataFactory) *data,
                                         bool backward, Long dft_size,
                                         Long x_stride, Long y_stride,
                                         bool inplace, int num_threads) {
  Long n = dft_size;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("CDftSingleton_InnerBatchDft(%s,%d,1;%d)\n", BOOL_STR(backward),
             (int)dft_size, num_threads);
#endif
      return NS(CDftSingleton_InnerBatchDft_new)(data, backward, (int)dft_size,
                                                 1, x_stride, y_stride, 1);
    } else {
      if (NS(CDftBluestein_ConvolutionSize)(dft_size) <= C_L1_CACHE_SIZE) {
        num_threads = 1;
      }
      return NS(CDftBluestein_Dft_new)(data, backward, dft_size, x_stride,
                                       y_stride, num_threads);
    }
  }
  if (dft_size <= (Long)INT_MAX &&
      NS(CDftCore_InnerBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("CDftCore_InnerBatchDft(%s,%d,1;%d)\n", BOOL_STR(backward),
           (int)dft_size, num_threads);
#endif
    return NS(CDftCore_InnerBatchDft_new)(backward, (int)dft_size, 1, x_stride,
                                          y_stride, 1);
  }
  if (dft_size <= C_L1_CACHE_SIZE) {
    num_threads = 1;
  }
  if (NS(CDftSquare_Dft_CanSplit)(&f, num_threads)) {
    return NS(CDftSquare_Dft_new)(data, backward, &f, x_stride, y_stride,
                                  inplace, num_threads);
  }
  return NS(CDftCooleyTurkey_Dft_new)(data, backward, &f, x_stride, y_stride,
                                      inplace, num_threads);
}
