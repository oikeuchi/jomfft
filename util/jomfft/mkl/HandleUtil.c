#include "HandleUtil.h"

#if USE_MKL

#include "../util/TensorUtil.h"

MKL_LONG *NS(Mkl_GetLength)(int dimension, const int *dft_sizes) {
  MKL_LONG *out = ARRAY_NEW(MKL_LONG, dimension);
  for (int i = 0; i < dimension; ++i) {
    out[i] = (MKL_LONG)dft_sizes[i];
  }
  return out;
}

MKL_LONG *NS(Mkl_GetStrides)(int dimension, const int *storage_sizes,
                             int stride, int distance) {
  const int storage_dimension = distance == 1 ? dimension + 1 : dimension;
  MKL_LONG *out = ARRAY_NEW(MKL_LONG, storage_dimension + 1);
  out[0] = 0;
  if (distance == 1) {
    out[storage_dimension] = 1;
  }
  MKL_LONG s = stride;
  for (int i = dimension - 1; i >= 0; --i) {
    out[i + 1] = s;
    s *= storage_sizes[i];
  }
  return out;
}

void NS(Mkl_Handle_destruct)(void *p) {
  if (p) {
    DFTI_DESCRIPTOR_HANDLE *o = (DFTI_DESCRIPTOR_HANDLE *)p;
    DftiFreeDescriptor(o);
    DELETE(o);
  }
}

#endif
