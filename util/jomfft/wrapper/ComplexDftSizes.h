#pragma once

#include "Wrapper.h"

struct NS(Wrapper_ComplexDftSizes) {
  int dimension;
  int *dft_sizes;
  int batch_size;
  int *in_storage_sizes;
  int in_stride;
  int in_distance;
  int *out_storage_sizes;
  int out_stride;
  int out_distance;
  int tensor_dimension;
  Long *tensor_sizes;
  Long *in_tensor_storage_sizes;
  Long *out_tensor_storage_sizes;
};

struct NS(Wrapper_ComplexDftSizes) *NS(Wrapper_ComplexDftSizes_new)(
    int dimension, const int *dft_sizes, int batch_size, int in_margin,
    int in_stride, int out_margin, int out_stride, bool inner_batch);

void NS(Wrapper_ComplexDftSizes_delete)(struct NS(Wrapper_ComplexDftSizes) *p);
