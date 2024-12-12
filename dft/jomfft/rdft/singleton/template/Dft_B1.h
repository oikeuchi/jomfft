{
  assert(dft_size % 2 == 1);
  const int m = (dft_size - 1) / 2;
  const int w_stride = m;
  RV_VECTOR y0;
  DECLARE_VLA(RV_VECTOR, cs, 2 * m);
  {
    RV_VECTOR x0 = RDFT_LOAD_RI_0_(dft_size);
    RV_VECTOR u, v;
    RDFT_LOAD_RI(1, &u, &v);
    u = RV_ADD(u, u);
    v = RV_ADD(v, v);
    y0 = RV_ADD(x0, u);
    for (int j = 0; j < m; ++j) {
      const Complex w_ = w[j];
      cs[2 * j] = RV_FMADD(RV_SCALAR(C_REAL(w_)), u, x0);
      cs[2 * j + 1] = RV_MUL(RV_SCALAR(C_IMAG(w_)), v);
    }
  }
  for (int i = 1; i < m; ++i) {
    RV_VECTOR u, v;
    RDFT_LOAD_RI(i + 1, &u, &v);
    u = RV_ADD(u, u);
    v = RV_ADD(v, v);
    y0 = RV_ADD(y0, u);
    for (int j = 0; j < m; ++j) {
      const Complex w_ = w[w_stride * i + j];
      cs[2 * j] = RV_FMADD(RV_SCALAR(C_REAL(w_)), u, cs[2 * j]);
      cs[2 * j + 1] = RV_FMADD(RV_SCALAR(C_IMAG(w_)), v, cs[2 * j + 1]);
    }
  }
  RDFT_STORE(0, y0);
  for (int i = 0; i < m; ++i) {
    RV_VECTOR c = cs[2 * i];
    RV_VECTOR s = cs[2 * i + 1];
    RV_DFT2(&c, &s);
    RDFT_STORE(i + 1, c);
    RDFT_STORE(dft_size - (i + 1), s);
  }
}
