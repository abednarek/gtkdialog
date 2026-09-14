# Process this file with autoconf to produce a configure script.
AC_INIT([gtkdialog], [0.9.0-dev], [artur@unix.org.pl], [gtkdialog],
	[https://github.com/abednarek/gtkdialog])
AM_INIT_AUTOMAKE([foreign])
#AM_CONFIG_HEADER([config.h]) Obsolete.
AC_CONFIG_HEADERS([config.h])
AM_MAINTAINER_MODE

AC_ARG_ENABLE([texinfo],
	[AS_HELP_STRING([--enable-texinfo],
		[build and install the Texinfo manual (default: no)])],
	[], [enable_texinfo=no])
AS_IF([test "x$enable_texinfo" = "xyes"], [
	AC_PATH_PROG([MAKEINFO], [makeinfo], [no])
	AS_IF([test "x$MAKEINFO" = "xno"], [
		AC_MSG_ERROR([--enable-texinfo requires makeinfo])
	])
])
AM_CONDITIONAL([BUILD_TEXINFO], [test "x$enable_texinfo" = "xyes"])

# Setting config.h macros.
AC_PATH_PROG(svnv, svnversion)
build_details=release
if test -n "$svnv"; then
	svn_revision=$("$svnv" -n 2>/dev/null)
	case "$svn_revision" in
		""|exported|*"Unversioned directory"*) ;;
		*) build_details="r$svn_revision" ;;
	esac
fi
AC_DEFINE_UNQUOTED(BUILD_DETAILS, ["$build_details"], [Build details])
# Don't uncomment this as there's a limit on the number of file
# descriptors -- see /proc/sys/fs/inotify/max_user_instances
#AC_CHECK_HEADER([sys/inotify.h], [AC_DEFINE([HAVE_SYS_INOTIFY_H], 1,
#	[Define to 1 if you have the <sys/inotify.h> header file.])])

# Checks for programs.
#AM_PROG_CC_STDC Obsolete.
AC_PROG_CC
AC_PROG_LEX([noyywrap])
AC_PROG_YACC
AC_SUBST(YACC)

pkg_modules="gtk+-2.0 >= 2.0.0"
PKG_CHECK_MODULES(GTK, [$pkg_modules])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES([JSON_GLIB], [json-glib-1.0 >= 1.0.0],
	[have_json_glib=1], [have_json_glib=0])
AC_DEFINE_UNQUOTED([HAVE_JSON_GLIB], [$have_json_glib],
	[Define to 1 when JSON-GLib is available.])

vte_lib="vte >= 0.23.5"
PKG_CHECK_MODULES([VTE], \
	[$vte_lib], \
	AC_SUBST(HAVE_VTE, 1),\
	AC_SUBST(HAVE_VTE, 0))
AC_SUBST(VTE_CFLAGS)
AC_SUBST(VTE_LIBS)

# Checks for header files.
AC_CHECK_HEADERS([locale.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	doc/Makefile
	doc/version.texi
	data/Makefile
	data/icons/Makefile
	data/icons/hicolor/Makefile
	data/icons/hicolor/32x32/Makefile
	data/icons/hicolor/32x32/apps/Makefile
])
AC_OUTPUT
