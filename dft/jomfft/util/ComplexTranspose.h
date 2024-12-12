#pragma once

#include "Util.h"

void NS(Complex_Transpose)(Long size, Complex *x, Long stride);

void NS(Complex_Transpose_)(Long size, Complex *x, Long stride,
                            int num_threads);
