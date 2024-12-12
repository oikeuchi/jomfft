#pragma once

#include "../../model/ComplexFunction.h"
#include "Core.h"

struct NS(ComplexFunction) *NS(CDftCore_BatchMultiply_new)(
    bool backward, Long batch_size, const Complex *w, Long x_stride,
    Long y_stride, int num_threads);
