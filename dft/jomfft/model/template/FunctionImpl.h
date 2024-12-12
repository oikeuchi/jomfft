
struct FUNCTION {
  FUNCTION_(_fn) evaluate;
  jomfftDestruct_fn destruct;
  void *data;
};

struct FUNCTION *FUNCTION_(_new)(FUNCTION_(_fn) evaluate,
                                 jomfftDestruct_fn destruct, void *data) {
  assert(evaluate);
  assert(data == NULL || destruct);
  struct FUNCTION *out = NEW(struct FUNCTION);
  *out = (struct FUNCTION){
      .evaluate = evaluate, .destruct = destruct, .data = data};
  return out;
}

void FUNCTION_(_delete)(struct FUNCTION *p) {
  if (p) {
    if (p->destruct && p->data) {
      p->destruct(p->data);
    }
    p->data = NULL;
    DELETE(p);
  }
}

void *FUNCTION_(_data)(struct FUNCTION *p) {
  assert(p);
  return p->data;
}

void FUNCTION_(_evaluate)(struct FUNCTION *p, INPUT_TYPE *x, OUTPUT_TYPE *y) {
  assert(p);
  assert(p->evaluate);
  p->evaluate(p, x, y);
}
