#include "TensorUtil.h"

Long *NS(Tensor_GetStrides)(int dimension, const Long *sizes) {
  Long *out = ARRAY_NEW(Long, dimension);
  NS(Tensor_GetStrides_)(dimension, sizes, out);
  return out;
}

void NS(Tensor_GetStrides_)(int dimension, const Long *sizes, Long *out) {
  Long s = 1;
  for (int k = dimension - 1; k >= 0; --k) {
    out[k] = s;
    s *= sizes[k];
  }
}
