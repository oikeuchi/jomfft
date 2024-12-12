#pragma once

#include <jomfft/math/Math.h>
#include <jomfft/util/ComplexTensor.h>
#include <jomfft/util/ComplexUtil.h>
#include <jomfft/util/RealTensor.h>
#include <jomfft/util/Variadic.h>

#include "Test_.h"

#define STR(x) #x

#define REPORT_ASSERT_SUCCESS NS(Test_ReportSuccess)()

#define REPORT_ASSERT_FAILURE                                                  \
  do {                                                                         \
    NS(Test_PrintCapture)                                                      \
    ();                                                                        \
    printf("%s:%d (%s)\n", __FILE__, __LINE__, __func__);                      \
    NS(Test_ReportFailure)                                                     \
    (true);                                                                    \
    assert(false);                                                             \
  } while (0)

#define ASSERT_TRUE_(expr, ...)                                                \
  do {                                                                         \
    if (!(expr)) {                                                             \
      printf(__VA_ARGS__);                                                     \
      REPORT_ASSERT_FAILURE;                                                   \
    }                                                                          \
  } while (0)

#define ASSERT_TRUE(expr)                                                      \
  ASSERT_TRUE_(expr, "Assertion " STR(expr) " == true failed.")
#define ASSERT_FALSE(expr)                                                     \
  ASSERT_TRUE_(!(expr), "Assertion " STR(expr) " == false failed.")

#define ASSERT_EQ__(type, fmt, a, e)                                           \
  do {                                                                         \
    const type a__ = (type)(a);                                                \
    const type e__ = (type)(e);                                                \
    if (a__ != e__) {                                                          \
      printf("Assertion " STR(a) " == " STR(e) " failed.\n");                  \
      printf("  Actual: " fmt "\n", a__);                                      \
      printf("  Expected: " fmt "\n", e__);                                    \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      REPORT_ASSERT_SUCCESS;                                                   \
    }                                                                          \
  } while (0)

#define ASSERT_INT_EQ(a, e) ASSERT_EQ__(int, "%d", a, e)
#define ASSERT_ENUM_EQ(a, e) ASSERT_EQ__(int, "%d", a, e)
#define ASSERT_LONG_EQ(a, e) ASSERT_EQ__(Long, "%ld", a, e)
#define ASSERT_REAL_EQ(a, e) ASSERT_EQ__(Real, R_FORMAT, a, e)

#define ASSERT_BOOL_EQ(a, e)                                                   \
  do {                                                                         \
    const bool a__ = (bool)(a);                                                \
    const bool e__ = (bool)(e);                                                \
    if (a__ != e__) {                                                          \
      printf("Assertion " STR(a) " == " STR(e) " failed.\n");                  \
      printf("  Actual: %s\n", BOOL_STR(a__));                                 \
      printf("  Expected: %s\n", BOOL_STR(e__));                               \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      REPORT_ASSERT_SUCCESS;                                                   \
    }                                                                          \
  } while (0)

#define ASSERT_REAL_AEQ_(a, e, eps)                                            \
  do {                                                                         \
    Real a__ = (Real)(a);                                                      \
    Real e__ = (Real)(e);                                                      \
    Real eps__ = (Real)(eps);                                                  \
    if (!NS(Real_almostEqual_)(a__, e__, eps__, eps__)) {                      \
      printf("Assertion " STR(a) " ~= " STR(e) " failed.\n");                  \
      printf("  Actual: " R_FORMAT "\n", a__);                                 \
      printf("  Expected: " R_FORMAT "\n", e__);                               \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      REPORT_ASSERT_SUCCESS;                                                   \
    }                                                                          \
  } while (0)

#define ASSERT_REAL_AEQ(a, e) ASSERT_REAL_AEQ_(a, e, R_EPSILON)

