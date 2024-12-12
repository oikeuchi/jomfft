#pragma once

#include "../../Test.h"

typedef void (*NS(RDftCore_ComplexToPairDft_fn))(Long, Complex *, Complex *,
                                                 Complex *, Complex *, Long);

void NS(RDftCore_ComplexToPairDftFunction_FTest)(
    NS(RDftCore_ComplexToPairDft_fn) func, Long dft_size);

void NS(RDftCore_ComplexToPairDftFunction_BTest)(
    NS(RDftCore_ComplexToPairDft_fn) func, Long dft_size);
