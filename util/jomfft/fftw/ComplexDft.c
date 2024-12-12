#include "ComplexDft.h"

#if USE_FFTW

#include "../util/random/ComplexTensor.h"
#include "../wrapper/DftSizeUtil.h"
#include "Plan.h"

static void Fftw_ComplexDft_evaluate(struct NS(ComplexFunction *) p, Complex *x,
                                     Complex *y) {
  assert(p);
  FFTW_(plan) *plan = (FFTW_(plan) *)NS(ComplexFunction_data)(p);
  FFTW_(execute_dft)(*plan, (FFTW_(complex) *)x, (FFTW_(complex) *)y);
}

struct NS(ComplexFunction) *NS(Fftw_ComplexDft_new)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace, unsigned int flags) {
  const struct NS(Wrapper_ComplexDftSizes) *s = sizes;
  const int sign = backward ? FFTW_BACKWARD : FFTW_FORWARD;
  FFTW_(plan) *plan = NS(Fftw_Plan_new)();
  if (inplace) {
    struct NS(ComplexTensor) *inout = NS(Random_ComplexTensor_new)(
        s->tensor_dimension, s->in_tensor_storage_sizes);
    *plan = FFTW_(plan_many_dft)(
        s->dimension, s->dft_sizes, s->batch_size,
        (FFTW_(complex) *)inout->data, s->in_storage_sizes, s->in_stride,
        s->in_distance, (FFTW_(complex) *)inout->data, s->out_storage_sizes,
        s->out_stride, s->out_distance, sign, flags);
    NS(ComplexTensor_delete)(inout);
  } else {
    struct NS(ComplexTensor) *in = NS(Random_ComplexTensor_new)(
        s->tensor_dimension, s->in_tensor_storage_sizes);
    struct NS(ComplexTensor) *out =
        NS(ComplexTensor_new)(s->tensor_dimension, s->out_tensor_storage_sizes);
    *plan = FFTW_(plan_many_dft)(
        s->dimension, s->dft_sizes, s->batch_size, (FFTW_(complex) *)in->data,
        s->in_storage_sizes, s->in_stride, s->in_distance,
        (FFTW_(complex) *)out->data, s->out_storage_sizes, s->out_stride,
        s->out_distance, sign, flags);
    NS(ComplexTensor_delete)(in);
    NS(ComplexTensor_delete)(out);
  }
  return NS(ComplexFunction_new)(Fftw_ComplexDft_evaluate,
                                 NS(Fftw_Plan_destruct), plan);
}

#endif
