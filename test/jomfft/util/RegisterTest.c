#include "RegisterTest.h"

#include "ComplexTensorTest.h"
#include "ComplexTransposeTest.h"
#include "RealTensorTest.h"
#include "TensorShapeTest.h"

void NS(Util_RegisterTest)() {
  REGISTER_TEST(TensorShapeTest);
  REGISTER_TEST(RealTensorTest);
  REGISTER_TEST(ComplexTensorTest);
  REGISTER_TEST(ComplexTransposeTest);
}
