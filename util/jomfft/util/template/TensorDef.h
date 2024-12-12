#include "../TensorShape.h"

struct TENSOR {
  struct NS(TensorShape) *shape;
  VALUE_TYPE *data;
};

struct TENSOR *TENSOR_(new)(int dimension, const Long *sizes);

struct TENSOR *TENSOR_(new1)(Long size);

struct TENSOR *TENSOR_(new2)(Long size0, Long size1);

struct TENSOR *TENSOR_(new3)(Long size0, Long size1, Long size2);

void TENSOR_(delete)(void *p);

struct TENSOR *TENSOR_(zerosLike)(const struct TENSOR *p);

const VALUE_TYPE *TENSOR_(cat)(const struct TENSOR *p, const Long *indexes);

const VALUE_TYPE *TENSOR_(cat1)(const struct TENSOR *p, Long i);

const VALUE_TYPE *TENSOR_(cat2)(const struct TENSOR *p, Long i, Long j);

const VALUE_TYPE *TENSOR_(cat3)(const struct TENSOR *p, Long i, Long j, Long k);

VALUE_TYPE *TENSOR_(at)(struct TENSOR *p, const Long *indexes);

VALUE_TYPE *TENSOR_(at1)(struct TENSOR *p, Long i);

VALUE_TYPE *TENSOR_(at2)(struct TENSOR *p, Long i, Long j);

VALUE_TYPE *TENSOR_(at3)(struct TENSOR *p, Long i, Long j, Long k);

VALUE_TYPE TENSOR_(get)(const struct TENSOR *p, const Long *indexes);

VALUE_TYPE TENSOR_(get1)(const struct TENSOR *p, Long i);

VALUE_TYPE TENSOR_(get2)(const struct TENSOR *p, Long i, Long j);

VALUE_TYPE TENSOR_(get3)(const struct TENSOR *p, Long i, Long j, Long k);

void TENSOR_(set)(struct TENSOR *p, const Long *indexes, VALUE_TYPE x);

void TENSOR_(set1)(struct TENSOR *p, Long i, VALUE_TYPE x);

void TENSOR_(set2)(struct TENSOR *p, Long i, Long j, VALUE_TYPE x);

void TENSOR_(set3)(struct TENSOR *p, Long i, Long j, Long k, VALUE_TYPE x);

const VALUE_TYPE *TENSOR_(cbegin)(const struct TENSOR *p);

const VALUE_TYPE *TENSOR_(cend)(const struct TENSOR *p);

VALUE_TYPE *TENSOR_(begin)(struct TENSOR *p);

VALUE_TYPE *TENSOR_(end)(struct TENSOR *p);

bool TENSOR_(equal)(const struct TENSOR *lhs, const struct TENSOR *rhs);

bool TENSOR_(equal_)(const struct TENSOR *lhs, const struct TENSOR *rhs,
                     Long *num_of_errors, Long *first_error_indexes);

bool TENSOR_(almostEqual)(const struct TENSOR *lhs, const struct TENSOR *rhs);

bool TENSOR_(almostEqual_)(const struct TENSOR *lhs, const struct TENSOR *rhs,
                           Real abs_eps, Real rel_eps, Long *num_of_errors,
                           Long *first_error_indexes);

struct TENSOR *TENSOR_(copy)(const struct TENSOR *p);

void TENSOR_(copy_)(const struct TENSOR *p, struct TENSOR *x);

void TENSOR_(CopyAs)(const struct NS(TensorShape) *shape,
                     const struct TENSOR *x, struct TENSOR *y);
