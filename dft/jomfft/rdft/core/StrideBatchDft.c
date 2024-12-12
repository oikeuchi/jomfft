#include "StrideBatchDft.h"

#include "../../model/ThreadingUtil.h"
#include "impl/StrideBatchDft_B10.h"
#include "impl/StrideBatchDft_B12.h"
#include "impl/StrideBatchDft_B128.h"
#include "impl/StrideBatchDft_B16.h"
#include "impl/StrideBatchDft_B2.h"
#include "impl/StrideBatchDft_B3.h"
#include "impl/StrideBatchDft_B32.h"
#include "impl/StrideBatchDft_B4.h"
#include "impl/StrideBatchDft_B5.h"
#include "impl/StrideBatchDft_B6.h"
#include "impl/StrideBatchDft_B64.h"
#include "impl/StrideBatchDft_B7.h"
#include "impl/StrideBatchDft_B8.h"
#include "impl/StrideBatchDft_B9.h"
#include "impl/StrideBatchDft_F10.h"
#include "impl/StrideBatchDft_F12.h"
#include "impl/StrideBatchDft_F128.h"
#include "impl/StrideBatchDft_F16.h"
#include "impl/StrideBatchDft_F2.h"
#include "impl/StrideBatchDft_F3.h"
#include "impl/StrideBatchDft_F32.h"
#include "impl/StrideBatchDft_F4.h"
#include "impl/StrideBatchDft_F5.h"
#include "impl/StrideBatchDft_F6.h"
#include "impl/StrideBatchDft_F64.h"
#include "impl/StrideBatchDft_F7.h"
#include "impl/StrideBatchDft_F8.h"
#include "impl/StrideBatchDft_F9.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(RDftCore_##n)

typedef void (*NS_(StrideBatchDft_F_fn))(Long, Real *, Long, Long, Complex *,
                                         Long, Long);

typedef void (*NS_(StrideBatchDft_B_fn))(Long, Complex *, Long, Long, Real *,
                                         Long, Long);

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
    NS_(StrideBatchDft_F_fn) evaluate, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  assert(evaluate);
  struct NS_(StrideBatchDft_FData) *out = NEW(struct NS_(StrideBatchDft_FData));
  *out = (struct NS_(StrideBatchDft_FData)){.evaluate = evaluate,
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
  data->evaluate(data->batch_size, x, data->x_stride_0, data->x_stride_1, y,
                 data->y_stride_0, data->y_stride_1);
}

#if USE_OPENMP

static void NS_(StrideBatchDft_F_evaluate_omp)(
    struct NS(RealToComplexFunction) *p, Real *x, Complex *y) {
  struct NS_(StrideBatchDft_FData) *data =
      (struct NS_(StrideBatchDft_FData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
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
    data->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                   data->y_stride_0, data->y_stride_1);
  }
}

#endif

static struct NS(RealToComplexFunction) *NS_(StrideBatchDft_F_new_)(
    NS_(StrideBatchDft_F_fn) evaluate, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
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
      NS_(StrideBatchDft_FData_new)(evaluate, batch_size, x_stride_0,
                                    x_stride_1, y_stride_0, y_stride_1,
                                    num_threads));
}

