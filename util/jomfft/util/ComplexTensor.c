#include "ComplexTensor.h"

#include <jomfft/math/Math.h>

#include "ComplexUtil.h"
#include "RealTensor.h"

#define TENSOR NS(ComplexTensor)
#define TENSOR_(n) NS(ComplexTensor_##n)
#define VALUE_TYPE Complex
#define VALUE_TYPE_(n) NS(Complex_##n)

#include "template/TensorImpl.h"

Real TENSOR_(absSum)(const struct TENSOR *p) {
  assert(p);
  const VALUE_TYPE *it = TENSOR_(cbegin)(p);
  const VALUE_TYPE *end = TENSOR_(cend)(p);
  Real sum = 0;
  for (; it != end; ++it) {
    sum += C_ABS(*it);
  }
  return sum;
}

static void TENSOR_(CopyAsReal_)(int dimension, int i, const Long *sizes,
                                 Long *indexes, const struct TENSOR *x,
                                 struct NS(RealTensor) *y) {
  assert(sizes);
  assert(indexes);
  if (i == dimension - 1) {
    for (Long j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      Complex x0 = TENSOR_(get)(x, indexes);
      indexes[i] = 2 * j;
      NS(RealTensor_set)(y, indexes, C_REAL(x0));
      indexes[i] = 2 * j + 1;
      NS(RealTensor_set)(y, indexes, C_IMAG(x0));
    }
  } else {
    for (Long j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      TENSOR_(CopyAsReal_)(dimension, i + 1, sizes, indexes, x, y);
    }
  }
}

void TENSOR_(CopyAsReal)(const struct NS(TensorShape) *shape,
                         const struct TENSOR *x, struct NS(RealTensor) *y) {
  assert(shape);
  if (shape->dimension > 0) {
    Long *indexes = ARRAY_NEW(Long, shape->dimension);
    TENSOR_(CopyAsReal_)(shape->dimension, 0, shape->sizes, indexes, x, y);
    ARRAY_DELETE(indexes);
  }
}
