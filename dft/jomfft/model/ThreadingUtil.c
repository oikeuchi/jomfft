#include "ThreadingUtil.h"

#include <jomfft/math/IntegerMath.h>
#include <jomfft/math/PrimeFactors.h>
#include <jomfft/util/IntVector.h>

#if USE_OPENMP

#define MIN_SIZE_PER_THREAD (8 * DFT_MIN_SIZE_PER_THREAD)
#define PREFERED_SIZE_PER_THREAD (16 * DFT_PREFERED_SIZE_PER_THREAD)

int NS(Threading_GetNumThreads)(Long batch_size, int block_size,
                                int num_threads) {
  return MAX((int)MIN(I_DIVIDE_CEIL(batch_size, block_size), (Long)num_threads),
             1);
}

void NS(Threading_GetBatchRange)(Long batch_size, int block_size,
                                 int num_threads, Long *range) {
  assert(batch_size > block_size);
  const Long num_blocks = batch_size / block_size;
  const Long r = num_blocks / num_threads;
  const Long q = num_blocks % num_threads;
  int i = 0;
  Long k = 0;
  for (; i < q; ++i, k += (r + 1) * block_size) {
    range[i] = k;
  }
  for (; i < num_threads; ++i, k += r * block_size) {
    range[i] = k;
  }
  range[num_threads] = k;
}

static int Threading_NumThreads(Long batch_size, Long dft_size, int cost) {
  return (int)MIN(batch_size * dft_size / cost, INT_MAX);
}

void NS(Threading_Split_)(int num_threads, Long batch_size_0, Long batch_size_1,
                          Long dft_size_1, int *num_threads_0,
                          int *num_threads_1, bool prefer_batch_0) {
  if (num_threads == 1) {
    *num_threads_0 = 1;
    *num_threads_1 = 1;
  } else if (batch_size_0 == 1) {
    *num_threads_0 = 1;
    *num_threads_1 = num_threads;
  } else {
    const int n =
        Threading_NumThreads(batch_size_1, dft_size_1, MIN_SIZE_PER_THREAD);
    const int n0 = Threading_NumThreads(batch_size_1, dft_size_1,
                                        PREFERED_SIZE_PER_THREAD);
    bool nested = false;
    if (prefer_batch_0) {
      if (n0 > 1) {
        *num_threads_0 = 1;
        *num_threads_1 = num_threads;
      } else if ((Long)num_threads <= batch_size_0) {
        *num_threads_0 = num_threads;
        *num_threads_1 = 1;
      } else if ((Long)num_threads <= n) {
        *num_threads_0 = 1;
        *num_threads_1 = num_threads;
      } else {
        nested = true;
      }
    } else {
      if (n0 > 1 || (Long)num_threads <= n) {
        *num_threads_0 = 1;
        *num_threads_1 = num_threads;
      } else if ((Long)num_threads <= batch_size_0) {
        *num_threads_0 = num_threads;
        *num_threads_1 = 1;
      } else {
        nested = true;
      }
    }
    if (nested) {
      if ((Long)n >= batch_size_0) {
        *num_threads_0 = 1;
        *num_threads_1 = num_threads;
      } else {
        *num_threads_0 = num_threads;
        *num_threads_1 = 1;
      }
    }
  }
}

#else

void NS(Threading_Split_)(int num_threads, Long batch_size_0, Long batch_size_1,
                          Long dft_size_1, int *num_threads_0,
                          int *num_threads_1, bool prefer_batch_0) {
  (void)num_threads;
  (void)batch_size_0;
  (void)batch_size_1;
  (void)dft_size_1;
  (void)prefer_batch_0;
  *num_threads_0 = 1;
  *num_threads_1 = 1;
}

#endif

void NS(Threading_Split)(int num_threads, Long batch_size_0, Long batch_size_1,
                         Long dft_size_1, int *num_threads_0,
                         int *num_threads_1) {
  NS(Threading_Split_)(num_threads, batch_size_0, batch_size_1, dft_size_1,
                       num_threads_0, num_threads_1, false);
}

void NS(Threading_GetTransposeBatchRange)(Long size, int block_size,
                                          int num_threads,
                                          struct LongPair *range) {
  assert(size % block_size == 0);
  const Long num_blocks_0 = size / block_size;
  const Long num_blocks = (num_blocks_0 * (num_blocks_0 + 1)) / 2;
  const Long r = num_blocks / num_threads;
  const Long q = num_blocks % num_threads;
  range[0] = (struct LongPair){0, 0};
  Long k = 1;
  Long c = 0;
  if (r == 0) {
    for (Long i = 0; i < size && k <= q; i += block_size) {
      for (Long j = 0; j <= i && k <= q; j += block_size, ++k) {
        range[k] = (struct LongPair){i, j};
      }
    }
    for (; k < num_threads; ++k) {
      range[k] = (struct LongPair){size, 0};
    }
  } else {
    Long num_blocks_per_thread = q == 0 ? r : r + 1;
    for (Long i = 0; i < size; i += block_size) {
      for (Long j = 0; j <= i; j += block_size, ++c) {
        if (c == num_blocks_per_thread) {
          range[k] = (struct LongPair){i, j};
          if (k == q) {
            num_blocks_per_thread -= 1;
          }
          ++k;
          c = 0;
        }
      }
    }
  }
  assert(k == num_threads);
  range[k] = (struct LongPair){size, 0};
}
