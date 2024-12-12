#pragma once

#include "Test_.h"

struct NS(CaptureKey) {
  const char *name;
  const char *file;
  int lineno;
};

bool NS(CaptureKey_equal)(struct NS(CaptureKey) a, struct NS(CaptureKey) b);

void NS(Capture_print)(struct NS(CaptureKey) key, struct NS(Variadic) *value);
