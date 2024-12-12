#include "ComplexDft.h"

#include <jomfft/util/LongUtil.h>

#include "../util/TensorUtil.h"

static void Wrapper_ComplexDft_F_evaluate(struct NS(ComplexFunction) *p,
                                          Complex *x, Complex *y) {
  NS(ComplexDftForward)((NS(ComplexDftHandle))NS(ComplexFunction_data)(p), x,
                        y);
}

static void Wrapper_ComplexDft_B_evaluate(struct NS(ComplexFunction) *p,
                                          Complex *x, Complex *y) {
  NS(ComplexDftBackward)((NS(ComplexDftHandle))NS(ComplexFunction_data)(p), x,
                         y);
}

static void Wrapper_ComplexDft_destruct(void *p) {
  NS(ComplexDftDestruct)((NS(ComplexDftHandle))p);
}

struct NS(ComplexFunction) *NS(Wrapper_ComplexDft_new_0)(bool backward,
                                                         int dimension,
                                                         const int *dft_sizes,
                                                         bool inplace) {
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(ComplexDftHandle) handle = 0;
  NS(ComplexFunction_fn) func =
      backward ? Wrapper_ComplexDft_B_evaluate : Wrapper_ComplexDft_F_evaluate;
  NS(ComplexDft)(&handle, dimension, dft_sizes, placement);
  return NS(ComplexFunction_new)(func, Wrapper_ComplexDft_destruct, handle);
}

struct NS(ComplexFunction) *NS(Wrapper_ComplexDft_new_1)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace) {
  const struct NS(Wrapper_ComplexDftSizes) *s = sizes;
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(ComplexDftHandle) handle = 0;
  NS(ComplexFunction_fn) func = 0;
  if (backward) {
    NS(ComplexDftBatch)(&handle, s->dimension, s->dft_sizes, s->batch_size,
                        s->out_storage_sizes, s->out_stride, s->out_distance,
                        s->in_storage_sizes, s->in_stride, s->in_distance,
                        placement);
    func = Wrapper_ComplexDft_B_evaluate;
  } else {
    NS(ComplexDftBatch)(&handle, s->dimension, s->dft_sizes, s->batch_size,
                        s->in_storage_sizes, s->in_stride, s->in_distance,
                        s->out_storage_sizes, s->out_stride, s->out_distance,
                        placement);
    func = Wrapper_ComplexDft_F_evaluate;
  }
  return NS(ComplexFunction_new)(func, Wrapper_ComplexDft_destruct, handle);
}

struct NS(ComplexFunction) *NS(Wrapper_ComplexDft_new_2)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace) {
  const struct NS(Wrapper_ComplexDftSizes) *s = sizes;
  Long *dft_sizes = jomfftLong_FromIntArray(s->dimension, s->dft_sizes);
  Long *in_strides =
      NS(Tensor_GetStrides)(s->tensor_dimension, s->in_tensor_storage_sizes);
  Long *out_strides =
      NS(Tensor_GetStrides)(s->tensor_dimension, s->out_tensor_storage_sizes);
  jomfftPlacement placement = inplace ? jomfftInplace : jomfftNotInplace;
  NS(ComplexDftHandle) handle = 0;
  NS(ComplexFunction_fn) func = 0;
  if (backward) {
    NS(ComplexDftBatch_)(&handle, s->dimension, dft_sizes, s->batch_size,
                         out_strides, s->out_distance, in_strides,
                         s->in_distance, placement);
    func = Wrapper_ComplexDft_B_evaluate;
  } else {
    NS(ComplexDftBatch_)(&handle, s->dimension, dft_sizes, s->batch_size,
                         in_strides, s->in_distance, out_strides,
                         s->out_distance, placement);
    func = Wrapper_ComplexDft_F_evaluate;
  }
  ARRAY_DELETE(dft_sizes);
  ARRAY_DELETE(in_strides);
  ARRAY_DELETE(out_strides);
  return NS(ComplexFunction_new)(func, Wrapper_ComplexDft_destruct, handle);
}
