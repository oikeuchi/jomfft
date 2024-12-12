#pragma once

#include "ComplexFunction.h"

struct NS(ComplexFunction) *NS(Complex_Transpose_new)(Long size, Long stride);

struct NS(ComplexFunction) *NS(Complex_Transpose_new_)(Long size, Long stride,
                                                       int num_threads);
