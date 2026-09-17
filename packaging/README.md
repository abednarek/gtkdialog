# Distribution package recipes

These recipes package GTK2 `gdlg2` without replacing a distribution's
`gtkdialog` command. All six version 0.9.2 recipes consume the same uploaded
`make dist` source archive attached to the 0.9.2 release. It
includes generated Autotools/parser files and the pinned `bundled/` sources.
An automatically generated Git tag archive is **not** a substitute.
The release asset must be accessible to the package builder.

The standard Alpine and Void recipes use distribution GTK2 and JSON-GLib;
other optional GTK2 integrations remain disabled. Arch does not use AUR: its
standard recipe builds private GTK2 2.24.33 under `/opt/gldg2` with
JSON-GLib and installs a `/usr/bin/gdlg2` symlink.

The separate `bundled/` recipes build eight pinned GTK2 extensions into
`/opt/gldg2`. Arch also builds private GTK2 2.24.33; Alpine and Void keep
distribution GTK2. All three use distribution JSON-GLib. Arch's two variants
share a private prefix and conflict with each other. The installed entry point
is `/opt/gldg2/bin/gdlg2`; its runtime lookup is scoped to that process.
See [the bundled build notes](../bundled/README.md).

Use `packaging/alpine/APKBUILD` or `packaging/alpine/bundled/APKBUILD` in an
Alpine `abuild` package directory, the corresponding Arch `PKGBUILD` in a
`makepkg` directory, or the Void templates in their respective
`void-packages/srcpkgs/gdlg2[-bundled]/template` paths. The recipes include
checksums of the exact uploaded archive. Staged builds and test suites do not
replace testing the finished packages in each distribution.
