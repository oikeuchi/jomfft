#include <jomfft/VLA.h>

#include "../ThreadingUtil.h"

struct VALUE_TYPE_(Copy_1Data) {
  Long size;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct VALUE_TYPE_(Copy_1Data) *VALUE_TYPE_(Copy_1Data_new)(
    Long size, int num_threads) {
  struct VALUE_TYPE_(Copy_1Data) *out = NEW(struct VALUE_TYPE_(Copy_1Data));
  *out = (struct VALUE_TYPE_(Copy_1Data)){.size = size};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void VALUE_TYPE_(Copy_1_evaluate)(struct FUNCTION *p, VALUE_TYPE *x,
                                         VALUE_TYPE *y) {
  struct VALUE_TYPE_(Copy_1Data) *data =
      (struct VALUE_TYPE_(Copy_1Data) *)FUNCTION_(data)(p);
  assert(data);
  VALUE_TYPE_(Copy_1)(data->size, x, y);
}

#if USE_OPENMP

static void VALUE_TYPE_(Copy_1_evaluate_omp)(struct FUNCTION *p, VALUE_TYPE *x,
                                             VALUE_TYPE *y) {
  struct VALUE_TYPE_(Copy_1Data) *data =
      (struct VALUE_TYPE_(Copy_1Data) *)FUNCTION_(data)(p);
  assert(data);
  const int num_threads = data->num_threads;
  DECLARE_VLA(Long, range, num_threads + 1);
  NS(Threading_GetBatchRange)(data->size, 1, num_threads, range);
  int k;
#pragma omp parallel for num_threads(num_threads)
  for (k = 0; k < num_threads; ++k) {
    const Long begin = range[k];
    const Long end = range[k + 1];
    const Long batch_size = end - begin;
    VALUE_TYPE_(Copy_1)(batch_size, x + begin, y + begin);
  }
}

#endif

struct VALUE_TYPE_(CopyData) {
  Long size;
  Long x_stride;
  Long y_stride;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct VALUE_TYPE_(CopyData) *VALUE_TYPE_(CopyData_new)(
    Long size, Long x_stride, Long y_stride, int num_threads) {
  struct VALUE_TYPE_(CopyData) *out = NEW(struct VALUE_TYPE_(CopyData));
  *out = (struct VALUE_TYPE_(CopyData)){
      .size = size, .x_stride = x_stride, .y_stride = y_stride};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void VALUE_TYPE_(Copy_evaluate)(struct FUNCTION *p, VALUE_TYPE *x,
                                       VALUE_TYPE *y) {
  struct VALUE_TYPE_(CopyData) *data =
      (struct VALUE_TYPE_(CopyData) *)FUNCTION_(data)(p);
  assert(data);
  VALUE_TYPE_(Copy)(data->size, x, data->x_stride, y, data->y_stride);
}

#if USE_OPENMP

static void VALUE_TYPE_(Copy_evaluate_omp)(struct FUNCTION *p, VALUE_TYPE *x,
                                           VALUE_TYPE *y) {
  struct VALUE_TYPE_(CopyData) *data =
      (struct VALUE_TYPE_(CopyData) *)FUNCTION_(data)(p);
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
    VALUE_TYPE *x_ = x + x_stride * begin;
    VALUE_TYPE *y_ = y + y_stride * begin;
    VALUE_TYPE_(Copy)(batch_size, x_, x_stride, y_, y_stride);
  }
}

#endif

struct FUNCTION *VALUE_TYPE_(Copy_new)(Long size, Long x_stride,
                                       Long y_stride) {
  return VALUE_TYPE_(Copy_new_)(size, x_stride, y_stride, 1);
}

struct FUNCTION *VALUE_TYPE_(Copy_new_)(Long size, Long x_stride, Long y_stride,
                                        int num_threads) {
  if (x_stride == 1 && y_stride == 1) {
#if USE_OPENMP
    num_threads = NS(Threading_GetNumThreads)(size, 1, num_threads);
    FUNCTION_(fn)
    evaluate = (num_threads > 1 && size > 1) ? VALUE_TYPE_(Copy_1_evaluate_omp)
                                             : VALUE_TYPE_(Copy_1_evaluate);
#else
    FUNCTION_(fn) evaluate = VALUE_TYPE_(Copy_1_evaluate);
#endif
    return FUNCTION_(new)(evaluate, jomfftDestructPointer,
                          VALUE_TYPE_(Copy_1Data_new)(size, num_threads));
  } else {
#if USE_OPENMP
    num_threads = NS(Threading_GetNumThreads)(size, 1, num_threads);
    FUNCTION_(fn)
    evaluate = (num_threads > 1 && size > 1) ? VALUE_TYPE_(Copy_evaluate_omp)
                                             : VALUE_TYPE_(Copy_evaluate);
#else
    FUNCTION_(fn) evaluate = VALUE_TYPE_(Copy_evaluate);
#endif
    return FUNCTION_(new)(
        evaluate, jomfftDestructPointer,
        VALUE_TYPE_(CopyData_new)(size, x_stride, y_stride, num_threads));
  }
}

struct VALUE_TYPE_(Copy2D_1Data) {
  Long size_0;
  Long size_1;
  Long x_stride;
  Long y_stride;
};

static struct VALUE_TYPE_(Copy2D_1Data) *VALUE_TYPE_(Copy2D_1Data_new)(
    Long size_0, Long size_1, Long x_stride, Long y_stride) {
  struct VALUE_TYPE_(Copy2D_1Data) *out = NEW(struct VALUE_TYPE_(Copy2D_1Data));
  *out = (struct VALUE_TYPE_(Copy2D_1Data)){.size_0 = size_0,
                                            .size_1 = size_1,
                                            .x_stride = x_stride,
                                            .y_stride = y_stride};
  return out;
}

static void VALUE_TYPE_(Copy2D_1_evaluate)(struct FUNCTION *p, VALUE_TYPE *x,
                                           VALUE_TYPE *y) {
  struct VALUE_TYPE_(Copy2D_1Data) *data =
      (struct VALUE_TYPE_(Copy2D_1Data) *)FUNCTION_(data)(p);
  assert(data);
  VALUE_TYPE_(Copy2D_1)
  (data->size_0, data->size_1, x, data->x_stride, y, data->y_stride);
}

static void VALUE_TYPE_(CopyTranspose_F_evaluate)(struct FUNCTION *p,
                                                  VALUE_TYPE *x,
                                                  VALUE_TYPE *y) {
  struct VALUE_TYPE_(Copy2D_1Data) *data =
      (struct VALUE_TYPE_(Copy2D_1Data) *)FUNCTION_(data)(p);
  assert(data);
  VALUE_TYPE_(CopyTranspose_F)
  (data->size_0, data->size_1, x, data->x_stride, y, data->y_stride);
}

static void VALUE_TYPE_(CopyTranspose_B_evaluate)(struct FUNCTION *p,
                                                  VALUE_TYPE *x,
                                                  VALUE_TYPE *y) {
  struct VALUE_TYPE_(Copy2D_1Data) *data =
      (struct VALUE_TYPE_(Copy2D_1Data) *)FUNCTION_(data)(p);
  assert(data);
  VALUE_TYPE_(CopyTranspose_B)
  (data->size_0, data->size_1, x, data->x_stride, y, data->y_stride);
}

struct VALUE_TYPE_(Copy2DData) {
  Long size_0;
  Long size_1;
  Long x_stride_0;
  Long x_stride_1;
  Long y_stride_0;
  Long y_stride_1;
};

static struct VALUE_TYPE_(Copy2DData) *VALUE_TYPE_(Copy2DData_new)(
    Long size_0, Long size_1, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1) {
  struct VALUE_TYPE_(Copy2DData) *out = NEW(struct VALUE_TYPE_(Copy2DData));
  *out = (struct VALUE_TYPE_(Copy2DData)){.size_0 = size_0,
                                          .size_1 = size_1,
                                          .x_stride_0 = x_stride_0,
                                          .x_stride_1 = x_stride_1,
                                          .y_stride_0 = y_stride_0,
                                          .y_stride_1 = y_stride_1};
  return out;
}

static void VALUE_TYPE_(Copy2D_evaluate)(struct FUNCTION *p, VALUE_TYPE *x,
                                         VALUE_TYPE *y) {
  struct VALUE_TYPE_(Copy2DData) *data =
      (struct VALUE_TYPE_(Copy2DData) *)FUNCTION_(data)(p);
  assert(data);
  VALUE_TYPE_(Copy2D)
  (data->size_0, data->size_1, x, data->x_stride_0, data->x_stride_1, y,
   data->y_stride_0, data->y_stride_1);
}

struct FUNCTION *VALUE_TYPE_(Copy2D_new)(Long size_0, Long size_1,
                                         Long x_stride_0, Long x_stride_1,
                                         Long y_stride_0, Long y_stride_1) {
  if (size_0 == 1) {
    return VALUE_TYPE_(Copy_new)(size_1, x_stride_1, y_stride_1);
  } else if (size_1 == 1) {
    return VALUE_TYPE_(Copy_new)(size_0, x_stride_0, y_stride_0);
  } else if (x_stride_1 == 1 && y_stride_1 == 1) {
    return FUNCTION_(new)(
        VALUE_TYPE_(Copy2D_1_evaluate), jomfftDestructPointer,
        VALUE_TYPE_(Copy2D_1Data_new)(size_0, size_1, x_stride_0, y_stride_0));
  } else if (x_stride_1 == 1 && y_stride_0 == 1) {
    return FUNCTION_(new)(
        VALUE_TYPE_(CopyTranspose_F_evaluate), jomfftDestructPointer,
        VALUE_TYPE_(Copy2D_1Data_new)(size_0, size_1, x_stride_0, y_stride_1));
  } else if (x_stride_0 == 1 && y_stride_1 == 1) {
    return FUNCTION_(new)(
        VALUE_TYPE_(CopyTranspose_B_evaluate), jomfftDestructPointer,
        VALUE_TYPE_(Copy2D_1Data_new)(size_0, size_1, x_stride_1, y_stride_0));
  } else {
    return FUNCTION_(new)(VALUE_TYPE_(Copy2D_evaluate), jomfftDestructPointer,
                          VALUE_TYPE_(Copy2DData_new)(size_0, size_1,
                                                      x_stride_0, x_stride_1,
                                                      y_stride_0, y_stride_1));
  }
}
