#pragma once

#include "../../Test.h"

typedef void (*NS(CDftCore_TwiddleDft_fn))(Long, const Complex *, Complex *,
                                           Complex *, Long);

void NS(CDftCore_TwiddleDftFunctionTest)(NS(CDftCore_TwiddleDft_fn) func,
                                         bool backward, int dft_size,
                                         Long twiddle_size);
