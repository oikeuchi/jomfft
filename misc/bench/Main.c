#include <time.h>

#include <jomfft/math/IntegerMath.h>
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

#if USE_FFTW
#include <jomfft/fftw/ComplexDft.h>
#include <jomfft/fftw/Plan.h>
#include <jomfft/fftw/RealDft.h>
#endif

#if USE_MKL
#include <jomfft/mkl/ComplexDft.h>
#include <jomfft/mkl/RealDft.h>
#endif

#define MAX_DIM 8

typedef enum { MODEL_JOMFFT, MODEL_FFTW, MODEL_MKL } Model_t;

struct BenchmarkResult {
  double seconds;
  double score;
  double abs_sum;
};

static double ClockDiffInSeconds(struct timespec start, struct timespec end) {
  return (double)(end.tv_sec - start.tv_sec) +
         (double)(end.tv_nsec - start.tv_nsec) * 1.e-9;
}

static double FftCost(int iterations, int dimension, const int *dft_sizes,
                      int batch_size) {
  double cube_size = 1;
  for (int i = 0; i < dimension; ++i) {
    cube_size *= dft_sizes[i];
  }
  double out = 0;
  for (int i = 0; i < dimension; ++i) {
    out += cube_size * log((double)dft_sizes[i]);
  }
  out *= iterations * batch_size;
  return out;
}

static struct BenchmarkResult
DoMainComplex(Model_t model_type, bool backward, int iterations, int dimension,
              const int *dft_sizes, int batch_size, bool inner_batch,
              bool inplace, unsigned flags, bool verbose) {
  (void)flags;
  (void)verbose;
  struct NS(Wrapper_ComplexDftSizes) *s = NS(Wrapper_ComplexDftSizes_new)(
      dimension, dft_sizes, batch_size, 0, 1, 0, 1, inner_batch);
  struct NS(ComplexTensor) *x =
      NS(Random_ComplexTensor_new)(s->tensor_dimension, s->tensor_sizes);
  struct NS(ComplexFunction) *func = NULL;
  switch (model_type) {
  case MODEL_JOMFFT:
    func = NS(Wrapper_ComplexDft_new_1)(backward, s, inplace);
    break;
#if USE_FFTW
  case MODEL_FFTW:
    func = NS(Fftw_ComplexDft_new)(backward, s, inplace, flags);
    break;
#endif
#if USE_MKL
  case MODEL_MKL:
    func = NS(Mkl_ComplexDft_new)(backward, s, inplace);
    break;
#endif
  default:
    fprintf(stderr, "Unsupported model type. '%d'", (int)model_type);
    abort();
  }
  double seconds = 0;
  double abs_sum = 0;
  if (inplace) {
    for (int i = 0; i < iterations; ++i) {
      struct NS(ComplexTensor) *y = NS(ComplexTensor_new)(
          s->tensor_dimension, s->out_tensor_storage_sizes);
      NS(ComplexTensor_CopyAs)(x->shape, x, y);
      struct timespec start;
      struct timespec end;
      clock_gettime(CLOCK_REALTIME, &start);
      NS(ComplexFunction_evaluate)(func, y->data, y->data);
      clock_gettime(CLOCK_REALTIME, &end);
      seconds += ClockDiffInSeconds(start, end);
      if (i == 0) {
        abs_sum = NS(ComplexTensor_absSum)(y);
      } else {
        abs_sum += R_ABS(C_REAL(y->data[0]));
      }
      NS(ComplexTensor_delete)(y);
    }
  } else {
    for (int i = 0; i < iterations; ++i) {
      struct NS(ComplexTensor) *x0 = NS(ComplexTensor_new)(
          s->tensor_dimension, s->in_tensor_storage_sizes);
      NS(ComplexTensor_CopyAs)(x->shape, x, x0);
      struct NS(ComplexTensor) *y = NS(ComplexTensor_new)(
          s->tensor_dimension, s->out_tensor_storage_sizes);
      struct timespec start;
      struct timespec end;
      clock_gettime(CLOCK_REALTIME, &start);
      NS(ComplexFunction_evaluate)(func, x0->data, y->data);
      clock_gettime(CLOCK_REALTIME, &end);
      seconds += ClockDiffInSeconds(start, end);
      if (i == 0) {
        abs_sum = NS(ComplexTensor_absSum)(y);
      } else {
        abs_sum += R_ABS(C_REAL(y->data[0]));
      }
      NS(ComplexTensor_delete)(x0);
      NS(ComplexTensor_delete)(y);
    }
  }
  NS(ComplexTensor_delete)(x);
#if USE_FFTW
  if (verbose) {
    if (model_type == MODEL_FFTW) {
      NS(Fftw_Plan_print)
      ((FFTW_(plan) *)NS(ComplexFunction_data)(func));
    }
  }
#endif
  NS(ComplexFunction_delete)(func);
  NS(Wrapper_ComplexDftSizes_delete)(s);
  const double cost = FftCost(iterations, dimension, dft_sizes, batch_size);
  struct BenchmarkResult result = {
      .seconds = seconds, .score = cost / seconds, .abs_sum = abs_sum};
  return result;
}

