#pragma once

#include "Util_.h"

Long *NS(Tensor_GetStrides)(int dimension, const Long *sizes);

void NS(Tensor_GetStrides_)(int dimension, const Long *sizes, Long *out);
