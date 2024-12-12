#pragma once

#include "../Core.h"

#if C_SIMD_SIZE >= 1
void NS(CDftCore_TwiddleDft_F4_1)(Long batch_size, const Complex *w, Complex *x,
                                  Complex *y, Long stride);
#endif
#if C_SIMD_SIZE >= 2
void NS(CDftCore_TwiddleDft_F4_2)(Long batch_size, const Complex *w, Complex *x,
                                  Complex *y, Long stride);
#endif
#if C_SIMD_SIZE >= 4
void NS(CDftCore_TwiddleDft_F4_4)(Long batch_size, const Complex *w, Complex *x,
                                  Complex *y, Long stride);
#endif
#if C_SIMD_SIZE >= 8
void NS(CDftCore_TwiddleDft_F4_8)(Long batch_size, const Complex *w, Complex *x,
                                  Complex *y, Long stride);
#endif
