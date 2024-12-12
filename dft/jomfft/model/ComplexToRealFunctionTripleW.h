#pragma once

#include "ComplexFunction.h"
#include "ComplexFunctionW.h"
#include "ComplexToRealFunction.h"
#include "ComplexToRealFunctionW.h"
#include "RealFunction.h"

#include "template/DefineMacroPairComplexToReal.h"

#include "template/FunctionTripleWDef_.h"

#include "template/UndefMacroPair.h"

struct NS(ComplexToRealFunctionW) *NS(ComplexToRealFunctionTripleDW_2_new)(
    struct NS(ComplexFunction) *func_0, struct NS(ComplexFunction) *func_1,
    struct NS(ComplexToRealFunction) *func_2, Long size);

struct NS(ComplexToRealFunctionW) *NS(ComplexToRealFunctionTripleWB_2_new)(
    struct NS(ComplexFunction) *func_0, struct NS(ComplexFunctionW) *func_1,
    struct NS(ComplexToRealFunction) *func_2);
