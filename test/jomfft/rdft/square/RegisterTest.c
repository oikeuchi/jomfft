#include "RegisterTest.h"

#include "EvenSizeBatchDftTest.h"
#include "EvenSizeDftTest.h"

void NS(RDftSquare_RegisterTest)() {
  REGISTER_TEST(RDftSquare_EvenSizeDft_FTest);
  REGISTER_TEST(RDftSquare_EvenSizeDft_BTest);
  REGISTER_TEST(RDftSquare_EvenSizeBatchDft_FTest);
  REGISTER_TEST(RDftSquare_EvenSizeBatchDft_BTest);
}
