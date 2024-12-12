#include "RealDftSizes.h"

#include "DftSizeUtil.h"

static int *NS(Wrapper_GetCEStorageSizes)(int dimension, const int *dft_sizes,
                                          int margin) {
  int *out = ARRAY_NEW(int, dimension);
  for (int i = 0; i < dimension - 1; ++i) {
    out[i] = dft_sizes[i] + margin;
  }
  out[dimension - 1] = CE_SIZE(dft_sizes[dimension - 1]) + margin;
  return out;
}

struct NS(Wrapper_RealDftSizes) *NS(Wrapper_RealDftSizes_new)(
    int dimension, const int *dft_sizes, int batch_size, int in_margin,
    int in_stride, int out_margin, int out_stride, bool inner_batch,
    bool inplace) {
  struct NS(Wrapper_RealDftSizes) *out = NEW(struct NS(Wrapper_RealDftSizes));
  out->dimension = dimension;
  out->dft_sizes = ARRAY_NEW(int, dimension);
  ARRAY_COPY(int, dimension, dft_sizes, out->dft_sizes);
  out->batch_size = batch_size;
  out->in_storage_sizes =
      NS(Wrapper_GetStorageSizes)(dimension, dft_sizes, in_margin);
  out->out_storage_sizes =
      NS(Wrapper_GetCEStorageSizes)(dimension, dft_sizes, out_margin);
  if (inplace) {
    out->in_storage_sizes[dimension - 1] =
        2 * out->out_storage_sizes[dimension - 1];
  }
  NS(Wrapper_GetBatchLayout)(dimension, out->in_storage_sizes, batch_size,
                             in_margin, in_stride, inner_batch, &out->in_stride,
                             &out->in_distance);
  NS(Wrapper_GetBatchLayout)(dimension, out->out_storage_sizes, batch_size,
                             out_margin, out_stride, inner_batch,
                             &out->out_stride, &out->out_distance);
  const bool strided = in_stride != 1 || out_stride != 1;
  out->tensor_dimension = dimension + (int)(batch_size > 1) + (int)strided;
  out->tensor_sizes = NS(Wrapper_GetTensorSizes)(
      dimension, dft_sizes, batch_size, 0, 1, inner_batch, strided);
  out->in_tensor_storage_sizes =
      NS(Wrapper_GetTensorSizes)(dimension, out->in_storage_sizes, batch_size,
                                 in_margin, in_stride, inner_batch, strided);
  out->out_tensor_storage_sizes =
      NS(Wrapper_GetTensorSizes)(dimension, out->out_storage_sizes, batch_size,
                                 out_margin, out_stride, inner_batch, strided);
  int *ce_sizes = NS(Wrapper_GetCEStorageSizes)(dimension, dft_sizes, 0);
  out->ce_tensor_sizes = NS(Wrapper_GetTensorSizes)(
      dimension, ce_sizes, batch_size, 0, 1, inner_batch, strided);
  out->ce_axis = (batch_size > 1 && !inner_batch) ? dimension : dimension - 1;
  ARRAY_DELETE(ce_sizes);
  return out;
}

void NS(Wrapper_RealDftSizes_delete)(struct NS(Wrapper_RealDftSizes) *p) {
  if (p) {
    if (p->dft_sizes) {
      ARRAY_DELETE(p->dft_sizes);
      p->dft_sizes = NULL;
    }
    if (p->in_storage_sizes) {
      ARRAY_DELETE(p->in_storage_sizes);
      p->in_storage_sizes = NULL;
    }
    if (p->out_storage_sizes) {
      ARRAY_DELETE(p->out_storage_sizes);
      p->out_storage_sizes = NULL;
    }
    if (p->tensor_sizes) {
      ARRAY_DELETE(p->tensor_sizes);
      p->tensor_sizes = NULL;
    }
    if (p->in_tensor_storage_sizes) {
      ARRAY_DELETE(p->in_tensor_storage_sizes);
      p->in_tensor_storage_sizes = NULL;
    }
    if (p->out_tensor_storage_sizes) {
      ARRAY_DELETE(p->out_tensor_storage_sizes);
      p->out_tensor_storage_sizes = NULL;
    }
    DELETE(p);
  }
}
