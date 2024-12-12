#include "BluesteinDftData.h"

#include "../cdft/Dft.h"
#include "../math/Math.h"
#include "DataFactory.h"
#include <jomfft/math/PrimeFactors.h>

bool NS(BluesteinDftDataKey_equal)(struct NS(BluesteinDftDataKey) a,
                                   struct NS(BluesteinDftDataKey) b) {
  return a.dft_size == b.dft_size && a.convolution_size == b.convolution_size;
}

struct NS(BluesteinDftData) *NS(BluesteinDftData_new)(
    struct NS(DataFactory) *data, struct NS(BluesteinDftDataKey) key) {
  assert(key.dft_size >= 2);
  assert(jomfftPrimeFactors_CanFactor(key.convolution_size));
  const Long dft_size = key.dft_size;
  const Long convolution_size = key.convolution_size;
  Complex *a = ARRAY_NEW(Complex, dft_size);
  Complex *b = ARRAY_NEW(Complex, convolution_size);
  const Real v = ((Real)1) / convolution_size;
  const Real theta = -((Real)M_PI) / dft_size;
  a[0] = Complex_R(1);
  b[0] = Complex_R(v);
  Long i;
#if USE_OPENMP
#pragma omp parallel for num_threads(jomfftGetMaxThreads())
#endif
  for (i = 1; i < dft_size; ++i) {
    Complex z = NS(Complex_fromArgument)(theta * (i * i));
    a[i] = z;
    z = C_MUL_R(C_CONJ(z), v);
    b[i] = z;
    b[convolution_size - i] = z;
  }
  ARRAY_ZERO(Complex, convolution_size - (2 * dft_size - 1), b + dft_size);
  struct NS(ComplexFunction) *dft = NS(CDft_Dft_new)(
      data, false, convolution_size, 1, 1, true, jomfftGetMaxThreads());
  NS(ComplexFunction_evaluate)(dft, b, b);
  NS(ComplexFunction_delete)(dft);
  struct NS(BluesteinDftData) *out = NEW(struct NS(BluesteinDftData));
  *out = (struct NS(BluesteinDftData)){.a = a, .b = b};
  return out;
}

void NS(BluesteinDftData_delete)(struct NS(BluesteinDftData) *p) {
  if (p) {
    if (p->a) {
      ARRAY_DELETE((Complex *)p->a);
      p->a = NULL;
    }
    if (p->b) {
      ARRAY_DELETE((Complex *)p->b);
      p->b = NULL;
    }
    DELETE(p);
  }
}
