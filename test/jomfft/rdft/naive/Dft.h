#pragma once

#include "Naive.h"

void NS(RDftNaive_Dft_F)(Long dft_size, const Real *x, Long x_stride,
                         Complex *y, Long y_stride);

void NS(RDftNaive_Dft_B)(Long dft_size, const Complex *x, Long x_stride,
                         Real *y, Long y_stride);
