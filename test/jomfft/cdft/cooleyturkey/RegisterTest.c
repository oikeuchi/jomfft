#include "RegisterTest.h"

#include "BatchDftTest.h"
#include "DftTest.h"
#include "TransposeDftTest.h"

void NS(CDftCooleyTurkey_RegisterTest)() {
  REGISTER_TEST(CDftCooleyTurkey_TransposeDftTest);
  REGISTER_TEST(CDftCooleyTurkey_DftTest);
  REGISTER_TEST(CDftCooleyTurkey_BatchDftTest);
}
