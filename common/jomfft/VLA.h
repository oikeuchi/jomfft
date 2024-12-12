#pragma once

#include "Common.h"

#ifdef _MSC_VER

static inline void *jomfftAlignedAddress(void *p, size_t alignment) {
  size_t a = (size_t)(p);
  size_t s = a % alignment;
  if (s != 0) {
    s = alignment - s;
  }
  return (char *)p + s;
}

#define DECLARE_VLA_(type, name, sz, alignment)                                \
  void *name##_raw__ = _alloca(sizeof(type) * (sz) + (alignment));             \
  type *name = (type *)jomfftAlignedAddress(name##_raw__, (alignment))

#define DECLARE_VLA(type, name, sz) DECLARE_VLA_(type, name, sz, alignof(type))

#define DECLARE_ALIGNED_VLA(type, name, sz)                                    \
  DECLARE_VLA_(type, name, sz, ALIGNMENT)

#else
#define DECLARE_VLA(type, name, sz) type name[sz]
#define DECLARE_ALIGNED_VLA(type, name, sz) alignas(ALIGNMENT) type name[sz]
#endif
