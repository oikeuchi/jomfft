#pragma once

#include "Naive.h"

void NS(CDftNaive_BatchDft)(bool backward, Long dft_size, Long batch_size,
                            const Complex *x, Long x_stride_0, Long x_stride_1,
                            Complex *y, Long y_stride_0, Long y_stride_1);
