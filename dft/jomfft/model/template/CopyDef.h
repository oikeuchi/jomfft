struct FUNCTION *VALUE_TYPE_(Copy_new)(Long size, Long x_stride, Long y_stride);

struct FUNCTION *VALUE_TYPE_(Copy_new_)(Long size, Long x_stride, Long y_stride,
                                        int num_threads);

struct FUNCTION *VALUE_TYPE_(Copy2D_new)(Long size_0, Long size_1,
                                         Long x_stride_0, Long x_stride_1,
                                         Long y_stride_0, Long y_stride_1);
