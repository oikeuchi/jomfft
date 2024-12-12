#include <stdlib.h>

#include <jomfft.h>

#if JOMFFT_USE_SINGLE
#define NS(n) jomfftf##n
#define Real float
#define Complex jomfftfComplex
#else
#define NS(n) jomfft##n
#define Real double
#define Complex jomfftComplex
#endif
#define Long long

static void NS(ComplexDftTest)() {
  int n = 64;
  NS(ComplexDftHandle) handle = 0;
  NS(ComplexDft)(&handle, 1, &n, jomfftInplace);
  Complex *x = malloc(sizeof(Complex) * n);
  for (int i = 0; i < n; ++i) {
    x[i] = (2 * i + 3) % 5;
  }
  NS(ComplexDftForward)(handle, x, x);
  NS(ComplexDftBackward)(handle, x, x);
  NS(ComplexDftDestruct)(handle);
}

static void NS(ComplexDftBatchTest)() {
  int b = 4;
  int n = 64;
  NS(ComplexDftHandle) handle = 0;
  NS(ComplexDftBatch)(&handle, 1, &n, b, &n, 1, n, &n, 1, n, jomfftInplace);
  Complex *x = malloc(sizeof(Complex) * b * n);
  for (int i = 0; i < b * n; ++i) {
    x[i] = (2 * i + 3) % 5;
  }
  NS(ComplexDftForward)(handle, x, x);
  NS(ComplexDftBackward)(handle, x, x);
  NS(ComplexDftDestruct)(handle);
}

static void NS(ComplexDftBatch_Test)() {
  Long b = 4;
  Long n = 64;
  Long s = 1;
  NS(ComplexDftHandle) handle = 0;
  NS(ComplexDftBatch_)(&handle, 1, &n, b, &s, n, &s, n, jomfftInplace);
  Complex *x = malloc(sizeof(Complex) * b * n);
  for (Long i = 0; i < b * n; ++i) {
    x[i] = (2 * i + 3) % 5;
  }
  NS(ComplexDftForward)(handle, x, x);
  NS(ComplexDftBackward)(handle, x, x);
  NS(ComplexDftDestruct)(handle);
}

static void NS(RealDftTest)() {
  int n = 64;
  int n_ce = n / 2 + 1;
  NS(RealDftHandle) handle = 0;
  NS(RealDft)(&handle, 1, &n, jomfftInplace);
  Complex *y = malloc(sizeof(Complex) * n_ce);
  Real *x = (Real *)y;
  for (int i = 0; i < n; ++i) {
    x[i] = (2 * i + 3) % 5;
  }
  NS(RealDftForward)(handle, x, y);
  NS(RealDftBackward)(handle, y, x);
  NS(RealDftDestruct)(handle);
}

static void NS(RealDftBatchTest)() {
  int b = 4;
  int n = 64;
  int n_ce = n / 2 + 1;
  int n_r = 2 * n_ce;
  NS(RealDftHandle) handle = 0;
  NS(RealDftBatch)(&handle, 1, &n, b, &n_r, 1, n_r, &n_ce, 1, n_ce,
                   jomfftInplace);
  Complex *y = malloc(sizeof(Complex) * b * n_ce);
  Real *x = (Real *)y;
  for (int i = 0; i < b; ++i) {
    for (int j = 0; j < n; ++j) {
      x[n_r * i + j] = (2 * j + 3) % 5;
    }
  }
  NS(RealDftForward)(handle, x, y);
  NS(RealDftBackward)(handle, y, x);
  NS(RealDftDestruct)(handle);
}

static void NS(RealDftBatch_Test)() {
  Long b = 4;
  Long n = 64;
  Long n_ce = n / 2 + 1;
  Long n_r = 2 * n_ce;
  Long s = 1;
  NS(RealDftHandle) handle = 0;
  NS(RealDftBatch_)(&handle, 1, &n, b, &s, n_r, &s, n_ce, jomfftInplace);
  Complex *y = malloc(sizeof(Complex) * b * n_ce);
  Real *x = (Real *)y;
  for (Long i = 0; i < b; ++i) {
    for (Long j = 0; j < n; ++j) {
      x[n_r * i + j] = (2 * j + 3) % 5;
    }
  }
  NS(RealDftForward)(handle, x, y);
  NS(RealDftBackward)(handle, y, x);
  NS(RealDftDestruct)(handle);
}

int main() {
  NS(ComplexDftTest)();
  NS(ComplexDftBatchTest)();
  NS(ComplexDftBatch_Test)();
  NS(RealDftTest)();
  NS(RealDftBatchTest)();
  NS(RealDftBatch_Test)();
  return 0;
}
