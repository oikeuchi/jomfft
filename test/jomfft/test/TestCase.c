#include "TestCase.h"

struct NS(TestCase) *NS(TestCase_new)(const char *name, NS(Test_fn) func) {
  struct NS(TestCase) *out = NEW(struct NS(TestCase));
  *out = (struct NS(TestCase)){.name = name, .func = func};
  return out;
}

void NS(TestCase_delete)(struct NS(TestCase) *p) {
  if (p) {
    DELETE(p);
  }
}
