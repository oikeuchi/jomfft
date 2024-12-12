#include "ComplexTranspose.h"

#include "../model/ThreadingUtil.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#if C_SIMD_SIZE_S >= 2
#define CV_SIZE 2
#else
#define CV_SIZE 1
#endif
#include "../simd/Complex_V.h"

#define BLOCK_SIZE 32
#define BLOCK_MIN_SIZE 128

static void Transpose_(Long size, Complex *x, Long stride) {
#if CV_SIZE == 2
  Long size0 = I_FLOOR_MULTIPLE(size, 2);
  for (Long i = 0; i < size0; i += 2) {
    for (Long j = 0; j < i; j += 2) {
      CV_VECTOR x00 = CV_LOAD(&x[stride * i + j]);
      CV_VECTOR x01 = CV_LOAD(&x[stride * (i + 1) + j]);
      CV_TRANSPOSE_2(&x00, &x01);
      CV_VECTOR x10 = CV_LOAD(&x[stride * j + i]);
      CV_VECTOR x11 = CV_LOAD(&x[stride * (j + 1) + i]);
      CV_TRANSPOSE_2(&x10, &x11);
      CV_STORE(&x[stride * j + i], x00);
      CV_STORE(&x[stride * (j + 1) + i], x01);
      CV_STORE(&x[stride * i + j], x10);
      CV_STORE(&x[stride * (i + 1) + j], x11);
    }
    {
      Long i0 = i + 1;
      Long j0 = i;
      const Complex x0 = x[stride * i0 + j0];
      const Complex x1 = x[stride * j0 + i0];
      x[stride * j0 + i0] = x0;
      x[stride * i0 + j0] = x1;
    }
  }
  if (size0 < size) {
    Long i = size0;
    for (Long j = 0; j < i; ++j) {
      const Complex x0 = x[stride * i + j];
      const Complex x1 = x[stride * j + i];
      x[stride * j + i] = x0;
      x[stride * i + j] = x1;
    }
  }
#else
  for (Long i = 1; i < size; ++i) {
    for (Long j = 0; j < i; ++j) {
      CV_VECTOR x0 = CV_LOAD(&x[stride * i + j]);
      CV_VECTOR x1 = CV_LOAD(&x[stride * j + i]);
      CV_STORE(&x[stride * j + i], x0);
      CV_STORE(&x[stride * i + j], x1);
    }
  }
#endif
}

static inline void Block_Copy(const Complex *x, Long x_stride, Complex *y,
                              Long y_stride) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      y[y_stride * i + j] = x[x_stride * i + j];
    }
  }
}

static inline void Block_Diagonal(Complex *x, Long stride) {
#if CV_SIZE == 2
  for (int i = 0; i < BLOCK_SIZE; i += 2) {
    for (int j = 0; j < i; j += 2) {
      CV_VECTOR x00 = CV_LOAD(&x[stride * i + j]);
      CV_VECTOR x01 = CV_LOAD(&x[stride * (i + 1) + j]);
      CV_TRANSPOSE_2(&x00, &x01);
      CV_VECTOR x10 = CV_LOAD(&x[stride * j + i]);
      CV_VECTOR x11 = CV_LOAD(&x[stride * (j + 1) + i]);
      CV_TRANSPOSE_2(&x10, &x11);
      CV_STORE(&x[stride * j + i], x00);
      CV_STORE(&x[stride * (j + 1) + i], x01);
      CV_STORE(&x[stride * i + j], x10);
      CV_STORE(&x[stride * (i + 1) + j], x11);
    }
    {
      Long i0 = i + 1;
      Long j0 = i;
      const Complex x0 = x[stride * i0 + j0];
      const Complex x1 = x[stride * j0 + i0];
      x[stride * j0 + i0] = x0;
      x[stride * i0 + j0] = x1;
    }
  }
#else
  for (int i = 1; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < i; ++j) {
      CV_VECTOR x0 = CV_LOAD(&x[stride * i + j]);
      CV_VECTOR x1 = CV_LOAD(&x[stride * j + i]);
      CV_STORE(&x[stride * j + i], x0);
      CV_STORE(&x[stride * i + j], x1);
    }
  }
