#pragma once

#include <jomfft/data/DataFactory.h>
#include <jomfft/model/ComplexFunction.h>

#include "../Test.h"

void NS(CDft_DftTest__)(struct NS(ComplexFunction) *func, bool backward,
                        Long dft_size, Long x_stride, Long y_stride,
                        bool inplace);

struct NS(CDft_DftFactory);

typedef struct NS(ComplexFunction) *(
    *NS(CDft_DftFactory_fn))(struct NS(CDft_DftFactory) *,
                             struct NS(DataFactory) *, bool, Long, Long, Long,
                             bool, int);

struct NS(CDft_DftFactory) *NS(CDft_DftFactory_new)(
    NS(CDft_DftFactory_fn) create, jomfftDestruct_fn destruct, void *data);

void NS(CDft_DftFactory_delete)(struct NS(CDft_DftFactory) *p);

void *NS(CDft_DftFactory_data)(struct NS(CDft_DftFactory) *p);

struct NS(ComplexFunction) *NS(CDft_DftFactory_create)(
    struct NS(CDft_DftFactory) *p, struct NS(DataFactory) *data, bool backward,
    Long dft_size, Long x_stride, Long y_stride, bool inplace, int num_threads);

void NS(CDft_DftTest_)(struct NS(CDft_DftFactory) *factory, Long dft_size);
