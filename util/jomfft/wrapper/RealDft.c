#include "RealDft.h"

#include <jomfft/util/LongUtil.h>

#include "../util/TensorUtil.h"

static void Wrapper_RealDft_F_evaluate(struct NS(RealToComplexFunction) *p,
                                       Real *x, Complex *y) {
  NS(RealDftForward)((NS(RealDftHandle))NS(RealToComplexFunction_data)(p), x,
                     y);
}

static void Wrapper_RealDft_B_evaluate(struct NS(ComplexToRealFunction) *p,
                                       Complex *x, Real *y) {
  NS(RealDftBackward)((NS(RealDftHandle))NS(ComplexToRealFunction_data)(p), x,
                      y);
}

static void Wrapper_RealDft_destruct(void *p) {
  NS(RealDftDestruct)((NS(RealDftHandle))p);
}

struct NS(RealToComplexFunction) *NS(Wrapper_RealDft_F_new_0)(
    int dimension, const int *dft_sizes, bool inplace) {
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(RealDftHandle) handle = 0;
  NS(RealDft)(&handle, dimension, dft_sizes, placement);
  return NS(RealToComplexFunction_new)(Wrapper_RealDft_F_evaluate,
                                       Wrapper_RealDft_destruct, handle);
}

struct NS(RealToComplexFunction) *NS(Wrapper_RealDft_F_new_1)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(RealDftHandle) handle = 0;
  NS(RealDftBatch)(&handle, s->dimension, s->dft_sizes, s->batch_size,
                   s->in_storage_sizes, s->in_stride, s->in_distance,
                   s->out_storage_sizes, s->out_stride, s->out_distance,
                   placement);
  return NS(RealToComplexFunction_new)(Wrapper_RealDft_F_evaluate,
                                       Wrapper_RealDft_destruct, handle);
}

struct NS(RealToComplexFunction) *NS(Wrapper_RealDft_F_new_2)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  Long *dft_sizes = jomfftLong_FromIntArray(s->dimension, s->dft_sizes);
  Long *in_strides =
      NS(Tensor_GetStrides)(s->tensor_dimension, s->in_tensor_storage_sizes);
  Long *out_strides =
      NS(Tensor_GetStrides)(s->tensor_dimension, s->out_tensor_storage_sizes);
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(RealDftHandle) handle = 0;
  NS(RealDftBatch_)(&handle, s->dimension, dft_sizes, s->batch_size, in_strides,
                    s->in_distance, out_strides, s->out_distance, placement);
  ARRAY_DELETE(dft_sizes);
  ARRAY_DELETE(in_strides);
  ARRAY_DELETE(out_strides);
  return NS(RealToComplexFunction_new)(Wrapper_RealDft_F_evaluate,
                                       Wrapper_RealDft_destruct, handle);
}

struct NS(ComplexToRealFunction) *NS(Wrapper_RealDft_B_new_0)(
    int dimension, const int *dft_sizes, bool inplace) {
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(RealDftHandle) handle = 0;
  NS(RealDft)(&handle, dimension, dft_sizes, placement);
  return NS(ComplexToRealFunction_new)(Wrapper_RealDft_B_evaluate,
                                       Wrapper_RealDft_destruct, handle);
}

struct NS(ComplexToRealFunction) *NS(Wrapper_RealDft_B_new_1)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(RealDftHandle) handle = 0;
  NS(RealDftBatch)(&handle, s->dimension, s->dft_sizes, s->batch_size,
                   s->in_storage_sizes, s->in_stride, s->in_distance,
                   s->out_storage_sizes, s->out_stride, s->out_distance,
                   placement);
  return NS(ComplexToRealFunction_new)(Wrapper_RealDft_B_evaluate,
                                       Wrapper_RealDft_destruct, handle);
}

struct NS(ComplexToRealFunction) *NS(Wrapper_RealDft_B_new_2)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  Long *dft_sizes = jomfftLong_FromIntArray(s->dimension, s->dft_sizes);
  Long *in_strides =
      NS(Tensor_GetStrides)(s->tensor_dimension, s->in_tensor_storage_sizes);
  Long *out_strides =
      NS(Tensor_GetStrides)(s->tensor_dimension, s->out_tensor_storage_sizes);
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(RealDftHandle) handle = 0;
  NS(RealDftBatch_)(&handle, s->dimension, dft_sizes, s->batch_size, in_strides,
                    s->in_distance, out_strides, s->out_distance, placement);
  ARRAY_DELETE(dft_sizes);
  ARRAY_DELETE(in_strides);
  ARRAY_DELETE(out_strides);
  return NS(ComplexToRealFunction_new)(Wrapper_RealDft_B_evaluate,
                                       Wrapper_RealDft_destruct, handle);
}
