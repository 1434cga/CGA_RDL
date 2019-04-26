#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(sldd, 0.1, daeun.ju@lge.com)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall gnu foreign])

LT_INIT([dlopen])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_LIBTOOL_DLOPEN

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([limits.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_PID_T
AC_TYPE_UINT32_T

##############################
# with / feature section
##############################
# Select AP/Modem AP compile
AC_ARG_WITH(nad,
	AC_HELP_STRING([--with-nad],[select _DEVICE_NAD or not]),
	[],
	with_nad=yes)
# 20160315. add the option "--with-debug"
AC_ARG_WITH(debug,
	AC_HELP_STRING([--with-debug],[select _DEBUG or not]),
	[enable_debug=$withval],
	enable_debug=no)

##############################
# FLAGS section
##############################
if test "x$with_nad" == "xyes"; then
	AM_CPPFLAGS="$AM_CPPFLAGS -D_DEVICE_NAD=1 -funwind-tables -Werror=return-type"
fi

if test "x$with_nad" == "xno"; then
#	AM_CPPFLAGS="$AM_CPPFLAGS -D_DEVICE_AP=1"
	AM_CPPFLAGS="$AM_CPPFLAGS -D_DEVICE_AP=1 -D_LGE_TCU3_HW_REV_C=1 -funwind-tables -Werror=return-type"
fi

if test "x$with_nad" == "xyes"; then
	AM_CPPFLAGS="$AM_CPPFLAGS -I$(sysdir)/usr/include/libxml2"
	AM_LDFLAGS="$AM_LDFLAGS -lxml2"
fi

if test "x$enable_debug" == "xyes"; then
	AM_CPPFLAGS="$AM_CPPFLAGS -g -D_DEBUG=1"
else
	AM_CPPFLAGS="$AM_CPPFLAGS -O2 -D_NDEBUG=1"
fi

##############################
# CONDITIONAL section
##############################
AM_CONDITIONAL(DEVICE_NAD, test "x${with_nad}" = "xyes")
AM_CONDITIONAL(DEVICE_AP, test "x${with_nad}" = "xno")
AM_CONDITIONAL(DEBUG, test "x${enable_debug}" = "xyes")
AM_CONDITIONAL(NODEBUG, test "x${enable_debug}" = "xno")

AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_CXXFLAGS)
AC_SUBST(AM_LDFLAGS)

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([select])

AC_CHECK_LIB(dl, dlopen)
AC_CHECK_FUNCS(dlopen)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT