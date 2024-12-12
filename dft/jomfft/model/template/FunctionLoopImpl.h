#include "../ThreadingUtil.h"

struct FUNCTION_(LoopData) {
  struct FUNCTION *func;
  Long loop_size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct FUNCTION_(LoopData) *FUNCTION_(LoopData_new)(
    struct FUNCTION *func, Long loop_size, Long x_stride, Long y_stride,
    int num_threads) {
  struct FUNCTION_(LoopData) *out = NEW(struct FUNCTION_(LoopData));
  *out = (struct FUNCTION_(LoopData)){.func = func,
                                      .loop_size = loop_size,
                                      .x_stride = x_stride,
                                      .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void FUNCTION_(LoopData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(LoopData) *o = (struct FUNCTION_(LoopData) *)p;
    if (o->func) {
      FUNCTION_(_delete)(o->func);
      o->func = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(Loop_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                     OUTPUT_TYPE *y) {
  struct FUNCTION_(LoopData) *data =
      (struct FUNCTION_(LoopData) *)FUNCTION_(_data)(p);
  assert(data);
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  for (Long i = 0; i < loop_size; ++i) {
    FUNCTION_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i]);
  }
}

#if USE_OPENMP

static void FUNCTION_(Loop_evaluate_omp)(struct FUNCTION *p, INPUT_TYPE *x,
                                         OUTPUT_TYPE *y) {
  struct FUNCTION_(LoopData) *data =
      (struct FUNCTION_(LoopData) *)FUNCTION_(_data)(p);
  assert(data);
  const int num_threads = data->num_threads;
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  Long i;
#pragma omp parallel for num_threads(num_threads)
  for (i = 0; i < loop_size; ++i) {
    FUNCTION_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i]);
  }
}

#endif

struct FUNCTION *FUNCTION_(Loop_new)(struct FUNCTION *func, Long loop_size,
                                     Long x_stride, Long y_stride) {
  return FUNCTION_(Loop_new_)(func, loop_size, x_stride, y_stride, 1);
}

struct FUNCTION *FUNCTION_(Loop_new_)(struct FUNCTION *func, Long loop_size,
                                      Long x_stride, Long y_stride,
                                      int num_threads) {
  assert(loop_size >= 1);
  if (loop_size == 1) {
    return func;
  } else {
#if USE_OPENMP
    num_threads = NS(Threading_GetNumThreads)(loop_size, 1, num_threads);
    FUNCTION_(_fn) evaluate = (num_threads > 1) ? FUNCTION_(Loop_evaluate_omp)
                                                : FUNCTION_(Loop_evaluate);
#else
    FUNCTION_(_fn) evaluate = FUNCTION_(Loop_evaluate);
#endif
    return FUNCTION_(_new)(evaluate, FUNCTION_(LoopData_destruct),
                           FUNCTION_(LoopData_new)(func, loop_size, x_stride,
                                                   y_stride, num_threads));
  }
}

struct FUNCTION_(LoopRData) {
  struct FUNCTION *func;
  struct FUNCTION *residual_func;
  Long loop_size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct FUNCTION_(LoopRData) *FUNCTION_(LoopRData_new)(
    struct FUNCTION *func, struct FUNCTION *residual_func, Long loop_size,
    Long x_stride, Long y_stride, int num_threads) {
  struct FUNCTION_(LoopRData) *out = NEW(struct FUNCTION_(LoopRData));
  *out = (struct FUNCTION_(LoopRData)){.func = func,
                                       .residual_func = residual_func,
                                       .loop_size = loop_size,
                                       .x_stride = x_stride,
                                       .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void FUNCTION_(LoopRData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(LoopRData) *o = (struct FUNCTION_(LoopRData) *)p;
    if (o->func) {
      FUNCTION_(_delete)(o->func);
      o->func = NULL;
    }
    if (o->residual_func) {
      FUNCTION_(_delete)(o->residual_func);
      o->residual_func = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(LoopR_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                      OUTPUT_TYPE *y) {
  struct FUNCTION_(LoopRData) *data =
      (struct FUNCTION_(LoopRData) *)FUNCTION_(_data)(p);
  assert(data);
  assert(data->func);
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  Long i = 0;
  for (; i < loop_size; ++i) {
    FUNCTION_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i]);
  }
  FUNCTION_(_evaluate)(data->residual_func, &x[x_stride * i], &y[y_stride * i]);
}

#if USE_OPENMP

static void FUNCTION_(LoopR_evaluate_omp)(struct FUNCTION *p, INPUT_TYPE *x,
                                          OUTPUT_TYPE *y) {
  struct FUNCTION_(LoopRData) *data =
      (struct FUNCTION_(LoopRData) *)FUNCTION_(_data)(p);
  assert(data);
  assert(data->func);
  const int num_threads = data->num_threads;
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
#pragma omp parallel num_threads(num_threads)
  {
    {
      Long i;
#pragma omp for nowait
      for (i = 0; i < loop_size; ++i) {
        FUNCTION_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i]);
      }
    }
#pragma omp single nowait
    {
      const Long i = loop_size;
      FUNCTION_(_evaluate)(data->residual_func, &x[x_stride * i],
                           &y[y_stride * i]);
    }
  }
}

#endif

struct FUNCTION *FUNCTION_(LoopR_new)(struct FUNCTION *func,
                                      struct FUNCTION *residual_func,
                                      Long loop_size, Long x_stride,
                                      Long y_stride) {
  return FUNCTION_(LoopR_new_)(func, residual_func, loop_size, x_stride,
                               y_stride, 1);
}

struct FUNCTION *FUNCTION_(LoopR_new_)(struct FUNCTION *func,
                                       struct FUNCTION *residual_func,
                                       Long loop_size, Long x_stride,
                                       Long y_stride, int num_threads) {
#if USE_OPENMP
  num_threads = NS(Threading_GetNumThreads)(loop_size + 1, 1, num_threads);
  FUNCTION_(_fn)
  evaluate = (num_threads > 1) ? FUNCTION_(LoopR_evaluate_omp)
                               : FUNCTION_(LoopR_evaluate);
#else
  FUNCTION_(_fn) evaluate = FUNCTION_(LoopR_evaluate);
#endif
  return FUNCTION_(_new)(evaluate, FUNCTION_(LoopRData_destruct),
                         FUNCTION_(LoopRData_new)(func, residual_func,
                                                  loop_size, x_stride, y_stride,
                                                  num_threads));
}
