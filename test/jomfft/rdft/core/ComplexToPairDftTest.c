#include <jomfft/rdft/core/ComplexToPairDft.h>
#include <jomfft/util/SplitComplexCopy.h>
#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "../../cdft/naive/Dft.h"
#include "../naive/BatchDft.h"
#include "ComplexToPairDftTest.h"

static void RDftCore_ComplexToPairDft_FTest__(Long dft_size, int num_threads) {
  CAPTURE_LONG(dft_size);
  CAPTURE_INT(num_threads);
  const Long y_size = CE_SIZE(dft_size);
  struct NS(RealTensor) *x = NS(Random_RealTensor_new2)(2, dft_size);
  struct NS(ComplexTensor) *z0 = NS(ComplexTensor_new1)(dft_size);
  struct NS(ComplexTensor) *z1 = NS(ComplexTensor_new1)(dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(2, y_size);
  NS(SplitComplex_Copy_F)(dft_size, x->data, NS(RealTensor_at2)(x, 1, 0), 1,
                          z0->data, 1);
  NS(CDftNaive_Dft)(false, dft_size, z0->data, 1, z1->data, 1);
  struct NS(ComplexFunction) *func =
      NS(RDftCore_ComplexToPairDft_F_new)(dft_size, 1, y_size, num_threads);
  NS(ComplexFunction_evaluate)(func, z1->data, y0->data);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_F)(dft_size, 2, x->data, 1, dft_size, y1->data, 1,
                           y_size);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(func);
  NS(RealTensor_delete)(x);
  NS(ComplexTensor_delete)(z0);
  NS(ComplexTensor_delete)(z1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

static void RDftCore_ComplexToPairDft_FTest_(Long dft_size) {
  RDftCore_ComplexToPairDft_FTest__(dft_size, 1);
}

static void RDftCore_ComplexToPairDft_BTest__(Long dft_size, int num_threads) {
  CAPTURE_LONG(dft_size);
  CAPTURE_INT(num_threads);
  const Long x_size = CE_SIZE(dft_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ConjugateEvenTensor_new2)(2, dft_size);
  struct NS(ComplexTensor) *z0 = NS(ComplexTensor_new1)(dft_size);
  struct NS(ComplexTensor) *z1 = NS(ComplexTensor_new1)(dft_size);
  struct NS(RealTensor) *y0 = NS(RealTensor_new2)(2, dft_size);
  struct NS(ComplexFunction) *func =
      NS(RDftCore_ComplexToPairDft_B_new)(dft_size, 1, x_size, num_threads);
  NS(ComplexFunction_evaluate)(func, x->data, z0->data);
  NS(CDftNaive_Dft)(true, dft_size, z0->data, 1, z1->data, 1);
  NS(SplitComplex_Copy_B)(dft_size, z1->data, 1, y0->data,
                          NS(RealTensor_at2)(y0, 1, 0), 1);
  struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_B)(dft_size, 2, x->data, 1, x_size, y1->data, 1,
                           dft_size);
  ASSERT_REALTENSOR_AEQ(y0, y1);
  NS(ComplexFunction_delete)(func);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(z0);
  NS(ComplexTensor_delete)(z1);
  NS(RealTensor_delete)(y0);
  NS(RealTensor_delete)(y1);
}

static void RDftCore_ComplexToPairDft_BTest_(Long dft_size) {
  RDftCore_ComplexToPairDft_BTest__(dft_size, 1);
}

void NS(RDftCore_ComplexToPairDft_FTest)() {
  RDftCore_ComplexToPairDft_FTest_(3);
  RDftCore_ComplexToPairDft_FTest_(5);
  RDftCore_ComplexToPairDft_FTest_(6);
  RDftCore_ComplexToPairDft_FTest_(8);
  RDftCore_ComplexToPairDft_FTest_(9);
  RDftCore_ComplexToPairDft_FTest_(11);
  RDftCore_ComplexToPairDft_FTest_(12);
  RDftCore_ComplexToPairDft_FTest_(14);
  RDftCore_ComplexToPairDft_FTest_(16);
  RDftCore_ComplexToPairDft_FTest_(20);
#if USE_OPENMP
  RDftCore_ComplexToPairDft_FTest__(20, 2);
  RDftCore_ComplexToPairDft_FTest__(55, omp_get_max_threads());
#endif
}

void NS(RDftCore_ComplexToPairDft_BTest)() {
  RDftCore_ComplexToPairDft_BTest_(3);
  RDftCore_ComplexToPairDft_BTest_(5);
  RDftCore_ComplexToPairDft_BTest_(6);
  RDftCore_ComplexToPairDft_BTest_(8);
  RDftCore_ComplexToPairDft_BTest_(9);
  RDftCore_ComplexToPairDft_BTest_(11);
  RDftCore_ComplexToPairDft_BTest_(12);
  RDftCore_ComplexToPairDft_BTest_(14);
  RDftCore_ComplexToPairDft_BTest_(16);
  RDftCore_ComplexToPairDft_BTest_(20);
#if USE_OPENMP
  RDftCore_ComplexToPairDft_BTest__(20, 2);
  RDftCore_ComplexToPairDft_BTest__(55, omp_get_max_threads());
#endif
}
