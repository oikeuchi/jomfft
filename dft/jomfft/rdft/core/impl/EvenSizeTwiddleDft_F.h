#pragma once

#include "../Core.h"

#if C_SIMD_SIZE >= 1
void NS(RDftCore_EvenSizeTwiddleDft_F_1)(Long batch_size, const Complex *w,
                                         Complex *x0, Complex *x1, Complex *y0,
                                         Complex *y1);
#endif
#if C_SIMD_SIZE >= 2
void NS(RDftCore_EvenSizeTwiddleDft_F_2)(Long batch_size, const Complex *w,
                                         Complex *x0, Complex *x1, Complex *y0,
                                         Complex *y1);
#endif
#if C_SIMD_SIZE >= 4
void NS(RDftCore_EvenSizeTwiddleDft_F_4)(Long batch_size, const Complex *w,
                                         Complex *x0, Complex *x1, Complex *y0,
                                         Complex *y1);
#endif
#if C_SIMD_SIZE >= 8
void NS(RDftCore_EvenSizeTwiddleDft_F_8)(Long batch_size, const Complex *w,
                                         Complex *x0, Complex *x1, Complex *y0,
                                         Complex *y1);
#endif
