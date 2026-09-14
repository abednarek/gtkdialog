
EXTRA_DIST = gtkdialog.texi version.texi.in mdate-sh texinfo.tex

if BUILD_TEXINFO
gtkdialog_infodir = $(infodir)
gtkdialog_info_DATA = gtkdialog.info

gtkdialog.info: $(srcdir)/gtkdialog.texi version.texi
	$(MAKEINFO) -I $(srcdir) -I . -o $@ $(srcdir)/gtkdialog.texi

TEXI2DVI = texi2dvi
.PHONY: pdf
pdf: gtkdialog.pdf

gtkdialog.pdf: $(srcdir)/gtkdialog.texi version.texi
	$(TEXI2DVI) --pdf --batch -I $(srcdir) -I . $(srcdir)/gtkdialog.texi

CLEANFILES = gtkdialog.info gtkdialog.pdf
endif
