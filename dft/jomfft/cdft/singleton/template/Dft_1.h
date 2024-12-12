{
  assert(dft_size > 2);
  assert(dft_size % 2 == 1);
  const int m = (dft_size - 1) / 2;
  const int w_stride = m;
  CV_VECTOR y0;
  DECLARE_VLA(CV_VECTOR, cs, 2 * m);
  {
    CV_VECTOR x0 = CDFT_LOAD(0);
    CV_VECTOR u = CDFT_LOAD(1);
    CV_VECTOR v = CDFT_LOAD(dft_size - 1);
    CV_DFT2(&u, &v);
    y0 = CV_ADD(x0, u);
    for (int j = 0; j < m; ++j) {
      const Complex w_ = w[j];
      cs[2 * j] = RV_FMADD(RV_SCALAR(C_REAL(w_)), u, x0);
      cs[2 * j + 1] = RV_MUL(RV_SCALAR(C_IMAG(w_)), v);
    }
  }
  for (int i = 1; i < m; ++i) {
    CV_VECTOR u = CDFT_LOAD(i + 1);
    CV_VECTOR v = CDFT_LOAD(dft_size - (i + 1));
    CV_DFT2(&u, &v);
    y0 = CV_ADD(y0, u);
    for (int j = 0; j < m; ++j) {
      const Complex w_ = w[w_stride * i + j];
      cs[2 * j] = RV_FMADD(RV_SCALAR(C_REAL(w_)), u, cs[2 * j]);
      cs[2 * j + 1] = RV_FMADD(RV_SCALAR(C_IMAG(w_)), v, cs[2 * j + 1]);
    }
  }
  CDFT_STORE(0, y0);
  for (int i = 0; i < m; ++i) {
    CV_VECTOR c = cs[2 * i];
    CV_VECTOR s = cs[2 * i + 1];
    CV_N_MULI_DFT2(&c, &s);
    CDFT_STORE(i + 1, c);
    CDFT_STORE(dft_size - (i + 1), s);
  }
}
