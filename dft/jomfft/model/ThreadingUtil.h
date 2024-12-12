#pragma once

#include "Model.h"

int NS(Threading_GetNumThreads)(Long batch_size, int block_size,
                                int num_threads);

void NS(Threading_GetBatchRange)(Long batch_size, int block_size,
                                 int num_threads, Long *range);

void NS(Threading_Split)(int num_threads, Long batch_size_0, Long batch_size_1,
                         Long dft_size_1, int *num_threads_0,
                         int *num_threads_1);

void NS(Threading_Split_)(int num_threads, Long batch_size_0, Long batch_size_1,
                          Long dft_size_1, int *num_threads_0,
                          int *num_threads_1, bool prefer_batch_0);

struct LongPair {
  Long first;
  Long second;
};

void NS(Threading_GetTransposeBatchRange)(Long size, int block_size,
                                          int num_threads,
                                          struct LongPair *range);
