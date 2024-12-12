#include "TensorShape.h"

#include "TensorUtil.h"

struct NS(TensorShape) *NS(TensorShape_new)(int dimension, const Long *sizes) {
  assert(dimension >= 0);
  struct NS(TensorShape) *out = NEW(struct NS(TensorShape));
  out->dimension = dimension;
  out->sizes = NULL;
  if (dimension) {
    out->sizes = ARRAY_NEW(Long, dimension);
    ARRAY_COPY(Long, dimension, sizes, out->sizes);
  }
  return out;
}

void NS(TensorShape_delete)(struct NS(TensorShape) *p) {
  if (p) {
    if (p->sizes) {
      ARRAY_DELETE(p->sizes);
      p->sizes = NULL;
    }
    DELETE(p);
  }
}

bool NS(TensorShape_equal)(const struct NS(TensorShape) *lhs,
                           const struct NS(TensorShape) *rhs) {
  if (lhs->dimension != rhs->dimension) {
    return false;
  }
  bool result = true;
  for (int k = 0; result && k < lhs->dimension; ++k) {
    result = result && lhs->sizes[k] == rhs->sizes[k];
  }
  return result;
}

static Long TensorDataSize(int dimension, const Long *sizes) {
  if (dimension == 1) {
    return sizes[0];
  } else {
    return sizes[0] * TensorDataSize(dimension - 1, sizes + 1);
  }
}

Long NS(TensorShape_dataSize)(const struct NS(TensorShape) *p) {
  assert(p);
  if (p->dimension <= 0) {
    return 0;
  }
  assert(p->dimension >= 1);
  assert(p->sizes);
  return TensorDataSize(p->dimension, p->sizes);
}

Long NS(TensorShape_dataIndex)(const struct NS(TensorShape) *p,
                               const Long *indexes) {
  assert(p);
  assert(p->dimension >= 1);
  assert(indexes);
  Long out = 0;
  for (int k = 0; k < p->dimension; ++k) {
    out *= p->sizes[k];
    out += indexes[k];
  }
  return out;
}

void NS(TensorShape_strides)(const struct NS(TensorShape) *p, Long *out) {
  NS(Tensor_GetStrides_)(p->dimension, p->sizes, out);
}

void NS(TensorShape_print)(const struct NS(TensorShape) *p, FILE *stream) {
  NS(TensorShape_printIndexes)(p, stream, p->sizes);
}

void NS(TensorShape_printIndexes)(const struct NS(TensorShape) *p, FILE *stream,
                                  const Long *indexes) {
  assert(p);
  fprintf(stream, "[");
  for (int k = 0; k < p->dimension; ++k) {
    if (k) {
      fprintf(stream, ",");
    }
    fprintf(stream, "%ld", indexes[k]);
  }
  fprintf(stream, "]");
}
