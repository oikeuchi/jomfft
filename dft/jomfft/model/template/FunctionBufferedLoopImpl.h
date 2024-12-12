#if USE_OPENMP

#include "../ThreadingUtil.h"
#include <jomfft/VLA.h>

struct FUNCTION_(BufferedLoopData) {
  struct FUNCTIONW *func;
  Long loop_size;
  Long x_stride;
  Long y_stride;
  int num_threads;
};

static struct FUNCTION_(BufferedLoopData) *FUNCTION_(BufferedLoopData_new)(
    struct FUNCTIONW *func, Long loop_size, Long x_stride, Long y_stride,
    int num_threads) {
  struct FUNCTION_(BufferedLoopData) *out =
      NEW(struct FUNCTION_(BufferedLoopData));
  *out = (struct FUNCTION_(BufferedLoopData)){.func = func,
                                              .loop_size = loop_size,
                                              .x_stride = x_stride,
                                              .y_stride = y_stride,
                                              .num_threads = num_threads};
  return out;
}

static void FUNCTION_(BufferedLoopData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(BufferedLoopData) *o =
        (struct FUNCTION_(BufferedLoopData) *)p;
    if (o->func) {
      FUNCTIONW_(_delete)(o->func);
      o->func = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(BufferedHeapLoop_evaluate_omp)(struct FUNCTION *p,
                                                     INPUT_TYPE *x,
                                                     OUTPUT_TYPE *y) {
  struct FUNCTION_(BufferedLoopData) *data =
      (struct FUNCTION_(BufferedLoopData) *)FUNCTION_(_data)(p);
  assert(data);
  const int num_threads = data->num_threads;
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const Long work_size = FUNCTIONW_(_size)(data->func);
  DECLARE_VLA(char *, works, num_threads);
  for (int k = 0; k < num_threads; ++k) {
    works[k] = ARRAY_NEW(char, work_size);
  }
  Long i;
#pragma omp parallel for num_threads(num_threads)
  for (i = 0; i < loop_size; ++i) {
    char *work = works[omp_get_thread_num()];
    FUNCTIONW_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i], work);
  }
  for (int k = 0; k < num_threads; ++k) {
    ARRAY_DELETE(works[k]);
  }
}

static void FUNCTION_(BufferedStackLoop_evaluate_omp)(struct FUNCTION *p,
                                                      INPUT_TYPE *x,
                                                      OUTPUT_TYPE *y) {
  struct FUNCTION_(BufferedLoopData) *data =
      (struct FUNCTION_(BufferedLoopData) *)FUNCTION_(_data)(p);
  assert(data);
  const int num_threads = data->num_threads;
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const Long work_size = FUNCTIONW_(_size)(data->func);
  Long i;
#pragma omp parallel for num_threads(num_threads)
  for (i = 0; i < loop_size; ++i) {
    DECLARE_ALIGNED_VLA(char, work, work_size);
    FUNCTIONW_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i], work);
  }
}

struct FUNCTION_(BufferedLoopRData) {
  struct FUNCTIONW *func;
  struct FUNCTIONW *residual_func;
  Long loop_size;
  Long x_stride;
  Long y_stride;
  int num_threads;
};

static struct FUNCTION_(BufferedLoopRData) *FUNCTION_(BufferedLoopRData_new)(
    struct FUNCTIONW *func, struct FUNCTIONW *residual_func, Long loop_size,
    Long x_stride, Long y_stride, int num_threads) {
  struct FUNCTION_(BufferedLoopRData) *out =
      NEW(struct FUNCTION_(BufferedLoopRData));
  *out = (struct FUNCTION_(BufferedLoopRData)){.func = func,
                                               .residual_func = residual_func,
                                               .loop_size = loop_size,
                                               .x_stride = x_stride,
                                               .y_stride = y_stride,
                                               .num_threads = num_threads};
  return out;
}

static void FUNCTION_(BufferedLoopRData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(BufferedLoopRData) *o =
        (struct FUNCTION_(BufferedLoopRData) *)p;
    if (o->func) {
      FUNCTIONW_(_delete)(o->func);
      o->func = NULL;
    }
    if (o->residual_func) {
      FUNCTIONW_(_delete)(o->residual_func);
      o->residual_func = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(BufferedHeapLoopR_evaluate_omp)(struct FUNCTION *p,
                                                      INPUT_TYPE *x,
                                                      OUTPUT_TYPE *y) {
  struct FUNCTION_(BufferedLoopRData) *data =
      (struct FUNCTION_(BufferedLoopRData) *)FUNCTION_(_data)(p);
  assert(data);
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const Long work_size = MAX(FUNCTIONW_(_size)(data->func),
                             FUNCTIONW_(_size)(data->residual_func));
  const int num_threads = data->num_threads;
  DECLARE_VLA(char *, works, num_threads);
  for (int k = 0; k < num_threads; ++k) {
    works[k] = ARRAY_NEW(char, work_size);
  }
  Long i;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for nowait
    for (i = 0; i < loop_size; ++i) {
      char *work = works[omp_get_thread_num()];
      FUNCTIONW_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i],
                            work);
    }
#pragma omp single nowait
    {
      i = loop_size;
      char *work = works[omp_get_thread_num()];
      FUNCTIONW_(_evaluate)(data->residual_func, &x[x_stride * i],
                            &y[y_stride * i], work);
    }
  }
}

