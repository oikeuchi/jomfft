#include "InnerBatchDft.h"

#include "../../model/ThreadingUtil.h"
#include "impl/InnerBatchDft_B10.h"
#include "impl/InnerBatchDft_B12.h"
#include "impl/InnerBatchDft_B128.h"
#include "impl/InnerBatchDft_B16.h"
#include "impl/InnerBatchDft_B2.h"
#include "impl/InnerBatchDft_B3.h"
#include "impl/InnerBatchDft_B32.h"
#include "impl/InnerBatchDft_B4.h"
#include "impl/InnerBatchDft_B5.h"
#include "impl/InnerBatchDft_B6.h"
#include "impl/InnerBatchDft_B64.h"
#include "impl/InnerBatchDft_B7.h"
#include "impl/InnerBatchDft_B8.h"
#include "impl/InnerBatchDft_B9.h"
#include "impl/InnerBatchDft_F10.h"
#include "impl/InnerBatchDft_F12.h"
#include "impl/InnerBatchDft_F128.h"
#include "impl/InnerBatchDft_F16.h"
#include "impl/InnerBatchDft_F2.h"
#include "impl/InnerBatchDft_F3.h"
#include "impl/InnerBatchDft_F32.h"
#include "impl/InnerBatchDft_F4.h"
#include "impl/InnerBatchDft_F5.h"
#include "impl/InnerBatchDft_F6.h"
#include "impl/InnerBatchDft_F64.h"
#include "impl/InnerBatchDft_F7.h"
#include "impl/InnerBatchDft_F8.h"
#include "impl/InnerBatchDft_F9.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(CDftCore_##n)

