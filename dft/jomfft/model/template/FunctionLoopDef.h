struct FUNCTION *FUNCTION_(Loop_new)(struct FUNCTION *func, Long loop_size,
                                     Long x_stride, Long y_stride);

struct FUNCTION *FUNCTION_(LoopR_new)(struct FUNCTION *func,
                                      struct FUNCTION *residual_func,
                                      Long loop_size, Long x_stride,
                                      Long y_stride);

struct FUNCTION *FUNCTION_(Loop_new_)(struct FUNCTION *func, Long loop_size,
                                      Long x_stride, Long y_stride,
                                      int num_threads);

struct FUNCTION *FUNCTION_(LoopR_new_)(struct FUNCTION *func,
                                       struct FUNCTION *residual_func,
                                       Long loop_size, Long x_stride,
                                       Long y_stride, int num_threads);
