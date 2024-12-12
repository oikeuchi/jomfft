#include "SplitUtil.h"
#include "../VLA.h"

int jomfftGetDftSize(struct jomfftPrimeFactors *primes,
                     const struct jomfftIntVector *keys) {
  return jomfftGetDftSize_(primes, keys, INT_MAX);
}

int jomfftGetDftSize_(struct jomfftPrimeFactors *primes,
                      const struct jomfftIntVector *keys, int max_size) {
  assert(primes);
  assert(keys);
  const int *it = jomfftIntVector_cend(keys) - 1;
  const int *end = jomfftIntVector_cbegin(keys) - 1;
  for (; it != end; --it) {
    if (*it <= max_size) {
      struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(*it);
      if (jomfftPrimeFactors_hasPrimeFactors(primes, &f)) {
        jomfftPrimeFactors_subtract_(primes, &f);
        return *it;
      }
    }
  }
  return 1;
}

int jomfftGetDftSizeWithFactor(struct jomfftPrimeFactors *primes,
                               const struct jomfftIntVector *keys, int factor,
                               int max_size) {
  assert(primes);
  assert(keys);
  assert(jomfftPrimeFactors_hasFactor(primes, factor));
  assert(jomfftIntVector_binarySearch(keys, factor) >= 0);
  const struct jomfftPrimeFactors factor_ = jomfftPrimeFactors_Factor_(factor);
  const int *it = jomfftIntVector_cend(keys) - 1;
  const int *end = jomfftIntVector_cbegin(keys) - 1;
  for (; it != end; --it) {
    if (*it <= max_size) {
      struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(*it);
      if (jomfftPrimeFactors_hasPrimeFactors(primes, &f) &&
          jomfftPrimeFactors_hasPrimeFactors(&f, &factor_)) {
        jomfftPrimeFactors_subtract_(primes, &f);
        return *it;
      }
    }
  }
  return 1;
}

int jomfftGetDftSizeNearestToMultiple(struct jomfftPrimeFactors *primes,
                                      const struct jomfftIntVector *keys,
                                      int factor, int max_size) {
  assert(primes);
  assert(keys);
  assert(factor >= 2);
  DECLARE_VLA(int, candidates, factor);
  for (int i = 0; i < factor; ++i) {
    candidates[i] = 0;
  }
  const int *it = jomfftIntVector_cend(keys) - 1;
  const int *end = jomfftIntVector_cbegin(keys) - 1;
  for (; it != end; --it) {
    if (*it <= max_size) {
      const int q = *it;
      struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(q);
      if (jomfftPrimeFactors_hasPrimeFactors(primes, &f) &&
          candidates[q % factor] == 0) {
        candidates[q % factor] = q;
      }
    }
  }
  for (int i = factor; i > 0; --i) {
    const int q = i == factor ? candidates[0] : candidates[i];
    if (q > 1) {
      struct jomfftPrimeFactors f = jomfftPrimeFactors_Factor_(q);
      jomfftPrimeFactors_subtract_(primes, &f);
      return q;
    }
  }
  return 1;
}

void jomfftGetDftSizePair(struct jomfftPrimeFactors *primes,
                          const struct jomfftIntVector *first_keys,
                          const struct jomfftIntVector *second_keys,
                          int max_size, int *first, int *second) {
  assert(primes);
  assert(first_keys);
  assert(second_keys);
  assert(first);
  assert(second);
  const int *primes_ = jomfftPrimeFactors_Primes();
  int *exponents = primes->exponents;
  int first_ = 1;
  int second_ = 1;
  for (int i = 0; i < NUM_OF_PRIMES; ++i) {
    while (exponents[i] > 0) {
      if (first_ < second_) {
        const int first_candidate = first_ * primes_[i];
        if ((jomfftIntVector_binarySearch(first_keys, first_candidate) >= 0) &&
            (first_ == 1 || first_candidate * second_ <= max_size)) {
          exponents[i] -= 1;
          first_ = first_candidate;
        } else {
          const int second_candidate = second_ * primes_[i];
          if ((jomfftIntVector_binarySearch(second_keys, second_candidate) >=
               0) &&
              (second_ == 1 || first_ * second_candidate <= max_size)) {
            exponents[i] -= 1;
            second_ = second_candidate;
          } else {
            break;
          }
        }
      } else {
        const int second_candidate = second_ * primes_[i];
        if ((jomfftIntVector_binarySearch(second_keys, second_candidate) >=
             0) &&
            (second_ == 1 || first_ * second_candidate <= max_size)) {
          exponents[i] -= 1;
          second_ = second_candidate;
        } else {
          const int first_candidate = first_ * primes_[i];
          if ((jomfftIntVector_binarySearch(first_keys, first_candidate) >=
               0) &&
              (first_ == 1 || first_candidate * second_ <= max_size)) {
            exponents[i] -= 1;
            first_ = first_candidate;
          } else {
            break;
          }
        }
      }
    }
  }
  *first = first_;
  *second = second_;
}
