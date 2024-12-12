#pragma once

#include "../../Test.h"

typedef void (*NS(CDftCore_SquareSplitTwiddleDft_fn))(Long, const Complex *,
                                                      const Complex *, int,
                                                      Complex *, Complex *,
                                                      Long);

void NS(CDftCore_SquareSplitTwiddleDftFunctionTest)(
    NS(CDftCore_SquareSplitTwiddleDft_fn) func, bool backward, int dft_size,
    Long twiddle_size);

void NS(CDftCore_SquareSplitTwiddleDftFunctionTest_)(
    NS(CDftCore_SquareSplitTwiddleDft_fn) func, bool backward, int dft_size,
    Long twiddle_size, int vector_size);
