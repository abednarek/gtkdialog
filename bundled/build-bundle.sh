#!/bin/sh

set -eu

if [ "$#" -ne 4 ]; then
	echo "usage: $0 SOURCE_DIR WORK_DIR PACKAGE_DIR system-gtk2|private-gtk2" >&2
	exit 2
fi

source_dir=$(CDPATH= cd -P "$1" && pwd)
mkdir -p "$2" "$3"
work_dir=$(CDPATH= cd -P "$2" && pwd)
package_dir=$(CDPATH= cd -P "$3" && pwd)
gtk_mode=$4

case $gtk_mode in
	system-gtk2|private-gtk2) ;;
	*) echo "unknown GTK2 mode: $gtk_mode" >&2; exit 2 ;;
esac
case $work_dir/ in
	"$source_dir/"*) echo "build directory must be outside the source tree" >&2; exit 2 ;;
esac
case $package_dir/ in
	"$source_dir/"*) echo "package directory must be outside the source tree" >&2; exit 2 ;;
esac

if [ -e "$work_dir/stage" ]; then
	echo "build staging directory already exists: $work_dir/stage" >&2
	exit 2
fi

(cd "$source_dir/bundled/distfiles" && sha256sum -c ../SHA256SUMS)

prefix=/opt/gldg2
stage=$work_dir/stage
stage_prefix=$stage$prefix
pc_overlay=$work_dir/pkgconfig
build_jobs=${BUNDLE_JOBS:-2}
mkdir -p "$stage_prefix/lib" "$work_dir/sources" "$work_dir/build" "$pc_overlay"

PKG_CONFIG_PATH=$pc_overlay${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}
LD_LIBRARY_PATH=$stage_prefix/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
XDG_DATA_DIRS=$stage_prefix/share:${XDG_DATA_DIRS:-/usr/local/share:/usr/share}
LDFLAGS=${LDFLAGS:+$LDFLAGS }-Wl,-rpath-link,$stage_prefix/lib
export PKG_CONFIG_PATH LD_LIBRARY_PATH XDG_DATA_DIRS LDFLAGS

refresh_pc_overlay()
{
	for pc in "$stage_prefix"/lib/pkgconfig/*.pc; do
		[ -f "$pc" ] || continue
		sed "s|^prefix=$prefix$|prefix=$stage_prefix|" "$pc" > "$pc_overlay/${pc##*/}"
	done
}

build_component()
{
	component=$1
	archive=$2
	directory=$3
	shift 3
	source=$work_dir/sources/$directory
	build=$work_dir/build/$component

	echo "Building $component ($gtk_mode)"
	tar --no-same-owner -xf "$source_dir/bundled/distfiles/$archive" \
		-C "$work_dir/sources"
	for local_patch in "$source_dir/bundled/patches/$component/"*.patch; do
		[ -f "$local_patch" ] || continue
		patch --batch -d "$source" -p1 < "$local_patch"
	done

	case $component in
		gtk2)
			# Preserve the patched generated Makefiles without regenerating
			# this historical Autotools project.
			touch "$source/Makefile.in" "$source/gdk/Makefile.in" \
				"$source/gtk/Makefile.in"
			;;
		gtkdatabox) touch "$source/Makefile.in" ;;
		gtkspell) touch "$source/configure" ;;
		gtksheet) (cd "$source" && autoreconf -fi) ;;
	esac

	mkdir -p "$build"
	(cd "$build" && "$source/configure" --prefix="$prefix" "$@" && \
		make -j "$build_jobs" && make install DESTDIR="$stage")
	refresh_pc_overlay
}

if [ "$gtk_mode" != system-gtk2 ]; then
	build_component gtk2 gtk+-2.24.33.tar.xz gtk+-2.24.33 \
		--with-gdktarget=x11 --disable-introspection --disable-gtk-doc \
		--disable-static
fi

build_component gtkspell gtkspell-2.0.16.tar.gz gtkspell-2.0.16 --disable-static
build_component gtksourceview gtksourceview-2.10.5.tar.bz2 \
	gtksourceview-2.10.5 --disable-gtk-doc --disable-introspection
build_component gtkdatabox gtkdatabox-0.9.3.1.tar.gz \
	gtkdatabox-0.9.3.1 --disable-libglade --disable-glade
build_component gtksheet gtksheet-3.5.1.tar.gz gtksheet-3.5.1 \
	--disable-glade --disable-tests
build_component gdl gdl-2.30.1.tar.bz2 gdl-2.30.1 \
	--disable-gtk-doc --disable-introspection --disable-glade
build_component goocanvas goocanvas-1.0.0.tar.bz2 goocanvas-1.0.0 \
	--disable-gtk-doc --disable-introspection
build_component libwnck libwnck-2.30.7.tar.bz2 libwnck-2.30.7 \
	--disable-gtk-doc --disable-introspection
build_component vte vte-gtk2-0.28.2-gtk2.4.tar.gz \
	vte-gtk2-0.28.2-gtk2.4 --with-gtk=2.0 --disable-python \
	--disable-introspection --disable-gtk-doc --disable-gnome-pty-helper \
	--disable-glade --disable-static

echo "Building gdlg2 ($gtk_mode)"
cp -a "$source_dir/." "$work_dir/sources/gtkdialog"
(cd "$work_dir/sources/gtkdialog" && autoreconf -fi)
mkdir -p "$work_dir/build/gtkdialog"
(cd "$work_dir/build/gtkdialog" && \
	"$work_dir/sources/gtkdialog/configure" --prefix="$prefix" \
	--with-json-glib --with-vte --with-gtkspell --with-gtksourceview \
	--with-gtkdatabox --with-gtksheet --with-gdl --with-goocanvas \
	--with-libwnck --with-unix-print && \
	make -j "$build_jobs" && make install DESTDIR="$stage")

mkdir -p "$stage_prefix/libexec" "$stage_prefix/bin"
mv "$stage_prefix/bin/gdlg2" "$stage_prefix/libexec/gdlg2"
install -m 755 "$source_dir/bundled/gdlg2-launcher" "$stage_prefix/bin/gdlg2"
cp -a "$stage/." "$package_dir/"

echo "Bundled gdlg2 staged in $package_dir$prefix"
