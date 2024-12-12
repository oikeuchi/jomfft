#pragma once

#include "Util_.h"

struct NS(TensorShape) {
  int dimension;
  Long *sizes;
};

struct NS(TensorShape) *NS(TensorShape_new)(int dimension, const Long *sizes);

void NS(TensorShape_delete)(struct NS(TensorShape) *p);

bool NS(TensorShape_equal)(const struct NS(TensorShape) *lhs,
                           const struct NS(TensorShape) *rhs);

Long NS(TensorShape_dataSize)(const struct NS(TensorShape) *p);

Long NS(TensorShape_dataIndex)(const struct NS(TensorShape) *p,
                               const Long *indexes);

void NS(TensorShape_strides)(const struct NS(TensorShape) *p, Long *out);

void NS(TensorShape_print)(const struct NS(TensorShape) *p, FILE *stream);

void NS(TensorShape_printIndexes)(const struct NS(TensorShape) *p, FILE *stream,
                                  const Long *indexes);
