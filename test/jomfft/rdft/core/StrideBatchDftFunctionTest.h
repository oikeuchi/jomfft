#pragma once

#include "../../Test.h"

typedef void (*NS(RDftCore_StrideBatchDft_F_fn))(Long, Real *, Long, Long,
                                                 Complex *, Long, Long);

typedef void (*NS(RDftCore_StrideBatchDft_B_fn))(Long, Complex *, Long, Long,
                                                 Real *, Long, Long);

void NS(RDftCore_StrideBatchDftFunction_FTest)(
    NS(RDftCore_StrideBatchDft_F_fn) func, int dft_size, Long batch_size);

void NS(RDftCore_StrideBatchDftFunction_BTest)(
    NS(RDftCore_StrideBatchDft_B_fn) func, int dft_size, Long batch_size);
