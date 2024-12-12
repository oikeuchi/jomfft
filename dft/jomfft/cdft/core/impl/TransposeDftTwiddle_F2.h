#pragma once

#include "../Core.h"

void NS(CDftCore_TransposeDftTwiddle_F2)(Long batch_size, const Complex *w,
                                         Complex *x, Long x_stride_0,
                                         Long x_stride_1, Complex *y,
                                         Long y_stride);

#if C_SIMD_SIZE >= 4
void NS(CDftCore_TransposeDftTwiddle_F2_4)(Long batch_size, const Complex *w,
                                           Complex *x, Long x_stride_0,
                                           Long x_stride_1, Complex *y,
                                           Long y_stride);
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftCore_TransposeDftTwiddle_F2_8)(Long batch_size, const Complex *w,
                                           Complex *x, Long x_stride_0,
                                           Long x_stride_1, Complex *y,
                                           Long y_stride);
#endif
