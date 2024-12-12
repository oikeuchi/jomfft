#include "StrideBatchDft.h"

#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#include "../../data/TwiddleMatrix.h"
#include "../../model/ThreadingUtil.h"
#include "impl/StrideBatchDft_B0.h"
#include "impl/StrideBatchDft_B1.h"
#include "impl/StrideBatchDft_F0.h"
#include "impl/StrideBatchDft_F1.h"

#define NS_(n) NS(CDftSingleton_##n)

typedef void (*NS_(StrideBatchDft_fn))(int, const Complex *, Long, Complex *,
                                       Long, Long, Complex *, Long, Long);

struct NS_(StrideBatchDftFunctions) {
#if C_SIMD_SIZE_S == 1
  NS_(StrideBatchDft_fn) evaluate;
#elif C_SIMD_SIZE_S == 2
  NS_(StrideBatchDft_fn) evaluate_1;
  NS_(StrideBatchDft_fn) evaluate;
#elif C_SIMD_SIZE_S == 4
  NS_(StrideBatchDft_fn) evaluate_1;
  NS_(StrideBatchDft_fn) evaluate_2;
  NS_(StrideBatchDft_fn) evaluate;
#endif
};

struct NS_(StrideBatchDftData) {
  NS_(StrideBatchDft_fn) evaluate;
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

static struct NS_(StrideBatchDftData) *NS_(StrideBatchDftData_new)(
    NS_(StrideBatchDft_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
  assert(evaluate);
  struct NS_(StrideBatchDftData) *out = NEW(struct NS_(StrideBatchDftData));
  *out = (struct NS_(StrideBatchDftData)){.evaluate = evaluate,
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

static void NS_(StrideBatchDft_evaluate)(struct NS(ComplexFunction) *p,
                                         Complex *x, Complex *y) {
  struct NS_(StrideBatchDftData) *data =
      (struct NS_(StrideBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  data->evaluate(data->dft_size, data->w, data->batch_size, x, data->x_stride_0,
                 data->x_stride_1, y, data->y_stride_0, data->y_stride_1);
}

#if USE_OPENMP

static void NS_(StrideBatchDft_evaluate_omp)(struct NS(ComplexFunction) *p,
                                             Complex *x, Complex *y) {
  struct NS_(StrideBatchDftData) *data =
      (struct NS_(StrideBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, C_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads) schedule(static)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Complex *x_ = x + data->x_stride_1 * begin;
    Complex *y_ = y + data->y_stride_1 * begin;
    data->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                   data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(StrideBatchDft_new__)(
    NS_(StrideBatchDft_fn) evaluate, int dft_size, const Complex *w,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(StrideBatchDft_evaluate_omp)
                                         : NS_(StrideBatchDft_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(StrideBatchDft_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(StrideBatchDftData_new)(evaluate, dft_size, w, batch_size, x_stride_0,
                                  x_stride_1, y_stride_0, y_stride_1,
                                  num_threads));
}

#if C_SIMD_SIZE_S >= 2

struct NS_(StrideBatchDftRData) {
  struct NS_(StrideBatchDftFunctions) f;
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

static struct NS_(StrideBatchDftRData) *NS_(StrideBatchDftRData_new)(
    const struct NS_(StrideBatchDftFunctions) *f, int dft_size,
    const Complex *w, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
  struct NS_(StrideBatchDftRData) *out = NEW(struct NS_(StrideBatchDftRData));
  *out = (struct NS_(StrideBatchDftRData)){.f = *f,
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

static void NS_(StrideBatchDftR_EvaluateTail)(
    struct NS_(StrideBatchDftRData) *data, Complex *x, Complex *y) {
  assert(data);
  const struct NS_(StrideBatchDftFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
#if C_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, 1, x, data->x_stride_0, data->x_stride_1, y,
                  data->y_stride_0, data->y_stride_1);
  }
#elif C_SIMD_SIZE_S == 4
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

static void NS_(StrideBatchDftR_evaluate)(struct NS(ComplexFunction) *p,
                                          Complex *x, Complex *y) {
  struct NS_(StrideBatchDftRData) *data =
      (struct NS_(StrideBatchDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(StrideBatchDftFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, C_SIMD_SIZE_S);
    f->evaluate(dft_size, w, batch_size, x, data->x_stride_0, data->x_stride_1,
                y, data->y_stride_0, data->y_stride_1);
    x += data->x_stride_1 * batch_size;
    y += data->y_stride_1 * batch_size;
  }
  NS_(StrideBatchDftR_EvaluateTail)(data, x, y);
}

#if USE_OPENMP

static void NS_(StrideBatchDftR_evaluate_omp)(struct NS(ComplexFunction) *p,
                                              Complex *x, Complex *y) {
  struct NS_(StrideBatchDftRData) *data =
      (struct NS_(StrideBatchDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(StrideBatchDftFunctions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, C_SIMD_SIZE_S, num_threads, range);
  int k;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for schedule(static) nowait
    for (k = 0; k < num_threads - 1; ++k) {
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + data->x_stride_1 * begin;
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
      Complex *x_ = x + data->x_stride_1 * begin;
      Complex *y_ = y + data->y_stride_1 * begin;
      f->evaluate(dft_size, w, batch_size, x_, data->x_stride_0,
                  data->x_stride_1, y_, data->y_stride_0, data->y_stride_1);
      x_ += data->x_stride_1 * batch_size;
      y_ += data->y_stride_1 * batch_size;
      NS_(StrideBatchDftR_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(StrideBatchDftR_new)(
    const struct NS_(StrideBatchDftFunctions) *f, int dft_size,
    const Complex *w, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate = (num_threads > 1)
                                        ? NS_(StrideBatchDftR_evaluate_omp)
                                        : NS_(StrideBatchDftR_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(StrideBatchDftR_evaluate);
#endif
  struct NS_(StrideBatchDftFunctions) f_ =
      (struct NS_(StrideBatchDftFunctions)) {
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
      NS_(StrideBatchDftRData_new)(&f_, dft_size, w, batch_size, x_stride_0,
                                   x_stride_1, y_stride_0, y_stride_1,
                                   num_threads));
}

#endif

struct NS_(StrideBatchDftFactory) {
  struct NS_(StrideBatchDftFunctions) forward;
  struct NS_(StrideBatchDftFunctions) backward;
};

#if C_SIMD_SIZE_S == 1
#define FACTORY(n)                                                             \
  {                                                                            \
    {NS_(StrideBatchDft_F##n##_1)}, { NS_(StrideBatchDft_B##n##_1) }           \
  }
#elif C_SIMD_SIZE_S == 2
#define FACTORY(n)                                                             \
  {                                                                            \
    {NS_(StrideBatchDft_F##n##_1), NS_(StrideBatchDft_F##n##_2)}, {            \
      NS_(StrideBatchDft_B##n##_1), NS_(StrideBatchDft_B##n##_2)               \
    }                                                                          \
  }
#elif C_SIMD_SIZE_S == 4
#define FACTORY(n)                                                             \
  {                                                                            \
    {NS_(StrideBatchDft_F##n##_1), NS_(StrideBatchDft_F##n##_2),               \
     NS_(StrideBatchDft_F##n##_4)},                                            \
    {                                                                          \
      NS_(StrideBatchDft_B##n##_1), NS_(StrideBatchDft_B##n##_2),              \
          NS_(StrideBatchDft_B##n##_4)                                         \
    }                                                                          \
  }
#endif

static const struct NS_(StrideBatchDftFactory) g_factory_0 = FACTORY(0);

static const struct NS_(StrideBatchDftFactory) g_factory_1 = FACTORY(1);

static struct NS(ComplexFunction) *NS_(StrideBatchDftFactory_create)(
    const struct NS_(StrideBatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, int dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  assert(p);
  const int m = (dft_size - 1) / 2;
  const Complex *w = NS(DataFactory_twiddleMatrix)(data, m, m, dft_size, 1, 1);
  const struct NS_(StrideBatchDftFunctions) *f =
      backward ? &(p->backward) : &(p->forward);
#if C_SIMD_SIZE_S > 1
  if (batch_size % C_SIMD_SIZE_S) {
    return NS_(StrideBatchDftR_new)(f, dft_size, w, batch_size, x_stride_0,
                                    x_stride_1, y_stride_0, y_stride_1,
                                    num_threads);
  } else {
#else
  {
#endif
    return NS_(StrideBatchDft_new__)(f->evaluate, dft_size, w, batch_size,
                                     x_stride_0, x_stride_1, y_stride_0,
                                     y_stride_1, num_threads);
  }
}

struct NS(ComplexFunction) *NS_(StrideBatchDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1) {
  return NS_(StrideBatchDft_new_)(data, backward, dft_size, batch_size,
                                  x_stride_0, x_stride_1, y_stride_0,
                                  y_stride_1, 1);
}

struct NS(ComplexFunction) *NS_(StrideBatchDft_new_)(
    struct NS(DataFactory) *data, bool backward, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const struct NS_(StrideBatchDftFactory) *p =
      (dft_size % 2) ? &g_factory_1 : &g_factory_0;
  return NS_(StrideBatchDftFactory_create)(p, data, backward, dft_size,
                                           batch_size, x_stride_0, x_stride_1,
                                           y_stride_0, y_stride_1, num_threads);
}
