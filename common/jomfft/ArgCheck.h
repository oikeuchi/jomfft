#pragma once

#include "Common.h"

#define ARGCHECK(expr, msg)                                                    \
  do {                                                                         \
    if (!(expr)) {                                                             \
      jomfftPrintError(msg);                                                   \
      return jomfftInvalidArgument;                                            \
    }                                                                          \
  } while (0)

#define ARGCHECK_POSITIVE(x) ARGCHECK(x > 0, #x " must be positive.")

#define ARGCHECK_NOTZERO(x) ARGCHECK(x != 0, #x " must not be zero.")

#define ARGCHECK_NOTNULL(x) ARGCHECK(x != NULL, #x " must not be null.")

#define ARGCHECK_HANDLE(x)                                                     \
  ARGCHECK((x != NULL && x->data != NULL), #x " is invalid.")

#define ARGCHECK_PLACEMENT(x)                                                  \
  ARGCHECK(jomfftIsPlacementValid(x), #x " must be a valid enum value.")

#define ARGCHECK_MULTIPLEOF2(x)                                                \
  ARGCHECK(x % 2 == 0, #x " must be a multiple of 2.")

#define ARGCHECK_GE(x, a) ARGCHECK(x >= a, #x " must be >= " #a ".")

#define ARGCHECK_ARRAY_GE(sz, x, a)                                            \
  do {                                                                         \
    long i__ = 0;                                                              \
    for (; i__ < sz; ++i__) {                                                  \
      if (!(x[i__] >= a)) {                                                    \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (i__ < sz) {                                                            \
      jomfftPrintError(#x "[%ld] must be >= " #a ".", i__);                    \
      return jomfftInvalidArgument;                                            \
    }                                                                          \
  } while (0)

#define ARGCHECK_ARRAY_GE_(sz, x, y)                                           \
  do {                                                                         \
    long i__ = 0;                                                              \
    for (; i__ < sz; ++i__) {                                                  \
      if (!(x[i__] >= y[i__])) {                                               \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (i__ < sz) {                                                            \
      jomfftPrintError(#x "[%ld] must be >= " #y "[%ld].", i__, i__);          \
      return jomfftInvalidArgument;                                            \
    }                                                                          \
  } while (0)

static inline void jomfftPrintError(const char *format, ...) {
#if PRINT_ERROR
  fprintf(stderr, "JomfftError: ");
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
#else
  (void)format;
#endif
}

static inline int jomfftIsPlacementValid(jomfftPlacement x) {
  return 0 <= x && x <= 1;
}
