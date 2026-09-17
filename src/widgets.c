/*
 * widgets.c: Widget manipulation functions
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2003-2007  László Pere <pipas@linux.pte.hu>
 * Copyright (C) 2011-2012  Thunor <thunorsif@hotmail.com>
 * Copyright (C) 2026       Artur Bednarek <artur@unix.org.pl>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* 
**
** $Id: widgets.c,v 1.2 2004/11/25 19:11:12 pipas Exp pipas $
** $Log: widgets.c,v $
** Revision 1.2  2004/11/25 19:11:12  pipas
** Small enhancements.
**
** Revision 1.1  2004/11/18 16:47:05  root
** Initial revision
**
 */

#include <gtk/gtk.h>
#include <errno.h>
#include <math.h>
#ifndef G_OS_WIN32
#include <sys/wait.h>
#endif
#include "gtkdialog.h"
#include "widgets.h"
#include "stringman.h"
#include "widget_accellabel.h"
#include "widget_arrow.h"
#include "widget_alignment.h"
#include "widget_assistant.h"
#include "widget_aspectframe.h"
#include "widget_button.h"
#include "widget_buttonbox.h"
#include "widget_calendar.h"
#include "widget_cellview.h"
#include "widget_checkbox.h"
#include "widget_chooser.h"
#include "widget_colorbutton.h"
#include "widget_colorselection.h"
#include "widget_combobox.h"
#include "widget_comboboxtext.h"
#include "widget_curve.h"
#include "widget_dialog.h"
#include "widget_dock.h"
#include "widget_drawingarea.h"
#include "widget_imageview.h"
#include "widget_edit.h"
#include "widget_entry.h"
#include "widget_eventbox.h"
#include "widget_expander.h"
#include "widget_filechooserbutton.h"
#include "widget_filechooserdialog.h"
#include "widget_fixed.h"
#include "widget_fontbutton.h"
#include "widget_fontselection.h"
#include "widget_frame.h"
#include "widget_grid.h"
#include "widget_handlebox.h"
#include "widget_hbox.h"
#include "widget_hscale.h"
#include "widget_hseparator.h"
#include "widget_hsv.h"
#include "widget_ruler.h"
#include "widget_iconview.h"
#include "widget_infobar.h"
#include "widget_layout.h"
#include "widget_linkbutton.h"
#include "widget_list.h"
#include "widget_messagedialog.h"
#include "widget_menubar.h"
#include "widget_menuitem.h"
#include "widget_notebook.h"
#include "widget_paned.h"
#include "widget_pixmap.h"
#include "widget_progressbar.h"
#include "widget_radiobutton.h"
#include "widget_recentchooser.h"
#include "widget_scalebutton.h"
#include "widget_scrollbar.h"
#include "widget_scrolledwindow.h"
#include "widget_sheet.h"
#include "widget_socket.h"
#include "widget_statusicon.h"
#include "widget_spinbutton.h"
#include "widget_spinner.h"
#include "widget_statusbar.h"
#include "widget_table.h"
#include "widget_tasklist.h"
#include "widget_pager.h"
#include "widget_windowselector.h"
#include "widget_offscreenwindow.h"
#include "widget_pagesetupdialog.h"
#include "widget_printdialog.h"
#include "widget_terminal.h"
#include "widget_text.h"
#include "widget_timer.h"
#include "widget_toolbar.h"
#include "widget_toolpalette.h"
#include "widget_tree.h"
#include "widget_vbox.h"
#include "widget_viewport.h"
#include "widget_window.h"
#include "signals.h"
#include "tag_attributes.h"
#include "macros.h"

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS
#undef DEBUG
#undef WARNING

extern gchar *option_include_file;

static gboolean widget_try_parse_bounded_integer(const gchar *value,
	gint64 minimum, gint64 maximum, gint *result)
{
	const gchar *start;
	gchar *end;
	gint64 number;

	start = value;
	while (g_ascii_isspace(*start))
		++start;
	errno = 0;
	number = g_ascii_strtoll(start, &end, 10);
	while (g_ascii_isspace(*end))
		++end;

	if (errno != 0 || end == start || *end != '\0' ||
		number < minimum || number > maximum)
		return FALSE;

	*result = (gint)number;
	return TRUE;
}

gint widget_parse_bounded_integer(const gchar *value, gint minimum,
	gint maximum, gint fallback, const gchar *attribute)
{
	gint number;

	if (value == NULL)
		return fallback;

	if (!widget_try_parse_bounded_integer(value, minimum, maximum, &number)) {
		gtkdialog_warning("Invalid %s value '%s'; using %d (valid range is %d..%d).",
			attribute, value, fallback, minimum, maximum);
		return fallback;
	}

	return number;
}

gboolean widget_parse_finite_double(const gchar *value, gdouble *result,
	const gchar *attribute)
{
	const gchar *start;
	gchar *end;
	gdouble number;

	if (value == NULL || result == NULL)
		return FALSE;

	start = value;
	while (g_ascii_isspace(*start))
		++start;
	errno = 0;
	number = g_ascii_strtod(start, &end);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == start || *end != '\0' || !isfinite(number)) {
		gtkdialog_warning("Invalid %s value '%s'; ignored.", attribute, value);
		return FALSE;
	}

	*result = number;
	return TRUE;
}

gboolean widget_parse_boolean(const gchar *value, gboolean *result)
{
	const gchar *start;
	gchar *end;
	gint64 number;

	if (value == NULL || result == NULL)
		return FALSE;

	if (strcasecmp(value, "true") == 0 || strcasecmp(value, "yes") == 0) {
		*result = TRUE;
		return TRUE;
	}
	if (strcasecmp(value, "false") == 0 || strcasecmp(value, "no") == 0) {
		*result = FALSE;
		return TRUE;
	}

	start = value;
	while (g_ascii_isspace(*start))
		++start;
	errno = 0;
	number = g_ascii_strtoll(start, &end, 10);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == start || *end != '\0')
		return FALSE;

	*result = number != 0;
	return TRUE;
}

WidgetCompletionMatch widget_parse_completion_match(const gchar *value,
	const gchar *attribute)
{
	if (value == NULL || strcasecmp(value, "prefix") == 0)
		return WIDGET_COMPLETION_MATCH_PREFIX;
	if (strcasecmp(value, "contains") == 0)
		return WIDGET_COMPLETION_MATCH_CONTAINS;

	gtkdialog_warning("Invalid %s value '%s'; using prefix.", attribute,
		value);
	return WIDGET_COMPLETION_MATCH_PREFIX;
}

