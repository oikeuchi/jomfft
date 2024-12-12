#include "InnerBatchDft.h"

#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#include "../../data/TwiddleMatrix.h"
#include "../../model/ThreadingUtil.h"
#include "impl/InnerBatchDft_B0.h"
#include "impl/InnerBatchDft_B1.h"
#include "impl/InnerBatchDft_F0.h"
#include "impl/InnerBatchDft_F1.h"

#define NS_(n) NS(CDftSingleton_##n)

typedef void (*NS_(InnerBatchDft_fn))(int, const Complex *, Long, Complex *,
                                      Long, Complex *, Long);

struct NS_(InnerBatchDftFunctions) {
#if C_SIMD_SIZE == 1
  NS_(InnerBatchDft_fn) evaluate;
#elif C_SIMD_SIZE == 2
  NS_(InnerBatchDft_fn) evaluate_1;
  NS_(InnerBatchDft_fn) evaluate;
#elif C_SIMD_SIZE == 4
  NS_(InnerBatchDft_fn) evaluate_1;
  NS_(InnerBatchDft_fn) evaluate_2;
  NS_(InnerBatchDft_fn) evaluate;
#elif C_SIMD_SIZE == 8
  NS_(InnerBatchDft_fn) evaluate_1;
  NS_(InnerBatchDft_fn) evaluate_2;
  NS_(InnerBatchDft_fn) evaluate_4;
  NS_(InnerBatchDft_fn) evaluate;
#endif
};

