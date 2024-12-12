#include "ComplexDft.h"

#if USE_MKL

#include "HandleUtil.h"

static void Mkl_ComplexDft_F_evaluate(struct NS(ComplexFunction) *p, Complex *x,
                                      Complex *y) {
  DFTI_DESCRIPTOR_HANDLE *handle =
      (DFTI_DESCRIPTOR_HANDLE *)NS(ComplexFunction_data)(p);
  DftiComputeForward(*handle, x, y);
}

static void Mkl_ComplexDft_B_evaluate(struct NS(ComplexFunction) *p, Complex *x,
                                      Complex *y) {

  DFTI_DESCRIPTOR_HANDLE *handle =
      (DFTI_DESCRIPTOR_HANDLE *)NS(ComplexFunction_data)(p);
  DftiComputeBackward(*handle, x, y);
}

struct NS(ComplexFunction) *NS(Mkl_ComplexDft_new)(
    bool backward, const struct NS(Wrapper_ComplexDftSizes) *sizes,
    bool inplace) {
  const struct NS(Wrapper_ComplexDftSizes) *s = sizes;
  MKL_LONG *length = NS(Mkl_GetLength)(s->dimension, s->dft_sizes);
  MKL_LONG *in_strides = NS(Mkl_GetStrides)(s->dimension, s->in_storage_sizes,
                                            s->in_stride, s->in_distance);
  MKL_LONG *out_strides = NS(Mkl_GetStrides)(s->dimension, s->out_storage_sizes,
                                             s->out_stride, s->out_distance);
  DFTI_DESCRIPTOR_HANDLE *handle = NEW(DFTI_DESCRIPTOR_HANDLE);
  if (s->dimension == 1) {
    DftiCreateDescriptor(handle, MKL_PRECISION__, DFTI_COMPLEX,
                         (MKL_LONG)s->dimension, *length);
  } else {
    DftiCreateDescriptor(handle, MKL_PRECISION__, DFTI_COMPLEX,
                         (MKL_LONG)s->dimension, length);
  }
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

  return NS(ComplexFunction_new)(backward ? Mkl_ComplexDft_B_evaluate
                                          : Mkl_ComplexDft_F_evaluate,
                                 NS(Mkl_Handle_destruct), handle);
}

#endif
