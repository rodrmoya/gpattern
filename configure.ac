dnl -*- mode: m4 -*-
AC_PREREQ(2.63)
AC_INIT([gpattern],[0.1.0],[],[gpattern])

AC_CONFIG_HEADERS([config/config.h])
AC_CONFIG_SRCDIR([gpattern/gpattern.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([config])

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_CANONICAL_TARGET

# require automake 1.11
AM_INIT_AUTOMAKE([1.11 dist-xz no-dist-gzip tar-ustar foreign])
AM_MAINTAINER_MODE([enable])

GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE,"$GETTEXT_PACKAGE",[The name of the gettext domain])
IT_PROG_INTLTOOL(0.50.1)
PKG_PROG_PKG_CONFIG([0.22])

## don't rerun to this point if we abort
AC_CACHE_SAVE

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_ISC_POSIX
AC_HEADER_STDC

# no stupid static libraries
AM_DISABLE_STATIC
# enable libtool
AC_PROG_LIBTOOL
# avoid libtool for LTCOMPILE, use it only to link

## don't rerun to this point if we abort
AC_CACHE_SAVE

GLIB_GSETTINGS
GETTING_STARTED_INIT

PKG_CHECK_MODULES(GPATTERN,
                  glib-2.0 >= 2.37.0
                  gobject-2.0)

AC_CONFIG_FILES([
Makefile
gpattern/Makefile
po/Makefile.in
])

AC_OUTPUT
