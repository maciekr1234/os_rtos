/**
 * \file compiler_gcc.h
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

#include <cmsis_compiler.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <sys/errno.h>

#ifndef __set_errno
#define __set_errno(val) (errno = (val))
#endif

// IS_IRQ_MASKED(void)
static int IS_IRQ_MASKED(void) {
#if ((defined(__ARM_ARCH_7M__) && __ARM_ARCH_7M__ == 1) || (defined(__ARM_ARCH_7EM__) && __ARM_ARCH_7EM__ == 1) || \
     (defined(__ARM_ARCH_8M_MAIN__) && __ARM_ARCH_8M_MAIN__ == 1))
    return ((__get_PRIMASK() != 0U) || (__get_BASEPRI() != 0U));
#elif (defined(__ARM_ARCH_6M__) && __ARM_ARCH_6M__ == 1))
    return (__get_PRIMASK() != 0U);
#elif (defined(__ARM_ARCH_7A__) && __ARM_ARCH_7A__ == 1))
#define CPSR_MASKBIT_I 0x80U
 return (__get_CPSR() & CPSR_MASKBIT_I) != 0U);
#else
    return (__get_PRIMASK() != 0U);
#endif
}

// IS_IRQ_MODE()
static int IS_IRQ_MODE(void) {
#if (defined(__ARM_ARCH_7A__) && (__ARM_ARCH_7A__ == 1))
#define CPSR_MODE_USER   0x10U
#define CPSR_MODE_SYSTEM 0x1FU
    return ((__get_mode() != CPSR_MODE_USER) && (__get_mode() != CPSR_MODE_SYSTEM));
#else
    return (__get_IPSR() != 0);
#endif
}

extern int IS_IRQ(void);
#ifdef __cplusplus
}
#endif
