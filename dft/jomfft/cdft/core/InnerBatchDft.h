#pragma once

#include "../../model/ComplexFunction.h"
#include "Core.h"

bool NS(CDftCore_InnerBatchDft_HasKey)(int dft_size);

struct NS(ComplexFunction) *NS(CDftCore_InnerBatchDft_new)(
    bool backward, int dft_size, Long batch_size, Long x_stride, Long y_stride,
    int num_threads);

struct NS(ComplexFunction) *NS(CDftCore_NestedInnerBatchDft_new)(
    bool backward, int dft_size, Long batch_size_0, Long batch_size_1,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads);
