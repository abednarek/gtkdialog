# Bundled GTK2 extension sources

This directory contains the upstream source archives needed to build a
separate `gdlg2-bundled` package. The normal Alpine and Void `gdlg2` packages
do not use them. The 0.9.2 release separates the ordinary `make dist` archive
from `gtkdialog-0.9.2-bundled-sources.tar.gz`; unpack both into the same
source directory before a bundled build. Arch offers only the bundled recipe.
The bundled installation prefix is `/opt/gldg2`; it must not replace a
distribution's GTK libraries or the normal `gdlg2` executable.

`sources.lock` records the exact archive and source location; `SHA256SUMS`
records its digest. The archives in `distfiles/` are unmodified upstream
releases except for `vte-gtk2`, which is a snapshot of the maintained sibling
project at commit `c88cbdba1d03a1c182e3dbd7fa2f15b3b789c052` (Git tree
`59ea77aa708667e0b71b8a1f613ca835debaf055`). This snapshot includes
that project's GTK2 fixes and retains the historical `vte` pkg-config name.
The source archives' own copyright and license files remain inside them.
Any additional local adaptation must be kept as a separately reviewable
patch under `patches/<component>/`, not applied to the archived source in Git.
Apply patches in filename order after unpacking a component.

For a release, run `make dist` in an out-of-tree build to create the ordinary
source archive. Create the companion archive with
`./bundled/create-distfiles-archive.sh "$PWD" 0.9.2 "$OUTPUT_ARCHIVE"`.
The helper checks every pinned checksum before packaging the sources. Both
archives use the same top-level directory so package builders can unpack them
together.

## Included VTE fork

The pinned `vte-gtk2` snapshot is based on GNOME VTE 0.28.2 and includes
the maintained fork's `0.28.2-gtk2.4` changes plus its subsequent Unix98 PTY
slave-name leak fix. Relative to unmodified 0.28.2, it contains:

- current GCC/GLib and musl build fixes, updated platform detection, and
  corrected PTY setup, child signal handling, and partial-failure cleanup;
- bounded terminal-sequence parsing, corrected split/invalid UTF-8 handling,
  and fixes for embedded NUL input;
- xterm SGR 1006 mouse and 1004 focus reporting, corrected Alt/Meta, Home/End,
  function-key and keypad modifier sequences, and improved 256-colour and
  OSC palette handling;
- italic text rendering, saved-scrollback clearing (`CSI 3 J`), DECSCUSR
  cursor styles, and fixes for cursor addressing, autowrap, resets, TAB/CJK
  cells, selection redraw and regular-expression matches;
- a complete in-memory visible screen and reduced redundant scrollback I/O.

The fork preserves the historical VTE 0.0 public API, `vte` pkg-config name,
and `libvte.so.9` SONAME. Bracketed paste was already present in upstream
0.28.2; it is tested here, not a newly added feature. The fork can optionally
build truecolor support, but this bundle does **not** pass `--enable-truecolor`
and therefore retains the default compact cell layout. The bundle also
disables Python bindings, introspection, gtk-doc and `gnome-pty-helper`; these
components are not required by the GTK2 `gdlg2` terminal widget.

For the exact change history and compatibility details, unpack
`distfiles/vte-gtk2-0.28.2-gtk2.4.tar.gz` and read its `CHANGELOG.md`,
`README.md` and `NOTES.md`. These files are part of the pinned source archive.

GtkSheet 3.5.1 needs its bundled Autotools patch before `autoreconf -fi`.
Configure it with `--disable-glade --disable-tests`; the patch omits generated
documentation from this library-only build, while keeping upstream source
documentation in the archive. No Glade integration is built for gdlg2.
GtkSpell 2's patch updates its generated `configure` script as well as the
source macro, so it does not require regenerating Autotools files.
GTK+ 2.24.33's first patch adapts pointer types for current GLib headers; its
second patch excludes upstream demos and test programs from this runtime-only
build. Both generated and source Makefiles are patched so an Autotools
regeneration retains the same selection of build directories.

GTK+ 2.24.33 is included for the Arch package only. Alpine and Void use their
distribution GTK2 packages. All three use distribution JSON-GLib. The other
eight components are the GTK2 extension libraries requested by `configure`.
The extensions have been staged together on glibc and Alpine/musl; private
GTK2 has been staged on glibc. The staged launchers have also been exercised
in Arch, Alpine and Void root filesystems. The 0.9.2 bundled-sources archive
includes these files, and the package recipes verify both asset checksums. Staged
builds are not finished distribution packages.

An integrated glibc build with all eight extension libraries, including this
VTE snapshot, passed `make check` when the staging directory's `lib` was on
`LD_LIBRARY_PATH` and its `share` was on `XDG_DATA_DIRS`. The latter is needed
for GtkSourceView 2's language definitions and style schemes. A bundled
package must provide these paths to its own process without changing the
system-wide library or data search paths. The extension-only build used
distribution GTK2. A separate glibc staging build also compiled GTK2 2.24.33
privately and linked `gdlg2` against it. That staging build passed all ten
`gdlg2` test groups. A current Void rootfs with the bundled dependencies
passed all ten test groups. Alpine/musl built the bundle and completed
`make dist`; nine test groups passed in a chroot. Its CLI group checks for
empty stderr, while the system Glycin loader reports that it is running
without its own sandbox in that chroot. A direct PNG-loading dialog worked.
This does not establish a passing Alpine package check. The Arch private
GTK2 and bundle builds have passed their staged checks, but no actual 0.9.2
package has yet been built from a published release archive.

`gdlg2-launcher` is intended to be installed as `/opt/gldg2/bin/gdlg2`, with
the actual executable at `/opt/gldg2/libexec/gdlg2`. It derives its own prefix
and prepends only its process's library and data paths. Do not install the
binary directly over the launcher or export these variables system-wide.

Build the staged package from a clean source tree with the necessary build
dependencies already installed. Both output paths must be outside the source
tree; the work directory must not contain an earlier `stage` directory:

```sh
./bundled/build-bundle.sh "$PWD" "$BUILD_DIR" "$PACKAGE_DIR" private-gtk2
```

Use `system-gtk2` instead of `private-gtk2` on Alpine and Void. The script
verifies the pinned archive checksums,
applies the patches, runs all builds outside the source tree and populates
`$PACKAGE_DIR/opt/gldg2`. Its staged
`gdlg2` build can be checked before packaging with:

```sh
LD_LIBRARY_PATH="$BUILD_DIR/stage/opt/gldg2/lib" \
XDG_DATA_DIRS="$BUILD_DIR/stage/opt/gldg2/share:/usr/local/share:/usr/share" \
make -C "$BUILD_DIR/build/gtkdialog" check
```

The build script and staging checks do not substitute for testing an actual
Arch, Alpine or Void package and its declared dependencies.

Verify the archives from the repository root with:

```sh
cd bundled/distfiles
sha256sum -c ../SHA256SUMS
```

Keep generated build trees and installation staging in `/tmp`, never here.
