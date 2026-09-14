SUBDIRS = src data
if BUILD_TEXINFO
SUBDIRS += doc
endif
DIST_SUBDIRS = src doc data

DIST_GLOBS = \
	doc/reference/* \
	screenshots/* \
	examples/accellabel/* \
	examples/aboutdialog/* \
	examples/arrow/* \
	examples/alignment/* \
	examples/assistant/* \
	examples/aspectframe/* \
	examples/button/* \
	examples/calendar/* \
	examples/cellview/* \
	examples/checkbox/* \
	examples/chooser/* \
	examples/colorbutton/* \
	examples/colorselection/* \
	examples/combobox/* \
	examples/comboboxentry/* \
	examples/comboboxtext/* \
	examples/demoscene/* \
	examples/dialog/* \
	examples/drawingarea/* \
	examples/edit/* \
	examples/entry/* \
	examples/eventbox/* \
	examples/expander/* \
	examples/filechooserbutton/* \
	examples/fixed/* \
	examples/fontbutton/* \
	examples/fontselection/* \
	examples/frame/* \
	examples/grid/* \
	examples/handlebox/* \
	examples/hbox/* \
	examples/hbuttonbox/* \
	examples/hpaned/* \
	examples/hscale/* \
	examples/hscrollbar/* \
	examples/hruler/* \
	examples/hsv/* \
	examples/hseparator/* \
	examples/iconview/* \
	examples/infobar/* \
	examples/layout/* \
	examples/list/* \
	examples/linkbutton/* \
	examples/menu/* \
	examples/menubar/* \
	examples/menuitem/* \
	examples/menuitemseparator/* \
	examples/messagedialog/* \
	examples/miscellaneous/* \
	examples/notebook/* \
	examples/pixmap/* \
	examples/plug/* \
	examples/popupmenu/* \
	examples/pfeme/* \
	examples/pfontview/* \
	examples/playmusic/* \
	examples/progressbar/* \
	examples/radiobutton/* \
	examples/recentchooser/* \
	examples/recentchoosermenu/* \
	examples/scalebutton/* \
	examples/scrolledwindow/* \
	examples/showcase/* \
	examples/sizegroup/* \
	examples/socket/* \
	examples/spinbutton/* \
	examples/spinner/* \
	examples/statusbar/* \
	examples/statusicon/* \
	examples/table/* \
	examples/terminal/* \
	examples/text/* \
	examples/timer/* \
	examples/togglebutton/* \
	examples/toolbar/* \
	examples/toolpalette/* \
	examples/tree/* \
	examples/vbox/* \
	examples/vbuttonbox/* \
	examples/volumebutton/* \
	examples/vpaned/* \
	examples/vscale/* \
	examples/vscrollbar/* \
	examples/vruler/* \
	examples/viewport/* \
	examples/vseparator/* \
	examples/window/*

EXTRA_DIST = \
	.editorconfig \
	autogen.sh \
	README.md \
	$(patsubst $(srcdir)/%,%,$(wildcard $(addprefix $(srcdir)/,$(DIST_GLOBS))))
