struct FUNCTION *FUNCTION_(BufferedTriple_new)(struct FUNCTION_0 *func0,
                                               struct FUNCTION *func1,
                                               struct FUNCTION_1 *func2,
                                               Long buffer_size) {
  return FUNCTION_(Buffered_new)(
      FUNCTION_(TripleW_new)(func0, func1, func2, buffer_size));
}