static struct BenchmarkResult DoMainReal_F(Model_t model_type, int iterations,
                                           int dimension, const int *dft_sizes,
                                           int batch_size, bool inner_batch,
                                           bool inplace, unsigned flags,
                                           bool verbose) {
  (void)flags;
  (void)verbose;
  struct NS(Wrapper_RealDftSizes) *s = NS(Wrapper_RealDftSizes_new)(
      dimension, dft_sizes, batch_size, 0, 1, 0, 1, inner_batch, inplace);
  struct NS(RealTensor) *x = NS(Random_RealTensor_new)(
      s->tensor_dimension, s->in_tensor_storage_sizes);
  struct NS(RealToComplexFunction) *func = NULL;
  switch (model_type) {
  case MODEL_JOMFFT:
    func = NS(Wrapper_RealDft_F_new_1)(s, inplace);
    break;
#if USE_FFTW
  case MODEL_FFTW:
    func = NS(Fftw_RealDft_F_new)(s, inplace, flags);
    break;
#endif
#if USE_MKL
  case MODEL_MKL:
    func = NS(Mkl_RealDft_F_new)(s, inplace);
    break;
#endif
  default:
    fprintf(stderr, "Unsupported model type. '%d'", (int)model_type);
    abort();
  }
  double seconds = 0;
  double abs_sum = 0;
  if (inplace) {
    for (int i = 0; i < iterations; ++i) {
      struct NS(ComplexTensor) *y = NS(ComplexTensor_new)(
          s->tensor_dimension, s->out_tensor_storage_sizes);
      NS(RealTensor_CopyAsComplex)(x->shape, x, y);
      struct timespec start;
      struct timespec end;
      clock_gettime(CLOCK_REALTIME, &start);
      NS(RealToComplexFunction_evaluate)(func, (Real *)y->data, y->data);
      clock_gettime(CLOCK_REALTIME, &end);
      seconds += ClockDiffInSeconds(start, end);
      if (i == 0) {
        abs_sum = NS(ComplexTensor_absSum)(y);
      } else {
        abs_sum += R_ABS(C_REAL(y->data[0]));
      }
      NS(ComplexTensor_delete)(y);
    }
  } else {
    for (int i = 0; i < iterations; ++i) {
      struct NS(RealTensor) *x0 =
          NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
      NS(RealTensor_CopyAs)(x->shape, x, x0);
      struct NS(ComplexTensor) *y = NS(ComplexTensor_new)(
          s->tensor_dimension, s->out_tensor_storage_sizes);
      struct timespec start;
      struct timespec end;
      clock_gettime(CLOCK_REALTIME, &start);
      NS(RealToComplexFunction_evaluate)(func, x0->data, y->data);
      clock_gettime(CLOCK_REALTIME, &end);
      seconds += ClockDiffInSeconds(start, end);
      if (i == 0) {
        abs_sum = NS(ComplexTensor_absSum)(y);
      } else {
        abs_sum += R_ABS(C_REAL(y->data[0]));
      }
      NS(RealTensor_delete)(x0);
      NS(ComplexTensor_delete)(y);
    }
  }
  NS(RealTensor_delete)(x);
#if USE_FFTW
  if (verbose) {
    if (model_type == MODEL_FFTW) {
      NS(Fftw_Plan_print)
      ((FFTW_(plan) *)NS(RealToComplexFunction_data)(func));
    }
  }
#endif
  NS(RealToComplexFunction_delete)(func);
  NS(Wrapper_RealDftSizes_delete)(s);
  const double cost = FftCost(iterations, dimension, dft_sizes, batch_size);
  struct BenchmarkResult result = {
      .seconds = seconds, .score = cost / seconds, .abs_sum = abs_sum};
  return result;
}

