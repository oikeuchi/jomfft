#include "Plan.h"

#if USE_FFTW

FFTW_(plan) * NS(Fftw_Plan_new)() {
  FFTW_(plan) *out = NEW(FFTW_(plan));
  *out = 0;
  return out;
}

void NS(Fftw_Plan_destruct)(void *p) {
  if (p) {
    FFTW_(plan) *o = (FFTW_(plan) *)p;
    FFTW_(destroy_plan)(*o);
    DELETE(o);
  }
}

void NS(Fftw_Plan_print)(FFTW_(plan) * p) {
  FFTW_(print_plan)(*p);
  printf("\n");
}

#endif
