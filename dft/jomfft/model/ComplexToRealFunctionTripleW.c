#include "ComplexToRealFunctionTripleW.h"

#include "template/DefineMacroPairComplexToReal.h"

#include "template/FunctionTripleWImpl_.h"

struct NS(ComplexToRealFunctionTripleDW_2Data) {
  struct NS(ComplexFunction) *func_0;
  struct NS(ComplexFunction) *func_1;
  struct NS(ComplexToRealFunction) *func_2;
  Long size;
};

static struct NS(ComplexToRealFunctionTripleDW_2Data) *
ComplexToRealFunctionTripleDW_2Data_new(
    struct NS(ComplexFunction) *func_0, struct NS(ComplexFunction) *func_1,
    struct NS(ComplexToRealFunction) *func_2, Long size) {
  struct NS(ComplexToRealFunctionTripleDW_2Data) *out =
      NEW(struct NS(ComplexToRealFunctionTripleDW_2Data));
  *out = (struct NS(ComplexToRealFunctionTripleDW_2Data)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2, .size = size};
  return out;
}

static void ComplexToRealFunctionTripleDW_2Data_destruct(void *p) {
  if (p) {
    struct NS(ComplexToRealFunctionTripleDW_2Data) *o =
        (struct NS(ComplexToRealFunctionTripleDW_2Data) *)p;
    if (o->func_0) {
      NS(ComplexFunction_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      NS(ComplexFunction_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      NS(ComplexToRealFunction_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static Long
ComplexToRealFunctionTripleDW_2_size(struct NS(ComplexToRealFunctionW) *p) {
  struct NS(ComplexToRealFunctionTripleDW_2Data) *data =
      (struct NS(ComplexToRealFunctionTripleDW_2Data) *)
          NS(ComplexToRealFunctionW_data)(p);
  return 2 * data->size * sizeof(Complex);
}

static void
ComplexToRealFunctionTripleDW_2_evaluate(struct NS(ComplexToRealFunctionW) *p,
                                         Complex *x, Real *y, void *work) {
  struct NS(ComplexToRealFunctionTripleDW_2Data) *data =
      (struct NS(ComplexToRealFunctionTripleDW_2Data) *)
          NS(ComplexToRealFunctionW_data)(p);
  Complex *b_x = (Complex *)work;
  Complex *b_y = (Complex *)((char *)work + data->size);
  NS(ComplexFunction_evaluate)(data->func_0, x, b_x);
  NS(ComplexFunction_evaluate)(data->func_1, b_x, b_y);
  NS(ComplexToRealFunction_evaluate)(data->func_2, b_y, y);
}

struct NS(ComplexToRealFunctionW) *NS(ComplexToRealFunctionTripleDW_2_new)(
    struct NS(ComplexFunction) *func_0, struct NS(ComplexFunction) *func_1,
    struct NS(ComplexToRealFunction) *func_2, Long size) {
  return NS(ComplexToRealFunctionW_new)(
      ComplexToRealFunctionTripleDW_2_evaluate,
      ComplexToRealFunctionTripleDW_2_size,
      ComplexToRealFunctionTripleDW_2Data_destruct,
      ComplexToRealFunctionTripleDW_2Data_new(func_0, func_1, func_2, size));
}

struct NS(ComplexToRealFunctionTripleWB_2Data) {
  struct NS(ComplexFunction) *func_0;
  struct NS(ComplexFunctionW) *func_1;
  struct NS(ComplexToRealFunction) *func_2;
};

static struct NS(ComplexToRealFunctionTripleWB_2Data) *
ComplexToRealFunctionTripleWB_2Data_new(
    struct NS(ComplexFunction) *func_0, struct NS(ComplexFunctionW) *func_1,
    struct NS(ComplexToRealFunction) *func_2) {
  struct NS(ComplexToRealFunctionTripleWB_2Data) *out =
      NEW(struct NS(ComplexToRealFunctionTripleWB_2Data));
  *out = (struct NS(ComplexToRealFunctionTripleWB_2Data)){
      .func_0 = func_0, .func_1 = func_1, .func_2 = func_2};
  return out;
}

static void ComplexToRealFunctionTripleWB_2Data_destruct(void *p) {
  if (p) {
    struct NS(ComplexToRealFunctionTripleWB_2Data) *o =
        (struct NS(ComplexToRealFunctionTripleWB_2Data) *)p;
    if (o->func_0) {
      NS(ComplexFunction_delete)(o->func_0);
      o->func_0 = NULL;
    }
    if (o->func_1) {
      NS(ComplexFunctionW_delete)(o->func_1);
      o->func_1 = NULL;
    }
    if (o->func_2) {
      NS(ComplexToRealFunction_delete)(o->func_2);
      o->func_2 = NULL;
    }
    DELETE(o);
  }
}

static Long
ComplexToRealFunctionTripleWB_2_size(struct NS(ComplexToRealFunctionW) *p) {
  struct NS(ComplexToRealFunctionTripleWB_2Data) *data =
      (struct NS(ComplexToRealFunctionTripleWB_2Data) *)
          NS(ComplexToRealFunctionW_data)(p);
  return NS(ComplexFunctionW_size)(data->func_1);
}

static void
ComplexToRealFunctionTripleWB_2_evaluate(struct NS(ComplexToRealFunctionW) *p,
                                         Complex *x, Real *y, void *work) {
  struct NS(ComplexToRealFunctionTripleWB_2Data) *data =
      (struct NS(ComplexToRealFunctionTripleWB_2Data) *)
          NS(ComplexToRealFunctionW_data)(p);
  Complex *b = (Complex *)work;
  NS(ComplexFunction_evaluate)(data->func_0, x, b);
  NS(ComplexFunctionW_evaluate)(data->func_1, b, b, b);
  NS(ComplexToRealFunction_evaluate)(data->func_2, b, y);
}

struct NS(ComplexToRealFunctionW) *NS(ComplexToRealFunctionTripleWB_2_new)(
    struct NS(ComplexFunction) *func_0, struct NS(ComplexFunctionW) *func_1,
    struct NS(ComplexToRealFunction) *func_2) {
  return NS(ComplexToRealFunctionW_new)(
      ComplexToRealFunctionTripleWB_2_evaluate,
      ComplexToRealFunctionTripleWB_2_size,
      ComplexToRealFunctionTripleWB_2Data_destruct,
      ComplexToRealFunctionTripleWB_2Data_new(func_0, func_1, func_2));
}
