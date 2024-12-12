#include "StrideBatchDft.h"

#include "../../data/TwiddleMatrix.h"
#include "../../model/ThreadingUtil.h"
#include "impl/StrideBatchDft_B0.h"
#include "impl/StrideBatchDft_B1.h"
#include "impl/StrideBatchDft_F0.h"
#include "impl/StrideBatchDft_F1.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(RDftSingleton_##n)

typedef void (*NS_(StrideBatchDft_F_fn))(int, const Complex *, Long, Real *,
                                         Long, Long, Complex *, Long, Long);

typedef void (*NS_(StrideBatchDft_B_fn))(int, const Complex *, Long, Complex *,
                                         Long, Long, Real *, Long, Long);

struct NS_(StrideBatchDft_FFunctions) {
#if R_SIMD_SIZE_S == 1
  NS_(StrideBatchDft_F_fn) evaluate;
#elif R_SIMD_SIZE_S == 2
  NS_(StrideBatchDft_F_fn) evaluate_1;
  NS_(StrideBatchDft_F_fn) evaluate;
#elif R_SIMD_SIZE_S == 4
  NS_(StrideBatchDft_F_fn) evaluate_1;
  NS_(StrideBatchDft_F_fn) evaluate_2;
  NS_(StrideBatchDft_F_fn) evaluate;
#elif R_SIMD_SIZE_S == 8
  NS_(StrideBatchDft_F_fn) evaluate_1;
  NS_(StrideBatchDft_F_fn) evaluate_2;
  NS_(StrideBatchDft_F_fn) evaluate_4;
  NS_(StrideBatchDft_F_fn) evaluate;
#endif
};

struct NS_(StrideBatchDft_BFunctions) {
#if R_SIMD_SIZE_S == 1
  NS_(StrideBatchDft_B_fn) evaluate;
#elif R_SIMD_SIZE_S == 2
  NS_(StrideBatchDft_B_fn) evaluate_1;
  NS_(StrideBatchDft_B_fn) evaluate;
#elif R_SIMD_SIZE_S == 4
  NS_(StrideBatchDft_B_fn) evaluate_1;
  NS_(StrideBatchDft_B_fn) evaluate_2;
  NS_(StrideBatchDft_B_fn) evaluate;
#elif R_SIMD_SIZE_S == 8
  NS_(StrideBatchDft_B_fn) evaluate_1;
  NS_(StrideBatchDft_B_fn) evaluate_2;
  NS_(StrideBatchDft_B_fn) evaluate_4;
  NS_(StrideBatchDft_B_fn) evaluate;
#endif
};

