#pragma once

#include <jomfft/data/DataFactory.h>
#include <jomfft/model/ComplexFunction.h>

#include "../Test.h"

struct NS(CDft_BatchDftFactory);

typedef struct NS(ComplexFunction) *(
    *NS(CDft_BatchDftFactory_fn))(struct NS(CDft_BatchDftFactory) *,
                                  struct NS(DataFactory) *, bool, Long, Long,
                                  Long, Long, Long, Long, bool, int);

struct NS(CDft_BatchDftFactory) *NS(CDft_BatchDftFactory_new)(
    NS(CDft_BatchDftFactory_fn) create, jomfftDestruct_fn destruct, void *data);

void NS(CDft_BatchDftFactory_delete)(struct NS(CDft_BatchDftFactory) *p);

void *NS(CDft_BatchDftFactory_data)(struct NS(CDft_BatchDftFactory) *p);

struct NS(ComplexFunction) *NS(CDft_BatchDftFactory_create)(
    struct NS(CDft_BatchDftFactory) *p, struct NS(DataFactory) *data,
    bool backward, Long dft_size, Long batch_size, Long x_stride_0,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, bool inplace,
    int num_threads);

void NS(CDft_BatchDftTest_)(struct NS(CDft_BatchDftFactory) *factory,
                            Long dft_size);