#endif
}

static inline void Block_Offdiagonal(Complex *x0, Complex *x1, Long stride) {
  DECLARE_VLA(Complex, z0, BLOCK_SIZE * BLOCK_SIZE);
  DECLARE_VLA(Complex, z1, BLOCK_SIZE * BLOCK_SIZE);
  Block_Copy(x0, stride, z0, BLOCK_SIZE);
  Block_Diagonal(z0, BLOCK_SIZE);
  Block_Copy(x1, stride, z1, BLOCK_SIZE);
  Block_Diagonal(z1, BLOCK_SIZE);
  Block_Copy(z0, BLOCK_SIZE, x1, stride);
  Block_Copy(z1, BLOCK_SIZE, x0, stride);
}

static void Block_Copy_(int block_size_0, int block_size_1, const Complex *x,
                        Long x_stride, Complex *y, Long y_stride) {
  for (int i = 0; i < block_size_0; ++i) {
    for (int j = 0; j < block_size_1; ++j) {
      y[y_stride * i + j] = x[x_stride * i + j];
    }
  }
}

static void Block_CopyTranspose_F(int block_size_0, int block_size_1,
                                  const Complex *x, Long x_stride, Complex *y,
                                  Long y_stride) {
  for (int i = 0; i < block_size_0; ++i) {
    for (int j = 0; j < block_size_1; ++j) {
      y[y_stride * j + i] = x[x_stride * i + j];
    }
  }
}

static void Block_CopyTranspose_B(int block_size_0, int block_size_1,
                                  const Complex *x, Long x_stride, Complex *y,
                                  Long y_stride) {
  for (int i = 0; i < block_size_0; ++i) {
    for (int j = 0; j < block_size_1; ++j) {
      y[y_stride * i + j] = x[x_stride * j + i];
    }
  }
}

static void Block_Diagonal_(int block_size, Complex *x, Long stride) {
  for (int i = 1; i < block_size; ++i) {
    for (int j = 0; j < i; ++j) {
      Complex x0 = x[stride * i + j];
      Complex x1 = x[stride * j + i];
      x[stride * i + j] = x1;
      x[stride * j + i] = x0;
    }
  }
}

static void Block_Offdiagonal_(int block_size_0, int block_size_1, Complex *x0,
                               Complex *x1, Long stride) {
  DECLARE_VLA(Complex, z0, block_size_0 * block_size_1);
  Block_CopyTranspose_F(block_size_0, block_size_1, x0, stride, z0,
                        block_size_0);
  Block_CopyTranspose_B(block_size_0, block_size_1, x1, stride, x0, stride);
  Block_Copy_(block_size_1, block_size_0, z0, block_size_0, x1, stride);
}

static void BlockTranspose(Long size, Complex *x, Long stride) {
  const Long size_ = I_FLOOR_MULTIPLE(size, BLOCK_SIZE);
  for (Long i = 0; i < size_; i += BLOCK_SIZE) {
    for (Long j = 0; j < i; j += BLOCK_SIZE) {
      Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
    }
    Block_Diagonal(&x[stride * i + i], stride);
  }
  if (size > size_) {
    const Long i = size_;
    const int block_size_0 = (int)(size - size_);
    for (Long j = 0; j < i; j += BLOCK_SIZE) {
      Block_Offdiagonal_(block_size_0, BLOCK_SIZE, &x[stride * i + j],
                         &x[stride * j + i], stride);
    }
    Block_Diagonal_(block_size_0, &x[stride * i + i], stride);
  }
}

#if USE_OPENMP

