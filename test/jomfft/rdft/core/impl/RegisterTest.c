#include "RegisterTest.h"

#include "StrideBatchDft_B10Test.h"
#include "StrideBatchDft_B12Test.h"
#include "StrideBatchDft_B16Test.h"
#include "StrideBatchDft_B2Test.h"
#include "StrideBatchDft_B32Test.h"
#include "StrideBatchDft_B3Test.h"
#include "StrideBatchDft_B4Test.h"
#include "StrideBatchDft_B5Test.h"
#include "StrideBatchDft_B64Test.h"
#include "StrideBatchDft_B6Test.h"
#include "StrideBatchDft_B7Test.h"
#include "StrideBatchDft_B8Test.h"
#include "StrideBatchDft_B9Test.h"
#include "StrideBatchDft_F10Test.h"
#include "StrideBatchDft_F12Test.h"
#include "StrideBatchDft_F16Test.h"
#include "StrideBatchDft_F2Test.h"
#include "StrideBatchDft_F32Test.h"
#include "StrideBatchDft_F3Test.h"
#include "StrideBatchDft_F4Test.h"
#include "StrideBatchDft_F5Test.h"
#include "StrideBatchDft_F64Test.h"
#include "StrideBatchDft_F6Test.h"
#include "StrideBatchDft_F7Test.h"
#include "StrideBatchDft_F8Test.h"
#include "StrideBatchDft_F9Test.h"

#include "EvenSizeTwiddleDft_BTest.h"
#include "EvenSizeTwiddleDft_FTest.h"

#include "ComplexToPairDft_BTest.h"
#include "ComplexToPairDft_FTest.h"

#define REGISTER_SINGLE_TEST(m, fb, n, v)                                      \
  REGISTER_TEST(RDftCore_##m##_##fb##n##_##v##Test)

#define REGISTER_FB_TEST(m, n, v)                                              \
  do {                                                                         \
    REGISTER_SINGLE_TEST(m, F, n, v);                                          \
    REGISTER_SINGLE_TEST(m, B, n, v);                                          \
  } while (0)

#define REGISTER_MODULE_TEST_1(m, n) REGISTER_FB_TEST(m, n, 1)

#define REGISTER_MODULE_TEST_2(m, n)                                           \
  do {                                                                         \
    REGISTER_FB_TEST(m, n, 1);                                                 \
    REGISTER_FB_TEST(m, n, 2);                                                 \
  } while (0)

#define REGISTER_MODULE_TEST_4(m, n)                                           \
  do {                                                                         \
    REGISTER_FB_TEST(m, n, 1);                                                 \
    REGISTER_FB_TEST(m, n, 2);                                                 \
    REGISTER_FB_TEST(m, n, 4);                                                 \
  } while (0)

#define REGISTER_MODULE_TEST_8(m, n)                                           \
  do {                                                                         \
    REGISTER_FB_TEST(m, n, 1);                                                 \
    REGISTER_FB_TEST(m, n, 2);                                                 \
    REGISTER_FB_TEST(m, n, 8);                                                 \
  } while (0)

#if R_SIMD_SIZE_S == 1
#define REGISTER_MODULE_TEST_S REGISTER_MODULE_TEST_1
#elif R_SIMD_SIZE_S == 2
#define REGISTER_MODULE_TEST_S REGISTER_MODULE_TEST_2
#elif R_SIMD_SIZE_S == 4
#define REGISTER_MODULE_TEST_S REGISTER_MODULE_TEST_4
#elif R_SIMD_SIZE_S == 8
#define REGISTER_MODULE_TEST_S REGISTER_MODULE_TEST_8
#endif

#if C_SIMD_SIZE == 1
#define REGISTER_MODULE_TEST_C REGISTER_MODULE_TEST_1
#elif C_SIMD_SIZE == 2
#define REGISTER_MODULE_TEST_C REGISTER_MODULE_TEST_2
#elif C_SIMD_SIZE == 4
#define REGISTER_MODULE_TEST_C REGISTER_MODULE_TEST_4
#elif C_SIMD_SIZE == 8
#define REGISTER_MODULE_TEST_C REGISTER_MODULE_TEST_8
#endif

#if C_SIMD_SIZE_S == 1
#define REGISTER_MODULE_TEST_C_S REGISTER_MODULE_TEST_1
#elif C_SIMD_SIZE_S == 2
#define REGISTER_MODULE_TEST_C_S REGISTER_MODULE_TEST_2
#elif C_SIMD_SIZE_S == 4
#define REGISTER_MODULE_TEST_C_S REGISTER_MODULE_TEST_4
#endif

void NS(RDftCoreImpl_RegisterTest)() {
  REGISTER_MODULE_TEST_S(StrideBatchDft, 2);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 3);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 4);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 5);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 6);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 7);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 8);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 9);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 10);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 12);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 16);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 32);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 64);

  REGISTER_MODULE_TEST_C(EvenSizeTwiddleDft, );

  REGISTER_MODULE_TEST_C_S(ComplexToPairDft, );
}
