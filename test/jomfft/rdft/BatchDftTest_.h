#pragma once

#include <jomfft/data/DataFactory.h>
#include <jomfft/model/ComplexFunction.h>

#include "../Test.h"

struct NS(RDft_BatchDftFactory_F);

typedef struct NS(RealToComplexFunction) *(
    *NS(RDft_BatchDftFactory_F_fn))(struct NS(RDft_BatchDftFactory_F) *,
                                    struct NS(DataFactory) *, Long, Long, Long,
                                    Long, Long, Long, bool, int);

struct NS(RDft_BatchDftFactory_F) *NS(RDft_BatchDftFactory_F_new)(
    NS(RDft_BatchDftFactory_F_fn) create, jomfftDestruct_fn destruct,
    void *data);

void NS(RDft_BatchDftFactory_F_delete)(struct NS(RDft_BatchDftFactory_F) *p);

void *NS(RDft_BatchDftFactory_F_data)(struct NS(RDft_BatchDftFactory_F) *p);

struct NS(RealToComplexFunction) *NS(RDft_BatchDftFactory_F_create)(
    struct NS(RDft_BatchDftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads);

struct NS(RDft_BatchDftFactory_B);

typedef struct NS(ComplexToRealFunction) *(
    *NS(RDft_BatchDftFactory_B_fn))(struct NS(RDft_BatchDftFactory_B) *,
                                    struct NS(DataFactory) *, Long, Long, Long,
                                    Long, Long, Long, bool, int);

struct NS(RDft_BatchDftFactory_B) *NS(RDft_BatchDftFactory_B_new)(
    NS(RDft_BatchDftFactory_B_fn) create, jomfftDestruct_fn destruct,
    void *data);

void NS(RDft_BatchDftFactory_B_delete)(struct NS(RDft_BatchDftFactory_B) *p);

void *NS(RDft_BatchDftFactory_B_data)(struct NS(RDft_BatchDftFactory_B) *p);

struct NS(ComplexToRealFunction) *NS(RDft_BatchDftFactory_B_create)(
    struct NS(RDft_BatchDftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long batch_size, Long x_stride_0, Long x_stride_1,
    Long y_stride_0, Long y_stride_1, bool inplace, int num_threads);

void NS(RDft_BatchDft_FTest_)(struct NS(RDft_BatchDftFactory_F) *factory,
                              Long dft_size);

void NS(RDft_BatchDft_BTest_)(struct NS(RDft_BatchDftFactory_B) *factory,
                              Long dft_size);