typedef void (*NS_(InnerBatchDft_fn))(Long, Complex *, Long, Complex *, Long);

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
  Long batch_size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(InnerBatchDftData) *NS_(InnerBatchDftData_new)(
    NS_(InnerBatchDft_fn) evaluate, Long batch_size, Long x_stride,
    Long y_stride, int num_threads) {
  assert(evaluate);
  struct NS_(InnerBatchDftData) *out = NEW(struct NS_(InnerBatchDftData));
  *out = (struct NS_(InnerBatchDftData)){.evaluate = evaluate,
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
  data->evaluate(data->batch_size, x, data->x_stride, y, data->y_stride);
}

#if USE_OPENMP

static void NS_(InnerBatchDft_evaluate_omp)(struct NS(ComplexFunction) *p,
                                            Complex *x, Complex *y) {
  struct NS_(InnerBatchDftData) *data =
      (struct NS_(InnerBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (data->batch_size, C_SIMD_SIZE, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads) schedule(static)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Complex *x_ = x + begin;
    Complex *y_ = y + begin;
    data->evaluate(batch_size, x_, data->x_stride, y_, data->y_stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(InnerBatchDft_new_)(
    NS_(InnerBatchDft_fn) evaluate, Long batch_size, Long x_stride,
    Long y_stride, int num_threads) {
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
      NS_(InnerBatchDftData_new)(evaluate, batch_size, x_stride, y_stride,
                                 num_threads));
}

#if C_SIMD_SIZE >= 2

struct NS_(InnerBatchDftRData) {
  struct NS_(InnerBatchDftFunctions) f;
  Long batch_size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(InnerBatchDftRData) *NS_(InnerBatchDftRData_new)(
    const struct NS_(InnerBatchDftFunctions) *f, Long batch_size, Long x_stride,
    Long y_stride, int num_threads) {
  struct NS_(InnerBatchDftRData) *out = NEW(struct NS_(InnerBatchDftRData));
  *out = (struct NS_(InnerBatchDftRData)){.f = *f,
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
#if C_SIMD_SIZE == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride, y, data->y_stride);
  }
#elif C_SIMD_SIZE == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, x, data->x_stride, y, data->y_stride);
    x += 2;
    y += 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride, y, data->y_stride);
  }
#elif C_SIMD_SIZE == 8
  if (f->evaluate_4) {
    f->evaluate_4(4, x, data->x_stride, y, data->y_stride);
    x += 4;
    y += 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(2, x, data->x_stride, y, data->y_stride);
    x += 2;
    y += 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, x, data->x_stride, y, data->y_stride);
  }
#endif
}

static void NS_(InnerBatchDftR_evaluate)(struct NS(ComplexFunction) *p,
                                         Complex *x, Complex *y) {
  struct NS_(InnerBatchDftRData) *data =
      (struct NS_(InnerBatchDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(InnerBatchDftFunctions) *f = &(data->f);
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(data->batch_size, C_SIMD_SIZE);
    f->evaluate(batch_size, x, data->x_stride, y, data->y_stride);
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
  const struct NS_(InnerBatchDftFunctions) *f = &(data->f);
  assert(f->evaluate);
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
      f->evaluate(batch_size, x_, data->x_stride, y_, data->y_stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size, x_, data->x_stride, y_, data->y_stride);
      x_ += batch_size;
      y_ += batch_size;
      NS_(InnerBatchDftR_EvaluateTail)(data, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(InnerBatchDftR_new)(
    const struct NS_(InnerBatchDftFunctions) *f, Long batch_size, Long x_stride,
    Long y_stride, int num_threads) {
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
  return NS(ComplexFunction_new)(evaluate, jomfftDestructPointer,
                                 NS_(InnerBatchDftRData_new)(&f_, batch_size,
                                                             x_stride, y_stride,
                                                             num_threads));
}

#endif

struct NS_(InnerBatchDftFactory) {
  int dft_size;
  struct NS_(InnerBatchDftFunctions) forward;
  struct NS_(InnerBatchDftFunctions) backward;
};

#if C_SIMD_SIZE == 1
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(InnerBatchDft_F##n##_1)}, { NS_(InnerBatchDft_B##n##_1) }          \
  }
#elif C_SIMD_SIZE == 2
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(InnerBatchDft_F##n##_1), NS_(InnerBatchDft_F##n##_2)}, {           \
      NS_(InnerBatchDft_B##n##_1), NS_(InnerBatchDft_B##n##_2)                 \
    }                                                                          \
  }
#elif C_SIMD_SIZE == 4
#define FACTORY(n)                                                             \
  {                                                                            \
    n,                                                                         \
        {NS_(InnerBatchDft_F##n##_1), NS_(InnerBatchDft_F##n##_2),             \
         NS_(InnerBatchDft_F##n##_4)},                                         \
    {                                                                          \
      NS_(InnerBatchDft_B##n##_1), NS_(InnerBatchDft_B##n##_2),                \
          NS_(InnerBatchDft_B##n##_4)                                          \
    }                                                                          \
  }
#elif C_SIMD_SIZE == 8
#define FACTORY(n)                                                             \
  {                                                                            \
    n,                                                                         \
        {NS_(InnerBatchDft_F##n##_1), NS_(InnerBatchDft_F##n##_2),             \
         NS_(InnerBatchDft_F##n##_4), NS_(InnerBatchDft_F##n##_8)},            \
    {                                                                          \
      NS_(InnerBatchDft_B##n##_1), NS_(InnerBatchDft_B##n##_2),                \
          NS_(InnerBatchDft_B##n##_4), NS_(InnerBatchDft_B##n##_8)             \
    }                                                                          \
  }
#endif

static const struct NS_(InnerBatchDftFactory) g_table[] = {
    FACTORY(2),  FACTORY(3),  FACTORY(4),  FACTORY(5),  FACTORY(6),
    FACTORY(7),  FACTORY(8),  FACTORY(9),  FACTORY(10), FACTORY(12),
    FACTORY(16), FACTORY(32), FACTORY(64), FACTORY(128)};

#define g_table_size                                                           \
  (sizeof(g_table) / sizeof(struct NS_(InnerBatchDftFactory)))

static const struct NS_(InnerBatchDftFactory) *NS_(InnerBatchDftFactory_Find)(
    Long table_size, const struct NS_(InnerBatchDftFactory) *table,
    int dft_size) {
  const struct NS_(InnerBatchDftFactory) *it = table;
  const struct NS_(InnerBatchDftFactory) *end = table + table_size;
  const struct NS_(InnerBatchDftFactory) *out = NULL;
  for (; it != end; ++it) {
    if (it->dft_size == dft_size) {
      out = it;
      break;
    }
  }
  return out;
}

static struct NS(ComplexFunction) *NS_(InnerBatchDftFactory_create)(
    const struct NS_(InnerBatchDftFactory) *p, bool backward, Long batch_size,
    Long x_stride, Long y_stride, int num_threads) {
  assert(p);
  const struct NS_(InnerBatchDftFunctions) *f =
      backward ? &(p->backward) : &(p->forward);
#if C_SIMD_SIZE > 1
  if (batch_size % C_SIMD_SIZE) {
    return NS_(InnerBatchDftR_new)(f, batch_size, x_stride, y_stride,
                                   num_threads);
  } else {
#else
  {
#endif
    return NS_(InnerBatchDft_new_)(f->evaluate, batch_size, x_stride, y_stride,
                                   num_threads);
  }
}

bool NS_(InnerBatchDft_HasKey)(int dft_size) {
  return NS_(InnerBatchDftFactory_Find)(g_table_size, g_table, dft_size) !=
         NULL;
}

struct NS(ComplexFunction) *NS_(InnerBatchDft_new)(bool backward, int dft_size,
                                                   Long batch_size,
                                                   Long x_stride, Long y_stride,
                                                   int num_threads) {
  const struct NS_(InnerBatchDftFactory) *p =
      NS_(InnerBatchDftFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  return NS_(InnerBatchDftFactory_create)(p, backward, batch_size, x_stride,
                                          y_stride, num_threads);
}

struct NS_(NestedInnerBatchDftData) {
  NS_(InnerBatchDft_fn) evaluate;
  Long batch_size_0;
  Long batch_size_1;
  Long x_stride_0;
  Long x_stride_1;
  Long y_stride_0;
  Long y_stride_1;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(NestedInnerBatchDftData) *NS_(NestedInnerBatchDftData_new)(
    NS_(InnerBatchDft_fn) evaluate, Long batch_size_0, Long batch_size_1,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  assert(evaluate);
  struct NS_(NestedInnerBatchDftData) *out =
      NEW(struct NS_(NestedInnerBatchDftData));
  *out = (struct NS_(NestedInnerBatchDftData)){.evaluate = evaluate,
                                               .batch_size_0 = batch_size_0,
                                               .batch_size_1 = batch_size_1,
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

static void NS_(NestedInnerBatchDft_evaluate)(struct NS(ComplexFunction) *p,
                                              Complex *x, Complex *y) {
  struct NS_(NestedInnerBatchDftData) *data =
      (struct NS_(NestedInnerBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  for (Long i = 0; i < data->batch_size_0; ++i) {
    data->evaluate(data->batch_size_1, x + data->x_stride_0 * i,
                   data->x_stride_1, y + data->y_stride_0 * i,
                   data->y_stride_1);
  }
}

#if USE_OPENMP

static void NS_(NestedInnerBatchDft_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                  Complex *x, Complex *y) {
  struct NS_(NestedInnerBatchDftData) *data =
      (struct NS_(NestedInnerBatchDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long batch_size_0 = data->batch_size_0;
  const int num_threads = data->num_threads;
  const int num_threads_1 = data->batch_size_1 / C_SIMD_SIZE;
  DECLARE_VLA(Long, range, num_threads_1 + 1);
  if (num_threads_1 > 1) {
    NS(Threading_GetBatchRange)
    (data->batch_size_1, C_SIMD_SIZE, num_threads_1, range);
  } else {
    range[0] = 0;
    range[1] = data->batch_size_1;
  }
  Long i;
  int k;
#pragma omp parallel for collapse(2) num_threads(num_threads)
  for (i = 0; i < batch_size_0; ++i) {
    for (k = 0; k < num_threads_1; ++k) {
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size = end - begin;
      Complex *x_ = x + data->x_stride_0 * i + begin;
      Complex *y_ = y + data->y_stride_0 * i + begin;
      data->evaluate(batch_size, x_, data->x_stride_1, y_, data->y_stride_1);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(NestedInnerBatchDft_new_)(
    NS_(InnerBatchDft_fn) evaluate, Long batch_size_0, Long batch_size_1,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
#if USE_OPENMP
  assert(batch_size_1 % C_SIMD_SIZE == 0);
  num_threads = NS(Threading_GetNumThreads)(
      batch_size_0 * (batch_size_1 / C_SIMD_SIZE), 1, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(NestedInnerBatchDft_evaluate_omp)
                                         : NS_(NestedInnerBatchDft_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(NestedInnerBatchDft_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(NestedInnerBatchDftData_new)(evaluate, batch_size_0, batch_size_1,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1, num_threads));
}

struct NS(ComplexFunction) *NS_(NestedInnerBatchDft_new)(
    bool backward, int dft_size, Long batch_size_0, Long batch_size_1,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const struct NS_(InnerBatchDftFactory) *p =
      NS_(InnerBatchDftFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  assert(batch_size_1 % C_SIMD_SIZE == 0);
  const struct NS_(InnerBatchDftFunctions) *f =
      backward ? &(p->backward) : &(p->forward);
  return NS_(NestedInnerBatchDft_new_)(f->evaluate, batch_size_0, batch_size_1,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1, num_threads);
}
