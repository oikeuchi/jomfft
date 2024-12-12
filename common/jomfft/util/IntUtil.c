#include "IntUtil.h"

static int jomfftInt_compare(const void *lhs, const void *rhs) {
  const int a = *((int *)lhs);
  const int b = *((int *)rhs);
  return a - b;
}

void jomfftInt_Sort(Long size, int *data) {
  qsort(data, (size_t)size, sizeof(int), jomfftInt_compare);
}

Long jomfftInt_BinarySearch(Long size, const int *data, int key) {
  void *p = bsearch(&key, data, (size_t)size, sizeof(int), jomfftInt_compare);
  if (p) {
    return (Long)(((int *)p) - data);
  } else {
    return -1;
  }
}

int jomfftInt_Product(int size, const int *data) {
  int out = 1;
  for (int i = 0; i < size; ++i) {
    out *= data[i];
  }
  return out;
}

Long jomfftInt_ProductLong(Long size, const int *data) {
  Long out = 1;
  for (Long i = 0; i < size; ++i) {
    out *= (Long)data[i];
  }
  return out;
}
