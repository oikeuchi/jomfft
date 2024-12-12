#pragma once

#include "Wrapper.h"

struct NS(Wrapper_RealDftSizes) {
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
  Long *ce_tensor_sizes;
  int ce_axis;
};

struct NS(Wrapper_RealDftSizes) *NS(Wrapper_RealDftSizes_new)(
    int dimension, const int *dft_sizes, int batch_size, int in_margin,
    int in_stride, int out_margin, int out_stride, bool inner_batch,
    bool inplace);

void NS(Wrapper_RealDftSizes_delete)(struct NS(Wrapper_RealDftSizes) *p);
