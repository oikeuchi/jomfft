#include <jomfft/fftw/ComplexDft.h>
#include <jomfft/fftw/Plan.h>
#include <jomfft/fftw/RealDft.h>
#include <jomfft/math/Math.h>
#include <jomfft/util/GetOpt.h>
#include <jomfft/util/LongUtil.h>
#include <jomfft/util/random/ComplexTensor.h>
#include <jomfft/util/random/ConjugateEvenTensor.h>
#include <jomfft/util/random/RealTensor.h>
#include <jomfft/wrapper/ComplexDft.h>
#include <jomfft/wrapper/ComplexDftSizes.h>
#include <jomfft/wrapper/RealDft.h>
#include <jomfft/wrapper/RealDftSizes.h>

#if !(USE_FFTW)
#error "Comparison module requires fftw."
#endif

#define FLAGS FFTW_ESTIMATE
#define MAX_DIM 8

static void PrintResult_(Complex mean, Complex stdev, Real max_abs) {
  printf("mean: (%.7e,%.7e), stdev: (%.7e,%7e), max_abs: %.7e\n", C_REAL(mean),
         C_IMAG(mean), C_REAL(stdev), C_IMAG(stdev), max_abs);
}

struct ComplexStats {
  Complex mean;
  Complex stdev;
  Real max_abs;
};

static void ComplexTensor_Compare_(int dimension, int i, const Long *sizes,
                                   Long *indexes, struct NS(ComplexTensor) *x,
                                   struct NS(ComplexTensor) *y,
                                   Complex *diff_sum, Complex *sq_diff_sum,
                                   Real *max_abs) {
  if (i == dimension - 1) {
    for (int j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      Complex x0 = NS(ComplexTensor_get)(x, indexes);
      Complex y0 = NS(ComplexTensor_get)(y, indexes);
      Complex d = x0 - y0;
      *diff_sum += d;
      Real s_r = C_REAL(d) * C_REAL(d);
      Real s_i = C_IMAG(d) * C_IMAG(d);
      *sq_diff_sum = C_ADD(*sq_diff_sum, Complex_(s_r, s_i));
      Real a = R_SQRT(s_r + s_i);
      *max_abs = MAX(*max_abs, a);
    }
  } else {
    for (int j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      ComplexTensor_Compare_(dimension, i + 1, sizes, indexes, x, y, diff_sum,
                             sq_diff_sum, max_abs);
    }
  }
}

static struct ComplexStats ComplexTensor_Compare(int dimension,
                                                 const Long *sizes,
                                                 struct NS(ComplexTensor) *x,
                                                 struct NS(ComplexTensor) *y) {
  Complex diff_sum = 0;
  Complex sq_diff_sum = 0;
  Real max_abs = 0;
  {
    Long *indexes = ARRAY_NEW(Long, dimension);
    ARRAY_ZERO(Long, dimension, indexes);
    ComplexTensor_Compare_(dimension, 0, sizes, indexes, x, y, &diff_sum,
                           &sq_diff_sum, &max_abs);
    ARRAY_DELETE(indexes);
  }
  Long data_size = jomfftLong_Product(dimension, sizes);
  Complex mean = diff_sum / data_size;
  Real s_r = R_SQRT(C_REAL(sq_diff_sum) / data_size);
  Real s_i = R_SQRT(C_IMAG(sq_diff_sum) / data_size);
  Complex stdev = Complex_(s_r, s_i);
  return (struct ComplexStats){
      .mean = mean, .stdev = stdev, .max_abs = max_abs};
}

static void PrintResultComplex(struct ComplexStats s) {
  PrintResult_(s.mean, s.stdev, s.max_abs);
}

static struct NS(ComplexFunction) *
ComplexDft_new(bool backward, const struct NS(Wrapper_ComplexDftSizes) *s,
               bool inplace, int mode) {
  switch (mode) {
  case 0:
    return NS(Wrapper_ComplexDft_new_0)(backward, s->dimension, s->dft_sizes,
                                        inplace);
  case 1:
    return NS(Wrapper_ComplexDft_new_1)(backward, s, inplace);
  case 2:
  default:
    assert(mode == 2);
    return NS(Wrapper_ComplexDft_new_2)(backward, s, inplace);
  }
}

