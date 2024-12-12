#include "Random.h"

void NS(Random_Init)(unsigned int seed) { srand(seed); }

Real NS(Random_Real)() { return (Real)rand() / ((Real)(RAND_MAX) + 1); }

Complex NS(Random_Complex)() {
  return Complex_(NS(Random_Real)(), NS(Random_Real)());
}
