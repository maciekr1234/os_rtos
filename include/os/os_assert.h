/**
 * \file os_assert.h
 */
#pragma once

#include <inttypes.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void _os_assert_failed(const char* expr, const char* file, int line) __attribute__((__noreturn__));

#ifdef __cplusplus
}
#endif


#if !defined(OS_ASSERT_DISABLED)


#define OS_ASSERT(expr)                                  \
    do {                                                 \
        if (!(expr)) {                                   \
            _os_assert_failed(#expr, __FILE__, __LINE__); \
        }                                                \
    } while (0)


#else
#define OS_ASSERT(expr) ((void)0)
#endif
