struct FUNCTION_(Pair_0Data) {
  struct FUNCTION_0 *first;
  struct FUNCTION *second;
};

static struct FUNCTION_(Pair_0Data) *FUNCTION_(Pair_0Data_new)(
    struct FUNCTION_0 *first, struct FUNCTION *second) {
  struct FUNCTION_(Pair_0Data) *out = NEW(struct FUNCTION_(Pair_0Data));
  *out = (struct FUNCTION_(Pair_0Data)){.first = first, .second = second};
  return out;
}

static void FUNCTION_(Pair_0Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(Pair_0Data) *o = (struct FUNCTION_(Pair_0Data) *)p;
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

static void FUNCTION_(Pair_0_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                       OUTPUT_TYPE *y) {
  struct FUNCTION_(Pair_0Data) *data =
      (struct FUNCTION_(Pair_0Data) *)FUNCTION_(_data)(p);
  assert(data);
  FUNCTION_0_(_evaluate)(data->first, x, x);
  FUNCTION_(_evaluate)(data->second, x, y);
}

struct FUNCTION *FUNCTION_(Pair_0_new)(struct FUNCTION_0 *first,
                                       struct FUNCTION *second) {
  return FUNCTION_(_new)(FUNCTION_(Pair_0_evaluate),
                         FUNCTION_(Pair_0Data_destruct),
                         FUNCTION_(Pair_0Data_new)(first, second));
}

struct FUNCTION_(Pair_1Data) {
  struct FUNCTION *first;
  struct FUNCTION_1 *second;
};

static struct FUNCTION_(Pair_1Data) *FUNCTION_(Pair_1Data_new)(
    struct FUNCTION *first, struct FUNCTION_1 *second) {
  struct FUNCTION_(Pair_1Data) *out = NEW(struct FUNCTION_(Pair_1Data));
  *out = (struct FUNCTION_(Pair_1Data)){.first = first, .second = second};
  return out;
}

static void FUNCTION_(Pair_1Data_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(Pair_1Data) *o = (struct FUNCTION_(Pair_1Data) *)p;
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

static void FUNCTION_(Pair_1_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                       OUTPUT_TYPE *y) {
  struct FUNCTION_(Pair_1Data) *data =
      (struct FUNCTION_(Pair_1Data) *)FUNCTION_(_data)(p);
  assert(data);
  FUNCTION_(_evaluate)(data->first, x, y);
  FUNCTION_1_(_evaluate)(data->second, y, y);
}

struct FUNCTION *FUNCTION_(Pair_1_new)(struct FUNCTION *first,
                                       struct FUNCTION_1 *second) {
  return FUNCTION_(_new)(FUNCTION_(Pair_1_evaluate),
                         FUNCTION_(Pair_1Data_destruct),
                         FUNCTION_(Pair_1Data_new)(first, second));
}

static void FUNCTION_(Pair_C0_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                        OUTPUT_TYPE *y) {
  struct FUNCTION_(Pair_1Data) *data =
      (struct FUNCTION_(Pair_1Data) *)FUNCTION_(_data)(p);
  assert(data);
  OUTPUT_TYPE *x_ = (OUTPUT_TYPE *)x;
  FUNCTION_(_evaluate)(data->first, x, x_);
  FUNCTION_1_(_evaluate)(data->second, x_, y);
}

static void FUNCTION_(Pair_C1_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                        OUTPUT_TYPE *y) {
  struct FUNCTION_(Pair_0Data) *data =
      (struct FUNCTION_(Pair_0Data) *)FUNCTION_(_data)(p);
  assert(data);
  INPUT_TYPE *y_ = (INPUT_TYPE *)y;
  FUNCTION_0_(_evaluate)(data->first, x, y_);
  FUNCTION_(_evaluate)(data->second, y_, y);
}

struct FUNCTION *FUNCTION_(Pair_C0_new)(struct FUNCTION *first,
                                        struct FUNCTION_1 *second) {
  return FUNCTION_(_new)(FUNCTION_(Pair_C0_evaluate),
                         FUNCTION_(Pair_1Data_destruct),
                         FUNCTION_(Pair_1Data_new)(first, second));
}

struct FUNCTION *FUNCTION_(Pair_C1_new)(struct FUNCTION_0 *first,
                                        struct FUNCTION *second) {
  return FUNCTION_(_new)(FUNCTION_(Pair_C1_evaluate),
                         FUNCTION_(Pair_0Data_destruct),
                         FUNCTION_(Pair_0Data_new)(first, second));
}
