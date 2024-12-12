#include "RegisterTest.h"

#include "BatchDftTest.h"
#include "DftTest.h"
#include "LargeBatchDftTest.h"
#include "LargeDftTest.h"
#include "NestedBatchDftTest.h"
#include "NestedDftTest.h"
#include "PairTwiddleTransposeTest.h"
#include "SmallBatchDftTest.h"
#include "SmallDftTest.h"
#include "SmallTransposeDftTest.h"
#include "TwiddleTransposeTest.h"

void NS(CDftSquare_RegisterTest)() {
  REGISTER_TEST(CDftSquare_SmallDft_1Test);
  REGISTER_TEST(CDftSquare_SmallDftTest);
  REGISTER_TEST(CDftSquare_SmallTransposeDftTest);
  REGISTER_TEST(CDftSquare_SmallBatchDftTest);
  REGISTER_TEST(CDftSquare_NestedDft_1Test);
  REGISTER_TEST(CDftSquare_NestedDftTest);
  REGISTER_TEST(CDftSquare_NestedBatchDftTest);
  REGISTER_TEST(CDftSquare_TwiddleTransposeTest);
  REGISTER_TEST(CDftSquare_PairTwiddleTransposeTest);
  REGISTER_TEST(CDftSquare_LargeDft_1Test);
  REGISTER_TEST(CDftSquare_LargeDftTest);
  REGISTER_TEST(CDftSquare_LargeBatchDftTest);
  REGISTER_TEST(CDftSquare_DftTest);
  REGISTER_TEST(CDftSquare_BatchDftTest);
}
