#include "Common.h"

static inline void Block_Twiddle(const Complex *w, const Complex *x,
                                 Long x_stride, Complex *y, Long y_stride) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; j += CV_SIZE) {
      CV_VECTOR w0 = CV_LOAD(&w[BLOCK_SIZE * i + j]);
      CV_VECTOR z0 = CV_LOAD(&x[x_stride * i + j]);
      CV_STORE(&y[y_stride * i + j], CV_N_MUL(z0, w0));
    }
  }
}

static void Diagonal(const Complex *w, Complex *x, Long stride) {
  assert(BLOCK_SIZE % CV_SIZE == 0);
  DECLARE_ALIGNED_VLA(Complex, t, TWSZ);
  Block_Twiddle(w, x, stride, t, BLOCK_SIZE);
  Block_Transpose(t, BLOCK_SIZE);
  Block_Copy(t, BLOCK_SIZE, x, stride);
}

static void Offdiagonal(const Complex *w, Complex *x0, Complex *x1,
                        Long stride) {
  assert(BLOCK_SIZE % CV_SIZE == 0);
  DECLARE_ALIGNED_VLA(Complex, t, TWSZ);
  Block_Copy(x1, stride, t, BLOCK_SIZE);
  Block_Transpose(t, BLOCK_SIZE);
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; j += CV_SIZE) {
      CV_VECTOR w0 = CV_LOAD(&w[BLOCK_SIZE * i + j]);
      CV_VECTOR z0 = CV_LOAD(&x0[stride * i + j]);
      CV_STORE(&x0[stride * i + j],
               CV_N_MUL(CV_LOAD(&t[BLOCK_SIZE * i + j]), w0));
      CV_STORE(&t[BLOCK_SIZE * i + j], CV_N_MUL(z0, w0));
    }
  }
  Block_Transpose(t, BLOCK_SIZE);
  Block_Copy(t, BLOCK_SIZE, x1, stride);
}

static void TwiddleTranspose_(Long size, const Complex *w, Complex *x) {
  assert(size % BLOCK_SIZE == 0);
  const Long stride = size;
  for (Long i = 0; i < size; i += BLOCK_SIZE) {
    for (Long j = 0; j < i; j += BLOCK_SIZE, w += TWSZ) {
      Offdiagonal(w, &x[i * stride + j], &x[j * stride + i], stride);
    }
    Diagonal(w, &x[i * stride + i], stride);
    w += TWSZ;
  }
}

#if USE_OPENMP

static void Diagonal_omp(Long i, const Complex *w, Complex *x, Long stride) {
  const Long i_ = i * BLOCK_SIZE;
  const Complex *w_ = w + TWSZ * ((i * (i + 1)) / 2 + i);
  Diagonal(w_, &x[i_ * stride + i_], stride);
}

static void Offdiagonal_omp(Long i, Long j, const Complex *w, Complex *x,
                            Long stride) {
  const Long i_ = i * BLOCK_SIZE;
  const Long j_ = j * BLOCK_SIZE;
  const Complex *w_ = w + TWSZ * ((i * (i + 1)) / 2 + j);
  Offdiagonal(w_, &x[i_ * stride + j_], &x[j_ * stride + i_], stride);
}

static void TwiddleTranspose_omp(Long size, const Complex *w, Complex *x,
                                 int num_threads) {
  assert(size % BLOCK_SIZE == 0);
  const Long stride = size;
  const Long size_ = size / BLOCK_SIZE;
  DECLARE_VLA(struct LongPair, range, num_threads + 1);
  NS(Threading_GetTransposeBatchRange)(size_, 1, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    struct LongPair begin = range[k];
    struct LongPair end = range[k + 1];
    Long i = begin.first;
    Long j = begin.second;
    if (begin.first < end.first) {
      {
        for (; j < i; ++j) {
          Offdiagonal_omp(i, j, w, x, stride);
        }
        Diagonal_omp(i, w, x, stride);
        ++i;
      }
      for (; i < end.first; ++i) {
        for (j = 0; j < i; ++j) {
          Offdiagonal_omp(i, j, w, x, stride);
        }
        Diagonal_omp(i, w, x, stride);
      }
      {
        for (j = 0; j < end.second; ++j) {
          Offdiagonal_omp(i, j, w, x, stride);
        }
      }
    } else {
      for (; j < end.second; ++j) {
        Offdiagonal_omp(i, j, w, x, stride);
      }
    }
  }
}

#endif

void FUNCTION(Long size, const Complex *w, Complex *x, int num_threads) {
#if USE_OPENMP
  assert(size % BLOCK_SIZE == 0);
  const Long bsz = size / BLOCK_SIZE;
  num_threads =
      NS(Threading_GetNumThreads)((bsz * (bsz + 1)) / 2, 1, num_threads);
  if (num_threads > 1) {
    TwiddleTranspose_omp(size, w, x, num_threads);
  } else {
    TwiddleTranspose_(size, w, x);
  }
#else
  (void)num_threads;
  TwiddleTranspose_(size, w, x);
#endif
}
