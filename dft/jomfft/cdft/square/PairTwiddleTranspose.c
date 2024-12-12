#include "PairTwiddleTranspose.h"

#include "../../data/LargeTwiddleMatrix.h"
#include "../../data/TwiddleVector.h"
#include "BlockSize.h"
#include "impl/PairTwiddleTranspose_B.h"
#include "impl/PairTwiddleTranspose_F.h"

#define NS_(n) NS(CDftSquare_##n)

typedef void (*NS_(PairTwiddleTranspose_fn))(Long, const Complex *,
                                             const Complex *, Complex *, int);

struct NS_(PairTwiddleTransposeData) {
  NS_(PairTwiddleTranspose_fn) evaluate;
  Long size;
  const Complex *v;
  const Complex *w;
#if USE_OPENMP
  int num_threads;
#endif
};

static struct NS_(PairTwiddleTransposeData) *NS_(PairTwiddleTransposeData_new)(
    NS_(PairTwiddleTranspose_fn) evaluate, Long size, const Complex *v,
    const Complex *w, int num_threads) {
  assert(v);
  assert(w);
  struct NS_(PairTwiddleTransposeData) *out =
      NEW(struct NS_(PairTwiddleTransposeData));
  *out = (struct NS_(PairTwiddleTransposeData)){
      .evaluate = evaluate, .size = size, .v = v, .w = w};
#if USE_OPENMP
  out->num_threads = num_threads;
#else
  (void)num_threads;
#endif
  return out;
}

static void NS_(PairTwiddleTranspose_evaluate)(struct NS(ComplexFunction) *p,
                                               Complex *x, Complex *y) {
  assert(x == y);
  (void)y;
  const struct NS_(PairTwiddleTransposeData) *data =
      (const struct NS_(PairTwiddleTransposeData) *)NS(ComplexFunction_data)(p);
  assert(data);
#if USE_OPENMP
  data->evaluate(data->size, data->v, data->w, x, data->num_threads);
#else
  data->evaluate(data->size, data->v, data->w, x, 1);
#endif
}

struct NS(ComplexFunction) *NS_(PairTwiddleTranspose_new)(
    struct NS(DataFactory) *data, bool backward, Long size, int num_threads) {
  NS_(PairTwiddleTranspose_fn) evaluate =
      backward ? NS_(PairTwiddleTranspose_B) : NS_(PairTwiddleTranspose_F);
  return NS(ComplexFunction_new)(
      NS_(PairTwiddleTranspose_evaluate), jomfftDestructPointer,
      NS_(PairTwiddleTransposeData_new)(
          evaluate, size,
          NS(DataFactory_twiddleVector)(data, size, 2 * size, 0),
          NS(DataFactory_largeTwiddleMatrix)(data, size, 2 * size * size,
                                             BLOCK_SIZE),
          num_threads));
}
