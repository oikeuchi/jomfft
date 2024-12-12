#pragma once

#include "Mkl.h"

#if USE_MKL

MKL_LONG *NS(Mkl_GetLength)(int dimension, const int *dft_sizes);

MKL_LONG *NS(Mkl_GetStrides)(int dimension, const int *storage_sizes,
                             int stride, int distance);

void NS(Mkl_Handle_destruct)(void *p);

#endif
