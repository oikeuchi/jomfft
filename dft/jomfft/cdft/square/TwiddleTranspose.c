#include "TwiddleTranspose.h"

#include "../../data/LargeTwiddleMatrix.h"
#include "BlockSize.h"
#include "impl/TwiddleTranspose_B.h"
#include "impl/TwiddleTranspose_F.h"

#define NS_(n) NS(CDftSquare_##n)

typedef void (*NS_(TwiddleTranspose_fn))(Long, const Complex *, Complex *, int);

struct NS_(TwiddleTransposeData) {
  NS_(TwiddleTranspose_fn) evaluate;
  Long size;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(TwiddleTransposeData) *
CDftSquare_TwiddleTransposeData_new(NS_(TwiddleTranspose_fn) evaluate,
                                    Long size, const Complex *w,
                                    int num_threads) {
  assert(w);
  struct NS_(TwiddleTransposeData) *out = NEW(struct NS_(TwiddleTransposeData));
  *out = (struct NS_(TwiddleTransposeData)){
      .evaluate = evaluate, .size = size, .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void CDftSquare_TwiddleTranspose_evaluate(struct NS(ComplexFunction) *p,
                                                 Complex *x, Complex *y) {
  assert(x == y);
  (void)y;
  const struct NS_(TwiddleTransposeData) *data =
      (const struct NS_(TwiddleTransposeData) *)NS(ComplexFunction_data)(p);
  assert(data);
#if USE_OPENMP
  data->evaluate(data->size, data->w, x, data->num_threads);
#else
  data->evaluate(data->size, data->w, x, 1);
#endif
}

struct NS(ComplexFunction) *NS_(TwiddleTranspose_new)(
    struct NS(DataFactory) *data, bool backward, Long size, int num_threads) {
  NS_(TwiddleTranspose_fn) evaluate =
      backward ? NS_(TwiddleTranspose_B) : NS_(TwiddleTranspose_F);
  return NS(ComplexFunction_new)(CDftSquare_TwiddleTranspose_evaluate,
                                 jomfftDestructPointer,
                                 CDftSquare_TwiddleTransposeData_new(
                                     evaluate, size,
                                     NS(DataFactory_largeTwiddleMatrix)(
                                         data, size, size * size, BLOCK_SIZE),
                                     num_threads));
}
