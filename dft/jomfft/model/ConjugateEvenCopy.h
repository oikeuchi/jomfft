#pragma once

#include "ComplexFunction.h"
#include "Model.h"

struct NS(ComplexFunction) *NS(Complex_CopyCE_new)(Long size, Long x_stride,
                                                   Long y_stride,
                                                   int num_threads);
