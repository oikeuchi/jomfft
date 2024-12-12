#include "InnerBatchDft.h"

#include <jomfft/math/IntegerMath.h>
#include <jomfft/math/PrimeFactors.h>

#include "../model/ComplexFunctionLoop.h"
#include "../model/ComplexFunctionTriple.h"
#include "../model/ComplexTranspose.h"
#include "BatchUtil.h"
#include "StrideBatchDft.h"
#include "core/InnerBatchDft.h"
#include "singleton/InnerBatchDft.h"

#define NS_(n) NS(CDft_##n)

static struct NS(ComplexFunction) *NS_(InnerBatchDft_new_)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long batch_size,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  Long n = dft_size;
  jomfftPrimeFactors_Factor(&n);
  if (n > 1) {
    if (dft_size < DFT_BLUESTEIN_MIN_SIZE) {
#if PRINT_MODEL
      printf("CDftSingleton_InnerBatchDft(%s,%d,%ld;%d)\n", BOOL_STR(backward),
             (int)dft_size, batch_size, num_threads);
#endif
      return NS(CDftSingleton_InnerBatchDft_new)(data, backward, (int)dft_size,
                                                 batch_size, x_stride, y_stride,
                                                 num_threads);
    }
  }
  if (dft_size < (Long)INT_MAX &&
      NS(CDftCore_InnerBatchDft_HasKey)((int)dft_size)) {
#if PRINT_MODEL
    printf("CDftCore_InnerBatchDft(%s,%d,%ld;%d)\n", BOOL_STR(backward),
           (int)dft_size, batch_size, num_threads);
#endif
    return NS(CDftCore_InnerBatchDft_new)(backward, (int)dft_size, batch_size,
                                          x_stride, y_stride, num_threads);
  }
  return NS_(StrideBatchDft_new)(data, backward, dft_size, batch_size, x_stride,
                                 1, y_stride, 1, inplace, num_threads);
}

static struct NS(ComplexFunction) *NS_(SquareInnerBatchDft_new_)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long stride,
    int num_threads) {
  return NS(ComplexFunctionTriple_0_new)(
      NS(Complex_Transpose_new_)(dft_size, stride, num_threads),
      NS_(StrideBatchDft_new)(data, backward, dft_size, dft_size, 1, stride, 1,
                              stride, true, num_threads),
      NS(Complex_Transpose_new_)(dft_size, stride, num_threads));
}

static struct NS(ComplexFunction) *NS_(SquareInnerBatchDft_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long batch_size,
    Long stride, int num_threads) {
  const Long loop_size = I_DIVIDE_FLOOR(batch_size, dft_size);
  if (loop_size == 0) {
    return NS_(StrideBatchDft_new)(data, backward, dft_size, batch_size, 1,
                                   stride, 1, stride, true, num_threads);
  } else {
    if (batch_size % dft_size) {
      const Long residual = batch_size - loop_size * dft_size;
      return NS(ComplexFunctionLoopR_new)(
          NS_(SquareInnerBatchDft_new_)(data, backward, dft_size, stride,
                                        num_threads),
          NS_(InnerBatchDft_new_)(data, backward, dft_size, residual, stride,
                                  stride, true, num_threads),
          loop_size, dft_size, dft_size);
    } else {
      return NS(ComplexFunctionLoop_new)(
          NS_(SquareInnerBatchDft_new_)(data, backward, dft_size, stride,
                                        num_threads),
          loop_size, dft_size, dft_size);
    }
  }
}

struct NS(ComplexFunction) *NS_(InnerBatchDft_new)(
    struct NS(DataFactory) *data, bool backward, Long dft_size, Long batch_size,
    Long x_stride, Long y_stride, bool inplace, int num_threads) {
  // Heuristic threshold.
  if (inplace && x_stride == y_stride && batch_size >= dft_size &&
      8 * dft_size >= C_L2_CACHE_SIZE && dft_size / num_threads >= 2048 &&
      NS_(IsBadStrideForCache)(dft_size, x_stride)) {
    return NS_(SquareInnerBatchDft_new)(data, backward, dft_size, batch_size,
                                        x_stride, num_threads);
  } else {
    return NS_(InnerBatchDft_new_)(data, backward, dft_size, batch_size,
                                   x_stride, y_stride, inplace, num_threads);
  }
}
