void VALUE_TYPE_(Copy)(Long size, const VALUE_TYPE *x, Long x_stride,
                       VALUE_TYPE *y, Long y_stride);

void VALUE_TYPE_(Copy_1)(Long size, const VALUE_TYPE *x, VALUE_TYPE *y);

void VALUE_TYPE_(Copy2D)(Long size_0, Long size_1, const VALUE_TYPE *x,
                         Long x_stride_0, Long x_stride_1, VALUE_TYPE *y,
                         Long y_stride_0, Long y_stride_1);

void VALUE_TYPE_(Copy2D_1)(Long size_0, Long size_1, const VALUE_TYPE *x,
                           Long x_stride, VALUE_TYPE *y, Long y_stride);

void VALUE_TYPE_(CopyTranspose_F)(Long size_0, Long size_1, const VALUE_TYPE *x,
                                  Long x_stride, VALUE_TYPE *y, Long y_stride);

void VALUE_TYPE_(CopyTranspose_B)(Long size_0, Long size_1, const VALUE_TYPE *x,
                                  Long x_stride, VALUE_TYPE *y, Long y_stride);
