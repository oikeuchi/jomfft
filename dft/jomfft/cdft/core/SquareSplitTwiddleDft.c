#include "SquareSplitTwiddleDft.h"

#include "../../data/TwiddleMatrix.h"
#include "../../model/ThreadingUtil.h"
#include "impl/SquareSplitTwiddleDft_B2.h"
#include "impl/SquareSplitTwiddleDft_B4.h"
#include "impl/SquareSplitTwiddleDft_B8.h"
#include "impl/SquareSplitTwiddleDft_F2.h"
#include "impl/SquareSplitTwiddleDft_F4.h"
#include "impl/SquareSplitTwiddleDft_F8.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(CDftCore_##n)

typedef void (*NS_(SquareSplitTwiddleDft_fn))(Long, const Complex *,
                                              const Complex *, int, Complex *,
                                              Complex *, Long);

struct NS_(SquareSplitTwiddleDftData) {
  NS_(SquareSplitTwiddleDft_fn) evaluate;
  int dft_size;
  Long twiddle_size;
  const Complex *v;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(SquareSplitTwiddleDftData)
    *NS_(SquareSplitTwiddleDftData_new)(NS_(SquareSplitTwiddleDft_fn) evaluate,
                                        int Long dft_size, Long twiddle_size,
                                        const Complex *v, const Complex *w,
                                        int num_threads) {
  assert(evaluate);
  assert(v);
  assert(w);
  struct NS_(SquareSplitTwiddleDftData) *out =
      NEW(struct NS_(SquareSplitTwiddleDftData));
  *out = (struct NS_(SquareSplitTwiddleDftData)){.evaluate = evaluate,
                                                 .dft_size = dft_size,
                                                 .twiddle_size = twiddle_size,
                                                 .v = v,
                                                 .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(SquareSplitTwiddleDft_evaluate)(struct NS(ComplexFunction) *p,
                                                Complex *x, Complex *y) {
  struct NS_(SquareSplitTwiddleDftData) *data =
      (struct NS_(SquareSplitTwiddleDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const int w_stride = MAX(data->dft_size, C_SIMD_SIZE);
  const Long batch_size = data->dft_size * data->twiddle_size;
  data->evaluate(batch_size, data->v, data->w, w_stride, x, y, batch_size);
}

#if USE_OPENMP

static void NS_(SquareSplitTwiddleDft_evaluate_omp)(
    struct NS(ComplexFunction) *p, Complex *x, Complex *y) {
  struct NS_(SquareSplitTwiddleDftData) *data =
      (struct NS_(SquareSplitTwiddleDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const int dft_size = data->dft_size;
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Complex *v = data->v;
  const Complex *w = data->w;
  const int w_stride = MAX(data->dft_size, C_SIMD_SIZE);
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
    const Complex *v_ = v + begin;
    Complex *x_ = x + begin;
    Complex *y_ = y + begin;
    data->evaluate(batch_size_, v_, w, w_stride, x_, y_, stride);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(SquareSplitTwiddleDft_new_)(
    NS_(SquareSplitTwiddleDft_fn) evaluate, int dft_size, Long twiddle_size,
    const Complex *v, const Complex *w, int num_threads) {
#if USE_OPENMP
  const int block_size = MAX(dft_size, C_SIMD_SIZE);
  num_threads = NS(Threading_GetNumThreads)(dft_size * twiddle_size, block_size,
                                            num_threads);
  NS(ComplexFunction_fn) evaluate_ =
      (num_threads > 1) ? NS_(SquareSplitTwiddleDft_evaluate_omp)
                        : NS_(SquareSplitTwiddleDft_evaluate);
#else
  NS(ComplexFunction_fn) evaluate_ = NS_(SquareSplitTwiddleDft_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate_, jomfftDestructPointer,
      NS_(SquareSplitTwiddleDftData_new)(evaluate, dft_size, twiddle_size, v, w,
                                         num_threads));
}

#if C_SIMD_SIZE >= 4

struct NS_(SquareSplitTwiddleDftR2Functions) {
  NS_(SquareSplitTwiddleDft_fn) evaluate_1;
  NS_(SquareSplitTwiddleDft_fn) evaluate;
};

struct NS_(SquareSplitTwiddleDftR2Data) {
  struct NS_(SquareSplitTwiddleDftR2Functions) f;
  int dft_size;
  Long twiddle_size;
  const Complex *v;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(SquareSplitTwiddleDftR2Data)
    *NS_(SquareSplitTwiddleDftR2Data_new)(
        const struct NS_(SquareSplitTwiddleDftR2Functions) *f, int dft_size,
        Long twiddle_size, const Complex *v, const Complex *w,
        int num_threads) {
  assert(f);
  assert(v);
  assert(w);
  struct NS_(SquareSplitTwiddleDftR2Data) *out =
      NEW(struct NS_(SquareSplitTwiddleDftR2Data));
  *out = (struct NS_(SquareSplitTwiddleDftR2Data)){.f = *f,
                                                   .dft_size = dft_size,
                                                   .twiddle_size = twiddle_size,
                                                   .v = v,
                                                   .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(SquareSplitTwiddleDftR2_EvaluateTail)(
    struct NS_(SquareSplitTwiddleDftR2Data) *data, const Complex *v, Complex *x,
    Complex *y) {
  assert(data);
  const struct NS_(SquareSplitTwiddleDftR2Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long stride = data->dft_size * data->twiddle_size;
  const int w_stride = C_SIMD_SIZE;
  const Complex *w = data->w;
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, v, w, w_stride, x, y, stride);
  }
}

static void NS_(SquareSplitTwiddleDftR2_evaluate)(struct NS(ComplexFunction) *p,
                                                  Complex *x, Complex *y) {
  struct NS_(SquareSplitTwiddleDftR2Data) *data =
      (struct NS_(SquareSplitTwiddleDftR2Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(SquareSplitTwiddleDftR2Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long stride = data->dft_size * data->twiddle_size;
  const Complex *v = data->v;
  const Complex *w = data->w;
  const int w_stride = C_SIMD_SIZE;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(stride, 2 * dft_size);
    f->evaluate(batch_size, v, w, w_stride, x, y, stride);
    v += batch_size;
    x += batch_size;
    y += batch_size;
  }
  NS_(SquareSplitTwiddleDftR2_EvaluateTail)(data, v, x, y);
}

#if USE_OPENMP

static void NS_(SquareSplitTwiddleDftR2_evaluate_omp)(
    struct NS(ComplexFunction) *p, Complex *x, Complex *y) {
  struct NS_(SquareSplitTwiddleDftR2Data) *data =
      (struct NS_(SquareSplitTwiddleDftR2Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(SquareSplitTwiddleDftR2Functions) *f = &(data->f);
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Complex *v = data->v;
  const Complex *w = data->w;
  const int w_stride = C_SIMD_SIZE;
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
      const Complex *v_ = v + begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, v_, w, w_stride, x_, y_, stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      const Complex *v_ = v + begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, v_, w, w_stride, x_, y_, stride);
      v_ += batch_size_;
      x_ += batch_size_;
      y_ += batch_size_;
      NS_(SquareSplitTwiddleDftR2_EvaluateTail)(data, v_, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(SquareSplitTwiddleDftR2_new)(
    const struct NS_(SquareSplitTwiddleDftR2Functions) *f, int dft_size,
    Long twiddle_size, const Complex *v, const Complex *w, int num_threads) {
#if USE_OPENMP
  const int block_size = C_SIMD_SIZE;
  num_threads = NS(Threading_GetNumThreads)(dft_size * twiddle_size, block_size,
                                            num_threads);
  NS(ComplexFunction_fn) evaluate =
      (num_threads > 1) ? NS_(SquareSplitTwiddleDftR2_evaluate_omp)
                        : NS_(SquareSplitTwiddleDftR2_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(SquareSplitTwiddleDftR2_evaluate);
#endif
  struct NS_(SquareSplitTwiddleDftR2Functions) f_ =
      (struct NS_(SquareSplitTwiddleDftR2Functions)){
          .evaluate_1 = (twiddle_size % 2 >= 1) ? f->evaluate_1 : NULL,
          .evaluate = (twiddle_size >= 2) ? f->evaluate : NULL};
  return NS(ComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(SquareSplitTwiddleDftR2Data_new)(&f_, dft_size, twiddle_size, v, w,
                                           num_threads));
}

#endif

#if C_SIMD_SIZE >= 8

struct NS_(SquareSplitTwiddleDftR4Functions) {
  NS_(SquareSplitTwiddleDft_fn) evaluate_1;
  NS_(SquareSplitTwiddleDft_fn) evaluate_2;
  NS_(SquareSplitTwiddleDft_fn) evaluate;
};

struct NS_(SquareSplitTwiddleDftR4Data) {
  struct NS_(SquareSplitTwiddleDftR4Functions) f;
  int dft_size;
  Long twiddle_size;
  const Complex *v;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(SquareSplitTwiddleDftR4Data)
    *NS_(SquareSplitTwiddleDftR4Data_new)(
        const struct NS_(SquareSplitTwiddleDftR4Functions) *f, int dft_size,
        Long twiddle_size, const Complex *v, const Complex *w,
        int num_threads) {
  assert(v);
  assert(w);
  struct NS_(SquareSplitTwiddleDftR4Data) *out =
      NEW(struct NS_(SquareSplitTwiddleDftR4Data));
  *out = (struct NS_(SquareSplitTwiddleDftR4Data)){.f = *f,
                                                   .dft_size = dft_size,
                                                   .twiddle_size = twiddle_size,
                                                   .v = v,
                                                   .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(SquareSplitTwiddleDftR4_EvaluateTail)(
    struct NS_(SquareSplitTwiddleDftR4Data) *data, const Complex *v, Complex *x,
    Complex *y) {
  assert(data);
  const struct NS_(SquareSplitTwiddleDftR4Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long stride = data->dft_size * data->twiddle_size;
  const Complex *w = data->w;
  const int w_stride = C_SIMD_SIZE;
  if (f->evaluate_2) {
    f->evaluate_2(2 * dft_size, v, w, w_stride, x, y, stride);
    v += 2 * dft_size;
    x += 2 * dft_size;
    y += 2 * dft_size;
  }
  if (f->evaluate_1) {
    f->evaluate_1(dft_size, v, w, w_stride, x, y, stride);
  }
}

static void NS_(SquareSplitTwiddleDftR4_evaluate)(struct NS(ComplexFunction) *p,
                                                  Complex *x, Complex *y) {
  struct NS_(SquareSplitTwiddleDftR4Data) *data =
      (struct NS_(SquareSplitTwiddleDftR4Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(SquareSplitTwiddleDftR4Functions) *f = &(data->f);
  const int dft_size = data->dft_size;
  const Long stride = data->dft_size * data->twiddle_size;
  const Complex *v = data->v;
  const Complex *w = data->w;
  const int w_stride = C_SIMD_SIZE;
  if (f->evaluate) {
    Long batch_size = I_FLOOR_MULTIPLE(stride, 4 * dft_size);
    f->evaluate(batch_size, v, w, w_stride, x, y, stride);
    v += batch_size;
    x += batch_size;
    y += batch_size;
  }
  NS_(SquareSplitTwiddleDftR4_EvaluateTail)(data, v, x, y);
}

#if USE_OPENMP

static void NS_(SquareSplitTwiddleDftR4_evaluate_omp)(
    struct NS(ComplexFunction) *p, Complex *x, Complex *y) {
  struct NS_(SquareSplitTwiddleDftR4Data) *data =
      (struct NS_(SquareSplitTwiddleDftR4Data) *)NS(ComplexFunction_data)(p);
  assert(data);
  const struct NS_(SquareSplitTwiddleDftR4Functions) *f = &(data->f);
  const Long batch_size = data->dft_size * data->twiddle_size;
  const Long stride = batch_size;
  const Complex *v = data->v;
  const Complex *w = data->w;
  const int w_stride = C_SIMD_SIZE;
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
      const Complex *v_ = v + begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, v_, w, w_stride, x_, y_, stride);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      const Complex *v_ = v + begin;
      Complex *x_ = x + begin;
      Complex *y_ = y + begin;
      f->evaluate(batch_size_, v_, w, w_stride, x_, y_, stride);
      v_ += batch_size_;
      x_ += batch_size_;
      y_ += batch_size_;
      NS_(SquareSplitTwiddleDftR4_EvaluateTail)(data, v_, x_, y_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(SquareSplitTwiddleDftR4_new)(
    const struct NS_(SquareSplitTwiddleDftR4Functions) *f, int dft_size,
    Long twiddle_size, const Complex *v, const Complex *w, int num_threads) {
#if USE_OPENMP
  const int block_size = C_SIMD_SIZE;
  num_threads = NS(Threading_GetNumThreads)(dft_size * twiddle_size, block_size,
                                            num_threads);
  NS(ComplexFunction_fn) evaluate =
      (num_threads > 1) ? NS_(SquareSplitTwiddleDftR4_evaluate_omp)
                        : NS_(SquareSplitTwiddleDftR4_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(SquareSplitTwiddleDftR4_evaluate);
#endif
  struct NS_(SquareSplitTwiddleDftR4Functions) f_ =
      (struct NS_(SquareSplitTwiddleDftR4Functions)){
          .evaluate_1 = (twiddle_size % 2 >= 1) ? f->evaluate_1 : NULL,
          .evaluate_2 = (twiddle_size % 4 >= 2) ? f->evaluate_2 : NULL,
          .evaluate = (twiddle_size >= 4) ? f->evaluate : NULL};
  return NS(ComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(SquareSplitTwiddleDftR4Data_new)(&f_, dft_size, twiddle_size, v, w,
                                           num_threads));
}

#endif

struct NS_(SquareSplitTwiddleDftFactory) {
  int dft_size;
  NS_(SquareSplitTwiddleDft_fn) forward;
  NS_(SquareSplitTwiddleDft_fn) backward;
};

#define FACTORY(n)                                                             \
  { n, NS_(SquareSplitTwiddleDft_F##n), NS_(SquareSplitTwiddleDft_B##n) }

static const struct NS_(SquareSplitTwiddleDftFactory) g_table[] = {
    FACTORY(2), FACTORY(4), FACTORY(8)};

#define g_table_size                                                           \
  (sizeof(g_table) / sizeof(struct NS_(SquareSplitTwiddleDftFactory)))

static const struct NS_(SquareSplitTwiddleDftFactory)
    *NS_(SquareSplitTwiddleDftFactory_Find)(
        Long table_size, const struct NS_(SquareSplitTwiddleDftFactory) *table,
        int dft_size) {
  const struct NS_(SquareSplitTwiddleDftFactory) *it = table;
  const struct NS_(SquareSplitTwiddleDftFactory) *end = table + table_size;
  const struct NS_(SquareSplitTwiddleDftFactory) *out = NULL;
  for (; it != end; ++it) {
    if (it->dft_size == dft_size) {
      out = it;
      break;
    }
  }
  return out;
}

static struct NS(ComplexFunction) *NS_(SquareSplitTwiddleDftFactory_create)(
    const struct NS_(SquareSplitTwiddleDftFactory) *p,
    struct NS(DataFactory) *data, bool backward, Long twiddle_size,
    int num_threads) {
  assert(p);
  const int dft_size = p->dft_size;
  NS_(SquareSplitTwiddleDft_fn) evaluate = backward ? p->backward : p->forward;
  return NS_(SquareSplitTwiddleDft_new_)(
      evaluate, dft_size, twiddle_size,
      NS(DataFactory_twiddleMatrix)(data, twiddle_size, dft_size,
                                    twiddle_size * dft_size, 0, 0),
      NS(DataFactory_twiddleMatrix)(data, dft_size - 1, dft_size,
                                    dft_size * dft_size * twiddle_size, 1, 0),
      num_threads);
}

#if C_SIMD_SIZE >= 4

struct NS_(SquareSplitTwiddleDftR2Factory) {
  int dft_size;
  struct NS_(SquareSplitTwiddleDftR2Functions) forward;
  struct NS_(SquareSplitTwiddleDftR2Functions) backward;
};

static struct NS(ComplexFunction) *NS_(SquareSplitTwiddleDftR2Factory_create)(
    const struct NS_(SquareSplitTwiddleDftR2Factory) *p,
    struct NS(DataFactory) *data, bool backward, Long twiddle_size,
    int num_threads) {
  const struct NS_(SquareSplitTwiddleDftR2Functions) *f =
      backward ? &p->backward : &p->forward;
  const int dft_size = p->dft_size;
  const Complex *v = NS(DataFactory_twiddleMatrix)(
      data, twiddle_size, dft_size, twiddle_size * dft_size, 0, 0);
  const Complex *w =
      NS(DataFactory_twiddleMatrix)(data, dft_size - 1, 2 * dft_size,
                                    dft_size * dft_size * twiddle_size, 1, 0);
  if (twiddle_size % 2) {
    return NS_(SquareSplitTwiddleDftR2_new)(f, dft_size, twiddle_size, v, w,
                                            num_threads);
  } else {
    return NS_(SquareSplitTwiddleDft_new_)(f->evaluate, dft_size, twiddle_size,
                                           v, w, num_threads);
  }
}

#endif

#if C_SIMD_SIZE >= 8

struct NS_(SquareSplitTwiddleDftR4Factory) {
  int dft_size;
  struct NS_(SquareSplitTwiddleDftR4Functions) forward;
  struct NS_(SquareSplitTwiddleDftR4Functions) backward;
};

static struct NS(ComplexFunction) *NS_(SquareSplitTwiddleDftR4Factory_create)(
    const struct NS_(SquareSplitTwiddleDftR4Factory) *p,
    struct NS(DataFactory) *data, bool backward, Long twiddle_size,
    int num_threads) {
  const struct NS_(SquareSplitTwiddleDftR4Functions) *f =
      backward ? &p->backward : &p->forward;
  const int dft_size = p->dft_size;
  const Complex *v = NS(DataFactory_twiddleMatrix)(
      data, twiddle_size, dft_size, twiddle_size * dft_size, 0, 0);
  const Complex *w =
      NS(DataFactory_twiddleMatrix)(data, dft_size - 1, 4 * dft_size,
                                    dft_size * dft_size * twiddle_size, 1, 0);
  if (twiddle_size % 4) {
    return NS_(SquareSplitTwiddleDftR4_new)(f, dft_size, twiddle_size, v, w,
                                            num_threads);
  } else {
    return NS_(SquareSplitTwiddleDft_new_)(f->evaluate, dft_size, twiddle_size,
                                           v, w, num_threads);
  }
}

#endif

#if C_SIMD_SIZE == 4
static const struct NS_(SquareSplitTwiddleDftR2Factory) g_factory_2 = {
    2,
    {NS_(SquareSplitTwiddleDft_F2), NS_(SquareSplitTwiddleDft_F2_4)},
    {NS_(SquareSplitTwiddleDft_B2), NS_(SquareSplitTwiddleDft_B2_4)}};
#elif C_SIMD_SIZE == 8
static const struct NS_(SquareSplitTwiddleDftR4Factory) g_factory_2 = {
    2,
    {NS_(SquareSplitTwiddleDft_F2), NS_(SquareSplitTwiddleDft_F2_4),
     NS_(SquareSplitTwiddleDft_F2_8)},
    {NS_(SquareSplitTwiddleDft_B2), NS_(SquareSplitTwiddleDft_B2_4),
     NS_(SquareSplitTwiddleDft_B2_8)}};
static const struct NS_(SquareSplitTwiddleDftR2Factory) g_factory_4 = {
    4,
    {NS_(SquareSplitTwiddleDft_F4), NS_(SquareSplitTwiddleDft_F4_8)},
    {NS_(SquareSplitTwiddleDft_B4), NS_(SquareSplitTwiddleDft_B4_8)}};
#endif

bool NS_(SquareSplitTwiddleDft_HasKey)(int dft_size) {
  return NS_(SquareSplitTwiddleDftFactory_Find)(g_table_size, g_table,
                                                dft_size) != NULL;
}

struct NS(ComplexFunction) *NS_(SquareSplitTwiddleDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size,
    Long twiddle_size, int num_threads) {
#if C_SIMD_SIZE == 4
  if (dft_size == 2) {
    return NS_(SquareSplitTwiddleDftR2Factory_create)(
        &g_factory_2, data, backward, twiddle_size, num_threads);
  }
#elif C_SIMD_SIZE == 8
  switch (dft_size) {
  case 2:
    return NS_(SquareSplitTwiddleDftR4Factory_create)(
        &g_factory_2, data, backward, twiddle_size, num_threads);
  case 4:
    return NS_(SquareSplitTwiddleDftR2Factory_create)(
        &g_factory_4, data, backward, twiddle_size, num_threads);
  default:
    break;
  }
#endif
  const struct NS_(SquareSplitTwiddleDftFactory) *p =
      NS_(SquareSplitTwiddleDftFactory_Find)(g_table_size, g_table, dft_size);
  assert(p);
  return NS_(SquareSplitTwiddleDftFactory_create)(p, data, backward,
                                                  twiddle_size, num_threads);
}
