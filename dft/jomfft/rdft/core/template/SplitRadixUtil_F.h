#pragma once

#define RDFT_SPLITRADIX_MERGE_(r, s, i, t0_r, t0_i, t1_r, t1_i, u_r, u_i, v_r, \
                               v_i)                                            \
  do {                                                                         \
    RV_DFT2_MULI_CONJ(&u_r, &u_i, &v_r, &v_i);                                 \
    RV_DFT2(&t0_r, &u_r);                                                      \
    RV_DFT2_CONJ(&t0_i, &u_i);                                                 \
    RV_DFT2(&t1_r, &v_r);                                                      \
    RV_DFT2(&v_i, &t1_i);                                                      \
    RDFT_SPLITRADIX_STORE_RI((i), t0_r, t0_i);                                 \
    RDFT_SPLITRADIX_STORE_RI((i) + (s), t1_r, t1_i);                           \
    RDFT_SPLITRADIX_STORE_RI((r) - ((i) + 2 * (s)), u_r, u_i);                 \
    RDFT_SPLITRADIX_STORE_RI((r) - ((i) + 3 * (s)), v_r, v_i);                 \
  } while (0)

#define RDFT_SPLITRADIX_MERGE0(r, t, u, v)                                     \
  do {                                                                         \
    const int s__ = r / 4;                                                     \
    RV_VECTOR u_8, v_8;                                                        \
    {                                                                          \
      RV_VECTOR t0, t_4, u0, v0;                                               \
      RDFT_LOAD_ARRAY_RI_0(t, &t0, &t_4);                                      \
      RDFT_LOAD_ARRAY_RI_0(u, &u0, &u_8);                                      \
      RDFT_LOAD_ARRAY_RI_0(v, &v0, &v_8);                                      \
      RV_DFT2_CONJ(&u0, &v0);                                                  \
      RV_DFT2(&t0, &u0);                                                       \
      RDFT_SPLITRADIX_STORE_RI_0(r, t0, u0);                                   \
      RDFT_SPLITRADIX_STORE_RI(s__, t_4, v0);                                  \
    }                                                                          \
    {                                                                          \
      const int i__ = r / 8;                                                   \
      RV_VECTOR t_8_r, t_8_i;                                                  \
      RDFT_LOAD_ARRAY_RI(t, i__, &t_8_r, &t_8_i);                              \
      RV_DFT2_CONJ(&u_8, &v_8);                                                \
      u_8 = RV_MUL(RV_SCALAR(M_SQRT1_2), u_8);                                 \
      v_8 = RV_MUL(RV_SCALAR(M_SQRT1_2), v_8);                                 \
      RV_DFT2(&t_8_r, &u_8);                                                   \
      RV_DFT2_CONJ(&t_8_i, &v_8);                                              \
      RDFT_SPLITRADIX_STORE_RI(i__, t_8_r, t_8_i);                             \
      RDFT_SPLITRADIX_STORE_RI(i__ + s__, u_8, v_8);                           \
    }                                                                          \
  } while (0)

#define RDFT_SPLITRADIX_MERGE(r, i, wc, ws, t, u, v)                           \
  do {                                                                         \
    const int s__ = r / 4;                                                     \
    RV_VECTOR wc__ = RV_SCALAR(wc);                                            \
    RV_VECTOR ws__ = RV_SCALAR(ws);                                            \
    RV_VECTOR t0_r, t0_i, t1_r, t1_i, u_r, u_i, v_r, v_i;                      \
    RDFT_LOAD_ARRAY_RI(t, i, &t0_r, &t0_i);                                    \
    RDFT_LOAD_ARRAY_RI(t, s__ - (i), &t1_r, &t1_i);                            \
    RDFT_LOAD_ARRAY_RI(u, i, &u_r, &u_i);                                      \
    RDFT_LOAD_ARRAY_RI(v, i, &v_r, &v_i);                                      \
    RV_RI_MUL_CONJ(&u_r, &u_i, wc__, ws__);                                    \
    RV_RI_MUL(&v_r, &v_i, wc__, ws__);                                         \
    RDFT_SPLITRADIX_MERGE_(r, s__, i, t0_r, t0_i, t1_r, t1_i, u_r, u_i, v_r,   \
                           v_i);                                               \
  } while (0)
