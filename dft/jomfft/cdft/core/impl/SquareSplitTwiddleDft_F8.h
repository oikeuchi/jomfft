#pragma once

#include "../Core.h"

void NS(CDftCore_SquareSplitTwiddleDft_F8)(Long batch_size, const Complex *v,
                                           const Complex *w, int w_stride,
                                           Complex *x, Complex *y, Long stride);
