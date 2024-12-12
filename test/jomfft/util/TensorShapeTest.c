#include "TensorShapeTest.h"

#include <jomfft/util/TensorShape.h>

void NS(TensorShapeTest)() {
  Long sizes_0[2] = {3, 4};
  Long sizes_1[3] = {3, 2, 1};
  Long indexes[2] = {2, 1};
  Long strides[2] = {0};
  struct NS(TensorShape) *x = NS(TensorShape_new)(2, sizes_0);
  ASSERT_INT_EQ(x->dimension, 2);
  ASSERT_LONG_EQ(x->sizes[0], 3);
  ASSERT_LONG_EQ(NS(TensorShape_dataSize)(x), 12);
  ASSERT_LONG_EQ(NS(TensorShape_dataIndex)(x, indexes), 9);
  NS(TensorShape_strides)(x, strides);
  ASSERT_LONG_EQ(strides[0], 4);
  struct NS(TensorShape) *y = NS(TensorShape_new)(2, sizes_0);
  ASSERT_TRUE(NS(TensorShape_equal)(x, y));
  y->sizes[1] = 2;
  ASSERT_FALSE(NS(TensorShape_equal)(x, y));
  struct NS(TensorShape) *z = NS(TensorShape_new)(3, sizes_1);
  ASSERT_FALSE(NS(TensorShape_equal)(x, z));
  NS(TensorShape_delete)(x);
  NS(TensorShape_delete)(y);
  NS(TensorShape_delete)(z);
}
