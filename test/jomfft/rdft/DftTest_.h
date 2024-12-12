#pragma once

#include <jomfft/data/DataFactory.h>
#include <jomfft/model/ComplexToRealFunction.h>
#include <jomfft/model/RealToComplexFunction.h>

#include "../Test.h"

void NS(RDft_Dft_FTest__)(struct NS(RealToComplexFunction) *func, Long dft_size,
                          Long x_stride, Long y_stride, bool inplace);

void NS(RDft_Dft_BTest__)(struct NS(ComplexToRealFunction) *func, Long dft_size,
                          Long x_stride, Long y_stride, bool inplace);

struct NS(RDft_DftFactory_F);

typedef struct NS(RealToComplexFunction) *(
    *NS(RDft_DftFactory_F_fn))(struct NS(RDft_DftFactory_F) *,
                               struct NS(DataFactory) *, Long, Long, Long, bool,
                               int);

struct NS(RDft_DftFactory_F) *NS(RDft_DftFactory_F_new)(
    NS(RDft_DftFactory_F_fn) create, jomfftDestruct_fn destruct, void *data);

void NS(RDft_DftFactory_F_delete)(struct NS(RDft_DftFactory_F) *p);

void *NS(RDft_DftFactory_F_data)(struct NS(RDft_DftFactory_F) *p);

struct NS(RealToComplexFunction) *NS(RDft_DftFactory_F_create)(
    struct NS(RDft_DftFactory_F) *p, struct NS(DataFactory) *data,
    Long dft_size, Long x_stride, Long y_stride, bool inplace, int num_threads);

struct NS(RDft_DftFactory_B);

typedef struct NS(ComplexToRealFunction) *(
    *NS(RDft_DftFactory_B_fn))(struct NS(RDft_DftFactory_B) *,
                               struct NS(DataFactory) *, Long, Long, Long, bool,
                               int);

struct NS(RDft_DftFactory_B) *NS(RDft_DftFactory_B_new)(
    NS(RDft_DftFactory_B_fn) create, jomfftDestruct_fn destruct, void *data);

void NS(RDft_DftFactory_B_delete)(struct NS(RDft_DftFactory_B) *p);

void *NS(RDft_DftFactory_B_data)(struct NS(RDft_DftFactory_B) *p);

struct NS(ComplexToRealFunction) *NS(RDft_DftFactory_B_create)(
    struct NS(RDft_DftFactory_B) *p, struct NS(DataFactory) *data,
    Long dft_size, Long x_stride, Long y_stride, bool inplace, int num_threads);

void NS(RDft_Dft_FTest_)(struct NS(RDft_DftFactory_F) *factory, Long dft_size);

void NS(RDft_Dft_BTest_)(struct NS(RDft_DftFactory_B) *factory, Long dft_size);
