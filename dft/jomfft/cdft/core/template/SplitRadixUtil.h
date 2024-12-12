#pragma once

#define CDFT_SPLITRADIX_MERGE_(s, i, t0, t1, u, v)                             \
  do {                                                                         \
    CV_DFT2(&u, &v);                                                           \
    CV_DFT2(&t0, &u);                                                          \
    CV_C_MULI_DFT2(&t1, &v);                                                   \
    CDFT_SPLITRADIX_STORE((i), t0);                                            \
    CDFT_SPLITRADIX_STORE((i) + (s), t1);                                      \
    CDFT_SPLITRADIX_STORE((i) + 2 * (s), u);                                   \
    CDFT_SPLITRADIX_STORE((i) + 3 * (s), v);                                   \
  } while (0)

#define CDFT_SPLITRADIX_MERGE0(r, t, u, v)                                     \
  do {                                                                         \
    const int s__ = r / 4;                                                     \
    CV_VECTOR t0__ = t[0];                                                     \
    CV_VECTOR t1__ = t[s__];                                                   \
    CV_VECTOR u__ = u[0];                                                      \
    CV_VECTOR v__ = v[0];                                                      \
    CDFT_SPLITRADIX_MERGE_(s__, 0, t0__, t1__, u__, v__);                      \
  } while (0)

#define CDFT_SPLITRADIX_MERGE_8(r, t, u, v)                                    \
  do {                                                                         \
    const int s__ = r / 4;                                                     \
    const int i__ = r / 8;                                                     \
    CV_VECTOR t0__ = t[i__];                                                   \
    CV_VECTOR t1__ = t[i__ + s__];                                             \
    CV_VECTOR u__ = CV_N_MULW8(u[i__]);                                        \
    CV_VECTOR v__ = CV_C_MULW8(v[i__]);                                        \
    CDFT_SPLITRADIX_MERGE_(s__, i__, t0__, t1__, u__, v__);                    \
  } while (0)

#define CDFT_SPLITRADIX_MERGE(r, i, wc, ws, t, u, v)                           \
  do {                                                                         \
    const int s__ = r / 4;                                                     \
    CV_VECTOR wc__ = RV_SCALAR(wc);                                            \
    CV_VECTOR ws__ = RV_SCALAR(ws);                                            \
    {                                                                          \
      const int i__ = (i);                                                     \
      CV_VECTOR t0__ = t[i__];                                                 \
      CV_VECTOR t1__ = t[i__ + s__];                                           \
      CV_VECTOR u__ = CV_C_MUL_RI(u[i__], wc__, ws__);                         \
      CV_VECTOR v__ = CV_N_MUL_RI(v[i__], wc__, ws__);                         \
      CDFT_SPLITRADIX_MERGE_(s__, i__, t0__, t1__, u__, v__);                  \
    }                                                                          \
    {                                                                          \
      const int i__ = s__ - (i);                                               \
      CV_VECTOR t0__ = t[i__];                                                 \
      CV_VECTOR t1__ = t[i__ + s__];                                           \
      CV_VECTOR u__ = CV_C_MUL_RI(u[i__], ws__, wc__);                         \
      CV_VECTOR v__ = CV_N_MUL_RI(v[i__], ws__, wc__);                         \
      CDFT_SPLITRADIX_MERGE_(s__, i__, t0__, t1__, u__, v__);                  \
    }                                                                          \
  } while (0)
