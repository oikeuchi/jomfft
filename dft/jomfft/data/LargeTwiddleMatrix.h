#pragma once

#include "Data.h"

struct NS(LargeTwiddleMatrixKey) {
  Long size;
  Long twiddle_size;
  int block_size;
};

bool NS(LargeTwiddleMatrixKey_equal)(struct NS(LargeTwiddleMatrixKey) a,
                                     struct NS(LargeTwiddleMatrixKey) b);

Complex *NS(LargeTwiddleMatrix_new)(struct NS(LargeTwiddleMatrixKey) key);

void NS(LargeTwiddleMatrix_delete)(Complex *p);
