#include "RegisterTest.h"

#include "BatchMultiply_BTest.h"
#include "BatchMultiply_FTest.h"
#include "InnerBatchDft_B10Test.h"
#include "InnerBatchDft_B128Test.h"
#include "InnerBatchDft_B12Test.h"
#include "InnerBatchDft_B16Test.h"
#include "InnerBatchDft_B2Test.h"
#include "InnerBatchDft_B32Test.h"
#include "InnerBatchDft_B3Test.h"
#include "InnerBatchDft_B4Test.h"
#include "InnerBatchDft_B5Test.h"
#include "InnerBatchDft_B64Test.h"
#include "InnerBatchDft_B6Test.h"
#include "InnerBatchDft_B7Test.h"
#include "InnerBatchDft_B8Test.h"
#include "InnerBatchDft_B9Test.h"
#include "InnerBatchDft_F10Test.h"
#include "InnerBatchDft_F128Test.h"
#include "InnerBatchDft_F12Test.h"
#include "InnerBatchDft_F16Test.h"
#include "InnerBatchDft_F2Test.h"
#include "InnerBatchDft_F32Test.h"
#include "InnerBatchDft_F3Test.h"
#include "InnerBatchDft_F4Test.h"
#include "InnerBatchDft_F5Test.h"
#include "InnerBatchDft_F64Test.h"
#include "InnerBatchDft_F6Test.h"
#include "InnerBatchDft_F7Test.h"
#include "InnerBatchDft_F8Test.h"
#include "InnerBatchDft_F9Test.h"
#include "PairSquareTwiddleDft_B16Test.h"
#include "PairSquareTwiddleDft_B32Test.h"
#include "PairSquareTwiddleDft_B8Test.h"
#include "PairSquareTwiddleDft_F16Test.h"
#include "PairSquareTwiddleDft_F32Test.h"
#include "PairSquareTwiddleDft_F8Test.h"
#include "SquareSplitTwiddleDft_B2Test.h"
#include "SquareSplitTwiddleDft_B4Test.h"
#include "SquareSplitTwiddleDft_B8Test.h"
#include "SquareSplitTwiddleDft_F2Test.h"
#include "SquareSplitTwiddleDft_F4Test.h"
#include "SquareSplitTwiddleDft_F8Test.h"
#include "StrideBatchDft_B10Test.h"
#include "StrideBatchDft_B128Test.h"
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
#include "StrideBatchDft_F128Test.h"
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
#include "TransposeDftTwiddle_B2Test.h"
#include "TransposeDftTwiddle_B4Test.h"
#include "TransposeDftTwiddle_B8Test.h"
#include "TransposeDftTwiddle_F2Test.h"
#include "TransposeDftTwiddle_F4Test.h"
#include "TransposeDftTwiddle_F8Test.h"
#include "TwiddleDft_B10Test.h"
#include "TwiddleDft_B12Test.h"
#include "TwiddleDft_B16Test.h"
#include "TwiddleDft_B2Test.h"
#include "TwiddleDft_B32Test.h"
#include "TwiddleDft_B3Test.h"
#include "TwiddleDft_B4Test.h"
#include "TwiddleDft_B5Test.h"
#include "TwiddleDft_B64Test.h"
#include "TwiddleDft_B6Test.h"
#include "TwiddleDft_B7Test.h"
#include "TwiddleDft_B8Test.h"
#include "TwiddleDft_B9Test.h"
#include "TwiddleDft_F10Test.h"
#include "TwiddleDft_F12Test.h"
#include "TwiddleDft_F16Test.h"
#include "TwiddleDft_F2Test.h"
#include "TwiddleDft_F32Test.h"
#include "TwiddleDft_F3Test.h"
#include "TwiddleDft_F4Test.h"
#include "TwiddleDft_F5Test.h"
#include "TwiddleDft_F64Test.h"
#include "TwiddleDft_F6Test.h"
#include "TwiddleDft_F7Test.h"
#include "TwiddleDft_F8Test.h"
#include "TwiddleDft_F9Test.h"

