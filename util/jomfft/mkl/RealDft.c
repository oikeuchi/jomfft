#include "RealDft.h"

#if USE_MKL

#include "HandleUtil.h"

static void Mkl_RealDft_F_evaluate(struct NS(RealToComplexFunction) *p, Real *x,
                                   Complex *y) {
  DFTI_DESCRIPTOR_HANDLE *handle =
      (DFTI_DESCRIPTOR_HANDLE *)NS(RealToComplexFunction_data)(p);
  DftiComputeForward(*handle, x, y);
}

static void Mkl_RealDft_B_evaluate(struct NS(ComplexToRealFunction) *p,
                                   Complex *x, Real *y) {
  DFTI_DESCRIPTOR_HANDLE *handle =
      (DFTI_DESCRIPTOR_HANDLE *)NS(ComplexToRealFunction_data)(p);
  DftiComputeBackward(*handle, x, y);
}

struct NS(RealToComplexFunction) *NS(Mkl_RealDft_F_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  MKL_LONG *length = NS(Mkl_GetLength)(s->dimension, s->dft_sizes);
  MKL_LONG *in_strides = NS(Mkl_GetStrides)(s->dimension, s->in_storage_sizes,
                                            s->in_stride, s->in_distance);
  MKL_LONG *out_strides = NS(Mkl_GetStrides)(s->dimension, s->out_storage_sizes,
                                             s->out_stride, s->out_distance);
  DFTI_DESCRIPTOR_HANDLE *handle = NEW(DFTI_DESCRIPTOR_HANDLE);
  if (s->dimension == 1) {
    DftiCreateDescriptor(handle, MKL_PRECISION__, DFTI_REAL,
                         (MKL_LONG)s->dimension, *length);
  } else {
    DftiCreateDescriptor(handle, MKL_PRECISION__, DFTI_REAL,
                         (MKL_LONG)s->dimension, length);
  }
  DftiSetValue(*handle, DFTI_CONJUGATE_EVEN_STORAGE, DFTI_COMPLEX_COMPLEX);
  DftiSetValue(*handle, DFTI_PLACEMENT,
               inplace ? DFTI_INPLACE : DFTI_NOT_INPLACE);
  DftiSetValue(*handle, DFTI_ORDERING, DFTI_ORDERED);
  DftiSetValue(*handle, DFTI_WORKSPACE, DFTI_ALLOW);
  DftiSetValue(*handle, DFTI_NUMBER_OF_TRANSFORMS, (MKL_LONG)s->batch_size);
  DftiSetValue(*handle, DFTI_INPUT_STRIDES, in_strides);
  DftiSetValue(*handle, DFTI_OUTPUT_STRIDES, out_strides);
  if (s->batch_size > 1) {
    DftiSetValue(*handle, DFTI_INPUT_DISTANCE, (MKL_LONG)s->in_distance);
    DftiSetValue(*handle, DFTI_OUTPUT_DISTANCE, (MKL_LONG)s->out_distance);
  }
  DftiCommitDescriptor(*handle);

  ARRAY_DELETE(length);
  ARRAY_DELETE(in_strides);
  ARRAY_DELETE(out_strides);

  return NS(RealToComplexFunction_new)(Mkl_RealDft_F_evaluate,
                                       NS(Mkl_Handle_destruct), handle);
}

struct NS(ComplexToRealFunction) *NS(Mkl_RealDft_B_new)(
    const struct NS(Wrapper_RealDftSizes) *sizes, bool inplace) {
  const struct NS(Wrapper_RealDftSizes) *s = sizes;
  MKL_LONG *length = NS(Mkl_GetLength)(s->dimension, s->dft_sizes);
  MKL_LONG *in_strides = NS(Mkl_GetStrides)(s->dimension, s->in_storage_sizes,
                                            s->in_stride, s->in_distance);
  MKL_LONG *out_strides = NS(Mkl_GetStrides)(s->dimension, s->out_storage_sizes,
                                             s->out_stride, s->out_distance);
  DFTI_DESCRIPTOR_HANDLE *handle = NEW(DFTI_DESCRIPTOR_HANDLE);
  if (s->dimension == 1) {
    DftiCreateDescriptor(handle, MKL_PRECISION__, DFTI_REAL,
                         (MKL_LONG)s->dimension, *length);
  } else {
    DftiCreateDescriptor(handle, MKL_PRECISION__, DFTI_REAL,
                         (MKL_LONG)s->dimension, length);
  }
  DftiSetValue(*handle, DFTI_CONJUGATE_EVEN_STORAGE, DFTI_COMPLEX_COMPLEX);
  DftiSetValue(*handle, DFTI_PLACEMENT,
               inplace ? DFTI_INPLACE : DFTI_NOT_INPLACE);
  DftiSetValue(*handle, DFTI_ORDERING, DFTI_ORDERED);
  DftiSetValue(*handle, DFTI_WORKSPACE, DFTI_ALLOW);
  DftiSetValue(*handle, DFTI_NUMBER_OF_TRANSFORMS, (MKL_LONG)s->batch_size);
  DftiSetValue(*handle, DFTI_INPUT_STRIDES, out_strides);
  DftiSetValue(*handle, DFTI_OUTPUT_STRIDES, in_strides);
  if (s->batch_size > 1) {
    DftiSetValue(*handle, DFTI_INPUT_DISTANCE, (MKL_LONG)s->out_distance);
    DftiSetValue(*handle, DFTI_OUTPUT_DISTANCE, (MKL_LONG)s->in_distance);
  }
  DftiCommitDescriptor(*handle);

  ARRAY_DELETE(length);
  ARRAY_DELETE(in_strides);
  ARRAY_DELETE(out_strides);

  return NS(ComplexToRealFunction_new)(Mkl_RealDft_B_evaluate,
                                       NS(Mkl_Handle_destruct), handle);
}

#endif
