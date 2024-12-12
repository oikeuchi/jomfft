#include <jomfft/rdft/core/EvenSizeTwiddleDft.h>
#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "../../cdft/naive/Dft.h"
#include "../naive/Dft.h"
#include "EvenSizeTwiddleDftTest.h"

static void RDftCore_EvenSizeTwiddleDft_FTest__(Long dft_size,
                                                int num_threads) {
  CAPTURE_LONG(dft_size);
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(RealTensor) *x = NS(Random_RealTensor_new1)(2 * dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new1)(dft_size + 1);
  NS(CDftNaive_Dft)(false, dft_size, (Complex *)x->data, 1, y0->data, 1);
  struct NS(ComplexFunction) *func =
      NS(RDftCore_EvenSizeTwiddleDft_F_new_)(data, dft_size, num_threads);
  NS(ComplexFunction_evaluate)(func, y0->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(RDftNaive_Dft_F)(2 * dft_size, x->data, 1, y1->data, 1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(func);
  NS(RealTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(DataFactory_delete)(data);
}

static void RDftCore_EvenSizeTwiddleDft_BTest__(Long dft_size,
                                                int num_threads) {
  CAPTURE_LONG(dft_size);
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexTensor) *x =
      NS(Random_ConjugateEvenTensor_new1)(2 * dft_size);
  struct NS(RealTensor) *y0 = NS(RealTensor_new1)(2 * dft_size);
  struct NS(ComplexTensor) *z = NS(ComplexTensor_new1)(dft_size);
  struct NS(ComplexFunction) *func =
      NS(RDftCore_EvenSizeTwiddleDft_B_new_)(data, dft_size, num_threads);
  NS(ComplexFunction_evaluate)(func, x->data, z->data);
  NS(CDftNaive_Dft)(true, dft_size, z->data, 1, (Complex *)y0->data, 1);
  struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
  NS(RDftNaive_Dft_B)(2 * dft_size, x->data, 1, y1->data, 1);
  ASSERT_REALTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(func);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(z);
  NS(RealTensor_delete)(y0);
  NS(RealTensor_delete)(y1);
  NS(DataFactory_delete)(data);
}

static void RDftCore_EvenSizeTwiddleDft_FTest_(Long dft_size) {
  RDftCore_EvenSizeTwiddleDft_FTest__(dft_size, 1);
}

static void RDftCore_EvenSizeTwiddleDft_BTest_(Long dft_size) {
  RDftCore_EvenSizeTwiddleDft_BTest__(dft_size, 1);
}

void NS(RDftCore_EvenSizeTwiddleDft_FTest)() {
  RDftCore_EvenSizeTwiddleDft_FTest_(2);
  RDftCore_EvenSizeTwiddleDft_FTest_(3);
  RDftCore_EvenSizeTwiddleDft_FTest_(5);
  RDftCore_EvenSizeTwiddleDft_FTest_(6);
  RDftCore_EvenSizeTwiddleDft_FTest_(8);
  RDftCore_EvenSizeTwiddleDft_FTest_(9);
  RDftCore_EvenSizeTwiddleDft_FTest_(11);
  RDftCore_EvenSizeTwiddleDft_FTest_(12);
  RDftCore_EvenSizeTwiddleDft_FTest_(14);
  RDftCore_EvenSizeTwiddleDft_FTest_(16);
  RDftCore_EvenSizeTwiddleDft_FTest_(20);
#if USE_OPENMP
  RDftCore_EvenSizeTwiddleDft_FTest__(33, 2);
  RDftCore_EvenSizeTwiddleDft_FTest__(48, 2);
#endif
}

void NS(RDftCore_EvenSizeTwiddleDft_BTest)() {
  RDftCore_EvenSizeTwiddleDft_BTest_(2);
  RDftCore_EvenSizeTwiddleDft_BTest_(3);
  RDftCore_EvenSizeTwiddleDft_BTest_(5);
  RDftCore_EvenSizeTwiddleDft_BTest_(6);
  RDftCore_EvenSizeTwiddleDft_BTest_(8);
  RDftCore_EvenSizeTwiddleDft_BTest_(9);
  RDftCore_EvenSizeTwiddleDft_BTest_(11);
  RDftCore_EvenSizeTwiddleDft_BTest_(12);
  RDftCore_EvenSizeTwiddleDft_BTest_(14);
  RDftCore_EvenSizeTwiddleDft_BTest_(16);
  RDftCore_EvenSizeTwiddleDft_BTest_(20);
#if USE_OPENMP
  RDftCore_EvenSizeTwiddleDft_BTest__(33, 2);
  RDftCore_EvenSizeTwiddleDft_BTest__(48, 2);
#endif
}
