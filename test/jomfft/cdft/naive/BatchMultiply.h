#pragma once

#include "Naive.h"

void NS(CDftNaive_BatchMultiply)(bool backward, Long batch_size,
                                 const Complex *w, Long w_stride,
                                 const Complex *x, Long x_stride, Complex *y,
                                 Long y_stride);
