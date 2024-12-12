#include <jomfft/data/TwiddleVector.h>
#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>

#include "EvenSizeTwiddleDftFunctionTest.h"

#include "../../cdft/naive/Dft.h"
#include "../naive/Dft.h"

typedef void (*NS(RDftCore_EvenSizeTwiddleDft_fn))(Long, const Complex *,
                                                   Complex *, Complex *,
                                                   Complex *, Complex *);

void NS(RDftCore_EvenSizeTwiddleDftFunction_FTest)(
    NS(RDftCore_EvenSizeTwiddleDft_fn) func, Long batch_size) {
  CAPTURE_LONG(batch_size);
  const Long dft_size = 2 * batch_size + 1;
  struct NS(RealTensor) *x = NS(Random_RealTensor_new1)(2 * dft_size);
  struct NS(ComplexTensor) *y0 = NS(ComplexTensor_new1)(dft_size + 1);
  NS(CDftNaive_Dft)(false, dft_size, (Complex *)x->data, 1, y0->data, 1);
  {
    Complex z0 = NS(ComplexTensor_get1)(y0, 0);
    NS(ComplexTensor_set1)(y0, 0, Complex_R(C_REAL(z0) + C_IMAG(z0)));
    NS(ComplexTensor_set1)
    (y0, dft_size, Complex_R(C_REAL(z0) - C_IMAG(z0)));
  }
  struct NS(TwiddleVectorKey) w_key = {batch_size, 2 * dft_size, 1};
  Complex *w = NS(TwiddleVector_new)(w_key);
  func(batch_size, w, NS(ComplexTensor_at1)(y0, 1),
       NS(ComplexTensor_at1)(y0, dft_size), NS(ComplexTensor_at1)(y0, 1),
       NS(ComplexTensor_at1)(y0, dft_size));
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  NS(RDftNaive_Dft_F)(2 * dft_size, x->data, 1, y1->data, 1);
  ASSERT_COMPLEXTENSOR_AEQ(y0, y1);
  NS(TwiddleVector_delete)(w);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}

void NS(RDftCore_EvenSizeTwiddleDftFunction_BTest)(
    NS(RDftCore_EvenSizeTwiddleDft_fn) func, Long batch_size) {
  CAPTURE_LONG(batch_size);
  const Long dft_size = 2 * batch_size + 1;
  struct NS(ComplexTensor) *x =
      NS(Random_ConjugateEvenTensor_new1)(2 * dft_size);
  struct NS(RealTensor) *y0 = NS(RealTensor_new1)(2 * dft_size);
  struct NS(ComplexTensor) *z = NS(ComplexTensor_new1)(dft_size);
  {
    Real z0 = C_REAL(NS(ComplexTensor_get1)(x, 0));
    Real z1 = C_REAL(NS(ComplexTensor_get1)(x, dft_size));
    NS(ComplexTensor_set1)(z, 0, Complex_((z0 + z1), (z0 - z1)));
  }
  struct NS(TwiddleVectorKey) w_key = {batch_size, 2 * dft_size, 1};
  Complex *w = NS(TwiddleVector_new)(w_key);
  func(batch_size, w, NS(ComplexTensor_at1)(x, 1),
       NS(ComplexTensor_at1)(x, dft_size), NS(ComplexTensor_at1)(z, 1),
       NS(ComplexTensor_end)(z));
  NS(CDftNaive_Dft)(true, dft_size, z->data, 1, (Complex *)y0->data, 1);
  struct NS(RealTensor) *y1 = NS(RealTensor_zerosLike)(y0);
  NS(RDftNaive_Dft_B)(2 * dft_size, x->data, 1, y1->data, 1);
  ASSERT_REALTENSOR_AEQ(y0, y1);
  NS(TwiddleVector_delete)(w);
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(z);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
}
