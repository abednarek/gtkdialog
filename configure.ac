# Process this file with autoconf to produce a configure script.
AC_INIT([gtkdialog], [0.9.2], [artur@unix.org.pl], [gtkdialog],
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

pkg_modules="gtk+-2.0 >= 2.24.33 fontconfig"
PKG_CHECK_MODULES(GTK, [$pkg_modules])
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

AC_ARG_ENABLE([all-extensions],
	[AS_HELP_STRING([--enable-all-extensions],
		[enable every optional GTK2 extension (default: no)])],
	[], [enable_all_extensions=no])
AS_CASE([$enable_all_extensions],
	[yes|no], [],
	[AC_MSG_ERROR([--enable-all-extensions accepts only yes or no])])

AC_ARG_WITH([unix-print],
	[AS_HELP_STRING([--with-unix-print],
		[enable GTK2 Unix printing support (default: no)])],
	[], [with_unix_print=$enable_all_extensions])
AS_CASE([$with_unix_print],
	[yes|no], [],
	[AC_MSG_ERROR([--with-unix-print accepts only yes or no])])
have_gtk_unix_print=0
gtk_unix_print_status=disabled
GTK_UNIX_PRINT_CFLAGS=
GTK_UNIX_PRINT_LIBS=
AS_IF([test "x$with_unix_print" = "xyes"], [
	PKG_CHECK_MODULES([GTK_UNIX_PRINT], [gtk+-unix-print-2.0 >= 2.10.0],
		[have_gtk_unix_print=1
		 gtk_unix_print_status=enabled],
		[AC_MSG_ERROR([--with-unix-print requires gtk+-unix-print-2.0 2.10 or later])])
])
AC_DEFINE_UNQUOTED([HAVE_GTK_UNIX_PRINT], [$have_gtk_unix_print],
	[Define to 1 when GTK2 Unix printing support is available.])
AC_SUBST([GTK_UNIX_PRINT_CFLAGS])
AC_SUBST([GTK_UNIX_PRINT_LIBS])

AC_ARG_WITH([json-glib],
	[AS_HELP_STRING([--with-json-glib],
		[enable JSON-GLib tree support (default: no)])],
	[], [with_json_glib=$enable_all_extensions])
AS_CASE([$with_json_glib],
	[yes|no], [],
	[AC_MSG_ERROR([--with-json-glib accepts only yes or no])])
have_json_glib=0
json_glib_status=disabled
JSON_GLIB_CFLAGS=
JSON_GLIB_LIBS=
AS_IF([test "x$with_json_glib" = "xyes"], [
	PKG_CHECK_MODULES([JSON_GLIB], [json-glib-1.0 >= 1.0.0],
		[have_json_glib=1
		 json_glib_status=enabled],
		[AC_MSG_ERROR([--with-json-glib requires JSON-GLib 1.0])])
])
AC_DEFINE_UNQUOTED([HAVE_JSON_GLIB], [$have_json_glib],
	[Define to 1 when JSON-GLib is available.])
AC_SUBST([JSON_GLIB_CFLAGS])
AC_SUBST([JSON_GLIB_LIBS])

AC_ARG_WITH([goocanvas],
	[AS_HELP_STRING([--with-goocanvas],
		[enable GooCanvas 1 GTK2 support (default: no)])],
	[goocanvas_option=explicit],
	[with_goocanvas=$enable_all_extensions
	 goocanvas_option=inherited])
AS_CASE([$with_goocanvas],
	[yes|no], [],
	[AC_MSG_ERROR([--with-goocanvas accepts only yes or no])])
have_goocanvas=0
goocanvas_status=disabled
GOOCANVAS_CFLAGS=
GOOCANVAS_LIBS=
AS_IF([test "x$with_goocanvas" = "xyes" && test "x$with_json_glib" != "xyes"], [
	AS_IF([test "x$goocanvas_option" = "xexplicit"], [
		AC_MSG_ERROR([--with-goocanvas also requires --with-json-glib])
	], [
		AC_MSG_NOTICE([disabling GooCanvas because JSON-GLib was explicitly excluded])
		with_goocanvas=no
	])
])
AS_IF([test "x$with_goocanvas" = "xyes"], [
	PKG_CHECK_MODULES([GOOCANVAS], [goocanvas >= 1.0.0],
		[have_goocanvas=1
		 goocanvas_status=enabled],
		[AC_MSG_ERROR([--with-goocanvas requires GooCanvas 1 for GTK2])])
])
AC_DEFINE_UNQUOTED([HAVE_GOOCANVAS], [$have_goocanvas],
	[Define to 1 when GooCanvas 1 for GTK2 is available.])
AC_SUBST([GOOCANVAS_CFLAGS])
AC_SUBST([GOOCANVAS_LIBS])

AC_ARG_WITH([gtkspell],
	[AS_HELP_STRING([--with-gtkspell],
		[enable GtkSpell 2 support (default: no)])],
	[], [with_gtkspell=$enable_all_extensions])
AS_CASE([$with_gtkspell],
	[yes|no], [],
	[AC_MSG_ERROR([--with-gtkspell accepts only yes or no])])
have_gtkspell=0
gtkspell_status=disabled
GTKSPELL_CFLAGS=
GTKSPELL_LIBS=
AS_IF([test "x$with_gtkspell" = "xyes"], [
	PKG_CHECK_MODULES([GTKSPELL], [gtkspell-2.0 >= 2.0.0],
		[have_gtkspell=1
		 gtkspell_status=enabled],
		[AC_MSG_ERROR([--with-gtkspell requires GtkSpell 2])])
])
AC_DEFINE_UNQUOTED([HAVE_GTKSPELL], [$have_gtkspell],
	[Define to 1 when GtkSpell 2 is available.])
AC_SUBST([GTKSPELL_CFLAGS])
AC_SUBST([GTKSPELL_LIBS])

AC_ARG_WITH([gtksourceview],
	[AS_HELP_STRING([--with-gtksourceview],
		[enable GtkSourceView 2 support (default: no)])],
	[], [with_gtksourceview=$enable_all_extensions])
AS_CASE([$with_gtksourceview],
	[yes|no], [],
	[AC_MSG_ERROR([--with-gtksourceview accepts only yes or no])])
have_gtksourceview=0
gtksourceview_status=disabled
GTKSOURCEVIEW_CFLAGS=
GTKSOURCEVIEW_LIBS=
AS_IF([test "x$with_gtksourceview" = "xyes"], [
	PKG_CHECK_MODULES([GTKSOURCEVIEW], [gtksourceview-2.0 >= 2.10.0],
		[have_gtksourceview=1
		 gtksourceview_status=enabled],
		[AC_MSG_ERROR([--with-gtksourceview requires GtkSourceView 2])])
])
AC_DEFINE_UNQUOTED([HAVE_GTKSOURCEVIEW], [$have_gtksourceview],
	[Define to 1 when GtkSourceView 2 is available.])
AC_SUBST([GTKSOURCEVIEW_CFLAGS])
AC_SUBST([GTKSOURCEVIEW_LIBS])

AC_ARG_WITH([gtkdatabox],
	[AS_HELP_STRING([--with-gtkdatabox],
		[enable GtkDatabox 0.9 GTK2 support (default: no)])],
	[], [with_gtkdatabox=$enable_all_extensions])
AS_CASE([$with_gtkdatabox],
	[yes|no], [],
	[AC_MSG_ERROR([--with-gtkdatabox accepts only yes or no])])
have_gtkdatabox=0
gtkdatabox_status=disabled
GTKDATABOX_CFLAGS=
GTKDATABOX_LIBS=
AS_IF([test "x$with_gtkdatabox" = "xyes"], [
	PKG_CHECK_MODULES([GTKDATABOX], [gtkdatabox >= 0.9.3 gtkdatabox < 1.0],
		[have_gtkdatabox=1
		 gtkdatabox_status=enabled],
		[AC_MSG_ERROR([--with-gtkdatabox requires GtkDatabox 0.9 for GTK2])])
])
AC_DEFINE_UNQUOTED([HAVE_GTKDATABOX], [$have_gtkdatabox],
	[Define to 1 when GtkDatabox 0.9 for GTK2 is available.])
AC_SUBST([GTKDATABOX_CFLAGS])
AC_SUBST([GTKDATABOX_LIBS])
AS_IF([test "x$have_gtkdatabox" = "x1"], [
	AC_SEARCH_LIBS([pow], [m], [], [
		AC_MSG_ERROR([GtkDatabox logarithmic scaling requires pow()])
	])
])

AC_ARG_WITH([gtksheet],
	[AS_HELP_STRING([--with-gtksheet],
		[enable GtkSheet 3 GTK2 support (default: no)])],
	[], [with_gtksheet=$enable_all_extensions])
AS_CASE([$with_gtksheet],
	[yes|no], [],
	[AC_MSG_ERROR([--with-gtksheet accepts only yes or no])])
have_gtksheet=0
gtksheet_status=disabled
GTKSHEET_CFLAGS=
GTKSHEET_LIBS=
AS_IF([test "x$with_gtksheet" = "xyes"], [
	PKG_CHECK_MODULES([GTKSHEET], [gtksheet-3.0 >= 3.5.1],
		[have_gtksheet=1
		 gtksheet_status=enabled],
		[AC_MSG_ERROR([--with-gtksheet requires GtkSheet 3.5.1 for GTK2])])
])
AC_DEFINE_UNQUOTED([HAVE_GTKSHEET], [$have_gtksheet],
	[Define to 1 when GtkSheet 3.5.1 for GTK2 is available.])
AC_SUBST([GTKSHEET_CFLAGS])
AC_SUBST([GTKSHEET_LIBS])

AC_ARG_WITH([gdl],
	[AS_HELP_STRING([--with-gdl],
		[enable GDL 2 GTK2 docking support (default: no)])],
	[], [with_gdl=$enable_all_extensions])
AS_CASE([$with_gdl],
	[yes|no], [],
	[AC_MSG_ERROR([--with-gdl accepts only yes or no])])
have_gdl=0
gdl_status=disabled
GDL_CFLAGS=
GDL_LIBS=
AS_IF([test "x$with_gdl" = "xyes"], [
	PKG_CHECK_MODULES([GDL], [gdl-1.0 >= 2.30.1],
		[have_gdl=1
		 gdl_status=enabled],
		[AC_MSG_ERROR([--with-gdl requires GDL 2.30.1 for GTK2])])
])
AC_DEFINE_UNQUOTED([HAVE_GDL], [$have_gdl],
	[Define to 1 when GDL 2.30.1 for GTK2 is available.])
AC_SUBST([GDL_CFLAGS])
AC_SUBST([GDL_LIBS])

AC_ARG_WITH([libwnck],
	[AS_HELP_STRING([--with-libwnck],
		[enable libwnck 2 GTK2 desktop support (default: no)])],
	[], [with_libwnck=$enable_all_extensions])
AS_CASE([$with_libwnck],
	[yes|no], [],
	[AC_MSG_ERROR([--with-libwnck accepts only yes or no])])
have_libwnck=0
libwnck_status=disabled
LIBWNCK_CFLAGS=
LIBWNCK_LIBS=
AS_IF([test "x$with_libwnck" = "xyes"], [
	PKG_CHECK_MODULES([LIBWNCK], [libwnck-1.0 >= 2.30.0],
		[have_libwnck=1
		 libwnck_status=enabled],
		[AC_MSG_ERROR([--with-libwnck requires libwnck 2 for GTK2])])
])
AC_DEFINE_UNQUOTED([HAVE_WNCK], [$have_libwnck],
	[Define to 1 when libwnck 2 for GTK2 is available.])
AC_SUBST([LIBWNCK_CFLAGS])
AC_SUBST([LIBWNCK_LIBS])

AC_ARG_WITH([vte],
	[AS_HELP_STRING([--with-vte],
		[enable VTE 0.23.5 GTK2 terminal support (default: no)])],
	[], [with_vte=$enable_all_extensions])
AS_CASE([$with_vte],
	[yes|no], [],
	[AC_MSG_ERROR([--with-vte accepts only yes or no])])
have_vte=0
vte_status=disabled
VTE_CFLAGS=
VTE_LIBS=
AS_IF([test "x$with_vte" = "xyes"], [
	PKG_CHECK_MODULES([VTE], [vte >= 0.23.5],
		[have_vte=1
		 vte_status=enabled],
		[AC_MSG_ERROR([--with-vte requires VTE 0.23.5 for GTK2])])
])
HAVE_VTE=$have_vte
AC_SUBST([HAVE_VTE])
AC_SUBST([VTE_CFLAGS])
AC_SUBST([VTE_LIBS])

# Checks for header files.
AC_CHECK_HEADERS([locale.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([setlocale])

AC_MSG_NOTICE([])
AC_MSG_NOTICE([Optional GTK2 extensions:])
AC_MSG_NOTICE([  JSON-GLib:       $json_glib_status])
AC_MSG_NOTICE([  VTE:             $vte_status])
AC_MSG_NOTICE([  GtkSpell:        $gtkspell_status])
AC_MSG_NOTICE([  GtkSourceView:   $gtksourceview_status])
AC_MSG_NOTICE([  GtkDatabox:      $gtkdatabox_status])
AC_MSG_NOTICE([  GtkSheet:        $gtksheet_status])
AC_MSG_NOTICE([  GDL:             $gdl_status])
AC_MSG_NOTICE([  GooCanvas:       $goocanvas_status])
AC_MSG_NOTICE([  libwnck:         $libwnck_status])
AC_MSG_NOTICE([  GTK Unix print:  $gtk_unix_print_status])

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
