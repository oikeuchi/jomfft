#pragma once

#include "Naive.h"

void NS(CDftNaive_Dft)(bool backward, Long dft_size, const Complex *x,
                       Long x_stride, Complex *y, Long y_stride);

void NS(CDftNaive_Dft_)(bool backward, Long dft_size, Complex *x,
                        Long x_stride);