static struct BenchmarkResult DoMainReal_B(Model_t model_type, int iterations,
                                           int dimension, const int *dft_sizes,
                                           int batch_size, bool inner_batch,
                                           bool inplace, unsigned flags,
                                           bool verbose) {
  (void)flags;
  (void)verbose;
  struct NS(Wrapper_RealDftSizes) *s = NS(Wrapper_RealDftSizes_new)(
      dimension, dft_sizes, batch_size, 0, 1, 0, 1, inner_batch, inplace);
  struct NS(ComplexTensor) *x = NS(Random_ConjugateEvenTensor_new_)(
      s->tensor_dimension, s->tensor_sizes, s->ce_axis);
  struct NS(ComplexToRealFunction) *func = NULL;
  switch (model_type) {
  case MODEL_JOMFFT:
    func = NS(Wrapper_RealDft_B_new_1)(s, inplace);
    break;
#if USE_FFTW
  case MODEL_FFTW:
    func = NS(Fftw_RealDft_B_new)(s, inplace, flags);
    break;
#endif
#if USE_MKL
  case MODEL_MKL:
    func = NS(Mkl_RealDft_B_new)(s, inplace);
    break;
#endif
  default:
    fprintf(stderr, "Unsupported model type. '%d'", (int)model_type);
    abort();
  }
  double seconds = 0;
  double abs_sum = 0;
  if (inplace) {
    for (int i = 0; i < iterations; ++i) {
      struct NS(RealTensor) *y =
          NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
      NS(ComplexTensor_CopyAsReal)(x->shape, x, y);
      struct timespec start;
      struct timespec end;
      clock_gettime(CLOCK_REALTIME, &start);
      NS(ComplexToRealFunction_evaluate)
      (func, (Complex *)y->data, y->data);
      clock_gettime(CLOCK_REALTIME, &end);
      seconds += ClockDiffInSeconds(start, end);
      if (i == 0) {
        abs_sum = NS(RealTensor_absSum)(y);
      } else {
        abs_sum += R_ABS(y->data[0]);
      }
      NS(RealTensor_delete)(y);
    }
  } else {
    for (int i = 0; i < iterations; ++i) {
      struct NS(ComplexTensor) *x0 = NS(ComplexTensor_new)(
          s->tensor_dimension, s->out_tensor_storage_sizes);
      NS(ComplexTensor_CopyAs)(x->shape, x, x0);
      struct NS(RealTensor) *y =
          NS(RealTensor_new)(s->tensor_dimension, s->in_tensor_storage_sizes);
      struct timespec start;
      struct timespec end;
      clock_gettime(CLOCK_REALTIME, &start);
      NS(ComplexToRealFunction_evaluate)(func, x0->data, y->data);
      clock_gettime(CLOCK_REALTIME, &end);
      seconds += ClockDiffInSeconds(start, end);
      if (i == 0) {
        abs_sum = NS(RealTensor_absSum)(y);
      } else {
        abs_sum += R_ABS(y->data[0]);
      }
      NS(ComplexTensor_delete)(x0);
      NS(RealTensor_delete)(y);
    }
  }
  NS(ComplexTensor_delete)(x);
#if USE_FFTW
  if (verbose) {
    if (model_type == MODEL_FFTW) {
      NS(Fftw_Plan_print)
      ((FFTW_(plan) *)NS(ComplexToRealFunction_data)(func));
    }
  }
#endif
  NS(ComplexToRealFunction_delete)(func);
  NS(Wrapper_RealDftSizes_delete)(s);
  const double cost = FftCost(iterations, dimension, dft_sizes, batch_size);
  struct BenchmarkResult result = {
      .seconds = seconds, .score = cost / seconds, .abs_sum = abs_sum};
  return result;
}