#define ASSERT_COMPLEX_EQ(a, e)                                                \
  do {                                                                         \
    Complex a__ = (a);                                                         \
    Complex e__ = (e);                                                         \
    if (!NS(Complex_equal)(a__, e__)) {                                        \
      printf("Assertion " STR(a) " == " STR(e) " failed.\n");                  \
      printf("  Actual: " C_FORMAT "\n", C_REAL(a__), C_IMAG(a__));            \
      printf("  Expected: " C_FORMAT "\n", C_REAL(e__), C_IMAG(e__));          \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      REPORT_ASSERT_SUCCESS;                                                   \
    }                                                                          \
  } while (0)

#define ASSERT_COMPLEX_AEQ_(a, e, eps)                                         \
  do {                                                                         \
    Complex a__ = (a);                                                         \
    Complex e__ = (e);                                                         \
    Real eps__ = (Real)(eps);                                                  \
    if (!NS(Complex_almostEqual_)(a__, e__, eps__, eps__)) {                   \
      printf("Assertion " STR(a) " ~= " STR(e) " failed.\n");                  \
      printf("  Actual: " C_FORMAT "\n", C_REAL(a__), C_IMAG(a__));            \
      printf("  Expected: " R_FORMAT "\n", C_REAL(e__), C_IMAG(e__));          \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      REPORT_ASSERT_SUCCESS;                                                   \
    }                                                                          \
  } while (0)

#define ASSERT_COMPLEX_AEQ(a, e) ASSERT_COMPLEX_AEQ_(a, e, R_EPSILON)

