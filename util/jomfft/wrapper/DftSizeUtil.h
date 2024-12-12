#pragma once

#include "Wrapper.h"

int *NS(Wrapper_GetStorageSizes)(int dimension, const int *dft_sizes,
                                 int margin);

void NS(Wrapper_GetBatchLayout)(int dimension, const int *storage_sizes,
                                int batch_size, int margin, int stride,
                                bool inner_batch, int *batch_stride,
                                int *batch_distance);

Long *NS(Wrapper_GetTensorSizes)(int dimension, const int *storage_sizes,
                                 int batch_size, int margin, int stride,
                                 bool inner_batch, bool strided);
