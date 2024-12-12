struct TENSOR *TENSOR_(new)(int dimension, const Long *sizes) {
  struct TENSOR *out = NEW(struct TENSOR);
  out->shape = NS(TensorShape_new)(dimension, sizes);
  Long data_size = NS(TensorShape_dataSize)(out->shape);
  if (data_size) {
    out->data = ARRAY_NEW(VALUE_TYPE, data_size);
    ARRAY_ZERO(VALUE_TYPE, data_size, out->data);
  }
  return out;
}

struct TENSOR *TENSOR_(new1)(Long size) {
  return TENSOR_(new)(1, &size);
}

struct TENSOR *TENSOR_(new2)(Long size0, Long size1) {
  Long sizes[2] = {size0, size1};
  return TENSOR_(new)(2, sizes);
}

struct TENSOR *TENSOR_(new3)(Long size0, Long size1, Long size2) {
  Long sizes[3] = {size0, size1, size2};
  return TENSOR_(new)(3, sizes);
}

void TENSOR_(delete)(void *p) {
  if (p) {
    struct TENSOR *o = (struct TENSOR *)p;
    if (o->shape) {
      NS(TensorShape_delete)(o->shape);
      o->shape = NULL;
    }
    if (o->data) {
      ARRAY_DELETE(o->data);
      o->data = NULL;
    }
    DELETE(o);
  }
}

struct TENSOR *TENSOR_(zerosLike)(const struct TENSOR *p) {
  assert(p);
  return TENSOR_(new)(p->shape->dimension, p->shape->sizes);
}

const VALUE_TYPE *TENSOR_(cat)(const struct TENSOR *p, const Long *indexes) {
  assert(p);
  assert(p->shape);
  assert(p->data);
  Long index = NS(TensorShape_dataIndex)(p->shape, indexes);
  assert(0 <= index && index < NS(TensorShape_dataSize)(p->shape));
  return p->data + index;
}

const VALUE_TYPE *TENSOR_(cat1)(const struct TENSOR *p, Long i) {
  assert(p);
  assert(p->shape);
  assert(p->shape->dimension >= 1);
  assert(p->data);
  assert(0 <= i && i < p->shape->sizes[0]);
  return p->data + i;
}

const VALUE_TYPE *TENSOR_(cat2)(const struct TENSOR *p, Long i, Long j) {
  assert(p);
  assert(p->shape);
  assert(p->shape->dimension >= 2);
  assert(p->data);
  assert(0 <= i && i < p->shape->sizes[0]);
  assert(0 <= j && j < p->shape->sizes[1]);
  Long index = p->shape->sizes[1] * i + j;
  return p->data + index;
}

const VALUE_TYPE *TENSOR_(cat3)(const struct TENSOR *p, Long i, Long j,
                                Long k) {
  assert(p);
  assert(p->shape);
  assert(p->shape->dimension >= 3);
  assert(p->data);
  assert(0 <= i && i < p->shape->sizes[0]);
  assert(0 <= j && j < p->shape->sizes[1]);
  assert(0 <= k && k < p->shape->sizes[2]);
  Long index = p->shape->sizes[2] * (p->shape->sizes[1] * i + j) + k;
  return p->data + index;
}

VALUE_TYPE *TENSOR_(at)(struct TENSOR *p, const Long *indexes) {
  return (VALUE_TYPE *)TENSOR_(cat)(p, indexes);
}

VALUE_TYPE *TENSOR_(at1)(struct TENSOR *p, Long i) {
  return (VALUE_TYPE *)TENSOR_(cat1)(p, i);
}

VALUE_TYPE *TENSOR_(at2)(struct TENSOR *p, Long i, Long j) {
  return (VALUE_TYPE *)TENSOR_(cat2)(p, i, j);
}

VALUE_TYPE *TENSOR_(at3)(struct TENSOR *p, Long i, Long j, Long k) {
  return (VALUE_TYPE *)TENSOR_(cat3)(p, i, j, k);
}

VALUE_TYPE
TENSOR_(get)(const struct TENSOR *p, const Long *indexes) {
  return *TENSOR_(cat)(p, indexes);
}

VALUE_TYPE
TENSOR_(get1)(const struct TENSOR *p, Long i) { return *TENSOR_(cat1)(p, i); }

