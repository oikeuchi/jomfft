struct FUNCTION_(TripleWData) {
  struct FUNCTION_0 *func_0;
  struct FUNCTION *func_1;
  struct FUNCTION_1 *func_2;
  Long size;
};

static struct FUNCTION_(TripleWData) *FUNCTION_(TripleWData_new)(
    struct FUNCTION_0 *func_0, struct FUNCTION *func_1,
    struct FUNCTION_1 *func_2, Long size) {
  struct FUNCTION_(TripleWData) *out = NEW(struct FUNCTION_(TripleWData));
  *out = (struct FUNCTION_(TripleWData)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2, .size = size};
  return out;
}

static void FUNCTION_(TripleWData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(TripleWData) *o = (struct FUNCTION_(TripleWData) *)p;
    if (o->func_0) {
      FUNCTION_0_(_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      FUNCTION_(_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      FUNCTION_1_(_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(TripleW_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                        OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(TripleWData) *data =
      (struct FUNCTION_(TripleWData) *)FUNCTIONW_(_data)(p);
  assert(data);
  INPUT_TYPE *b_x = (INPUT_TYPE *)work;
  OUTPUT_TYPE *b_y = (OUTPUT_TYPE *)work;
  FUNCTION_0_(_evaluate)(data->func_0, x, b_x);
  FUNCTION_(_evaluate)(data->func_1, b_x, b_y);
  FUNCTION_1_(_evaluate)(data->func_2, b_y, y);
}

static Long FUNCTION_(TripleW_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(TripleWData) *data =
      (struct FUNCTION_(TripleWData) *)FUNCTIONW_(_data)(p);
  assert(data);
  return data->size;
}

struct FUNCTIONW *FUNCTION_(TripleW_new)(struct FUNCTION_0 *func_0,
                                         struct FUNCTION *func_1,
                                         struct FUNCTION_1 *func_2, Long size) {
  return FUNCTIONW_(_new)(
      FUNCTION_(TripleW_evaluate), FUNCTION_(TripleW_size),
      FUNCTION_(TripleWData_destruct),
      FUNCTION_(TripleWData_new)(func_0, func_1, func_2, size));
}

struct FUNCTION_(TripleDW_1Data) {
  struct FUNCTION_0 *func_0;
  struct FUNCTION *func_1;
  struct FUNCTION_1 *func_2;
  Long x_size;
  Long y_size;
};

static struct FUNCTION_(TripleDW_1Data) *FUNCTION_(TripleDW_1Data_new)(
    struct FUNCTION_0 *func_0, struct FUNCTION *func_1,
    struct FUNCTION_1 *func_2, Long x_size, Long y_size) {
  struct FUNCTION_(TripleDW_1Data) *out = NEW(struct FUNCTION_(TripleDW_1Data));
  *out = (struct FUNCTION_(TripleDW_1Data)){.func_0 = func_0,
                                            .func_1 = func_1,
                                            .func_2 = func_2,
                                            .x_size = x_size,
                                            .y_size = y_size};
  return out;
}

static void FUNCTION_(TripleDW_1Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(TripleDW_1Data) *o = (struct FUNCTION_(TripleDW_1Data) *)p;
    if (o->func_0) {
      FUNCTION_0_(_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      FUNCTION_(_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      FUNCTION_1_(_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(TripleDW_1_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                           OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(TripleDW_1Data) *data =
      (struct FUNCTION_(TripleDW_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  INPUT_TYPE *b_x = (INPUT_TYPE *)work;
  OUTPUT_TYPE *b_y = (OUTPUT_TYPE *)((char *)work + data->x_size);
  FUNCTION_0_(_evaluate)(data->func_0, x, b_x);
  FUNCTION_(_evaluate)(data->func_1, b_x, b_y);
  FUNCTION_1_(_evaluate)(data->func_2, b_y, y);
}

static Long FUNCTION_(TripleDW_1_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(TripleDW_1Data) *data =
      (struct FUNCTION_(TripleDW_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  return data->x_size + data->y_size;
}

struct FUNCTIONW *FUNCTION_(TripleDW_1_new)(struct FUNCTION_0 *func_0,
                                            struct FUNCTION *func_1,
                                            struct FUNCTION_1 *func_2,
                                            Long x_size, Long y_size) {
  return FUNCTIONW_(_new)(
      FUNCTION_(TripleDW_1_evaluate), FUNCTION_(TripleDW_1_size),
      FUNCTION_(TripleDW_1Data_destruct),
      FUNCTION_(TripleDW_1Data_new)(func_0, func_1, func_2, x_size, y_size));
}

struct FUNCTION_(TripleWB_1Data) {
  struct FUNCTION_0 *func_0;
  struct FUNCTIONW *func_1;
  struct FUNCTION_1 *func_2;
};

static struct FUNCTION_(TripleWB_1Data) *FUNCTION_(TripleWB_1Data_new)(
    struct FUNCTION_0 *func_0, struct FUNCTIONW *func_1,
    struct FUNCTION_1 *func_2) {
  struct FUNCTION_(TripleWB_1Data) *out = NEW(struct FUNCTION_(TripleWB_1Data));
  *out = (struct FUNCTION_(TripleWB_1Data)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2};
  return out;
}

static void FUNCTION_(TripleWB_1Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(TripleWB_1Data) *o = (struct FUNCTION_(TripleWB_1Data) *)p;
    if (o->func_0) {
      FUNCTION_0_(_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      FUNCTIONW_(_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      FUNCTION_1_(_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(TripleWB_1_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                           OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(TripleWB_1Data) *data =
      (struct FUNCTION_(TripleWB_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  INPUT_TYPE *b_x = (INPUT_TYPE *)work;
  OUTPUT_TYPE *b_y = (OUTPUT_TYPE *)work;
  FUNCTION_0_(_evaluate)(data->func_0, x, b_x);
  FUNCTIONW_(_evaluate)(data->func_1, b_x, b_y, work);
  FUNCTION_1_(_evaluate)(data->func_2, b_y, y);
}

static Long FUNCTION_(TripleWB_1_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(TripleWB_1Data) *data =
      (struct FUNCTION_(TripleWB_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  return FUNCTIONW_(_size)(data->func_1);
}

struct FUNCTIONW *FUNCTION_(TripleWB_1_new)(struct FUNCTION_0 *func_0,
                                            struct FUNCTIONW *func_1,
                                            struct FUNCTION_1 *func_2) {
  return FUNCTIONW_(_new)(
      FUNCTION_(TripleWB_1_evaluate), FUNCTION_(TripleWB_1_size),
      FUNCTION_(TripleWB_1Data_destruct),
      FUNCTION_(TripleWB_1Data_new)(func_0, func_1, func_2));
}
