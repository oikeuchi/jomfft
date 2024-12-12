#include <jomfft/math/IntegerMath.h>

#define COPY_STRIDE_THRESHOLD (STRIDE_THRESHOLD / sizeof(VALUE_TYPE))
#define BLOCK_SIZE (int)(MEMORY_BUS_SIZE / sizeof(VALUE_TYPE))
#define COPY_TRANSPOSE_BLOCK_MIN_SIZE (4 * BLOCK_SIZE)

void VALUE_TYPE_(Copy_1)(Long size, const VALUE_TYPE *x, VALUE_TYPE *y) {
  ARRAY_COPY(VALUE_TYPE, size, x, y);
}

void VALUE_TYPE_(Copy)(Long size, const VALUE_TYPE *x, Long x_stride,
                       VALUE_TYPE *y, Long y_stride) {
  if (x_stride == 1 && y_stride == 1) {
    VALUE_TYPE_(Copy_1)(size, x, y);
  } else if (x_stride == 1) {
    const VALUE_TYPE *xend = x + size;
    for (; x != xend; ++x, y += y_stride) {
      *y = *x;
    }
  } else if (y_stride == 1) {
    VALUE_TYPE *yend = y + size;
    for (; y != yend; x += x_stride, ++y) {
      *y = *x;
    }
  } else {
    const VALUE_TYPE *xend = x + x_stride * size;
    for (; x != xend; x += x_stride, y += y_stride) {
      *y = *x;
    }
  }
}

void VALUE_TYPE_(Copy2D_1)(Long size_0, Long size_1, const VALUE_TYPE *x,
                           Long x_stride, VALUE_TYPE *y, Long y_stride) {
  const size_t column_bytes = sizeof(VALUE_TYPE) * size_1;
  for (Long i = 0; i < size_0; ++i) {
    memcpy(y + y_stride * i, x + x_stride * i, column_bytes);
  }
}

static void VALUE_TYPE_(CopyTranspose_F_)(Long size_0, Long size_1,
                                          const VALUE_TYPE *x, Long x_stride,
                                          VALUE_TYPE *y, Long y_stride) {
  for (Long i = 0; i < size_0; ++i) {
    for (Long j = 0; j < size_1; ++j) {
      y[y_stride * j + i] = x[x_stride * i + j];
    }
  }
}

static void VALUE_TYPE_(CopyTranspose_F_Block_)(const VALUE_TYPE *x,
                                                Long x_stride, VALUE_TYPE *y,
                                                Long y_stride) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      y[y_stride * j + i] = x[x_stride * i + j];
    }
  }
}

static void VALUE_TYPE_(CopyTranspose_F_Block)(Long size_0, Long size_1,
                                               const VALUE_TYPE *x,
                                               Long x_stride, VALUE_TYPE *y,
                                               Long y_stride) {
  const Long size_0_0 = I_FLOOR_MULTIPLE(size_0, BLOCK_SIZE);
  const Long size_1_0 = I_FLOOR_MULTIPLE(size_1, BLOCK_SIZE);
  Long i = 0;
  for (; i < size_0_0; i += BLOCK_SIZE) {
    Long j = 0;
    for (; j < size_1_0; j += BLOCK_SIZE) {
      VALUE_TYPE_(CopyTranspose_F_Block_)(&x[x_stride * i + j], x_stride,
                                          &y[y_stride * j + i], y_stride);
    }
    if (j < size_1) {
      VALUE_TYPE_(CopyTranspose_F_)(BLOCK_SIZE, size_1 - size_1_0,
                                    &x[x_stride * i + j], x_stride,
                                    &y[y_stride * j + i], y_stride);
    }
  }
  if (i < size_0) {
    VALUE_TYPE_(CopyTranspose_F_)(size_0 - size_0_0, size_1, &x[x_stride * i],
                                  x_stride, &y[i], y_stride);
  }
}

void VALUE_TYPE_(CopyTranspose_F)(Long size_0, Long size_1, const VALUE_TYPE *x,
                                  Long x_stride, VALUE_TYPE *y, Long y_stride) {
  if (size_1 >= COPY_TRANSPOSE_BLOCK_MIN_SIZE &&
      (x_stride % COPY_STRIDE_THRESHOLD == 0 ||
       y_stride % COPY_STRIDE_THRESHOLD == 0)) {
    VALUE_TYPE_(CopyTranspose_F_Block)(size_0, size_1, x, x_stride, y,
                                       y_stride);
  } else {
    VALUE_TYPE_(CopyTranspose_F_)(size_0, size_1, x, x_stride, y, y_stride);
  }
}

