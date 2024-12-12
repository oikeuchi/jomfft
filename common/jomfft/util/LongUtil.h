#pragma once

#include "../Common.h"

bool jomfftLong_equal(Long a, Long b);

Long jomfftLong_Product(Long size, const Long *data);

Long *jomfftLong_FromIntArray(Long size, const int *data);
