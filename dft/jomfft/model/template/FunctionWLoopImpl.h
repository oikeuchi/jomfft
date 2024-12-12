struct FUNCTIONW_(LoopData) {
  struct FUNCTIONW *func;
  Long loop_size;
  Long x_stride;
  Long y_stride;
};

static struct FUNCTIONW_(LoopData) *FUNCTIONW_(LoopData_new)(
    struct FUNCTIONW *func, Long loop_size, Long x_stride, Long y_stride) {
  struct FUNCTIONW_(LoopData) *out = NEW(struct FUNCTIONW_(LoopData));
  *out = (struct FUNCTIONW_(LoopData)){.func = func,
                                       .loop_size = loop_size,
                                       .x_stride = x_stride,
                                       .y_stride = y_stride};
  return out;
}

static void FUNCTIONW_(LoopData_destruct)(void *p) {
  if (p) {
    struct FUNCTIONW_(LoopData) *o = (struct FUNCTIONW_(LoopData) *)p;
    if (o->func) {
      FUNCTIONW_(_delete)(o->func);
      o->func = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTIONW_(Loop_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                      OUTPUT_TYPE *y, void *work) {
  struct FUNCTIONW_(LoopData) *data =
      (struct FUNCTIONW_(LoopData) *)FUNCTIONW_(_data)(p);
  assert(data);
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  for (Long i = 0; i < loop_size; ++i) {
    FUNCTIONW_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i], work);
  }
}

static Long FUNCTIONW_(Loop_size)(struct FUNCTIONW *p) {
  struct FUNCTIONW_(LoopData) *data =
      (struct FUNCTIONW_(LoopData) *)FUNCTIONW_(_data)(p);
  assert(data);
  return FUNCTIONW_(_size)(data->func);
}

struct FUNCTIONW *FUNCTIONW_(Loop_new)(struct FUNCTIONW *func, Long loop_size,
                                       Long x_stride, Long y_stride) {
  assert(loop_size >= 1);
  if (loop_size == 1) {
    return func;
  } else {
    return FUNCTIONW_(_new)(
        FUNCTIONW_(Loop_evaluate), FUNCTIONW_(Loop_size),
        FUNCTIONW_(LoopData_destruct),
        FUNCTIONW_(LoopData_new)(func, loop_size, x_stride, y_stride));
  }
}

struct FUNCTIONW_(LoopRData) {
  struct FUNCTIONW *func;
  struct FUNCTIONW *residual_func;
  Long loop_size;
  Long x_stride;
  Long y_stride;
};

static struct FUNCTIONW_(LoopRData) *FUNCTIONW_(LoopRData_new)(
    struct FUNCTIONW *func, struct FUNCTIONW *residual_func, Long loop_size,
    Long x_stride, Long y_stride) {
  struct FUNCTIONW_(LoopRData) *out = NEW(struct FUNCTIONW_(LoopRData));
  *out = (struct FUNCTIONW_(LoopRData)){.func = func,
                                        .residual_func = residual_func,
                                        .loop_size = loop_size,
                                        .x_stride = x_stride,
                                        .y_stride = y_stride};
  return out;
}

static void FUNCTIONW_(LoopRData_destruct)(void *p) {
  if (p) {
    struct FUNCTIONW_(LoopRData) *o = (struct FUNCTIONW_(LoopRData) *)p;
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

static void FUNCTIONW_(LoopR_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                       OUTPUT_TYPE *y, void *work) {
  struct FUNCTIONW_(LoopRData) *data =
      (struct FUNCTIONW_(LoopRData) *)FUNCTIONW_(_data)(p);
  assert(data);
  const Long loop_size = data->loop_size;
  const Long x_stride = data->x_stride;
  const Long y_stride = data->y_stride;
  Long i = 0;
  for (; i < loop_size; ++i) {
    FUNCTIONW_(_evaluate)(data->func, &x[x_stride * i], &y[y_stride * i], work);
  }
  FUNCTIONW_(_evaluate)(data->residual_func, &x[x_stride * i], &y[y_stride * i],
                        work);
}

static Long FUNCTIONW_(LoopR_size)(struct FUNCTIONW *p) {
  struct FUNCTIONW_(LoopRData) *data =
      (struct FUNCTIONW_(LoopRData) *)FUNCTIONW_(_data)(p);
  assert(data);
  return MAX(FUNCTIONW_(_size)(data->func),
             FUNCTIONW_(_size)(data->residual_func));
}

struct FUNCTIONW *FUNCTIONW_(LoopR_new)(struct FUNCTIONW *func,
                                        struct FUNCTIONW *residual_func,
                                        Long loop_size, Long x_stride,
                                        Long y_stride) {
  return FUNCTIONW_(_new)(FUNCTIONW_(LoopR_evaluate), FUNCTIONW_(LoopR_size),
                          FUNCTIONW_(LoopRData_destruct),
                          FUNCTIONW_(LoopRData_new)(func, residual_func,
                                                    loop_size, x_stride,
                                                    y_stride));
}
