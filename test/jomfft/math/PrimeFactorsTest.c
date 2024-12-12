#include "PrimeFactorsTest.h"

#include <jomfft/math/PrimeFactors.h>

static void PrimeFactors_ConsistencyTest_(int n) {
  CAPTURE_INT(n);
  Long r = n;
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&r);
  int n0 = (int)(jomfftPrimeFactors_productLong(&f) * r);
  ASSERT_INT_EQ(n0, n);
  ASSERT_BOOL_EQ(jomfftPrimeFactors_CanFactor(n), r == 1);
  if (r == 1) {
    struct jomfftPrimeFactors f0 = jomfftPrimeFactors_Factor_(n);
    ASSERT_TRUE(jomfftPrimeFactors_equal(&f, &f0));
  }
}

static void PrimeFactors_ConsistencyTest() {
  for (int i = 1; i <= 64; ++i) {
    PrimeFactors_ConsistencyTest_(i);
  }
}

void NS(PrimeFactorsTest)() {
  struct jomfftPrimeFactors x;
  struct jomfftPrimeFactors y;
  struct jomfftPrimeFactors z;
  memset(x.exponents, 0, sizeof(x.exponents));
  ASSERT_TRUE(jomfftPrimeFactors_empty(&x));
  x.exponents[0] = 2;
  x.exponents[1] = 1;
  x.exponents[2] = 1;
  x.exponents[3] = 1;
  ASSERT_INT_EQ(jomfftPrimeFactors_count(&x), 5);
  ASSERT_TRUE(jomfftPrimeFactors_equal(&x, &x));
  ASSERT_TRUE(jomfftPrimeFactors_hasFactor(&x, 4));
  ASSERT_FALSE(jomfftPrimeFactors_hasFactor(&x, 25));
  ASSERT_INT_EQ(jomfftPrimeFactors_product(&x), 420);
  ASSERT_LONG_EQ(jomfftPrimeFactors_productLong(&x), 420);
  y.exponents[0] = 2;
  y.exponents[1] = 2;
  y.exponents[2] = 1;
  y.exponents[3] = 1;
  ASSERT_FALSE(jomfftPrimeFactors_equal(&y, &x));
  ASSERT_TRUE(jomfftPrimeFactors_hasPrimeFactors(&y, &x));
  ASSERT_FALSE(jomfftPrimeFactors_hasPrimeFactors(&x, &y));
  z = y;
  jomfftPrimeFactors_subtract_(&z, &x);
  ASSERT_LONG_EQ(jomfftPrimeFactors_productLong(&z), 3);
  jomfftPrimeFactors_add_(&z, &x);
  ASSERT_TRUE(jomfftPrimeFactors_equal(&z, &y));
  x.exponents[0] = 4;
  x.exponents[1] = 2;
  x.exponents[2] = 0;
  x.exponents[3] = 1;
  struct jomfftPrimeFactors q;
  struct jomfftPrimeFactors r;
  jomfftPrimeFactors_sqrt(&x, &q, &r);
  ASSERT_LONG_EQ(jomfftPrimeFactors_productLong(&q), 12);
  ASSERT_LONG_EQ(jomfftPrimeFactors_productLong(&r), 7);

  PrimeFactors_ConsistencyTest();
}
