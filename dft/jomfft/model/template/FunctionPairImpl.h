struct FUNCTION_(PairData) {
  struct FUNCTION *first;
  struct FUNCTION *second;
};

static struct FUNCTION_(PairData) *FUNCTION_(PairData_new)(
    struct FUNCTION *first, struct FUNCTION *second) {
  struct FUNCTION_(PairData) *out = NEW(struct FUNCTION_(PairData));
  *out = (struct FUNCTION_(PairData)){.first = first, .second = second};
  return out;
}

static void FUNCTION_(PairData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(PairData) *o = (struct FUNCTION_(PairData) *)p;
    if (o->first) {
      FUNCTION_(_delete)(o->first);
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
  struct FUNCTION_(PairData) *data =
      (struct FUNCTION_(PairData) *)FUNCTION_(_data)(p);
  assert(data);
  FUNCTION_(_evaluate)(data->first, x, x);
  FUNCTION_(_evaluate)(data->second, x, y);
}

static void FUNCTION_(Pair_1_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                       OUTPUT_TYPE *y) {
  struct FUNCTION_(PairData) *data =
      (struct FUNCTION_(PairData) *)FUNCTION_(_data)(p);
  assert(data);
  FUNCTION_(_evaluate)(data->first, x, y);
  FUNCTION_(_evaluate)(data->second, y, y);
}

struct FUNCTION *FUNCTION_(Pair_0_new)(struct FUNCTION *first,
                                       struct FUNCTION *second) {
  return FUNCTION_(_new)(FUNCTION_(Pair_0_evaluate),
                         FUNCTION_(PairData_destruct),
                         FUNCTION_(PairData_new)(first, second));
}

struct FUNCTION *FUNCTION_(Pair_1_new)(struct FUNCTION *first,
                                       struct FUNCTION *second) {
  return FUNCTION_(_new)(FUNCTION_(Pair_1_evaluate),
                         FUNCTION_(PairData_destruct),
                         FUNCTION_(PairData_new)(first, second));
}
