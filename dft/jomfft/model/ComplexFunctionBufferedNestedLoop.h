#pragma once

#include "ComplexFunction.h"
#include "ComplexFunctionW.h"
#include "Model.h"

struct NS(ComplexFunction) *NS(ComplexFunctionBufferedNestedLoop_new)(
    struct NS(ComplexFunctionW) *func, Long batch_size_0, Long batch_size_1,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads);
