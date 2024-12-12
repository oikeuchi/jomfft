#include "ComplexTensorTest.h"

#include <jomfft/util/ComplexTensor.h>

static void ComplexTensorTest_1() {
  Complex c0 = Complex_(1, 1);
  struct NS(ComplexTensor) *x = NS(ComplexTensor_new1)(4);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get1)(x, 0), Complex_R(0));
  NS(ComplexTensor_set1)(x, 1, c0);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get1)(x, 1), c0);
  struct NS(ComplexTensor) *y = NS(ComplexTensor_new1)(4);
  NS(ComplexTensor_set1)(y, 1, c0);
  ASSERT_COMPLEXTENSOR_EQ(x, y);
  ASSERT_COMPLEXTENSOR_AEQ(x, y);
  NS(ComplexTensor_set1)(y, 2, Complex_R(2));
  ASSERT_FALSE(NS(ComplexTensor_equal(x, y)));
  struct NS(ComplexTensor) *z = NS(ComplexTensor_new1)(3);
  NS(ComplexTensor_set1)(z, 1, Complex_R(1));
  ASSERT_FALSE(NS(ComplexTensor_equal(x, z)));
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y);
  NS(ComplexTensor_delete)(z);
}

static void ComplexTensorTest_2() {
  Complex c0 = Complex_(1, 1);
  struct NS(ComplexTensor) *x = NS(ComplexTensor_new2)(2, 3);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get2)(x, 0, 0), Complex_R(0));
  NS(ComplexTensor_set2)(x, 1, 1, c0);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get2)(x, 1, 1), c0);
  struct NS(ComplexTensor) *y = NS(ComplexTensor_new2)(2, 3);
  NS(ComplexTensor_set2)(y, 1, 1, c0);
  ASSERT_COMPLEXTENSOR_EQ(x, y);
  ASSERT_COMPLEXTENSOR_AEQ(x, y);
  NS(ComplexTensor_set2)(y, 0, 2, Complex_R(2));
  ASSERT_FALSE(NS(ComplexTensor_equal(x, y)));
  struct NS(ComplexTensor) *z = NS(ComplexTensor_new1)(3);
  ASSERT_FALSE(NS(ComplexTensor_equal(x, z)));
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y);
  NS(ComplexTensor_delete)(z);
}

static void ComplexTensorTest_3() {
  Complex c0 = Complex_(1, 1);
  struct NS(ComplexTensor) *x = NS(ComplexTensor_new3)(2, 3, 2);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get3)(x, 0, 1, 0), Complex_R(0));
  NS(ComplexTensor_set3)(x, 1, 1, 0, c0);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get3)(x, 1, 1, 0), c0);
  struct NS(ComplexTensor) *y = NS(ComplexTensor_new3)(2, 3, 2);
  NS(ComplexTensor_set3)(y, 1, 1, 0, c0);
  ASSERT_COMPLEXTENSOR_EQ(x, y);
  ASSERT_COMPLEXTENSOR_AEQ(x, y);
  NS(ComplexTensor_set3)(y, 0, 2, 0, Complex_R(2));
  ASSERT_FALSE(NS(ComplexTensor_equal(x, y)));
  struct NS(ComplexTensor) *z = NS(ComplexTensor_new2)(2, 3);
  ASSERT_FALSE(NS(ComplexTensor_equal(x, z)));
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y);
  NS(ComplexTensor_delete)(z);
}

static void ComplexTensorTest_v() {
  Complex c0 = Complex_(1, 1);
  Long sizes[4] = {2, 3, 2, 3};
  Long indexes_0[4] = {0, 1, 1, 0};
  Long indexes_1[4] = {0, 1, 1, 1};
  struct NS(ComplexTensor) *x = NS(ComplexTensor_new)(4, sizes);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get)(x, indexes_0), Complex_R(0));
  NS(ComplexTensor_set)(x, indexes_0, c0);
  ASSERT_COMPLEX_EQ(NS(ComplexTensor_get)(x, indexes_0), c0);
  struct NS(ComplexTensor) *y = NS(ComplexTensor_new)(4, sizes);
  NS(ComplexTensor_set)(y, indexes_0, c0);
  ASSERT_COMPLEXTENSOR_EQ(x, y);
  ASSERT_COMPLEXTENSOR_AEQ(x, y);
  NS(ComplexTensor_set)(y, indexes_1, Complex_R(2));
  ASSERT_FALSE(NS(ComplexTensor_equal(x, y)));
  struct NS(ComplexTensor) *z = NS(ComplexTensor_new2)(2, 3);
  ASSERT_FALSE(NS(ComplexTensor_equal(x, z)));
  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y);
  NS(ComplexTensor_delete)(z);
}

void NS(ComplexTensorTest)() {
  ComplexTensorTest_1();
  ComplexTensorTest_2();
  ComplexTensorTest_3();
  ComplexTensorTest_v();
}