VALUE_TYPE
TENSOR_(get2)(const struct TENSOR *p, Long i, Long j) {
  return *TENSOR_(cat2)(p, i, j);
}

VALUE_TYPE
TENSOR_(get3)(const struct TENSOR *p, Long i, Long j, Long k) {
  return *TENSOR_(cat3)(p, i, j, k);
}

void TENSOR_(set)(struct TENSOR *p, const Long *indexes, VALUE_TYPE x) {
  *TENSOR_(at)(p, indexes) = x;
}

void TENSOR_(set1)(struct TENSOR *p, Long i, VALUE_TYPE x) {
  *TENSOR_(at1)(p, i) = x;
}

void TENSOR_(set2)(struct TENSOR *p, Long i, Long j, VALUE_TYPE x) {
  *TENSOR_(at2)(p, i, j) = x;
}

void TENSOR_(set3)(struct TENSOR *p, Long i, Long j, Long k, VALUE_TYPE x) {
  *TENSOR_(at3)(p, i, j, k) = x;
}

const VALUE_TYPE *TENSOR_(cbegin)(const struct TENSOR *p) {
  assert(p);
  return p->data;
}

const VALUE_TYPE *TENSOR_(cend)(const struct TENSOR *p) {
  assert(p);
  return p->data + NS(TensorShape_dataSize)(p->shape);
}

VALUE_TYPE *TENSOR_(begin)(struct TENSOR *p) {
  return (VALUE_TYPE *)TENSOR_(cbegin)(p);
}

VALUE_TYPE *TENSOR_(end)(struct TENSOR *p) {
  return (VALUE_TYPE *)TENSOR_(cend)(p);
}

static void TENSOR_(equal__)(const struct NS(TensorShape) *shape,
                             const VALUE_TYPE *lhs_data,
                             const VALUE_TYPE *rhs_data, const Long *strides,
                             int axis, Long *num_of_errors,
                             Long *first_error_indexes) {
  assert(shape);
  assert(lhs_data);
  assert(rhs_data);
  assert(strides);
  if (axis == shape->dimension - 1) {
    assert(strides[axis] == 1);
    for (Long k = 0; k < shape->sizes[axis]; ++k) {
      if (!VALUE_TYPE_(equal)(lhs_data[k], rhs_data[k])) {
        if (*num_of_errors == 0 && first_error_indexes) {
          first_error_indexes[axis] = k;
        }
        ++(*num_of_errors);
      }
    }
  } else {
    for (Long k = 0; k < shape->sizes[axis]; ++k) {
      if (*num_of_errors == 0 && first_error_indexes) {
        first_error_indexes[axis] = k;
      }
      TENSOR_(equal__)(shape, lhs_data + strides[axis] * k,
                       rhs_data + strides[axis] * k, strides, axis + 1,
                       num_of_errors, first_error_indexes);
    }
  }
}

static void TENSOR_(almostEqual__)(const struct NS(TensorShape) *shape,
                                   const VALUE_TYPE *lhs_data,
                                   const VALUE_TYPE *rhs_data, Real abs_eps,
                                   Real rel_eps, const Long *strides, int axis,
                                   Long *num_of_errors,
                                   Long *first_error_indexes) {
  assert(shape);
  assert(lhs_data);
  assert(rhs_data);
  assert(strides);
  if (axis == shape->dimension - 1) {
    assert(strides[axis] == 1);
    for (Long k = 0; k < shape->sizes[axis]; ++k) {
      if (!VALUE_TYPE_(almostEqual_)(lhs_data[k], rhs_data[k], abs_eps,
                                     rel_eps)) {
        if (*num_of_errors == 0 && first_error_indexes) {
          first_error_indexes[axis] = k;
        }
        ++(*num_of_errors);
      }
    }
  } else {
    for (Long k = 0; k < shape->sizes[axis]; ++k) {
      if (*num_of_errors == 0 && first_error_indexes) {
        first_error_indexes[axis] = k;
      }
      TENSOR_(almostEqual__)
      (shape, lhs_data + strides[axis] * k, rhs_data + strides[axis] * k,
       abs_eps, rel_eps, strides, axis + 1, num_of_errors, first_error_indexes);
    }
  }
}

