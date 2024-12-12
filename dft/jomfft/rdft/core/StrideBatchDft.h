#pragma once

#include "../../model/ComplexToRealFunction.h"
#include "../../model/RealToComplexFunction.h"
#include "Core.h"

bool NS(RDftCore_StrideBatchDft_HasKey)(int dft_size);

struct NS(RealToComplexFunction) *NS(RDftCore_StrideBatchDft_F_new)(
    int dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDftCore_StrideBatchDft_B_new)(
    int dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, int num_threads);
