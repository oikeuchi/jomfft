#include "Dft.h"

#include <jomfft/ArgCheck.h>
#include <jomfft/util/LongUtil.h>

#include "cdft/MainDft.h"
#include "rdft/MainDft.h"

static Long *GetStrides(int dimension, const int *sizes, int stride) {
  Long *out = ARRAY_NEW(Long, dimension);
  Long s = stride;
  for (int k = dimension - 1; k >= 0; --k) {
    out[k] = s;
    s *= sizes[k];
  }
  return out;
}

struct NS(ComplexDftHandle_) {
  struct NS(DataFactory) *data;
  struct NS(ComplexFunction) *forward;
  struct NS(ComplexFunction) *backward;
};

static struct NS(ComplexDftHandle_) *
ComplexDft_new(Long dimension, const Long *dft_sizes, Long batch_size,
               const Long *in_strides, Long in_distance,
               const Long *out_strides, Long out_distance,
               jomfftPlacement placement) {
  const bool inplace = placement == jomfftInplace;
  struct NS(ComplexDftHandle_) *out = NEW(struct NS(ComplexDftHandle_));
  const int num_threads = jomfftGetMaxThreads();
  out->data = NS(DataFactory_new)();
  out->forward = NS(CDft_MainDft_new)(
      out->data, false, dimension, dft_sizes, batch_size, in_strides,
      in_distance, out_strides, out_distance, inplace, num_threads);
  out->backward = NS(CDft_MainDft_new)(
      out->data, true, dimension, dft_sizes, batch_size, out_strides,
      out_distance, in_strides, in_distance, inplace, num_threads);
  return out;
}