struct NS_(StrideBatchDft_FData) {
  NS_(StrideBatchDft_F_fn) evaluate;
  int dft_size;
  const Complex *w;
  Long batch_size;
  Long x_stride_0;
  Long x_stride_1;
  Long y_stride_0;
  Long y_stride_1;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(StrideBatchDft_FData) *NS_(StrideBatchDft_FData_new)(
    NS_(StrideBatchDft_F_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
  assert(evaluate);
  struct NS_(StrideBatchDft_FData) *out = NEW(struct NS_(StrideBatchDft_FData));
  *out = (struct NS_(StrideBatchDft_FData)){.evaluate = evaluate,
                                            .dft_size = dft_size,
                                            .w = w,
                                            .batch_size = batch_size,
                                            .x_stride_0 = x_stride_0,
                                            .x_stride_1 = x_stride_1,
                                            .y_stride_0 = y_stride_0,
                                            .y_stride_1 = y_stride_1};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(StrideBatchDft_F_evaluate)(struct NS(RealToComplexFunction) *p,
                                           Real *x, Complex *y) {
  struct NS_(StrideBatchDft_FData) *data =
      (struct NS_(StrideBatchDft_FData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  data->evaluate(data->dft_size, data->w, data->batch_size, x, data->x_stride_0,
                 data->x_stride_1, y, data->y_stride_0, data->y_stride_1);
}

#if USE_OPENMP

static void NS_(StrideBatchDft_F_evaluate_omp)(
    struct NS(RealToComplexFunction) *p, Real *x, Complex *y) {
  struct NS_(StrideBatchDft_FData) *data =
      (struct NS_(StrideBatchDft_FData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, R_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads) schedule(static)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Real *x_ = x + data->x_stride_1 * begin;
    Complex *y_ = y + data->y_stride_1 * begin;
    data->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                   data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
  }
}

#endif

static struct NS(RealToComplexFunction) *NS_(StrideBatchDft_F_new_)(
    NS_(StrideBatchDft_F_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, R_SIMD_SIZE_S, num_threads);
  NS(RealToComplexFunction_fn) evaluate_ =
      (num_threads > 1) ? NS_(StrideBatchDft_F_evaluate_omp)
                        : NS_(StrideBatchDft_F_evaluate);
#else
  NS(RealToComplexFunction_fn) evaluate_ = NS_(StrideBatchDft_F_evaluate);
#endif
  return NS(RealToComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(StrideBatchDft_FData_new)(evaluate, dft_size, w, batch_size,
                                    x_stride_0, x_stride_1, y_stride_0,
                                    y_stride_1, num_threads));
}

struct NS_(StrideBatchDftR_FData) {
  struct NS_(StrideBatchDft_FFunctions) f;
  int dft_size;
  const Complex *w;
  Long batch_size;
  Long x_stride_0;
  Long x_stride_1;
  Long y_stride_0;
  Long y_stride_1;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(StrideBatchDftR_FData) *NS_(StrideBatchDftR_FData_new)(
    const struct NS_(StrideBatchDft_FFunctions) *f, int dft_size,
    const Complex *w, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
  struct NS_(StrideBatchDftR_FData) *out =
      NEW(struct NS_(StrideBatchDftR_FData));
  *out = (struct NS_(StrideBatchDftR_FData)){.f = *f,
                                             .dft_size = dft_size,
                                             .w = w,
                                             .batch_size = batch_size,
                                             .x_stride_0 = x_stride_0,
                                             .x_stride_1 = x_stride_1,
                                             .y_stride_0 = y_stride_0,
                                             .y_stride_1 = y_stride_1};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(StrideBatchDftR_F_EvaluateTail)(
    struct NS_(StrideBatchDftR_FData) *data, Real *x, Complex *y) {
  assert(data);
  const struct NS_(StrideBatchDft_FFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
#if R_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 4
  if (f->evaluate_2) {
    f->evaluate_2(dft_size, w, 2, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 8
  if (f->evaluate_4) {
    f->evaluate_4(dft_size, w, 4, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * 4;
    y += data->y_stride_1 * 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(dft_size, w, 2, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
  }
#endif
}

static void NS_(StrideBatchDftR_F_evaluate)(struct NS(RealToComplexFunction) *p,
                                            Real *x, Complex *y) {
  struct NS_(StrideBatchDftR_FData) *data =
      (struct NS_(StrideBatchDftR_FData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  const struct NS_(StrideBatchDft_FFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, R_SIMD_SIZE_S);
    f->evaluate(dft_size, w, batch_size, x, data->x_stride_0, data->x_stride_1,
                y, data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * batch_size;
    y += data->y_stride_1 * batch_size;
  }
  NS_(StrideBatchDftR_F_EvaluateTail)(data, x, y);
}

#if USE_OPENMP

static void NS_(StrideBatchDftR_F_evaluate_omp)(
    struct NS(RealToComplexFunction) *p, Real *x, Complex *y) {
  struct NS_(StrideBatchDftR_FData) *data =
      (struct NS_(StrideBatchDftR_FData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  const struct NS_(StrideBatchDft_FFunctions) *f = &(data->f);
  assert(f->evaluate);
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, R_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for schedule(static) nowait
    for (k = 0; k < num_threads - 1; ++k) {
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Real *x_ = x + data->x_stride_1 * begin;
      Complex *y_ = y + data->y_stride_1 * begin;
      f->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                  data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Real *x_ = x + data->x_stride_1 * begin;
      Complex *y_ = y + data->y_stride_1 * begin;
      f->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                  data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
      x_ += data->x_stride_1 * batch_size;
      y_ += data->y_stride_1 * batch_size;
      NS_(StrideBatchDftR_F_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(RealToComplexFunction) *NS_(StrideBatchDftR_F_new)(
    const struct NS_(StrideBatchDft_FFunctions) *f, int dft_size,
    const Complex *w, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, R_SIMD_SIZE_S, num_threads);
  NS(RealToComplexFunction_fn) evaluate =
      (num_threads > 1) ? NS_(StrideBatchDftR_F_evaluate_omp)
                        : NS_(StrideBatchDftR_F_evaluate);
#else
  NS(RealToComplexFunction_fn) evaluate = NS_(StrideBatchDftR_F_evaluate);
#endif
  struct NS_(StrideBatchDft_FFunctions) f_ =
      (struct NS_(StrideBatchDft_FFunctions)) {
#if R_SIMD_SIZE_S == 2
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate = (batch_size >= 2) ? f->evaluate : NULL
#elif R_SIMD_SIZE_S == 4
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate = (batch_size >= 4) ? f->evaluate : NULL
#elif R_SIMD_SIZE_S == 8
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate_4 = (batch_size % 8 >= 4) ? f->evaluate_4 : NULL,
    .evaluate = (batch_size >= 8) ? f->evaluate : NULL
#endif
  };
  return NS(RealToComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(StrideBatchDftR_FData_new)(&f_, dft_size, w, batch_size, x_stride_0,
                                     x_stride_1, y_stride_0, y_stride_1,
                                     num_threads));
}

struct NS_(StrideBatchDft_BData) {
  NS_(StrideBatchDft_B_fn) evaluate;
  int dft_size;
  const Complex *w;
  Long batch_size;
  Long x_stride_0;
  Long x_stride_1;
  Long y_stride_0;
  Long y_stride_1;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(StrideBatchDft_BData) *NS_(StrideBatchDft_BData_new)(
    NS_(StrideBatchDft_B_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
  assert(evaluate);
  struct NS_(StrideBatchDft_BData) *out = NEW(struct NS_(StrideBatchDft_BData));
  *out = (struct NS_(StrideBatchDft_BData)){.evaluate = evaluate,
                                            .batch_size = batch_size,
                                            .dft_size = dft_size,
                                            .w = w,
                                            .x_stride_0 = x_stride_0,
                                            .x_stride_1 = x_stride_1,
                                            .y_stride_0 = y_stride_0,
                                            .y_stride_1 = y_stride_1};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(StrideBatchDft_B_evaluate)(struct NS(ComplexToRealFunction) *p,
                                           Complex *x, Real *y) {
  struct NS_(StrideBatchDft_BData) *data =
      (struct NS_(StrideBatchDft_BData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  data->evaluate(data->dft_size, data->w, data->batch_size, x, data->x_stride_0,
                 data->x_stride_1, y, data->y_stride_0, data->y_stride_1);
}

#if USE_OPENMP

static void NS_(StrideBatchDft_B_evaluate_omp)(
    struct NS(ComplexToRealFunction) *p, Complex *x, Real *y) {
  struct NS_(StrideBatchDft_BData) *data =
      (struct NS_(StrideBatchDft_BData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, R_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads) schedule(static)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Complex *x_ = x + data->x_stride_1 * begin;
    Real *y_ = y + data->y_stride_1 * begin;
    data->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                   data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
  }
}

#endif

static struct NS(ComplexToRealFunction) *NS_(StrideBatchDft_B_new_)(
    NS_(StrideBatchDft_B_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, R_SIMD_SIZE_S, num_threads);
  NS(ComplexToRealFunction_fn) evaluate_ =
      (num_threads > 1) ? NS_(StrideBatchDft_B_evaluate_omp)
                        : NS_(StrideBatchDft_B_evaluate);
#else
  NS(ComplexToRealFunction_fn) evaluate_ = NS_(StrideBatchDft_B_evaluate);
#endif
  return NS(ComplexToRealFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(StrideBatchDft_BData_new)(evaluate, dft_size, w, batch_size,
                                    x_stride_0, x_stride_1, y_stride_0,
                                    y_stride_1, num_threads));
}

struct NS_(StrideBatchDftR_BData) {
  struct NS_(StrideBatchDft_BFunctions) f;
  int dft_size;
  const Complex *w;
  Long batch_size;
  Long x_stride_0;
  Long x_stride_1;
  Long y_stride_0;
  Long y_stride_1;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(StrideBatchDftR_BData) *NS_(StrideBatchDftR_BData_new)(
    const struct NS_(StrideBatchDft_BFunctions) *f, int dft_size,
    const Complex *w, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
  struct NS_(StrideBatchDftR_BData) *out =
      NEW(struct NS_(StrideBatchDftR_BData));
  *out = (struct NS_(StrideBatchDftR_BData)){.f = *f,
                                             .dft_size = dft_size,
                                             .w = w,
                                             .batch_size = batch_size,
                                             .x_stride_0 = x_stride_0,
                                             .x_stride_1 = x_stride_1,
                                             .y_stride_0 = y_stride_0,
                                             .y_stride_1 = y_stride_1};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(StrideBatchDftR_B_EvaluateTail)(
    struct NS_(StrideBatchDftR_BData) *data, Complex *x, Real *y) {
  assert(data);
  const struct NS_(StrideBatchDft_BFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
#if R_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 4
  if (f->evaluate_2) {
    f->evaluate_2(dft_size, w, 2, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 8
  if (f->evaluate_4) {
    f->evaluate_4(dft_size, w, 4, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * 4;
    y += data->y_stride_1 * 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(dft_size, w, 2, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
  }
#endif
}

static void NS_(StrideBatchDftR_B_evaluate)(struct NS(ComplexToRealFunction) *p,
                                            Complex *x, Real *y) {
  struct NS_(StrideBatchDftR_BData) *data =
      (struct NS_(StrideBatchDftR_BData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  const struct NS_(StrideBatchDft_BFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, R_SIMD_SIZE_S);
    f->evaluate(dft_size, w, batch_size, x, data->x_stride_0, data->x_stride_1,
                y, data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * batch_size;
    y += data->y_stride_1 * batch_size;
  }
  NS_(StrideBatchDftR_B_EvaluateTail)(data, x, y);
}

#if USE_OPENMP

static void NS_(StrideBatchDftR_B_evaluate_omp)(
    struct NS(ComplexToRealFunction) *p, Complex *x, Real *y) {
  struct NS_(StrideBatchDftR_BData) *data =
      (struct NS_(StrideBatchDftR_BData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  const struct NS_(StrideBatchDft_BFunctions) *f = &(data->f);
  assert(f->evaluate);
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, R_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for schedule(static) nowait
    for (k = 0; k < num_threads - 1; ++k) {
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + data->x_stride_1 * begin;
      Real *y_ = y + data->y_stride_1 * begin;
      f->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                  data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + data->x_stride_1 * begin;
      Real *y_ = y + data->y_stride_1 * begin;
      f->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                  data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
      x_ += data->x_stride_1 * batch_size;
      y_ += data->y_stride_1 * batch_size;
      NS_(StrideBatchDftR_B_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexToRealFunction) *NS_(StrideBatchDftR_B_new)(
    const struct NS_(StrideBatchDft_BFunctions) *f, int dft_size,
    const Complex *w, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, R_SIMD_SIZE_S, num_threads);
  NS(ComplexToRealFunction_fn) evaluate =
      (num_threads > 1) ? NS_(StrideBatchDftR_B_evaluate_omp)
                        : NS_(StrideBatchDftR_B_evaluate);
#else
  NS(ComplexToRealFunction_fn) evaluate = NS_(StrideBatchDftR_B_evaluate);
#endif
  struct NS_(StrideBatchDft_BFunctions) f_ =
      (struct NS_(StrideBatchDft_BFunctions)) {
#if R_SIMD_SIZE_S == 2
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate = (batch_size >= 2) ? f->evaluate : NULL
#elif R_SIMD_SIZE_S == 4
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate = (batch_size >= 4) ? f->evaluate : NULL
#elif R_SIMD_SIZE_S == 8
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate_4 = (batch_size % 8 >= 4) ? f->evaluate_4 : NULL,
    .evaluate = (batch_size >= 8) ? f->evaluate : NULL
#endif
  };
  return NS(ComplexToRealFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(StrideBatchDftR_BData_new)(&f_, dft_size, w, batch_size, x_stride_0,
                                     x_stride_1, y_stride_0, y_stride_1,
                                     num_threads));
}

struct NS_(StrideBatchDftFactory) {
  int dft_size;
  struct NS_(StrideBatchDft_FFunctions) forward;
  struct NS_(StrideBatchDft_BFunctions) backward;
};

#if R_SIMD_SIZE_S == 1
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(StrideBatchDft_F##n##_1)}, { NS_(StrideBatchDft_B##n##_1) }        \
  }
#elif R_SIMD_SIZE_S == 2
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(StrideBatchDft_F##n##_1), NS_(StrideBatchDft_F##n##_2)}, {         \
      NS_(StrideBatchDft_B##n##_1), NS_(StrideBatchDft_B##n##_2)               \
    }                                                                          \
  }
#elif R_SIMD_SIZE_S == 4
#define FACTORY(n)                                                             \
  {                                                                            \
    n,                                                                         \
        {NS_(StrideBatchDft_F##n##_1), NS_(StrideBatchDft_F##n##_2),           \
         NS_(StrideBatchDft_F##n##_4)},                                        \
    {                                                                          \
      NS_(StrideBatchDft_B##n##_1), NS_(StrideBatchDft_B##n##_2),              \
          NS_(StrideBatchDft_B##n##_4)                                         \
    }                                                                          \
  }
#elif R_SIMD_SIZE_S == 8
#define FACTORY(n)                                                             \
  {                                                                            \
    n,                                                                         \
        {NS_(StrideBatchDft_F##n##_1), NS_(StrideBatchDft_F##n##_2),           \
         NS_(StrideBatchDft_F##n##_4), NS_(StrideBatchDft_F##n##_8)},          \
    {                                                                          \
      NS_(StrideBatchDft_B##n##_1), NS_(StrideBatchDft_B##n##_2),              \
          NS_(StrideBatchDft_B##n##_4), NS_(StrideBatchDft_B##n##_8)           \
    }                                                                          \
  }
#endif

static const struct NS_(StrideBatchDftFactory) g_factory_0 = FACTORY(0);
static const struct NS_(StrideBatchDftFactory) g_factory_1 = FACTORY(1);

static struct NS(RealToComplexFunction) *NS_(StrideBatchDftFactory_create_F)(
    const struct NS_(StrideBatchDft_FFunctions) *f,
    struct NS(DataFactory) *data, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  assert(f);
  const int m = (dft_size - 1) / 2;
  const Complex *w = NS(DataFactory_twiddleMatrix)(data, m, m, dft_size, 1, 1);
#if R_SIMD_SIZE_S > 1
  if (batch_size % R_SIMD_SIZE_S) {
    return NS_(StrideBatchDftR_F_new)(f, dft_size, w, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  } else {
#else
  {
#endif
    return NS_(StrideBatchDft_F_new_)(f->evaluate, dft_size, w, batch_size,
                                      x_stride_0, x_stride_1, y_stride_0,
                                      y_stride_1, num_threads);
  }
}

static struct NS(ComplexToRealFunction) *NS_(StrideBatchDftFactory_create_B)(
    const struct NS_(StrideBatchDft_BFunctions) *f,
    struct NS(DataFactory) *data, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  assert(f);
  const int m = (dft_size - 1) / 2;
  const Complex *w = NS(DataFactory_twiddleMatrix)(data, m, m, dft_size, 1, 1);
#if R_SIMD_SIZE_S > 1
  if (batch_size % R_SIMD_SIZE_S) {
    return NS_(StrideBatchDftR_B_new)(f, dft_size, w, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  } else {
#else
  {
#endif
    return NS_(StrideBatchDft_B_new_)(f->evaluate, dft_size, w, batch_size,
                                      x_stride_0, x_stride_1, y_stride_0,
                                      y_stride_1, num_threads);
  }
}

struct NS(RealToComplexFunction) *NS_(StrideBatchDft_F_new)(
    struct NS(DataFactory) *data, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const struct NS_(StrideBatchDftFactory) *p =
      (dft_size % 2) ? &g_factory_1 : &g_factory_0;
  return NS_(StrideBatchDftFactory_create_F)(
      &p->forward, data, dft_size, batch_size, x_stride_0, x_stride_1,
      y_stride_0, y_stride_1, num_threads);
}

struct NS(ComplexToRealFunction) *NS_(StrideBatchDft_B_new)(
    struct NS(DataFactory) *data, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const struct NS_(StrideBatchDftFactory) *p =
      (dft_size % 2) ? &g_factory_1 : &g_factory_0;
  return NS_(StrideBatchDftFactory_create_B)(
      &p->backward, data, dft_size, batch_size, x_stride_0, x_stride_1,
      y_stride_0, y_stride_1, num_threads);
}
