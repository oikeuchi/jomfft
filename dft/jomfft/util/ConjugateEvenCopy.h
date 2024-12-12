#pragma once

#include "Util.h"

void NS(Complex_CopyCE)(Long size, const Complex *x, Long x_stride, Complex *y,
                        Long y_stride);

void NS(Complex_CopyCE_)(Long size, const Complex *x, Long x_stride, Complex *y,
                         Long y_stride, int num_threads);
