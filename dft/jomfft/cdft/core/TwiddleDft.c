#include "TwiddleDft.h"

#include "../../data/TwiddleMatrix.h"
#include "../../model/ThreadingUtil.h"
#include "impl/TwiddleDft_B10.h"
#include "impl/TwiddleDft_B12.h"
#include "impl/TwiddleDft_B16.h"
#include "impl/TwiddleDft_B2.h"
#include "impl/TwiddleDft_B3.h"
#include "impl/TwiddleDft_B32.h"
#include "impl/TwiddleDft_B4.h"
#include "impl/TwiddleDft_B5.h"
#include "impl/TwiddleDft_B6.h"
#include "impl/TwiddleDft_B64.h"
#include "impl/TwiddleDft_B7.h"
#include "impl/TwiddleDft_B8.h"
#include "impl/TwiddleDft_B9.h"
#include "impl/TwiddleDft_F10.h"
#include "impl/TwiddleDft_F12.h"
#include "impl/TwiddleDft_F16.h"
#include "impl/TwiddleDft_F2.h"
#include "impl/TwiddleDft_F3.h"
#include "impl/TwiddleDft_F32.h"
#include "impl/TwiddleDft_F4.h"
#include "impl/TwiddleDft_F5.h"
#include "impl/TwiddleDft_F6.h"
#include "impl/TwiddleDft_F64.h"
#include "impl/TwiddleDft_F7.h"
#include "impl/TwiddleDft_F8.h"
#include "impl/TwiddleDft_F9.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(CDftCore_##n)

typedef void (*NS_(TwiddleDft_fn))(Long, const Complex *, Complex *, Complex *,
                                   Long);

struct NS_(TwiddleDftFunctions) {
#if C_SIMD_SIZE == 1
  NS_(TwiddleDft_fn) evaluate;
#elif C_SIMD_SIZE == 2
  NS_(TwiddleDft_fn) evaluate_1;
  NS_(TwiddleDft_fn) evaluate;
#endif
#if C_SIMD_SIZE == 4
  NS_(TwiddleDft_fn) evaluate_1;
  NS_(TwiddleDft_fn) evaluate_2;
  NS_(TwiddleDft_fn) evaluate;
#endif
#if C_SIMD_SIZE == 8
  NS_(TwiddleDft_fn) evaluate_1;
  NS_(TwiddleDft_fn) evaluate_2;
  NS_(TwiddleDft_fn) evaluate_4;
  NS_(TwiddleDft_fn) evaluate;
#endif
};

