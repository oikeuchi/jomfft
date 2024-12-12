#pragma once

#include "../Core.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_BatchMultiply_B_1)(Long batch_size, const Complex *w,
                                    Complex *x, Long x_stride, Complex *y,
                                    Long y_stride);
#endif
#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_BatchMultiply_B_2)(Long batch_size, const Complex *w,
                                    Complex *x, Long x_stride, Complex *y,
                                    Long y_stride);
#endif
#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_BatchMultiply_B_4)(Long batch_size, const Complex *w,
                                    Complex *x, Long x_stride, Complex *y,
                                    Long y_stride);
#endif
