#pragma once

#include "../../Test.h"

typedef void (*NS(RDftCore_EvenSizeTwiddleDft_fn))(Long, const Complex *,
                                                   Complex *, Complex *,
                                                   Complex *, Complex *);

void NS(RDftCore_EvenSizeTwiddleDftFunction_FTest)(
    NS(RDftCore_EvenSizeTwiddleDft_fn) func, Long batch_size);

void NS(RDftCore_EvenSizeTwiddleDftFunction_BTest)(
    NS(RDftCore_EvenSizeTwiddleDft_fn) func, Long batch_size);
