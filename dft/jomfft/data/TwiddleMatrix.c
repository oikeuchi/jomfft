#include "TwiddleMatrix.h"

#include "../math/Math.h"

bool NS(TwiddleMatrixKey_equal)(struct NS(TwiddleMatrixKey) a,
                                struct NS(TwiddleMatrixKey) b) {
  return a.size_0 == b.size_0 && a.size_1 == b.size_1 &&
         a.twiddle_size == b.twiddle_size &&
         a.first_index_0 == b.first_index_0 &&
         a.first_index_1 == b.first_index_1;
}

Complex *NS(TwiddleMatrix_new)(struct NS(TwiddleMatrixKey) key) {
  assert(key.size_0 >= 0);
  assert(key.size_1 >= 0);
  assert(key.twiddle_size >= 0);
  const Long size_0 = key.size_0;
  const Long size_1 = key.size_1;
  Complex *data = NULL;
  const Long data_size = size_0 * size_1;
  if (data_size) {
    data = ARRAY_NEW(Complex, data_size);
    const Real theta = ((Real)(-2 * M_PI)) / key.twiddle_size;
    const Long first_index_0 = key.first_index_0;
    const Long first_index_1 = key.first_index_1;
    Long i, j;
#if USE_OPENMP
#pragma omp parallel for collapse(2) num_threads(jomfftGetMaxThreads())
#endif
    for (i = 0; i < size_0; ++i) {
      for (j = 0; j < size_1; ++j) {
        const Long i_ = i + first_index_0;
        const Long j_ = j + first_index_1;
        data[size_1 * i + j] = NS(Complex_fromArgument)(theta * (i_ * j_));
      }
    }
  }
  return data;
}

void NS(TwiddleMatrix_delete)(Complex *p) {
  if (p) {
    ARRAY_DELETE(p);
  }
}
