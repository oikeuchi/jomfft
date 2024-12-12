#pragma once

#include "ComplexFunction.h"
#include "ComplexFunctionW.h"
#include "RealFunction.h"
#include "RealToComplexFunction.h"
#include "RealToComplexFunctionW.h"

#include "template/DefineMacroPairRealToComplex.h"

#include "template/FunctionTripleWDef_.h"

#include "template/UndefMacroPair.h"

struct NS(RealToComplexFunctionW) *NS(RealToComplexFunctionTripleDW_0_new)(
    struct NS(RealToComplexFunction) *func_0,
    struct NS(ComplexFunction) *func_1, struct NS(ComplexFunction) *func_2,
    Long size);

struct NS(RealToComplexFunctionW) *NS(RealToComplexFunctionTripleWB_0_new)(
    struct NS(RealToComplexFunction) *func_0,
    struct NS(ComplexFunctionW) *func_1, struct NS(ComplexFunction) *func_2);
