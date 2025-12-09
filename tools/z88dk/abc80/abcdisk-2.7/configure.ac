dnl Process this file with autoconf 2.69 or later to produce
dnl a configure script.
AC_PREREQ([2.69])

AC_CONFIG_MACRO_DIR(autoconf)
AC_CONFIG_MACRO_DIRS(autoconf)

AC_INIT(abcwrite.c)
AC_CONFIG_HEADERS(config/config.h)

AC_CONFIG_AUX_DIR(autoconf/helpers)

dnl If we have wine installed, don't run it when checking for cross-compile
WINELOADER=/dev/null
export WINELOADER

AC_PREFIX_PROGRAM(abcwrite)

dnl Get canonical target ("host") name
AC_CANONICAL_HOST

dnl Checks for programs.
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_INSTALL

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT
AC_TYPE_SIZE_T

dnl Check for endianness
AC_C_BIGENDIAN(AC_DEFINE(WORDS_BIGENDIAN),AC_DEFINE(WORDS_LITTLEENDIAN),,)
AH_TEMPLATE(WORDS_BIGENDIAN,
[Define to 1 if your processor stores words with the most significant
byte first (like Motorola and SPARC, unlike Intel and VAX).])
AH_TEMPLATE(WORDS_LITTLEENDIAN,
[Define to 1 if your processor stores words with the least significant
byte first (like Intel and VAX, unlike Motorola and SPARC).])

dnl Checks for standard header files.
AC_HEADER_STDC
AC_HEADER_STDBOOL
AC_CHECK_HEADERS(inttypes.h)
AC_CHECK_HEADERS(stdnoreturn.h)

dnl Force gcc and gcc-compatible compilers treat signed integers
dnl as 2's complement
PA_ADD_CFLAGS([-fwrapv])

dnl Some environments abuse __STRICT_ANSI__ to disable some
dnl function declarations
PA_ADD_CPPFLAGS([-U__STRICT_ANSI__])

dnl Don't put things in common if we can avoid it.  We don't want to
dnl assume all compilers support common, and this will help find those
dnl problems.  This also works around an OSX linker problem.
PA_ADD_CFLAGS([-fno-common])

dnl No need for super strict IEEE compatibility
PA_ADD_CFLAGS([-ffast-math])

dnl Can we drop unneeded libraries?
PA_ADD_LDFLAGS([-Wl,--as-needed])

dnl gcc stack protector; also needed for MinGW to build dirent.h correctly...
PA_ADD_CFLAGS([-fstack-protector])

dnl Compiler intrinsics
PA_ADD_HEADERS(intrin.h)
PA_HAVE_FUNC(__builtin_expect, (1,1))
PA_HAVE_FUNC(__builtin_constant_p, (0))

AC_CHECK_HEADERS(dirent.h)
AC_CHECK_HEADERS(sys/types.h)
AC_CHECK_HEADERS(sys/stat.h)
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(io.h)
AC_CHECK_HEADERS(windows.h)
AC_CHECK_HEADERS(direct.h)

dnl Useful functions...
AC_CHECK_FUNCS(mempcpy)

dnl These types are POSIX-specific, and Windows does it differently...
AC_CHECK_TYPES([struct _stat64])
AC_CHECK_TYPES([struct stat])
AC_CHECK_FUNCS([stat _stat64])
AC_CHECK_FUNCS([fstat _fstat64])

dnl Newer versions of POSIX have this
AC_CHECK_TYPES(mode_t)

dnl Windows directory handling
AC_CHECK_FUNCS(_mkdir)
AC_CHECK_FUNCS([_findfirst _findfirst64])

dnl Math
AC_SEARCH_LIBS(exp, [m])

dnl libsndfile
AC_ARG_WITH([libsndfile],
  AS_HELP_STRING([--with-libsndfile=dir],
  [location of libsndfile install]))
AS_IF([test x"$with_libsndfile" != xno],
[AS_IF([test x"$with_libsndfile" != x -a x"$with_libsndfile" != xyes],
 [for f in "$with_libsndfile"/lib*; do
   AS_IF([test -d "$f"], [PA_ADD_LDFLAGS([-L"$f"])])
  done
  for f in "$with_libsndfile"/include*; do
   AS_IF([test -d "$f"], [PA_ADD_CPPFLAGS([-I"$f"])])
  done
 ])
AC_SEARCH_LIBS(sf_readf_double, [sndfile sndfile-1],
 [EXTRA_PROGS='casdecode casencode'])
])
AC_SUBST(EXTRA_PROGS)

dnl Programs
AC_PROG_MKDIR_P
AC_PROG_INSTALL

dnl To build the assembly sources, we need z80asm from z88dk
_pa_check_z80asm='z88dk/z80asm z88dk-z80asm z80asm'
AC_ARG_WITH([z80asm],
    AS_HELP_STRING([--with-z80asm=name],
    [name of z88dk z80asm assembler with --reloc-info support]),
    [_pa_check_z80asm="'$withval' $_pa_check_z80asm"])

_pa_search_path_Z80ASM="${PATH_SEPARATOR}$PATH"
AC_ARG_WITH([z88dk],
    AS_HELP_STRING([--with-z88dk=path],
    [path to z88dk executables (used for z80asm)]),
    [_pa_search_path_Z80ASM="${PATH_SEPARATOR}${withval}${_pa_search_path_Z80ASM}"])

