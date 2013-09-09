#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([toolkit], [0.0.1], [gsingh2011@gmail.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([xcb], [xcb_connect], [], [AC_MSG_ERROR([Missing libcairo])])
AC_CHECK_LIB([cairo], [cairo_create], [], [AC_MSG_ERROR([Missing libxcb])])
PKG_CHECK_MODULES([DEPS], [cairo xcb], [], [AC_MSG_ERROR([Missing required libraries' pkg-config files])])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

AC_OUTPUT
