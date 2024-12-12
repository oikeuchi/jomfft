struct FUNCTIONW *FUNCTIONW_(Loop_new)(struct FUNCTIONW *func, Long loop_size,
                                       Long x_stride, Long y_stride);

struct FUNCTIONW *FUNCTIONW_(LoopR_new)(struct FUNCTIONW *func,
                                        struct FUNCTIONW *residual_func,
                                        Long loop_size, Long x_stride,
                                        Long y_stride);
