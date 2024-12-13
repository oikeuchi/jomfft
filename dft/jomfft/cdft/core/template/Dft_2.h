#ifndef CDFT_2_LOAD
#define CDFT_2_LOAD CDFT_LOAD
#endif

#ifndef CDFT_2_STORE
#define CDFT_2_STORE CDFT_STORE
#endif

#ifndef CDFT_2_INPUT_INDEX
#define CDFT_2_INPUT_INDEX(i) (i)
#endif

#ifndef CDFT_2_OUTPUT_INDEX
#define CDFT_2_OUTPUT_INDEX(i) (i)
#endif

{
  CV_VECTOR z0, z1;
  CDFT_2_LOAD(CDFT_2_INPUT_INDEX(0), &z0);
  CDFT_2_LOAD(CDFT_2_INPUT_INDEX(1), &z1);
  CV_DFT2(&z0, &z1);
  CDFT_2_STORE(CDFT_2_OUTPUT_INDEX(0), z0);
  CDFT_2_STORE(CDFT_2_OUTPUT_INDEX(1), z1);
}

#undef CDFT_2_LOAD
#undef CDFT_2_STORE
#undef CDFT_2_INPUT_INDEX
#undef CDFT_2_OUTPUT_INDEX
