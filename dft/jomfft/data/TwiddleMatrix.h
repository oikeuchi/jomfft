#pragma once

#include "Data.h"

struct NS(TwiddleMatrixKey) {
  Long size_0;
  Long size_1;
  Long twiddle_size;
  Long first_index_0;
  Long first_index_1;
};

bool NS(TwiddleMatrixKey_equal)(struct NS(TwiddleMatrixKey) a,
                                struct NS(TwiddleMatrixKey) b);

Complex *NS(TwiddleMatrix_new)(struct NS(TwiddleMatrixKey) key);

void NS(TwiddleMatrix_delete)(Complex *p);
