#include "LongUtil.h"

bool jomfftLong_equal(Long a, Long b) { return a == b; }

Long jomfftLong_Product(Long size, const Long *data) {
  Long out = 1;
  for (Long i = 0; i < size; ++i) {
    out *= data[i];
  }
  return out;
}

Long *jomfftLong_FromIntArray(Long size, const int *data) {
  Long *out = ARRAY_NEW(Long, size);
  for (Long i = 0; i < size; ++i) {
    out[i] = (Long)data[i];
  }
  return out;
}
