SUBDIRS=32x32

UPDATE_ICON_CACHE=gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook:
	@if test -z "$(DESTDIR)"; then $(UPDATE_ICON_CACHE); fi
uninstall-hook:
	@if test -z "$(DESTDIR)"; then $(UPDATE_ICON_CACHE); fi
