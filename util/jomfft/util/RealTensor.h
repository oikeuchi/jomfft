#pragma once

#include "Util_.h"

struct NS(ComplexTensor);

#define TENSOR NS(RealTensor)
#define TENSOR_(n) NS(RealTensor_##n)
#define VALUE_TYPE Real

#include "template/TensorDef.h"

Real TENSOR_(absSum)(const struct TENSOR *p);

void TENSOR_(CopyAsComplex)(const struct NS(TensorShape) *shape,
                            const struct TENSOR *x,
                            struct NS(ComplexTensor) *y);

#include "template/UndefTensorMacro.h"
