#pragma once

#include "../Core.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_InnerBatchDft_F10_1)(Long batch_size, Complex *x,
                                      Long x_stride, Complex *y, Long y_stride);
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftCore_InnerBatchDft_F10_2)(Long batch_size, Complex *x,
                                      Long x_stride, Complex *y, Long y_stride);
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftCore_InnerBatchDft_F10_4)(Long batch_size, Complex *x,
                                      Long x_stride, Complex *y, Long y_stride);
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftCore_InnerBatchDft_F10_8)(Long batch_size, Complex *x,
                                      Long x_stride, Complex *y, Long y_stride);
#endif
