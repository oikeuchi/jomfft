#pragma once

#include "Data.h"

struct NS(TwiddleVectorKey) {
  Long size;
  Long twiddle_size;
  Long first_index;
};

bool NS(TwiddleVectorKey_equal)(struct NS(TwiddleVectorKey) a,
                                struct NS(TwiddleVectorKey) b);

Complex *NS(TwiddleVector_new)(struct NS(TwiddleVectorKey) key);

void NS(TwiddleVector_delete)(Complex *p);
