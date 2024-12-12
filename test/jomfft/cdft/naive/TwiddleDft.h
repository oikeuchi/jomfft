#pragma once

#include "Naive.h"

void NS(CDftNaive_TwiddleDftForward)(Long dft_size, Long twiddle_size,
                                     const Complex *x, Complex *y);

void NS(CDftNaive_TwiddleDftBackward)(Long dft_size, Long twiddle_size,
                                      const Complex *x, Complex *y);

void NS(CDftNaive_TwiddleDft)(bool backward, Long dft_size, Long twiddle_size,
                              const Complex *x, Complex *y);