static void VALUE_TYPE_(CopyTranspose_B_)(Long size_0, Long size_1,
                                          const VALUE_TYPE *x, Long x_stride,
                                          VALUE_TYPE *y, Long y_stride) {
  for (Long i = 0; i < size_0; ++i) {
    for (Long j = 0; j < size_1; ++j) {
      y[y_stride * i + j] = x[x_stride * j + i];
    }
  }
}

static void VALUE_TYPE_(CopyTranspose_B_Block_)(const VALUE_TYPE *x,
                                                Long x_stride, VALUE_TYPE *y,
                                                Long y_stride) {
  for (int i = 0; i < BLOCK_SIZE; ++i) {
    for (int j = 0; j < BLOCK_SIZE; ++j) {
      y[y_stride * i + j] = x[x_stride * j + i];
    }
  }
}

static void VALUE_TYPE_(CopyTranspose_B_Block)(Long size_0, Long size_1,
                                               const VALUE_TYPE *x,
                                               Long x_stride, VALUE_TYPE *y,
                                               Long y_stride) {
  const Long size_0_0 = I_FLOOR_MULTIPLE(size_0, BLOCK_SIZE);
  const Long size_1_0 = I_FLOOR_MULTIPLE(size_1, BLOCK_SIZE);
  Long i = 0;
  for (; i < size_0_0; i += BLOCK_SIZE) {
    Long j = 0;
    for (; j < size_1_0; j += BLOCK_SIZE) {
      VALUE_TYPE_(CopyTranspose_B_Block_)(&x[x_stride * j + i], x_stride,
                                          &y[y_stride * i + j], y_stride);
    }
    if (j < size_1) {
      VALUE_TYPE_(CopyTranspose_B_)(BLOCK_SIZE, size_1 - size_1_0,
                                    &x[x_stride * j + i], x_stride,
                                    &y[y_stride * i + j], y_stride);
    }
  }
  if (i < size_0) {
    VALUE_TYPE_(CopyTranspose_B_)(size_0 - size_0_0, size_1, &x[i], x_stride,
                                  &y[y_stride * i], y_stride);
  }
}

void VALUE_TYPE_(CopyTranspose_B)(Long size_0, Long size_1, const VALUE_TYPE *x,
                                  Long x_stride, VALUE_TYPE *y, Long y_stride) {
  if (size_1 >= COPY_TRANSPOSE_BLOCK_MIN_SIZE &&
      (x_stride % COPY_STRIDE_THRESHOLD == 0 ||
       y_stride % COPY_STRIDE_THRESHOLD == 0)) {
    VALUE_TYPE_(CopyTranspose_B_Block)(size_0, size_1, x, x_stride, y,
                                       y_stride);
  } else {
    VALUE_TYPE_(CopyTranspose_B_)(size_0, size_1, x, x_stride, y, y_stride);
  }
}

static void VALUE_TYPE_(Copy2D_)(Long size_0, Long size_1, const VALUE_TYPE *x,
                                 Long x_stride_0, Long x_stride_1,
                                 VALUE_TYPE *y, Long y_stride_0,
                                 Long y_stride_1) {
  for (Long i = 0; i < size_0; ++i) {
    for (Long j = 0; j < size_1; ++j) {
      y[y_stride_0 * i + y_stride_1 * j] = x[x_stride_0 * i + x_stride_1 * j];
    }
  }
}

void VALUE_TYPE_(Copy2D)(Long size_0, Long size_1, const VALUE_TYPE *x,
                         Long x_stride_0, Long x_stride_1, VALUE_TYPE *y,
                         Long y_stride_0, Long y_stride_1) {
  if (x_stride_1 == 1 && y_stride_1 == 1) {
    VALUE_TYPE_(Copy2D_1)
    (size_0, size_1, x, x_stride_0, y, y_stride_0);
  } else if (x_stride_1 == 1 && y_stride_0 == 1) {
    VALUE_TYPE_(CopyTranspose_F)
    (size_0, size_1, x, x_stride_0, y, y_stride_1);
  } else if (x_stride_0 == 1 && y_stride_1 == 1) {
    VALUE_TYPE_(CopyTranspose_B)
    (size_1, size_0, x, x_stride_1, y, y_stride_0);
  } else if (y_stride_0 == 1) {
    VALUE_TYPE_(Copy2D_)(size_1, size_0, x, x_stride_1, x_stride_0, y,
                         y_stride_1, y_stride_0);
  } else {
    VALUE_TYPE_(Copy2D_)(size_0, size_1, x, x_stride_0, x_stride_1, y,
                         y_stride_0, y_stride_1);
  }
}
