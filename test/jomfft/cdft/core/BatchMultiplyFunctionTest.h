#pragma once

#include "../../Test.h"

typedef void (*NS(CDftCore_BatchMultiply_fn))(Long, const Complex *, Complex *,
                                              Long, Complex *, Long);

void NS(CDftCore_BatchMultiplyFunctionTest)(NS(CDftCore_BatchMultiply_fn) func,
                                            bool backward, Long batch_size);
