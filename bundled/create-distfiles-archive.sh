#!/bin/sh

set -eu

if [ "$#" -ne 3 ]; then
	echo "usage: $0 SOURCE_DIR VERSION OUTPUT_ARCHIVE" >&2
	exit 2
fi

source_dir=$(CDPATH= cd -P "$1" && pwd)
version=$2
output=$3

case $version in
	''|*[!A-Za-z0-9._-]*)
		echo "invalid version: $version" >&2
		exit 2
		;;
esac

(cd "$source_dir/bundled/distfiles" && sha256sum -c ../SHA256SUMS)

stage=$(mktemp -d)
trap 'rm -rf "$stage"' EXIT HUP INT TERM
mkdir -p "$stage/gtkdialog-$version/bundled"
cp -a "$source_dir/bundled/distfiles" "$stage/gtkdialog-$version/bundled/"
tar -C "$stage" -czf "$output" "gtkdialog-$version/bundled/distfiles"
