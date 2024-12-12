struct FUNCTION_(PairWData) {
  struct FUNCTION *first;
  struct FUNCTION *second;
  Long size;
};

static struct FUNCTION_(PairWData) *FUNCTION_(PairWData_new)(
    struct FUNCTION *first, struct FUNCTION *second, Long size) {
  struct FUNCTION_(PairWData) *out = NEW(struct FUNCTION_(PairWData));
  *out = (struct FUNCTION_(PairWData)){
      .first = first, .second = second, .size = size};
  return out;
}

static void FUNCTION_(PairWData_destruct)(void *p) {
  if (p) {
    struct FUNCTION_(PairWData) *o = (struct FUNCTION_(PairWData) *)p;
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

static void FUNCTION_(PairW_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x,
                                      OUTPUT_TYPE *y, void *work) {
  struct FUNCTION_(PairWData) *data =
      (struct FUNCTION_(PairWData) *)FUNCTIONW_(_data)(p);
  assert(data);
  OUTPUT_TYPE *b = (OUTPUT_TYPE *)work;
  FUNCTION_(_evaluate)(data->first, x, b);
  FUNCTION_(_evaluate)(data->second, b, y);
}

static Long FUNCTION_(PairW_size)(struct FUNCTIONW *p) {
  struct FUNCTION_(PairWData) *data =
      (struct FUNCTION_(PairWData) *)FUNCTIONW_(_data)(p);
  assert(data);
  return data->size;
}

struct FUNCTIONW *FUNCTION_(PairW_new)(struct FUNCTION *first,
                                       struct FUNCTION *second, Long size) {
  return FUNCTIONW_(_new)(FUNCTION_(PairW_evaluate), FUNCTION_(PairW_size),
                          FUNCTION_(PairWData_destruct),
                          FUNCTION_(PairWData_new)(first, second, size));
}
