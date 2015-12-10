AC_INIT([nebredis], [0.1])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])
m4_pattern_allow([AM_PROG_AR], [AM_PROG_AR])
LT_INIT

PKG_CHECK_MODULES([GLIB], [glib-2.0])
PKG_CHECK_MODULES([NAEMON], [naemon])

PKG_CHECK_MODULES([HIREDIS], [hiredis], [], [:])

if test "x$HIREDIS_LIBS" = "x"; then
	AC_CHECK_LIB(hiredis, redisReaderCreate,
				 [AC_CHECK_HEADER([hiredis/hiredis.h], [HAVE_HIREDIS=yes], [HAVE_HIREDIS=no])],
				 [HAVE_HIREDIS=no]
				 )
	if test "x$HAVE_HIREDIS" = "xyes"; then
		AC_DEFINE([HAVE_HIREDIS], [yes], [Define if you have the hiredis library and headers])
		AC_SUBST([HIREDIS_LIBS], [-lhiredis])
		AC_SUBST([HIREDIS_CFLAGS], [])

	else
		AC_MSG_ERROR([Please install hiredis before trying to build nebredis])
	fi


fi




AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AM_PROG_CC_C_O

AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_CONFIG_FILES([
	Makefile
	])
AC_OUTPUT
