struct FUNCTIONW;

typedef void (*FUNCTIONW_(_fn))(struct FUNCTIONW *, INPUT_TYPE *, OUTPUT_TYPE *,
                                void *);

typedef Long (*FUNCTIONW_(_Size_fn))(struct FUNCTIONW *);

struct FUNCTIONW *FUNCTIONW_(_new)(FUNCTIONW_(_fn) evaluate,
                                   FUNCTIONW_(_Size_fn) size,
                                   jomfftDestruct_fn destruct, void *data);

void FUNCTIONW_(_delete)(struct FUNCTIONW *p);

void *FUNCTIONW_(_data)(struct FUNCTIONW *p);

Long FUNCTIONW_(_size)(struct FUNCTIONW *p);

void FUNCTIONW_(_evaluate)(struct FUNCTIONW *p, INPUT_TYPE *x, OUTPUT_TYPE *y,
                           void *work);
