#include "Variadic.h"

#include <jomfft/util/IntVector.h>

struct NS(Variadic) *NS(Variadic_newInt)(int value) {
  struct NS(Variadic) *out = NEW(struct NS(Variadic));
  out->type = VARIADIC_TYPE_INT;
  int *p = NEW(int);
  *p = value;
  out->value = p;
  return out;
}

struct NS(Variadic) *NS(Variadic_newLong)(Long value) {
  struct NS(Variadic) *out = NEW(struct NS(Variadic));
  out->type = VARIADIC_TYPE_LONG;
  Long *p = NEW(Long);
  *p = value;
  out->value = p;
  return out;
}

struct NS(Variadic) *NS(Variadic_newBool)(bool value) {
  struct NS(Variadic) *out = NEW(struct NS(Variadic));
  out->type = VARIADIC_TYPE_BOOL;
  bool *p = NEW(bool);
  *p = value;
  out->value = p;
  return out;
}

struct NS(Variadic) *NS(Variadic_newIntArray)(Long size, const int *data) {
  struct NS(Variadic) *out = NEW(struct NS(Variadic));
  out->type = VARIADIC_TYPE_INTARRAY;
  struct jomfftIntVector *p = jomfftIntVector_new();
  jomfftIntVector_resize(p, size);
  ARRAY_COPY(int, size, data, jomfftIntVector_data(p));
  out->value = p;
  return out;
}

void NS(Variadic_delete)(struct NS(Variadic) *p) {
  if (p) {
    if (p->value) {
      switch (p->type) {
      case VARIADIC_TYPE_INTARRAY:
        jomfftIntVector_delete(p->value);
        break;
      default:
        DELETE(p->value);
        break;
      }
      p->value = NULL;
    }
    DELETE(p);
  }
}

int NS(Variadic_getInt)(const struct NS(Variadic) *p) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_INT);
  return *((int *)p->value);
}

Long NS(Variadic_getLong)(const struct NS(Variadic) *p) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_LONG);
  return *((Long *)p->value);
}

bool NS(Variadic_getBool)(const struct NS(Variadic) *p) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_BOOL);
  return *((bool *)p->value);
}

const struct jomfftIntVector *NS(Variadic_getIntArray)(
    const struct NS(Variadic) *p) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_INTARRAY);
  return (const struct jomfftIntVector *)p->value;
}

void NS(Variadic_setInt)(struct NS(Variadic) *p, int value) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_INT);
  *((int *)p->value) = value;
}

void NS(Variadic_setLong)(struct NS(Variadic) *p, Long value) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_LONG);
  *((Long *)p->value) = value;
}

void NS(Variadic_setBool)(struct NS(Variadic) *p, bool value) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_BOOL);
  *((bool *)p->value) = value;
}

void NS(Variadic_setIntArray)(struct NS(Variadic) *p, Long size,
                              const int *data) {
  assert(p);
  assert(p->type == VARIADIC_TYPE_INTARRAY);
  struct jomfftIntVector *list = (struct jomfftIntVector *)p->value;
  jomfftIntVector_clear(list);
  jomfftIntVector_resize(list, size);
  ARRAY_COPY(int, size, data, jomfftIntVector_data(list));
}

static void printBool(FILE *stream, bool value) {
  fprintf(stream, BOOL_STR(value));
}

static void printIntArray(FILE *stream, const struct jomfftIntVector *p) {
  assert(p);
  fprintf(stream, "[");
  const int *begin = jomfftIntVector_cbegin(p);
  const int *end = jomfftIntVector_cend(p);
  for (const int *it = begin; it != end; ++it) {
    if (it != begin) {
      fprintf(stream, ",");
    }
    fprintf(stream, "%d", *it);
  }
  fprintf(stream, "]");
}

void NS(Variadic_print)(struct NS(Variadic) *p, FILE *stream) {
  assert(p);
  switch (p->type) {
  case VARIADIC_TYPE_INT:
    fprintf(stream, "%d", NS(Variadic_getInt)(p));
    break;
  case VARIADIC_TYPE_LONG:
    fprintf(stream, "%ld", (long)NS(Variadic_getLong)(p));
    break;
  case VARIADIC_TYPE_BOOL:
    printBool(stream, NS(Variadic_getBool)(p));
    break;
  case VARIADIC_TYPE_INTARRAY:
    printIntArray(stream, NS(Variadic_getIntArray)(p));
    break;
  default:
    break;
  }
}
