struct FUNCTION *FUNCTION_(BufferedLoop_new)(struct FUNCTIONW *func,
                                             Long loop_size, Long x_stride,
                                             Long y_stride);

struct FUNCTION *FUNCTION_(BufferedLoopR_new)(struct FUNCTIONW *func,
                                              struct FUNCTIONW *residual_func,
                                              Long loop_size, Long x_stride,
                                              Long y_stride);

struct FUNCTION *FUNCTION_(BufferedLoop_new_)(struct FUNCTIONW *func,
                                              Long loop_size, Long x_stride,
                                              Long y_stride, int num_threads);

struct FUNCTION *FUNCTION_(BufferedLoopR_new_)(struct FUNCTIONW *func,
                                               struct FUNCTIONW *residual_func,
                                               Long loop_size, Long x_stride,
                                               Long y_stride, int num_threads);
