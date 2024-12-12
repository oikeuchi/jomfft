#include "EvenSizeBatch.h"

#include "../model/ComplexCopy.h"
#include "../model/ComplexToRealFunctionBuffered.h"
#include "../model/ComplexToRealFunctionBufferedLoop.h"
#include "../model/ComplexToRealFunctionLoop.h"
#include "../model/ComplexToRealFunctionPair.h"
#include "../model/ComplexToRealFunctionTripleW.h"
#include "../model/RealCopy.h"
#include "../model/RealToComplexFunction.h"
#include "../model/RealToComplexFunctionBuffered.h"
#include "../model/RealToComplexFunctionBufferedLoop.h"
#include "../model/RealToComplexFunctionLoop.h"
#include "../model/RealToComplexFunctionPair.h"
#include "../model/RealToComplexFunctionTripleW.h"

#define C_BUFFER_SIZE (C_L1_CACHE_SIZE / 4)

#define NS_(n) NS(RDft_##n)

struct NS_(OuterBatchFactory_F) {
  NS_(OuterBatchFactory_F_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(OuterBatchFactory_F) *NS_(OuterBatchFactory_F_new)(
    NS_(OuterBatchFactory_F_fn) create, jomfftDestruct_fn destruct,
    void *data) {
  struct NS_(OuterBatchFactory_F) *out = NEW(struct NS_(OuterBatchFactory_F));
  *out = (struct NS_(OuterBatchFactory_F)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(OuterBatchFactory_F_delete)(struct NS_(OuterBatchFactory_F) *p) {
  if (p) {
    if (p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(OuterBatchFactory_F_data)(struct NS_(OuterBatchFactory_F) *p) {
  assert(p);
  return p->data;
}

struct NS(RealToComplexFunction) *NS_(OuterBatchFactory_F_create)(
    struct NS_(OuterBatchFactory_F) *p, Long batch_size, Long x_stride,
    Long y_stride, bool inplace) {
  assert(p);
  return p->create(p, batch_size, x_stride, y_stride, inplace);
}

struct NS_(OuterBatchFactory_B) {
  NS_(OuterBatchFactory_B_fn) create;
  jomfftDestruct_fn destruct;
  void *data;
};

struct NS_(OuterBatchFactory_B) *NS_(OuterBatchFactory_B_new)(
    NS_(OuterBatchFactory_B_fn) create, jomfftDestruct_fn destruct,
    void *data) {
  struct NS_(OuterBatchFactory_B) *out = NEW(struct NS_(OuterBatchFactory_B));
  *out = (struct NS_(OuterBatchFactory_B)){
      .create = create, .destruct = destruct, .data = data};
  return out;
}

void NS_(OuterBatchFactory_B_delete)(struct NS_(OuterBatchFactory_B) *p) {
  if (p) {
    if (p->data) {
      p->destruct(p->data);
      p->data = NULL;
    }
    DELETE(p);
  }
}

void *NS_(OuterBatchFactory_B_data)(struct NS_(OuterBatchFactory_B) *p) {
  assert(p);
  return p->data;
}

struct NS(ComplexToRealFunction) *NS_(OuterBatchFactory_B_create)(
    struct NS_(OuterBatchFactory_B) *p, Long batch_size, Long x_stride,
    Long y_stride, bool inplace) {
  assert(p);
  return p->create(p, batch_size, x_stride, y_stride, inplace);
}

static Long NS_(EvenSizeBatch_ChunkSize)(Long dft_size) {
  const Long data_size = CE_SIZE(2 * dft_size);
  const Long chunk_size = (Long)(C_BUFFER_SIZE / data_size);
  return MAX(chunk_size, C_SIMD_SIZE_S);
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeOuterBatch_F_new_)(
    struct NS_(OuterBatchFactory_F) *factory, Long dft_size, Long batch_size,
    Long x_stride_1, Long y_stride_1, bool inplace, int num_threads) {
  const Long chunk_size = NS_(EvenSizeBatch_ChunkSize)(dft_size);
  if (batch_size <= chunk_size) {
    return NS_(OuterBatchFactory_F_create)(factory, batch_size, x_stride_1,
                                           y_stride_1, inplace);
  } else {
    const Long residual = batch_size % chunk_size;
    if (residual == 0) {
      return NS(RealToComplexFunctionLoop_new_)(
          NS_(OuterBatchFactory_F_create)(factory, chunk_size, x_stride_1,
                                          y_stride_1, inplace),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    } else {
      return NS(RealToComplexFunctionLoopR_new_)(
          NS_(OuterBatchFactory_F_create)(factory, chunk_size, x_stride_1,
                                          y_stride_1, inplace),
          NS_(OuterBatchFactory_F_create)(factory, residual, x_stride_1,
                                          y_stride_1, inplace),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    }
  }
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeBatch_F_new_1_)(
    struct NS_(OuterBatchFactory_F) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_1) {
  const Long x_data_size = 2 * dft_size;
  return NS(RealToComplexFunctionPair_C1_new)(
      NS(Real_Copy2D_new)(x_data_size, batch_size, x_stride_0, x_stride_1, 1,
                          2 * y_stride_1),
      NS_(OuterBatchFactory_F_create)(factory, batch_size, 2 * y_stride_1,
                                      y_stride_1, true));
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeBatch_F_new_1)(
    struct NS_(OuterBatchFactory_F) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_1, int num_threads) {
  const Long chunk_size = NS_(EvenSizeBatch_ChunkSize)(dft_size);
  if (batch_size <= chunk_size) {
    return NS_(EvenSizeBatch_F_new_1_)(factory, dft_size, batch_size,
                                       x_stride_0, x_stride_1, y_stride_1);
  } else {
    const Long residual = batch_size % chunk_size;
    if (residual == 0) {
      return NS(RealToComplexFunctionLoop_new_)(
          NS_(EvenSizeBatch_F_new_1_)(factory, dft_size, chunk_size, x_stride_0,
                                      x_stride_1, y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    } else {
      return NS(RealToComplexFunctionLoopR_new_)(
          NS_(EvenSizeBatch_F_new_1_)(factory, dft_size, chunk_size, x_stride_0,
                                      x_stride_1, y_stride_1),
          NS_(EvenSizeBatch_F_new_1_)(factory, dft_size, residual, x_stride_0,
                                      x_stride_1, y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    }
  }
}

static struct NS(RealToComplexFunctionW) *NS_(EvenSizeBatch_F_newW_2_)(
    struct NS_(OuterBatchFactory_F) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1) {
  const Long x_data_size = 2 * dft_size;
  const Long y_data_size = CE_SIZE(2 * dft_size);
  const Long buffer_size = y_data_size * batch_size * sizeof(Complex);
  return NS(RealToComplexFunctionTripleW_new)(
      NS(Real_Copy2D_new)(x_data_size, batch_size, x_stride_0, x_stride_1, 1,
                          2 * y_data_size),
      NS_(OuterBatchFactory_F_create)(factory, batch_size, 2 * y_data_size,
                                      y_data_size, true),
      NS(Complex_Copy2D_new)(y_data_size, batch_size, 1, y_data_size,
                             y_stride_0, y_stride_1),
      buffer_size);
}

static struct NS(RealToComplexFunction) *NS_(EvenSizeBatch_F_new_2)(
    struct NS_(OuterBatchFactory_F) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const Long chunk_size = NS_(EvenSizeBatch_ChunkSize)(dft_size);
  if (batch_size <= chunk_size) {
    return NS(RealToComplexFunctionBuffered_new)(
        NS_(EvenSizeBatch_F_newW_2_)(factory, dft_size, batch_size, x_stride_0,
                                     x_stride_1, y_stride_0, y_stride_1));
  } else {
    const Long residual = batch_size % chunk_size;
    if (residual == 0) {
      return NS(RealToComplexFunctionBufferedLoop_new_)(
          NS_(EvenSizeBatch_F_newW_2_)(factory, dft_size, chunk_size,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    } else {
      return NS(RealToComplexFunctionBufferedLoopR_new_)(
          NS_(EvenSizeBatch_F_newW_2_)(factory, dft_size, chunk_size,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1),
          NS_(EvenSizeBatch_F_newW_2_)(factory, dft_size, residual, x_stride_0,
                                       x_stride_1, y_stride_0, y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    }
  }
}

struct NS(RealToComplexFunction) *NS_(EvenSizeBatch_F_new)(
    struct NS_(OuterBatchFactory_F) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  if (x_stride_0 == 1 && y_stride_0 == 1 &&
      (batch_size == 1 || x_stride_1 % 2 == 0)) {
    return NS_(EvenSizeOuterBatch_F_new_)(factory, dft_size, batch_size,
                                          x_stride_1, y_stride_1, inplace,
                                          num_threads);
  } else if (!inplace && y_stride_0 == 1) {
    return NS_(EvenSizeBatch_F_new_1)(factory, dft_size, batch_size, x_stride_0,
                                      x_stride_1, y_stride_1, num_threads);
  } else {
    return NS_(EvenSizeBatch_F_new_2)(factory, dft_size, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  }
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeOuterBatch_B_new_)(
    struct NS_(OuterBatchFactory_B) *factory, Long dft_size, Long batch_size,
    Long x_stride_1, Long y_stride_1, bool inplace, int num_threads) {
  const Long chunk_size = NS_(EvenSizeBatch_ChunkSize)(dft_size);
  if (batch_size <= chunk_size) {
    return NS_(OuterBatchFactory_B_create)(factory, batch_size, x_stride_1,
                                           y_stride_1, inplace);
  } else {
    const Long residual = batch_size % chunk_size;
    if (residual == 0) {
      return NS(ComplexToRealFunctionLoop_new_)(
          NS_(OuterBatchFactory_B_create)(factory, chunk_size, x_stride_1,
                                          y_stride_1, inplace),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    } else {
      return NS(ComplexToRealFunctionLoopR_new_)(
          NS_(OuterBatchFactory_B_create)(factory, chunk_size, x_stride_1,
                                          y_stride_1, inplace),
          NS_(OuterBatchFactory_B_create)(factory, residual, x_stride_1,
                                          y_stride_1, inplace),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    }
  }
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeBatch_B_new_0_)(
    struct NS_(OuterBatchFactory_B) *factory, Long dft_size, Long batch_size,
    Long x_stride_1, Long y_stride_0, Long y_stride_1) {
  const Long y_data_size = 2 * dft_size;
  return NS(ComplexToRealFunctionPair_C0_new)(
      NS_(OuterBatchFactory_B_create)(factory, batch_size, x_stride_1,
                                      2 * x_stride_1, true),
      NS(Real_Copy2D_new)(y_data_size, batch_size, 1, 2 * x_stride_1,
                          y_stride_0, y_stride_1));
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeBatch_B_new_0)(
    struct NS_(OuterBatchFactory_B) *factory, Long dft_size, Long batch_size,
    Long x_stride_1, Long y_stride_0, Long y_stride_1, int num_threads) {
  const Long chunk_size = NS_(EvenSizeBatch_ChunkSize)(dft_size);
  if (batch_size <= chunk_size) {
    return NS_(EvenSizeBatch_B_new_0_)(factory, dft_size, batch_size,
                                       x_stride_1, y_stride_0, y_stride_1);
  } else {
    const Long residual = batch_size % chunk_size;
    if (residual == 0) {
      return NS(ComplexToRealFunctionLoop_new_)(
          NS_(EvenSizeBatch_B_new_0_)(factory, dft_size, chunk_size, x_stride_1,
                                      y_stride_0, y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    } else {
      return NS(ComplexToRealFunctionLoopR_new_)(
          NS_(EvenSizeBatch_B_new_0_)(factory, dft_size, chunk_size, x_stride_1,
                                      y_stride_0, y_stride_1),
          NS_(EvenSizeBatch_B_new_0_)(factory, dft_size, residual, x_stride_1,
                                      y_stride_0, y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    }
  }
}

static struct NS(ComplexToRealFunctionW) *NS_(EvenSizeBatch_B_newW_2_)(
    struct NS_(OuterBatchFactory_B) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1) {
  const Long x_data_size = CE_SIZE(2 * dft_size);
  const Long y_data_size = 2 * dft_size;
  const Long buffer_size = x_data_size * batch_size * sizeof(Complex);
  return NS(ComplexToRealFunctionTripleW_new)(
      NS(Complex_Copy2D_new)(x_data_size, batch_size, x_stride_0, x_stride_1, 1,
                             x_data_size),
      NS_(OuterBatchFactory_B_create)(factory, batch_size, x_data_size,
                                      2 * x_data_size, true),
      NS(Real_Copy2D_new)(y_data_size, batch_size, 1, 2 * x_data_size,
                          y_stride_0, y_stride_1),
      buffer_size);
}

static struct NS(ComplexToRealFunction) *NS_(EvenSizeBatch_B_new_2)(
    struct NS_(OuterBatchFactory_B) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  const Long chunk_size = NS_(EvenSizeBatch_ChunkSize)(dft_size);
  if (batch_size <= chunk_size) {
    return NS(ComplexToRealFunctionBuffered_new)(
        NS_(EvenSizeBatch_B_newW_2_)(factory, dft_size, batch_size, x_stride_0,
                                     x_stride_1, y_stride_0, y_stride_1));
  } else {
    const Long residual = batch_size % chunk_size;
    if (residual == 0) {
      return NS(ComplexToRealFunctionBufferedLoop_new_)(
          NS_(EvenSizeBatch_B_newW_2_)(factory, dft_size, chunk_size,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    } else {
      return NS(ComplexToRealFunctionBufferedLoopR_new_)(
          NS_(EvenSizeBatch_B_newW_2_)(factory, dft_size, chunk_size,
                                       x_stride_0, x_stride_1, y_stride_0,
                                       y_stride_1),
          NS_(EvenSizeBatch_B_newW_2_)(factory, dft_size, residual, x_stride_0,
                                       x_stride_1, y_stride_0, y_stride_1),
          batch_size / chunk_size, x_stride_1 * chunk_size,
          y_stride_1 * chunk_size, num_threads);
    }
  }
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeBatch_B_new)(
    struct NS_(OuterBatchFactory_B) *factory, Long dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    bool inplace, int num_threads) {
  if ((x_stride_0 == 1 && y_stride_0 == 1 &&
       (batch_size == 1 || y_stride_1 % 2 == 0))) {
    return NS_(EvenSizeOuterBatch_B_new_)(factory, dft_size, batch_size,
                                          x_stride_1, y_stride_1, inplace,
                                          num_threads);
  } else if (!inplace && x_stride_0 == 1) {
    return NS_(EvenSizeBatch_B_new_0)(factory, dft_size, batch_size, x_stride_1,
                                      y_stride_0, y_stride_1, num_threads);
  } else {
    return NS_(EvenSizeBatch_B_new_2)(factory, dft_size, batch_size, x_stride_0,
                                      x_stride_1, y_stride_0, y_stride_1,
                                      num_threads);
  }
}
