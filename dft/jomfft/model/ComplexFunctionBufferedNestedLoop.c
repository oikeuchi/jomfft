#include "ComplexFunctionBufferedNestedLoop.h"
#include "ComplexFunctionBuffered.h"
#include "ComplexFunctionWLoop.h"
#include <jomfft/VLA.h>

#if USE_OPENMP

struct NS(ComplexFunctionBufferedNestedLoopData) {
  struct NS(ComplexFunctionW) *func;
  Long loop_size_0;
  Long loop_size_1;
  Long x_stride_0;
  Long x_stride_1;
  Long y_stride_0;
  Long y_stride_1;
  int num_threads;
};

static struct NS(ComplexFunctionBufferedNestedLoopData) *
ComplexFunctionBufferedNestedLoopData_new(struct NS(ComplexFunctionW) *func,
                                          Long loop_size_0, Long loop_size_1,
                                          Long x_stride_0, Long x_stride_1,
                                          Long y_stride_0, Long y_stride_1,
                                          int num_threads) {
  struct NS(ComplexFunctionBufferedNestedLoopData) *out =
      NEW(struct NS(ComplexFunctionBufferedNestedLoopData));
  *out = (struct NS(ComplexFunctionBufferedNestedLoopData)){
      .func = func,
      .loop_size_0 = loop_size_0,
      .loop_size_1 = loop_size_1,
      .x_stride_0 = x_stride_0,
      .x_stride_1 = x_stride_1,
      .y_stride_0 = y_stride_0,
      .y_stride_1 = y_stride_1,
      .num_threads = num_threads};
  return out;
}

static void ComplexFunctionBufferedNestedLoopData_destruct(void *p) {
  if (p) {
    struct NS(ComplexFunctionBufferedNestedLoopData) *o =
        (struct NS(ComplexFunctionBufferedNestedLoopData) *)p;
    if (o->func) {
      NS(ComplexFunctionW_delete)(o->func);
      o->func = NULL;
    }
    DELETE(o);
  }
}

static void ComplexFunctionBufferedHeapNestedLoop_evaluate_omp(
    struct NS(ComplexFunction) *p, Complex *x, Complex *y) {
  struct NS(ComplexFunctionBufferedNestedLoopData) *data =
      (struct NS(ComplexFunctionBufferedNestedLoopData) *)
          NS(ComplexFunction_data)(p);
  const int num_threads = data->num_threads;
  const Long work_size = NS(ComplexFunctionW_size)(data->func);
  DECLARE_VLA(char *, works, num_threads);
  for (int k = 0; k < num_threads; ++k) {
    works[k] = ARRAY_NEW(char, work_size);
  }
  const Long loop_size_0 = data->loop_size_0;
  const Long loop_size_1 = data->loop_size_1;
  Long i, j;
#pragma omp parallel for collapse(2) num_threads(num_threads)
  for (i = 0; i < loop_size_0; ++i) {
    for (j = 0; j < loop_size_1; ++j) {
      char *work = works[omp_get_thread_num()];
      Complex *x_ = &x[data->x_stride_0 * i + data->x_stride_1 * j];
      Complex *y_ = &y[data->y_stride_0 * i + data->y_stride_1 * j];
      NS(ComplexFunctionW_evaluate)(data->func, x_, y_, work);
    }
  }
  for (int k = 0; k < num_threads; ++k) {
    ARRAY_DELETE(works[k]);
  }
}

static void ComplexFunctionBufferedStackNestedLoop_evaluate_omp(
    struct NS(ComplexFunction) *p, Complex *x, Complex *y) {
  struct NS(ComplexFunctionBufferedNestedLoopData) *data =
      (struct NS(ComplexFunctionBufferedNestedLoopData) *)
          NS(ComplexFunction_data)(p);
  const int num_threads = data->num_threads;
  const Long work_size = NS(ComplexFunctionW_size)(data->func);
  const Long loop_size_0 = data->loop_size_0;
  const Long loop_size_1 = data->loop_size_1;
  Long i, j;
#pragma omp parallel for collapse(2) num_threads(num_threads)
  for (i = 0; i < loop_size_0; ++i) {
    for (j = 0; j < loop_size_1; ++j) {
      DECLARE_ALIGNED_VLA(char, work, work_size);
      Complex *x_ = &x[data->x_stride_0 * i + data->x_stride_1 * j];
      Complex *y_ = &y[data->y_stride_0 * i + data->y_stride_1 * j];
      NS(ComplexFunctionW_evaluate)(data->func, x_, y_, work);
    }
  }
}

#endif

struct NS(ComplexFunction) *NS(ComplexFunctionBufferedNestedLoop_new)(
    struct NS(ComplexFunctionW) *func, Long loop_size_0, Long loop_size_1,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  assert(loop_size_0 >= 1);
  assert(loop_size_1 >= 1);
#if USE_OPENMP
  if (num_threads > 1) {
    num_threads = (int)MIN((Long)num_threads, loop_size_0 * loop_size_1);
    const Long size = NS(ComplexFunctionW_size)(func);
    NS(ComplexFunction_fn) evaluate =
        (size > STACK_BUFFER_MAX_SIZE)
            ? ComplexFunctionBufferedHeapNestedLoop_evaluate_omp
            : ComplexFunctionBufferedStackNestedLoop_evaluate_omp;
    return NS(ComplexFunction_new)(
        evaluate, ComplexFunctionBufferedNestedLoopData_destruct,
        ComplexFunctionBufferedNestedLoopData_new(
            func, loop_size_0, loop_size_1, x_stride_0, x_stride_1, y_stride_0,
            y_stride_1, num_threads));
  } else {
#else
  {
    (void)num_threads;
#endif
    return NS(ComplexFunctionBuffered_new)(NS(ComplexFunctionWLoop_new)(
        NS(ComplexFunctionWLoop_new)(func, loop_size_1, x_stride_1, y_stride_1),
        loop_size_0, x_stride_0, y_stride_0));
  }
}
