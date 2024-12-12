#pragma once

#include "Util_.h"

struct NS(RealTensor);

#define TENSOR NS(ComplexTensor)
#define TENSOR_(n) NS(ComplexTensor_##n)
#define VALUE_TYPE Complex

#include "template/TensorDef.h"

Real TENSOR_(absSum)(const struct TENSOR *p);

void TENSOR_(CopyAsReal)(const struct NS(TensorShape) *shape,
                         const struct TENSOR *x, struct NS(RealTensor) *y);

#include "template/UndefTensorMacro.h"
