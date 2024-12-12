#pragma once

#include "../Singleton.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftSingleton_StrideBatchDft_F0_1)(int dft_size, const Complex *w,
                                           Long batch_size, Complex *x,
                                           Long x_stride_0, Long x_stride_1,
                                           Complex *y, Long y_stride_0,
                                           Long y_stride_1);
#endif
#if C_SIMD_SIZE_S >= 2
void NS(CDftSingleton_StrideBatchDft_F0_2)(int dft_size, const Complex *w,
                                           Long batch_size, Complex *x,
                                           Long x_stride_0, Long x_stride_1,
                                           Complex *y, Long y_stride_0,
                                           Long y_stride_1);
#endif
#if C_SIMD_SIZE_S >= 4
void NS(CDftSingleton_StrideBatchDft_F0_4)(int dft_size, const Complex *w,
                                           Long batch_size, Complex *x,
                                           Long x_stride_0, Long x_stride_1,
                                           Complex *y, Long y_stride_0,
                                           Long y_stride_1);
#endif
