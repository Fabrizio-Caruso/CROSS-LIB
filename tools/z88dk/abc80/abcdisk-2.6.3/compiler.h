/*
 * Common definitions that we need for everything
 */

#ifndef COMPILER_H
#define COMPILER_H

#ifdef HAVE_CONFIG_H
# include "config/config.h"
#else
# error "Need compiler-specific hacks here"
#endif

/* On Microsoft platforms we support multibyte character sets in filenames */
#define _MBCS 1

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#else
# include "clib/inttypes.h"	/* Ersatz header file */
#endif

/* These header files should pretty much always be included... */
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <locale.h>
#include <wchar.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_IO_H
# include <io.h>
#endif
#ifdef HAVE_WINDOWS_H
# include <windows.h>
#endif
#ifdef HAVE_DIRECT_H
# include <direct.h>
#endif

#ifdef HAVE_DIRENT_H
# include <dirent.h>
#else
# include "clib/dirent.h"
#endif

#ifndef __cplusplus		/* C++ has false, true, bool as keywords */
# ifdef HAVE_STDBOOL_H
#  include <stdbool.h>
# else
/* This is sort of dangerous, since casts will behave different than
   casting to the standard boolean type.  Always use !!, not (bool). */
typedef enum bool { false, true } bool;
# endif
#endif

/*
 * Endianness, if we can get this from the compiler.  It's better
 * than relying on autoconf, if we can, because of "fat binaries".
 */
#ifdef __BYTE_ORDER__
# undef WORDS_LITTLEENDIAN
# undef WORDS_BIGENDIAN
# if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define WORDS_LITTLEENDIAN 1
# elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#elif defined(__i386__) || defined(__386__) || \
  defined(__x86_64__) || defined(__amd64__)
/* It's an x86, so we know it's littleendian */
# undef WORDS_LITTLEENDIAN
# undef WORDS_BIGENDIAN
# define WORDS_LITTLEENDIAN 1
#endif

/*
 * mempcpy() replacement
 */
#ifndef HAVE_MEMPCPY
static inline void *mempcpy(void *dest, const void *src, size_t n)
{
    memcpy(dest, src, n);
    return (char *)dest + n;
}
#endif

/*
 * asprintf()
 */
#ifndef HAVE_ASPRINTF
extern int asprintf(char **, const char *, ...);
#endif

/*
 * mode_t
 */
#ifndef HAVE_MODE_T
typedef int mode_t;
#endif

/*
 * Hack to support external-linkage inline functions
 */
#ifndef HAVE_STDC_INLINE
# ifdef __GNUC__
#  ifdef __GNUC_STDC_INLINE__
#   define HAVE_STDC_INLINE
#  else
#   define HAVE_GNU_INLINE
#  endif
# elif defined(__GNUC_GNU_INLINE__)
/* Some other compiler implementing only GNU inline semantics? */
#   define HAVE_GNU_INLINE
# elif defined(__STDC_VERSION__)
#  if __STDC_VERSION__ >= 199901L
#   define HAVE_STDC_INLINE
#  endif
# endif
#endif

#ifdef HAVE_STDC_INLINE
# define extern_inline inline
#elif defined(HAVE_GNU_INLINE)
# define extern_inline extern inline
# define inline_prototypes
#else
# define inline_prototypes
#endif

/*
 * Hints to the compiler that a particular branch of code is more or
 * less likely to be taken.
 */
#if HAVE___BUILTIN_EXPECT
# define likely(x)	__builtin_expect(!!(x), 1)
# define unlikely(x)	__builtin_expect(!!(x), 0)
#else
# define likely(x)	(!!(x))
# define unlikely(x)	(!!(x))
#endif

/*
 * How to tell the compiler that a function doesn't return
 */
#ifdef HAVE_STDNORETURN_H
# include <stdnoreturn.h>
# define no_return noreturn void
#elif defined(HAVE_FUNC_ATTRIBUTE_NORETURN)
# define no_return void __attribute__((noreturn))
#elif defined(_MSC_VER)
# define no_return __declspec(noreturn) void
#else
# define no_return void
#endif

/*
 * How to tell the compiler that a function is pure arithmetic
 */
#ifdef HAVE_FUNC_ATTRIBUTE_CONST
# define const_func __attribute__((const))
#else
# define const_func
#endif

/*
 * This function has no side effects, but depends on its arguments,
 * memory pointed to by its arguments, or global variables.
 * NOTE: functions that return a value by modifying memory pointed to
 * by a pointer argument are *NOT* considered pure.
 */
#ifdef HAVE_FUNC_ATTRIBUTE_PURE
# define pure_func __attribute__((pure))
#else
# define pure_func
#endif

/* Determine probabilistically if something is a compile-time constant */
#ifdef HAVE___BUILTIN_CONSTANT_P
# define is_constant(x) __builtin_constant_p(x)
#else
# define is_constant(x) false
#endif

/* Provide a substitute for offsetof() if we don't have one.  This
   variant works on most (but not *all*) systems... */
#ifndef offsetof
# define offsetof(t,m) ((size_t)&(((t *)0)->m))
#endif

/* The container_of construct: if p is a pointer to member m of
   container class c, then return a pointer to the container of which
   *p is a member. */
#ifndef container_of
# define container_of(p, c, m) ((c *)((char *)(p) - offsetof(c,m)))
#endif

/*
 * Hints about malloc-like functions that never return NULL
 */
#ifdef HAVE_FUNC_ATTRIBUTE_RETURNS_NONNULL
# define never_null __attribute__((returns_nonnull))
#else
# define never_null
#endif

#ifdef HAVE_FUNC_ATTRIBUTE_MALLOC
# define safe_alloc never_null __attribute__((malloc))
#else
# define safe_alloc never_null
#endif

#ifdef HAVE_FUNC_ATTRIBUTE_ALLOC_SIZE
# define safe_malloc(s) safe_alloc __attribute__((alloc_size(s)))
# define safe_malloc2(s1,s2) safe_alloc __attribute__((alloc_size(s1,s2)))
# define safe_realloc(s) never_null __attribute__((alloc_size(s)))
#else
# define safe_malloc(s) safe_alloc
# define safe_malloc2(s1,s2) safe_alloc
# define safe_realloc(s) never_null
#endif

#ifdef HAVE_FUNC_ATTRIBUTE_SENTINEL
# define end_with_null __attribute__((sentinel))
#else
# define end_with_null
#endif

/* Useful construct */
#define ARRAY_SIZE(x) ((sizeof x)/(sizeof *(x)))

#endif /* COMPILER_H */
