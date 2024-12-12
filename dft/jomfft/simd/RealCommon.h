static FORCEINLINE void RV_DFT2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = RV_ADD(*x0, *x1);
  *x1 = RV_SUB(*x0, *x1);
  *x0 = t;
}

static FORCEINLINE void RV_DFT2_CONJ(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = RV_ADD(*x0, *x1);
  *x1 = RV_SUB(*x1, *x0);
  *x0 = t;
}

static FORCEINLINE void RV_CONJ_DFT2(RV_VECTOR *x0, RV_VECTOR *x1) {
  RV_VECTOR t = RV_ADD(*x0, *x1);
  *x0 = RV_SUB(*x0, *x1);
  *x1 = t;
}

static FORCEINLINE void RV_DFT2_MULI_CONJ(RV_VECTOR *x0_r, RV_VECTOR *x0_i,
                                          RV_VECTOR *x1_r, RV_VECTOR *x1_i) {
  RV_VECTOR t = RV_SUB(*x1_r, *x0_r);
  *x0_r = RV_ADD(*x1_r, *x0_r);
  *x1_r = RV_SUB(*x0_i, *x1_i);
  *x0_i = RV_ADD(*x0_i, *x1_i);
  *x1_i = t;
}

static FORCEINLINE void RV_MULI_DFT2(RV_VECTOR *x0_r, RV_VECTOR *x0_i,
                                     RV_VECTOR *x1_r, RV_VECTOR *x1_i) {
  RV_VECTOR t = RV_ADD(*x0_r, *x1_i);
  *x0_r = RV_SUB(*x0_r, *x1_i);
  *x1_i = RV_SUB(*x0_i, *x1_r);
  *x0_i = RV_ADD(*x0_i, *x1_r);
  *x1_r = t;
}

static FORCEINLINE void RV_RI_MUL(RV_VECTOR *x_r, RV_VECTOR *x_i, RV_VECTOR y_r,
                                  RV_VECTOR y_i) {
  RV_VECTOR t = RV_FMSUB(*x_r, y_r, RV_MUL(*x_i, y_i));
  *x_i = RV_FMADD(*x_i, y_r, RV_MUL(*x_r, y_i));
  *x_r = t;
}

static FORCEINLINE void RV_RI_MUL_CONJ(RV_VECTOR *x_r, RV_VECTOR *x_i,
                                       RV_VECTOR y_r, RV_VECTOR y_i) {
  RV_VECTOR t = RV_FMADD(*x_r, y_r, RV_MUL(*x_i, y_i));
  *x_i = RV_FMSUB(*x_i, y_r, RV_MUL(*x_r, y_i));
  *x_r = t;
}
