#pragma once

#define NEW(type) (type *)malloc(sizeof(type))
#define DELETE(p) free(p)

#define ALIGNED_SIZE_(sz, align) (align * (((sz) + align - 1) / align))
#define ALIGNED_SIZE(sz) (size_t) ALIGNED_SIZE_((sz), ALIGNMENT)

#ifdef _MSC_VER
#define ARRAY_NEW(type, sz)                                                    \
  (type *)_aligned_malloc(ALIGNED_SIZE(sizeof(type) * (size_t)(sz)), ALIGNMENT)
#define ARRAY_DELETE(p) _aligned_free(p)
#else
#define ARRAY_NEW(type, sz)                                                    \
  (type *)aligned_alloc(ALIGNMENT, ALIGNED_SIZE(sizeof(type) * (size_t)(sz)))
#define ARRAY_DELETE(p) free(p)
#endif

#define ARRAY_ZERO(type, sz, x) memset((x), 0, sizeof(type) * (size_t)(sz))
#define ARRAY_COPY(type, sz, x, y) memcpy((y), (x), sizeof(type) * (size_t)(sz))
