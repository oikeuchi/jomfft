#include "PrimeFactors.h"

#define SMALL_TABLE_SIZE 32

static const int g_primes[NUM_OF_PRIMES] = {2, 3, 5, 7};

static const uint8_t g_exponents_table[SMALL_TABLE_SIZE][NUM_OF_PRIMES] = {
    {0, 0, 0, 0}, //  1
    {1, 0, 0, 0}, //  2
    {0, 1, 0, 0}, //  3
    {2, 0, 0, 0}, //  4
    {0, 0, 1, 0}, //  5
    {1, 1, 0, 0}, //  6
    {0, 0, 0, 1}, //  7
    {3, 0, 0, 0}, //  8
    {0, 2, 0, 0}, //  9
    {1, 0, 1, 0}, // 10
    {0, 0, 0, 0}, // 11
    {2, 1, 0, 0}, // 12
    {0, 0, 0, 0}, // 13
    {1, 0, 0, 1}, // 14
    {0, 1, 1, 0}, // 15
    {4, 0, 0, 0}, // 16
    {0, 0, 0, 0}, // 17
    {1, 2, 0, 0}, // 18
    {0, 0, 0, 0}, // 19
    {2, 0, 1, 0}, // 20
    {0, 1, 0, 1}, // 21
    {1, 0, 0, 0}, // 22
    {0, 0, 0, 0}, // 23
    {3, 1, 0, 0}, // 24
    {0, 0, 2, 0}, // 25
    {1, 0, 0, 0}, // 26
    {0, 3, 0, 0}, // 27
    {2, 0, 0, 1}, // 28
    {0, 0, 0, 0}, // 29
    {1, 1, 1, 0}, // 30
    {0, 0, 0, 0}, // 31
    {5, 0, 0, 0}  // 32
};

static const uint8_t g_residual_table[SMALL_TABLE_SIZE] = {
    1,  //  1
    1,  //  2
    1,  //  3
    1,  //  4
    1,  //  5
    1,  //  6
    1,  //  7
    1,  //  8
    1,  //  9
    1,  // 10
    11, // 11
    1,  // 12
    13, // 13
    1,  // 14
    1,  // 15
    1,  // 16
    17, // 17
    1,  // 18
    19, // 19
    1,  // 20
    1,  // 21
    11, // 22
    23, // 23
    1,  // 24
    1,  // 25
    13, // 26
    1,  // 27
    1,  // 28
    29, // 29
    1,  // 30
    31, // 31
    1   // 32
};

static int factorExponent(Long *n, Long d) {
  assert(*n > 0 && d > 0);
  Long out = 0;
  Long q = *n / d;
  Long r = *n % d;
  while (r == 0) {
    ++out;
    *n = q;
    q = *n / d;
    r = *n % d;
  }
  return out;
}

const int *jomfftPrimeFactors_Primes() { return g_primes; }

int jomfftPrimeFactors_Prime(int i) { return g_primes[i]; }

bool jomfftPrimeFactors_empty(const struct jomfftPrimeFactors *p) {
  return jomfftPrimeFactors_count(p) == 0;
}

int jomfftPrimeFactors_count(const struct jomfftPrimeFactors *p) {
  int sum = 0;
  for (int i = 0; i < NUM_OF_PRIMES; ++i) {
    assert(p->exponents[i] >= 0);
    sum += p->exponents[i];
  }
  return sum;
}

int jomfftPrimeFactors_product(const struct jomfftPrimeFactors *p) {
  assert(g_primes[0] == 2);
  int out = 1 << p->exponents[0];
  for (int i = 1; i < NUM_OF_PRIMES; ++i) {
    const int r = g_primes[i];
    for (int j = 0; j < p->exponents[i]; ++j) {
      out *= r;
    }
  }
  return out;
}

Long jomfftPrimeFactors_productLong(const struct jomfftPrimeFactors *p) {
  assert(g_primes[0] == 2);
  Long out = 1 << p->exponents[0];
  for (int i = 1; i < NUM_OF_PRIMES; ++i) {
    const Long r = g_primes[i];
    for (int j = 0; j < p->exponents[i]; ++j) {
      out *= r;
    }
  }
  return out;
}

