#pragma once

#include "../Core.h"

void NS(CDftCore_SquareSplitTwiddleDft_B4)(Long batch_size, const Complex *v,
                                           const Complex *w, int w_stride,
                                           Complex *x, Complex *y, Long stride);
#if C_SIMD_SIZE >= 8
void NS(CDftCore_SquareSplitTwiddleDft_B4_8)(Long batch_size, const Complex *v,
                                             const Complex *w, int w_stride,
                                             Complex *x, Complex *y,
                                             Long stride);
#endif
