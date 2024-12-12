#include "RegisterTest.h"

#include "BatchDftTest.h"
#include "DftTest.h"

void NS(CDftBluestein_RegisterTest)() {
  REGISTER_TEST(CDftBluestein_DftTest);
  REGISTER_TEST(CDftBluestein_BatchDftTest);
}
