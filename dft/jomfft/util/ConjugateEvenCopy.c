#include "ConjugateEvenCopy.h"

#include "../math/Math.h"

static void Complex_CopyCE_(Long size, const Complex *x, Long x_stride,
                            Complex *y, Long y_stride) {
#define X(i) (x[x_stride * (i)])
#define Y(i) (y[y_stride * (i)])
  Y(0) = X(0);
  const Long n = (size + 1) / 2;
  for (Long i = 1; i < n; ++i) {
    Y(i) = X(i);
    Y(size - i) = C_CONJ(X(i));
  }
  if (size % 2 == 0) {
    Y(size / 2) = X(size / 2);
  }
#undef X
#undef Y
}

#if USE_OPENMP

static void Complex_CopyCE_omp(Long size, const Complex *x, Long x_stride,
                               Complex *y, Long y_stride, int num_threads) {
#define X(i) (x[x_stride * (i)])
#define Y(i) (y[y_stride * (i)])
  Y(0) = X(0);
  const Long n = (size + 1) / 2;
  Long i;
#pragma omp parallel for num_threads(num_threads)
  for (i = 1; i < n; ++i) {
    Y(i) = X(i);
    Y(size - i) = C_CONJ(X(i));
  }
  if (size % 2 == 0) {
    Y(size / 2) = X(size / 2);
  }
#undef X
#undef Y
}

#endif

void NS(Complex_CopyCE)(Long size, const Complex *x, Long x_stride, Complex *y,
                        Long y_stride) {
  Complex_CopyCE_(size, x, x_stride, y, y_stride);
}

void NS(Complex_CopyCE_)(Long size, const Complex *x, Long x_stride, Complex *y,
                         Long y_stride, int num_threads) {
#if USE_OPENMP
  if (num_threads > 1 && size > 1) {
    num_threads = (int)MIN((Long)num_threads, size);
    Complex_CopyCE_omp(size, x, x_stride, y, y_stride, num_threads);
  } else {
    Complex_CopyCE_(size, x, x_stride, y, y_stride);
  }
#else
  (void)num_threads;
  Complex_CopyCE_(size, x, x_stride, y, y_stride);
#endif
}
