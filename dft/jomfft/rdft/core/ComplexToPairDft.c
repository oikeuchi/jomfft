#include "ComplexToPairDft.h"

#include "../../data/TwiddleVector.h"
#include "../../math/Math.h"
#include "../../model/ThreadingUtil.h"
#include "impl/ComplexToPairDft_B.h"
#include "impl/ComplexToPairDft_F.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(RDftCore_##n)

typedef void (*NS_(ComplexToPairDft_fn))(Long, Complex *, Complex *, Complex *,
                                         Complex *, Long);

struct NS_(ComplexToPairDftFunctions) {
#if C_SIMD_SIZE_S == 1
  NS_(ComplexToPairDft_fn) evaluate;
#elif C_SIMD_SIZE_S == 2
  NS_(ComplexToPairDft_fn) evaluate_1;
  NS_(ComplexToPairDft_fn) evaluate;
#elif C_SIMD_SIZE_S == 4
  NS_(ComplexToPairDft_fn) evaluate_1;
  NS_(ComplexToPairDft_fn) evaluate_2;
  NS_(ComplexToPairDft_fn) evaluate;
#endif
};

struct NS_(ComplexToPairDftData) {
  NS_(ComplexToPairDft_fn) evaluate;
  Long dft_size;
  Long stride_0;
  Long stride_1;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(ComplexToPairDftData) *NS_(ComplexToPairDftData_new)(
    NS_(ComplexToPairDft_fn) evaluate, Long dft_size, Long stride_0,
    Long stride_1, int num_threads) {
  assert(evaluate);
  struct NS_(ComplexToPairDftData) *out = NEW(struct NS_(ComplexToPairDftData));
  *out = (struct NS_(ComplexToPairDftData)){.evaluate = evaluate,
                                            .dft_size = dft_size,
                                            .stride_0 = stride_0,
                                            .stride_1 = stride_1};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(ComplexToPairDft_F_EvaluateHead)(Complex *x, Complex *y0,
                                                 Complex *y1) {
  *y0 = Complex_R(C_REAL(*x));
  *y1 = Complex_R(C_IMAG(*x));
}

static void NS_(ComplexToPairDft_B_EvaluateHead)(Complex *x0, Complex *x1,
                                                 Complex *y) {
  *y = Complex_(C_REAL(*x0), C_REAL(*x1));
}

static void NS_(ComplexToPairDft_F_evaluate)(struct NS(ComplexFunction) *p,
                                             Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftData) *data =
      (struct NS_(ComplexToPairDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *y0 = y;
  Complex *y1 = y + data->stride_1;
  NS_(ComplexToPairDft_F_EvaluateHead)(x, y0, y1);
  data->evaluate(batch_size, x + 1, x + dft_size, y0 + stride, y1 + stride,
                 stride);
}

#if USE_OPENMP

static void NS_(ComplexToPairDft_F_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                 Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftData) *data =
      (struct NS_(ComplexToPairDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *y0 = y;
  Complex *y1 = y + data->stride_1;
  NS_(ComplexToPairDft_F_EvaluateHead)(x, y0, y1);
  Complex *x0 = x + 1;
  Complex *x1 = x + dft_size;
  y0 += stride;
  y1 += stride;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (batch_size, C_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size_ = end - begin;
    Complex *x0_ = x0 + begin;
    Complex *x1_ = x1 - begin;
    Complex *y0_ = y0 + stride * begin;
    Complex *y1_ = y1 + stride * begin;
    data->evaluate(batch_size_, x0_, x1_, y0_, y1_, stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(ComplexToPairDft_F_new_)(
    NS_(ComplexToPairDft_fn) evaluate, Long dft_size, Long stride_0,
    Long stride_1, int num_threads) {
#if USE_OPENMP
  const Long batch_size = dft_size / 2;
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(ComplexToPairDft_F_evaluate_omp)
                                         : NS_(ComplexToPairDft_F_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(ComplexToPairDft_F_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(ComplexToPairDftData_new)(evaluate, dft_size, stride_0, stride_1,
                                    num_threads));
}

static void NS_(ComplexToPairDft_B_evaluate)(struct NS(ComplexFunction) *p,
                                             Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftData) *data =
      (struct NS_(ComplexToPairDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *x0 = x;
  Complex *x1 = x + data->stride_1;
  NS_(ComplexToPairDft_B_EvaluateHead)(x0, x1, y);
  data->evaluate(batch_size, x0 + stride, x1 + stride, y + 1, y + dft_size,
                 stride);
}

#if USE_OPENMP

static void NS_(ComplexToPairDft_B_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                 Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftData) *data =
      (struct NS_(ComplexToPairDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *x0 = x;
  Complex *x1 = x + data->stride_1;
  NS_(ComplexToPairDft_B_EvaluateHead)(x0, x1, y);
  x0 += stride;
  x1 += stride;
  Complex *y0 = y + 1;
  Complex *y1 = y + dft_size;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (batch_size, C_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size_ = end - begin;
    Complex *x0_ = x0 + stride * begin;
    Complex *x1_ = x1 + stride * begin;
    Complex *y0_ = y0 + begin;
    Complex *y1_ = y1 - begin;
    data->evaluate(batch_size_, x0_, x1_, y0_, y1_, stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(ComplexToPairDft_B_new_)(
    NS_(ComplexToPairDft_fn) evaluate, Long dft_size, Long stride_0,
    Long stride_1, int num_threads) {
#if USE_OPENMP
  const Long batch_size = dft_size / 2;
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(ComplexToPairDft_B_evaluate_omp)
                                         : NS_(ComplexToPairDft_B_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(ComplexToPairDft_B_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(ComplexToPairDftData_new)(evaluate, dft_size, stride_0, stride_1,
                                    num_threads));
}

#if C_SIMD_SIZE_S >= 2

struct NS_(ComplexToPairDftRData) {
  struct NS_(ComplexToPairDftFunctions) f;
  Long dft_size;
  Long stride_0;
  Long stride_1;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(ComplexToPairDftRData) *NS_(ComplexToPairDftRData_new)(
    const struct NS_(ComplexToPairDftFunctions) *f, Long dft_size,
    Long stride_0, Long stride_1, int num_threads) {
  struct NS_(ComplexToPairDftRData) *out =
      NEW(struct NS_(ComplexToPairDftRData));
  *out = (struct NS_(ComplexToPairDftRData)){.f = *f,
                                             .dft_size = dft_size,
                                             .stride_0 = stride_0,
                                             .stride_1 = stride_1};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static struct NS(ComplexFunction) *NS_(ComplexToPairDftR_new)(
    NS(ComplexFunction_fn) evaluate,
    const struct NS_(ComplexToPairDftFunctions) *f, Long dft_size,
    Long stride_0, Long stride_1, int num_threads) {
  const Long batch_size = dft_size / 2;
  struct NS_(ComplexToPairDftFunctions) f_ =
      (struct NS_(ComplexToPairDftFunctions)) {
#if C_SIMD_SIZE_S == 2
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate = (batch_size >= 2) ? f->evaluate : NULL
#elif C_SIMD_SIZE_S == 4
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate = (batch_size >= 4) ? f->evaluate : NULL
#endif
  };
  return NS(ComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(ComplexToPairDftRData_new)(&f_, dft_size, stride_0, stride_1,
                                     num_threads));
}

static void NS_(ComplexToPairDftR_F_EvaluateTail)(
    struct NS_(ComplexToPairDftRData) *data, Complex *x0, Complex *x1,
    Complex *y0, Complex *y1) {
  assert(data);
  const struct NS_(ComplexToPairDftFunctions) *f = &(data->f);
  const Long stride = data->stride_0;
#if C_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, x0, x1, y0, y1, stride);
  }
#elif C_SIMD_SIZE_S == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, x0, x1, y0, y1, stride);
    x0 += 2;
    x1 -= 2;
    y0 += stride * 2;
    y1 += stride * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x0, x1, y0, y1, stride);
  }
#endif
}

static void NS_(ComplexToPairDftR_F_evaluate)(struct NS(ComplexFunction) *p,
                                              Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftRData) *data =
      (struct NS_(ComplexToPairDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(ComplexToPairDftFunctions) *f = &(data->f);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *y0 = y;
  Complex *y1 = y + data->stride_1;
  NS_(ComplexToPairDft_F_EvaluateHead)(x, y0, y1);
  y0 += stride;
  y1 += stride;
  Complex *x0 = x + 1;
  Complex *x1 = x + dft_size;
  if (f->evaluate) {
    Long batch_size_ = I_FLOOR_MULTIPLE(batch_size, C_SIMD_SIZE_S);
    f->evaluate(batch_size_, x0, x1, y0, y1, stride);
    x0 += batch_size_;
    x1 -= batch_size_;
    y0 += stride * batch_size_;
    y1 += stride * batch_size_;
  }
  NS_(ComplexToPairDftR_F_EvaluateTail)(data, x0, x1, y0, y1);
}

#if USE_OPENMP

static void NS_(ComplexToPairDftR_F_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                  Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftRData) *data =
      (struct NS_(ComplexToPairDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(ComplexToPairDftFunctions) *f = &(data->f);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *y0 = y;
  Complex *y1 = y + data->stride_1;
  NS_(ComplexToPairDft_F_EvaluateHead)(x, y0, y1);
  y0 += stride;
  y1 += stride;
  Complex *x0 = x + 1;
  Complex *x1 = x + dft_size;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (batch_size, C_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for schedule(static) nowait
    for (k = 0; k < num_threads - 1; ++k) {
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      Complex *x0_ = x0 + begin;
      Complex *x1_ = x1 - begin;
      Complex *y0_ = y0 + stride * begin;
      Complex *y1_ = y1 + stride * begin;
      f->evaluate(batch_size_, x0_, x1_, y0_, y1_, stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      Complex *x0_ = x0 + begin;
      Complex *x1_ = x1 - begin;
      Complex *y0_ = y0 + stride * begin;
      Complex *y1_ = y1 + stride * begin;
      f->evaluate(batch_size_, x0_, x1_, y0_, y1_, stride);
      x0_ += batch_size_;
      x1_ -= batch_size_;
      y0_ += stride * batch_size_;
      y1_ += stride * batch_size_;
      NS_(ComplexToPairDftR_F_EvaluateTail)(data, x0_, x1_, y0_, y1_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(ComplexToPairDftR_F_new)(
    const struct NS_(ComplexToPairDftFunctions) *f, Long dft_size,
    Long stride_0, Long stride_1, int num_threads) {
#if USE_OPENMP
  const Long batch_size = dft_size / 2;
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate = (num_threads > 1)
                                        ? NS_(ComplexToPairDftR_F_evaluate_omp)
                                        : NS_(ComplexToPairDftR_F_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(ComplexToPairDftR_F_evaluate);
#endif
  return NS_(ComplexToPairDftR_new)(evaluate, f, dft_size, stride_0, stride_1,
                                    num_threads);
}

static void NS_(ComplexToPairDftR_B_EvaluateTail)(
    struct NS_(ComplexToPairDftRData) *data, Complex *x0, Complex *x1,
    Complex *y0, Complex *y1) {
  assert(data);
  const struct NS_(ComplexToPairDftFunctions) *f = &(data->f);
  const Long stride = data->stride_0;
#if C_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, x0, x1, y0, y1, stride);
  }
#elif C_SIMD_SIZE_S == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, x0, x1, y0, y1, stride);
    x0 += stride * 2;
    x1 += stride * 2;
    y0 += 2;
    y1 -= 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x0, x1, y0, y1, stride);
  }
#endif
}

static void NS_(ComplexToPairDftR_B_evaluate)(struct NS(ComplexFunction) *p,
                                              Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftRData) *data =
      (struct NS_(ComplexToPairDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(ComplexToPairDftFunctions) *f = &(data->f);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *x0 = x;
  Complex *x1 = x + data->stride_1;
  NS_(ComplexToPairDft_B_EvaluateHead)(x0, x1, y);
  x0 += stride;
  x1 += stride;
  Complex *y0 = y + 1;
  Complex *y1 = y + dft_size;
  if (f->evaluate) {
    Long batch_size_ = I_FLOOR_MULTIPLE(batch_size, C_SIMD_SIZE_S);
    f->evaluate(batch_size_, x0, x1, y0, y1, stride);
    x0 += stride * batch_size_;
    x1 += stride * batch_size_;
    y0 += batch_size_;
    y1 -= batch_size_;
  }
  NS_(ComplexToPairDftR_B_EvaluateTail)(data, x0, x1, y0, y1);
}

#if USE_OPENMP

static void NS_(ComplexToPairDftR_B_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                  Complex *x, Complex *y) {
  struct NS_(ComplexToPairDftRData) *data =
      (struct NS_(ComplexToPairDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(ComplexToPairDftFunctions) *f = &(data->f);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  const Long stride = data->stride_0;
  Complex *x0 = x;
  Complex *x1 = x + data->stride_1;
  NS_(ComplexToPairDft_B_EvaluateHead)(x0, x1, y);
  x0 += stride;
  x1 += stride;
  Complex *y0 = y + 1;
  Complex *y1 = y + dft_size;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (batch_size, C_SIMD_SIZE_S, num_threads, range);
#pragma omp parallel num_threads(num_threads)
  {
    {
      int k;
#pragma omp for schedule(static) nowait
      for (k = 0; k < num_threads - 1; ++k) {
        const Long begin = range[k];
        const Long end = range[k + 1];
        const Long batch_size_ = end - begin;
        Complex *x0_ = x0 + stride * begin;
        Complex *x1_ = x1 + stride * begin;
        Complex *y0_ = y0 + begin;
        Complex *y1_ = y1 - begin;
        f->evaluate(batch_size_, x0_, x1_, y0_, y1_, stride);
      }
    }
#pragma omp single nowait
    {
      const int k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      Complex *x0_ = x0 + stride * begin;
      Complex *x1_ = x1 + stride * begin;
      Complex *y0_ = y0 + begin;
      Complex *y1_ = y1 - begin;
      f->evaluate(batch_size_, x0_, x1_, y0_, y1_, stride);
      x0_ += stride * batch_size_;
      x1_ += stride * batch_size_;
      y0_ += batch_size_;
      y1_ -= batch_size_;
      NS_(ComplexToPairDftR_B_EvaluateTail)(data, x0_, x1_, y0_, y1_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(ComplexToPairDftR_B_new)(
    const struct NS_(ComplexToPairDftFunctions) *f, Long dft_size,
    Long stride_0, Long stride_1, int num_threads) {
#if USE_OPENMP
  const Long batch_size = dft_size / 2;
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate = (num_threads > 1)
                                        ? NS_(ComplexToPairDftR_B_evaluate_omp)
                                        : NS_(ComplexToPairDftR_B_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(ComplexToPairDftR_B_evaluate);
#endif
  return NS_(ComplexToPairDftR_new)(evaluate, f, dft_size, stride_0, stride_1,
                                    num_threads);
}

#endif

struct NS_(ComplexToPairDftFactory) {
  struct NS_(ComplexToPairDftFunctions) forward;
  struct NS_(ComplexToPairDftFunctions) backward;
};

#if C_SIMD_SIZE_S == 1
static const struct NS_(ComplexToPairDftFactory) g_factory = {
    {NS_(ComplexToPairDft_F_1)}, {NS_(ComplexToPairDft_B_1)}};
#elif C_SIMD_SIZE_S == 2
static const struct NS_(ComplexToPairDftFactory) g_factory = {
    {NS_(ComplexToPairDft_F_1), NS_(ComplexToPairDft_F_2)},
    {NS_(ComplexToPairDft_B_1), NS_(ComplexToPairDft_B_2)}};
#elif C_SIMD_SIZE_S == 4
static const struct NS_(ComplexToPairDftFactory) g_factory = {
    {NS_(ComplexToPairDft_F_1), NS_(ComplexToPairDft_F_2),
     NS_(ComplexToPairDft_F_4)},
    {NS_(ComplexToPairDft_B_1), NS_(ComplexToPairDft_B_2),
     NS_(ComplexToPairDft_B_4)}};
#endif

static struct NS(ComplexFunction) *NS_(ComplexToPairDftFactory_create_F)(
    const struct NS_(ComplexToPairDftFunctions) *f, Long dft_size,
    Long stride_0, Long stride_1, int num_threads) {
  assert(f);
#if C_SIMD_SIZE_S > 1
  const Long batch_size = dft_size / 2;
  if (batch_size % C_SIMD_SIZE_S) {
    return NS_(ComplexToPairDftR_F_new)(f, dft_size, stride_0, stride_1,
                                        num_threads);
  } else {
#else
  {
#endif
    return NS_(ComplexToPairDft_F_new_)(f->evaluate, dft_size, stride_0,
                                        stride_1, num_threads);
  }
}

static struct NS(ComplexFunction) *NS_(ComplexToPairDftFactory_create_B)(
    const struct NS_(ComplexToPairDftFunctions) *f, Long dft_size,
    Long stride_0, Long stride_1, int num_threads) {
  assert(f);
#if C_SIMD_SIZE_S > 1
  const Long batch_size = dft_size / 2;
  if (batch_size % C_SIMD_SIZE_S) {
    return NS_(ComplexToPairDftR_B_new)(f, dft_size, stride_0, stride_1,
                                        num_threads);
  } else {
#else
  {
#endif
    return NS_(ComplexToPairDft_B_new_)(f->evaluate, dft_size, stride_0,
                                        stride_1, num_threads);
  }
}

struct NS(ComplexFunction) *NS_(ComplexToPairDft_F_new)(Long dft_size,
                                                        Long stride_0,
                                                        Long stride_1,
                                                        int num_threads) {
  return NS_(ComplexToPairDftFactory_create_F)(&g_factory.forward, dft_size,
                                               stride_0, stride_1, num_threads);
}

struct NS(ComplexFunction) *NS_(ComplexToPairDft_B_new)(Long dft_size,
                                                        Long stride_0,
                                                        Long stride_1,
                                                        int num_threads) {
  return NS_(ComplexToPairDftFactory_create_B)(&g_factory.backward, dft_size,
                                               stride_0, stride_1, num_threads);
}
