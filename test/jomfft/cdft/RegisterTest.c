#include "RegisterTest.h"

#include "BatchDftTest.h"
#include "DftTest.h"
#include "bluestein/RegisterTest.h"
#include "cooleyturkey/RegisterTest.h"
#include "core/RegisterTest.h"
#include "singleton/RegisterTest.h"
#include "square/RegisterTest.h"

void NS(CDft_RegisterTest)() {
  NS(CDftCore_RegisterTest)();
  NS(CDftCooleyTurkey_RegisterTest)();
  NS(CDftSquare_RegisterTest)();
  NS(CDftSingleton_RegisterTest)();
  NS(CDftBluestein_RegisterTest)();
  REGISTER_TEST(CDft_DftTest);
  REGISTER_TEST(CDft_BatchDftTest);
}
