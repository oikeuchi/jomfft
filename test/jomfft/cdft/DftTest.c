#include "DftTest.h"

#include <jomfft/cdft/Dft.h>

#include "DftTest_.h"

#define NS_(n) NS(CDft_##n)

static struct NS(ComplexFunction) *
DftFactory_create(struct NS_(DftFactory) *p, struct NS(DataFactory) *data,
                  bool backward, Long dft_size, Long x_stride, Long y_stride,
                  bool inplace, int num_threads) {
  (void)p;
  return NS_(Dft_new)(data, backward, dft_size, x_stride, y_stride, inplace,
                      num_threads);
}

void NS_(DftTest)() {
  struct NS_(DftFactory) *factory =
      NS_(DftFactory_new)(DftFactory_create, NULL, NULL);
  NS_(DftTest_)(factory, 4);
  NS_(DftTest_)(factory, 31);
  NS_(DftTest_)(factory, 256);
  NS_(DftFactory_delete)(factory);
}
