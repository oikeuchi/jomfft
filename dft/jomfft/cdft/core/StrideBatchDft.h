#pragma once

#include "../../model/ComplexFunction.h"
#include "Core.h"
#include <jomfft/util/IntVector.h>

bool NS(CDftCore_StrideBatchDft_HasKey)(int dft_size);

struct jomfftIntVector *NS(CDftCore_StrideBatchDft_Keys)();

struct NS(ComplexFunction) *NS(CDftCore_StrideBatchDft_new)(
    bool backward, int dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1);

struct NS(ComplexFunction) *NS(CDftCore_StrideBatchDft_new_)(
    bool backward, int dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads);
