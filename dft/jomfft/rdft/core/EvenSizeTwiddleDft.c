#include "EvenSizeTwiddleDft.h"

#include "../../data/TwiddleVector.h"
#include "../../math/Math.h"
#include "../../model/ThreadingUtil.h"
#include "impl/EvenSizeTwiddleDft_B.h"
#include "impl/EvenSizeTwiddleDft_F.h"
#include <jomfft/VLA.h>
#include <jomfft/math/IntegerMath.h>

#define NS_(n) NS(RDftCore_##n)

typedef void (*NS_(EvenSizeTwiddleDft_fn))(Long, const Complex *, Complex *,
                                           Complex *, Complex *, Complex *);

typedef void (*NS_(EvenSizeTwiddleDftHead_fn))(Long, Complex *, Complex *);

struct NS_(EvenSizeTwiddleDftFunctions) {
#if C_SIMD_SIZE == 1
  NS_(EvenSizeTwiddleDft_fn) evaluate;
#elif C_SIMD_SIZE == 2
  NS_(EvenSizeTwiddleDft_fn) evaluate_1;
  NS_(EvenSizeTwiddleDft_fn) evaluate;
#elif C_SIMD_SIZE == 4
  NS_(EvenSizeTwiddleDft_fn) evaluate_1;
  NS_(EvenSizeTwiddleDft_fn) evaluate_2;
  NS_(EvenSizeTwiddleDft_fn) evaluate;
#elif C_SIMD_SIZE == 8
  NS_(EvenSizeTwiddleDft_fn) evaluate_1;
  NS_(EvenSizeTwiddleDft_fn) evaluate_2;
  NS_(EvenSizeTwiddleDft_fn) evaluate_4;
  NS_(EvenSizeTwiddleDft_fn) evaluate;
#endif
};

