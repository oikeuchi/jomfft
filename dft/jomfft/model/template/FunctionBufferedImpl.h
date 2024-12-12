#include <jomfft/VLA.h>

static void FUNCTION_(Buffered_destruct)(void *p) {
  if (p) {
    FUNCTIONW_(_delete)((struct FUNCTIONW *)p);
  }
}

static void FUNCTION_(BufferedHeap_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                             OUTPUT_TYPE *y) {
  struct FUNCTIONW *func = (struct FUNCTIONW *)FUNCTION_(_data)(p);
  assert(func);
  char *work = ARRAY_NEW(char, FUNCTIONW_(_size)(func));
  FUNCTIONW_(_evaluate)(func, x, y, work);
  ARRAY_DELETE(work);
}

static void FUNCTION_(BufferedStack_evaluate)(struct FUNCTION *p, INPUT_TYPE *x,
                                              OUTPUT_TYPE *y) {
  struct FUNCTIONW *func = (struct FUNCTIONW *)FUNCTION_(_data)(p);
  assert(func);
  const Long size = FUNCTIONW_(_size)(func);
  DECLARE_ALIGNED_VLA(char, work, size);
  FUNCTIONW_(_evaluate)(func, x, y, work);
}

struct FUNCTION *FUNCTION_(Buffered_new)(struct FUNCTIONW *func) {
  const Long size = FUNCTIONW_(_size)(func);
  FUNCTION_(_fn) evaluate;
  if (size > STACK_BUFFER_MAX_SIZE) {
    evaluate = FUNCTION_(BufferedHeap_evaluate);
  } else {
    evaluate = FUNCTION_(BufferedStack_evaluate);
  }
  return FUNCTION_(_new)(evaluate, FUNCTION_(Buffered_destruct), func);
}
