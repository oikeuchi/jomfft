#include "TwiddleVector.h"

#include "../math/Math.h"

bool NS(TwiddleVectorKey_equal)(struct NS(TwiddleVectorKey) a,
                                struct NS(TwiddleVectorKey) b) {
  return a.size == b.size && a.twiddle_size == b.twiddle_size &&
         a.first_index == b.first_index;
}

Complex *NS(TwiddleVector_new)(struct NS(TwiddleVectorKey) key) {
  assert(key.size >= 0);
  assert(key.twiddle_size >= 0);
  Complex *data = NULL;
  if (key.size) {
    const Long size = key.size;
    const Long first_index = key.first_index;
    data = ARRAY_NEW(Complex, size);
    const Real theta = ((Real)(-2 * M_PI)) / key.twiddle_size;
    Long i;
#if USE_OPENMP
#pragma omp parallel for num_threads(jomfftGetMaxThreads())
#endif
    for (i = 0; i < size; ++i) {
      data[i] = NS(Complex_fromArgument)(theta * (i + first_index));
    }
  }
  return data;
}

void NS(TwiddleVector_delete)(Complex *p) {
  if (p) {
    ARRAY_DELETE(p);
  }
}
