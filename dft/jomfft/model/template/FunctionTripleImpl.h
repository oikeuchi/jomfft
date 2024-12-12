struct FUNCTION_(TripleData) {
  struct FUNCTION *func_0;
  struct FUNCTION *func_1;
  struct FUNCTION *func_2;
};

static struct FUNCTION_(TripleData) *FUNCTION_(TripleData_new)(
    struct FUNCTION *func_0, struct FUNCTION *func_1, struct FUNCTION *func_2) {
  struct FUNCTION_(TripleData) *out = NEW(struct FUNCTION_(TripleData));
  *out = (struct FUNCTION_(TripleData)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2};
  return out;
}

static void FUNCTION_(TripleData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(TripleData) *o = (struct FUNCTION_(TripleData) *)p;
    if (o->func_0) {
      FUNCTION_(_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      FUNCTION_(_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      FUNCTION_(_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static void FUNCTION_(Triple_0_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                         OUTPUT_TYPE *y) {
  struct FUNCTION_(TripleData) *data =
      (struct FUNCTION_(TripleData) *)FUNCTION_(_data)(p);
  assert(data);
  FUNCTION_(_evaluate)(data->func_0, x, y);
  FUNCTION_(_evaluate)(data->func_1, y, y);
  FUNCTION_(_evaluate)(data->func_2, y, y);
}

static void FUNCTION_(Triple_2_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                         OUTPUT_TYPE *y) {
  struct FUNCTION_(TripleData) *data =
      (struct FUNCTION_(TripleData) *)FUNCTION_(_data)(p);
  assert(data);
  FUNCTION_(_evaluate)(data->func_0, x, x);
  FUNCTION_(_evaluate)(data->func_1, x, x);
  FUNCTION_(_evaluate)(data->func_2, x, y);
}

struct FUNCTION *FUNCTION_(Triple_0_new)(struct FUNCTION *func_0,
                                         struct FUNCTION *func_1,
                                         struct FUNCTION *func_2) {
  return FUNCTION_(_new)(FUNCTION_(Triple_0_evaluate),
                         FUNCTION_(TripleData_destruct),
                         FUNCTION_(TripleData_new)(func_0, func_1, func_2));
}

struct FUNCTION *FUNCTION_(Triple_2_new)(struct FUNCTION *func_0,
                                         struct FUNCTION *func_1,
                                         struct FUNCTION *func_2) {
  return FUNCTION_(_new)(FUNCTION_(Triple_2_evaluate),
                         FUNCTION_(TripleData_destruct),
                         FUNCTION_(TripleData_new)(func_0, func_1, func_2));
}
