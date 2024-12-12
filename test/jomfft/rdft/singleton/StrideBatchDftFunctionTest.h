#pragma once

#include "../../Test.h"

typedef void (*NS(RDftSingleton_StrideBatchDft_F_fn))(int, const Complex *,
                                                      Long, Real *, Long, Long,
                                                      Complex *, Long, Long);

typedef void (*NS(RDftSingleton_StrideBatchDft_B_fn))(int, const Complex *,
                                                      Long, Complex *, Long,
                                                      Long, Real *, Long, Long);

void NS(RDftSingleton_StrideBatchDftFunction_FTest)(
    NS(RDftSingleton_StrideBatchDft_F_fn) func, int dft_size, Long batch_size);

void NS(RDftSingleton_StrideBatchDftFunction_BTest)(
    NS(RDftSingleton_StrideBatchDft_B_fn) func, int dft_size, Long batch_size);
