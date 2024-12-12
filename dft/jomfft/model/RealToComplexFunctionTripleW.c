#include "RealToComplexFunctionTripleW.h"

#include "template/DefineMacroPairRealToComplex.h"

#include "template/FunctionTripleWImpl_.h"

struct NS(RealToComplexFunctionTripleDW_0Data) {
  struct NS(RealToComplexFunction) *func_0;
  struct NS(ComplexFunction) *func_1;
  struct NS(ComplexFunction) *func_2;
  Long size;
};

static struct NS(RealToComplexFunctionTripleDW_0Data) *
RealToComplexFunctionTripleDW_0Data_new(
    struct NS(RealToComplexFunction) *func_0,
    struct NS(ComplexFunction) *func_1, struct NS(ComplexFunction) *func_2,
    Long size) {
  struct NS(RealToComplexFunctionTripleDW_0Data) *out =
      NEW(struct NS(RealToComplexFunctionTripleDW_0Data));
  *out = (struct NS(RealToComplexFunctionTripleDW_0Data)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2, .size = size};
  return out;
}

static void RealToComplexFunctionTripleDW_0Data_destruct(void *p) {
  if (p) {
    struct NS(RealToComplexFunctionTripleDW_0Data) *o =
        (struct NS(RealToComplexFunctionTripleDW_0Data) *)p;
    if (o->func_0) {
      NS(RealToComplexFunction_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      NS(ComplexFunction_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      NS(ComplexFunction_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static Long
RealToComplexFunctionTripleDW_0_size(struct NS(RealToComplexFunctionW) *p) {
  struct NS(RealToComplexFunctionTripleDW_0Data) *data =
      (struct NS(RealToComplexFunctionTripleDW_0Data) *)
          NS(RealToComplexFunctionW_data)(p);
  return 2 * data->size;
}

static void
RealToComplexFunctionTripleDW_0_evaluate(struct NS(RealToComplexFunctionW) *p,
                                         Real *x, Complex *y, void *work) {
  struct NS(RealToComplexFunctionTripleDW_0Data) *data =
      (struct NS(RealToComplexFunctionTripleDW_0Data) *)
          NS(RealToComplexFunctionW_data)(p);
  Complex *b_x = (Complex *)work;
  Complex *b_y = (Complex *)((char *)work + data->size);
  NS(RealToComplexFunction_evaluate)(data->func_0, x, b_x);
  NS(ComplexFunction_evaluate)(data->func_1, b_x, b_y);
  NS(ComplexFunction_evaluate)(data->func_2, b_y, y);
}

struct NS(RealToComplexFunctionW) *NS(RealToComplexFunctionTripleDW_0_new)(
    struct NS(RealToComplexFunction) *func_0,
    struct NS(ComplexFunction) *func_1, struct NS(ComplexFunction) *func_2,
    Long size) {
  return NS(RealToComplexFunctionW_new)(
      RealToComplexFunctionTripleDW_0_evaluate,
      RealToComplexFunctionTripleDW_0_size,
      RealToComplexFunctionTripleDW_0Data_destruct,
      RealToComplexFunctionTripleDW_0Data_new(func_0, func_1, func_2, size));
}

struct NS(RealToComplexFunctionTripleWB_0Data) {
  struct NS(RealToComplexFunction) *func_0;
  struct NS(ComplexFunctionW) *func_1;
  struct NS(ComplexFunction) *func_2;
};

static struct NS(RealToComplexFunctionTripleWB_0Data) *
RealToComplexFunctionTripleWB_0Data_new(
    struct NS(RealToComplexFunction) *func_0,
    struct NS(ComplexFunctionW) *func_1, struct NS(ComplexFunction) *func_2) {
  struct NS(RealToComplexFunctionTripleWB_0Data) *out =
      NEW(struct NS(RealToComplexFunctionTripleWB_0Data));
  *out = (struct NS(RealToComplexFunctionTripleWB_0Data)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2};
  return out;
}

static void RealToComplexFunctionTripleWB_0Data_destruct(void *p) {
  if (p) {
    struct NS(RealToComplexFunctionTripleWB_0Data) *o =
        (struct NS(RealToComplexFunctionTripleWB_0Data) *)p;
    if (o->func_0) {
      NS(RealToComplexFunction_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      NS(ComplexFunctionW_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      NS(ComplexFunction_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static Long
RealToComplexFunctionTripleWB_0_size(struct NS(RealToComplexFunctionW) *p) {
  struct NS(RealToComplexFunctionTripleWB_0Data) *data =
      (struct NS(RealToComplexFunctionTripleWB_0Data) *)
          NS(RealToComplexFunctionW_data)(p);
  return NS(ComplexFunctionW_size)(data->func_1);
}

static void
RealToComplexFunctionTripleWB_0_evaluate(struct NS(RealToComplexFunctionW) *p,
                                         Real *x, Complex *y, void *work) {
  struct NS(RealToComplexFunctionTripleWB_0Data) *data =
      (struct NS(RealToComplexFunctionTripleWB_0Data) *)
          NS(RealToComplexFunctionW_data)(p);
  Complex *b = (Complex *)work;
  NS(RealToComplexFunction_evaluate)(data->func_0, x, b);
  NS(ComplexFunctionW_evaluate)(data->func_1, b, b, b);
  NS(ComplexFunction_evaluate)(data->func_2, b, y);
}

struct NS(RealToComplexFunctionW) *NS(RealToComplexFunctionTripleWB_0_new)(
    struct NS(RealToComplexFunction) *func_0,
    struct NS(ComplexFunctionW) *func_1, struct NS(ComplexFunction) *func_2) {
  return NS(RealToComplexFunctionW_new)(
      RealToComplexFunctionTripleWB_0_evaluate,
      RealToComplexFunctionTripleWB_0_size,
      RealToComplexFunctionTripleWB_0Data_destruct,
      RealToComplexFunctionTripleWB_0Data_new(func_0, func_1, func_2));
}
