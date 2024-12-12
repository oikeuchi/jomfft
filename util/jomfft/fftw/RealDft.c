#include "RealDft.h"

#if USE_FFTW

#include "../util/random/ComplexTensor.h"
#include "../util/random/RealTensor.h"
#include "../wrapper/DftSizeUtil.h"
#include "Plan.h"

static void Fftw_RealDft_F_evaluate(struct NS(RealToComplexFunction *) p,
                                    Real *x, Complex *y) {
  assert(p);
  FFTW_(plan) *plan = (FFTW_(plan) *)NS(RealToComplexFunction_data)(p);
  FFTW_(execute_dft_r2c)(*plan, x, (FFTW_(complex) *)y);
}

static void Fftw_RealDft_B_evaluate(struct NS(ComplexToRealFunction *) p,
                                    Complex *x, Real *y) {
  assert(p);
  FFTW_(plan) *plan = (FFTW_(plan) *)NS(ComplexToRealFunction_data)(p);
  FFTW_(execute_dft_c2r)(*plan, (FFTW_(complex) *)x, y);
}

struct NS(RealToComplexFunction) *NS(Fftw_RealDft_F_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace,
    unsigned int flags) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  FFTW_(plan) *plan = NS(Fftw_Plan_new)();
  if (inplace) {
    struct NS(RealTensor) *inout = NS(Random_RealTensor_new)(
        s->tensor_dimension, s->in_tensor_storage_sizes);
    *plan = FFTW_(plan_many_dft_r2c)(
        s->dimension, s->dft_sizes, s->batch_size, inout->data,
        s->in_storage_sizes, s->in_stride, s->in_distance,
        (FFTW_(complex) *)inout->data, s->out_storage_sizes, s->out_stride,
        s->out_distance, flags);
    NS(RealTensor_delete)(inout);
  } else {
    struct NS(RealTensor) *in = NS(Random_RealTensor_new)(
        s->tensor_dimension, s->in_tensor_storage_sizes);
    struct NS(ComplexTensor) *out =
        NS(ComplexTensor_new)(s->tensor_dimension, s->out_tensor_storage_sizes);
    *plan = FFTW_(plan_many_dft_r2c)(
        s->dimension, s->dft_sizes, s->batch_size, in->data,
        s->in_storage_sizes, s->in_stride, s->in_distance,
        (FFTW_(complex) *)out->data, s->out_storage_sizes, s->out_stride,
        s->out_distance, flags);
    NS(RealTensor_delete)(in);
    NS(RealTensor_delete)(out);
  }
  return NS(RealToComplexFunction_new)(Fftw_RealDft_F_evaluate,
                                       NS(Fftw_Plan_destruct), plan);
}

struct NS(ComplexToRealFunction) *NS(Fftw_RealDft_B_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace,
    unsigned int flags) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  FFTW_(plan) *plan = NS(Fftw_Plan_new)();
  if (inplace) {
    struct NS(ComplexTensor) *inout = NS(Random_ComplexTensor_new)(
        s->tensor_dimension, s->out_tensor_storage_sizes);
    *plan = FFTW_(plan_many_dft_c2r)(
        s->dimension, s->dft_sizes, s->batch_size,
        (FFTW_(complex) *)inout->data, s->out_storage_sizes, s->out_stride,
        s->out_distance, (Real *)inout->data, s->in_storage_sizes, s->in_stride,
        s->in_distance, flags);
    NS(RealTensor_delete)(inout);
  } else {
    struct NS(ComplexTensor) *in = NS(Random_ComplexTensor_new)(
        s->tensor_dimension, s->out_tensor_storage_sizes);
    struct NS(RealTensor) *out =
        NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
    *plan = FFTW_(plan_many_dft_c2r)(
        s->dimension, s->dft_sizes, s->batch_size, (FFTW_(complex) *)in->data,
        s->out_storage_sizes, s->out_stride, s->out_distance, out->data,
        s->in_storage_sizes, s->in_stride, s->in_distance, flags);
    NS(RealTensor_delete)(in);
    NS(RealTensor_delete)(out);
  }
  return NS(ComplexToRealFunction_new)(Fftw_RealDft_B_evaluate,
                                       NS(Fftw_Plan_destruct), plan);
}

#endif
