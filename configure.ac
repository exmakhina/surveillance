#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT(MotionApp, 0.1, thomas@exmakhina.com)
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([capture.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

#
# opencv (HAVE_LIBCV)
#
AC_CHECK_LIB([cv], [cvLoad],[opencvfound=1],[opencvfound=0])
AC_CHECK_LIB([highgui], [cvGrabFrame])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_OUTPUT(Makefile)
