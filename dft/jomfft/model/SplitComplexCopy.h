#pragma once

#include "ComplexToRealFunction.h"
#include "Model.h"
#include "RealToComplexFunction.h"

struct NS(RealToComplexFunction) *NS(SplitComplex_Copy_F_new)(
    Long size, Long x_stride, Long y_stride, Long split_distance,
    int num_threads);

struct NS(ComplexToRealFunction) *NS(SplitComplex_Copy_B_new)(
    Long size, Long x_stride, Long y_stride, Long split_distance,
    int num_threads);

struct NS(RealToComplexFunction) *NS(SplitComplex_CopyReal_F_new)(
    Long size, Long x_stride, Long y_stride, int num_threads);

struct NS(ComplexToRealFunction) *NS(SplitComplex_CopyReal_B_new)(
    Long size, Long x_stride, Long y_stride, int num_threads);
