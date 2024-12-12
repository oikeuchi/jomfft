#include "RegisterTest.h"

#include "BatchDftTest.h"
#include "DftTest.h"
#include "bluestein/RegisterTest.h"
#include "cooleyturkey/RegisterTest.h"
#include "core/RegisterTest.h"
#include "singleton/RegisterTest.h"
#include "square/RegisterTest.h"

void NS(RDft_RegisterTest)() {
  NS(RDftCore_RegisterTest)();
  NS(RDftSingleton_RegisterTest)();
  NS(RDftSquare_RegisterTest)();
  NS(RDftCooleyTurkey_RegisterTest)();
  NS(RDftBluestein_RegisterTest)();
  REGISTER_TEST(RDft_Dft_FTest);
  REGISTER_TEST(RDft_Dft_BTest);
  REGISTER_TEST(RDft_BatchDft_FTest);
  REGISTER_TEST(RDft_BatchDft_BTest);
}
