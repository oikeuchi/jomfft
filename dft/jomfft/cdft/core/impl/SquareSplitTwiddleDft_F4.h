#pragma once

#include "../Core.h"

void NS(CDftCore_SquareSplitTwiddleDft_F4)(Long batch_size, const Complex *v,
                                           const Complex *w, int w_stride,
                                           Complex *x, Complex *y, Long stride);
#if C_SIMD_SIZE >= 8
void NS(CDftCore_SquareSplitTwiddleDft_F4_8)(Long batch_size, const Complex *v,
                                             const Complex *w, int w_stride,
                                             Complex *x, Complex *y,
                                             Long stride);
#endif