static void DoMainComplex(bool backward, int dimension, const int *dft_sizes,
                          int batch_size, int x_margin, int x_stride,
                          int y_margin, int y_stride, bool inner_batch,
                          bool inplace, int mode) {
  struct NS(Wrapper_ComplexDftSizes) *s = NS(Wrapper_ComplexDftSizes_new)(
      dimension, dft_sizes, batch_size, x_margin, x_stride, y_margin, y_stride,
      inner_batch);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new)(s->tensor_dimension, s->tensor_sizes);
  struct NS(ComplexTensor) *y0 =
      NS(ComplexTensor_new)(s->tensor_dimension, s->out_tensor_storage_sizes);
  struct NS(ComplexTensor) *y1 = NS(ComplexTensor_zerosLike)(y0);
  if (inplace) {
    NS(ComplexTensor_CopyAs)(x->shape, x, y0);
    {
      struct NS(ComplexFunction) *f =
          ComplexDft_new(backward, s, inplace, mode);
      NS(ComplexFunction_evaluate)(f, y0->data, y0->data);
      NS(ComplexFunction_delete)(f);
    }
    NS(ComplexTensor_CopyAs)(x->shape, x, y1);
    {
      struct NS(ComplexFunction) *f =
          NS(Fftw_ComplexDft_new)(backward, s, inplace, FLAGS);
      NS(ComplexFunction_evaluate)(f, y1->data, y1->data);
      NS(ComplexFunction_delete)(f);
    }
  } else {
    {
      struct NS(ComplexTensor) *x_ = NS(ComplexTensor_new)(
          s->tensor_dimension, s->in_tensor_storage_sizes);
      NS(ComplexTensor_CopyAs)(x->shape, x, x_);
      struct NS(ComplexFunction) *f =
          ComplexDft_new(backward, s, inplace, mode);
      NS(ComplexFunction_evaluate)(f, x_->data, y0->data);
      NS(ComplexFunction_delete)(f);
      NS(ComplexTensor_delete)(x_);
    }
    {
      struct NS(ComplexTensor) *x_ = NS(ComplexTensor_new)(
          s->tensor_dimension, s->in_tensor_storage_sizes);
      NS(ComplexTensor_CopyAs)(x->shape, x, x_);
      struct NS(ComplexFunction) *f =
          NS(Fftw_ComplexDft_new)(backward, s, inplace, FLAGS);
      NS(ComplexFunction_evaluate)(f, x_->data, y1->data);
      NS(ComplexFunction_delete)(f);
      NS(ComplexTensor_delete)(x_);
    }
  }
  struct ComplexStats stats =
      ComplexTensor_Compare(s->tensor_dimension, s->tensor_sizes, y0, y1);

  PrintResultComplex(stats);

  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(Wrapper_ComplexDftSizes_delete)(s);
}

struct RealStats {
  Real mean;
  Real stdev;
  Real max_abs;
};

static void RealTensor_Compare_(int dimension, int i, const Long *sizes,
                                Long *indexes, struct NS(RealTensor) *x,
                                struct NS(RealTensor) *y, Real *diff_sum,
                                Real *sq_diff_sum, Real *max_abs) {
  if (i == dimension - 1) {
    for (int j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      Real x0 = NS(RealTensor_get)(x, indexes);
      Real y0 = NS(RealTensor_get)(y, indexes);
      Real d = x0 - y0;
      *diff_sum += d;
      Real s = d * d;
      *sq_diff_sum += s;
      Real a = R_SQRT(s);
      *max_abs = MAX(*max_abs, a);
    }
  } else {
    for (int j = 0; j < sizes[i]; ++j) {
      indexes[i] = j;
      RealTensor_Compare_(dimension, i + 1, sizes, indexes, x, y, diff_sum,
                          sq_diff_sum, max_abs);
    }
  }
}

static struct RealStats RealTensor_Compare(int dimension, const Long *sizes,
                                           struct NS(RealTensor) *x,
                                           struct NS(RealTensor) *y) {
  Real diff_sum = 0;
  Real sq_diff_sum = 0;
  Real max_abs = 0;
  {
    Long *indexes = ARRAY_NEW(Long, dimension);
    ARRAY_ZERO(Long, dimension, indexes);
    RealTensor_Compare_(dimension, 0, sizes, indexes, x, y, &diff_sum,
                        &sq_diff_sum, &max_abs);
    ARRAY_DELETE(indexes);
  }
  Long data_size = jomfftLong_Product(dimension, sizes);
  Real mean = diff_sum / data_size;
  Real stdev = sq_diff_sum / data_size;
  return (struct RealStats){.mean = mean, .stdev = stdev, .max_abs = max_abs};
}

static void PrintResultReal(struct RealStats s) {
  PrintResult_(s.mean, s.stdev, s.max_abs);
}

static struct NS(RealToComplexFunction) *
RealDft_F_new(const struct NS(Wrapper_RealDftSizes) *s, bool inplace,
              int mode) {
  switch (mode) {
  case 0:
    return NS(Wrapper_RealDft_F_new_0)(s->dimension, s->dft_sizes, inplace);
  case 1:
    return NS(Wrapper_RealDft_F_new_1)(s, inplace);
  case 2:
  default:
    assert(mode == 2);
    return NS(Wrapper_RealDft_F_new_2)(s, inplace);
  }
}

