/*
 * widgets.h: Widget manipulation functions
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

#ifndef WIDGETS_H
#define WIDGETS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <gtk/gtk.h>
#include "gtkdialog.h"
#include "stack.h"
#include "attributes.h"
#include "stringman.h"
#include "variables.h"
#include "automaton.h"

typedef enum {
	WIDGET_COMPLETION_MATCH_PREFIX,
	WIDGET_COMPLETION_MATCH_CONTAINS
} WidgetCompletionMatch;

/* The caller owns the returned string and must release it with g_free(). */
char *widget_get_text_value(GtkWidget *widget, int type);
gint widget_parse_bounded_integer(const gchar *value, gint minimum,
	gint maximum, gint fallback, const gchar *attribute);
gboolean widget_parse_finite_double(const gchar *value, gdouble *result,
	const gchar *attribute);
gboolean widget_parse_boolean(const gchar *value, gboolean *result);
WidgetCompletionMatch widget_parse_completion_match(const gchar *value,
	const gchar *attribute);
gboolean widget_parse_completion_case_sensitive(const gchar *value,
	const gchar *attribute);
void widget_entry_completion_set_matching(GtkEntryCompletion *completion,
	gint text_column, WidgetCompletionMatch match, gboolean case_sensitive);
gboolean widget_parse_legacy_input_boolean(const gchar *value);
gboolean widget_attribute_is_true(const gchar *value);
gboolean widget_attribute_is_false(const gchar *value);
gint widget_parse_size(const gchar *value, const gchar *attribute);
gint widget_parse_size_request(const gchar *value, const gchar *attribute);
GtkSelectionMode widget_parse_selection_mode(const gchar *value,
	gboolean allow_none, const gchar *attribute);
gint widget_parse_column_index(const gchar *value, gint n_columns,
	const gchar *attribute);
gint widget_parse_sort_type(const gchar *value, const gchar *attribute);
gint widget_parse_stock_icon_size(const gchar *value, gint fallback,
	const gchar *attribute);
gint widget_parse_nonnegative_integer(const gchar *value, gint fallback,
	const gchar *attribute);
void widget_report_icon_error(const gchar *icon_name, GError *error);
GdkPixbuf *widget_load_pixbuf(GtkWidget *widget, const gchar *stock_name,
	const gchar *icon_name, const gchar *image_name, gint icon_size);
FILE *widget_opencommand(const char *command);
void widget_report_shell_status(const gchar *command, gint status);
gint widget_closecommand(FILE *stream, const gchar *command);
gint widget_close_output(FILE *stream, const gchar *filename);
gchar *widget_read_line(FILE *stream);
gchar *widget_read_all(FILE *stream);
char *widgets_to_str(int itype);
gboolean widget_connect_signals(GtkWidget *widget, AttributeSet *Attr);
void widget_file_monitor_try_create(variable *var, gchar *filename);
void widget_signal_executor(GtkWidget *widget, AttributeSet *Attr,
	const gchar *signal_name);
void widget_visibility_list_add(GtkWidget *widget, tag_attr *attr);
void widget_show_all(void);

#endif
