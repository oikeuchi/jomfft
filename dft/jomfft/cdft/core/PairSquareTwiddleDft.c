#include "PairSquareTwiddleDft.h"

#include "../../data/TwiddleMatrix.h"
#include "impl/PairSquareTwiddleDft_B16.h"
#include "impl/PairSquareTwiddleDft_B32.h"
#include "impl/PairSquareTwiddleDft_B8.h"
#include "impl/PairSquareTwiddleDft_F16.h"
#include "impl/PairSquareTwiddleDft_F32.h"
#include "impl/PairSquareTwiddleDft_F8.h"

#define NS_(n) NS(CDftCore_##n)

typedef void (*NS_(PairSquareTwiddleDft_fn))(const Complex *, const Complex *,
                                             Complex *, Complex *);

struct NS_(PairSquareTwiddleDftData) {
  NS_(PairSquareTwiddleDft_fn) evaluate;
  const Complex *w0;
  const Complex *w1;
};

static struct NS_(PairSquareTwiddleDftData) *NS_(PairSquareTwiddleDftData_new)(
    NS_(PairSquareTwiddleDft_fn) evaluate, const Complex *w0,
    const Complex *w1) {
  assert(evaluate);
  assert(w0);
  assert(w1);
  struct NS_(PairSquareTwiddleDftData) *out =
      NEW(struct NS_(PairSquareTwiddleDftData));
  *out = (struct NS_(PairSquareTwiddleDftData)){
      .evaluate = evaluate, .w0 = w0, .w1 = w1};
  return out;
}

static void NS_(PairSquareTwiddleDft_evaluate)(struct NS(ComplexFunction) *p,
                                               Complex *x, Complex *y) {
  struct NS_(PairSquareTwiddleDftData) *data =
      (struct NS_(PairSquareTwiddleDftData) *)NS(ComplexFunction_data)(p);
  assert(data);
  data->evaluate(data->w0, data->w1, x, y);
}

static struct NS(ComplexFunction) *NS_(PairSquareTwiddleDft_new_)(
    NS_(PairSquareTwiddleDft_fn) evaluate, const Complex *w0,
    const Complex *w1) {
  return NS(ComplexFunction_new)(
      NS_(PairSquareTwiddleDft_evaluate), jomfftDestructPointer,
      NS_(PairSquareTwiddleDftData_new)(evaluate, w0, w1));
}

struct NS_(PairSquareTwiddleDftFactory) {
  int dft_size;
  NS_(PairSquareTwiddleDft_fn) forward;
  NS_(PairSquareTwiddleDft_fn) backward;
};

#define FACTORY(n)                                                             \
  { n, NS_(PairSquareTwiddleDft_F##n), NS_(PairSquareTwiddleDft_B##n) }

static const struct NS_(PairSquareTwiddleDftFactory) g_table[] = {
    FACTORY(8), FACTORY(16), FACTORY(32)};

#define g_table_size                                                           \
  (sizeof(g_table) / sizeof(struct NS_(PairSquareTwiddleDftFactory)))

static const struct NS_(PairSquareTwiddleDftFactory)
    *NS_(PairSquareTwiddleDftFactory_Find)(
        Long table_size, const struct NS_(PairSquareTwiddleDftFactory) *table,
        int dft_size) {
  const struct NS_(PairSquareTwiddleDftFactory) *it = table;
  const struct NS_(PairSquareTwiddleDftFactory) *end = table + table_size;
  const struct NS_(PairSquareTwiddleDftFactory) *out = NULL;
  for (; it != end; ++it) {
    if (it->dft_size == dft_size) {
      out = it;
      break;
    }
  }
  return out;
}

static struct NS(ComplexFunction) *NS_(PairSquareTwiddleDftFactory_create)(
    const struct NS_(PairSquareTwiddleDftFactory) *p,
    struct NS(DataFactory) *data, bool backward) {
  assert(p);
  NS_(PairSquareTwiddleDft_fn) func = backward ? p->backward : p->forward;
  return NS_(PairSquareTwiddleDft_new_)(
      func,
      NS(DataFactory_twiddleMatrix)(data, 2, p->dft_size, 2 * p->dft_size, 0,
                                    0),
      NS(DataFactory_twiddleMatrix)(data, p->dft_size, 2 * p->dft_size,
                                    2 * p->dft_size * p->dft_size, 0, 0));
}

bool NS_(PairSquareTwiddleDft_HasKey)(int dft_size) {
  return NS_(PairSquareTwiddleDftFactory_Find)(g_table_size, g_table,
                                               dft_size) != NULL;
}

struct NS(ComplexFunction) *NS_(PairSquareTwiddleDft_new)(
    struct NS(DataFactory) *data, bool backward, int dft_size) {
  const struct NS_(PairSquareTwiddleDftFactory) *p =
      NS_(PairSquareTwiddleDftFactory_Find)(g_table_size, g_table, dft_size);
  if (p) {
    return NS_(PairSquareTwiddleDftFactory_create)(p, data, backward);
  } else {
    return NULL;
  }
}
