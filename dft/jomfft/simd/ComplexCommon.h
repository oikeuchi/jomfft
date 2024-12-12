static FORCEINLINE CV_VECTOR CV_LOAD(const Complex *p) {
  return RV_LOAD((const Real *)p);
}

static FORCEINLINE void CV_STORE(Complex *p, CV_VECTOR x) {
  RV_STORE((Real *)p, x);
}

#define CV_ZERO RV_ZERO
#define CV_ADD RV_ADD
#define CV_SUB RV_SUB

static FORCEINLINE CV_VECTOR CV_MUL(CV_VECTOR x, CV_VECTOR y) {
  return CV_MUL_RI(x, RV_DUPL(y), RV_DUPH(y));
}

static FORCEINLINE CV_VECTOR CV_MUL_CONJ(CV_VECTOR x, CV_VECTOR y) {
  return CV_MUL_RI_CONJ(x, RV_DUPL(y), RV_DUPH(y));
}

static FORCEINLINE CV_VECTOR CV_MUL_SCALAR(CV_VECTOR x, const Complex y) {
  return CV_MUL_RI(x, RV_SCALAR(C_REAL(y)), RV_SCALAR(C_IMAG(y)));
}

static FORCEINLINE CV_VECTOR CV_MUL_SCALAR_CONJ(CV_VECTOR x, const Complex y) {
  return CV_MUL_RI_CONJ(x, RV_SCALAR(C_REAL(y)), RV_SCALAR(C_IMAG(y)));
}

static FORCEINLINE CV_VECTOR CV_MULI(CV_VECTOR x) {
  return RV_SWAP_LH(CV_CONJ(x));
}

static FORCEINLINE CV_VECTOR CV_MULI_CONJ(CV_VECTOR x) {
  return CV_CONJ(RV_SWAP_LH(x));
}

static FORCEINLINE CV_VECTOR CV_MULI_ADD(CV_VECTOR x, CV_VECTOR y) {
  return CV_CONJ_SUB(x, RV_SWAP_LH(y));
}

static FORCEINLINE CV_VECTOR CV_MULI_SUB(CV_VECTOR x, CV_VECTOR y) {
  return CV_CONJ_ADD(x, RV_SWAP_LH(y));
}

#define CV_MULI_ADD_CONJ CV_MULI_SUB
#define CV_MULI_SUB_CONJ CV_MULI_ADD

#define CV_DFT2 RV_DFT2

static FORCEINLINE void CV_CONJ_DFT2(CV_VECTOR *x0, CV_VECTOR *x1) {
  CV_VECTOR t = CV_CONJ_ADD(*x0, *x1);
  *x1 = CV_CONJ_SUB(*x0, *x1);
  *x0 = t;
}

static FORCEINLINE void CV_MULI_DFT2(CV_VECTOR *x0, CV_VECTOR *x1) {
  CV_VECTOR t = CV_MULI_ADD(*x0, *x1);
  *x1 = CV_MULI_SUB(*x0, *x1);
  *x0 = t;
}

static FORCEINLINE void CV_MULI_DFT2_CONJ(CV_VECTOR *x0, CV_VECTOR *x1) {
  CV_VECTOR t = CV_MULI_ADD_CONJ(*x0, *x1);
  *x1 = CV_MULI_SUB_CONJ(*x0, *x1);
  *x0 = t;
}
