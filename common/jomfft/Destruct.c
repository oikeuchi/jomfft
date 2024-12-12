#include "Destruct.h"

void jomfftDestructPointer(void *p) {
  if (p) {
    DELETE(p);
  }
}

void jomfftDestructArray(void *p) {
  if (p) {
    ARRAY_DELETE(p);
  }
}