AC_CACHE_CHECK([for z80asm from zd88k],
[ac_cv_path_Z80ASM],
[AC_PATH_PROGS_FEATURE_CHECK([Z80ASM],
	[$_pa_check_z80asm],
  [[
    if "$ac_path_Z80ASM" 2>&1 | grep -q '^Z80 Module Assembler' > /dev/null 2>&1
    then
        ac_cv_path_Z80ASM="$ac_path_Z80ASM"
	ac_path_Z80ASM_found=':'
    fi
    ]],
    [ac_cv_path_Z80ASM='not found'],
    [$_pa_search_path_Z80ASM])])

Z80ASM=z80asm
AS_IF([test x"$ac_cv_path_Z80ASM" != x'not found'],
      [
        Z80ASM="$ac_cv_path_Z80ASM"
        EXTRA_PROGS="$EXTRA_PROGS bin2abc"
      ])
AC_SUBST(Z80ASM)

dnl
dnl Check for supported gcc attributes; some compilers (e.g. Sun CC)
dnl support these, but don't define __GNUC__ as they don't support
dnl some other features of gcc.
dnl
PA_ADD_CPPFLAGS([-Werror=attributes])
PA_FUNC_ATTRIBUTE(noreturn)
PA_FUNC_ATTRIBUTE(returns_nonnull)
PA_FUNC_ATTRIBUTE(malloc)
PA_FUNC_ATTRIBUTE(alloc_size, (1))
PA_FUNC_ATTRIBUTE(sentinel,,, [const char *, ...], ["a","b",NULL])
PA_FUNC_ATTRIBUTE(format, [(printf,1,2)], int, [const char *, ...], ["%d",1])
PA_FUNC_ATTRIBUTE(const)
PA_FUNC_ATTRIBUTE(pure)
PA_FUNC_ATTRIBUTE(cold)
PA_FUNC_ATTRIBUTE_ERROR

dnl
dnl support function sections (if available)
dnl
PA_ARG_ENABLED([sections],
 [compile with function/data section support],
 [PA_ADD_CFLAGS([-ffunction-sections])
  PA_ADD_CFLAGS([-fdata-sections])
  PA_ADD_CFLAGS([-Wl,--gc-sections])],
 [])

dnl
dnl support LTO
dnl
PA_ARG_ENABLED([lto],
 [compile with gcc-style link time optimization],
 [PA_ADD_CFLAGS([-flto])
  dnl Note: we use _PROG rather than _TOOL since we are prepending the full
  dnl CC name which ought to already contain the host triplet if needed
  ccbase=`echo "$CC" | awk '{ print $1; }'`
  AC_CHECK_PROGS(CC_AR, [${ccbase}-ar], [$ac_cv_prog_AR])
  AR="$CC_AR"
  AC_CHECK_PROGS(CC_RANLIB, [${ccbase}-ranlib], [$ac_cv_prog_RANLIB])
  RANLIB="$CC_RANLIB"], [])

dnl
dnl support sanitizers (if available)
dnl
PA_ARG_ENABLED([sanitizer],
 [compile with sanitizers enabled],
 [PA_ADD_CFLAGS([-fno-omit-frame-pointer])
  PA_ADD_CFLAGS([-fsanitize=address])
  PA_ADD_CFLAGS([-fsanitize=undefined])])

dnl
dnl Don't make symbols visible, there is no point and it just
dnl makes the code slower.
dnl
PA_ADD_CFLAGS([-fvisibility=hidden])

dnl If we have gcc, add appropriate code cleanliness options
PA_ADD_CPPFLAGS([-W])
PA_ADD_CPPFLAGS([-Wall])
PA_ADD_CPPFLAGS([-pedantic])
dnl LLVM doesn't error out on invalid -W options unless this option is
dnl specified first.  Enable this so this script can actually discover
dnl which -W options are possible for this compiler.
PA_ADD_CPPFLAGS([-Werror=unknown-warning-option])
dnl Suppress format warning on Windows targets due to their <inttypes.h>
PA_ADD_CPPFLAGS([-Wpedantic-ms-format],[-Wno-pedantic-ms-format])
dnl This is needed because we intentionally expect strncpy() to fill
dnl in a zero-padded (not zero-terminated) buffer in several backends
PA_ADD_CPPFLAGS([-Wstringop-truncation],[-Wno-stringop-truncation])
dnl PA_ADD_CPPFLAGS([-Wwrite-strings])
PA_ARG_ENABLED([werror],
 [compile with -Werror to error out on any warning],
 [PA_ADD_CPPFLAGS([-Werror])],
 [PA_ADD_CPPFLAGS([-Werror=implicit])
  PA_ADD_CPPFLAGS([-Werror=missing-braces])
  PA_ADD_CPPFLAGS([-Werror=return-type])
  PA_ADD_CPPFLAGS([-Werror=trigraphs])
  PA_ADD_CPPFLAGS([-Werror=pointer-arith])
  PA_ADD_CPPFLAGS([-Werror=strict-prototypes])
  PA_ADD_CPPFLAGS([-Werror=missing-prototypes])
  PA_ADD_CPPFLAGS([-Werror=missing-declarations])
  PA_ADD_CPPFLAGS([-Werror=comment])
  PA_ADD_CPPFLAGS([-Werror=vla])]
)

dnl
dnl On some versions of gcc, -Werror=missing-prototypes causes problems
dnl with C99-style external inlines.  Test this *after* adding the -Werror
dnl options.
dnl
PA_CHECK_BAD_STDC_INLINE

dnl
dnl support ccache
dnl
PA_ARG_ENABLED([ccache], [compile with ccache], [CC="ccache $CC"], [])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
