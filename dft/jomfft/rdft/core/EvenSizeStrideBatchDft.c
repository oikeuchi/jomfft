#include "EvenSizeStrideBatchDft.h"

#include "../../cdft/core/StrideBatchDft.h"
#include "../EvenSizeBatch.h"
#include "../EvenSizeDft.h"

#define NS_(n) NS(RDftCore_##n)

struct NS_(EvenSizeOuterBatchDftFactoryData) {
  struct NS(DataFactory) *data;
  int dft_size;
};

static struct NS_(EvenSizeOuterBatchDftFactoryData)
    *NS_(EvenSizeOuterBatchDftFactoryData_new)(struct NS(DataFactory) *data,
                                               int dft_size) {
  struct NS_(EvenSizeOuterBatchDftFactoryData) *out =
      NEW(struct NS_(EvenSizeOuterBatchDftFactoryData));
  *out = (struct NS_(EvenSizeOuterBatchDftFactoryData)){.data = data,
                                                        .dft_size = dft_size};
  return out;
}

static struct NS(RealToComplexFunction)
    *NS_(EvenSizeOuterBatchDftFactory_F_create)(
        struct NS(RDft_OuterBatchFactory_F) *p, Long batch_size, Long x_stride,
        Long y_stride, bool inplace) {
  assert(batch_size == 1 || x_stride % 2 == 0);
  (void)inplace;
  struct NS_(EvenSizeOuterBatchDftFactoryData) *data =
      NS(RDft_OuterBatchFactory_F_data)(p);
  const Long x_stride_1 = batch_size == 1 ? 1 : x_stride / 2;
  return NS(RDft_EvenSizeOuterBatchDft_F_new)(
      data->data,
      NS(CDftCore_StrideBatchDft_new)(false, data->dft_size, batch_size, 1,
                                      x_stride_1, 1, y_stride),
      data->dft_size, batch_size, y_stride);
}

static struct NS(ComplexToRealFunction)
    *NS_(EvenSizeOuterBatchDftFactory_B_create)(
        struct NS(RDft_OuterBatchFactory_B) *p, Long batch_size, Long x_stride,
        Long y_stride, bool inplace) {
  assert(batch_size == 1 || y_stride % 2 == 0);
  (void)inplace;
  struct NS_(EvenSizeOuterBatchDftFactoryData) *data =
      NS(RDft_OuterBatchFactory_B_data)(p);
  const Long y_stride_1 = batch_size == 1 ? 1 : y_stride / 2;
  return NS(RDft_EvenSizeOuterBatchDft_B_new)(
      data->data,
      NS(CDftCore_StrideBatchDft_new)(true, data->dft_size, batch_size, 1,
                                      x_stride, 1, y_stride_1),
      data->dft_size, batch_size, x_stride);
}

static struct NS(RDft_OuterBatchFactory_F)
    *NS_(EvenSizeOuterBatchDftFactory_F_new)(struct NS(DataFactory) *data,
                                             int dft_size) {
  return NS(RDft_OuterBatchFactory_F_new)(
      NS_(EvenSizeOuterBatchDftFactory_F_create), jomfftDestructPointer,
      NS_(EvenSizeOuterBatchDftFactoryData_new)(data, dft_size));
}

static struct NS(RDft_OuterBatchFactory_B)
    *NS_(EvenSizeOuterBatchDftFactory_B_new)(struct NS(DataFactory) *data,
                                             int dft_size) {
  return NS(RDft_OuterBatchFactory_B_new)(
      NS_(EvenSizeOuterBatchDftFactory_B_create), jomfftDestructPointer,
      NS_(EvenSizeOuterBatchDftFactoryData_new)(data, dft_size));
}

bool NS_(EvenSizeStrideBatchDft_HasKey)(int dft_size) {
  return NS(CDftCore_StrideBatchDft_HasKey)(dft_size);
}

struct NS(RealToComplexFunction) *NS_(EvenSizeStrideBatchDft_F_new)(
    struct NS(DataFactory) *data, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  struct NS(RDft_OuterBatchFactory_F) *factory =
      NS_(EvenSizeOuterBatchDftFactory_F_new)(data, dft_size);
  struct NS(RealToComplexFunction) *out = NS(RDft_EvenSizeBatch_F_new)(
      factory, dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, false, num_threads);
  NS(RDft_OuterBatchFactory_F_delete)(factory);
  return out;
}

struct NS(ComplexToRealFunction) *NS_(EvenSizeStrideBatchDft_B_new)(
    struct NS(DataFactory) *data, int dft_size, Long batch_size,
    Long x_stride_0, Long x_stride_1, Long y_stride_0, Long y_stride_1,
    int num_threads) {
  struct NS(RDft_OuterBatchFactory_B) *factory =
      NS_(EvenSizeOuterBatchDftFactory_B_new)(data, dft_size);
  struct NS(ComplexToRealFunction) *out = NS(RDft_EvenSizeBatch_B_new)(
      factory, dft_size, batch_size, x_stride_0, x_stride_1, y_stride_0,
      y_stride_1, false, num_threads);
  NS(RDft_OuterBatchFactory_B_delete)(factory);
  return out;
}
