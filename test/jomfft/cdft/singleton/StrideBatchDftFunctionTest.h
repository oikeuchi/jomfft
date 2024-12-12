#pragma once

#include "../../Test.h"

typedef void (*NS(CDftSingleton_StrideBatchDft_fn))(int, const Complex *, Long,
                                                    Complex *, Long, Long,
                                                    Complex *, Long, Long);

void NS(CDftSingleton_StrideBatchDftFunctionTest)(
    NS(CDftSingleton_StrideBatchDft_fn) func, bool backward, int dft_size,
    Long batch_size);
