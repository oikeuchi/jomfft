#include "SplitComplexCopy.h"

#include "../util/SplitComplexCopy.h"
#include "ThreadingUtil.h"
#include <jomfft/VLA.h>

struct NS(SplitComplex_CopyData) {
  Long size;
  Long x_stride;
  Long y_stride;
  Long split_distance;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS(SplitComplex_CopyData) *
SplitComplex_CopyData_new(Long size, Long x_stride, Long y_stride,
                          Long split_distance, int num_threads) {
  struct NS(SplitComplex_CopyData) *out = NEW(struct NS(SplitComplex_CopyData));
  *out = (struct NS(SplitComplex_CopyData)){.size = size,
                                            .x_stride = x_stride,
                                            .y_stride = y_stride,
                                            .split_distance = split_distance};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void SplitComplex_Copy_F_evaluate(struct NS(RealToComplexFunction) *p,
                                         Real *x, Complex *y) {
  struct NS(SplitComplex_CopyData) *data =
      (struct NS(SplitComplex_CopyData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  NS(SplitComplex_Copy_F)(data->size, x, x + data->split_distance,
                          data->x_stride, y, data->y_stride);
}

#if USE_OPENMP

static void
SplitComplex_Copy_F_evaluate_omp(struct NS(RealToComplexFunction) *p, Real *x,
                                 Complex *y) {
  struct NS(SplitComplex_CopyData) *data =
      (struct NS(SplitComplex_CopyData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  Real *x0 = x;
  Real *x1 = x + data->split_distance;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(data->size, 1, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Real *x0_ = x0 + x_stride * begin;
    Real *x1_ = x1 + x_stride * begin;
    Complex *y_ = y + y_stride * begin;
    NS(SplitComplex_Copy_F)(batch_size, x0_, x1_, x_stride, y_, y_stride);
  }
}

#endif

struct NS(RealToComplexFunction) *NS(SplitComplex_Copy_F_new)(
    Long size, Long x_stride, Long y_stride, Long split_distance,
    int num_threads) {
#if USE_OPENMP
  num_threads = NS(Threading_GetNumThreads)(size, 1, num_threads);
  NS(RealToComplexFunction_fn) evaluate = (num_threads > 1 && size > 1)
                                              ? SplitComplex_Copy_F_evaluate_omp
                                              : SplitComplex_Copy_F_evaluate;
#else
  NS(RealToComplexFunction_fn) evaluate = SplitComplex_Copy_F_evaluate;
#endif
  return NS(RealToComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      SplitComplex_CopyData_new(size, x_stride, y_stride, split_distance,
                                num_threads));
}

static void SplitComplex_Copy_B_evaluate(struct NS(ComplexToRealFunction) *p,
                                         Complex *x, Real *y) {
  struct NS(SplitComplex_CopyData) *data =
      (struct NS(SplitComplex_CopyData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  NS(SplitComplex_Copy_B)(data->size, x, data->x_stride, y,
                          y + data->split_distance, data->y_stride);
}

#if USE_OPENMP

static void
SplitComplex_Copy_B_evaluate_omp(struct NS(ComplexToRealFunction) *p,
                                 Complex *x, Real *y) {
  struct NS(SplitComplex_CopyData) *data =
      (struct NS(SplitComplex_CopyData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  Real *y0 = y;
  Real *y1 = y + data->split_distance;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(data->size, 1, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Complex *x_ = x + x_stride * begin;
    Real *y0_ = y0 + y_stride * begin;
    Real *y1_ = y1 + y_stride * begin;
    NS(SplitComplex_Copy_B)(batch_size, x_, x_stride, y0_, y1_, y_stride);
  }
}

#endif

struct NS(ComplexToRealFunction) *NS(SplitComplex_Copy_B_new)(
    Long size, Long x_stride, Long y_stride, Long split_distance,
    int num_threads) {
#if USE_OPENMP
  num_threads = NS(Threading_GetNumThreads)(size, 1, num_threads);
  NS(ComplexToRealFunction_fn) evaluate = (num_threads > 1 && size > 1)
                                              ? SplitComplex_Copy_B_evaluate_omp
                                              : SplitComplex_Copy_B_evaluate;
#else
  NS(ComplexToRealFunction_fn) evaluate = SplitComplex_Copy_B_evaluate;
#endif
  return NS(ComplexToRealFunction_new)(
      evaluate, jomfftDestructPointer,
      SplitComplex_CopyData_new(size, x_stride, y_stride, split_distance,
                                num_threads));
}

struct NS(SplitComplex_CopyRealData) {
  Long size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS(SplitComplex_CopyRealData) *
SplitComplex_CopyRealData_new(Long size, Long x_stride, Long y_stride,
                              int num_threads) {
  struct NS(SplitComplex_CopyRealData) *out =
      NEW(struct NS(SplitComplex_CopyRealData));
  *out = (struct NS(SplitComplex_CopyRealData)){
      .size = size, .x_stride = x_stride, .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void
SplitComplex_CopyReal_F_evaluate(struct NS(RealToComplexFunction) *p, Real *x,
                                 Complex *y) {
  struct NS(SplitComplex_CopyRealData) *data =
      (struct NS(SplitComplex_CopyRealData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  NS(SplitComplex_CopyReal_F)(data->size, x, data->x_stride, y, data->y_stride);
}

#if USE_OPENMP

static void
SplitComplex_CopyReal_F_evaluate_omp(struct NS(RealToComplexFunction) *p,
                                     Real *x, Complex *y) {
  struct NS(SplitComplex_CopyRealData) *data =
      (struct NS(SplitComplex_CopyRealData) *)NS(RealToComplexFunction_data)(p);
  assert(data);
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(data->size, 1, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Real *x_ = x + x_stride * begin;
    Complex *y_ = y + y_stride * begin;
    NS(SplitComplex_CopyReal_F)(batch_size, x_, x_stride, y_, y_stride);
  }
}

#endif

struct NS(RealToComplexFunction) *NS(SplitComplex_CopyReal_F_new)(
    Long size, Long x_stride, Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads = NS(Threading_GetNumThreads)(size, 1, num_threads);
  NS(RealToComplexFunction_fn) evaluate =
      (num_threads > 1 && size > 1) ? SplitComplex_CopyReal_F_evaluate_omp
                                    : SplitComplex_CopyReal_F_evaluate;
#else
  NS(RealToComplexFunction_fn) evaluate = SplitComplex_CopyReal_F_evaluate;
#endif
  return NS(RealToComplexFunction_new)(
      evaluate, jomfftDestructPointer,
      SplitComplex_CopyRealData_new(size, x_stride, y_stride, num_threads));
}

static void
SplitComplex_CopyReal_B_evaluate(struct NS(ComplexToRealFunction) *p,
                                 Complex *x, Real *y) {
  struct NS(SplitComplex_CopyRealData) *data =
      (struct NS(SplitComplex_CopyRealData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  NS(SplitComplex_CopyReal_B)(data->size, x, data->x_stride, y, data->y_stride);
}

#if USE_OPENMP

static void
SplitComplex_CopyReal_B_evaluate_omp(struct NS(ComplexToRealFunction) *p,
                                     Complex *x, Real *y) {
  struct NS(SplitComplex_CopyRealData) *data =
      (struct NS(SplitComplex_CopyRealData) *)NS(ComplexToRealFunction_data)(p);
  assert(data);
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(data->size, 1, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    Complex *x_ = x + x_stride * begin;
    Real *y_ = y + y_stride * begin;
    NS(SplitComplex_CopyReal_B)(batch_size, x_, x_stride, y_, y_stride);
  }
}

#endif

struct NS(ComplexToRealFunction) *NS(SplitComplex_CopyReal_B_new)(
    Long size, Long x_stride, Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads = NS(Threading_GetNumThreads)(size, 1, num_threads);
  NS(ComplexToRealFunction_fn) evaluate =
      (num_threads > 1 && size > 1) ? SplitComplex_CopyReal_B_evaluate_omp
                                    : SplitComplex_CopyReal_B_evaluate;
#else
  NS(ComplexToRealFunction_fn) evaluate = SplitComplex_CopyReal_B_evaluate;
#endif
  return NS(ComplexToRealFunction_new)(
      evaluate, jomfftDestructPointer,
      SplitComplex_CopyRealData_new(size, x_stride, y_stride, num_threads));
}
