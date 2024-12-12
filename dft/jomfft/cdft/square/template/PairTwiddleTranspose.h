#include "Common.h"

static inline void Block_TwiddleDft2Twiddle0(const Complex *v, const Complex *w,
                                             const Complex *x0,
                                             const Complex *x1, Long stride,
                                             Complex *y0, Complex *y1) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; j += CV_SIZE) {
      CV_VECTOR
      z0 = CV_LOAD(&x0[stride * i + j]);
      CV_VECTOR z1 = CV_LOAD(&x1[stride * i + j]);
      z1 = CV_N_MUL(z1, CV_LOAD(&v[j]));
      CV_DFT2(&z0, &z1);
      CV_VECTOR w0 = CV_LOAD(&w[BLOCK_SIZE * i + j]);
      z0 = CV_N_MUL(z0, w0);
      z1 = CV_N_MUL(z1, w0);
      CV_STORE(&y0[BLOCK_SIZE * i + j], z0);
      CV_STORE(&y1[BLOCK_SIZE * i + j], z1);
    }
  }
}

static inline void Block_TwiddleDft2Twiddle1(const Complex *v,
                                             const Complex *x0,
                                             const Complex *x1, Long stride,
                                             Complex *y0, Complex *y1) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; j += CV_SIZE) {
      CV_VECTOR z0 = CV_LOAD(&x0[stride * i + j]);
      CV_VECTOR z1 = CV_LOAD(&x1[stride * i + j]);
      z1 = CV_N_MUL(z1, CV_LOAD(&v[j]));
      CV_DFT2(&z0, &z1);
      CV_STORE(&y0[BLOCK_SIZE * i + j], z0);
      CV_STORE(&y1[BLOCK_SIZE * i + j], z1);
    }
  }
}

static inline void Block_Twiddle1(const Complex *v, const Complex *x,
                                  Long x_stride, Complex *y, Long y_stride) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; j += CV_SIZE) {
      CV_VECTOR z0 = CV_LOAD(&x[x_stride * i + j]);
      z0 = CV_N_MUL(z0, CV_LOAD(&v[j]));
      CV_STORE(&y[y_stride * i + j], z0);
    }
  }
}

static void Diagonal(const Complex *v, const Complex *w, Complex *x0,
                     Complex *x1, Long stride) {
  assert(BLOCK_SIZE % CV_SIZE == 0);
  DECLARE_ALIGNED_VLA(Complex, t0, TWSZ);
  DECLARE_ALIGNED_VLA(Complex, t1, TWSZ);
  Block_TwiddleDft2Twiddle0(v, w, x0, x1, stride, t0, t1);
  Block_Transpose(t0, BLOCK_SIZE);
  Block_Transpose(t1, BLOCK_SIZE);
  Block_Copy(t0, BLOCK_SIZE, x0, stride);
  Block_Twiddle1(v, t1, BLOCK_SIZE, x1, stride);
}

static void Offdiagonal(const Complex *v0, const Complex *v1, const Complex *w,
                        Complex *x0_0, Complex *x0_1, Complex *x1_0,
                        Complex *x1_1, Long stride) {
  assert(BLOCK_SIZE % CV_SIZE == 0);
  DECLARE_ALIGNED_VLA(Complex, t0, TWSZ);
  DECLARE_ALIGNED_VLA(Complex, t1, TWSZ);
  Block_TwiddleDft2Twiddle1(v1, x0_1, x1_1, stride, t0, t1);
  Block_Transpose(t0, BLOCK_SIZE);
  Block_Transpose(t1, BLOCK_SIZE);
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; j += CV_SIZE) {
      CV_VECTOR z0 = CV_LOAD(&x0_0[stride * i + j]);
      CV_VECTOR z1 = CV_LOAD(&x1_0[stride * i + j]);
      z1 = CV_N_MUL(z1, CV_LOAD(&v0[j]));
      CV_DFT2(&z0, &z1);
      CV_VECTOR w0 = CV_LOAD(&w[BLOCK_SIZE * i + j]);
      z0 = CV_N_MUL(z0, w0);
      z1 = CV_N_MUL(z1, w0);
      CV_VECTOR s0 = CV_LOAD(&t0[BLOCK_SIZE * i + j]);
      CV_VECTOR s1 = CV_LOAD(&t1[BLOCK_SIZE * i + j]);
      s0 = CV_N_MUL(s0, w0);
      s1 = CV_N_MUL(s1, w0);
      s1 = CV_N_MUL(s1, CV_LOAD(&v0[j]));
      CV_STORE(&x0_0[stride * i + j], s0);
      CV_STORE(&x1_0[stride * i + j], s1);
      CV_STORE(&t0[BLOCK_SIZE * i + j], z0);
      CV_STORE(&t1[BLOCK_SIZE * i + j], z1);
    }
  }
  Block_Transpose(t0, BLOCK_SIZE);
  Block_Transpose(t1, BLOCK_SIZE);
  Block_Copy(t0, BLOCK_SIZE, x0_1, stride);
  Block_Copy(t1, BLOCK_SIZE, x1_1, stride);
  Block_Twiddle1(v1, t1, BLOCK_SIZE, x1_1, stride);
}

