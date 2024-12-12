#include "DftSizeUtil.h"

#include <jomfft/util/IntUtil.h>
#include <jomfft/util/LongUtil.h>

int *NS(Wrapper_GetStorageSizes)(int dimension, const int *dft_sizes,
                                 int margin) {
  int *out = ARRAY_NEW(int, dimension);
  for (int i = 0; i < dimension; ++i) {
    out[i] = dft_sizes[i] + margin;
  }
  return out;
}

void NS(Wrapper_GetBatchLayout)(int dimension, const int *storage_sizes,
                                int batch_size, int margin, int stride,
                                bool inner_batch, int *batch_stride,
                                int *batch_distance) {
  if (batch_size == 1) {
    *batch_stride = stride;
    *batch_distance = 0;
  } else {
    if (inner_batch) {
      *batch_stride = (batch_size + margin) * stride;
      *batch_distance = stride;
    } else {
      *batch_stride = 1;
      *batch_distance = jomfftInt_Product(dimension, storage_sizes) * stride;
    }
  }
}

Long *NS(Wrapper_GetTensorSizes)(int dimension, const int *storage_sizes,
                                 int batch_size, int margin, int stride,
                                 bool inner_batch, bool strided) {
  if (batch_size == 1) {
    if (strided) {
      Long *out = ARRAY_NEW(Long, dimension + 1);
      for (int i = 0; i < dimension; ++i) {
        out[i] = storage_sizes[i];
      }
      out[dimension] = stride;
      return out;
    } else {
      return jomfftLong_FromIntArray(dimension, storage_sizes);
    }
  } else {
    Long *out = ARRAY_NEW(Long, dimension + 1 + (int)strided);
    if (inner_batch) {
      for (int i = 0; i < dimension; ++i) {
        out[i] = storage_sizes[i];
      }
      out[dimension] = batch_size + margin;
    } else {
      out[0] = batch_size;
      for (int i = 0; i < dimension; ++i) {
        out[i + 1] = storage_sizes[i];
      }
    }
    if (strided) {
      out[dimension + 1] = stride;
    }
    return out;
  }
}
