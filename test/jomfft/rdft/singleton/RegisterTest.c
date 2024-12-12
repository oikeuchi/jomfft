#include "RegisterTest.h"

#include "StrideBatchDftTest.h"
#include "impl/RegisterTest.h"

void NS(RDftSingleton_RegisterTest)() {
  NS(RDftSingletonImpl_RegisterTest)();
  REGISTER_TEST(RDftSingleton_StrideBatchDft_F_1Test);
  REGISTER_TEST(RDftSingleton_StrideBatchDft_B_1Test);
  REGISTER_TEST(RDftSingleton_StrideBatchDft_FTest);
  REGISTER_TEST(RDftSingleton_StrideBatchDft_BTest);
}
