/**
 * \file compiler_gcc.h
 */
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/errno.h>

#include <cmsis_compiler.h>

#ifdef __cplusplus
extern "C" {
#endif
// -------------------------------------------------------------------------- //


#ifndef __SYMBOL_PREFIX
#ifdef NO_UNDERSCORES
#define __SYMBOL_PREFIX
#else
#define __SYMBOL_PREFIX "_"
#endif
#endif

#ifndef C_SYMBOL_NAME
#ifdef NO_UNDERSCORES
#define C_SYMBOL_NAME(name) name
#else
#define C_SYMBOL_NAME(name) _##name
#endif
#endif


#ifndef ASM_LINE_SEP
#define ASM_LINE_SEP ;
#endif

#ifndef C_SYMBOL_DOT_NAME
#define C_SYMBOL_DOT_NAME(name) .##name
#endif

#ifndef WRAP
#define WRAP(x) __wrap_##x
#else
#define WRAP(x) x
#endif

#ifndef __ASSEMBLER__
/* GCC understands weak symbols and aliases; use its interface where
   possible, instead of embedded assembly language.  */

/* Define ALIASNAME as a strong alias for NAME.  */
#define strong_alias(name, aliasname)  _strong_alias(name, aliasname)
#define _strong_alias(name, aliasname) extern __typeof(name) aliasname __attribute__((__alias__(#name)));

/* This comes between the return type and function name in
   a function definition to make that definition weak.  */
#define weak_function       __attribute__((__weak__))
#define weak_const_function __attribute__((__weak__, __const__))

/* Define ALIASNAME as a weak alias for NAME.
   If weak aliases are not available, this defines a strong alias.  */
#define weak_alias(name, aliasname)  _weak_alias(name, aliasname)
#define _weak_alias(name, aliasname) extern __typeof(name) aliasname __attribute__((__weak__, __alias__(#name)));

/* Declare SYMBOL as weak undefined symbol (resolved to 0 if not defined).  */
#define weak_extern(symbol) _weak_extern(symbol)
#define _weak_extern(symbol) asm(".weak " __SYMBOL_PREFIX #symbol);


#endif /* __ASSEMBLER__ */

// -------------------------------------------------------------------------- //


// -------------------------------------------------------------------------- //
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
