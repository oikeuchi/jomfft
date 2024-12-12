#ifndef RDFT_2_LOAD_RI_0
#define RDFT_2_LOAD_RI_0 RDFT_LOAD_RI_0
#endif

#ifndef RDFT_2_STORE
#define RDFT_2_STORE RDFT_STORE
#endif

#ifndef RDFT_2_INPUT_INDEX
#define RDFT_2_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_2_OUTPUT_INDEX
#define RDFT_2_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR z0, z1;
  RDFT_2_LOAD_RI_0(2, &z0, &z1);
  RV_DFT2(&z0, &z1);
  RDFT_2_STORE(RDFT_2_OUTPUT_INDEX(0), z0);
  RDFT_2_STORE(RDFT_2_OUTPUT_INDEX(1), z1);
}

#undef RDFT_2_LOAD_RI_0
#undef RDFT_2_STORE
#undef RDFT_2_INPUT_INDEX
#undef RDFT_2_OUTPUT_INDEX