struct jomfftPrimeFactors jomfftPrimeFactors_Factor(Long *n) {
  assert(n);
  assert(*n >= 1);
  if (*n <= SMALL_TABLE_SIZE) {
    Long n_ = *n;
    struct jomfftPrimeFactors o;
    for (int i = 0; i < NUM_OF_PRIMES; ++i) {
      o.exponents[i] = (int)g_exponents_table[n_ - 1][i];
    }
    *n = (Long)g_residual_table[n_ - 1];
    return o;
  } else {
    struct jomfftPrimeFactors o;
    int i = 0;
    for (; *n > 1 && i < NUM_OF_PRIMES; ++i) {
      o.exponents[i] = factorExponent(n, g_primes[i]);
    }
    for (; i < NUM_OF_PRIMES; ++i) {
      o.exponents[i] = 0;
    }
    return o;
  }
}

struct jomfftPrimeFactors jomfftPrimeFactors_Factor_(Long n) {
  struct jomfftPrimeFactors out = jomfftPrimeFactors_Factor(&n);
  assert(n == 1);
  return out;
}

bool jomfftPrimeFactors_CanFactor(Long n) {
  jomfftPrimeFactors_Factor(&n);
  return n == 1;
}

bool jomfftPrimeFactors_equal(struct jomfftPrimeFactors *p,
                              const struct jomfftPrimeFactors *rhs) {
  bool out = true;
  for (int i = 0; out && i < NUM_OF_PRIMES; ++i) {
    out = out && p->exponents[i] == rhs->exponents[i];
  }
  return out;
}

bool jomfftPrimeFactors_hasFactor(const struct jomfftPrimeFactors *p,
                                  Long factor) {
  assert(p);
  assert(factor >= 1);
  struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor(&factor);
  if (factor > 1) {
    return false;
  } else {
    return jomfftPrimeFactors_hasPrimeFactors(p, &f);
  }
}

bool jomfftPrimeFactors_hasPrimeFactors(
    const struct jomfftPrimeFactors *p,
    const struct jomfftPrimeFactors *factor) {
  assert(p);
  assert(factor);
  bool result = true;
  for (int i = 0; result && i < NUM_OF_PRIMES; ++i) {
    result = result && p->exponents[i] >= factor->exponents[i];
  }
  return result;
}

void jomfftPrimeFactors_add_(struct jomfftPrimeFactors *p,
                             const struct jomfftPrimeFactors *rhs) {
  assert(p);
  assert(rhs);
  for (int i = 0; i < NUM_OF_PRIMES; ++i) {
    p->exponents[i] += rhs->exponents[i];
  }
}

void jomfftPrimeFactors_subtract_(struct jomfftPrimeFactors *p,
                                  const struct jomfftPrimeFactors *rhs) {
  assert(p);
  assert(rhs);
  for (int i = 0; i < NUM_OF_PRIMES; ++i) {
    p->exponents[i] -= rhs->exponents[i];
  }
}

void jomfftPrimeFactors_sqrt(const struct jomfftPrimeFactors *p,
                             struct jomfftPrimeFactors *q,
                             struct jomfftPrimeFactors *r) {
  assert(p);
  assert(q);
  assert(r);
  for (int i = 0; i < NUM_OF_PRIMES; ++i) {
    q->exponents[i] = p->exponents[i] / 2;
    r->exponents[i] = p->exponents[i] % 2;
  }
}

void jomfftPrimeFactors_sqrtLong(const struct jomfftPrimeFactors *p, Long *q,
                                 Long *r) {
  assert(q);
  assert(r);
  struct jomfftPrimeFactors qf;
  struct jomfftPrimeFactors rf;
  jomfftPrimeFactors_sqrt(p, &qf, &rf);
  *q = jomfftPrimeFactors_productLong(&qf);
  *r = jomfftPrimeFactors_productLong(&rf);
}