#define REGISTER_SINGLE_TEST(m, fb, n, v)                                      \
  REGISTER_TEST(CDftCore_##m##_##fb##n##_##v##Test)

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

#define REGISTER_SINGLE_TEST_1(m, fb, n)                                       \
  REGISTER_TEST(CDftCore_##m##_##fb##n##Test)

#define REGISTER_FB_TEST_1(m, n)                                               \
  do {                                                                         \
    REGISTER_SINGLE_TEST_1(m, F, n);                                           \
    REGISTER_SINGLE_TEST_1(m, B, n);                                           \
  } while (0)

#define REGISTER_SQUARE_TEST REGISTER_FB_TEST_1

void NS(CDftCoreImpl_RegisterTest)() {
  REGISTER_MODULE_TEST(InnerBatchDft, 2);
  REGISTER_MODULE_TEST(InnerBatchDft, 3);
  REGISTER_MODULE_TEST(InnerBatchDft, 4);
  REGISTER_MODULE_TEST(InnerBatchDft, 5);
  REGISTER_MODULE_TEST(InnerBatchDft, 6);
  REGISTER_MODULE_TEST(InnerBatchDft, 7);
  REGISTER_MODULE_TEST(InnerBatchDft, 8);
  REGISTER_MODULE_TEST(InnerBatchDft, 9);
  REGISTER_MODULE_TEST(InnerBatchDft, 10);
  REGISTER_MODULE_TEST(InnerBatchDft, 12);
  REGISTER_MODULE_TEST(InnerBatchDft, 16);
  REGISTER_MODULE_TEST(InnerBatchDft, 32);
  REGISTER_MODULE_TEST(InnerBatchDft, 64);
  REGISTER_MODULE_TEST(InnerBatchDft, 128);

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
  REGISTER_MODULE_TEST_S(StrideBatchDft, 128);

  REGISTER_MODULE_TEST(TwiddleDft, 2);
  REGISTER_MODULE_TEST(TwiddleDft, 3);
  REGISTER_MODULE_TEST(TwiddleDft, 4);
  REGISTER_MODULE_TEST(TwiddleDft, 5);
  REGISTER_MODULE_TEST(TwiddleDft, 6);
  REGISTER_MODULE_TEST(TwiddleDft, 7);
  REGISTER_MODULE_TEST(TwiddleDft, 8);
  REGISTER_MODULE_TEST(TwiddleDft, 9);
  REGISTER_MODULE_TEST(TwiddleDft, 10);
  REGISTER_MODULE_TEST(TwiddleDft, 12);
  REGISTER_MODULE_TEST(TwiddleDft, 16);
  REGISTER_MODULE_TEST(TwiddleDft, 32);
  REGISTER_MODULE_TEST(TwiddleDft, 64);

  REGISTER_SQUARE_TEST(PairSquareTwiddleDft, 8);
  REGISTER_SQUARE_TEST(PairSquareTwiddleDft, 16);
  REGISTER_SQUARE_TEST(PairSquareTwiddleDft, 32);

  REGISTER_SQUARE_TEST(TransposeDftTwiddle, 2);
  REGISTER_SQUARE_TEST(TransposeDftTwiddle, 4);
  REGISTER_SQUARE_TEST(TransposeDftTwiddle, 8);
#if C_SIMD_SIZE >= 4
  REGISTER_SQUARE_TEST(TransposeDftTwiddle, 2_4);
#endif

  REGISTER_SQUARE_TEST(SquareSplitTwiddleDft, 2);
  REGISTER_SQUARE_TEST(SquareSplitTwiddleDft, 4);
  REGISTER_SQUARE_TEST(SquareSplitTwiddleDft, 8);
#if C_SIMD_SIZE >= 4
  REGISTER_SQUARE_TEST(SquareSplitTwiddleDft, 2_4);
#endif

  REGISTER_MODULE_TEST_S(BatchMultiply, );
}