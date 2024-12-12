#include "TransposeDftTwiddle.h"

#include "../../data/TwiddleMatrix.h"
#include "../../model/ThreadingUtil.h"
#include "impl/TransposeDftTwiddle_B2.h"
#include "impl/TransposeDftTwiddle_B4.h"
#include "impl/TransposeDftTwiddle_B8.h"
#include "impl/TransposeDftTwiddle_F2.h"
#include "impl/TransposeDftTwiddle_F4.h"
#include "impl/TransposeDftTwiddle_F8.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(CDftCore_##n)

typedef void (*NS_(TransposeDftTwiddle_fn))(Long, const Complex *, Complex *,
                                            Long, Long, Complex *, Long);

struct NS_(TransposeDftTwiddleData) {
  NS_(TransposeDftTwiddle_fn) evaluate;
  int dft_size;
  Long twiddle_size;
  const Complex *w;
  Long x_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(TransposeDftTwiddleData) *NS_(TransposeDftTwiddleData_new)(
    NS_(TransposeDftTwiddle_fn) evaluate, int dft_size, Long twiddle_size,
    const Complex *w, Long x_stride, int num_threads) {
  assert(evaluate);
  assert(w);
  struct NS_(TransposeDftTwiddleData) *out =
      NEW(struct NS_(TransposeDftTwiddleData));
  *out = (struct NS_(TransposeDftTwiddleData)){.evaluate = evaluate,
                                               .dft_size = dft_size,
                                               .twiddle_size = twiddle_size,
                                               .w = w,
                                               .x_stride = x_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(TransposeDftTwiddle_evaluate)(struct NS(ComplexFunction) *p,
                                              Complex *x, Complex *y) {
  struct NS_(TransposeDftTwiddleData) *data =
      (struct NS_(TransposeDftTwiddleData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  const Complex *w = data->w;
  data->evaluate(batch_size, w, x, x_stride_0, x_stride_1, y, stride);
}

#if USE_OPENMP

static void NS_(TransposeDftTwiddle_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                  Complex *x, Complex *y) {
  struct NS_(TransposeDftTwiddleData) *data =
      (struct NS_(TransposeDftTwiddleData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  const Complex *w = data->w;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (batch_size, MAX(dft_size, C_SIMD_SIZE), num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads) schedule(static)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size_ = end - begin;
    const Complex *w_ = w + begin;
    Complex *x_ = x + x_stride_1 * begin;
    Complex *y_ = y + begin;
    data->evaluate(batch_size_, w_, x_, x_stride_0, x_stride_1, y_, stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(TransposeDftTwiddle_new_)(
    NS_(TransposeDftTwiddle_fn) evaluate, int dft_size, Long twiddle_size,
    const Complex *w, Long x_stride, int num_threads) {
#if USE_OPENMP
  const Long batch_size = dft_size * twiddle_size;
  const int block_size = MAX(dft_size, C_SIMD_SIZE);
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, block_size, num_threads);
  NS(ComplexFunction_fn) evaluate_ = (num_threads > 1)
                                         ? NS_(TransposeDftTwiddle_evaluate_omp)
                                         : NS_(TransposeDftTwiddle_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(TransposeDftTwiddle_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(TransposeDftTwiddleData_new)(evaluate, dft_size, twiddle_size, w,
                                       x_stride, num_threads));
}

#if C_SIMD_SIZE >= 4

struct NS_(TransposeDftTwiddleR2Functions) {
  NS_(TransposeDftTwiddle_fn) evaluate_1;
  NS_(TransposeDftTwiddle_fn) evaluate;
};

struct NS_(TransposeDftTwiddleR2Data) {
  struct NS_(TransposeDftTwiddleR2Functions) f;
  int dft_size;
  Long twiddle_size;
  const Complex *w;
  Long x_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(TransposeDftTwiddleR2Data)
    *NS_(TransposeDftTwiddleR2Data_new)(
        const struct NS_(TransposeDftTwiddleR2Functions) *f, int dft_size,
        Long twiddle_size, const Complex *w, Long x_stride, int num_threads) {
  assert(w);
  struct NS_(TransposeDftTwiddleR2Data) *out =
      NEW(struct NS_(TransposeDftTwiddleR2Data));
  *out = (struct NS_(TransposeDftTwiddleR2Data)){.f = *f,
                                                 .dft_size = dft_size,
                                                 .twiddle_size = twiddle_size,
                                                 .w = w,
                                                 .x_stride = x_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(TransposeDftTwiddleR2_EvaluateTail)(
    struct NS_(TransposeDftTwiddleR2Data) *data, const Complex *w, Complex *x,
    Complex *y) {
  assert(data);
  const struct NS_(TransposeDftTwiddleR2Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long stride = data->dft_size * data->twiddle_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, x, x_stride_0, x_stride_1, y, stride);
  }
}

static void NS_(TransposeDftTwiddleR2_evaluate)(struct NS(ComplexFunction) *p,
                                                Complex *x, Complex *y) {
  struct NS_(TransposeDftTwiddleR2Data) *data =
      (struct NS_(TransposeDftTwiddleR2Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(TransposeDftTwiddleR2Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  const Complex *w = data->w;
  if (f->evaluate) {
    Long batch_size_ = I_FLOOR_MULTIPLE(stride, 2 * dft_size);
    f->evaluate(batch_size_, w, x, x_stride_0, x_stride_1, y, stride);
    w += batch_size_;
    x += x_stride_1 * batch_size_;
    y += batch_size_;
  }
  NS_(TransposeDftTwiddleR2_EvaluateTail)(data, w, x, y);
}

#if USE_OPENMP

static void NS_(TransposeDftTwiddleR2_evaluate_omp)(
    struct NS(ComplexFunction) *p, Complex *x, Complex *y) {
  struct NS_(TransposeDftTwiddleR2Data) *data =
      (struct NS_(TransposeDftTwiddleR2Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(TransposeDftTwiddleR2Functions) *f = &(data->f);
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  const Complex *w = data->w;
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
      Complex *x_ = x + x_stride_1 * begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, w_, x_, x_stride_0, x_stride_1, y_, stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      const Complex *w_ = w + begin;
      Complex *x_ = x + x_stride_1 * begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, w_, x_, x_stride_0, x_stride_1, y_, stride);
      w_ += batch_size_;
      x_ += x_stride_1 * batch_size_;
      y_ += batch_size_;
      NS_(TransposeDftTwiddleR2_EvaluateTail)(data, w_, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(TransposeDftTwiddleR2_new)(
    const struct NS_(TransposeDftTwiddleR2Functions) *f, int dft_size,
    Long twiddle_size, const Complex *w, Long x_stride, int num_threads) {
#if USE_OPENMP
  const int block_size = C_SIMD_SIZE;
  num_threads = NS(Threading_GetNumThreads)(dft_size * twiddle_size, block_size,
                                            num_threads);
  NS(ComplexFunction_fn) evaluate =
      (num_threads > 1 && twiddle_size > 2)
          ? NS_(TransposeDftTwiddleR2_evaluate_omp)
          : NS_(TransposeDftTwiddleR2_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(TransposeDftTwiddleR2_evaluate);
#endif
  struct NS_(TransposeDftTwiddleR2Functions) f_ =
      (struct NS_(TransposeDftTwiddleR2Functions)){
          .evaluate_1 = (twiddle_size % 2 >= 1) ? f->evaluate_1 : NULL,
          .evaluate = (twiddle_size >= 2) ? f->evaluate : NULL};
  return NS(ComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(TransposeDftTwiddleR2Data_new)(&f_, dft_size, twiddle_size, w,
                                         x_stride, num_threads));
}

#endif

#if C_SIMD_SIZE >= 8

struct NS_(TransposeDftTwiddleR4Functions) {
  NS_(TransposeDftTwiddle_fn) evaluate_1;
  NS_(TransposeDftTwiddle_fn) evaluate_2;
  NS_(TransposeDftTwiddle_fn) evaluate;
};

struct NS_(TransposeDftTwiddleR4Data) {
  struct NS_(TransposeDftTwiddleR4Functions) f;
  int dft_size;
  Long twiddle_size;
  const Complex *w;
  Long x_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(TransposeDftTwiddleR4Data)
    *NS_(TransposeDftTwiddleR4Data_new)(
        const struct NS_(TransposeDftTwiddleR4Functions) *f, int dft_size,
        Long twiddle_size, const Complex *w, Long x_stride, int num_threads) {
  assert(w);
  struct NS_(TransposeDftTwiddleR4Data) *out =
      NEW(struct NS_(TransposeDftTwiddleR4Data));
  *out = (struct NS_(TransposeDftTwiddleR4Data)){.f = *f,
                                                 .dft_size = dft_size,
                                                 .twiddle_size = twiddle_size,
                                                 .w = w,
                                                 .x_stride = x_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(TransposeDftTwiddleR4_EvaluateTail)(
    struct NS_(TransposeDftTwiddleR4Data) *data, const Complex *w, Complex *x,
    Complex *y) {
  assert(data);
  const struct NS_(TransposeDftTwiddleR4Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long stride = data->dft_size * data->twiddle_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  if (f->evaluate_2) {
    f->evaluate_2(2 * dft_size, w, x, x_stride_0, x_stride_1, y, stride);
    w += 2 * dft_size;
    x += x_stride_1 * 2 * dft_size;
    y += 2 * dft_size;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, w, x, x_stride_0, x_stride_1, y, stride);
  }
}

static void NS_(TransposeDftTwiddleR4_evaluate)(struct NS(ComplexFunction) *p,
                                                Complex *x, Complex *y) {
  struct NS_(TransposeDftTwiddleR4Data) *data =
      (struct NS_(TransposeDftTwiddleR4Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(TransposeDftTwiddleR4Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long stride = data->dft_size * data->twiddle_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  const Complex *w = data->w;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(stride, 4 * dft_size);
    f->evaluate(batch_size, w, x, x_stride_0, x_stride_1, y, stride);
    w += batch_size;
    x += x_stride_1 * batch_size;
    y += batch_size;
  }
  NS_(TransposeDftTwiddleR4_EvaluateTail)(data, w, x, y);
}

#if USE_OPENMP

static void NS_(TransposeDftTwiddleR4_evaluate_omp)(
    struct NS(ComplexFunction) *p, Complex *x, Complex *y) {
  struct NS_(TransposeDftTwiddleR4Data) *data =
      (struct NS_(TransposeDftTwiddleR4Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(TransposeDftTwiddleR4Functions) *f = &(data->f);
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Long x_stride_1 = data->x_stride;
  const Long x_stride_0 = stride * x_stride_1;
  const Complex *w = data->w;
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
      Complex *x_ = x + x_stride_1 * begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, w_, x_, x_stride_0, x_stride_1, y_, stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      const Complex *w_ = w + begin;
      Complex *x_ = x + x_stride_1 * begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, w_, x_, x_stride_0, x_stride_1, y_, stride);
      w_ += batch_size_;
      x_ += x_stride_1 * batch_size_;
      y_ += batch_size_;
      NS_(TransposeDftTwiddleR4_EvaluateTail)(data, w_, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(TransposeDftTwiddleR4_new)(
    const struct NS_(TransposeDftTwiddleR4Functions) *f, int dft_size,
    Long twiddle_size, const Complex *w, Long x_stride, int num_threads) {
#if USE_OPENMP
  const int block_size = C_SIMD_SIZE;
  num_threads = NS(Threading_GetNumThreads)(dft_size * twiddle_size, block_size,
                                            num_threads);
  NS(ComplexFunction_fn) evaluate =
      (num_threads > 1 && twiddle_size > 4)
          ? NS_(TransposeDftTwiddleR4_evaluate_omp)
          : NS_(TransposeDftTwiddleR4_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(TransposeDftTwiddleR4_evaluate);
#endif
  struct NS_(TransposeDftTwiddleR4Functions) f_ =
      (struct NS_(TransposeDftTwiddleR4Functions)){
          .evaluate_1 = (twiddle_size % 2 >= 1) ? f->evaluate_1 : NULL,
          .evaluate_2 = (twiddle_size % 4 >= 2) ? f->evaluate_2 : NULL,
          .evaluate = (twiddle_size >= 4) ? f->evaluate : NULL};
  return NS(ComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(TransposeDftTwiddleR4Data_new)(&f_, dft_size, twiddle_size, w,
                                         x_stride, num_threads));
}

#endif

struct NS_(TransposeDftTwiddleFactory) {
  int dft_size;
  NS_(TransposeDftTwiddle_fn) forward;
  NS_(TransposeDftTwiddle_fn) backward;
};

#define FACTORY(n)                                                             \
  { n, NS_(TransposeDftTwiddle_F##n), NS_(TransposeDftTwiddle_B##n) }

static const struct NS_(TransposeDftTwiddleFactory) g_table[] = {
    FACTORY(2), FACTORY(4), FACTORY(8)};

#define g_table_size                                                           \
  (sizeof(g_table) / sizeof(struct NS_(TransposeDftTwiddleFactory)))

static const struct NS_(TransposeDftTwiddleFactory)
    *NS_(TransposeDftTwiddleFactory_Find)(
        Long table_size, const struct NS_(TransposeDftTwiddleFactory) *table,
        int dft_size) {
  const struct NS_(TransposeDftTwiddleFactory) *it = table;
  const struct NS_(TransposeDftTwiddleFactory) *end = table + table_size;
  const struct NS_(TransposeDftTwiddleFactory) *out = NULL;
  for (; it != end; ++it) {
    if (it->dft_size == dft_size) {
      out = it;
      break;
    }
  }
  return out;
}

static struct NS(ComplexFunction) *NS_(TransposeDftTwiddleFactory_create)(
    const struct NS_(TransposeDftTwiddleFactory) *p,
    struct NS(DataFactory) *data, bool backward, Long twiddle_size,
    Long x_stride, int num_threads) {
  assert(p);
  const int dft_size = p->dft_size;
  NS_(TransposeDftTwiddle_fn) evaluate = backward ? p->backward : p->forward;
  return NS_(TransposeDftTwiddle_new_)(
      evaluate, dft_size, twiddle_size,
      NS(DataFactory_twiddleMatrix)(data, twiddle_size, dft_size,
                                    twiddle_size * dft_size, 0, 0),
      x_stride, num_threads);
}

#if C_SIMD_SIZE >= 4

struct NS_(TransposeDftTwiddleR2Factory) {
  int dft_size;
  struct NS_(TransposeDftTwiddleR2Functions) forward;
  struct NS_(TransposeDftTwiddleR2Functions) backward;
};

static struct NS(ComplexFunction) *NS_(TransposeDftTwiddleR2Factory_create)(
    const struct NS_(TransposeDftTwiddleR2Factory) *p,
    struct NS(DataFactory) *data, bool backward, Long twiddle_size,
    Long x_stride, int num_threads) {
  const int dft_size = p->dft_size;
  const struct NS_(TransposeDftTwiddleR2Functions) *f =
      backward ? &p->backward : &p->forward;
  const Complex *w = NS(DataFactory_twiddleMatrix)(
      data, twiddle_size, dft_size, twiddle_size * dft_size, 0, 0);
  if (twiddle_size % 2) {
    return NS_(TransposeDftTwiddleR2_new)(f, dft_size, twiddle_size, w,
                                          x_stride, num_threads);
  } else {
    return NS_(TransposeDftTwiddle_new_)(f->evaluate, dft_size, twiddle_size, w,
                                         x_stride, num_threads);
  }
}

#endif

#if C_SIMD_SIZE >= 8

struct NS_(TransposeDftTwiddleR4Factory) {
  int dft_size;
  struct NS_(TransposeDftTwiddleR4Functions) forward;
  struct NS_(TransposeDftTwiddleR4Functions) backward;
};

static struct NS(ComplexFunction) *NS_(TransposeDftTwiddleR4Factory_create)(
    const struct NS_(TransposeDftTwiddleR4Factory) *p,
    struct NS(DataFactory) *data, bool backward, Long twiddle_size,
    Long x_stride, int num_threads) {
  const int dft_size = p->dft_size;
  const struct NS_(TransposeDftTwiddleR4Functions) *f =
      backward ? &p->backward : &p->forward;
  const Complex *w = NS(DataFactory_twiddleMatrix)(
      data, twiddle_size, dft_size, twiddle_size * dft_size, 0, 0);
  if (twiddle_size % 4) {
    return NS_(TransposeDftTwiddleR4_new)(f, dft_size, twiddle_size, w,
                                          x_stride, num_threads);
  } else {
    return NS_(TransposeDftTwiddle_new_)(f->evaluate, dft_size, twiddle_size, w,
                                         x_stride, num_threads);
  }
}

#endif

#if C_SIMD_SIZE == 4
static const struct NS_(TransposeDftTwiddleR2Factory) g_factory_2 = {
    2,
    {NS_(TransposeDftTwiddle_F2), NS_(TransposeDftTwiddle_F2_4)},
    {NS_(TransposeDftTwiddle_B2), NS_(TransposeDftTwiddle_B2_4)}};
#elif C_SIMD_SIZE == 8
static const struct NS_(TransposeDftTwiddleR4Factory) g_factory_2 = {
    2,
    {NS_(TransposeDftTwiddle_F2), NS_(TransposeDftTwiddle_F2_4),
     NS_(TransposeDftTwiddle_F2_8)},
    {NS_(TransposeDftTwiddle_B2), NS_(TransposeDftTwiddle_B2_4),
     NS_(TransposeDftTwiddle_B2_8)}};
static const struct NS_(TransposeDftTwiddleR2Factory) g_factory_4 = {
    4,
    {NS_(TransposeDftTwiddle_F4), NS_(TransposeDftTwiddle_F4_8)},
    {NS_(TransposeDftTwiddle_B4), NS_(TransposeDftTwiddle_B4_8)}};
#endif

struct NS(ComplexFunction) *NS_(TransposeDftTwiddle_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size,
    Long twiddle_size, Long x_stride, int num_threads) {
#if C_SIMD_SIZE == 4
  if (dft_size == 2) {
    return NS_(TransposeDftTwiddleR2Factory_create)(
        &g_factory_2, data, backward, twiddle_size, x_stride, num_threads);
  }
#elif C_SIMD_SIZE == 8
  switch (dft_size) {
  case 2:
    return NS_(TransposeDftTwiddleR4Factory_create)(
        &g_factory_2, data, backward, twiddle_size, x_stride, num_threads);
  case 4:
    return NS_(TransposeDftTwiddleR2Factory_create)(
        &g_factory_4, data, backward, twiddle_size, x_stride, num_threads);
  default:
    break;
  }
#endif
  const struct NS_(TransposeDftTwiddleFactory) *p =
      NS_(TransposeDftTwiddleFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  return NS_(TransposeDftTwiddleFactory_create)(p, data, backward, twiddle_size,
                                                x_stride, num_threads);
}
