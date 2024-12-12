#include "RegisterTest.h"

#include "InnerBatchDft_B0Test.h"
#include "InnerBatchDft_B1Test.h"
#include "InnerBatchDft_F0Test.h"
#include "InnerBatchDft_F1Test.h"
#include "StrideBatchDft_B0Test.h"
#include "StrideBatchDft_B1Test.h"
#include "StrideBatchDft_F0Test.h"
#include "StrideBatchDft_F1Test.h"

#define REGISTER_SINGLE_TEST(m, fb, n, v)                                      \
  REGISTER_TEST(CDftSingleton_##m##_##fb##n##_##v##Test)

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

#if C_SIMD_SIZE == 1
#define REGISTER_MODULE_TEST REGISTER_MODULE_TEST_1
#elif C_SIMD_SIZE == 2
#define REGISTER_MODULE_TEST REGISTER_MODULE_TEST_2
#elif C_SIMD_SIZE == 4
#define REGISTER_MODULE_TEST REGISTER_MODULE_TEST_4
#elif C_SIMD_SIZE == 8
#define REGISTER_MODULE_TEST REGISTER_MODULE_TEST_4
#endif

#if C_SIMD_SIZE_S == 1
#define REGISTER_MODULE_TEST_S REGISTER_MODULE_TEST_1
#elif C_SIMD_SIZE_S == 2
#define REGISTER_MODULE_TEST_S REGISTER_MODULE_TEST_2
#elif C_SIMD_SIZE_S == 4
#define REGISTER_MODULE_TEST_S REGISTER_MODULE_TEST_4
#endif

void NS(CDftSingletonImpl_RegisterTest)() {
  REGISTER_MODULE_TEST(InnerBatchDft, 0);
  REGISTER_MODULE_TEST(InnerBatchDft, 1);

  REGISTER_MODULE_TEST_S(StrideBatchDft, 0);
  REGISTER_MODULE_TEST_S(StrideBatchDft, 1);
}