static void ComplexDft_delete(struct NS(ComplexDftHandle_) *p) {
  if (p) {
    if (p->forward) {
      NS(ComplexFunction_delete)(p->forward);
      p->forward = NULL;
    }
    if (p->backward) {
      NS(ComplexFunction_delete)(p->backward);
      p->backward = NULL;
    }
    if (p->data) {
      NS(DataFactory_delete)(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

struct NS(RealDftHandle_) {
  struct NS(DataFactory) *data;
  struct NS(RealToComplexFunction) *forward;
  struct NS(ComplexToRealFunction) *backward;
};

static struct NS(RealDftHandle_) *
RealDft_new(Long dimension, const Long *dft_sizes, Long batch_size,
            const Long *in_strides, Long in_distance, const Long *out_strides,
            Long out_distance, jomfftPlacement placement) {
  const bool inplace = placement == jomfftInplace;
  struct NS(RealDftHandle_) *out = NEW(struct NS(RealDftHandle_));
  const int num_threads = jomfftGetMaxThreads();
  out->data = NS(DataFactory_new)();
  out->forward = NS(RDft_MainDft_F_new)(
      out->data, dimension, dft_sizes, batch_size, in_strides, in_distance,
      out_strides, out_distance, inplace, num_threads);
  out->backward = NS(RDft_MainDft_B_new)(
      out->data, dimension, dft_sizes, batch_size, out_strides, out_distance,
      in_strides, in_distance, inplace, num_threads);
  return out;
}

static void RealDft_delete(struct NS(RealDftHandle_) *p) {
  if (p) {
    if (p->forward) {
      NS(RealToComplexFunction_delete)(p->forward);
      p->forward = NULL;
    }
    if (p->backward) {
      NS(ComplexToRealFunction_delete)(p->backward);
      p->backward = NULL;
    }
    if (p->data) {
      NS(DataFactory_delete)(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

static int *GetRealDftInputStorageSizes(int dimension, const int *dft_sizes,
                                        bool inplace) {
  int *out = ARRAY_NEW(int, dimension);
  ARRAY_COPY(int, dimension, dft_sizes, out);
  if (inplace) {
    out[dimension - 1] = 2 * CE_SIZE(out[dimension - 1]);
  }
  return out;
}

static int *GetRealDftOutputStorageSizes(int dimension, const int *dft_sizes) {
  int *out = ARRAY_NEW(int, dimension);
  ARRAY_COPY(int, dimension, dft_sizes, out);
  out[dimension - 1] = CE_SIZE(out[dimension - 1]);
  return out;
}

JOMFFT_API jomfftStatus NS(ComplexDft)(NS(ComplexDftHandle) *handle,
                                       int dimension, const int *dft_sizes,
                                       jomfftPlacement placement) {
  return NS(ComplexDftBatch)(handle, dimension, dft_sizes, 1, dft_sizes, 1, 0,
                             dft_sizes, 1, 0, placement);
}

JOMFFT_API jomfftStatus NS(ComplexDftBatch)(
    NS(ComplexDftHandle) *handle, int dimension, const int *dft_sizes,
    int batch_size, const int *in_storage_sizes, int in_stride, int in_distance,
    const int *out_storage_sizes, int out_stride, int out_distance,
    jomfftPlacement placement) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_POSITIVE(dimension);
  ARGCHECK_NOTNULL(dft_sizes);
  ARGCHECK_ARRAY_GE(dimension, dft_sizes, 2);
  ARGCHECK_POSITIVE(batch_size);
  ARGCHECK_NOTNULL(in_storage_sizes);
  ARGCHECK_ARRAY_GE_(dimension, in_storage_sizes, dft_sizes);
  ARGCHECK_NOTZERO(in_stride);
  ARGCHECK_NOTNULL(out_storage_sizes);
  ARGCHECK_NOTZERO(out_stride);
  ARGCHECK_ARRAY_GE_(dimension, out_storage_sizes, dft_sizes);
  ARGCHECK_PLACEMENT(placement);
  Long *dft_sizes_ = jomfftLong_FromIntArray(dimension, dft_sizes);
  Long *in_strides = GetStrides(dimension, in_storage_sizes, in_stride);
  Long *out_strides = GetStrides(dimension, out_storage_sizes, out_stride);
  *handle = ComplexDft_new(dimension, dft_sizes_, batch_size, in_strides,
                           in_distance, out_strides, out_distance, placement);
  ARRAY_DELETE(dft_sizes_);
  ARRAY_DELETE(in_strides);
  ARRAY_DELETE(out_strides);
  return jomfftSuccess;
}

JOMFFT_API jomfftStatus NS(ComplexDftBatch_)(
    NS(ComplexDftHandle) *handle, Long dimension, const Long *dft_sizes,
    Long batch_size, const Long *in_strides, Long in_distance,
    const Long *out_strides, Long out_distance, jomfftPlacement placement) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_POSITIVE(dimension);
  ARGCHECK_NOTNULL(dft_sizes);
  ARGCHECK_ARRAY_GE(dimension, dft_sizes, 2);
  ARGCHECK_POSITIVE(batch_size);
  ARGCHECK_NOTNULL(in_strides);
  ARGCHECK_NOTNULL(out_strides);
  ARGCHECK_PLACEMENT(placement);
  *handle = ComplexDft_new(dimension, dft_sizes, batch_size, in_strides,
                           in_distance, out_strides, out_distance, placement);
  return jomfftSuccess;
}

JOMFFT_API jomfftStatus NS(ComplexDftForward)(NS(ComplexDftHandle) handle,
                                              Complex *x, Complex *y) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_NOTNULL(x);
  ARGCHECK_NOTNULL(y);
  NS(ComplexFunction_evaluate)(handle->forward, x, y);
  return jomfftSuccess;
}

JOMFFT_API jomfftStatus NS(ComplexDftBackward)(NS(ComplexDftHandle) handle,
                                               Complex *x, Complex *y) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_NOTNULL(x);
  ARGCHECK_NOTNULL(y);
  NS(ComplexFunction_evaluate)(handle->backward, x, y);
  return jomfftSuccess;
}

JOMFFT_API jomfftStatus NS(ComplexDftDestruct)(NS(ComplexDftHandle) handle) {
  ComplexDft_delete(handle);
  return jomfftSuccess;
}

JOMFFT_API jomfftStatus NS(RealDft)(NS(RealDftHandle) *handle, int dimension,
                                    const int *dft_sizes,
                                    jomfftPlacement placement) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_POSITIVE(dimension);
  ARGCHECK_NOTNULL(dft_sizes);
  ARGCHECK_PLACEMENT(placement);
  int *in_storage_sizes = GetRealDftInputStorageSizes(
      dimension, dft_sizes, placement == jomfftInplace);
  int *out_storage_sizes = GetRealDftOutputStorageSizes(dimension, dft_sizes);
  jomfftStatus result =
      NS(RealDftBatch)(handle, dimension, dft_sizes, 1, in_storage_sizes, 1, 0,
                       out_storage_sizes, 1, 0, placement);
  ARRAY_DELETE(in_storage_sizes);
  ARRAY_DELETE(out_storage_sizes);
  return result;
}

JOMFFT_API jomfftStatus NS(RealDftBatch)(
    NS(RealDftHandle) *handle, int dimension, const int *dft_sizes,
    int batch_size, const int *in_storage_sizes, int in_stride, int in_distance,
    const int *out_storage_sizes, int out_stride, int out_distance,
    jomfftPlacement placement) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_POSITIVE(dimension);
  ARGCHECK_NOTNULL(dft_sizes);
  ARGCHECK_ARRAY_GE(dimension, dft_sizes, 2);
  ARGCHECK_POSITIVE(batch_size);
  ARGCHECK_NOTNULL(in_storage_sizes);
  ARGCHECK_ARRAY_GE_(dimension, in_storage_sizes, dft_sizes);
  ARGCHECK_NOTZERO(out_stride);
  ARGCHECK_NOTZERO(in_stride);
  ARGCHECK_NOTNULL(out_storage_sizes);
  if (dimension >= 2) {
    ARGCHECK_ARRAY_GE_(dimension - 1, out_storage_sizes, dft_sizes);
  }
  ARGCHECK_GE(out_storage_sizes[dimension - 1],
              CE_SIZE(dft_sizes[dimension - 1]));
  ARGCHECK_PLACEMENT(placement);
  Long *dft_sizes_ = jomfftLong_FromIntArray(dimension, dft_sizes);
  Long *in_strides = GetStrides(dimension, in_storage_sizes, in_stride);
  Long *out_strides = GetStrides(dimension, out_storage_sizes, out_stride);
  *handle = RealDft_new(dimension, dft_sizes_, batch_size, in_strides,
                        in_distance, out_strides, out_distance, placement);
  ARRAY_DELETE(dft_sizes_);
  ARRAY_DELETE(in_strides);
  ARRAY_DELETE(out_strides);
  return jomfftSuccess;
}

JOMFFT_API jomfftStatus NS(RealDftBatch_)(
    NS(RealDftHandle) *handle, Long dimension, const Long *dft_sizes,
    Long batch_size, const Long *in_strides, Long in_distance,
    const Long *out_strides, Long out_distance, jomfftPlacement placement) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_POSITIVE(dimension);
  ARGCHECK_NOTNULL(dft_sizes);
  ARGCHECK_POSITIVE(batch_size);
  ARGCHECK_NOTNULL(in_strides);
  ARGCHECK_NOTNULL(out_strides);
  ARGCHECK_ARRAY_GE(dimension, dft_sizes, 2);
  ARGCHECK_PLACEMENT(placement);
  *handle = RealDft_new(dimension, dft_sizes, batch_size, in_strides,
                        in_distance, out_strides, out_distance, placement);
  return jomfftSuccess;
}

JOMFFT_API
jomfftStatus NS(RealDftForward)(NS(RealDftHandle) handle, Real *x, Complex *y) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_NOTNULL(x);
  ARGCHECK_NOTNULL(y);
  NS(RealToComplexFunction_evaluate)(handle->forward, x, y);
  return jomfftSuccess;
}

JOMFFT_API
jomfftStatus NS(RealDftBackward)(NS(RealDftHandle) handle, Complex *x,
                                 Real *y) {
  ARGCHECK_NOTNULL(handle);
  ARGCHECK_NOTNULL(x);
  ARGCHECK_NOTNULL(y);
  NS(ComplexToRealFunction_evaluate)(handle->backward, x, y);
  return jomfftSuccess;
}

JOMFFT_API jomfftStatus NS(RealDftDestruct)(NS(RealDftHandle) handle) {
  RealDft_delete(handle);
  return jomfftSuccess;
}
