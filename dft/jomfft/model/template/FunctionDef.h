struct FUNCTION;

typedef void (*FUNCTION_(_fn))(struct FUNCTION *, INPUT_TYPE *, OUTPUT_TYPE *);

struct FUNCTION *FUNCTION_(_new)(FUNCTION_(_fn) evaluate,
                                 jomfftDestruct_fn destruct, void *data);

void FUNCTION_(_delete)(struct FUNCTION *p);

void *FUNCTION_(_data)(struct FUNCTION *p);

void FUNCTION_(_evaluate)(struct FUNCTION *p, INPUT_TYPE *x, OUTPUT_TYPE *y);
