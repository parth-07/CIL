AC_INIT([CIL], [0.1.0])

# Create a build-dir for automake work
AC_CONFIG_AUX_DIR([build-dir])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/config.h])

# Add support for C and CPP files
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LN_S
AM_CPPFLAGS="-std=c++11 -Wall -Wextra -Werror -pedantic -g "
AC_SUBST([AM_CPPFLAGS])
AX_PTHREAD

# The project does not follow GNU Coding standards
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Initialize LIBTOOL_DEPS
AC_SUBST([LIBTOOL_DEPS])
LT_INIT([disable-shared])

AC_ARG_ENABLE([examples],
              [AS_HELP_STRING([--enable-examples],
                              [Build examples @<:@no@:>@])],
                              [examples="$enableval"],
                              [examples="no"])
AM_CONDITIONAL([EXAMPLES], [test x$examples = xyes])

AC_ARG_ENABLE([tests],
              [AS_HELP_STRING([--enable-tests],
                              [Build Tests @<:@no@:>@])],
                              [tests="$enableval"],
                              [tests="no"])
AM_CONDITIONAL([TESTS], [test x$tests = xyes ])
AS_IF(
    [test "xtests$" = "xyes"],
    [PKG_CHECK_MODULES(
      [catch2], [catch2 >= 2.13.5],
    )]
)


AC_ARG_ENABLE([libjpeg],
              [AS_HELP_STRING([--enable-libjpeg],
                              [Enable libjpeg support @<:@no@:>@])],
                              [libjpeg="$enableval"],
                              [libjpeg="no"])

# This can be used in Makefile.am
AM_CONDITIONAL([JPEG], [test x$libjpeg = xyes])

# AC_DEFINE can be used in the cpp source files by include <config.h>
AS_IF(
    [test "x$libjpeg" = "xyes"],
    [PKG_CHECK_MODULES(
      [libturbojpeg], [libturbojpeg >= 2.1.1], 
      [AC_DEFINE([CIL_JPEG_ENABLED], [1], [Use LIBJPEG_TURBO])], 
      [PKG_CHECK_MODULES(
         [libjpeg], [libjpeg = 9.4.0], 
         [AC_DEFINE([CIL_JPEG_ENABLED], [1], [Use LIBJPEG])] 
      )]
    )]
)

AC_ARG_ENABLE([libpng],
              [AS_HELP_STRING([--enable-libpng],
                              [Enable libpng support @<:@no@:>@])],
                              [libpng="$enableval"],
                              [libpng="no"])
AM_CONDITIONAL([PNG], [test x$libpng = xyes])
AS_IF(
    [test "x$libpng" = "xyes"],
    [PKG_CHECK_MODULES(
      [libpng], [libpng = 1.6.37],
      [AC_DEFINE([CIL_PNG_ENABLED], [1], [Use LIBPNG])], 
    )]
)

AC_CONFIG_FILES([
    Makefile include/Makefile
    tests/Makefile
])

AC_OUTPUT

AC_MSG_RESULT([-------------------------------------------------------------------------------
CIL is configured as follows. Please verify that this configuration
matches your expectations.

Installation prefix: $prefix
Build examples:      $examples
Enable Tests:        $tests
-------------------------------------------------------------------------------
CIL additional library support:

libpng:       $libpng
libjpeg:      $libjpeg
-------------------------------------------------------------------------------])
