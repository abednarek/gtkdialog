# Distribution package recipes

These recipes package GTK2 `gdlg2` without replacing a distribution's
`gtkdialog` command. The 0.9.2 release has two complementary source assets:
`gtkdialog-0.9.2.tar.gz`, produced by `make dist`, and
`gtkdialog-0.9.2-bundled-sources.tar.gz`, containing the pinned extension
source archives. Both unpack into the same `gtkdialog-0.9.2` directory.
The ordinary source archive includes generated Autotools/parser files, but
not the large extension archives. An automatically generated Git tag archive
is **not** a substitute. Both release assets must be accessible to builders
that need the bundled sources.

The standard Alpine and Void recipes use only the ordinary source archive,
distribution GTK2 and JSON-GLib; other optional GTK2 integrations remain
disabled. Arch has only a bundled recipe and does not use AUR.

The separate `bundled/` recipes consume both archives and build eight pinned
GTK2 extensions into `/opt/gldg2`. Arch also builds private GTK2 2.24.33;
Alpine and Void keep distribution GTK2. All three use distribution JSON-GLib.
The installed entry point is `/opt/gldg2/bin/gdlg2`; its runtime lookup is
scoped to that process.
See [the bundled build notes](../bundled/README.md).

Use `packaging/alpine/APKBUILD` or `packaging/alpine/bundled/APKBUILD` in an
Alpine `abuild` package directory, `packaging/arch/bundled/PKGBUILD` in a
`makepkg` directory, or the Void templates in their respective
`void-packages/srcpkgs/gdlg2[-bundled]/template` paths. The recipes include
checksums of the exact uploaded archives. Staged builds and test suites do
not replace testing the finished packages in each distribution.
