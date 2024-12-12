#pragma once

#include "../Common.h"

#define I_DIVIDE_FLOOR(n, d) ((n) / (d))

#define I_DIVIDE_CEIL(n, d) (((n) + (d)-1) / (d))

#define I_FLOOR_MULTIPLE(n, d) ((d)*I_DIVIDE_FLOOR((n), (d)))

#define I_CEIL_MULTIPLE(n, d) ((d)*I_DIVIDE_CEIL((n), (d)))
