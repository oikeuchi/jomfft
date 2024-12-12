#include "RegisterTest.h"

#include "BatchMultiplyTest.h"
#include "InnerBatchDftTest.h"
#include "PairSquareTwiddleDftTest.h"
#include "SquareSplitTwiddleDftTest.h"
#include "StrideBatchDftTest.h"
#include "TransposeDftTwiddleTest.h"
#include "TwiddleDftTest.h"
#include "impl/RegisterTest.h"

void NS(CDftCore_RegisterTest)() {
  NS(CDftCoreImpl_RegisterTest)();
  REGISTER_TEST(CDftCore_InnerBatchDftTest);
  REGISTER_TEST(CDftCore_NestedInnerBatchDftTest);
  REGISTER_TEST(CDftCore_StrideBatchDft_1Test);
  REGISTER_TEST(CDftCore_StrideBatchDftTest);
  REGISTER_TEST(CDftCore_TwiddleDftTest);
  REGISTER_TEST(CDftCore_PairSquareTwiddleDftTest);
  REGISTER_TEST(CDftCore_TransposeDftTwiddleTest);
  REGISTER_TEST(CDftCore_SquareSplitTwiddleDftTest);
  REGISTER_TEST(CDftCore_BatchMultiplyTest);
}
