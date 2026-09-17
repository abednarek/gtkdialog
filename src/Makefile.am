#
# We do this becouse this way we can use multiple parsers in a single binary
# program.
#
AM_LFLAGS=-P$$(basename $@ lexer.c) -olex.yy.c
AM_YFLAGS=-d -p $$(basename $@ parser.c)
BUILT_SOURCES = \
	gtkdialog_parser.h 

bin_PROGRAMS = gdlg2
gdlg2_SOURCES = \
	widget_accellabel.c widget_accellabel.h \
	widget_arrow.c widget_arrow.h \
	gtkdialog.c gtkdialog.h \
	macros.h \
	printing.c printing.h \
	variables.c variables.h \
	gtkdialog_parser.y gtkdialog_lexer.l \
	stack.c stack.h \
	automaton.c automaton.h \
	attributes.c attributes.h \
	signals.c signals.h \
	stringman.c stringman.h \
	widgets.c widgets.h \
	actions.c actions.h \
	tag_attributes.c tag_attributes.h \
	scrolling.c scrolling.h \
	size_groups.c size_groups.h \
	widget_aboutdialog.c widget_aboutdialog.h \
	widget_alignment.c widget_alignment.h \
	widget_assistant.c widget_assistant.h \
	widget_aspectframe.c widget_aspectframe.h \
	widget_button.c widget_button.h \
	widget_buttonbox.c widget_buttonbox.h \
	widget_calendar.c widget_calendar.h \
	widget_cellview.c widget_cellview.h \
	widget_checkbox.c widget_checkbox.h \
	widget_chooser.c widget_chooser.h \
	widget_colorbutton.c widget_colorbutton.h \
	widget_colorselection.c widget_colorselection.h \
	widget_combobox.c widget_combobox.h \
	widget_comboboxtext.c widget_comboboxtext.h \
	widget_curve.c widget_curve.h \
	widget_dialog.c widget_dialog.h \
	widget_canvas.c widget_canvas.h \
	widget_databox.c widget_databox.h \
	widget_dock.c widget_dock.h \
	widget_drawingarea.c widget_drawingarea.h \
	widget_imageview.c widget_imageview.h \
	widget_edit.c widget_edit.h \
	widget_entry.c widget_entry.h \
	widget_eventbox.c widget_eventbox.h \
	widget_expander.c widget_expander.h \
	widget_filechooserbutton.c widget_filechooserbutton.h \
	widget_filechooserdialog.c widget_filechooserdialog.h \
	widget_fixed.c widget_fixed.h \
	widget_fontbutton.c widget_fontbutton.h \
	widget_fontselection.c widget_fontselection.h \
	widget_frame.c widget_frame.h \
	widget_grid.c widget_grid.h \
	widget_handlebox.c widget_handlebox.h \
	widget_hbox.c widget_hbox.h \
	widget_hscale.c widget_hscale.h \
	widget_hseparator.c widget_hseparator.h \
	widget_hsv.c widget_hsv.h \
	widget_ruler.c widget_ruler.h \
	widget_iconview.c widget_iconview.h \
	widget_infobar.c widget_infobar.h \
	widget_layout.c widget_layout.h \
	widget_linkbutton.c widget_linkbutton.h \
	widget_list.c widget_list.h \
	widget_messagedialog.c widget_messagedialog.h \
	widget_menubar.c widget_menubar.h \
	widget_menuitem.c widget_menuitem.h \
	widget_menu_json.c widget_menu_json.h \
	widget_notebook.c widget_notebook.h \
	widget_paned.c widget_paned.h \
	widget_pixmap.c widget_pixmap.h \
	widget_progressbar.c widget_progressbar.h \
	widget_radiobutton.c widget_radiobutton.h \
	widget_recentchooser.c widget_recentchooser.h \
	widget_scalebutton.c widget_scalebutton.h \
	widget_scrollbar.c widget_scrollbar.h \
	widget_scrolledwindow.c widget_scrolledwindow.h \
	widget_sheet.c widget_sheet.h \
	widget_socket.c widget_socket.h \
	widget_spinbutton.c widget_spinbutton.h \
	widget_spinner.c widget_spinner.h \
	widget_statusbar.c widget_statusbar.h \
	widget_statusicon.c widget_statusicon.h \
	widget_table.c widget_table.h \
	widget_tasklist.c widget_tasklist.h \
	widget_pager.c widget_pager.h \
	widget_windowselector.c widget_windowselector.h \
	widget_offscreenwindow.c widget_offscreenwindow.h \
	widget_pagesetupdialog.c widget_pagesetupdialog.h \
	widget_printdialog.c widget_printdialog.h \
	widget_printing.c widget_printing.h \
	widget_terminal.c widget_terminal.h \
	widget_text.c widget_text.h \
	widget_timer.c widget_timer.h \
	widget_toolbar.c widget_toolbar.h \
	widget_toolpalette.c widget_toolpalette.h \
	widget_tree.c widget_tree.h \
	widget_vbox.c widget_vbox.h \
	widget_viewport.c widget_viewport.h \
	widget_window.c widget_window.h

AM_CFLAGS = \
	$(GTK_CFLAGS) \
	$(GTK_UNIX_PRINT_CFLAGS) \
	$(GTKDATABOX_CFLAGS) \
	$(GOOCANVAS_CFLAGS) \
	$(GDL_CFLAGS) \
	$(LIBWNCK_CFLAGS) \
	$(GTKSHEET_CFLAGS) \
	$(GTKSPELL_CFLAGS) \
	$(GTKSOURCEVIEW_CFLAGS) \
	$(JSON_GLIB_CFLAGS) \
	$(VTE_CFLAGS) -DHAVE_VTE=$(HAVE_VTE)

LIBS = \
	$(GTK_LIBS) \
	$(GTK_UNIX_PRINT_LIBS) \
	$(GTKDATABOX_LIBS) \
	$(GOOCANVAS_LIBS) \
	$(GDL_LIBS) \
	$(LIBWNCK_LIBS) \
	$(GTKSHEET_LIBS) \
	$(GTKSPELL_LIBS) \
	$(GTKSOURCEVIEW_LIBS) \
	$(JSON_GLIB_LIBS) \
	$(VTE_LIBS) \
	@LIBS@
