#pragma once

#include "../Common.h"

#define NUM_OF_PRIMES 4

struct jomfftPrimeFactors {
  int exponents[NUM_OF_PRIMES];
};

const int *jomfftPrimeFactors_Primes();

int jomfftPrimeFactors_Prime(int i);

struct jomfftPrimeFactors jomfftPrimeFactors_Factor(Long *n);

struct jomfftPrimeFactors jomfftPrimeFactors_Factor_(Long n);

bool jomfftPrimeFactors_CanFactor(Long n);

bool jomfftPrimeFactors_empty(const struct jomfftPrimeFactors *p);

int jomfftPrimeFactors_count(const struct jomfftPrimeFactors *p);

int jomfftPrimeFactors_product(const struct jomfftPrimeFactors *p);

Long jomfftPrimeFactors_productLong(const struct jomfftPrimeFactors *p);

bool jomfftPrimeFactors_equal(struct jomfftPrimeFactors *p,
                              const struct jomfftPrimeFactors *rhs);

bool jomfftPrimeFactors_hasFactor(const struct jomfftPrimeFactors *p,
                                  Long factor);

bool jomfftPrimeFactors_hasPrimeFactors(
    const struct jomfftPrimeFactors *p,
    const struct jomfftPrimeFactors *factor);

void jomfftPrimeFactors_add_(struct jomfftPrimeFactors *p,
                             const struct jomfftPrimeFactors *rhs);

void jomfftPrimeFactors_subtract_(struct jomfftPrimeFactors *p,
                                  const struct jomfftPrimeFactors *rhs);

void jomfftPrimeFactors_sqrt(const struct jomfftPrimeFactors *p,
                             struct jomfftPrimeFactors *q,
                             struct jomfftPrimeFactors *r);

void jomfftPrimeFactors_sqrtLong(const struct jomfftPrimeFactors *p, Long *q,
                                 Long *r);
