#pragma once

#include "../Core.h"

#if C_SIMD_SIZE_S >= 1
void NS(CDftCore_StrideBatchDft_F4_1)(Long batch_size, Complex *x,
                                      Long x_stride_0, Long x_stride_1,
                                      Complex *y, Long y_stride_0,
                                      Long y_stride_1);
#endif
#if C_SIMD_SIZE_S >= 2
void NS(CDftCore_StrideBatchDft_F4_2)(Long batch_size, Complex *x,
                                      Long x_stride_0, Long x_stride_1,
                                      Complex *y, Long y_stride_0,
                                      Long y_stride_1);
#endif
#if C_SIMD_SIZE_S >= 4
void NS(CDftCore_StrideBatchDft_F4_4)(Long batch_size, Complex *x,
                                      Long x_stride_0, Long x_stride_1,
                                      Complex *y, Long y_stride_0,
                                      Long y_stride_1);
#endif