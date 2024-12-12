#pragma once

#include "../../Test.h"

typedef void (*NS(CDftCore_StrideBatchDft_fn))(Long, Complex *, Long, Long,
                                               Complex *, Long, Long);

void NS(CDftCore_StrideBatchDftFunctionTest)(
    NS(CDftCore_StrideBatchDft_fn) func, bool backward, int dft_size,
    Long batch_size);
