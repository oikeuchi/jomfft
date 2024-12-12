#include <jomfft/cdft/MainDft.h>
#include <jomfft/rdft/MainDft.h>
#include <jomfft/util/random/ComplexTensor.h>
#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "DftTest.h"

void NS(ComplexDftTest)() {
  NS(ComplexDftHandle) handle = 0;
  jomfftPlacement placement = jomfftNotInplace;
  int dimension = 2;
  int dft_sizes[] = {4, 8};
  int bad_sizes[] = {4, 1};

  ASSERT_ENUM_EQ(NS(ComplexDft)(NULL, dimension, dft_sizes, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDft)(&handle, 0, dft_sizes, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDft)(&handle, dimension, NULL, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDft)(&handle, dimension, bad_sizes, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(
      NS(ComplexDft)(&handle, dimension, dft_sizes, (jomfftPlacement)-1),
      jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDft)(&handle, dimension, dft_sizes, placement),
                 jomfftSuccess);

  Long dft_sizes_[] = {4, 8};
  Long strides[] = {8, 1};

  const bool inplace = placement == jomfftInplace;
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *forward =
      NS(CDft_MainDft_new)(data, false, dimension, dft_sizes_, 1, strides, 0,
                           strides, 0, inplace, 1);
  struct NS(ComplexFunction) *backward = NS(CDft_MainDft_new)(
      data, true, dimension, dft_sizes_, 1, strides, 0, strides, 0, inplace, 1);
  struct NS(ComplexTensor) *x0 = NS(Random_ComplexTensor_new)(2, dft_sizes_);
  struct NS(ComplexTensor) *x1 = NS(ComplexTensor_copy)(x0);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_zerosLike)(x0);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);

  ASSERT_ENUM_EQ(NS(ComplexDftForward)(handle, x0->data, y0->data),
                 jomfftSuccess);
  NS(ComplexFunction_evaluate)(forward, x1->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);

  ASSERT_ENUM_EQ(NS(ComplexDftBackward)(handle, y0->data, x0->data),
                 jomfftSuccess);
  NS(ComplexFunction_evaluate)(backward, y1->data, x1->data);
  ASSERT_COMPLEXTENSOR_AEQ(x0, x1);

  ASSERT_ENUM_EQ(NS(ComplexDftDestruct)(handle), jomfftSuccess);
  NS(DataFactory_delete)(data);
  NS(ComplexFunction_delete)(forward);
  NS(ComplexFunction_delete)(backward);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(x1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(ComplexDftBatchTest)() {
  NS(ComplexDftHandle) handle = 0;
  jomfftPlacement placement = jomfftNotInplace;
  int dimension = 2;
  int dft_sizes[] = {2, 4};
  int batch_size = 2;
  int in_storage_sizes[] = {2, 4};
  int in_stride = 1;
  int in_distance = 8;
  int out_storage_sizes[] = {2, 8};
  int out_stride = 1;
  int out_distance = 16;
  int bad_sizes[] = {2, 0};
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(NULL, dimension, dft_sizes, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, 0, dft_sizes, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, NULL, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, bad_sizes, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, 0,
                                     in_storage_sizes, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                     NULL, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                     bad_sizes, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                     in_storage_sizes, 0, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     NULL, out_stride, out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     bad_sizes, out_stride, out_distance,
                                     placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     out_storage_sizes, 0, out_distance,
                                     placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                     in_storage_sizes, in_stride, in_distance,
                                     out_storage_sizes, out_stride,
                                     out_distance, placement),
                 jomfftSuccess);

  Long dft_sizes_[] = {2, 4};
  Long in_sizes[] = {2, 2, 4};
  Long in_strides[] = {4, 1};
  Long out_sizes[] = {2, 2, 8};
  Long out_strides[] = {8, 1};

  const bool inplace = placement == jomfftInplace;
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *forward = NS(CDft_MainDft_new)(
      data, false, dimension, dft_sizes_, batch_size, in_strides, in_distance,
      out_strides, out_distance, inplace, 1);
  struct NS(ComplexFunction) *backward = NS(CDft_MainDft_new)(
      data, true, dimension, dft_sizes_, batch_size, out_strides, out_distance,
      in_strides, in_distance, inplace, 1);
  struct NS(ComplexTensor) *x0 = NS(Random_ComplexTensor_new)(3, in_sizes);
  struct NS(ComplexTensor) *x1 = NS(ComplexTensor_copy)(x0);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new)(3, out_sizes);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);

  ASSERT_ENUM_EQ(NS(ComplexDftForward)(handle, x0->data, y0->data),
                 jomfftSuccess);
  NS(ComplexFunction_evaluate)(forward, x1->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);

  ASSERT_ENUM_EQ(NS(ComplexDftBackward)(handle, y0->data, x0->data),
                 jomfftSuccess);
  NS(ComplexFunction_evaluate)(backward, y1->data, x1->data);
  ASSERT_COMPLEXTENSOR_AEQ(x0, x1);

  ASSERT_ENUM_EQ(NS(ComplexDftDestruct)(handle), jomfftSuccess);
  NS(DataFactory_delete)(data);
  NS(ComplexFunction_delete)(forward);
  NS(ComplexFunction_delete)(backward);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(x1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(ComplexDftBatch_Test)() {
  NS(ComplexDftHandle) handle = 0;
  jomfftPlacement placement = jomfftNotInplace;
  int dimension = 2;
  Long batch_size = 2;
  Long in_distance = 8;
  Long out_distance = 16;
  Long dft_sizes[] = {2, 4};
  Long in_strides[] = {4, 1};
  Long out_strides[] = {8, 2};
  Long bad_sizes[] = {0, 4};
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(NULL, dimension, dft_sizes, batch_size,
                                      in_strides, in_distance, out_strides,
                                      out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(&handle, 0, dft_sizes, batch_size,
                                      in_strides, in_distance, out_strides,
                                      out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(&handle, dimension, NULL, batch_size,
                                      in_strides, in_distance, out_strides,
                                      out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(&handle, dimension, bad_sizes, batch_size,
                                      in_strides, in_distance, out_strides,
                                      out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(&handle, dimension, dft_sizes, 0,
                                      in_strides, in_distance, out_strides,
                                      out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(&handle, dimension, dft_sizes, batch_size,
                                      NULL, in_distance, out_strides,
                                      out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(&handle, dimension, dft_sizes, batch_size,
                                      in_strides, in_distance, NULL,
                                      out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(ComplexDftBatch_)(&handle, dimension, dft_sizes, batch_size,
                                      in_strides, in_distance, out_strides,
                                      out_distance, placement),
                 jomfftSuccess);

  Long in_sizes[] = {2, 2, 4};
  Long out_sizes[] = {2, 2, 8};

  const bool inplace = placement == jomfftInplace;
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *forward = NS(CDft_MainDft_new)(
      data, false, dimension, dft_sizes, batch_size, in_strides, in_distance,
      out_strides, out_distance, inplace, 1);
  struct NS(ComplexFunction) *backward = NS(CDft_MainDft_new)(
      data, true, dimension, dft_sizes, batch_size, out_strides, out_distance,
      in_strides, in_distance, inplace, 1);
  struct NS(ComplexTensor) *x0 = NS(Random_ComplexTensor_new)(3, in_sizes);
  struct NS(ComplexTensor) *x1 = NS(ComplexTensor_copy)(x0);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new)(3, out_sizes);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);

  ASSERT_ENUM_EQ(NS(ComplexDftForward)(handle, x0->data, y0->data),
                 jomfftSuccess);
  NS(ComplexFunction_evaluate)(forward, x1->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);

  ASSERT_ENUM_EQ(NS(ComplexDftBackward)(handle, y0->data, x0->data),
                 jomfftSuccess);
  NS(ComplexFunction_evaluate)(backward, y1->data, x1->data);
  ASSERT_COMPLEXTENSOR_AEQ(x0, x1);

  ASSERT_ENUM_EQ(NS(ComplexDftDestruct)(handle), jomfftSuccess);
  NS(DataFactory_delete)(data);
  NS(ComplexFunction_delete)(forward);
  NS(ComplexFunction_delete)(backward);
  NS(ComplexTensor_delete)(x0);
  NS(ComplexTensor_delete)(x1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(RealDftTest)() {
  NS(RealDftHandle) handle = 0;
  jomfftPlacement placement = jomfftInplace;
  int dimension = 2;
  int dft_sizes[] = {4, 8};
  int bad_sizes[] = {4, 1};

  ASSERT_ENUM_EQ(NS(RealDft)(NULL, dimension, dft_sizes, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDft)(&handle, 0, dft_sizes, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDft)(&handle, dimension, NULL, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDft)(&handle, dimension, bad_sizes, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(
      NS(RealDft)(&handle, dimension, dft_sizes, (jomfftPlacement)-1),
      jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDft)(&handle, dimension, dft_sizes, placement),
                 jomfftSuccess);

  Long dft_sizes_[] = {4, 8};
  Long in_sizes[] = {4, 10};
  Long in_strides[] = {10, 1};
  Long out_sizes[] = {4, 5};
  Long out_strides[] = {5, 1};

  const bool inplace = placement == jomfftInplace;
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(RealToComplexFunction) *forward =
      NS(RDft_MainDft_F_new)(data, dimension, dft_sizes_, 1, in_strides, 0,
                             out_strides, 0, inplace, 1);
  struct NS(ComplexToRealFunction) *backward =
      NS(RDft_MainDft_B_new)(data, dimension, dft_sizes_, 1, out_strides, 0,
                             in_strides, 0, inplace, 1);
  struct NS(RealTensor) *x = NS(Random_RealTensor_new)(2, in_sizes);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new)(2, out_sizes);
  NS(RealTensor_CopyAsComplex)(x->shape, x, y0);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_copy)(y0);

  ASSERT_ENUM_EQ(NS(RealDftForward)(handle, (Real *)y0->data, y0->data),
                 jomfftSuccess);
  NS(RealToComplexFunction_evaluate)(forward, (Real *)y1->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);

  ASSERT_ENUM_EQ(NS(RealDftBackward)(handle, y0->data, (Real *)y0->data),
                 jomfftSuccess);
  NS(ComplexToRealFunction_evaluate)(backward, y1->data, (Real *)y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);

  ASSERT_ENUM_EQ(NS(RealDftDestruct)(handle), jomfftSuccess);
  NS(DataFactory_delete)(data);
  NS(RealToComplexFunction_delete)(forward);
  NS(ComplexToRealFunction_delete)(backward);
  NS(RealTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(RealDftBatchTest)() {
  NS(RealDftHandle) handle = 0;
  jomfftPlacement placement = jomfftNotInplace;
  int dimension = 2;
  int dft_sizes[] = {2, 4};
  int batch_size = 2;
  int in_storage_sizes[] = {2, 4};
  int in_stride = 1;
  int in_distance = 8;
  int out_storage_sizes[] = {2, 6};
  int out_stride = 1;
  int out_distance = 12;
  int bad_sizes[] = {2, 0};
  ASSERT_ENUM_EQ(NS(RealDftBatch)(NULL, dimension, dft_sizes, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, 0, dft_sizes, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, NULL, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, bad_sizes, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, 0,
                                  in_storage_sizes, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                  NULL, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                  bad_sizes, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                  in_storage_sizes, 0, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  NULL, out_stride, out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  bad_sizes, out_stride, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  out_storage_sizes, 0, out_distance,
                                  placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch)(&handle, dimension, dft_sizes, batch_size,
                                  in_storage_sizes, in_stride, in_distance,
                                  out_storage_sizes, out_stride, out_distance,
                                  placement),
                 jomfftSuccess);

  Long dft_sizes_[] = {2, 4};
  Long in_sizes[] = {2, 2, 4};
  Long in_strides[] = {4, 1};
  Long out_sizes[] = {2, 2, 6};
  Long out_strides[] = {6, 1};

  const bool inplace = placement == jomfftInplace;
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(RealToComplexFunction) *forward = NS(RDft_MainDft_F_new)(
      data, dimension, dft_sizes_, batch_size, in_strides, in_distance,
      out_strides, out_distance, inplace, 1);
  struct NS(ComplexToRealFunction) *backward = NS(RDft_MainDft_B_new)(
      data, dimension, dft_sizes_, batch_size, out_strides, out_distance,
      in_strides, in_distance, inplace, 1);
  struct NS(RealTensor) *x0 = NS(Random_RealTensor_new)(3, in_sizes);
  struct NS(RealTensor) *x1 = NS(RealTensor_copy)(x0);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new)(3, out_sizes);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);

  ASSERT_ENUM_EQ(NS(RealDftForward)(handle, x0->data, y0->data), jomfftSuccess);
  NS(RealToComplexFunction_evaluate)(forward, x1->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);

  ASSERT_ENUM_EQ(NS(RealDftBackward)(handle, y0->data, x0->data),
                 jomfftSuccess);
  NS(ComplexToRealFunction_evaluate)(backward, y1->data, x1->data);
  ASSERT_REALTENSOR_AEQ(x0, x1);

  ASSERT_ENUM_EQ(NS(RealDftDestruct)(handle), jomfftSuccess);
  NS(DataFactory_delete)(data);
  NS(RealToComplexFunction_delete)(forward);
  NS(ComplexToRealFunction_delete)(backward);
  NS(RealTensor_delete)(x0);
  NS(RealTensor_delete)(x1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(RealDftBatch_Test)() {
  NS(RealDftHandle) handle = 0;
  jomfftPlacement placement = jomfftNotInplace;
  int dimension = 2;
  Long batch_size = 2;
  Long in_distance = 8;
  Long out_distance = 6;
  Long dft_sizes[] = {2, 4};
  Long in_strides[] = {4, 1};
  Long out_strides[] = {3, 1};
  Long bad_sizes[] = {0, 4};
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(NULL, dimension, dft_sizes, batch_size,
                                   in_strides, in_distance, out_strides,
                                   out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(&handle, 0, dft_sizes, batch_size,
                                   in_strides, in_distance, out_strides,
                                   out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(&handle, dimension, NULL, batch_size,
                                   in_strides, in_distance, out_strides,
                                   out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(&handle, dimension, bad_sizes, batch_size,
                                   in_strides, in_distance, out_strides,
                                   out_distance, placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(&handle, dimension, dft_sizes, 0, in_strides,
                                   in_distance, out_strides, out_distance,
                                   placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(&handle, dimension, dft_sizes, batch_size,
                                   NULL, in_distance, out_strides, out_distance,
                                   placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(&handle, dimension, dft_sizes, batch_size,
                                   in_strides, in_distance, NULL, out_distance,
                                   placement),
                 jomfftInvalidArgument);
  ASSERT_ENUM_EQ(NS(RealDftBatch_)(&handle, dimension, dft_sizes, batch_size,
                                   in_strides, in_distance, out_strides,
                                   out_distance, placement),
                 jomfftSuccess);

  Long in_sizes[] = {2, 2, 4};
  Long out_sizes[] = {2, 2, 3};

  const bool inplace = placement == jomfftInplace;
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(RealToComplexFunction) *forward = NS(RDft_MainDft_F_new)(
      data, dimension, dft_sizes, batch_size, in_strides, in_distance,
      out_strides, out_distance, inplace, 1);
  struct NS(ComplexToRealFunction) *backward = NS(RDft_MainDft_B_new)(
      data, dimension, dft_sizes, batch_size, out_strides, out_distance,
      in_strides, in_distance, inplace, 1);
  struct NS(RealTensor) *x0 = NS(Random_RealTensor_new)(3, in_sizes);
  struct NS(RealTensor) *x1 = NS(RealTensor_copy)(x0);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new)(3, out_sizes);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);

  ASSERT_ENUM_EQ(NS(RealDftForward)(handle, x0->data, y0->data), jomfftSuccess);
  NS(RealToComplexFunction_evaluate)(forward, x1->data, y1->data);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);

  ASSERT_ENUM_EQ(NS(RealDftBackward)(handle, y0->data, x0->data),
                 jomfftSuccess);
  NS(ComplexToRealFunction_evaluate)(backward, y1->data, x1->data);
  ASSERT_REALTENSOR_AEQ(x0, x1);

  ASSERT_ENUM_EQ(NS(RealDftDestruct)(handle), jomfftSuccess);
  NS(DataFactory_delete)(data);
  NS(RealToComplexFunction_delete)(forward);
  NS(ComplexToRealFunction_delete)(backward);
  NS(RealTensor_delete)(x0);
  NS(RealTensor_delete)(x1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
