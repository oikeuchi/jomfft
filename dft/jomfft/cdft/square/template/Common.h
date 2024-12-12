#include <jomfft/VLA.h>

#include "../../../simd/Complex_V.h"
#include "../BlockSize.h"

#if USE_OPENMP
#include "../../../model/ThreadingUtil.h"
#endif

#define TWSZ (BLOCK_SIZE * BLOCK_SIZE)

static inline void Block_Transpose(Complex *x, Long stride) {
  for (Long i = 1; i < BLOCK_SIZE; ++i) {
    for (Long j = 0; j < i; ++j) {
      const Complex x0 = x[stride * i + j];
      const Complex x1 = x[stride * j + i];
      x[stride * j + i] = x0;
      x[stride * i + j] = x1;
    }
  }
}

static inline void Block_Copy(const Complex *x, Long x_stride, Complex *y,
                              Long y_stride) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      y[y_stride * i + j] = x[x_stride * i + j];
    }
  }
}
