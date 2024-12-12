#include <jomfft/data/TwiddleVector.h>
#include <jomfft/math/Math.h>
#include <jomfft/util/SplitComplexCopy.h>
#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "ComplexToPairDftFunctionTest.h"

#include "../../cdft/naive/Dft.h"
#include "../naive/BatchDft.h"

typedef void (*NS(RDftCore_ComplexToPairDft_fn))(Long, Complex *, Complex *,
                                                 Complex *, Complex *, Long);

void NS(RDftCore_ComplexToPairDftFunction_FTest)(
    NS(RDftCore_ComplexToPairDft_fn) func, Long dft_size) {
  CAPTURE_LONG(dft_size);
  const Long y_size = CE_SIZE(dft_size);
  struct NS(RealTensor) *x = NS(Random_RealTensor_new2)(2, dft_size);
  struct NS(ComplexTensor) *z0 = NS(ComplexTensor_new1)(dft_size);
  struct NS(ComplexTensor) *z1 = NS(ComplexTensor_new1)(dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new2)(2, y_size);
  NS(SplitComplex_Copy_F)(dft_size, x->data, NS(RealTensor_at2)(x, 1, 0), 1,
                          z0->data, 1);
  NS(CDftNaive_Dft)(false, dft_size, z0->data, 1, z1->data, 1);
  NS(ComplexTensor_set2)(y0, 0, 0, Complex_R(C_REAL(z1->data[0])));
  NS(ComplexTensor_set2)(y0, 1, 0, Complex_R(C_IMAG(z1->data[0])));
  func(dft_size / 2, z1->data + 1, z1->data + dft_size,
       NS(ComplexTensor_at2)(y0, 0, 1), NS(ComplexTensor_at2)(y0, 1, 1), 1);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_F)(dft_size, 2, x->data, 1, dft_size, y1->data, 1,
                           y_size);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(z0);
  NS(ComplexTensor_delete)(z1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(RDftCore_ComplexToPairDftFunction_BTest)(
    NS(RDftCore_ComplexToPairDft_fn) func, Long dft_size) {
  CAPTURE_LONG(dft_size);
  const Long x_size = CE_SIZE(dft_size);
  struct NS(ComplexTensor) *x =
      NS(Random_ConjugateEvenTensor_new2)(2, dft_size);
  struct NS(ComplexTensor) *z0 = NS(ComplexTensor_new1)(dft_size);
  struct NS(ComplexTensor) *z1 = NS(ComplexTensor_new1)(dft_size);
  struct NS(RealTensor) *y0 = NS(Random_RealTensor_new2)(2, dft_size);
  z0->data[0] = Complex_(C_REAL(NS(ComplexTensor_get2(x, 0, 0))),
                         C_REAL(NS(ComplexTensor_get2(x, 1, 0))));
  func(dft_size / 2, NS(ComplexTensor_at2)(x, 0, 1),
       NS(ComplexTensor_at2)(x, 1, 1), z0->data + 1, z0->data + dft_size, 1);
  NS(CDftNaive_Dft)(true, dft_size, z0->data, 1, z1->data, 1);
  NS(SplitComplex_Copy_B)(dft_size, z1->data, 1, y0->data,
                          NS(RealTensor_at2)(y0, 1, 0), 1);
  struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
  NS(RDftNaive_BatchDft_B)(dft_size, 2, x->data, 1, x_size, y1->data, 1,
                           dft_size);
  ASSERT_REALTENSOR_AEQ(y0, y1);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(z0);
  NS(ComplexTensor_delete)(z1);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
