#include "Math.h"

#ifdef __GNUC__
#if USE_SINGLE
#define R_SINCOS sincosf
#else
#define R_SINCOS sincos
#endif
#else
static inline void R_SINCOS(Real t, Real *s, Real *c) {
  const Real t__ = (t);
  *(s) = R_SIN(t__);
  *(c) = R_COS(t__);
}
#endif

Complex NS(Complex_fromArgument)(Real theta) {
  Real s, c;
  R_SINCOS(theta, &s, &c);
  return Complex_(c, s);
}