int main(int argc, char **argv) {
  Model_t model_type = MODEL_JOMFFT;
  bool inplace = true;
  bool backward = false;
  unsigned flags = 0;
  int iterations = 1;
  int batch_size = 1;
  int dimension = 0;
  int dft_sizes[MAX_DIM] = {0};
#if USE_FFTW
  bool export_to_file = false;
  const char *file_path = "wisdom.dat";
#endif
  bool real = false;
  bool inner_batch = false;
  bool verbose = false;
  {
    int opt = 0;
    while ((opt = getopt(argc, argv, "m:l:i:b:f:cntrxvh")) != -1) {
      switch (opt) {
      case 'm': {
        int model = atoi(optarg);
        switch (model) {
        case 0:
          model_type = MODEL_JOMFFT;
          break;
#if USE_FFTW
        case 1:
          model_type = MODEL_FFTW;
          break;
#endif
#if USE_MKL
        case 2:
          model_type = MODEL_MKL;
          break;
#endif
        default:
          fprintf(stderr, "Unsupported module. value=%d.\n", model);
          return -1;
        }
      } break;
      case 'l':
#if USE_FFTW
        switch (atoi(optarg)) {
        case 1:
          flags = FFTW_ESTIMATE;
          break;
        case 3:
          flags = FFTW_PATIENT;
          break;
        case 4:
          flags = FFTW_WISDOM_ONLY;
          break;
        case 0:
        case 2:
        default:
          flags = FFTW_MEASURE;
          break;
        }
#else
        fprintf(stderr, "Warning: The option '-l' is only for FFTW.\n");
#endif
        break;
      case 'i':
        iterations = atoi(optarg);
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
#if USE_FFTW
      case 'f':
        file_path = optarg;
        break;
      case 'x':
        export_to_file = true;
        break;
      case 'v':
        verbose = true;
        break;
#endif
      case 'h':
        printf("Usage: jomfft_bench <size0> <size1> ...\n"
               "  -m <module_id>: module (0:jomfft, 1:fftw, 2:mkl).\n"
               "  -l <level>: fftw optimization level (0:wisdom_only, "
               "1:estimate, 2:measure, 3:patient, 4:exhaustive).\n"
               "  -i <size>: number of iterations.\n"
               "  -b <size>: batch size.\n"
               "  -c: use conjugate (backward) algorithm.\n"
               "  -r: use real algorithm.\n"
               "  -n: use not-in-place algorithm.\n"
               "  -t: use inner (tail) batch.\n"
               "  -f: fftw wisdom file path.\n"
               "  -x: export fftw wisdom to file.\n"
               "  -v: print fftw model.\n"
               "  -h: this message.\n");
        return 0;
      default:
        return -1;
      }
    }
#if USE_FFTW
    if (model_type == MODEL_FFTW &&
        (flags == FFTW_WISDOM_ONLY || export_to_file) && !file_path) {
      fprintf(stderr, "-f <file path> is required.\n");
      return -1;
    }
#endif
    dimension = MIN(argc - optind, MAX_DIM);
    for (int i = 0; i < dimension; ++i) {
      dft_sizes[i] = (Long)atoi(argv[optind + i]);
    }
    if (dimension == 0) {
      fprintf(stderr, "At least one size is required.\n");
      return -1;
    }
  }
  NS(Random_Init)(123456U);
#if USE_FFTW
#if USE_OPENMP
  FFTW_(init_threads)();
  FFTW_(plan_with_nthreads)(omp_get_max_threads());
#endif
  if (model_type == MODEL_FFTW && flags == FFTW_WISDOM_ONLY) {
    if (!FFTW_(import_wisdom_from_filename)(file_path)) {
      fprintf(stderr, "import wisdom failed. file=%s\n", file_path);
      abort();
    }
  }
#endif
  struct BenchmarkResult result;
  if (real) {
    if (backward) {
      result = DoMainReal_B(model_type, iterations, dimension, dft_sizes,
                            batch_size, inner_batch, inplace, flags, verbose);
    } else {
      result = DoMainReal_F(model_type, iterations, dimension, dft_sizes,
                            batch_size, inner_batch, inplace, flags, verbose);
    }
  } else {
    result =
        DoMainComplex(model_type, backward, iterations, dimension, dft_sizes,
                      batch_size, inner_batch, inplace, flags, verbose);
  }
#if USE_FFTW
  if (export_to_file) {
    if (!FFTW_(export_wisdom_to_filename)(file_path)) {
      fprintf(stderr, "export wisdom failed. file=%s\n", file_path);
      abort();
    }
  }
#endif
  printf("Elapsed: %.4e seconds, Score: %.7e, Abs sum: %.7e.\n", result.seconds,
         result.score, result.abs_sum);
  return 0;
}
