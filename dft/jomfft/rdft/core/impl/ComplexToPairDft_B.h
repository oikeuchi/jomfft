#pragma once

#include "../Core.h"

#if C_SIMD_SIZE_S >= 1
void NS(RDftCore_ComplexToPairDft_B_1)(Long batch_size, Complex *x0,
                                       Complex *x1, Complex *y0, Complex *y1,
                                       Long stride);
#endif
#if C_SIMD_SIZE_S >= 2
void NS(RDftCore_ComplexToPairDft_B_2)(Long batch_size, Complex *x0,
                                       Complex *x1, Complex *y0, Complex *y1,
                                       Long stride);
#endif
#if C_SIMD_SIZE_S >= 4
void NS(RDftCore_ComplexToPairDft_B_4)(Long batch_size, Complex *x0,
                                       Complex *x1, Complex *y0, Complex *y1,
                                       Long stride);
#endif
