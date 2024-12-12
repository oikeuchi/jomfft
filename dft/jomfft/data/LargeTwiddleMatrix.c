#include "LargeTwiddleMatrix.h"

#include <jomfft/VLA.h>

#include "../math/Math.h"
#include "../model/ThreadingUtil.h"

bool NS(LargeTwiddleMatrixKey_equal)(struct NS(LargeTwiddleMatrixKey) a,
                                     struct NS(LargeTwiddleMatrixKey) b) {
  return a.size == b.size && a.twiddle_size == b.twiddle_size &&
         a.block_size == b.block_size;
}

static void FillTwiddleBlock(int block_size, Long i, Long j, Real theta,
                             Complex *data) {
  const int bsq = block_size * block_size;
  Complex *x = data + bsq * ((i * (i + 1)) / 2 + j);
  for (int k = 0; k < block_size; ++k) {
    for (int l = 0; l < block_size; ++l, ++x) {
      const Long i_ = block_size * i + k;
      const Long j_ = block_size * j + l;
      *x = NS(Complex_fromArgument)(theta * i_ * j_);
    }
  }
}

Complex *NS(LargeTwiddleMatrix_new)(struct NS(LargeTwiddleMatrixKey) key) {
  assert(key.size % key.block_size == 0);
  const Long size = key.size;
  const Long twiddle_size = key.twiddle_size;
  const int block_size = key.block_size;
  Complex *data = NULL;
  const Long n = size / block_size;
  const int bsq = block_size * block_size;
  const Long data_size = ((n * (n + 1)) / 2) * bsq;
  if (data_size) {
    data = ARRAY_NEW(Complex, data_size);
    const Real theta = ((Real)(-2 * M_PI)) / twiddle_size;
#if USE_OPENMP
    const int num_threads = jomfftGetMaxThreads();
    DECLARE_VLA(struct LongPair, range, num_threads + 1);
    NS(Threading_GetTransposeBatchRange)(n, 1, num_threads, range);
    int k;
#pragma omp parallel for num_threads(num_threads)
    for (k = 0; k < num_threads; ++k) {
      struct LongPair begin = range[k];
      struct LongPair end = range[k + 1];
      Long i = begin.first;
      Long j = begin.second;
      if (begin.first < end.first) {
        {
          for (; j <= i; ++j) {
            FillTwiddleBlock(block_size, i, j, theta, data);
          }
        }
        for (; i < end.first; ++i) {
          for (j = 0; j <= i; ++j) {
            FillTwiddleBlock(block_size, i, j, theta, data);
          }
        }
        {
          for (j = 0; j < end.second; ++j) {
            FillTwiddleBlock(block_size, i, j, theta, data);
          }
        }
      } else {
        for (; j < end.second; ++j) {
          FillTwiddleBlock(block_size, i, j, theta, data);
        }
      }
    }
#else
    for (Long i = 0; i < n; ++i) {
      for (Long j = 0; j <= i; ++j) {
        FillTwiddleBlock(block_size, i, j, theta, data);
      }
    }
#endif
  }
  return data;
}

void NS(LargeTwiddleMatrix_delete)(Complex *p) {
  if (p) {
    ARRAY_DELETE(p);
  }
}
