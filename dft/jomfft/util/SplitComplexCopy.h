#pragma once

#include "Util.h"

void NS(SplitComplex_Copy_F)(Long size, const Real *x_r, const Real *x_i,
                             Long x_stride, Complex *y, Long y_stride);

void NS(SplitComplex_Copy_B)(Long size, const Complex *x, Long x_stride,
                             Real *y_r, Real *y_i, Long y_stride);

void NS(SplitComplex_CopyReal_F)(Long size, const Real *x_r, Long x_stride,
                                 Complex *y, Long y_stride);

void NS(SplitComplex_CopyReal_B)(Long size, const Complex *x, Long x_stride,
                                 Real *y_r, Long y_stride);
