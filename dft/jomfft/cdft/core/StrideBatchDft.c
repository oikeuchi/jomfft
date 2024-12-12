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

#define NS_(n) NS(CDftCore_##n)

typedef void (*NS_(StrideBatchDft_fn))(Long, Complex *, Long, Long, Complex *,
                                       Long, Long);

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
    NS_(StrideBatchDft_fn) evaluate, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  assert(evaluate);
  struct NS_(StrideBatchDftData) *out = NEW(struct NS_(StrideBatchDftData));
  *out = (struct NS_(StrideBatchDftData)){.evaluate = evaluate,
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
  data->evaluate(data->batch_size, x, data->x_stride_0, data->x_stride_1, y,
                 data->y_stride_0, data->y_stride_1);
}

#if USE_OPENMP

static void NS_(StrideBatchDft_evaluate_omp)(struct NS(ComplexFunction) *p,
                                             Complex *x, Complex *y) {
  struct NS_(StrideBatchDftData) *data =
      (struct NS_(StrideBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
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
    data->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                   data->y_stride_0, data->y_stride_1);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(StrideBatchDft_new__)(
    NS_(StrideBatchDft_fn) evaluate, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
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
      NS_(StrideBatchDftData_new)(evaluate, batch_size, x_stride_0, x_stride_1,
                                  y_stride_0, y_stride_1, num_threads));
}

#if C_SIMD_SIZE_S >= 2

struct NS_(StrideBatchDftRData) {
  struct NS_(StrideBatchDftFunctions) f;
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
    const struct NS_(StrideBatchDftFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  struct NS_(StrideBatchDftRData) *out = NEW(struct NS_(StrideBatchDftRData));
  *out = (struct NS_(StrideBatchDftRData)){.f = *f,
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
#if C_SIMD_SIZE_S == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride_0, data->x_stride_1, y, data->y_stride_0,
                  data->y_stride_1);
  }
#elif C_SIMD_SIZE_S == 4
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

static void NS_(StrideBatchDftR_evaluate)(struct NS(ComplexFunction) *p,
                                          Complex *x, Complex *y) {
  struct NS_(StrideBatchDftRData) *data =
      (struct NS_(StrideBatchDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(StrideBatchDftFunctions) *f = &(data->f);
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, C_SIMD_SIZE_S);
    f->evaluate(batch_size, x, data->x_stride_0, data->x_stride_1, y,
                data->y_stride_0, data->y_stride_1);
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
  const int num_threads = data->num_threads;
  const struct NS_(StrideBatchDftFunctions) *f = &(data->f);
  assert(f->evaluate);
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
      Complex *y_ = y + data->y_stride_1 * begin;
      f->evaluate(batch_size, x_, data->x_stride_0, data->x_stride_1, y_,
                  data->y_stride_0, data->y_stride_1);
      x_ += data->x_stride_1 * batch_size;
      y_ += data->y_stride_1 * batch_size;
      NS_(StrideBatchDftR_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(StrideBatchDftR_new)(
    const struct NS_(StrideBatchDftFunctions) *f, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
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
      NS_(StrideBatchDftRData_new)(&f_, batch_size, x_stride_0, x_stride_1,
                                   y_stride_0, y_stride_1, num_threads));
}

#endif

struct NS_(StrideBatchDftFactory) {
  int dft_size;
  struct NS_(StrideBatchDftFunctions) forward;
  struct NS_(StrideBatchDftFunctions) backward;
};

#if C_SIMD_SIZE_S == 1
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(StrideBatchDft_F##n##_1)}, { NS_(StrideBatchDft_B##n##_1) }        \
  }
#elif C_SIMD_SIZE_S == 2
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(StrideBatchDft_F##n##_1), NS_(StrideBatchDft_F##n##_2)}, {         \
      NS_(StrideBatchDft_B##n##_1), NS_(StrideBatchDft_B##n##_2)               \
    }                                                                          \
  }
#elif C_SIMD_SIZE_S == 4
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

static struct jomfftIntVector *NS_(StrideBatchDftFactory_Keys)(
    Long table_size, const struct NS_(StrideBatchDftFactory) *table) {
  const struct NS_(StrideBatchDftFactory) *it = table;
  const struct NS_(StrideBatchDftFactory) *end = table + table_size;
  struct jomfftIntVector *out = jomfftIntVector_new();
  for (; it != end; ++it) {
    jomfftIntVector_append(out, it->dft_size);
  }
  jomfftIntVector_sort(out);
  return out;
}

static struct NS(ComplexFunction) *NS_(StrideBatchDftFactory_create)(
    const struct NS_(StrideBatchDftFactory) *p, bool backward, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  assert(p);
  const struct NS_(StrideBatchDftFunctions) *f =
      backward ? &(p->backward) : &(p->forward);
#if C_SIMD_SIZE_S > 1
  if (batch_size % C_SIMD_SIZE_S) {
    return NS_(StrideBatchDftR_new)(f, batch_size, x_stride_0, x_stride_1,
                                    y_stride_0, y_stride_1, num_threads);
  } else {
#else
  {
#endif
    return NS_(StrideBatchDft_new__)(f->evaluate, batch_size, x_stride_0,
                                     x_stride_1, y_stride_0, y_stride_1,
                                     num_threads);
  }
}

bool NS_(StrideBatchDft_HasKey)(int dft_size) {
  return NS_(StrideBatchDftFactory_Find)(g_table_size, g_table, dft_size) !=
         NULL;
}

struct jomfftIntVector *NS_(StrideBatchDft_Keys)() {
  return NS_(StrideBatchDftFactory_Keys)(g_table_size, g_table);
}

struct NS(ComplexFunction) *NS_(StrideBatchDft_new)(
    bool backward, int dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1) {
  return NS_(StrideBatchDft_new_)(backward, dft_size, batch_size, x_stride_0,
                                  x_stride_1, y_stride_0, y_stride_1, 1);
}

struct NS(ComplexFunction) *NS_(StrideBatchDft_new_)(
    bool backward, int dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  const struct NS_(StrideBatchDftFactory) *p =
      NS_(StrideBatchDftFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  return NS_(StrideBatchDftFactory_create)(p, backward, batch_size, x_stride_0,
                                           x_stride_1, y_stride_0, y_stride_1,
                                           num_threads);
}
