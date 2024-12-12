#include "RegisterTest.h"

#include "InnerBatchDftTest.h"
#include "StrideBatchDftTest.h"
#include "impl/RegisterTest.h"

void NS(CDftSingleton_RegisterTest)() {
  NS(CDftSingletonImpl_RegisterTest)();
  REGISTER_TEST(CDftSingleton_InnerBatchDftTest);
  REGISTER_TEST(CDftSingleton_StrideBatchDft_1Test);
  REGISTER_TEST(CDftSingleton_StrideBatchDftTest);
}