static void PairTwiddleTranspose_(Long size, const Complex *v, const Complex *w,
                                  Complex *x) {
  assert(size % BLOCK_SIZE == 0);
  Complex *x0 = x;
  Complex *x1 = x + size;
  const Long stride = 2 * size;
  for (Long i = 0; i < size; i += BLOCK_SIZE) {
    for (Long j = 0; j < i; j += BLOCK_SIZE, w += TWSZ) {
      Offdiagonal(&v[j], &v[i], w, &x0[i * stride + j], &x0[j * stride + i],
                  &x1[i * stride + j], &x1[j * stride + i], stride);
    }
    Diagonal(&v[i], w, &x0[i * stride + i], &x1[i * stride + i], stride);
    w += TWSZ;
  }
}

#if USE_OPENMP

static void Diagonal_omp(Long i, const Complex *v, const Complex *w,
                         Complex *x0, Complex *x1, Long stride) {
  const Long i_ = i * BLOCK_SIZE;
  const Complex *w_ = w + TWSZ * ((i * (i + 1)) / 2 + i);
  Diagonal(&v[i_], w_, &x0[i_ * stride + i_], &x1[i_ * stride + i_], stride);
}

static void Offdiagonal_omp(Long i, Long j, const Complex *v, const Complex *w,
                            Complex *x0, Complex *x1, Long stride) {
  const Long i_ = i * BLOCK_SIZE;
  const Long j_ = j * BLOCK_SIZE;
  const Complex *w_ = w + TWSZ * ((i * (i + 1)) / 2 + j);
  Offdiagonal(&v[j_], &v[i_], w_, &x0[i_ * stride + j_], &x0[j_ * stride + i_],
              &x1[i_ * stride + j_], &x1[j_ * stride + i_], stride);
}

static void PairTwiddleTranspose_omp(Long size, const Complex *v,
                                     const Complex *w, Complex *x,
                                     int num_threads) {
  assert(size % BLOCK_SIZE == 0);
  Complex *x0 = x;
  Complex *x1 = x + size;
  const Long stride = 2 * size;
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
          Offdiagonal_omp(i, j, v, w, x0, x1, stride);
        }
        Diagonal_omp(i, v, w, x0, x1, stride);
        ++i;
      }
      for (; i < end.first; ++i) {
        for (j = 0; j < i; ++j) {
          Offdiagonal_omp(i, j, v, w, x0, x1, stride);
        }
        Diagonal_omp(i, v, w, x0, x1, stride);
      }
      {
        for (j = 0; j < end.second; ++j) {
          Offdiagonal_omp(i, j, v, w, x0, x1, stride);
        }
      }
    } else {
      for (; j < end.second; ++j) {
        Offdiagonal_omp(i, j, v, w, x0, x1, stride);
      }
    }
  }
}

#endif

void FUNCTION(Long size, const Complex *v, const Complex *w, Complex *x,
              int num_threads) {
#if USE_OPENMP
  assert(size % BLOCK_SIZE == 0);
  const Long bsz = size / BLOCK_SIZE;
  num_threads =
      NS(Threading_GetNumThreads)((bsz * (bsz + 1)) / 2, 1, num_threads);
  if (num_threads > 1) {
    PairTwiddleTranspose_omp(size, v, w, x, num_threads);
  } else {
    PairTwiddleTranspose_(size, v, w, x);
  }
#else
  (void)num_threads;
  PairTwiddleTranspose_(size, v, w, x);
#endif
}
