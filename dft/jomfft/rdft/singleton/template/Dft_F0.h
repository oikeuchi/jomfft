{
  assert(dft_size % 2 == 0);
  assert(dft_size > 2);
  const int m = (dft_size - 2) / 2;
  const int w_stride = m;
  RV_VECTOR y0;
  RV_VECTOR y1;
  DECLARE_VLA(RV_VECTOR, cs, 2 * m);
  {
    RV_VECTOR x0 = RDFT_LOAD(0);
    RV_VECTOR u = RDFT_LOAD(1);
    RV_VECTOR v = RDFT_LOAD(dft_size - 1);
    RV_DFT2(&u, &v);
    y0 = RV_ADD(x0, u);
    y1 = RV_SUB(x0, u);
    for (int j = 0; j < m; ++j) {
      const Complex w_ = w[j];
      cs[2 * j] = RV_FMADD(RV_SCALAR(C_REAL(w_)), u, x0);
      cs[2 * j + 1] = RV_MUL(RV_SCALAR(C_IMAG(w_)), v);
    }
  }
  for (int i = 1; i < m; ++i) {
    RV_VECTOR u = RDFT_LOAD(i + 1);
    RV_VECTOR v = RDFT_LOAD(dft_size - (i + 1));
    RV_DFT2(&u, &v);
    y0 = RV_ADD(y0, u);
    if (i % 2) {
      y1 = RV_ADD(y1, u);
    } else {
      y1 = RV_SUB(y1, u);
    }
    for (int j = 0; j < m; ++j) {
      const Complex w_ = w[w_stride * i + j];
      cs[2 * j] = RV_FMADD(RV_SCALAR(C_REAL(w_)), u, cs[2 * j]);
      cs[2 * j + 1] = RV_FMADD(RV_SCALAR(C_IMAG(w_)), v, cs[2 * j + 1]);
    }
  }
  RV_VECTOR x1 = RDFT_LOAD(m + 1);
  y0 = RV_ADD(y0, x1);
  if (m % 2) {
    y1 = RV_ADD(y1, x1);
  } else {
    y1 = RV_SUB(y1, x1);
  }
  RDFT_STORE_RI_0(dft_size, y0, y1);
  for (int i = 0; i < m; ++i) {
    RV_VECTOR c = cs[2 * i];
    RV_VECTOR s = cs[2 * i + 1];
    if (i % 2) {
      c = RV_ADD(c, x1);
    } else {
      c = RV_SUB(c, x1);
    }
    RDFT_STORE_RI(i + 1, c, s);
  }
}
