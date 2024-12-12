#pragma once

#include "../../Test.h"

typedef void (*NS(CDftCore_TransposeDftTwiddle_fn))(Long, const Complex *,
                                                    Complex *, Long, Long,
                                                    Complex *, Long);

void NS(CDftCore_TransposeDftTwiddleFunctionTest)(
    NS(CDftCore_TransposeDftTwiddle_fn) func, bool backward, int dft_size,
    Long twiddle_size);
