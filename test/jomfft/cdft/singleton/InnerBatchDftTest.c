#include "../InnerBatchDftTest.h"

#include <jomfft/cdft/singleton/InnerBatchDft.h>

#include "InnerBatchDftTest.h"

static void CDftSingleton_InnerBatchDftTest__(bool backward, int dft_size,
                                              Long batch_size,
                                              int num_threads) {
  CAPTURE_INT(num_threads);
  struct NS(DataFactory) *data = NS(DataFactory_new)();
  struct NS(ComplexFunction) *p =
      NS(CDftSingleton_InnerBatchDft_new)(data, backward, dft_size, batch_size,
                                          batch_size, batch_size, num_threads);
  NS(CDft_InnerBatchDftTest)(p, backward, dft_size, batch_size, false);
  NS(ComplexFunction_delete)(p);
  NS(DataFactory_delete)(data);
}

static void CDftSingleton_InnerBatchDftTest_(bool backward, int dft_size,
                                             Long batch_size) {
  CDftSingleton_InnerBatchDftTest__(backward, dft_size, batch_size, 1);
}

void NS(CDftSingleton_InnerBatchDftTest)() {
  CDftSingleton_InnerBatchDftTest_(false, 61, 1);
  CDftSingleton_InnerBatchDftTest_(false, 22, 2);
  CDftSingleton_InnerBatchDftTest_(true, 57, 3);
  CDftSingleton_InnerBatchDftTest_(true, 28, 4);
  CDftSingleton_InnerBatchDftTest_(false, 11, 5);
#if USE_OPENMP
  CDftSingleton_InnerBatchDftTest__(false, 11, 4, 2);
  CDftSingleton_InnerBatchDftTest__(true, 11, 3, 2);
  CDftSingleton_InnerBatchDftTest__(false, 22, 5, 2);
  CDftSingleton_InnerBatchDftTest__(true, 22, 8, 2);
#endif
}