static struct NS(ComplexToRealFunction) *
RealDft_B_new(const struct NS(Wrapper_RealDftSizes) *s, bool inplace,
              int mode) {
  switch (mode) {
  case 0:
    return NS(Wrapper_RealDft_B_new_0)(s->dimension, s->dft_sizes, inplace);
  case 1:
    return NS(Wrapper_RealDft_B_new_1)(s, inplace);
  case 2:
  default:
    assert(mode == 2);
    return NS(Wrapper_RealDft_B_new_2)(s, inplace);
  }
}

static void DoMainReal_F(int dimension, const int *dft_sizes, int batch_size,
                         int x_margin, int x_stride, int y_margin, int y_stride,
                         bool inner_batch, bool inplace, int mode) {
  struct NS(Wrapper_RealDftSizes) *s = NS(Wrapper_RealDftSizes_new)(
      dimension, dft_sizes, batch_size, x_margin, x_stride, y_margin, y_stride,
      inner_batch, inplace);
  struct NS(RealTensor) *x =
      NS(Random_RealTensor_new)(s->tensor_dimension, s->tensor_sizes);
  struct NS(ComplexTensor) *y0 =
      NS(ComplexTensor_new)(s->tensor_dimension, s->out_tensor_storage_sizes);
  struct NS(ComplexTensor) *y1 =
      NS(ComplexTensor_new)(s->tensor_dimension, s->out_tensor_storage_sizes);
  if (inplace) {
    NS(RealTensor_CopyAsComplex)(x->shape, x, y0);
    {
      struct NS(RealToComplexFunction) *f = RealDft_F_new(s, inplace, mode);
      NS(RealToComplexFunction_evaluate)(f, (Real *)y0->data, y0->data);
      NS(RealToComplexFunction_delete)(f);
    }
    NS(RealTensor_CopyAsComplex)(x->shape, x, y1);
    {
      struct NS(RealToComplexFunction) *f =
          NS(Fftw_RealDft_F_new)(s, inplace, FLAGS);
      NS(RealToComplexFunction_evaluate)(f, (Real *)y1->data, y1->data);
      NS(RealToComplexFunction_delete)(f);
    }
  } else {
    {
      struct NS(RealTensor) *x_ =
          NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
      NS(RealTensor_CopyAs)(x->shape, x, x_);
      struct NS(RealToComplexFunction) *f = RealDft_F_new(s, inplace, mode);
      NS(RealToComplexFunction_evaluate)(f, x_->data, y0->data);
      NS(RealToComplexFunction_delete)(f);
      NS(RealTensor_delete)(x_);
    }
    {
      struct NS(RealTensor) *x_ =
          NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
      NS(RealTensor_CopyAs)(x->shape, x, x_);
      struct NS(RealToComplexFunction) *f =
          NS(Fftw_RealDft_F_new)(s, inplace, FLAGS);
      NS(RealToComplexFunction_evaluate)(f, x_->data, y1->data);
      NS(RealToComplexFunction_delete)(f);
      NS(RealTensor_delete)(x_);
    }
  }
  struct ComplexStats stats =
      ComplexTensor_Compare(s->tensor_dimension, s->ce_tensor_sizes, y0, y1);

  PrintResultComplex(stats);

  NS(RealTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(Wrapper_RealDftSizes_delete)(s);
}

static void DoMainReal_B(int dimension, const int *dft_sizes, int batch_size,
                         int x_margin, int x_stride, int y_margin, int y_stride,
                         bool inner_batch, bool inplace, int mode) {
  struct NS(Wrapper_RealDftSizes) *s = NS(Wrapper_RealDftSizes_new)(
      dimension, dft_sizes, batch_size, x_margin, x_stride, y_margin, y_stride,
      inner_batch, inplace);
  struct NS(ComplexTensor) *x = NS(Random_ConjugateEvenTensor_new_)(
      s->tensor_dimension, s->tensor_sizes, s->ce_axis);
  struct NS(RealTensor) *y0 =
      NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
  struct NS(RealTensor) *y1 =
      NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
  if (inplace) {
    NS(ComplexTensor_CopyAsReal)(x->shape, x, y0);
    {
      struct NS(ComplexToRealFunction) *f = RealDft_B_new(s, inplace, mode);
      NS(ComplexToRealFunction_evaluate)
      (f, (Complex *)y0->data, y0->data);
      NS(ComplexToRealFunction_delete)(f);
    }
    NS(ComplexTensor_CopyAsReal)(x->shape, x, y1);
    {
      struct NS(ComplexToRealFunction) *f =
          NS(Fftw_RealDft_B_new)(s, inplace, FLAGS);
      NS(ComplexToRealFunction_evaluate)(f, (Complex *)y1->data, y1->data);
      NS(ComplexToRealFunction_delete)(f);
    }
  } else {
    {
      struct NS(ComplexTensor) *x_ = NS(ComplexTensor_new)(
          s->tensor_dimension, s->out_tensor_storage_sizes);
      NS(ComplexTensor_CopyAs)(x->shape, x, x_);
      struct NS(ComplexToRealFunction) *f = RealDft_B_new(s, inplace, mode);
      NS(ComplexToRealFunction_evaluate)(f, x_->data, y0->data);
      NS(ComplexToRealFunction_delete)(f);
      NS(RealTensor_delete)(x_);
    }
    {
      struct NS(ComplexTensor) *x_ = NS(ComplexTensor_new)(
          s->tensor_dimension, s->out_tensor_storage_sizes);
      NS(ComplexTensor_CopyAs)(x->shape, x, x_);
      struct NS(ComplexToRealFunction) *f =
          NS(Fftw_RealDft_B_new)(s, inplace, FLAGS);
      NS(ComplexToRealFunction_evaluate)(f, x_->data, y1->data);
      NS(ComplexToRealFunction_delete)(f);
      NS(RealTensor_delete)(x_);
    }
  }
  struct RealStats stats =
      RealTensor_Compare(s->tensor_dimension, s->tensor_sizes, y0, y1);

  PrintResultReal(stats);

  NS(ComplexTensor_delete)(x);
  NS(ComplexTensor_delete)(y0);
  NS(ComplexTensor_delete)(y1);
  NS(Wrapper_RealDftSizes_delete)(s);
}

int main(int argc, char **argv) {
  unsigned int seed = 123456U;
  bool inplace = true;
  bool backward = false;
  int batch_size = 1;
  int x_margin = 0;
  int x_stride = 1;
  int y_margin = 0;
  int y_stride = 1;
  int dimension = 0;
  int sizes[MAX_DIM] = {0};
  bool real = false;
  bool inner_batch = false;
  int mode;
  {
    int opt = 0;
    while ((opt = getopt(argc, argv, "s:m:b:x:y:u:v:cntrxvh")) != -1) {
      switch (opt) {
      case 's':
        seed = (unsigned int)atoi(optarg);
        break;
      case 'm':
        mode = atoi(optarg);
        break;
      case 'b':
        batch_size = atoi(optarg);
        break;
      case 'c':
        backward = true;
        break;
      case 'n':
        inplace = false;
        break;
      case 't':
        inner_batch = true;
        break;
      case 'r':
        real = true;
        break;
      case 'x':
        x_margin = atoi(optarg);
        break;
      case 'y':
        y_margin = atoi(optarg);
        break;
      case 'u':
        x_stride = atoi(optarg);
        break;
      case 'v':
        y_stride = atoi(optarg);
        break;
      case 'h':
        printf("Usage: jomfft_compare <size0> <size1> ...\n"
               "  -s <seed>: seed for random number generation.\n"
               "  -b <size>: batch size.\n"
               "  -c: use conjugate (backward) algorithm.\n"
               "  -r: use real algorithm.\n"
               "  -n: use not-in-place algorithm.\n"
               "  -t: use inner (tail) batch.\n"
               "  -x <margin>: margin size of input data.\n"
               "  -y <margin>: margin size of output data.\n"
               "  -u <stride>: stride size of input data.\n"
               "  -v <stride>: stride size of output data.\n"
               "  -h: this message.\n");
        return 0;
      default:
        return -1;
      }
    }
    dimension = MIN(argc - optind, MAX_DIM);
    for (int i = 0; i < dimension; ++i) {
      sizes[i] = atoi(argv[optind + i]);
    }
    if (dimension == 0) {
      fprintf(stderr, "At least one size is required.\n");
      return -1;
    }
  }
  if (real &&
      (x_stride != 1 || y_stride != 1 || (batch_size > 1 && inner_batch))) {
    inplace = false;
  }
  if (inplace) {
    y_margin = x_margin;
    y_stride = x_stride;
  }
  if (batch_size > 1 || x_margin != 0 || x_stride != 1 || y_margin != 0 ||
      y_stride != 1) {
    mode = MAX(mode, 1);
  }
  NS(Random_Init)(seed);
  if (real) {
    if (backward) {
      DoMainReal_B(dimension, sizes, batch_size, x_margin, x_stride, y_margin,
                   y_stride, inner_batch, inplace, mode);
    } else {
      DoMainReal_F(dimension, sizes, batch_size, x_margin, x_stride, y_margin,
                   y_stride, inner_batch, inplace, mode);
    }
  } else {
    DoMainComplex(backward, dimension, sizes, batch_size, x_margin, x_stride,
                  y_margin, y_stride, inner_batch, inplace, mode);
  }
  return 0;
}
