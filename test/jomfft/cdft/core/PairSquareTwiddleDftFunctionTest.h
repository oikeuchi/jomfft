#pragma once

#include "../../Test.h"

typedef void (*NS(CDftCore_PairSquareTwiddleDft_fn))(const Complex *,
                                                     const Complex *, Complex *,
                                                     Complex *);

void NS(CDftCore_PairSquareTwiddleDftFunctionTest)(
    NS(CDftCore_PairSquareTwiddleDft_fn) func, bool backward, int dft_size);
