#pragma once

#include "../Core.h"

void NS(CDftCore_SquareSplitTwiddleDft_F2)(Long batch_size, const Complex *v,
                                           const Complex *w, int w_stride,
                                           Complex *x, Complex *y, Long stride);

#if C_SIMD_SIZE >= 4
void NS(CDftCore_SquareSplitTwiddleDft_F2_4)(Long batch_size, const Complex *v,
                                             const Complex *w, int w_stride,
                                             Complex *x, Complex *y,
                                             Long stride);
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftCore_SquareSplitTwiddleDft_F2_8)(Long batch_size, const Complex *v,
                                             const Complex *w, int w_stride,
                                             Complex *x, Complex *y,
                                             Long stride);
#endif