struct NS_(EvenSizeTwiddleDftData) {
  NS_(EvenSizeTwiddleDftHead_fn) evaluate_head;
  NS_(EvenSizeTwiddleDft_fn) evaluate;
  Long dft_size;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(EvenSizeTwiddleDftData) *NS_(EvenSizeTwiddleDftData_new)(
    NS_(EvenSizeTwiddleDftHead_fn) evaluate_head,
    NS_(EvenSizeTwiddleDft_fn) evaluate, Long dft_size, const Complex *w,
    int num_threads) {
  assert(evaluate);
  struct NS_(EvenSizeTwiddleDftData) *out =
      NEW(struct NS_(EvenSizeTwiddleDftData));
  *out = (struct NS_(EvenSizeTwiddleDftData)){.evaluate_head = evaluate_head,
                                              .evaluate = evaluate,
                                              .dft_size = dft_size,
                                              .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(EvenSizeTwiddleDft_F_EvaluateHead)(Long dft_size, Complex *x,
                                                   Complex *y) {
  const Real x0 = C_REAL(x[0]);
  const Real x1 = C_IMAG(x[0]);
  y[0] = Complex_R(x0 + x1);
  y[dft_size] = Complex_R(x0 - x1);
}

static void NS_(EvenSizeTwiddleDft_B_EvaluateHead)(Long dft_size, Complex *x,
                                                   Complex *y) {
  const Real x0 = C_REAL(x[0]);
  const Real x1 = C_REAL(x[dft_size]);
  y[0] = Complex_((x0 + x1), (x0 - x1));
}

static void NS_(EvenSizeTwiddleDft_evaluate)(struct NS(ComplexFunction) *p,
                                             Complex *x, Complex *y) {
  struct NS_(EvenSizeTwiddleDftData) *data =
      (struct NS_(EvenSizeTwiddleDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  data->evaluate_head(data->dft_size, x, y);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  data->evaluate(batch_size, data->w, x + 1, x + dft_size, y + 1, y + dft_size);
}

#if USE_OPENMP

static void NS_(EvenSizeTwiddleDft_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                 Complex *x, Complex *y) {
  struct NS_(EvenSizeTwiddleDftData) *data =
      (struct NS_(EvenSizeTwiddleDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long dft_size = data->dft_size;
  const Long batch_size = dft_size / 2;
  data->evaluate_head(dft_size, x, y);
  const Complex *w = data->w;
  Complex *x0 = x + 1;
  Complex *x1 = x + dft_size;
  Complex *y0 = y + 1;
  Complex *y1 = y + dft_size;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)
  (batch_size, C_SIMD_SIZE, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size_ = end - begin;
    const Complex *w_ = w + begin;
    Complex *x0_ = x0 + begin;
    Complex *x1_ = x1 - begin;
    Complex *y0_ = y0 + begin;
    Complex *y1_ = y1 - begin;
    data->evaluate(batch_size_, w_, x0_, x1_, y0_, y1_);
  }
}

#endif

static struct NS(ComplexFunction) *NS_(EvenSizeTwiddleDft_new)(
    NS_(EvenSizeTwiddleDftHead_fn) evaluate_head,
    NS_(EvenSizeTwiddleDft_fn) evaluate_body, Long dft_size, const Complex *w,
    int num_threads) {
#if USE_OPENMP
  const Long batch_size = dft_size / 2;
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE, num_threads);
  NS(ComplexFunction_fn) evaluate = (num_threads > 1)
                                        ? NS_(EvenSizeTwiddleDft_evaluate_omp)
                                        : NS_(EvenSizeTwiddleDft_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(EvenSizeTwiddleDft_evaluate);
#endif
  return NS(ComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      NS_(EvenSizeTwiddleDftData_new)(evaluate_head, evaluate_body, dft_size, w,
                                      num_threads));
}

#if C_SIMD_SIZE >= 2

struct NS_(EvenSizeTwiddleDftRData) {
  NS_(EvenSizeTwiddleDftHead_fn) evaluate_head;
  struct NS_(EvenSizeTwiddleDftFunctions) f;
  Long dft_size;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(EvenSizeTwiddleDftRData) *NS_(EvenSizeTwiddleDftRData_new)(
    NS_(EvenSizeTwiddleDftHead_fn) evaluate_head,
    const struct NS_(EvenSizeTwiddleDftFunctions) *f, Long dft_size,
    const Complex *w, int num_threads) {
  struct NS_(EvenSizeTwiddleDftRData) *out =
      NEW(struct NS_(EvenSizeTwiddleDftRData));
  *out = (struct NS_(EvenSizeTwiddleDftRData)){
      .evaluate_head = evaluate_head, .f = *f, .dft_size = dft_size, .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(EvenSizeTwiddleDftR_EvaluateTail)(
    struct NS_(EvenSizeTwiddleDftRData) *data, const Complex *w, Complex *x0,
    Complex *x1, Complex *y0, Complex *y1) {
  assert(data);
  const struct NS_(EvenSizeTwiddleDftFunctions) *f = &(data->f);
#if C_SIMD_SIZE == 2
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x0, x1, y0, y1);
  }
#elif C_SIMD_SIZE == 4
  if (f->evaluate_2) {
    f->evaluate_2(2, w, x0, x1, y0, y1);
    w += 2;
    x0 += 2;
    x1 -= 2;
    y0 += 2;
    y1 -= 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x0, x1, y0, y1);
  }
#elif C_SIMD_SIZE == 8
  if (f->evaluate_4) {
    f->evaluate_4(4, w, x0, x1, y0, y1);
    w += 4;
    x0 += 4;
    x1 -= 4;
    y0 += 4;
    y1 -= 4;
  }
  if (f->evaluate_2) {
    f->evaluate_2(2, w, x0, x1, y0, y1);
    w += 2;
    x0 += 2;
    x1 -= 2;
    y0 += 2;
    y1 -= 2;
  }
  if (f->evaluate_1) {
    f->evaluate_1(1, w, x0, x1, y0, y1);
  }
#endif
}

static void NS_(EvenSizeTwiddleDftR_evaluate)(struct NS(ComplexFunction) *p,
                                              Complex *x, Complex *y) {
  struct NS_(EvenSizeTwiddleDftRData) *data =
      (struct NS_(EvenSizeTwiddleDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long dft_size = data->dft_size;
  data->evaluate_head(dft_size, x, y);
  const Long batch_size = dft_size / 2;
  const Complex *w = data->w;
  Complex *x0 = x + 1;
  Complex *x1 = x + dft_size;
  Complex *y0 = y + 1;
  Complex *y1 = y + dft_size;
  const struct NS_(EvenSizeTwiddleDftFunctions) *f = &(data->f);
  if (f->evaluate) {
    Long batch_size_ = I_FLOOR_MULTIPLE(batch_size, C_SIMD_SIZE);
    f->evaluate(batch_size_, w, x0, x1, y0, y1);
    w += batch_size_;
    x0 += batch_size_;
    x1 -= batch_size_;
    y0 += batch_size_;
    y1 -= batch_size_;
  }
  NS_(EvenSizeTwiddleDftR_EvaluateTail)(data, w, x0, x1, y0, y1);
}

#if USE_OPENMP

static void NS_(EvenSizeTwiddleDftR_evaluate_omp)(struct NS(ComplexFunction) *p,
                                                  Complex *x, Complex *y) {
  struct NS_(EvenSizeTwiddleDftRData) *data =
      (struct NS_(EvenSizeTwiddleDftRData) *)NS(ComplexFunction_data)(p);
  assert(data);
  const Long dft_size = data->dft_size;
  data->evaluate_head(dft_size, x, y);
  const Long batch_size = dft_size / 2;
  const Complex *w = data->w;
  Complex *x0 = x + 1;
  Complex *x1 = x + dft_size;
  Complex *y0 = y + 1;
  Complex *y1 = y + dft_size;
  const struct NS_(EvenSizeTwiddleDftFunctions) *f = &(data->f);
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
      Complex *x0_ = x0 + begin;
      Complex *x1_ = x1 - begin;
      Complex *y0_ = y0 + begin;
      Complex *y1_ = y1 - begin;
      f->evaluate(batch_size_, w_, x0_, x1_, y0_, y1_);
    }
#pragma omp single nowait
    {
      k = num_threads - 1;
      const Long begin = range[k];
      const Long end = range[k + 1];
      const Long batch_size_ = end - begin;
      const Complex *w_ = w + begin;
      Complex *x0_ = x0 + begin;
      Complex *x1_ = x1 - begin;
      Complex *y0_ = y0 + begin;
      Complex *y1_ = y1 - begin;
      f->evaluate(batch_size_, w_, x0_, x1_, y0_, y1_);
      w_ += batch_size_;
      x0_ += batch_size_;
      x1_ -= batch_size_;
      y0_ += batch_size_;
      y1_ -= batch_size_;
      NS_(EvenSizeTwiddleDftR_EvaluateTail)(data, w_, x0_, x1_, y0_, y1_);
    }
  }
}

#endif

static struct NS(ComplexFunction) *NS_(EvenSizeTwiddleDftR_new)(
    NS_(EvenSizeTwiddleDftHead_fn) evaluate_head,
    const struct NS_(EvenSizeTwiddleDftFunctions) *f, Long dft_size,
    const Complex *w, int num_threads) {
  const Long batch_size = dft_size / 2;
#if USE_OPENMP
  num_threads =
      NS(Threading_GetNumThreads)(batch_size, C_SIMD_SIZE, num_threads);
  NS(ComplexFunction_fn) evaluate = (num_threads > 1)
                                        ? NS_(EvenSizeTwiddleDftR_evaluate_omp)
                                        : NS_(EvenSizeTwiddleDftR_evaluate);
#else
  NS(ComplexFunction_fn) evaluate = NS_(EvenSizeTwiddleDftR_evaluate);
#endif
  struct NS_(EvenSizeTwiddleDftFunctions) f_ =
      (struct NS_(EvenSizeTwiddleDftFunctions)) {
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
      NS_(EvenSizeTwiddleDftRData_new)(evaluate_head, &f_, dft_size, w,
                                       num_threads));
}

#endif

struct NS_(EvenSizeTwiddleDftFactory) {
  struct NS_(EvenSizeTwiddleDftFunctions) forward;
  struct NS_(EvenSizeTwiddleDftFunctions) backward;
};

#if C_SIMD_SIZE == 1
static const struct NS_(EvenSizeTwiddleDftFactory) g_factory = {
    {NS_(EvenSizeTwiddleDft_F_1)}, {NS_(EvenSizeTwiddleDft_B_1)}};
#elif C_SIMD_SIZE == 2
static const struct NS_(EvenSizeTwiddleDftFactory) g_factory = {
    {NS_(EvenSizeTwiddleDft_F_1), NS_(EvenSizeTwiddleDft_F_2)},
    {NS_(EvenSizeTwiddleDft_B_1), NS_(EvenSizeTwiddleDft_B_2)}};
#elif C_SIMD_SIZE == 4
static const struct NS_(EvenSizeTwiddleDftFactory) g_factory = {
    {NS_(EvenSizeTwiddleDft_F_1), NS_(EvenSizeTwiddleDft_F_2),
     NS_(EvenSizeTwiddleDft_F_4)},
    {NS_(EvenSizeTwiddleDft_B_1), NS_(EvenSizeTwiddleDft_B_2),
     NS_(EvenSizeTwiddleDft_B_4)}};
#elif C_SIMD_SIZE == 8
static const struct NS_(EvenSizeTwiddleDftFactory) g_factory = {
    {NS_(EvenSizeTwiddleDft_F_1), NS_(EvenSizeTwiddleDft_F_2),
     NS_(EvenSizeTwiddleDft_F_4), NS_(EvenSizeTwiddleDft_F_8)},
    {NS_(EvenSizeTwiddleDft_B_1), NS_(EvenSizeTwiddleDft_B_2),
     NS_(EvenSizeTwiddleDft_B_4), NS_(EvenSizeTwiddleDft_B_8)}};
#endif

static struct NS(ComplexFunction) *NS_(EvenSizeTwiddleDftFactory_create)(
    NS_(EvenSizeTwiddleDftHead_fn) evaluate_head,
    const struct NS_(EvenSizeTwiddleDftFunctions) *f,
    struct NS(DataFactory) *data, Long dft_size, int num_threads) {
  assert(f);
  const Long batch_size = dft_size / 2;
  const Complex *w =
      NS(DataFactory_twiddleVector)(data, batch_size, 2 * dft_size, 1);
#if C_SIMD_SIZE > 1
  if (batch_size % C_SIMD_SIZE) {
    return NS_(EvenSizeTwiddleDftR_new)(evaluate_head, f, dft_size, w,
                                        num_threads);
  } else {
#else
  {
#endif
    return NS_(EvenSizeTwiddleDft_new)(evaluate_head, f->evaluate, dft_size, w,
                                       num_threads);
  }
}

struct NS(ComplexFunction) *NS_(EvenSizeTwiddleDft_F_new_)(
    struct NS(DataFactory) *data, Long dft_size, int num_threads) {
  return NS_(EvenSizeTwiddleDftFactory_create)(
      NS_(EvenSizeTwiddleDft_F_EvaluateHead), &g_factory.forward, data,
      dft_size, num_threads);
}

struct NS(ComplexFunction) *NS_(EvenSizeTwiddleDft_B_new_)(
    struct NS(DataFactory) *data, Long dft_size, int num_threads) {
  return NS_(EvenSizeTwiddleDftFactory_create)(
      NS_(EvenSizeTwiddleDft_B_EvaluateHead), &g_factory.backward, data,
      dft_size, num_threads);
}

struct NS(ComplexFunction) *NS_(EvenSizeTwiddleDft_F_new)(
    struct NS(DataFactory) *data, Long dft_size) {
  return NS_(EvenSizeTwiddleDft_F_new_)(data, dft_size, 1);
}

struct NS(ComplexFunction) *NS_(EvenSizeTwiddleDft_B_new)(
    struct NS(DataFactory) *data, Long dft_size) {
  return NS_(EvenSizeTwiddleDft_B_new_)(data, dft_size, 1);
}