static void BlockTranspose_omp(Long size, Complex *x, Long stride,
                               int num_threads) {
  assert(size % BLOCK_SIZE == 0);
  DECLARE_VLA(struct LongPair, range, num_threads + 1);
  NS(Threading_GetTransposeBatchRange)
  (size, BLOCK_SIZE, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    struct LongPair begin = range[k];
    struct LongPair end = range[k + 1];
    Long i = begin.first;
    Long j = begin.second;
    if (begin.first < end.first) {
      {
        for (; j < i; j += BLOCK_SIZE) {
          Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
        }
        Block_Diagonal(&x[stride * i + i], stride);
        i += BLOCK_SIZE;
      }
      for (; i < end.first; i += BLOCK_SIZE) {
        for (j = 0; j < i; j += BLOCK_SIZE) {
          Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
        }
        Block_Diagonal(&x[stride * i + i], stride);
      }
      {
        for (j = 0; j < end.second; j += BLOCK_SIZE) {
          Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
        }
      }
    } else {
      for (; j < end.second; j += BLOCK_SIZE) {
        Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
      }
    }
  }
}

static void BlockTransposeR_omp(Long size, Complex *x, Long stride,
                                int num_threads) {
  assert(size % BLOCK_SIZE != 0);
  const Long size_0 = I_FLOOR_MULTIPLE(size, BLOCK_SIZE);
  const int block_size_0 = (int)(size - size_0);
  assert(block_size_0 > 0);
  DECLARE_VLA(struct LongPair, range, num_threads + 1);
  NS(Threading_GetTransposeBatchRange)
  (size_0 + BLOCK_SIZE, BLOCK_SIZE, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    struct LongPair begin = range[k];
    struct LongPair end = range[k + 1];
    Long i = begin.first;
    Long j = begin.second;
    if (begin.first < end.first) {
      if (begin.first < size_0) {
        const Long end_i = MIN(end.first, size_0);
        const Long end_j = end.first == size_0 ? 0 : end.second;
        {
          for (; j < i; j += BLOCK_SIZE) {
            Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
          }
          Block_Diagonal(&x[stride * i + i], stride);
          i += BLOCK_SIZE;
        }
        for (; i < end_i; i += BLOCK_SIZE) {
          for (j = 0; j < i; j += BLOCK_SIZE) {
            Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
          }
          Block_Diagonal(&x[stride * i + i], stride);
        }
        {
          for (j = 0; j < end_j; j += BLOCK_SIZE) {
            Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
          }
        }
      }
      if (i == size_0) {
        j = begin.first == size_0 ? begin.second : 0;
        const Long end_j_0 = end.first == size_0 ? end.second : size_0;
        for (; j < end_j_0; j += BLOCK_SIZE) {
          Block_Offdiagonal_(block_size_0, BLOCK_SIZE, &x[stride * i + j],
                             &x[stride * j + i], stride);
        }
        if (end.first > size_0) {
          Block_Diagonal_(block_size_0, &x[stride * i + i], stride);
        }
      }
    } else {
      if (i < size_0) {
        for (; j < end.second; j += BLOCK_SIZE) {
          Block_Offdiagonal(&x[stride * i + j], &x[stride * j + i], stride);
        }
      } else {
        for (; j < end.second; j += BLOCK_SIZE) {
          Block_Offdiagonal_(block_size_0, BLOCK_SIZE, &x[stride * i + j],
                             &x[stride * j + i], stride);
        }
      }
    }
  }
}

#endif

void NS(Complex_Transpose)(Long size, Complex *x, Long stride) {
  if (size >= BLOCK_MIN_SIZE) {
    BlockTranspose(size, x, stride);
  } else {
    Transpose_(size, x, stride);
  }
}

void NS(Complex_Transpose_)(Long size, Complex *x, Long stride,
                            int num_threads) {
  if (size >= BLOCK_MIN_SIZE) {
#if USE_OPENMP
    const Long bsz = I_DIVIDE_CEIL(size, BLOCK_SIZE);
    num_threads =
        NS(Threading_GetNumThreads)((bsz * (bsz + 1)) / 2, 1, num_threads);
    if (num_threads > 1) {
      if (size % BLOCK_SIZE) {
        BlockTransposeR_omp(size, x, stride, num_threads);
      } else {
        BlockTranspose_omp(size, x, stride, num_threads);
      }
    } else {
      BlockTranspose(size, x, stride);
    }
#else
    (void)num_threads;
    BlockTranspose(size, x, stride);
#endif
  } else {
    Transpose_(size, x, stride);
  }
}
