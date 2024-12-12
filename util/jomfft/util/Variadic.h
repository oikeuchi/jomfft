#pragma once

#include "Util_.h"

struct jomfftIntVector;

typedef enum {
  VARIADIC_TYPE_INT = 0,
  VARIADIC_TYPE_LONG = 1,
  VARIADIC_TYPE_BOOL = 2,
  VARIADIC_TYPE_INTARRAY = 16
} NS(VariadicType_t);

struct NS(Variadic) {
  NS(VariadicType_t) type;
  void *value;
};

struct NS(Variadic) *NS(Variadic_newInt)(int value);
struct NS(Variadic) *NS(Variadic_newLong)(Long value);
struct NS(Variadic) *NS(Variadic_newBool)(bool value);
struct NS(Variadic) *NS(Variadic_newIntArray)(Long size, const int *data);

void NS(Variadic_delete)(struct NS(Variadic) *p);

int NS(Variadic_getInt)(const struct NS(Variadic) *p);
Long NS(Variadic_getLong)(const struct NS(Variadic) *p);
bool NS(Variadic_getBool)(const struct NS(Variadic) *p);
const struct jomfftIntVector *NS(Variadic_getIntArray)(
    const struct NS(Variadic) *p);

void NS(Variadic_setInt)(struct NS(Variadic) *p, int value);
void NS(Variadic_setLong)(struct NS(Variadic) *p, Long value);
void NS(Variadic_setBool)(struct NS(Variadic) *p, bool value);
void NS(Variadic_setIntArray)(struct NS(Variadic) *p, Long size,
                              const int *data);

void NS(Variadic_print)(struct NS(Variadic) *p, FILE *stream);
