#include "BatchUtil.h"

#define NS_(n) NS(CDft_##n)

Long NS_(IsBadStrideForCache)(Long dft_size, Long stride) {
  (void)dft_size;
  return stride % C_STRIDE_THRESHOLD == 0;
}

Long NS_(BatchChunkSize)(Long dft_size, Long batch_size, Long x_stride,
                         Long y_stride) {
  if (batch_size == 1) {
    return 1;
  }
  const int num_threads = jomfftGetMaxThreads();
  if (dft_size * C_MEMORY_BUS_SIZE >= C_L3_CACHE_SIZE / num_threads) {
    return 1;
  }
  if (dft_size * C_MEMORY_BUS_SIZE > C_L2_CACHE_SIZE) {
    return 1;
  }
  if (!NS_(IsBadStrideForCache)(dft_size, x_stride) &&
      !NS_(IsBadStrideForCache)(dft_size, y_stride) &&
      dft_size * C_MEMORY_BUS_SIZE >= C_L1_CACHE_SIZE) {
    return 1;
  }
  // A heuristic chunk size.
  if (dft_size * 2 * C_MEMORY_BUS_SIZE < C_L1_CACHE_SIZE) {
    return 2 * C_MEMORY_BUS_SIZE;
  } else {
    return C_MEMORY_BUS_SIZE;
  }
}
