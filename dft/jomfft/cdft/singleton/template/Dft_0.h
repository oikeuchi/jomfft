{
  assert(dft_size % 2 == 0);
  assert(dft_size > 2);
  const int m = (dft_size - 2) / 2;
  const int w_stride = m;
  CV_VECTOR y0;
  CV_VECTOR y1;
  DECLARE_VLA(CV_VECTOR, cs, 2 * m);
  {
    CV_VECTOR x0 = CDFT_LOAD(0);
    CV_VECTOR u = CDFT_LOAD(1);
    CV_VECTOR v = CDFT_LOAD(dft_size - 1);
    CV_DFT2(&u, &v);
    y0 = CV_ADD(x0, u);
    y1 = CV_SUB(x0, u);
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
    if (i % 2) {
      y1 = CV_ADD(y1, u);
    } else {
      y1 = CV_SUB(y1, u);
    }
    for (int j = 0; j < m; ++j) {
      const Complex w_ = w[w_stride * i + j];
      cs[2 * j] = RV_FMADD(RV_SCALAR(C_REAL(w_)), u, cs[2 * j]);
      cs[2 * j + 1] = RV_FMADD(RV_SCALAR(C_IMAG(w_)), v, cs[2 * j + 1]);
    }
  }
  CV_VECTOR x1 = CDFT_LOAD(m + 1);
  y0 = CV_ADD(y0, x1);
  if (m % 2) {
    y1 = CV_ADD(y1, x1);
  } else {
    y1 = CV_SUB(y1, x1);
  }
  CDFT_STORE(0, y0);
  CDFT_STORE(dft_size / 2, y1);
  for (int i = 0; i < m; ++i) {
    CV_VECTOR c = cs[2 * i];
    CV_VECTOR s = cs[2 * i + 1];
    if (i % 2) {
      c = RV_ADD(c, x1);
    } else {
      c = RV_SUB(c, x1);
    }
    CV_N_MULI_DFT2(&c, &s);
    CDFT_STORE(i + 1, c);
    CDFT_STORE(dft_size - (i + 1), s);
  }
}
