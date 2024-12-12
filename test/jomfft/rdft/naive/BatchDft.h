#pragma once

#include "Naive.h"

void NS(RDftNaive_BatchDft_F)(Long dft_size, Long batch_size, const Real *x,
                              Long x_stride_0, Long x_stride_1, Complex *y,
                              Long y_stride_0, Long y_stride_1);

void NS(RDftNaive_BatchDft_B)(Long dft_size, Long batch_size, const Complex *x,
                              Long x_stride_0, Long x_stride_1, Real *y,
                              Long y_stride_0, Long y_stride_1);