struct NS_(TwiddleDftData) {
  NS_(TwiddleDft_fn) evaluate;
  Long twiddle_size;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(TwiddleDftData) *NS_(TwiddleDftData_new)(
    NS_(TwiddleDft_fn) evaluate, Long twiddle_size, const Complex *w,
    int num_threads) {
  assert(evaluate);
  struct NS_(TwiddleDftData) *out = NEW(struct NS_(TwiddleDftData));
  *out = (struct NS_(TwiddleDftData)){
      .evaluate = evaluate, .twiddle_size = twiddle_size, .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(TwiddleDft_evaluate)(struct NS(ComplexFunction) *p, Complex *x,
                                     Complex *y) {
  struct NS_(TwiddleDftData) *data =
      (struct NS_(TwiddleDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long batch_size = data->twiddle_size;
  const Long stride = batch_size;
  data->evaluate(batch_size, data->w, x, y, stride);
}

#if USE_OPENMP

static void NS_(TwiddleDft_evaluate_omp)(struct NS(ComplexFunction) *p,
                                         Complex *x, Complex *y) {
  struct NS_(TwiddleDftData) *data =
      (struct NS_(TwiddleDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long batch_size = data->twiddle_size;
  const Long stride = batch_size;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(batch_size, C_SIMD_SIZE, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads) schedule(static)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size_ = end - begin;
    const Complex *w_ = w + begin;
    Complex *x_ = x + begin;
    Complex *y_ = y + begin;
    data->evaluate(batch_size_, w_, x_, y_, stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(TwiddleDft_new__)(
    NS_(TwiddleDft_fn) evaluate, Long twiddle_size, const Complex *w,
    int num_threads) {
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(twiddle_size, C_SIMD_SIZE, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(TwiddleDft_evaluate_omp)
                                         : NS_(TwiddleDft_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(TwiddleDft_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(TwiddleDftData_new)(evaluate, twiddle_size, w, num_threads));
}

#if C_SIMD_SIZE >= 2

struct NS_(TwiddleDftRData) {
  struct NS_(TwiddleDftFunctions) f;
  Long twiddle_size;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(TwiddleDftRData) *NS_(TwiddleDftRData_new)(
    const struct NS_(TwiddleDftFunctions) *f, Long twiddle_size,
    const Complex *w, int num_threads) {
  struct NS_(TwiddleDftRData) *out = NEW(struct NS_(TwiddleDftRData));
  *out = (struct NS_(TwiddleDftRData)){
      .f = *f, .twiddle_size = twiddle_size, .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(TwiddleDftR_EvaluateTail)(struct NS_(TwiddleDftRData) *data,
                                          const Complex *w, Complex *x,
                                          Complex *y) {
  assert(data);
  assert(data->w);
  const struct NS_(TwiddleDftFunctions) *f = &(data->f);
  const Long stride = data->twiddle_size;
#if C_SIMD_SIZE == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x, y, stride);
  }
#elif C_SIMD_SIZE == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, w, x, y, stride);
    w += 2;
    x += 2;
    y += 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x, y, stride);
  }
#elif C_SIMD_SIZE == 8
  if (f->evaluate_4) {
    f->evaluate_4(4, w, x, y, stride);
    w += 4;
    x += 4;
    y += 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(2, w, x, y, stride);
    w += 2;
    x += 2;
    y += 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x, y, stride);
  }
#endif
}

static void NS_(TwiddleDftR_evaluate)(struct NS(ComplexFunction) *p, Complex *x,
                                      Complex *y) {
  struct NS_(TwiddleDftRData) *data =
      (struct NS_(TwiddleDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  assert(data->w);
  const struct NS_(TwiddleDftFunctions) *f = &(data->f);
  const Long twiddle_size = data->twiddle_size;
  const Long stride = twiddle_size;
  const Complex *w = data->w;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(twiddle_size, C_SIMD_SIZE);
    f->evaluate(batch_size, w, x, y, stride);
    w += batch_size;
    x += batch_size;
    y += batch_size;
  }
  NS_(TwiddleDftR_EvaluateTail)(data, w, x, y);
}

#if USE_OPENMP

static void NS_(TwiddleDftR_evaluate_omp)(struct NS(ComplexFunction) *p,
                                          Complex *x, Complex *y) {
  struct NS_(TwiddleDftRData) *data =
      (struct NS_(TwiddleDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long batch_size = data->twiddle_size;
  const Long stride = batch_size;
  const Complex *w = data->w;
  const struct NS_(TwiddleDftFunctions) *f = &(data->f);
  assert(f->evaluate);
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(batch_size, C_SIMD_SIZE, num_threads, range);
  int k;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for schedule(static) nowait
    for (k = 0; k < num_threads - 1; ++k) {
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      const Complex *w_ = w + begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, w_, x_, y_, stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      const Complex *w_ = w + begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, w_, x_, y_, stride);
      w_ += batch_size_;
      x_ += batch_size_;
      y_ += batch_size_;
      NS_(TwiddleDftR_EvaluateTail)(data, w_, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(TwiddleDftR_new)(
    const struct NS_(TwiddleDftFunctions) *f, Long twiddle_size,
    const Complex *w, int num_threads) {
  assert(w);
  const Long batch_size = twiddle_size;
  NS(ComplexFunction_fn) evaluate;
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE, num_threads);
  evaluate = (num_threads > 1) ? NS_(TwiddleDftR_evaluate_omp)
                               : NS_(TwiddleDftR_evaluate);
#else
  evaluate = NS_(TwiddleDftR_evaluate);
#endif
  struct NS_(TwiddleDftFunctions) f_ = (struct NS_(TwiddleDftFunctions)) {
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
      NS_(TwiddleDftRData_new)(&f_, twiddle_size, w, num_threads));
}

#endif

struct NS_(TwiddleDftFactory) {
  int dft_size;
  struct NS_(TwiddleDftFunctions) forward;
  struct NS_(TwiddleDftFunctions) backward;
};

#if C_SIMD_SIZE == 1
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(TwiddleDft_F##n##_1)}, { NS_(TwiddleDft_B##n##_1) }                \
  }
#elif C_SIMD_SIZE == 2
#define FACTORY(n)                                                             \
  {                                                                            \
    n, {NS_(TwiddleDft_F##n##_1), NS_(TwiddleDft_F##n##_2)}, {                 \
      NS_(TwiddleDft_B##n##_1), NS_(TwiddleDft_B##n##_2)                       \
    }                                                                          \
  }
#elif C_SIMD_SIZE == 4
#define FACTORY(n)                                                             \
  {                                                                            \
    n,                                                                         \
        {NS_(TwiddleDft_F##n##_1), NS_(TwiddleDft_F##n##_2),                   \
         NS_(TwiddleDft_F##n##_4)},                                            \
    {                                                                          \
      NS_(TwiddleDft_B##n##_1), NS_(TwiddleDft_B##n##_2),                      \
          NS_(TwiddleDft_B##n##_4)                                             \
    }                                                                          \
  }
#elif C_SIMD_SIZE == 8
#define FACTORY(n)                                                             \
  {                                                                            \
    n,                                                                         \
        {NS_(TwiddleDft_F##n##_1), NS_(TwiddleDft_F##n##_2),                   \
         NS_(TwiddleDft_F##n##_4), NS_(TwiddleDft_F##n##_8)},                  \
    {                                                                          \
      NS_(TwiddleDft_B##n##_1), NS_(TwiddleDft_B##n##_2),                      \
          NS_(TwiddleDft_B##n##_4), NS_(TwiddleDft_B##n##_8)                   \
    }                                                                          \
  }
#endif

static const struct NS_(TwiddleDftFactory) g_table[] = {
    FACTORY(2),  FACTORY(3),  FACTORY(4), FACTORY(5),  FACTORY(6),
    FACTORY(7),  FACTORY(8),  FACTORY(9), FACTORY(10), FACTORY(12),
    FACTORY(16), FACTORY(32), FACTORY(64)};

#define g_table_size (sizeof(g_table) / sizeof(struct NS_(TwiddleDftFactory)))

static const struct NS_(TwiddleDftFactory) *NS_(TwiddleDftFactory_Find)(
    Long table_size, const struct NS_(TwiddleDftFactory) *table, int dft_size) {
  const struct NS_(TwiddleDftFactory) *it = table;
  const struct NS_(TwiddleDftFactory) *end = table + table_size;
  const struct NS_(TwiddleDftFactory) *out = NULL;
  for (; it != end; ++it) {
    if (it->dft_size == dft_size) {
      out = it;
      break;
    }
  }
  return out;
}

static struct jomfftIntVector *NS_(TwiddleDftFactory_Keys)(
    Long table_size, const struct NS_(TwiddleDftFactory) *table) {
  const struct NS_(TwiddleDftFactory) *it = table;
  const struct NS_(TwiddleDftFactory) *end = table + table_size;
  struct jomfftIntVector *out = jomfftIntVector_new();
  for (; it != end; ++it) {
    jomfftIntVector_append(out, it->dft_size);
  }
  jomfftIntVector_sort(out);
  return out;
}

static struct NS(ComplexFunction) *NS_(TwiddleDftFactory_create)(
    const struct NS_(TwiddleDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, int dft_size, Long twiddle_size, int num_threads) {
  assert(p);
  const Complex *w = NS(DataFactory_twiddleMatrix)(
      data, dft_size - 1, twiddle_size, dft_size * twiddle_size, 1, 0);
  const struct NS_(TwiddleDftFunctions) *f =
      backward ? &(p->backward) : &(p->forward);
#if C_SIMD_SIZE > 1
  const Long batch_size = twiddle_size;
  if (batch_size % C_SIMD_SIZE) {
    return NS_(TwiddleDftR_new)(f, twiddle_size, w, num_threads);
  } else {
#else
  {
#endif
    return NS_(TwiddleDft_new__)(f->evaluate, twiddle_size, w, num_threads);
  }
}

bool NS_(TwiddleDft_HasKey)(int dft_size) {
  return NS_(TwiddleDftFactory_Find)(g_table_size, g_table, dft_size) != NULL;
}

struct jomfftIntVector *NS_(TwiddleDft_Keys)() {
  return NS_(TwiddleDftFactory_Keys)(g_table_size, g_table);
}

struct NS(ComplexFunction) *NS_(TwiddleDft_new)(struct NS(DataFactory) *data,
                                                bool backward, int dft_size,
                                                Long twiddle_size) {
  return NS_(TwiddleDft_new_)(data, backward, dft_size, twiddle_size, 1);
}

struct NS(ComplexFunction) *NS_(TwiddleDft_new_)(struct NS(DataFactory) *data,
                                                 bool backward, int dft_size,
                                                 Long twiddle_size,
                                                 int num_threads) {
  const struct NS_(TwiddleDftFactory) *p =
      NS_(TwiddleDftFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  return NS_(TwiddleDftFactory_create)(p, data, backward, dft_size,
                                       twiddle_size, num_threads);
}
