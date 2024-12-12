#include "DftTest.h"

#include <jomfft/rdft/Dft.h>

#include "DftTest_.h"

#define NS_(n) NS(RDft_##n)

static struct NS(RealToComplexFunction) *
DftFactory_F_create(struct NS_(DftFactory_F) *p, struct NS(DataFactory) *data,
                    Long dft_size, Long x_stride, Long y_stride, bool inplace,
                    int num_threads) {
  (void)p;
  return NS_(Dft_F_new)(data, dft_size, x_stride, y_stride, inplace,
                        num_threads);
}

void NS_(Dft_FTest)() {
  struct NS_(DftFactory_F) *factory =
      NS_(DftFactory_F_new)(DftFactory_F_create, NULL, NULL);
  NS_(Dft_FTest_)(factory, 8);
  NS_(Dft_FTest_)(factory, 61);
  NS_(Dft_FTest_)(factory, 256);
  NS_(DftFactory_F_delete)(factory);
}

static struct NS(ComplexToRealFunction) *
DftFactory_B_create(struct NS_(DftFactory_B) *p, struct NS(DataFactory) *data,
                    Long dft_size, Long x_stride, Long y_stride, bool inplace,
                    int num_threads) {
  (void)p;
  return NS_(Dft_B_new)(data, dft_size, x_stride, y_stride, inplace,
                        num_threads);
}

void NS_(Dft_BTest)() {
  struct NS_(DftFactory_B) *factory =
      NS_(DftFactory_B_new)(DftFactory_B_create, NULL, NULL);
  NS_(Dft_BTest_)(factory, 8);
  NS_(Dft_BTest_)(factory, 61);
  NS_(Dft_BTest_)(factory, 256);
  NS_(DftFactory_B_delete)(factory);
}
