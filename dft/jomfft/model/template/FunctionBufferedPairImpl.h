struct FUNCTION *FUNCTION_(BufferedPair_new)(struct FUNCTION *first,
                                             struct FUNCTION *second,
                                             Long buffer_size) {
  return FUNCTION_(Buffered_new)(
      FUNCTION_(PairW_new)(first, second, buffer_size));
}
