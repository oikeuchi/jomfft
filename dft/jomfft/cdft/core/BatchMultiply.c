#include "BatchMultiply.h"

#include "../../model/ThreadingUtil.h"
#include "impl/BatchMultiply_B.h"
#include "impl/BatchMultiply_F.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(CDftCore_##n)

typedef void (*NS_(BatchMultiply_fn))(Long, const Complex *, Complex *, Long,
                                      Complex *, Long);

struct NS_(BatchMultiplyFunctions) {
#if C_SIMD_SIZE_S == 1
  NS_(BatchMultiply_fn) evaluate;
#elif C_SIMD_SIZE_S == 2
  NS_(BatchMultiply_fn) evaluate_1;
  NS_(BatchMultiply_fn) evaluate;
#elif C_SIMD_SIZE_S >= 4
  NS_(BatchMultiply_fn) evaluate_1;
  NS_(BatchMultiply_fn) evaluate_2;
  NS_(BatchMultiply_fn) evaluate;
#endif
};

struct NS_(BatchMultiplyData) {
  NS_(BatchMultiply_fn) evaluate;
  Long batch_size;
  const Complex *w;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(BatchMultiplyData) *NS_(BatchMultiplyData_new)(
    NS_(BatchMultiply_fn) evaluate, Long batch_size, const Complex *w,
    Long x_stride, Long y_stride, int num_threads) {
  assert(evaluate);
  struct NS_(BatchMultiplyData) *out = NEW(struct NS_(BatchMultiplyData));
  *out = (struct NS_(BatchMultiplyData)){.evaluate = evaluate,
                                         .batch_size = batch_size,
                                         .w = w,
                                         .x_stride = x_stride,
                                         .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(BatchMultiply_evaluate)(struct NS(ComplexFunction) *p,
                                        Complex *x, Complex *y) {
  struct NS_(BatchMultiplyData) *data =
      (struct NS_(BatchMultiplyData) *)NS(ComplexFunction_data)(p);
  assert(data);
  data->evaluate(data->batch_size, data->w, x, data->x_stride, y,
                 data->y_stride);
}

#if USE_OPENMP

static void NS_(BatchMultiply_evaluate_omp)(struct NS(ComplexFunction) *p,
                                            Complex *x, Complex *y) {
  struct NS_(BatchMultiplyData) *data =
      (struct NS_(BatchMultiplyData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Complex *w = data->w;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
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
    const Complex *w_ = w + begin;
    Complex *x_ = x + x_stride * begin;
    Complex *y_ = y + y_stride * begin;
    data->evaluate(batch_size, w_, x_, x_stride, y_, y_stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(BatchMultiply_new_)(
    NS_(BatchMultiply_fn) evaluate, Long batch_size, const Complex *w,
    Long x_stride, Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(BatchMultiply_evaluate_omp)
                                         : NS_(BatchMultiply_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(BatchMultiply_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(BatchMultiplyData_new)(evaluate, batch_size, w, x_stride, y_stride,
                                 num_threads));
}

#if C_SIMD_SIZE_S >= 2

struct NS_(BatchMultiplyRData) {
  struct NS_(BatchMultiplyFunctions) f;
  Long batch_size;
  const Complex *w;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(BatchMultiplyRData) *NS_(BatchMultiplyRData_new)(
    const struct NS_(BatchMultiplyFunctions) *f, Long batch_size,
    const Complex *w, Long x_stride, Long y_stride, int num_threads) {
  struct NS_(BatchMultiplyRData) *out = NEW(struct NS_(BatchMultiplyRData));
  *out = (struct NS_(BatchMultiplyRData)){.f = *f,
                                          .batch_size = batch_size,
                                          .w = w,
                                          .x_stride = x_stride,
                                          .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(BatchMultiplyR_EvaluateTail)(
    struct NS_(BatchMultiplyRData) *data, const Complex *w, Complex *x,
    Complex *y) {
  assert(data);
  const struct NS_(BatchMultiplyFunctions) *f = &(data->f);
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
#if C_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x, x_stride, y, y_stride);
  }
#elif C_SIMD_SIZE_S == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, w, x, x_stride, y, y_stride);
    w += 2;
    x += x_stride * 2;
    y += y_stride * 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x, x_stride, y, y_stride);
  }
#endif
}

static void NS_(BatchMultiplyR_evaluate)(struct NS(ComplexFunction) *p,
                                         Complex *x, Complex *y) {
  struct NS_(BatchMultiplyRData) *data =
      (struct NS_(BatchMultiplyRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(BatchMultiplyFunctions) *f = &(data->f);
  const Complex *w = data->w;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, C_SIMD_SIZE_S);
    f->evaluate(batch_size, w, x, x_stride, y, y_stride);
    w += batch_size;
    x += x_stride * batch_size;
    y += y_stride * batch_size;
  }
  NS_(BatchMultiplyR_EvaluateTail)(data, w, x, y);
}

#if USE_OPENMP

static void NS_(BatchMultiplyR_evaluate_omp)(struct NS(ComplexFunction) *p,
                                             Complex *x, Complex *y) {
  struct NS_(BatchMultiplyRData) *data =
      (struct NS_(BatchMultiplyRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(BatchMultiplyFunctions) *f = &(data->f);
  const Complex *w = data->w;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
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
      const Complex *w_ = w + begin;
      Complex *x_ = x + x_stride * begin;
      Complex *y_ = y + y_stride * begin;
      f->evaluate(batch_size, w_, x_, x_stride, y_, y_stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      const Complex *w_ = w + begin;
      Complex *x_ = x + x_stride * begin;
      Complex *y_ = y + y_stride * begin;
      f->evaluate(batch_size, w_, x_, x_stride, y_, y_stride);
      w_ += batch_size;
      x_ += x_stride * batch_size;
      y_ += y_stride * batch_size;
      NS_(BatchMultiplyR_EvaluateTail)(data, w_, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(BatchMultiplyR_new)(
    const struct NS_(BatchMultiplyFunctions) *f, Long batch_size,
    const Complex *w, Long x_stride, Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE_S, num_threads);
  NS(ComplexFunction_fn) evaluate = (num_threads > 1)
                                        ? NS_(BatchMultiplyR_evaluate_omp)
                                        : NS_(BatchMultiplyR_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(BatchMultiplyR_evaluate);
#endif
  struct NS_(BatchMultiplyFunctions) f_ = (struct NS_(BatchMultiplyFunctions)) {
#if C_SIMD_SIZE_S == 2
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate = (batch_size >= 2) ? f->evaluate : NULL
#elif C_SIMD_SIZE_S == 4
    .evaluate_1 = (batch_size % 2 >= 1) ? f->evaluate_1 : NULL,
    .evaluate_2 = (batch_size % 4 >= 2) ? f->evaluate_2 : NULL,
    .evaluate = (batch_size >= 4) ? f->evaluate : NULL
#endif
  };
  return NS(ComplexFunction_new)(evaluate, jomfftDestructPointer,
                                 NS_(BatchMultiplyRData_new)(&f_, batch_size, w,
                                                             x_stride, y_stride,
                                                             num_threads));
}

#endif

struct NS_(BatchMultiplyFactory) {
  struct NS_(BatchMultiplyFunctions) forward;
  struct NS_(BatchMultiplyFunctions) backward;
};

#if C_SIMD_SIZE_S == 1
static const struct NS_(BatchMultiplyFactory) g_factory = {
    {NS_(BatchMultiply_F_1)}, {NS_(BatchMultiply_B_1)}};
#elif C_SIMD_SIZE_S == 2
static const struct NS_(BatchMultiplyFactory) g_factory = {
    {NS_(BatchMultiply_F_1), NS_(BatchMultiply_F_2)},
    {NS_(BatchMultiply_B_1), NS_(BatchMultiply_B_2)}};
#elif C_SIMD_SIZE_S == 4
static const struct NS_(BatchMultiplyFactory) g_factory = {
    {NS_(BatchMultiply_F_1), NS_(BatchMultiply_F_2), NS_(BatchMultiply_F_4)},
    {NS_(BatchMultiply_B_1), NS_(BatchMultiply_B_2), NS_(BatchMultiply_B_4)}};
#endif

static struct NS(ComplexFunction) *NS_(BatchMultiplyFactory_create)(
    const struct NS_(BatchMultiplyFactory) *p, bool backward, Long batch_size,
    const Complex *w, Long x_stride, Long y_stride, int num_threads) {
  assert(p);
  const struct NS_(BatchMultiplyFunctions) *f =
      backward ? &(p->backward) : &(p->forward);
#if C_SIMD_SIZE_S > 1
  if (batch_size % C_SIMD_SIZE_S) {
    return NS_(BatchMultiplyR_new)(f, batch_size, w, x_stride, y_stride,
                                   num_threads);
  } else {
#else
  {
#endif
    return NS_(BatchMultiply_new_)(f->evaluate, batch_size, w, x_stride,
                                   y_stride, num_threads);
  }
}

struct NS(ComplexFunction) *NS_(BatchMultiply_new)(bool backward,
                                                   Long batch_size,
                                                   const Complex *w,
                                                   Long x_stride, Long y_stride,
                                                   int num_threads) {
  return NS_(BatchMultiplyFactory_create)(&g_factory, backward, batch_size, w,
                                          x_stride, y_stride, num_threads);
}
