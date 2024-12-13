#ifndef RDFT_4_LOAD
#define RDFT_4_LOAD RDFT_LOAD
#endif

#ifndef RDFT_4_STORE_RI_0
#define RDFT_4_STORE_RI_0 RDFT_STORE_RI_0
#endif

#ifndef RDFT_4_STORE_RI
#define RDFT_4_STORE_RI RDFT_STORE_RI
#endif

#ifndef RDFT_4_INPUT_INDEX
#define RDFT_4_INPUT_INDEX(i) (i)
#endif

#ifndef RDFT_4_OUTPUT_INDEX
#define RDFT_4_OUTPUT_INDEX(i) (i)
#endif

{
  RV_VECTOR z0 = RDFT_4_LOAD(RDFT_4_INPUT_INDEX(0));
  RV_VECTOR z1 = RDFT_4_LOAD(RDFT_4_INPUT_INDEX(2));
  RV_DFT2(&z0, &z1);
  RV_VECTOR z2 = RDFT_4_LOAD(RDFT_4_INPUT_INDEX(1));
  RV_VECTOR z3 = RDFT_4_LOAD(RDFT_4_INPUT_INDEX(3));
  RV_DFT2_CONJ(&z2, &z3);
  RV_DFT2(&z0, &z2);
  RDFT_4_STORE_RI_0(4, z0, z2);
  RDFT_4_STORE_RI(RDFT_4_OUTPUT_INDEX(1), z1, z3);
}

#undef RDFT_4_LOAD
#undef RDFT_4_STORE_RI_0
#undef RDFT_4_STORE_RI
#undef RDFT_4_INPUT_INDEX
#undef RDFT_4_OUTPUT_INDEX
