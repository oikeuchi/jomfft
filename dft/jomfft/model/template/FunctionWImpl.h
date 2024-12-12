
struct FUNCTIONW {
  FUNCTIONW_(_fn) evaluate;
  FUNCTIONW_(_Size_fn) size;
  jomfftDestruct_fn destruct;
  void *data;
};

struct FUNCTIONW *FUNCTIONW_(_new)(FUNCTIONW_(_fn) evaluate,
                                   FUNCTIONW_(_Size_fn) size,
                                   jomfftDestruct_fn destruct, void *data) {
  assert(evaluate && size);
  assert(data == NULL || destruct);
  struct FUNCTIONW *out = NEW(struct FUNCTIONW);
  *out = (struct FUNCTIONW){
      .evaluate = evaluate, .size = size, .destruct = destruct, .data = data};
  return out;
}

void FUNCTIONW_(_delete)(struct FUNCTIONW *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
    }
    p->data = NULL;
    DELETE(p);
  }
}

void *FUNCTIONW_(_data)(struct FUNCTIONW *p) {
  assert(p);
  return p->data;
}

Long FUNCTIONW_(_size)(struct FUNCTIONW *p) {
  assert(p);
  return p->size(p);
}

void FUNCTIONW_(_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x, OUTPUT_TYPE *y,
                           void *work) {
  assert(p);
  assert(p->evaluate);
  p->evaluate(p, x, y, work);
}
