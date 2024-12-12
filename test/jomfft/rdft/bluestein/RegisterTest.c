#include "RegisterTest.h"

#include "BatchDftTest.h"
#include "DftTest.h"
#include "EvenSizeBatchDftTest.h"
#include "EvenSizeDftTest.h"

void NS(RDftBluestein_RegisterTest)() {
  REGISTER_TEST(RDftBluestein_EvenSizeDft_FTest);
  REGISTER_TEST(RDftBluestein_EvenSizeDft_BTest);
  REGISTER_TEST(RDftBluestein_EvenSizeBatchDft_FTest);
  REGISTER_TEST(RDftBluestein_EvenSizeBatchDft_BTest);
  REGISTER_TEST(RDftBluestein_Dft_FTest);
  REGISTER_TEST(RDftBluestein_Dft_BTest);
  REGISTER_TEST(RDftBluestein_BatchDft_FTest);
  REGISTER_TEST(RDftBluestein_BatchDft_BTest);
}
