#include "RealTensor.h"

#include <jomfft/math/Math.h>

#include "ComplexTensor.h"
#include "RealUtil.h"

#define TENSOR NS(RealTensor)
#define TENSOR_(n) NS(RealTensor_##n)
#define VALUE_TYPE Real
#define VALUE_TYPE_(n) NS(Real_##n)

#include "template/TensorImpl.h"

Real TENSOR_(absSum)(const struct TENSOR *p) {
  assert(p);
  const VALUE_TYPE *it = TENSOR_(cbegin)(p);
  const VALUE_TYPE *end = TENSOR_(cend)(p);
  Real sum = 0;
  for (; it != end; ++it) {
    sum += R_ABS(*it);
  }
  return sum;
}

static void TENSOR_(CopyAsComplex_)(int dimension, int i, const Long *sizes,
                                    Long *indexes, const struct TENSOR *x,
                                    struct NS(ComplexTensor) *y) {
  assert(sizes);
  assert(indexes);
  if (i == dimension - 1) {
    Long j = 0;
    for (; j < sizes[i] - 1; j += 2) {
      indexes[i] = j;
      Real x_r = TENSOR_(get)(x, indexes);
      indexes[i] = j + 1;
      Real x_i = TENSOR_(get)(x, indexes);
      indexes[i] = j / 2;
      NS(ComplexTensor_set)(y, indexes, Complex_(x_r, x_i));
    }
    if (j < sizes[i]) {
      indexes[i] = j;
      Real x_r = TENSOR_(get)(x, indexes);
      indexes[i] = j / 2;
      NS(ComplexTensor_set)(y, indexes, Complex_R(x_r));
    }
  } else {
    for (Long j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      TENSOR_(CopyAsComplex_)(dimension, i + 1, sizes, indexes, x, y);
    }
  }
}

void TENSOR_(CopyAsComplex)(const struct NS(TensorShape) *shape,
                            const struct TENSOR *x,
                            struct NS(ComplexTensor) *y) {
  assert(shape);
  if (shape->dimension > 0) {
    Long *indexes = ARRAY_NEW(Long, shape->dimension);
    TENSOR_(CopyAsComplex_)(shape->dimension, 0, shape->sizes, indexes, x, y);
    ARRAY_DELETE(indexes);
  }
}
