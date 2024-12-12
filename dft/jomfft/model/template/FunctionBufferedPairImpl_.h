struct FUNCTION *FUNCTION_(BufferedPair_0_new)(struct FUNCTION_0 *first,
                                               struct FUNCTION *second,
                                               Long buffer_size) {
  return FUNCTION_(Buffered_new)(
      FUNCTION_(PairW_0_new)(first, second, buffer_size));
}

struct FUNCTION *FUNCTION_(BufferedPair_1_new)(struct FUNCTION *first,
                                               struct FUNCTION_1 *second,
                                               Long buffer_size) {
  return FUNCTION_(Buffered_new)(
      FUNCTION_(PairW_1_new)(first, second, buffer_size));
}
