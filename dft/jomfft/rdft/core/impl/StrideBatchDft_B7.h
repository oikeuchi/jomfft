#pragma once

#include "../Core.h"

#if R_SIMD_SIZE_S >= 1
void NS(RDftCore_StrideBatchDft_B7_1)(Long batch_size, Complex *x,
                                      Long x_stride_0, Long x_stride_1, Real *y,
                                      Long y_stride_0, Long y_stride_1);
#endif
#if R_SIMD_SIZE_S >= 2
void NS(RDftCore_StrideBatchDft_B7_2)(Long batch_size, Complex *x,
                                      Long x_stride_0, Long x_stride_1, Real *y,
                                      Long y_stride_0, Long y_stride_1);
#endif
#if R_SIMD_SIZE_S >= 4
void NS(RDftCore_StrideBatchDft_B7_4)(Long batch_size, Complex *x,
                                      Long x_stride_0, Long x_stride_1, Real *y,
                                      Long y_stride_0, Long y_stride_1);
#endif
#if R_SIMD_SIZE_S >= 8
void NS(RDftCore_StrideBatchDft_B7_8)(Long batch_size, Complex *x,
                                      Long x_stride_0, Long x_stride_1, Real *y,
                                      Long y_stride_0, Long y_stride_1);
#endif
