#pragma once

#if defined(__clang__) || defined(__gcc__)
    #define STATIC_ASSERT _Static_assert
#else
    #define STATIC_ASSERT static_assert
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define THALLIUM_PLATFORM_WINDOWS 1
    #if !defined(_WIN64)
        #error "64-bit is required on Windows!"
    #endif
#else
    #error "Unknown platform!"
#endif

#if defined(THALLIUM_EXPORT)
    #if defined(_MSC_VER)
        #define THALLIUM_API __declspec(dllexport)
    #else
        #define THALLIUM_API __attribute__((visibility("default")))
    #endif
#elif defined(THALLIUM_IMPORT)
    #if defined(_MSC_VER)
        #define THALLIUM_API __declspec(dllimport)
    #else
        #define THALLIUM_API
    #endif
#else
    #error "Please define THALLIUM_EXPORT or THALLIUM_IMPORT"
#endif

#if defined(_MSC_VER)
    #define THALLIUM_INLINE __forceinline
    #define THALLIUM_NO_INLINE __declspec(noinline)
#else
    #define THALLIUM_INLINE static inline
    #define THALLIUM_NO_INLINE
#endif

#define cast(type) (type)
#define nil (cast(void*) 0)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;

typedef u8 b8;
typedef int b32;

#define SIZE_ASSERT(type, size) \
    STATIC_ASSERT(sizeof(type) == size, "Expected sizeof " #type " to be " #size " bytes.")

SIZE_ASSERT(u8, 1);
SIZE_ASSERT(u16, 2);
SIZE_ASSERT(u32, 4);
SIZE_ASSERT(u64, 8);

SIZE_ASSERT(s8, 1);
SIZE_ASSERT(s16, 2);
SIZE_ASSERT(s32, 4);
SIZE_ASSERT(s64, 8);

SIZE_ASSERT(f32, 4);
SIZE_ASSERT(f64, 8);

SIZE_ASSERT(b8, 1);
SIZE_ASSERT(b32, 4);

#undef SIZE_ASSERT

enum {
    FALSE = 0,
    TRUE = 1,
};
