#pragma once

#include "../model/ComplexToRealFunction.h"
#include "../model/RealToComplexFunction.h"
#include "RDft.h"

struct NS(RDft_OuterBatchFactory_F);

typedef struct NS(RealToComplexFunction) *(
    *NS(RDft_OuterBatchFactory_F_fn))(struct NS(RDft_OuterBatchFactory_F) *,
                                      Long, Long, Long, bool);

struct NS(RDft_OuterBatchFactory_F) *NS(RDft_OuterBatchFactory_F_new)(
    NS(RDft_OuterBatchFactory_F_fn) create, jomfftDestruct_fn destruct,
    void *data);

void NS(RDft_OuterBatchFactory_F_delete)(
    struct NS(RDft_OuterBatchFactory_F) *p);

void *NS(RDft_OuterBatchFactory_F_data)(struct NS(RDft_OuterBatchFactory_F) *p);

struct NS(RealToComplexFunction) *NS(RDft_OuterBatchFactory_F_create)(
    struct NS(RDft_OuterBatchFactory_F) *p, Long batch_size, Long x_stride,
    Long y_stride, bool inplace);

struct NS(RDft_OuterBatchFactory_B);

typedef struct NS(ComplexToRealFunction) *(
    *NS(RDft_OuterBatchFactory_B_fn))(struct NS(RDft_OuterBatchFactory_B) *,
                                      Long, Long, Long, bool);

struct NS(RDft_OuterBatchFactory_B) *NS(RDft_OuterBatchFactory_B_new)(
    NS(RDft_OuterBatchFactory_B_fn) create, jomfftDestruct_fn destruct,
    void *data);

void NS(RDft_OuterBatchFactory_B_delete)(
    struct NS(RDft_OuterBatchFactory_B) *p);

void *NS(RDft_OuterBatchFactory_B_data)(struct NS(RDft_OuterBatchFactory_B) *p);

struct NS(ComplexToRealFunction) *NS(RDft_OuterBatchFactory_B_create)(
    struct NS(RDft_OuterBatchFactory_B) *p, Long batch_size, Long x_stride,
    Long y_stride, bool inplace);

struct NS(RealToComplexFunction) *NS(RDft_EvenSizeBatch_F_new)(
    struct NS(RDft_OuterBatchFactory_F) *factory, Long dft_size,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, bool inplace, int num_threads);

struct NS(ComplexToRealFunction) *NS(RDft_EvenSizeBatch_B_new)(
    struct NS(RDft_OuterBatchFactory_B) *factory, Long dft_size,
    Long batch_size, Long x_stride_0, Long x_stride_1, Long y_stride_0,
    Long y_stride_1, bool inplace, int num_threads);