#define ASSERT_REALTENSOR_EQ(a, e)                                             \
  do {                                                                         \
    const struct NS(RealTensor) *a__ = (a);                                    \
    const struct NS(RealTensor) *e__ = (e);                                    \
    assert(a__);                                                               \
    assert(e__);                                                               \
    if (!NS(TensorShape_equal)(a__->shape, e__->shape)) {                      \
      printf("Assertion " STR(a) " == " STR(e) " failed.\n");                  \
      printf("  Actual->shape: ");                                             \
      NS(TensorShape_print)                                                    \
      (a__->shape, stdout);                                                    \
      printf("\n");                                                            \
      printf("  Expected->shape: ");                                           \
      NS(TensorShape_print)                                                    \
      (e__->shape, stdout);                                                    \
      printf("\n");                                                            \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      if (e__->shape->dimension >= 1) {                                        \
        Long num_of_errors__ = 0;                                              \
        Long *indexes__ = ARRAY_NEW(Long, e__->shape->dimension);              \
        if (!NS(RealTensor_equal_)(a__, e__, &num_of_errors__, indexes__)) {   \
          printf("Assertion " STR(a) " == " STR(e) " failed.");                \
          printf(" [%ld/%ld]\n", num_of_errors__,                              \
                 NS(TensorShape_dataSize)(a__->shape));                        \
          printf("  Position: ");                                              \
          NS(TensorShape_printIndexes)                                         \
          (e__->shape, stdout, indexes__);                                     \
          printf("\n");                                                        \
          printf("  Actual: " R_FORMAT "\n",                                   \
                 NS(RealTensor_get)(a__, indexes__));                          \
          printf("  Expected: " R_FORMAT "\n",                                 \
                 NS(RealTensor_get)(e__, indexes__));                          \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_FAILURE;                                               \
        } else {                                                               \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_SUCCESS;                                               \
        }                                                                      \
      } else {                                                                 \
        REPORT_ASSERT_SUCCESS;                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

#define ASSERT_REALTENSOR_AEQ_(a, e, eps)                                      \
  do {                                                                         \
    const struct NS(RealTensor) *a__ = (a);                                    \
    const struct NS(RealTensor) *e__ = (e);                                    \
    Real eps__ = (Real)(eps);                                                  \
    assert(a__);                                                               \
    assert(e__);                                                               \
    if (!NS(TensorShape_equal)(a__->shape, e__->shape)) {                      \
      printf("Assertion " STR(a) " == " STR(e) " failed.\n");                  \
      printf("  Actual->shape: ");                                             \
      NS(TensorShape_print)                                                    \
      (a__->shape, stdout);                                                    \
      printf("\n");                                                            \
      printf("  Expected->shape: ");                                           \
      NS(TensorShape_print)                                                    \
      (e__->shape, stdout);                                                    \
      printf("\n");                                                            \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      if (e__->shape->dimension >= 1) {                                        \
        Long num_of_errors__ = 0;                                              \
        Long *indexes__ = ARRAY_NEW(Long, e__->shape->dimension);              \
        if (!NS(RealTensor_almostEqual_)(a__, e__, eps__, eps__,               \
                                         &num_of_errors__, indexes__)) {       \
          printf("Assertion " STR(a) " ~= " STR(e) " failed.");                \
          printf(" [%ld/%ld]\n", num_of_errors__,                              \
                 NS(TensorShape_dataSize)(a__->shape));                        \
          printf("  Position: ");                                              \
          NS(TensorShape_printIndexes)                                         \
          (e__->shape, stdout, indexes__);                                     \
          printf("\n");                                                        \
          printf("  Actual: " R_FORMAT "\n",                                   \
                 NS(RealTensor_get)(a__, indexes__));                          \
          printf("  Expected: " R_FORMAT "\n",                                 \
                 NS(RealTensor_get)(e__, indexes__));                          \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_FAILURE;                                               \
        } else {                                                               \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_SUCCESS;                                               \
        }                                                                      \
      } else {                                                                 \
        REPORT_ASSERT_SUCCESS;                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

#define ASSERT_REALTENSOR_AEQ(a, e) ASSERT_REALTENSOR_AEQ_(a, e, R_EPSILON)

#define ASSERT_COMPLEXTENSOR_EQ(a, e)                                          \
  do {                                                                         \
    const struct NS(ComplexTensor) *a__ = (a);                                 \
    const struct NS(ComplexTensor) *e__ = (e);                                 \
    assert(a__);                                                               \
    assert(e__);                                                               \
    if (!NS(TensorShape_equal)(a__->shape, e__->shape)) {                      \
      printf("Assertion " STR(a) " == " STR(e) " failed.\n");                  \
      printf("  Actual->shape: ");                                             \
      NS(TensorShape_print)                                                    \
      (a__->shape, stdout);                                                    \
      printf("\n");                                                            \
      printf("  Expected->shape: ");                                           \
      NS(TensorShape_print)                                                    \
      (e__->shape, stdout);                                                    \
      printf("\n");                                                            \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      if (e__->shape->dimension >= 1) {                                        \
        Long num_of_errors__ = 0;                                              \
        Long *indexes__ = ARRAY_NEW(Long, e__->shape->dimension);              \
        if (!NS(ComplexTensor_equal_)(a__, e__, &num_of_errors__,              \
                                      indexes__)) {                            \
          printf("Assertion " STR(a) " == " STR(e) " failed.");                \
          printf(" [%ld/%ld]\n", num_of_errors__,                              \
                 NS(TensorShape_dataSize)(a__->shape));                        \
          printf("  Position: ");                                              \
          NS(TensorShape_printIndexes)                                         \
          (e__->shape, stdout, indexes__);                                     \
          printf("\n");                                                        \
          Complex a0__ = NS(ComplexTensor_get)(a__, indexes__);                \
          Complex e0__ = NS(ComplexTensor_get)(e__, indexes__);                \
          printf("  Actual: " C_FORMAT "\n", C_REAL(a0__), C_IMAG(a0__));      \
          printf("  Expected: " C_FORMAT "\n", C_REAL(e0__), C_IMAG(e0__));    \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_FAILURE;                                               \
        } else {                                                               \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_SUCCESS;                                               \
        }                                                                      \
      } else {                                                                 \
        REPORT_ASSERT_SUCCESS;                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

#define ASSERT_COMPLEXTENSOR_AEQ_(a, e, eps)                                   \
  do {                                                                         \
    const struct NS(ComplexTensor) *a__ = (a);                                 \
    const struct NS(ComplexTensor) *e__ = (e);                                 \
    Real eps__ = (Real)(eps);                                                  \
    assert(a__);                                                               \
    assert(e__);                                                               \
    if (!NS(TensorShape_equal)(a__->shape, e__->shape)) {                      \
      printf("Assertion " STR(a) " == " STR(e) " failed.\n");                  \
      printf("  Actual->shape: ");                                             \
      NS(TensorShape_print)                                                    \
      (a__->shape, stdout);                                                    \
      printf("\n");                                                            \
      printf("  Expected->shape: ");                                           \
      NS(TensorShape_print)                                                    \
      (e__->shape, stdout);                                                    \
      printf("\n");                                                            \
      REPORT_ASSERT_FAILURE;                                                   \
    } else {                                                                   \
      if (e__->shape->dimension >= 1) {                                        \
        Long num_of_errors__ = 0;                                              \
        Long *indexes__ = ARRAY_NEW(Long, e__->shape->dimension);              \
        if (!NS(ComplexTensor_almostEqual_)(a__, e__, eps__, eps__,            \
                                            &num_of_errors__, indexes__)) {    \
          printf("Assertion " STR(a) " ~= " STR(e) " failed.");                \
          printf(" [%ld/%ld]\n", num_of_errors__,                              \
                 NS(TensorShape_dataSize)(a__->shape));                        \
          printf("  Position: ");                                              \
          NS(TensorShape_printIndexes)                                         \
          (e__->shape, stdout, indexes__);                                     \
          printf("\n");                                                        \
          Complex a0__ = NS(ComplexTensor_get)(a__, indexes__);                \
          Complex e0__ = NS(ComplexTensor_get)(e__, indexes__);                \
          printf("  Actual: " C_FORMAT "\n", C_REAL(a0__), C_IMAG(a0__));      \
          printf("  Expected: " C_FORMAT "\n", C_REAL(e0__), C_IMAG(e0__));    \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_FAILURE;                                               \
        } else {                                                               \
          ARRAY_DELETE(indexes__);                                             \
          REPORT_ASSERT_SUCCESS;                                               \
        }                                                                      \
      } else {                                                                 \
        REPORT_ASSERT_SUCCESS;                                                 \
      }                                                                        \
    }                                                                          \
  } while (0)

#define ASSERT_COMPLEXTENSOR_AEQ(a, e)                                         \
  ASSERT_COMPLEXTENSOR_AEQ_(a, e, R_EPSILON)

#define REGISTER_TEST(func) NS(Test_Register)(#func, NS(func))

#define CAPTURE_INT(x)                                                         \
  NS(Test_Capture)                                                             \
  (#x, __FILE__, __LINE__, NS(Variadic_newInt)(x))

#define CAPTURE_LONG(x)                                                        \
  NS(Test_Capture)                                                             \
  (#x, __FILE__, __LINE__, NS(Variadic_newLong)(x))

#define CAPTURE_BOOL(x)                                                        \
  NS(Test_Capture)                                                             \
  (#x, __FILE__, __LINE__, NS(Variadic_newBool)(x))

#define CAPTURE_INTARRAY(sz, x)                                                \
  NS(Test_Capture)                                                             \
  (#x, __FILE__, __LINE__, NS(Variadic_newIntArray)((sz), (x)))

void NS(Test_Begin)(unsigned int seed);

int NS(Test_End)();

void NS(Test_Clear)();

void NS(Test_Register)(const char *name, NS(Test_fn) func);

void NS(Test_Run)(const char *filter, bool use_regex);

void NS(Test_ReportSuccess)();

void NS(Test_ReportFailure)(bool print_error);

void NS(Test_Capture)(const char *name, const char *file, int lineno,
                      struct NS(Variadic) *value);

void NS(Test_ClearCapture)();

void NS(Test_PrintCapture)();