gboolean widget_parse_completion_case_sensitive(const gchar *value,
	const gchar *attribute)
{
	gboolean case_sensitive = FALSE;

	if (value != NULL && !widget_parse_boolean(value, &case_sensitive)) {
		gtkdialog_warning("Invalid %s value '%s'; using false.", attribute,
			value);
		case_sensitive = FALSE;
	}
	return case_sensitive;
}

typedef struct {
	gint text_column;
	WidgetCompletionMatch match;
	gboolean case_sensitive;
} WidgetCompletionMatchData;

static gchar *widget_completion_normalize(const gchar *text,
	gboolean case_sensitive)
{
	gchar *folded;
	gchar *normalized;

	normalized = g_utf8_normalize(text != NULL ? text : "", -1,
		G_NORMALIZE_ALL);
	if (normalized == NULL)
		normalized = g_strdup(text != NULL ? text : "");
	if (case_sensitive)
		return normalized;

	folded = g_utf8_casefold(normalized, -1);
	g_free(normalized);
	return folded != NULL ? folded : g_strdup("");
}

static gboolean widget_entry_completion_match_func(
	GtkEntryCompletion *completion, const gchar *key, GtkTreeIter *iter,
	gpointer user_data)
{
	WidgetCompletionMatchData *data = user_data;
	GtkTreeModel *model;
	GtkWidget *entry;
	const gchar *entry_text;
	gchar *candidate;
	gchar *normalized_candidate;
	gchar *normalized_key;
	gboolean matches;

	(void)key;
	model = gtk_entry_completion_get_model(completion);
	entry = gtk_entry_completion_get_entry(completion);
	if (model == NULL || !GTK_IS_ENTRY(entry))
		return FALSE;

	gtk_tree_model_get(model, iter, data->text_column, &candidate, -1);
	entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
	normalized_candidate = widget_completion_normalize(candidate,
		data->case_sensitive);
	normalized_key = widget_completion_normalize(entry_text,
		data->case_sensitive);
	if (data->match == WIDGET_COMPLETION_MATCH_CONTAINS)
		matches = strstr(normalized_candidate, normalized_key) != NULL;
	else
		matches = g_str_has_prefix(normalized_candidate, normalized_key);

	g_free(candidate);
	g_free(normalized_candidate);
	g_free(normalized_key);
	return matches;
}

void widget_entry_completion_set_matching(GtkEntryCompletion *completion,
	gint text_column, WidgetCompletionMatch match, gboolean case_sensitive)
{
	WidgetCompletionMatchData *data;

	/* Leave GTK's established matching path untouched for the default. */
	if (match == WIDGET_COMPLETION_MATCH_PREFIX && !case_sensitive)
		return;

	data = g_new(WidgetCompletionMatchData, 1);
	data->text_column = text_column;
	data->match = match;
	data->case_sensitive = case_sensitive;
	gtk_entry_completion_set_match_func(completion,
		widget_entry_completion_match_func, data, g_free);
}

gboolean widget_parse_legacy_input_boolean(const gchar *value)
{
	const gchar *start;
	gchar *end;
	gint64 number;

	if (value == NULL)
		return FALSE;
	if (strcasecmp(value, "true") == 0 || strcasecmp(value, "yes") == 0)
		return TRUE;

	/* Classic state-taking widget inputs used atoi(value) == 1.  Retain
	 * numeric-prefix handling without atoi's undefined overflow behaviour. */
	start = value;
	while (g_ascii_isspace(*start))
		++start;
	errno = 0;
	number = g_ascii_strtoll(start, &end, 10);
	return errno == 0 && end != start && number == 1;
}

gboolean widget_attribute_is_true(const gchar *value)
{
	return value != NULL && (strcasecmp(value, "true") == 0 ||
		strcasecmp(value, "yes") == 0 || strcmp(value, "1") == 0);
}

gboolean widget_attribute_is_false(const gchar *value)
{
	return value != NULL && (strcasecmp(value, "false") == 0 ||
		strcasecmp(value, "no") == 0 || strcmp(value, "0") == 0);
}

gint widget_parse_size(const gchar *value, const gchar *attribute)
{
	gint size;

	if (value == NULL)
		return -1;

	if (!widget_try_parse_bounded_integer(value, -1, G_MAXINT, &size) ||
		size == 0) {
		gtkdialog_warning("Invalid %s value '%s'; using -1.", attribute, value);
		return -1;
	}

	return size;
}

gint widget_parse_size_request(const gchar *value, const gchar *attribute)
{
	return widget_parse_bounded_integer(value, -1, G_MAXINT, -1, attribute);
}

