struct FUNCTION_(PairW_0Data) {
  struct FUNCTION_0 *first;
  struct FUNCTION *second;
  Long size;
};

static struct FUNCTION_(PairW_0Data) *FUNCTION_(PairW_0Data_new)(
    struct FUNCTION_0 *first, struct FUNCTION *second, Long size) {
  struct FUNCTION_(PairW_0Data) *out = NEW(struct FUNCTION_(PairW_0Data));
  *out = (struct FUNCTION_(PairW_0Data)){
      .first = first, .second = second, .size = size};
  return out;
}

static void FUNCTION_(PairW_0Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(PairW_0Data) *o = (struct FUNCTION_(PairW_0Data) *)p;
    if (o->first) {
      FUNCTION_0_(_delete)(o->first);
      o->first = NULL;
    }
    if (o->second) {
      FUNCTION_(_delete)(o->second);
      o->second = NULL;
    }
    DELETE(o);
  }
}

struct FUNCTION_(PairW_1Data) {
  struct FUNCTION *first;
  struct FUNCTION_1 *second;
  Long size;
};

static struct FUNCTION_(PairW_1Data) *FUNCTION_(PairW_1Data_new)(
    struct FUNCTION *first, struct FUNCTION_1 *second, Long size) {
  struct FUNCTION_(PairW_1Data) *out = NEW(struct FUNCTION_(PairW_1Data));
  *out = (struct FUNCTION_(PairW_1Data)){
      .first = first, .second = second, .size = size};
  return out;
}

static void FUNCTION_(PairW_1Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(PairW_1Data) *o = (struct FUNCTION_(PairW_1Data) *)p;
    if (o->first) {
      FUNCTION_(_delete)(o->first);
      o->first = NULL;
    }
    if (o->second) {
      FUNCTION_1_(_delete)(o->second);
      o->second = NULL;
    }
    DELETE(o);
  }
}

static Long FUNCTION_(PairW_0_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(PairW_0Data) *data =
      (struct FUNCTION_(PairW_0Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  return data->size;
}

static Long FUNCTION_(PairW_1_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(PairW_1Data) *data =
      (struct FUNCTION_(PairW_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  return data->size;
}

static void FUNCTION_(PairW_0_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                        OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(PairW_0Data) *data =
      (struct FUNCTION_(PairW_0Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  INPUT_TYPE *b = (INPUT_TYPE *)work;
  FUNCTION_0_(_evaluate)(data->first, x, b);
  FUNCTION_(_evaluate)(data->second, b, y);
}

struct FUNCTIONW *FUNCTION_(PairW_0_new)(struct FUNCTION_0 *first,
                                         struct FUNCTION *second, Long size) {
  return FUNCTIONW_(_new)(FUNCTION_(PairW_0_evaluate), FUNCTION_(PairW_0_size),
                          FUNCTION_(PairW_0Data_destruct),
                          FUNCTION_(PairW_0Data_new)(first, second, size));
}

static void FUNCTION_(PairW_1_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                        OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(PairW_1Data) *data =
      (struct FUNCTION_(PairW_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  OUTPUT_TYPE *b = (OUTPUT_TYPE *)work;
  FUNCTION_(_evaluate)(data->first, x, b);
  FUNCTION_1_(_evaluate)(data->second, b, y);
}

struct FUNCTIONW *FUNCTION_(PairW_1_new)(struct FUNCTION *first,
                                         struct FUNCTION_1 *second, Long size) {
  return FUNCTIONW_(_new)(FUNCTION_(PairW_1_evaluate), FUNCTION_(PairW_1_size),
                          FUNCTION_(PairW_1Data_destruct),
                          FUNCTION_(PairW_1Data_new)(first, second, size));
}

struct FUNCTION_(PairWB_0Data) {
  struct FUNCTION_0 *first;
  struct FUNCTIONW *second;
};

static struct FUNCTION_(PairWB_0Data) *FUNCTION_(PairWB_0Data_new)(
    struct FUNCTION_0 *first, struct FUNCTIONW *second) {
  struct FUNCTION_(PairWB_0Data) *out = NEW(struct FUNCTION_(PairWB_0Data));
  *out = (struct FUNCTION_(PairWB_0Data)){.first = first, .second = second};
  return out;
}

static void FUNCTION_(PairWB_0Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(PairWB_0Data) *o = (struct FUNCTION_(PairWB_0Data) *)p;
    if (o->first) {
      FUNCTION_0_(_delete)(o->first);
      o->first = NULL;
    }
    if (o->second) {
      FUNCTIONW_(_delete)(o->second);
      o->second = NULL;
    }
    DELETE(o);
  }
}

struct FUNCTION_(PairWB_1Data) {
  struct FUNCTIONW *first;
  struct FUNCTION_1 *second;
};

static struct FUNCTION_(PairWB_1Data) *FUNCTION_(PairWB_1Data_new)(
    struct FUNCTIONW *first, struct FUNCTION_1 *second) {
  struct FUNCTION_(PairWB_1Data) *out = NEW(struct FUNCTION_(PairWB_1Data));
  *out = (struct FUNCTION_(PairWB_1Data)){.first = first, .second = second};
  return out;
}

static void FUNCTION_(PairWB_1Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(PairWB_1Data) *o = (struct FUNCTION_(PairWB_1Data) *)p;
    if (o->first) {
      FUNCTIONW_(_delete)(o->first);
      o->first = NULL;
    }
    if (o->second) {
      FUNCTION_1_(_delete)(o->second);
      o->second = NULL;
    }
    DELETE(o);
  }
}

static Long FUNCTION_(PairWB_0_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(PairWB_0Data) *data =
      (struct FUNCTION_(PairWB_0Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  return FUNCTIONW_(_size)(data->second);
}

static Long FUNCTION_(PairWB_1_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(PairWB_1Data) *data =
      (struct FUNCTION_(PairWB_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  return FUNCTIONW_(_size)(data->first);
}

static void FUNCTION_(PairWB_0_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                         OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(PairWB_0Data) *data =
      (struct FUNCTION_(PairWB_0Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  INPUT_TYPE *b = (INPUT_TYPE *)work;
  FUNCTION_0_(_evaluate)(data->first, x, b);
  FUNCTIONW_(_evaluate)(data->second, b, y, work);
}

struct FUNCTIONW *FUNCTION_(PairWB_0_new)(struct FUNCTION_0 *first,
                                          struct FUNCTIONW *second) {
  return FUNCTIONW_(_new)(FUNCTION_(PairWB_0_evaluate),
                          FUNCTION_(PairWB_0_size),
                          FUNCTION_(PairWB_0Data_destruct),
                          FUNCTION_(PairWB_0Data_new)(first, second));
}

static void FUNCTION_(PairWB_1_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                         OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(PairWB_1Data) *data =
      (struct FUNCTION_(PairWB_1Data) *)FUNCTIONW_(_data)(p);
  assert(data);
  OUTPUT_TYPE *b = (OUTPUT_TYPE *)work;
  FUNCTIONW_(_evaluate)(data->first, x, b, work);
  FUNCTION_1_(_evaluate)(data->second, b, y);
}

struct FUNCTIONW *FUNCTION_(PairWB_1_new)(struct FUNCTIONW *first,
                                          struct FUNCTION_1 *second) {
  return FUNCTIONW_(_new)(FUNCTION_(PairWB_1_evaluate),
                          FUNCTION_(PairWB_1_size),
                          FUNCTION_(PairWB_1Data_destruct),
                          FUNCTION_(PairWB_1Data_new)(first, second));
}
