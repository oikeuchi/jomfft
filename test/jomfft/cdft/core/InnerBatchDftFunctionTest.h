#pragma once

#include "../../Test.h"

typedef void (*NS(CDftCore_InnerBatchDft_fn))(Long, Complex *, Long, Complex *,
                                              Long);

void NS(CDftCore_InnerBatchDftFunctionTest)(NS(CDftCore_InnerBatchDft_fn) func,
                                            bool backward, int dft_size,
                                            Long batch_size);
