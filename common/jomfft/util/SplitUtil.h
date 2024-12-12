#pragma once

#include "../Common.h"
#include "../math/PrimeFactors.h"
#include "IntVector.h"

int jomfftGetDftSize(struct jomfftPrimeFactors *primes,
                     const struct jomfftIntVector *keys);

int jomfftGetDftSize_(struct jomfftPrimeFactors *primes,
                      const struct jomfftIntVector *keys, int max_size);

int jomfftGetDftSizeWithFactor(struct jomfftPrimeFactors *primes,
                               const struct jomfftIntVector *keys, int factor,
                               int max_size);

int jomfftGetDftSizeNearestToMultiple(struct jomfftPrimeFactors *primes,
                                      const struct jomfftIntVector *keys,
                                      int factor, int max_size);

void jomfftGetDftSizePair(struct jomfftPrimeFactors *primes,
                          const struct jomfftIntVector *first_keys,
                          const struct jomfftIntVector *second_keys,
                          int max_size, int *first, int *second);
