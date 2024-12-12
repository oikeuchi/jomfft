#pragma once

#include "../../Test.h"

typedef void (*NS(CDftSingleton_InnerBatchDft_fn))(int, const Complex *, Long,
                                                   Complex *, Long, Complex *,
                                                   Long);

void NS(CDftSingleton_InnerBatchDftFunctionTest)(
    NS(CDftSingleton_InnerBatchDft_fn) func, bool backward, int dft_size,
    Long batch_size);
