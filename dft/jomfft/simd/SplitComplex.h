static FORCEINLINE void SV_ADD(const RV_VECTOR *x, const RV_VECTOR *y,
                               RV_VECTOR *z) {
  z[0] = RV_ADD(x[0], y[0]);
  z[1] = RV_ADD(x[1], y[1]);
}

static FORCEINLINE void SV_FMADD_(RV_VECTOR *x, RV_VECTOR y, RV_VECTOR *z) {
  x[0] = RV_FMADD(x[0], y, z[0]);
  x[1] = RV_FMADD(x[1], y, z[1]);
}

static FORCEINLINE void SV_MUL_R_(RV_VECTOR *x, RV_VECTOR y) {
  x[0] = RV_MUL(x[0], y);
  x[1] = RV_MUL(x[1], y);
}

static FORCEINLINE void SV_DFT2(RV_VECTOR *x, RV_VECTOR *y) {
  RV_DFT2(x, y);
  RV_DFT2(x + 1, y + 1);
}

static FORCEINLINE void SV_MUL_RI_(RV_VECTOR *x, RV_VECTOR y_r, RV_VECTOR y_i) {
  RV_VECTOR t = RV_FNMADD(x[1], y_i, RV_MUL(x[0], y_r));
  x[1] = RV_FMADD(x[0], y_i, RV_MUL(x[1], y_r));
  x[0] = t;
}

static FORCEINLINE void SV_MUL_RI_CONJ_(RV_VECTOR *x, RV_VECTOR y_r,
                                        RV_VECTOR y_i) {
  RV_VECTOR t = RV_FMADD(x[1], y_i, RV_MUL(x[0], y_r));
  x[1] = RV_FNMADD(x[0], y_i, RV_MUL(x[1], y_r));
  x[0] = t;
}

static FORCEINLINE void SV_MULI_DFT2(RV_VECTOR *x, RV_VECTOR *y) {
  RV_VECTOR t0 = RV_SUB(x[0], y[1]);
  RV_VECTOR t1 = RV_ADD(x[0], y[1]);
  y[1] = RV_SUB(x[1], y[0]);
  x[1] = RV_ADD(x[1], y[0]);
  x[0] = t0;
  y[0] = t1;
}

static FORCEINLINE void SV_MULI_DFT2_CONJ(RV_VECTOR *x, RV_VECTOR *y) {
  RV_VECTOR t0 = RV_ADD(x[0], y[1]);
  RV_VECTOR t1 = RV_SUB(x[0], y[1]);
  y[1] = RV_ADD(x[1], y[0]);
  x[1] = RV_SUB(x[1], y[0]);
  x[0] = t0;
  y[0] = t1;
}

#if USE_COMPLEX_CONJ
#define SV_N_MUL_RI_ SV_MUL_RI_CONJ_
#define SV_C_MUL_RI_ SV_MUL_RI_
#define SV_N_MULI_DFT2 SV_MULI_DFT2_CONJ
#define SV_C_MULI_DFT2 SV_MULI_DFT2
#else
#define SV_N_MUL_RI_ SV_MUL_RI_
#define SV_C_MUL_RI_ SV_MUL_RI_CONJ_
#define SV_N_MULI_DFT2 SV_MULI_DFT2
#define SV_C_MULI_DFT2 SV_MULI_DFT2_CONJ
#endif
