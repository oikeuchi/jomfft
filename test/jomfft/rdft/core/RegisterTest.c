#include "RegisterTest.h"

#include "ComplexToPairDftTest.h"
#include "EvenSizeStrideBatchDftTest.h"
#include "EvenSizeTwiddleDftTest.h"
#include "StrideBatchDftTest.h"
#include "impl/RegisterTest.h"

void NS(RDftCore_RegisterTest)() {
  NS(RDftCoreImpl_RegisterTest)();
  REGISTER_TEST(RDftCore_StrideBatchDft_F_1Test);
  REGISTER_TEST(RDftCore_StrideBatchDft_B_1Test);
  REGISTER_TEST(RDftCore_StrideBatchDft_FTest);
  REGISTER_TEST(RDftCore_StrideBatchDft_BTest);
  REGISTER_TEST(RDftCore_EvenSizeTwiddleDft_FTest);
  REGISTER_TEST(RDftCore_EvenSizeTwiddleDft_BTest);
  REGISTER_TEST(RDftCore_EvenSizeStrideBatchDft_FTest);
  REGISTER_TEST(RDftCore_EvenSizeStrideBatchDft_BTest);
  REGISTER_TEST(RDftCore_ComplexToPairDft_FTest);
  REGISTER_TEST(RDftCore_ComplexToPairDft_BTest);
}
