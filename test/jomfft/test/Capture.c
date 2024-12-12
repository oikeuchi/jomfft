#include "Capture.h"

bool NS(CaptureKey_equal)(struct NS(CaptureKey) a, struct NS(CaptureKey) b) {
  return !strcmp(a.name, b.name) && !strcmp(a.file, b.file) &&
         a.lineno == b.lineno;
}

void NS(Capture_print)(struct NS(CaptureKey) key, struct NS(Variadic) *value) {
  printf("  %s: ", key.name);
  NS(Variadic_print)(value, stdout);
  printf("\n");
}
