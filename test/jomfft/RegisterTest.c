#include "RegisterTest.h"

#include "CommonTest.h"
#include "DftTest.h"
#include "cdft/RegisterTest.h"
#include "math/RegisterTest.h"
#include "rdft/RegisterTest.h"
#include "util/RegisterTest.h"

void NS(RegisterTest)() {
  NS(Math_RegisterTest)();
  NS(Util_RegisterTest)();
  NS(CDft_RegisterTest)();
  NS(RDft_RegisterTest)();
  REGISTER_TEST(VersionTest);
  REGISTER_TEST(MaxThreadsTest);
  REGISTER_TEST(ComplexDftTest);
  REGISTER_TEST(ComplexDftBatchTest);
  REGISTER_TEST(ComplexDftBatch_Test);
  REGISTER_TEST(RealDftTest);
  REGISTER_TEST(RealDftBatchTest);
  REGISTER_TEST(RealDftBatch_Test);
}
