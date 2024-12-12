#include "RealTensorTest.h"

#include <jomfft/util/RealTensor.h>

static void RealTensorTest_1() {
  struct NS(RealTensor) *x = NS(RealTensor_new1)(4);
  ASSERT_REAL_EQ(NS(RealTensor_get1)(x, 0), 0);
  NS(RealTensor_set1)(x, 1, (Real)1);
  ASSERT_REAL_EQ(NS(RealTensor_get1)(x, 1), 1);
  struct NS(RealTensor) *y = NS(RealTensor_new1)(4);
  NS(RealTensor_set1)(y, 1, (Real)1);
  ASSERT_REALTENSOR_EQ(x, y);
  ASSERT_REALTENSOR_AEQ(x, y);
  NS(RealTensor_set1)(y, 2, (Real)2);
  ASSERT_FALSE(NS(RealTensor_equal(x, y)));
  struct NS(RealTensor) *z = NS(RealTensor_new1)(3);
  NS(RealTensor_set1)(z, 1, (Real)1);
  ASSERT_FALSE(NS(RealTensor_equal(x, z)));
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(y);
  NS(RealTensor_delete)(z);
}

static void RealTensorTest_2() {
  struct NS(RealTensor) *x = NS(RealTensor_new2)(2, 3);
  ASSERT_REAL_EQ(NS(RealTensor_get2)(x, 0, 0), 0);
  NS(RealTensor_set2)(x, 1, 1, (Real)1);
  ASSERT_REAL_EQ(NS(RealTensor_get2)(x, 1, 1), 1);
  struct NS(RealTensor) *y = NS(RealTensor_new2)(2, 3);
  NS(RealTensor_set2)(y, 1, 1, (Real)1);
  ASSERT_REALTENSOR_EQ(x, y);
  ASSERT_REALTENSOR_AEQ(x, y);
  NS(RealTensor_set2)(y, 0, 2, (Real)2);
  ASSERT_FALSE(NS(RealTensor_equal(x, y)));
  struct NS(RealTensor) *z = NS(RealTensor_new1)(3);
  ASSERT_FALSE(NS(RealTensor_equal(x, z)));
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(y);
  NS(RealTensor_delete)(z);
}

static void RealTensorTest_3() {
  struct NS(RealTensor) *x = NS(RealTensor_new3)(2, 3, 2);
  ASSERT_REAL_EQ(NS(RealTensor_get3)(x, 0, 1, 0), 0);
  NS(RealTensor_set3)(x, 1, 1, 0, (Real)1);
  ASSERT_REAL_EQ(NS(RealTensor_get3)(x, 1, 1, 0), 1);
  struct NS(RealTensor) *y = NS(RealTensor_new3)(2, 3, 2);
  NS(RealTensor_set3)(y, 1, 1, 0, (Real)1);
  ASSERT_REALTENSOR_EQ(x, y);
  ASSERT_REALTENSOR_AEQ(x, y);
  NS(RealTensor_set3)(y, 0, 2, 0, (Real)2);
  ASSERT_FALSE(NS(RealTensor_equal(x, y)));
  struct NS(RealTensor) *z = NS(RealTensor_new2)(2, 3);
  ASSERT_FALSE(NS(RealTensor_equal(x, z)));
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(y);
  NS(RealTensor_delete)(z);
}

static void RealTensorTest_v() {
  Long sizes[4] = {2, 3, 2, 3};
  Long indexes_0[4] = {0, 1, 1, 0};
  Long indexes_1[4] = {0, 1, 1, 1};
  struct NS(RealTensor) *x = NS(RealTensor_new)(4, sizes);
  ASSERT_REAL_EQ(NS(RealTensor_get)(x, indexes_0), 0);
  NS(RealTensor_set)(x, indexes_0, (Real)1);
  ASSERT_REAL_EQ(NS(RealTensor_get)(x, indexes_0), 1);
  struct NS(RealTensor) *y = NS(RealTensor_new)(4, sizes);
  NS(RealTensor_set)(y, indexes_0, (Real)1);
  ASSERT_REALTENSOR_EQ(x, y);
  ASSERT_REALTENSOR_AEQ(x, y);
  NS(RealTensor_set)(y, indexes_1, (Real)2);
  ASSERT_FALSE(NS(RealTensor_equal(x, y)));
  struct NS(RealTensor) *z = NS(RealTensor_new2)(2, 3);
  ASSERT_FALSE(NS(RealTensor_equal(x, z)));
  NS(RealTensor_delete)(x);
  NS(RealTensor_delete)(y);
  NS(RealTensor_delete)(z);
}

void NS(RealTensorTest)() {
  RealTensorTest_1();
  RealTensorTest_2();
  RealTensorTest_3();
  RealTensorTest_v();
}
