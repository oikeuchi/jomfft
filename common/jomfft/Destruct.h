#pragma once

#include "Common.h"

typedef void (*jomfftDestruct_fn)(void *);

void jomfftDestructPointer(void *p);

void jomfftDestructArray(void *p);