static void FUNCTION_(BufferedStackLoopR_evaluate_omp)(struct FUNCTION *p,
                                                       INPUT_TYPE *x,
                                                       OUTPUT_TYPE *y) {
  struct FUNCTION_(BufferedLoopRData) *data =
      (struct FUNCTION_(BufferedLoopRData) *)FUNCTION_(_data)(p);
  assert(data);
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  const Long work_size = FUNCTIONW_(_size)(data->func);
  const Long work_size_r = FUNCTIONW_(_size)(data->residual_func);
  const int num_threads = data->num_threads;
  Long i;
#pragma omp parallel num_threads(num_threads)
  {
#pragma omp for nowait
    for (i = 0; i < loop_size; ++i) {
      DECLARE_ALIGNED_VLA(char, work, work_size);
      FUNCTIONW_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i],
                            work);
    }
#pragma omp single nowait
    {
      i = loop_size;
      DECLARE_ALIGNED_VLA(char, work, work_size_r);
      FUNCTIONW_(_evaluate)(data->residual_func, &x[x_stride * i],
                            &y[y_stride * i], work);
    }
  }
}

#endif

struct FUNCTION *FUNCTION_(BufferedLoop_new_)(struct FUNCTIONW *func,
                                              Long loop_size, Long x_stride,
                                              Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads = (int)MIN((Long)num_threads, loop_size);
  if (num_threads > 1) {
    const Long size = FUNCTIONW_(_size)(func);
    FUNCTION_(_fn)
    evaluate = (size > STACK_BUFFER_MAX_SIZE)
                   ? FUNCTION_(BufferedHeapLoop_evaluate_omp)
                   : FUNCTION_(BufferedStackLoop_evaluate_omp);
    return FUNCTION_(_new)(evaluate, FUNCTION_(BufferedLoopData_destruct),
                           FUNCTION_(BufferedLoopData_new)(func, loop_size,
                                                           x_stride, y_stride,
                                                           num_threads));
  } else {
#else
  {
    (void)num_threads;
#endif
    return FUNCTION_(Buffered_new)(
        FUNCTIONW_(Loop_new)(func, loop_size, x_stride, y_stride));
  }
}

struct FUNCTION *FUNCTION_(BufferedLoopR_new_)(struct FUNCTIONW *func,
                                               struct FUNCTIONW *residual_func,
                                               Long loop_size, Long x_stride,
                                               Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads = (int)MIN((Long)num_threads, loop_size + 1);
  if (num_threads > 1) {
    const Long size =
        MAX(FUNCTIONW_(_size)(func), FUNCTIONW_(_size)(residual_func));
    FUNCTION_(_fn)
    evaluate = (size > STACK_BUFFER_MAX_SIZE)
                   ? FUNCTION_(BufferedHeapLoopR_evaluate_omp)
                   : FUNCTION_(BufferedStackLoopR_evaluate_omp);
    return FUNCTION_(_new)(
        evaluate, FUNCTION_(BufferedLoopRData_destruct),
        FUNCTION_(BufferedLoopRData_new)(func, residual_func, loop_size,
                                         x_stride, y_stride, num_threads));
  } else {
#else
  {
    (void)num_threads;
#endif
    return FUNCTION_(Buffered_new)(FUNCTIONW_(LoopR_new)(
        func, residual_func, loop_size, x_stride, y_stride));
  }
}

struct FUNCTION *FUNCTION_(BufferedLoop_new)(struct FUNCTIONW *func,
                                             Long loop_size, Long x_stride,
                                             Long y_stride) {
  return FUNCTION_(BufferedLoop_new_)(func, loop_size, x_stride, y_stride, 1);
}

struct FUNCTION *FUNCTION_(BufferedLoopR_new)(struct FUNCTIONW *func,
                                              struct FUNCTIONW *residual_func,
                                              Long loop_size, Long x_stride,
                                              Long y_stride) {
  return FUNCTION_(BufferedLoopR_new_)(func, residual_func, loop_size, x_stride,
                                       y_stride, 1);
}
