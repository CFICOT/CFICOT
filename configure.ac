AC_INIT([CFICOT], [m4_esyscmd_s([cat VERSION])], [])
: ${CFLAGS=""}

LT_INIT()
AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_AS()

case "$host_os" in
mingw* | cygwin* | msys*)
	OS="windows"
		;;
linux*)
	OS="linux"
		;;
*)
	OS="unknown"
		;;
esac

AM_CONDITIONAL(WINDOWS_BUILD, test x$OS = xwindows)
AM_CONDITIONAL(ENABLE_DYNAMIC_LIBRARIES, test x$OS = xlinux -o x$OS = xwindows)
if test x$OS = xlinux -o x$OS = xwindows; then
	ENABLE_DYNAMIC_LIBRARIES=yes
fi
AM_CONDITIONAL(ENABLE_X86_64, test x$OS = xlinux -o x$OS = xwindows)
if test x$OS = xlinux -o x$OS = xwindows; then
	ENABLE_X86_64=yes
fi

CFLAGS+=" -Wall -Wextra -Wstrict-prototypes -save-temps=obj"

if test x$OS = xlinux -o x$OS = xwindows; then
        LDFLAGS+=" -shared"
else
	LDFLAGS+=" -static"
fi

debug_default=yes
m4_define(m4_toupper(debug_default), [[yes]])
optimization_default=s
m4_define(m4_toupper(optimization_default), [[s]])
build_default=yes
m4_define(m4_toupper(build_default), [[yes]])
dist_profile_default=mcu
m4_define(m4_toupper(dist_profile_default), [[mcu]])

AC_MSG_CHECKING(optimization)
AC_ARG_VAR(optimization, [select compiler optimization (0, 1, 2, s (size)), default: ] OPTIMIZATION_DEFAULT)
AS_IF([test x$optimization = x], [OPTIMIZATION=$optimization_default], [OPTIMIZATION="$optimization"])
AC_MSG_RESULT($OPTIMIZATION)

CFLAGS+=" -O$OPTIMIZATION"

AC_MSG_CHECKING(debug)
AC_ARG_ENABLE(debug,
	      [AS_HELP_STRING([--enable-debug], [turns on debug, default: ]
	                                         DEBUG_DEFAULT)],
	      [], [enable_debug=$debug_default])
AM_CONDITIONAL(ENABLE_DEBUG, test x$enable_debug = xyes)
if test x$enable_debug = xyes; then
	CFLAGS+=" -ggdb"
	AC_MSG_RESULT(enabled)
else
	AC_MSG_RESULT(disabled)
fi

AC_MSG_CHECKING(build)
AC_ARG_ENABLE(build, [AS_HELP_STRING([--enable-build],
	      [build, default: ] BUILD_DEFAULT)],
	      [], [enable_build=$build_default])
if test x$enable_build = xno; then
	AC_MSG_RESULT(disabled)
else
	AC_MSG_RESULT(enabled)
fi
AM_CONDITIONAL(DISABLE_BUILD, test x$enable_build = xno)

# Native or generic implementation depending on the core family and Evaluation/Production
NATIVE_ARM_CODE=""
AC_MSG_CHECKING(ARM Cortex-M core)
if test x${CPU_FAMILY} = xARM; then
	NATIVE_ARM_CODE="yes"
	AC_MSG_RESULT(yes)
else
	NATIVE_ARM_CODE="no"
	AC_MSG_RESULT(no)
fi
AM_CONDITIONAL([ENABLE_NATIVE_ARM_CODE], [test x$NATIVE_ARM_CODE = xyes])

# Detect LX6/LX7 family
NATIVE_XTENSA_CODE=""
AC_MSG_CHECKING(Tensilica LX6/LX7 core)
if test x${CPU_FAMILY} = xXTENSA; then
	NATIVE_XTENSA_CODE="yes"
	AC_MSG_RESULT(yes)
else
	NATIVE_XTENSA_CODE="no"
	AC_MSG_RESULT(no)
fi
AM_CONDITIONAL([ENABLE_NATIVE_XTENSA_CODE], [test x$NATIVE_XTENSA_CODE = xyes])

#
# Distribution
#

AC_MSG_CHECKING(Distribution profile)
AC_ARG_VAR(dist_profile,
	   [Select distribution profile: full, mcu, autotools, default: ]
	   DIST_PROFILE_DEFAULT)
AS_IF([test x$dist_profile = x], [DIST_PROFILE="$dist_profile_default"],
      [DIST_PROFILE="$dist_profile"])
AM_CONDITIONAL(DIST_PROFILE_FULL,
	       test x$DIST_PROFILE = xfull || test x$DIST_PROFILE = xautotools)
AM_CONDITIONAL(DIST_PROFILE_MCU,
	       test x$DIST_PROFILE = xmcu)
AC_MSG_RESULT($DIST_PROFILE)

if test -f "$srcdir/doc/configure.ac"; then
	AC_CONFIG_SUBDIRS([doc])
	DOC_TOOLCHAIN="yes"
fi
AM_CONDITIONAL(DOC_TOOLCHAIN_PRESENT, test x$DOC_TOOLCHAIN = xyes)

AC_SUBST([INSTALL_HEADER], ["\$(INSTALL_DATA) -C"])

AC_MSG_NOTICE(................................................................)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
