#pragma once

#include "../Singleton.h"

#if C_SIMD_SIZE >= 1
void NS(CDftSingleton_InnerBatchDft_B0_1)(int dft_size, const Complex *w,
                                          Long batch_size, Complex *x,
                                          Long x_stride, Complex *y,
                                          Long y_stride);
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftSingleton_InnerBatchDft_B0_2)(int dft_size, const Complex *w,
                                          Long batch_size, Complex *x,
                                          Long x_stride, Complex *y,
                                          Long y_stride);
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftSingleton_InnerBatchDft_B0_4)(int dft_size, const Complex *w,
                                          Long batch_size, Complex *x,
                                          Long x_stride, Complex *y,
                                          Long y_stride);
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftSingleton_InnerBatchDft_B0_8)(int dft_size, const Complex *w,
                                          Long batch_size, Complex *x,
                                          Long x_stride, Complex *y,
                                          Long y_stride);
#endif