struct NS_(StrideBatchDftR_FData) {
  struct NS_(StrideBatchDft_FFunctions) f;
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
    const struct NS_(StrideBatchDft_FFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  struct NS_(StrideBatchDftR_FData) *out =
      NEW(struct NS_(StrideBatchDftR_FData));
  *out = (struct NS_(StrideBatchDftR_FData)){.f = *f,
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
#if R_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 8
  if (f->evaluate_4) {
    f->evaluate_4(4, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
    x += data->x_stride_1 * 4;
    y += data->y_stride_1 * 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(2, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
  }
#endif
}

static void NS_(StrideBatchDftR_F_evaluate)(struct NS(RealToComplexFunction) *p,
                                            Real *x, Complex *y) {
  struct NS_(StrideBatchDftR_FData) *data =
      (struct NS_(StrideBatchDftR_FData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  const struct NS_(StrideBatchDft_FFunctions) *f = &(data->f);
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, R_SIMD_SIZE_S);
    f->evaluate(batch_size, x, data->x_stride_0, data->x_stride_1, y,
                data->y_stride_0, data->y_stride_1);
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
      f->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                  data->y_stride_0, data->y_stride_1);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Real *x_ = x + data->x_stride_1 * begin;
      Complex *y_ = y + data->y_stride_1 * begin;
      f->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                  data->y_stride_0, data->y_stride_1);
      x_ += data->x_stride_1 * batch_size;
      y_ += data->y_stride_1 * batch_size;
      NS_(StrideBatchDftR_F_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(RealToComplexFunction) *NS_(StrideBatchDftR_F_new)(
    const struct NS_(StrideBatchDft_FFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
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
      NS_(StrideBatchDftR_FData_new)(&f_, batch_size, x_stride_0, x_stride_1,
                                     y_stride_0, y_stride_1, num_threads));
}

struct NS_(StrideBatchDft_BData) {
  NS_(StrideBatchDft_B_fn) evaluate;
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
    NS_(StrideBatchDft_B_fn) evaluate, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  assert(evaluate);
  struct NS_(StrideBatchDft_BData) *out = NEW(struct NS_(StrideBatchDft_BData));
  *out = (struct NS_(StrideBatchDft_BData)){.evaluate = evaluate,
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

static void NS_(StrideBatchDft_B_evaluate)(struct NS(ComplexToRealFunction) *p,
                                           Complex *x, Real *y) {
  struct NS_(StrideBatchDft_BData) *data =
      (struct NS_(StrideBatchDft_BData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  data->evaluate(data->batch_size, x, data->x_stride_0, data->x_stride_1, y,
                 data->y_stride_0, data->y_stride_1);
}

#if USE_OPENMP

static void NS_(StrideBatchDft_B_evaluate_omp)(
    struct NS(ComplexToRealFunction) *p, Complex *x, Real *y) {
  struct NS_(StrideBatchDft_BData) *data =
      (struct NS_(StrideBatchDft_BData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
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
    data->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                   data->y_stride_0, data->y_stride_1);
  }
}

#endif

static struct NS(ComplexToRealFunction) *NS_(StrideBatchDft_B_new_)(
    NS_(StrideBatchDft_B_fn) evaluate, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
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
      NS_(StrideBatchDft_BData_new)(evaluate, batch_size, x_stride_0,
                                    x_stride_1, y_stride_0, y_stride_1,
                                    num_threads));
}

struct NS_(StrideBatchDftR_BData) {
  struct NS_(StrideBatchDft_BFunctions) f;
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
    const struct NS_(StrideBatchDft_BFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  struct NS_(StrideBatchDftR_BData) *out =
      NEW(struct NS_(StrideBatchDftR_BData));
  *out = (struct NS_(StrideBatchDftR_BData)){.f = *f,
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
#if R_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
  }
#elif R_SIMD_SIZE_S == 8
  if (f->evaluate_4) {
    f->evaluate_4(4, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
    x += data->x_stride_1 * 4;
    y += data->y_stride_1 * 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(2, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
    x += data->x_stride_1 * 2;
    y += data->y_stride_1 * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
  }
#endif
}

static void NS_(StrideBatchDftR_B_evaluate)(struct NS(ComplexToRealFunction) *p,
                                            Complex *x, Real *y) {
  struct NS_(StrideBatchDftR_BData) *data =
      (struct NS_(StrideBatchDftR_BData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  const struct NS_(StrideBatchDft_BFunctions) *f = &(data->f);
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, R_SIMD_SIZE_S);
    f->evaluate(batch_size, x, data->x_stride_0, data->x_stride_1, y,
                data->y_stride_0, data->y_stride_1);
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
  const struct NS_(StrideBatchDft_BFunctions) *f = &(data->f);
  assert(f->evaluate);
  const int num_threads = data->num_threads;
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
      f->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                  data->y_stride_0, data->y_stride_1);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + data->x_stride_1 * begin;
      Real *y_ = y + data->y_stride_1 * begin;
      f->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                  data->y_stride_0, data->y_stride_1);
      x_ += data->x_stride_1 * batch_size;
      y_ += data->y_stride_1 * batch_size;
      NS_(StrideBatchDftR_B_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexToRealFunction) *NS_(StrideBatchDftR_B_new)(
    const struct NS_(StrideBatchDft_BFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
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
      NS_(StrideBatchDftR_BData_new)(&f_, batch_size, x_stride_0, x_stride_1,
                                     y_stride_0, y_stride_1, num_threads));
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

static const struct NS_(StrideBatchDftFactory) g_table[] = {
    FACTORY(2),  FACTORY(3),  FACTORY(4),  FACTORY(5),  FACTORY(6),
    FACTORY(7),  FACTORY(8),  FACTORY(9),  FACTORY(10), FACTORY(12),
    FACTORY(16), FACTORY(32), FACTORY(64), FACTORY(128)};

#define g_table_size                                                           \
  (sizeof(g_table) / sizeof(struct NS_(StrideBatchDftFactory)))

static const struct NS_(StrideBatchDftFactory) *NS_(StrideBatchDftFactory_Find)(
    Long table_size, const struct NS_(StrideBatchDftFactory) *table,
    int dft_size) {
  const struct NS_(StrideBatchDftFactory) *it = table;
  const struct NS_(StrideBatchDftFactory) *end = table + table_size;
  const struct NS_(StrideBatchDftFactory) *out = NULL;
  for (; it != end; ++it) {
    if (it->dft_size == dft_size) {
      out = it;
      break;
    }
  }
  return out;
}

static struct NS(RealToComplexFunction) *NS_(StrideBatchDftFactory_create_F)(
    const struct NS_(StrideBatchDft_FFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  assert(f);
#if R_SIMD_SIZE_S > 1
  if (batch_size % R_SIMD_SIZE_S) {
    return NS_(StrideBatchDftR_F_new)(f, batch_size, x_stride_0, x_stride_1,
                                      y_stride_0, y_stride_1, num_threads);
  } else {
#else
  {
#endif
    return NS_(StrideBatchDft_F_new_)(f->evaluate, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  }
}

static struct NS(ComplexToRealFunction) *NS_(StrideBatchDftFactory_create_B)(
    const struct NS_(StrideBatchDft_BFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  assert(f);
#if R_SIMD_SIZE_S > 1
  if (batch_size % R_SIMD_SIZE_S) {
    return NS_(StrideBatchDftR_B_new)(f, batch_size, x_stride_0, x_stride_1,
                                      y_stride_0, y_stride_1, num_threads);
  } else {
#else
  {
#endif
    return NS_(StrideBatchDft_B_new_)(f->evaluate, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  }
}

bool NS_(StrideBatchDft_HasKey)(int dft_size) {
  return NS_(StrideBatchDftFactory_Find)(g_table_size, g_table, dft_size) !=
         NULL;
}

struct NS(RealToComplexFunction) *NS_(StrideBatchDft_F_new)(
    int dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
  const struct NS_(StrideBatchDftFactory) *p =
      NS_(StrideBatchDftFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  return NS_(StrideBatchDftFactory_create_F)(&p->forward, batch_size,
                                             x_stride_0, x_stride_1, y_stride_0,
                                             y_stride_1, num_threads);
}

struct NS(ComplexToRealFunction) *NS_(StrideBatchDft_B_new)(
    int dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
  const struct NS_(StrideBatchDftFactory) *p =
      NS_(StrideBatchDftFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  return NS_(StrideBatchDftFactory_create_B)(&p->backward, batch_size,
                                             x_stride_0, x_stride_1, y_stride_0,
                                             y_stride_1, num_threads);
}
