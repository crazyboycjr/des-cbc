#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([des-cbc], [0.1], [crazyboycjr@gmail.com])
AC_CONFIG_SRCDIR([cbc.cpp])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE([-Wall foreign])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR

# Checks for libraries.
AC_PROG_RANLIB

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.

# Builtin functions - required since I am lazy :)
AX_GCC_BUILTIN(__builtin_bswap32)
AX_GCC_BUILTIN(__builtin_bswap64)

# Extra Functionalities
AC_ARG_ENABLE([debug], AS_HELP_STRING([--enable-debug],
			  [build with additional debugging code]),
			  [CPPFLAGS='-g'; AC_DEFINE([DEBUG], [], [Debugging])])

# DES Number of Rounds
AC_DEFINE([NR], [16], [DES Number of Rounds])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