GtkSelectionMode widget_parse_selection_mode(const gchar *value,
	gboolean allow_none, const gchar *attribute)
{
	gint mode;

	if (value == NULL)
		return GTK_SELECTION_SINGLE;

	if (strcasecmp(value, "single") == 0)
		return GTK_SELECTION_SINGLE;
	if (strcasecmp(value, "browse") == 0)
		return GTK_SELECTION_BROWSE;
	if (strcasecmp(value, "multiple") == 0)
		return GTK_SELECTION_MULTIPLE;
	if (allow_none && strcasecmp(value, "none") == 0)
		return GTK_SELECTION_NONE;

	if (!widget_try_parse_bounded_integer(value,
		allow_none ? GTK_SELECTION_NONE : GTK_SELECTION_SINGLE,
		GTK_SELECTION_MULTIPLE, &mode)) {
		gtkdialog_warning("Invalid %s value '%s'; using single (1).",
			attribute, value);
		return GTK_SELECTION_SINGLE;
	}

	return mode;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gint widget_parse_column_index(const gchar *value, gint n_columns,
	const gchar *attribute)
{
	gint column;

	if (value == NULL)
		return 0;

	if (!widget_try_parse_bounded_integer(value, 0, n_columns - 1, &column)) {
		gtkdialog_warning("Invalid %s value '%s'; using column 0 "
			"(valid range is 0..%d).", attribute, value,
			n_columns - 1);
		return 0;
	}

	return column;
}

gint widget_parse_sort_type(const gchar *value, const gchar *attribute)
{
	gint sort_type;

	if (value == NULL)
		return GTK_SORT_ASCENDING;

	if (!widget_try_parse_bounded_integer(value, GTK_SORT_ASCENDING,
		GTK_SORT_DESCENDING, &sort_type)) {
		gtkdialog_warning("Invalid %s value '%s'; using ascending (0).",
			attribute, value);
		return GTK_SORT_ASCENDING;
	}

	return sort_type;
}

gint widget_parse_stock_icon_size(const gchar *value, gint fallback,
	const gchar *attribute)
{
	gint icon_size;

	if (value == NULL)
		return fallback;

	if (!widget_try_parse_bounded_integer(value, -1, GTK_ICON_SIZE_DIALOG,
		&icon_size) || icon_size == GTK_ICON_SIZE_INVALID) {
		gtkdialog_warning("Invalid %s value '%s'; using %d.",
			attribute, value, fallback);
		return fallback;
	}

	return icon_size;
}

gint widget_parse_nonnegative_integer(const gchar *value, gint fallback,
	const gchar *attribute)
{
	gint number;

	if (value == NULL)
		return fallback;

	if (!widget_try_parse_bounded_integer(value, 0, G_MAXINT, &number)) {
		gtkdialog_warning("Invalid %s value '%s'; using %d.",
			attribute, value, fallback);
		return fallback;
	}

	return number;
}

/* Report and consume an error returned by a GTK icon-loading function. */
void widget_report_icon_error(const gchar *icon_name, GError *error)
{
	if (error == NULL)
		return;

	gtkdialog_warning("Couldn't load icon '%s': %s",
		icon_name ? icon_name : "(unknown)", error->message);
	g_error_free(error);
}

GdkPixbuf *widget_load_pixbuf(GtkWidget *widget, const gchar *stock_name,
	const gchar *icon_name, const gchar *image_name, gint icon_size)
{
	GError *error = NULL;
	GdkPixbuf *pixbuf = NULL;
	GdkPixbuf *scaled;
	gchar *filename;

	if (stock_name != NULL && stock_name[0] != '\0') {
		pixbuf = gtk_widget_render_icon(widget, stock_name,
			GTK_ICON_SIZE_DIALOG, NULL);
		if (pixbuf == NULL) {
			gtkdialog_warning("Couldn't load stock icon '%s'.", stock_name);
		} else if (gdk_pixbuf_get_width(pixbuf) != icon_size ||
			gdk_pixbuf_get_height(pixbuf) != icon_size) {
			scaled = gdk_pixbuf_scale_simple(pixbuf, icon_size, icon_size,
				GDK_INTERP_BILINEAR);
			g_object_unref(pixbuf);
			pixbuf = scaled;
		}
	} else if (icon_name != NULL && icon_name[0] != '\0') {
		pixbuf = gtk_icon_theme_load_icon(gtk_icon_theme_get_default(),
			icon_name, icon_size, 0, &error);
		if (pixbuf == NULL)
			widget_report_icon_error(icon_name, error);
	} else if (image_name != NULL && image_name[0] != '\0') {
		filename = find_pixmap(image_name);
		pixbuf = gdk_pixbuf_new_from_file_at_size(filename,
			icon_size, icon_size, &error);
		if (pixbuf == NULL)
			widget_report_icon_error(filename, error);
		g_free(filename);
	}
	return pixbuf;
}

gchar *widget_read_line(FILE *stream)
{
	GString *line;
	gboolean have_input = FALSE;
	gint character;

	g_return_val_if_fail(stream != NULL, NULL);

	line = g_string_sized_new(128);
	errno = 0;
	while ((character = fgetc(stream)) != EOF) {
		have_input = TRUE;
		if (character == '\n')
			break;
		g_string_append_c(line, character);
	}
	if (character == EOF && ferror(stream))
		gtkdialog_warning("Couldn't read widget input: %s",
			errno ? g_strerror(errno) : "I/O error");

	if (!have_input) {
		g_string_free(line, TRUE);
		return NULL;
	}

	while (line->len > 0 &&
		(line->str[line->len - 1] == '\r' || line->str[line->len - 1] == '\n'))
		g_string_truncate(line, line->len - 1);

	return g_string_free(line, FALSE);
}

gchar *widget_read_all(FILE *stream)
{
	GString *contents;
	gchar buffer[4096];
	size_t count;

	g_return_val_if_fail(stream != NULL, NULL);

	contents = g_string_sized_new(512);
	errno = 0;
	while ((count = fread(buffer, 1, sizeof(buffer), stream)) > 0)
		g_string_append_len(contents, buffer, count);
	if (ferror(stream))
		gtkdialog_warning("Couldn't read widget input: %s",
			errno ? g_strerror(errno) : "I/O error");

	return g_string_free(contents, FALSE);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

char *widget_get_text_value(GtkWidget *widget, int type)
{
	gchar            *string;

#ifdef DEBUG
	g_message("%s(): type: %08x\n", __func__, type);
#endif
	if (widget == NULL) {
		gtkdialog_warning("%s(): NULL Widget", __func__);
		return g_strdup("NULL WIDGET");
	}

	switch (type) {
		case WIDGET_ABOUTDIALOG:
			string = widget_dialog_envvar_construct(widget);
			return string;
			break;
		case WIDGET_ALIGNMENT:
			string = widget_alignment_envvar_construct(widget);
			return string;
			break;
		case WIDGET_ASPECTFRAME:
			string = widget_aspectframe_envvar_construct(widget);
			return string;
			break;
		case WIDGET_ASSISTANT:
			string = widget_assistant_envvar_construct(widget);
			return string;
			break;
		case WIDGET_CANCELBUTTON:
		case WIDGET_HELPBUTTON:
		case WIDGET_NOBUTTON:
		case WIDGET_OKBUTTON:
		case WIDGET_YESBUTTON:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_BUTTON:
			string = widget_button_envvar_construct(widget);
			return string;
			break;
		case WIDGET_CALENDAR:
			string = widget_calendar_envvar_construct(widget);
			return string;
			break;
		case WIDGET_ACCELLABEL:
			string = widget_accellabel_envvar_construct(widget);
			return string;
			break;
		case WIDGET_ARROW:
			string = widget_arrow_envvar_construct(widget);
			return string;
			break;
		case WIDGET_CELLVIEW:
			string = widget_cellview_envvar_construct(widget);
			return string;
			break;
		case WIDGET_CHECKBOX:
			string = widget_checkbox_envvar_construct(widget);
			return string;
			break;
		case WIDGET_COLORBUTTON:
			string = widget_colorbutton_envvar_construct(widget);
			return string;
			break;
		case WIDGET_COLORSELECTION:
			string = widget_colorselection_envvar_construct(widget);
			return string;
		case WIDGET_DIALOG:
			string = widget_dialog_envvar_construct(widget);
			return string;
			break;
		case WIDGET_DRAWINGAREA:
			string = widget_drawingarea_envvar_construct(widget);
			return string;
			break;
		case WIDGET_SHEET:
			return widget_sheet_envvar_construct(widget);
		case WIDGET_DOCK:
		case WIDGET_DOCKITEM:
			return widget_dock_envvar_construct(widget);
		case WIDGET_IMAGEVIEW:
			string = widget_imageview_envvar_construct(widget);
			return string;
			break;
		case WIDGET_COMBOBOX:
			string = widget_combobox_envvar_construct(widget);
			return string;
			break;
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
			string = widget_comboboxtext_envvar_construct(widget);
			return string;
			break;
		case WIDGET_EDIT:
			string = widget_edit_envvar_construct(widget);
			return string;
			break;
		case WIDGET_ENTRY:
			string = widget_entry_envvar_construct(widget);
			return string;
			break;
		case WIDGET_EVENTBOX:
			string = widget_eventbox_envvar_construct(widget);
			return string;
			break;
		case WIDGET_EXPANDER:
			string = widget_expander_envvar_construct(widget);
			return string;
			break;
		case WIDGET_FILECHOOSERBUTTON:
			string = widget_filechooserbutton_envvar_construct(widget);
			return string;
			break;
		case WIDGET_FIXED:
			string = widget_fixed_envvar_construct(widget);
			return string;
			break;
		case WIDGET_FONTBUTTON:
			string = widget_fontbutton_envvar_construct(widget);
			return string;
			break;
		case WIDGET_FONTSELECTION:
			string = widget_fontselection_envvar_construct(widget);
			return string;
			break;
		case WIDGET_FRAME:
			string = widget_frame_envvar_construct(widget);
			return string;
			break;
		case WIDGET_GRID:
			string = widget_grid_envvar_construct(widget);
			return string;
			break;
		case WIDGET_HBOX:
			string = widget_hbox_envvar_construct(widget);
			return string;
			break;
		case WIDGET_HBUTTONBOX:
		case WIDGET_VBUTTONBOX:
			string = widget_buttonbox_envvar_construct(widget);
			return string;
			break;
		case WIDGET_HPANED:
		case WIDGET_VPANED:
			string = widget_paned_envvar_construct(widget);
			return string;
			break;
		case WIDGET_HSV:
			return widget_hsv_envvar_construct(widget);
			break;
		case WIDGET_TASKLIST:
			return widget_tasklist_envvar_construct(widget);
			break;
		case WIDGET_PAGER:
			return widget_pager_envvar_construct(widget);
			break;
		case WIDGET_WINDOWSELECTOR:
			return widget_windowselector_envvar_construct(widget);
			break;
		case WIDGET_OFFSCREENWINDOW:
			return widget_offscreenwindow_envvar_construct(widget);
			break;
		case WIDGET_PAGESETUPDIALOG:
			return widget_pagesetupdialog_envvar_construct(widget);
			break;
		case WIDGET_PRINTDIALOG:
			return widget_printdialog_envvar_construct(widget);
			break;
		case WIDGET_FILECHOOSERDIALOG:
			return widget_filechooserdialog_envvar_construct(widget);
			break;
		case WIDGET_CURVE:
			return widget_curve_envvar_construct(widget);
			break;
		case WIDGET_HRULER:
		case WIDGET_VRULER:
			return widget_ruler_envvar_construct(widget);
			break;
		case WIDGET_HSCALE:
		case WIDGET_VSCALE:
			string = widget_hscale_envvar_construct(widget);
			return string;
			break;
		case WIDGET_HSCROLLBAR:
		case WIDGET_VSCROLLBAR:
			string = widget_scrollbar_envvar_construct(widget);
			return string;
		case WIDGET_HSEPARATOR:
		case WIDGET_VSEPARATOR:
			string = widget_hseparator_envvar_construct(widget);
			return string;
			break;
		case WIDGET_ICONVIEW:
			string = widget_iconview_envvar_construct(widget);
			return string;
			break;
		case WIDGET_INFOBAR:
			string = widget_infobar_envvar_construct(widget);
			return string;
			break;
		case WIDGET_LAYOUT:
			string = widget_layout_envvar_construct(widget);
			return string;
			break;
		case WIDGET_LIST:
			string = widget_list_envvar_construct(widget);
			return string;
			break;
		case WIDGET_MESSAGEDIALOG:
			string = widget_messagedialog_envvar_construct(widget);
			return string;
			break;
		case WIDGET_LINKBUTTON:
			string = widget_linkbutton_envvar_construct(widget);
			return string;
			break;
		case WIDGET_MENUBAR:
		case WIDGET_POPUPMENU:
			string = widget_menubar_envvar_construct(widget);
			return string;
			break;
		case WIDGET_MENUITEMSEPARATOR:
		case WIDGET_MENUITEM:
		case WIDGET_MENU:
			string = widget_menuitem_envvar_construct(widget);
			return string;
			break;
		case WIDGET_NOTEBOOK:
			string = widget_notebook_envvar_construct(widget);
			return string;
			break;
		case WIDGET_PIXMAP:
			string = widget_pixmap_envvar_construct(widget);
			return string;
			break;
		case WIDGET_PROGRESSBAR:
			string = widget_progressbar_envvar_construct(widget);
			return string;
			break;
		case WIDGET_RADIOBUTTON:
			string = widget_radiobutton_envvar_construct(widget);
			return string;
			break;
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_RECENTCHOOSER:
			string = widget_recentchooser_envvar_construct(widget);
			return string;
			break;
		case WIDGET_SCALEBUTTON:
		case WIDGET_VOLUMEBUTTON:
			string = widget_scalebutton_envvar_construct(widget);
			return string;
			break;
		case WIDGET_SCROLLEDWINDOW:
			string = widget_scrolledwindow_envvar_construct(widget);
			return string;
			break;
		case WIDGET_VIEWPORT:
			string = widget_viewport_envvar_construct(widget);
			return string;
		case WIDGET_HANDLEBOX:
			string = widget_handlebox_envvar_construct(widget);
			return string;
			break;
		case WIDGET_TOOLPALETTE:
		case WIDGET_TOOLITEMGROUP:
			string = widget_toolpalette_envvar_construct(widget);
			return string;
			break;
		case WIDGET_SPINBUTTON:
			string = widget_spinbutton_envvar_construct(widget);
			return string;
			break;
		case WIDGET_SPINNER:
			string = widget_spinner_envvar_construct(widget);
			return string;
			break;
		case WIDGET_SOCKET:
			return widget_socket_envvar_construct(widget);
			break;
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOOLBAR:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
			string = widget_toolbar_envvar_construct(widget);
			return string;
			break;
		case WIDGET_STATUSBAR:
			string = widget_statusbar_envvar_construct(widget);
			return string;
			break;
		case WIDGET_STATUSICON:
			string = widget_statusicon_envvar_construct(widget);
			return string;
			break;
		case WIDGET_TABLE:
			string = widget_table_envvar_construct(widget);
			return string;
			break;
		case WIDGET_TERMINAL:
			string = widget_terminal_envvar_construct(widget);
			return string;
			break;
		case WIDGET_TEXT:
			string = widget_text_envvar_construct(widget);
			return string;
			break;
		case WIDGET_TIMER:
			string = widget_timer_envvar_construct(widget);
			return string;
			break;
		case WIDGET_TREE:
			string = widget_tree_envvar_construct(widget);
			return string;
			break;
		case WIDGET_VBOX:
			string = widget_vbox_envvar_construct(widget);
			return string;
			break;
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			string = widget_window_envvar_construct(widget);
			return string;
			break;


		case WIDGET_CHOOSER:
			return widget_chooser_envvar_construct(widget);
			break;


		default:
			return NULL;
	}
	g_error("%s(): this should not be reached", __func__);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

FILE *widget_opencommand(const char *command)
{
	char *the_line;
	char *quoted_include;
	FILE *infile;

	PIP_DEBUG("Opening command: '%s'", command);

	if (option_include_file != NULL) {
		/* Debian 01_bashism patch: use dot rather than source.
		the_line = g_strdup_printf("source %s; %s", */
		quoted_include = g_shell_quote(option_include_file);
		the_line = g_strdup_printf(". %s; %s", quoted_include, command);
		infile = popen(the_line, "r");
		g_free(quoted_include);
		g_free(the_line);
	} else {
		infile = popen(command, "r");
	}
	
	if (infile == NULL)
		gtkdialog_warning("%s(): %s", __func__, g_strerror(errno));
	
	return infile;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void widget_report_shell_status(const gchar *command, gint status)
{
	const gchar *description = command ? command : "(unknown command)";

#ifndef G_OS_WIN32
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		gtkdialog_warning("Shell command exited with status %d: %s",
			WEXITSTATUS(status), description);
	else if (WIFSIGNALED(status))
		gtkdialog_warning("Shell command terminated by signal %d: %s",
			WTERMSIG(status), description);
#else
	(void)status;
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gint widget_closecommand(FILE *stream, const gchar *command)
{
	gint status;
	const gchar *description = command ? command : "(unknown command)";

	g_return_val_if_fail(stream != NULL, -1);

	status = pclose(stream);
	if (status == -1) {
		gtkdialog_warning("Couldn't collect shell command '%s': %s",
			description, g_strerror(errno));
		return status;
	}
	widget_report_shell_status(description, status);

	return status;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gint widget_close_output(FILE *stream, const gchar *filename)
{
	gint status;
	const gchar *description = filename ? filename : "(unknown file)";

	g_return_val_if_fail(stream != NULL, EOF);

	status = fclose(stream);
	if (status == EOF)
		gtkdialog_warning("Couldn't finish writing file '%s': %s",
			description, errno ? g_strerror(errno) : "I/O error");

	return status;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

char *widgets_to_str(int itype)
{
	char *type;
	switch (itype) {
		case WIDGET_ABOUTDIALOG:
			type = "ABOUTDIALOG";
			break;
		case WIDGET_ALIGNMENT:
			type = "ALIGNMENT";
			break;
		case WIDGET_ASPECTFRAME:
			type = "ASPECTFRAME";
			break;
		case WIDGET_ASSISTANT:
			type = "ASSISTANT";
			break;
		case WIDGET_CANCELBUTTON:
			type = "CANCELBUTTON";
			break;
		case WIDGET_HELPBUTTON:
			type = "HELPBUTTON";
			break;
		case WIDGET_NOBUTTON:
			type = "NOBUTTON";
			break;
		case WIDGET_OKBUTTON:
			type = "OKBUTTON";
			break;
		case WIDGET_YESBUTTON:
			type = "YESBUTTON";
			break;
		case WIDGET_BUTTON:
			type = "BUTTON";
			break;
		case WIDGET_CALENDAR:
			type = "CALENDAR";
			break;
		case WIDGET_ACCELLABEL:
			type = "ACCELLABEL";
			break;
		case WIDGET_ARROW:
			type = "ARROW";
			break;
		case WIDGET_CELLVIEW:
			type = "CELLVIEW";
			break;
		case WIDGET_CHECKBOX:
			type = "CHECKBOX";
			break;
		case WIDGET_COLORBUTTON:
			type = "COLORBUTTON";
			break;
		case WIDGET_COLORSELECTION:
			type = "COLORSELECTION";
			break;
		case WIDGET_DIALOG:
			type = "DIALOG";
			break;
		case WIDGET_DRAWINGAREA:
			type = "DRAWINGAREA";
			break;
		case WIDGET_SHEET:
			type = "SHEET";
			break;
		case WIDGET_DOCK:
			type = "DOCK";
			break;
		case WIDGET_DOCKITEM:
			type = "DOCKITEM";
			break;
		case WIDGET_IMAGEVIEW:
			type = "IMAGEVIEW";
			break;
		case WIDGET_COMBOBOX:
			type = "COMBOBOX";
			break;
		case WIDGET_COMBOBOXENTRY:
			type = "COMBOBOXENTRY";
			break;
		case WIDGET_COMBOBOXTEXT:
			type = "COMBOBOXTEXT";
			break;
		case WIDGET_EDIT:
			type = "EDIT";
			break;
		case WIDGET_ENTRY:
			type = "ENTRY";
			break;
		case WIDGET_EVENTBOX:
			type = "EVENTBOX";
			break;
		case WIDGET_EXPANDER:
			type = "EXPANDER";
			break;
		case WIDGET_FILECHOOSERBUTTON:
			type = "FILECHOOSERBUTTON";
			break;
		case WIDGET_FIXED:
			type = "FIXED";
			break;
		case WIDGET_FONTBUTTON:
			type = "FONTBUTTON";
			break;
		case WIDGET_FONTSELECTION:
			type = "FONTSELECTION";
			break;
		case WIDGET_FRAME:
			type = "FRAME";
			break;
		case WIDGET_GVIM:
			type = "GVIM";
			break;
		case WIDGET_GRID:
			type = "GRID";
			break;
		case WIDGET_HBOX:
			type = "HBOX";
			break;
		case WIDGET_HBUTTONBOX:
			type = "HBUTTONBOX";
			break;
		case WIDGET_HPANED:
			type = "HPANED";
			break;
		case WIDGET_HSV:
			type = "HSV";
			break;
		case WIDGET_TASKLIST:
			type = "TASKLIST";
			break;
		case WIDGET_PAGER:
			type = "PAGER";
			break;
		case WIDGET_WINDOWSELECTOR:
			type = "WINDOWSELECTOR";
			break;
		case WIDGET_OFFSCREENWINDOW:
			type = "OFFSCREENWINDOW";
			break;
		case WIDGET_PAGESETUPDIALOG:
			type = "PAGESETUPDIALOG";
			break;
		case WIDGET_PRINTDIALOG:
			type = "PRINTDIALOG";
			break;
		case WIDGET_FILECHOOSERDIALOG:
			type = "FILECHOOSERDIALOG";
			break;
		case WIDGET_CURVE:
			type = "CURVE";
			break;
		case WIDGET_HRULER:
			type = "HRULER";
			break;
		case WIDGET_VRULER:
			type = "VRULER";
			break;
		case WIDGET_HSCALE:
			type = "HSCALE";
			break;
		case WIDGET_HSCROLLBAR:
			type = "HSCROLLBAR";
			break;
		case WIDGET_HSEPARATOR:
			type = "HSEPARATOR";
			break;
		case WIDGET_ICONVIEW:
			type = "ICONVIEW";
			break;
		case WIDGET_INFOBAR:
			type = "INFOBAR";
			break;
		case WIDGET_LAYOUT:
			type = "LAYOUT";
			break;
		case WIDGET_LIST:
			type = "LIST";
			break;
		case WIDGET_LINKBUTTON:
			type = "LINKBUTTON";
			break;
		case WIDGET_MENU:
			type = "MENU";
			break;
		case WIDGET_POPUPMENU:
			type = "POPUPMENU";
			break;
		case WIDGET_MENUBAR:
			type = "MENUBAR";
			break;
		case WIDGET_MENUITEM:
			type = "MENUITEM";
			break;
		case WIDGET_MENUITEMSEPARATOR:
			type = "MENUITEMSEPARATOR";
			break;
		case WIDGET_MESSAGEDIALOG:
			type = "MESSAGEDIALOG";
			break;
		case WIDGET_NOTEBOOK:
			type = "NOTEBOOK";
			break;
		case WIDGET_PIXMAP:
			type = "PIXMAP";
			break;
		case WIDGET_PROGRESSBAR:
			type = "PROGRESSBAR";
			break;
		case WIDGET_RADIOBUTTON:
			type = "RADIOBUTTON";
			break;
		case WIDGET_RECENTCHOOSER:
			type = "RECENTCHOOSER";
			break;
		case WIDGET_RECENTCHOOSERMENU:
			type = "RECENTCHOOSERMENU";
			break;
		case WIDGET_SCALEBUTTON:
			type = "SCALEBUTTON";
			break;
		case WIDGET_SCROLLEDWINDOW:
			type = "SCROLLEDWINDOW";
			break;
		case WIDGET_HANDLEBOX:
			type = "HANDLEBOX";
			break;
		case WIDGET_TOOLPALETTE:
			type = "TOOLPALETTE";
			break;
		case WIDGET_TOOLITEMGROUP:
			type = "TOOLITEMGROUP";
			break;
		case WIDGET_SPINBUTTON:
			type = "SPINBUTTON";
			break;
		case WIDGET_SPINNER:
			type = "SPINNER";
			break;
		case WIDGET_SOCKET:
			type = "SOCKET";
			break;
		case WIDGET_SEPARATORTOOLITEM:
			type = "SEPARATORTOOLITEM";
			break;
		case WIDGET_STATUSBAR:
			type = "STATUSBAR";
			break;
		case WIDGET_STATUSICON:
			type = "STATUSICON";
			break;
		case WIDGET_TABLE:
			type = "TABLE";
			break;
		case WIDGET_TERMINAL:
			type = "TERMINAL";
			break;
		case WIDGET_TEXT:
			type = "TEXT";
			break;
		case WIDGET_TIMER:
			type = "TIMER";
			break;
		case WIDGET_TOOLBAR:
			type = "TOOLBAR";
			break;
		case WIDGET_TOOLBUTTON:
			type = "TOOLBUTTON";
			break;
		case WIDGET_TOOLITEM:
			type = "TOOLITEM";
			break;
		case WIDGET_TOGGLETOOLBUTTON:
			type = "TOGGLETOOLBUTTON";
			break;
		case WIDGET_RADIOTOOLBUTTON:
			type = "RADIOTOOLBUTTON";
			break;
		case WIDGET_MENUTOOLBUTTON:
			type = "MENUTOOLBUTTON";
			break;
		case WIDGET_TOGGLEBUTTON:
			type = "TOGGLEBUTTON";
			break;
		case WIDGET_TREE:
			type = "TREE";
			break;
		case WIDGET_VBOX:
			type = "VBOX";
			break;
		case WIDGET_VOLUMEBUTTON:
			type = "VOLUMEBUTTON";
			break;
		case WIDGET_VBUTTONBOX:
			type = "VBUTTONBOX";
			break;
		case WIDGET_VPANED:
			type = "VPANED";
			break;
		case WIDGET_VIEWPORT:
			type = "VIEWPORT";
			break;
		case WIDGET_VSCALE:
			type = "VSCALE";
			break;
		case WIDGET_VSCROLLBAR:
			type = "VSCROLLBAR";
			break;
		case WIDGET_VSEPARATOR:
			type = "VSEPARATOR";
			break;
		case WIDGET_PLUG:
			type = "PLUG";
			break;
		case WIDGET_WINDOW:
			type = "WINDOW";
			break;


	case WIDGET_SCROLLEDW:
		type = "SCROLLEDW";
		break;
	default:
		type = "THINGY";
	}
	return (type);
}

/***********************************************************************
 *  Widget Connect Signals                                             *
 ***********************************************************************/

typedef struct {
	GtkWidget *owner;
	AttributeSet *attributes;
	gulong owner_destroy_handler_id;
} WidgetInputSignalProxy;

static void widget_input_signal_proxy_owner_destroyed(GtkWidget *owner,
	gpointer data)
{
	WidgetInputSignalProxy *proxy = data;

	if (proxy->owner != NULL)
		g_object_remove_weak_pointer(G_OBJECT(owner),
			(gpointer *)&proxy->owner);
	proxy->owner = NULL;
	proxy->owner_destroy_handler_id = 0;
}

static gboolean widget_input_signal_proxy_free_idle(gpointer data)
{
	WidgetInputSignalProxy *proxy = data;

	if (proxy->owner != NULL) {
		if (proxy->owner_destroy_handler_id != 0)
			g_signal_handler_disconnect(proxy->owner,
				proxy->owner_destroy_handler_id);
		g_object_remove_weak_pointer(G_OBJECT(proxy->owner),
			(gpointer *)&proxy->owner);
	}
	g_free(proxy);
	return FALSE;
}

static void widget_input_signal_proxy_destroy(gpointer data)
{
	/* An input action may destroy its own window while this proxy callback is
	 * still on the stack. Keep the closure data alive until GTK returns to
	 * the main loop, just like the associated AttributeSet. */
	g_idle_add(widget_input_signal_proxy_free_idle, data);
}

static gboolean widget_input_signal_proxy_event(GtkWidget *event_widget,
	GdkEvent *event, gpointer data)
{
	WidgetInputSignalProxy *proxy = data;

	(void)event_widget;
	if (proxy->owner == NULL || event == NULL)
		return FALSE;

	switch (event->type) {
		case GDK_BUTTON_PRESS:
		case GDK_2BUTTON_PRESS:
		case GDK_3BUTTON_PRESS:
			return on_any_widget_button_pressed(proxy->owner,
				(GdkEventButton *)event, proxy->attributes);
		case GDK_BUTTON_RELEASE:
			return on_any_widget_button_released(proxy->owner,
				(GdkEventButton *)event, proxy->attributes);
		case GDK_KEY_PRESS:
			return on_any_widget_key_press_event(proxy->owner,
				(GdkEventKey *)event, proxy->attributes);
		case GDK_KEY_RELEASE:
			return on_any_widget_key_release_event(proxy->owner,
				(GdkEventKey *)event, proxy->attributes);
		case GDK_ENTER_NOTIFY:
			return on_any_widget_enter_notify_event(proxy->owner,
				(GdkEventCrossing *)event, proxy->attributes);
		case GDK_LEAVE_NOTIFY:
			return on_any_widget_leave_notify_event(proxy->owner,
				(GdkEventCrossing *)event, proxy->attributes);
		case GDK_FOCUS_CHANGE:
			if (((GdkEventFocus *)event)->in)
				return on_any_widget_focus_in_event(proxy->owner,
					(GdkEventFocus *)event, proxy->attributes);
			return on_any_widget_focus_out_event(proxy->owner,
				(GdkEventFocus *)event, proxy->attributes);
		default:
			return FALSE;
	}
}

static void widget_connect_forwarded_input_signals(GtkWidget *event_widget,
	GtkWidget *owner, AttributeSet *Attr)
{
	WidgetInputSignalProxy *proxy;

	if (g_object_get_data(G_OBJECT(event_widget),
		"gtkdialog-input-signal-proxy") != NULL)
		return;

	proxy = g_new0(WidgetInputSignalProxy, 1);
	proxy->owner = owner;
	proxy->attributes = Attr;
	g_object_add_weak_pointer(G_OBJECT(owner), (gpointer *)&proxy->owner);
	proxy->owner_destroy_handler_id = g_signal_connect(G_OBJECT(owner),
		"destroy", G_CALLBACK(widget_input_signal_proxy_owner_destroyed),
		proxy);
	g_object_set_data_full(G_OBJECT(event_widget),
		"gtkdialog-input-signal-proxy", proxy,
		widget_input_signal_proxy_destroy);

	/* GtkButton consumes some specific event signals in its class handler.
	 * The generic event signal runs first, so it provides one non-duplicating
	 * forwarding point for every input event. */
	g_signal_connect(G_OBJECT(event_widget), "event",
		G_CALLBACK(widget_input_signal_proxy_event), proxy);
}

static void widget_connect_tool_button_input_signals(GtkWidget *widget,
	GtkWidget *owner, AttributeSet *Attr)
{
	GList *children;
	GList *element;

	if (GTK_IS_BUTTON(widget)) {
		widget_connect_forwarded_input_signals(widget, owner, Attr);
		return;
	}
	if (!GTK_IS_CONTAINER(widget))
		return;

	children = gtk_container_get_children(GTK_CONTAINER(widget));
	for (element = children; element != NULL; element = element->next)
		widget_connect_tool_button_input_signals(GTK_WIDGET(element->data),
			owner, Attr);
	g_list_free(children);
}

static void widget_tool_button_parent_changed(GtkWidget *widget,
	GtkWidget *previous_parent, AttributeSet *Attr)
{
	(void)previous_parent;
	widget_connect_tool_button_input_signals(widget, widget, Attr);
}

static void widget_tool_button_realized(GtkWidget *widget, AttributeSet *Attr)
{
	widget_connect_tool_button_input_signals(widget, widget, Attr);
}

gboolean widget_connect_signals(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	g_return_val_if_fail(GTK_IS_WIDGET(widget), FALSE);

	PIP_DEBUG("Connecting signals for %p.", widget);

	/* GtkToolItem is only the logical XML widget. GTK+ delivers input to
	 * private GtkButton descendants, so forward those events while keeping
	 * the tool item as the action source. This is important for actions such
	 * as launch which inspect the originating widget. */
	if (GTK_IS_TOOL_BUTTON(widget)) {
		widget_connect_tool_button_input_signals(widget, widget, Attr);
		/* GtkMenuToolButton creates its private arrow button only when it is
		 * attached to a toolbar. Rescan after both parent and toplevel changes;
		 * the per-child marker prevents duplicate handlers. */
		g_signal_connect(G_OBJECT(widget), "parent-set",
			G_CALLBACK(widget_tool_button_parent_changed), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "hierarchy-changed",
			G_CALLBACK(widget_tool_button_parent_changed), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "realize",
			G_CALLBACK(widget_tool_button_realized), (gpointer)Attr);
	} else
	{
		g_signal_connect(G_OBJECT(widget), "event",
			G_CALLBACK(on_any_widget_context_event), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "button-press-event",
			G_CALLBACK(on_any_widget_button_pressed), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "button-release-event",
			G_CALLBACK(on_any_widget_button_released), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "key-press-event",
			G_CALLBACK(on_any_widget_key_press_event), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "key-release-event",
			G_CALLBACK(on_any_widget_key_release_event), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "enter-notify-event",
			G_CALLBACK(on_any_widget_enter_notify_event), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "leave-notify-event",
			G_CALLBACK(on_any_widget_leave_notify_event), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "focus-in-event",
			G_CALLBACK(on_any_widget_focus_in_event), (gpointer)Attr);
		g_signal_connect(G_OBJECT(widget), "focus-out-event",
			G_CALLBACK(on_any_widget_focus_out_event), (gpointer)Attr);
	}
	g_signal_connect(G_OBJECT(widget), "hide",
		G_CALLBACK(on_any_widget_hide), (gpointer)Attr);
	g_signal_connect(G_OBJECT(widget), "show",
		G_CALLBACK(on_any_widget_show), (gpointer)Attr);
	g_signal_connect(G_OBJECT(widget), "map-event",
		G_CALLBACK(on_any_widget_map_event), (gpointer)Attr);
	g_signal_connect(G_OBJECT(widget), "unmap-event",
		G_CALLBACK(on_any_widget_unmap_event), (gpointer)Attr);
	g_signal_connect(G_OBJECT(widget), "popup-menu",
		G_CALLBACK(on_any_widget_popup_menu), (gpointer)Attr);
	g_signal_connect(G_OBJECT(widget), "configure-event",
		G_CALLBACK(on_any_widget_configure_event), (gpointer)Attr);
	g_signal_connect(G_OBJECT(widget), "delete-event",
		G_CALLBACK(on_any_widget_delete_event), (gpointer)Attr);
	g_signal_connect(G_OBJECT(widget), "destroy-event",
		G_CALLBACK(on_any_widget_destroy_event), (gpointer)Attr);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return TRUE;
}

/***********************************************************************
 * Widget Visibility List Add                                          *
 ***********************************************************************/
/* Add a widget to the hide list or to the show list */

typedef struct {
	GtkWidget *widget;
	gulong destroy_handler_id;
} WidgetVisibilityEntry;

static void widget_visibility_destroyed(GtkWidget *widget, gpointer data)
{
	WidgetVisibilityEntry *entry = data;

	(void)widget;
	entry->widget = NULL;
	entry->destroy_handler_id = 0;
}

static void widget_visibility_entry_free(gpointer data, gpointer user_data)
{
	WidgetVisibilityEntry *entry = data;

	(void)user_data;
	if (entry->widget != NULL && entry->destroy_handler_id != 0)
		g_signal_handler_disconnect(entry->widget, entry->destroy_handler_id);
	g_free(entry);
}

void widget_visibility_list_add(GtkWidget *widget, tag_attr *attr)
{
	WidgetVisibilityEntry *entry;
	gchar            *value;
	gint              visible = TRUE;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (widget) {
		entry = g_new0(WidgetVisibilityEntry, 1);
		entry->widget = widget;
		entry->destroy_handler_id = g_signal_connect(widget, "destroy",
			G_CALLBACK(widget_visibility_destroyed), entry);
		if (attr) {
			if ((value = get_tag_attribute(attr, "visible"))) {
				if ((strcasecmp(value, "false") == 0) ||
					(strcasecmp(value, "no") == 0) ||
					(strcasecmp(value, "0") == 0)) {
					visible = FALSE;
				}
			}
		}
		if (visible) {
			widget_show_list = g_list_append(widget_show_list, entry);
		} else {
			widget_hide_list = g_list_append(widget_hide_list, entry);
		}
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Widget Show All                                                     *
 ***********************************************************************/
/* Show all widgets individually.
 * 
 * Visible widgets are simply shown.
 * 
 * Invisible widgets are shown and then immediately hidden to allocate
 * resources else widgets like checkboxes won't function until shown.
 */

void widget_show_all(void)
{
	WidgetVisibilityEntry *entry;
	GList            *element;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Show visible widgets */
	if (widget_show_list) {
		element = widget_show_list;
		while (element) {
			entry = element->data;
#ifdef DEBUG_CONTENT
			variable *debug_var = entry->widget == NULL ? NULL :
				find_variable_by_widget(entry->widget);

			fprintf(stderr, "%s(): showing Name='%s'\n",
				__func__, debug_var && debug_var->Name
					? debug_var->Name : "(unregistered)");
#endif
			if (entry->widget != NULL)
				gtk_widget_show(entry->widget);
			element = element->next;
		}
		/* Free the memory as we won't need it anymore */
		g_list_foreach(widget_show_list, widget_visibility_entry_free, NULL);
		g_list_free(widget_show_list);
		/* This may be reused so being initially NULL is important */
		widget_show_list = NULL;
	}

	/* Show and then immediately hide invisible widgets */
	if (widget_hide_list) {
		element = widget_hide_list;
		while (element) {
			entry = element->data;
#ifdef DEBUG_CONTENT
			variable *debug_var = entry->widget == NULL ? NULL :
				find_variable_by_widget(entry->widget);

			fprintf(stderr, "%s(): showing then hiding Name='%s'\n",
				__func__, debug_var && debug_var->Name
					? debug_var->Name : "(unregistered)");
#endif
			if (entry->widget != NULL) {
				gtk_widget_show(entry->widget);
				if (entry->widget != NULL)
					gtk_widget_hide(entry->widget);
			}
			element = element->next;
		}
		/* Free the memory as we won't need it anymore */
		g_list_foreach(widget_hide_list, widget_visibility_entry_free, NULL);
		g_list_free(widget_hide_list);
		/* This may be reused so being initially NULL is important */
		widget_hide_list = NULL;
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
