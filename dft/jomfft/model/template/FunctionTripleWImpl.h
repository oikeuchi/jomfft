struct FUNCTION_(TripleWData) {
  struct FUNCTION *func_0;
  struct FUNCTION *func_1;
  struct FUNCTION *func_2;
  Long size;
};

static struct FUNCTION_(TripleWData) *FUNCTION_(TripleWData_new)(
    struct FUNCTION *func_0, struct FUNCTION *func_1, struct FUNCTION *func_2,
    Long size) {
  struct FUNCTION_(TripleWData) *out = NEW(struct FUNCTION_(TripleWData));
  *out = (struct FUNCTION_(TripleWData)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2, .size = size};
  return out;
}

static void FUNCTION_(TripleWData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(TripleWData) *o = (struct FUNCTION_(TripleWData) *)p;
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

static void FUNCTION_(TripleW_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                        OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(TripleWData) *data =
      (struct FUNCTION_(TripleWData) *)FUNCTIONW_(_data)(p);
  assert(data);
  OUTPUT_TYPE *b = (OUTPUT_TYPE *)work;
  FUNCTION_(_evaluate)(data->func_0, x, b);
  FUNCTION_(_evaluate)(data->func_1, b, b);
  FUNCTION_(_evaluate)(data->func_2, b, y);
}

static Long FUNCTION_(TripleW_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(TripleWData) *data =
      (struct FUNCTION_(TripleWData) *)FUNCTIONW_(_data)(p);
  assert(data);
  return data->size;
}

struct FUNCTIONW *FUNCTION_(TripleW_new)(struct FUNCTION *func_0,
                                         struct FUNCTION *func_1,
                                         struct FUNCTION *func_2, Long size) {
  return FUNCTIONW_(_new)(
      FUNCTION_(TripleW_evaluate), FUNCTION_(TripleW_size),
      FUNCTION_(TripleWData_destruct),
      FUNCTION_(TripleWData_new)(func_0, func_1, func_2, size));
}