struct NS_(InnerBatchDftData) {
  NS_(InnerBatchDft_fn) evaluate;
  int dft_size;
  const Complex *w;
  Long batch_size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(InnerBatchDftData) *NS_(InnerBatchDftData_new)(
    NS_(InnerBatchDft_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride, Long y_stride, int num_threads) {
  assert(evaluate);
  struct NS_(InnerBatchDftData) *out = NEW(struct NS_(InnerBatchDftData));
  *out = (struct NS_(InnerBatchDftData)){.evaluate = evaluate,
                                         .dft_size = dft_size,
                                         .w = w,
                                         .batch_size = batch_size,
                                         .x_stride = x_stride,
                                         .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(InnerBatchDft_evaluate)(struct NS(ComplexFunction) *p,
                                        Complex *x, Complex *y) {
  struct NS_(InnerBatchDftData) *data =
      (struct NS_(InnerBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  data->evaluate(data->dft_size, data->w, data->batch_size, x, data->x_stride,
                 y, data->y_stride);
}

#if USE_OPENMP

static void NS_(InnerBatchDft_evaluate_omp)(struct NS(ComplexFunction) *p,
                                            Complex *x, Complex *y) {
  struct NS_(InnerBatchDftData) *data =
      (struct NS_(InnerBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(data->batch_size, C_SIMD_SIZE, num_threads,
                              range);
  int k;
#pragma omp parallel for num_threads(num_threads) schedule(static)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Complex *x_ = x + begin;
    Complex *y_ = y + begin;
    data->evaluate(dft_size, w, batch_size, x_, data->x_stride, y_,
                   data->y_stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(InnerBatchDft_new_)(
    NS_(InnerBatchDft_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride, Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(InnerBatchDft_evaluate_omp)
                                         : NS_(InnerBatchDft_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(InnerBatchDft_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(InnerBatchDftData_new)(evaluate, dft_size, w, batch_size, x_stride,
                                 y_stride, num_threads));
}

#if C_SIMD_SIZE >= 2

struct NS_(InnerBatchDftRData) {
  struct NS_(InnerBatchDftFunctions) f;
  int dft_size;
  const Complex *w;
  Long batch_size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(InnerBatchDftRData) *NS_(InnerBatchDftRData_new)(
    const struct NS_(InnerBatchDftFunctions) *f, int dft_size, const Complex *w,
    Long batch_size, Long x_stride, Long y_stride, int num_threads) {
  struct NS_(InnerBatchDftRData) *out = NEW(struct NS_(InnerBatchDftRData));
  *out = (struct NS_(InnerBatchDftRData)){.f = *f,
                                          .dft_size = dft_size,
                                          .w = w,
                                          .batch_size = batch_size,
                                          .x_stride = x_stride,
                                          .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(InnerBatchDftR_EvaluateTail)(
    struct NS_(InnerBatchDftRData) *data, Complex *x, Complex *y) {
  assert(data);
  const struct NS_(InnerBatchDftFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
#if C_SIMD_SIZE == 2
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride, y, data->y_stride);
  }
#elif C_SIMD_SIZE == 4
  if (f->evaluate_2) {
    f->evaluate_2(dft_size, w, 2, x, data->x_stride, y, data->y_stride);
    x += 2;
    y += 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride, y, data->y_stride);
  }
#elif C_SIMD_SIZE == 8
  if (f->evaluate_4) {
    f->evaluate_4(dft_size, w, 4, x, data->x_stride, y, data->y_stride);
    x += 4;
    y += 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(dft_size, w, 2, x, data->x_stride, y, data->y_stride);
    x += 2;
    y += 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride, y, data->y_stride);
  }
#endif
}
static void NS_(InnerBatchDftR_evaluate)(struct NS(ComplexFunction) *p,
                                         Complex *x, Complex *y) {
  struct NS_(InnerBatchDftRData) *data =
      (struct NS_(InnerBatchDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(InnerBatchDftFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, C_SIMD_SIZE);
    f->evaluate(dft_size, w, batch_size, x, data->x_stride, y, data->y_stride);
    x += batch_size;
    y += batch_size;
  }
  NS_(InnerBatchDftR_EvaluateTail)(data, x, y);
}

#if USE_OPENMP

static void NS_(InnerBatchDftR_evaluate_omp)(struct NS(ComplexFunction) *p,
                                             Complex *x, Complex *y) {
  struct NS_(InnerBatchDftRData) *data =
      (struct NS_(InnerBatchDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  assert(data->batch_size > C_SIMD_SIZE);
  const struct NS_(InnerBatchDftFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, C_SIMD_SIZE, num_threads, range);
  int k;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for schedule(static) nowait
    for (k = 0; k < num_threads - 1; ++k) {
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(dft_size, w, batch_size, x_, data->x_stride, y_,
                  data->y_stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(dft_size, w, batch_size, x_, data->x_stride, y_,
                  data->y_stride);
      x_ += batch_size;
      y_ += batch_size;
      NS_(InnerBatchDftR_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(InnerBatchDftR_new)(
    const struct NS_(InnerBatchDftFunctions) *f, int dft_size, const Complex *w,
    Long batch_size, Long x_stride, Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE, num_threads);
  NS(ComplexFunction_fn) evaluate = (num_threads > 1)
                                        ? NS_(InnerBatchDftR_evaluate_omp)
                                        : NS_(InnerBatchDftR_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(InnerBatchDftR_evaluate);
#endif
  struct NS_(InnerBatchDftFunctions) f_ = (struct NS_(InnerBatchDftFunctions)) {
#if C_SIMD_SIZE == 2
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate = (batch_size >= 2) ? f->evaluate : NULL
#elif C_SIMD_SIZE == 4
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate = (batch_size >= 4) ? f->evaluate : NULL
#elif C_SIMD_SIZE == 8
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate_4 = (batch_size % 8 >= 4) ? f->evaluate_4 : NULL,
    .evaluate = (batch_size >= 8) ? f->evaluate : NULL
#endif
  };
  return NS(ComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(InnerBatchDftRData_new)(&f_, dft_size, w, batch_size, x_stride,
                                  y_stride, num_threads));
}

#endif

struct NS_(InnerBatchDftFactory) {
  struct NS_(InnerBatchDftFunctions) forward;
  struct NS_(InnerBatchDftFunctions) backward;
};

#if C_SIMD_SIZE == 1
#define FACTORY(n)                                                             \
  {                                                                            \
    {NS_(InnerBatchDft_F##n##_1)}, { NS_(InnerBatchDft_B##n##_1) }             \
  }
#elif C_SIMD_SIZE == 2
#define FACTORY(n)                                                             \
  {                                                                            \
    {NS_(InnerBatchDft_F##n##_1), NS_(InnerBatchDft_F##n##_2)}, {              \
      NS_(InnerBatchDft_B##n##_1), NS_(InnerBatchDft_B##n##_2)                 \
    }                                                                          \
  }
#elif C_SIMD_SIZE == 4
#define FACTORY(n)                                                             \
  {                                                                            \
    {NS_(InnerBatchDft_F##n##_1), NS_(InnerBatchDft_F##n##_2),                 \
     NS_(InnerBatchDft_F##n##_4)},                                             \
    {                                                                          \
      NS_(InnerBatchDft_B##n##_1), NS_(InnerBatchDft_B##n##_2),                \
          NS_(InnerBatchDft_B##n##_4)                                          \
    }                                                                          \
  }
#elif C_SIMD_SIZE == 8
#define FACTORY(n)                                                             \
  {                                                                            \
    {NS_(InnerBatchDft_F##n##_1), NS_(InnerBatchDft_F##n##_2),                 \
     NS_(InnerBatchDft_F##n##_4), NS_(InnerBatchDft_F##n##_8)},                \
    {                                                                          \
      NS_(InnerBatchDft_B##n##_1), NS_(InnerBatchDft_B##n##_2),                \
          NS_(InnerBatchDft_B##n##_4), NS_(InnerBatchDft_B##n##_8)             \
    }                                                                          \
  }
#endif

static const struct NS_(InnerBatchDftFactory) g_factory_0 = FACTORY(0);

static const struct NS_(InnerBatchDftFactory) g_factory_1 = FACTORY(1);

static struct NS(ComplexFunction) *NS_(InnerBatchDftFactory_create)(
    const struct NS_(InnerBatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, int dft_size, Long batch_size, Long x_stride, Long y_stride,
    int num_threads) {
  assert(p);
  const int m = (dft_size - 1) / 2;
  const Complex *w = NS(DataFactory_twiddleMatrix)(data, m, m, dft_size, 1, 1);
  const struct NS_(InnerBatchDftFunctions) *f =
      backward ? &(p->backward) : &(p->forward);
#if C_SIMD_SIZE > 1
  if (batch_size % C_SIMD_SIZE) {
    return NS_(InnerBatchDftR_new)(f, dft_size, w, batch_size, x_stride,
                                   y_stride, num_threads);
  } else {
#else
  {
#endif
    return NS_(InnerBatchDft_new_)(f->evaluate, dft_size, w, batch_size,
                                   x_stride, y_stride, num_threads);
  }
}

struct NS(ComplexFunction) *NS_(InnerBatchDft_new)(struct NS(DataFactory) *data,
                                                   bool backward, int dft_size,
                                                   Long batch_size,
                                                   Long x_stride, Long y_stride,
                                                   int num_threads) {
  const struct NS_(InnerBatchDftFactory) *p =
      (dft_size % 2) ? &g_factory_1 : &g_factory_0;
  return NS_(InnerBatchDftFactory_create)(
      p, data, backward, dft_size, batch_size, x_stride, y_stride, num_threads);
}