bool TENSOR_(equal)(const struct TENSOR *lhs, const struct TENSOR *rhs) {
  Long num_of_errors = 0;
  return TENSOR_(equal_)(lhs, rhs, &num_of_errors, NULL);
}

bool TENSOR_(equal_)(const struct TENSOR *lhs, const struct TENSOR *rhs,
                     Long *num_of_errors, Long *first_error_indexes) {
  assert(lhs);
  assert(rhs);
  assert(num_of_errors);
  if (!NS(TensorShape_equal)(lhs->shape, rhs->shape)) {
    *num_of_errors = -1;
    return false;
  }
  if (lhs->shape->dimension == 0) {
    *num_of_errors = 0;
    return true;
  }
  Long *strides = ARRAY_NEW(Long, lhs->shape->dimension);
  NS(TensorShape_strides)(lhs->shape, strides);
  *num_of_errors = 0;
  TENSOR_(equal__)(lhs->shape, lhs->data, rhs->data, strides, 0, num_of_errors,
                   first_error_indexes);
  ARRAY_DELETE(strides);
  return *num_of_errors == 0;
}

bool TENSOR_(almostEqual)(const struct TENSOR *lhs, const struct TENSOR *rhs) {
  Long num_of_errors = 0;
  return TENSOR_(almostEqual_)(lhs, rhs, R_EPSILON, R_EPSILON, &num_of_errors,
                               NULL);
}

bool TENSOR_(almostEqual_)(const struct TENSOR *lhs, const struct TENSOR *rhs,
                           Real abs_eps, Real rel_eps, Long *num_of_errors,
                           Long *first_error_indexes) {
  assert(lhs);
  assert(rhs);
  assert(num_of_errors);
  if (!NS(TensorShape_equal)(lhs->shape, rhs->shape)) {
    *num_of_errors = -1;
    return false;
  }
  if (lhs->shape->dimension == 0) {
    *num_of_errors = 0;
    return true;
  }
  Long *strides = ARRAY_NEW(Long, lhs->shape->dimension);
  *num_of_errors = 0;
  NS(TensorShape_strides)(lhs->shape, strides);
  TENSOR_(almostEqual__)
  (lhs->shape, lhs->data, rhs->data, abs_eps, rel_eps, strides, 0,
   num_of_errors, first_error_indexes);
  ARRAY_DELETE(strides);
  return *num_of_errors == 0;
}

struct TENSOR *TENSOR_(copy)(const struct TENSOR *p) {
  struct TENSOR *out = TENSOR_(zerosLike)(p);
  TENSOR_(copy_)(p, out);
  return out;
}

void TENSOR_(copy_)(const struct TENSOR *p, struct TENSOR *x) {
  assert(p);
  assert(x);
  assert(NS(TensorShape_equal)(p->shape, x->shape));
  const Long data_size = NS(TensorShape_dataSize)(p->shape);
  ARRAY_COPY(VALUE_TYPE, data_size, p->data, x->data);
}

static void TENSOR_(CopyAs_)(int dimension, int i, const Long *sizes,
                             Long *indexes, const struct TENSOR *x,
                             struct TENSOR *y) {
  assert(sizes);
  assert(indexes);
  if (i == dimension - 1) {
    for (Long j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      TENSOR_(set)
      (y, indexes, TENSOR_(get)(x, indexes));
    }
  } else {
    for (Long j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      TENSOR_(CopyAs_)
      (dimension, i + 1, sizes, indexes, x, y);
    }
  }
}

void TENSOR_(CopyAs)(const struct NS(TensorShape) *shape,
                     const struct TENSOR *x, struct TENSOR *y) {
  assert(shape);
  if (shape->dimension > 0) {
    if (NS(TensorShape_equal)(shape, x->shape) &&
        NS(TensorShape_equal)(shape, y->shape)) {
      TENSOR_(copy_)
      (x, y);
    } else {
      Long *indexes = ARRAY_NEW(Long, shape->dimension);
      TENSOR_(CopyAs_)
      (shape->dimension, 0, shape->sizes, indexes, x, y);
      ARRAY_DELETE(indexes);
    }
  }
}
