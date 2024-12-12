#pragma once

#include "../Singleton.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftSingleton_StrideBatchDft_F1_1)(int dft_size, const Complex *w,
                                           Long batch_size, Real *x,
                                           Long x_stride_0, Long x_stride_1,
                                           Complex *y, Long y_stride_0,
                                           Long y_stride_1);
#endif
#if R_SIMD_SIZE_S >= 2
void NS(RDftSingleton_StrideBatchDft_F1_2)(int dft_size, const Complex *w,
                                           Long batch_size, Real *x,
                                           Long x_stride_0, Long x_stride_1,
                                           Complex *y, Long y_stride_0,
                                           Long y_stride_1);
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftSingleton_StrideBatchDft_F1_4)(int dft_size, const Complex *w,
                                           Long batch_size, Real *x,
                                           Long x_stride_0, Long x_stride_1,
                                           Complex *y, Long y_stride_0,
                                           Long y_stride_1);
#endif
#if R_SIMD_SIZE_S >= 8
void NS(RDftSingleton_StrideBatchDft_F1_8)(int dft_size, const Complex *w,
                                           Long batch_size, Real *x,
                                           Long x_stride_0, Long x_stride_1,
                                           Complex *y, Long y_stride_0,
                                           Long y_stride_1);
#endif
