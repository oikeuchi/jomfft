#include "RegisterTest.h"

#include "BatchDftTest.h"
#include "DftTest.h"
#include "EvenSizeBatchDftTest.h"
#include "EvenSizeDftTest.h"

void NS(RDftCooleyTurkey_RegisterTest)() {
  REGISTER_TEST(RDftCooleyTurkey_EvenSizeDft_FTest);
  REGISTER_TEST(RDftCooleyTurkey_EvenSizeDft_BTest);
  REGISTER_TEST(RDftCooleyTurkey_EvenSizeBatchDft_FTest);
  REGISTER_TEST(RDftCooleyTurkey_EvenSizeBatchDft_BTest);
  REGISTER_TEST(RDftCooleyTurkey_Dft_FTest);
  REGISTER_TEST(RDftCooleyTurkey_Dft_BTest);
  REGISTER_TEST(RDftCooleyTurkey_BatchDft_FTest);
  REGISTER_TEST(RDftCooleyTurkey_BatchDft_BTest);
}
