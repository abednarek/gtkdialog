/*
 * widget_fontselection.c: GtkFontSelection widget.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
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
 */

#define _GNU_SOURCE
#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widget_fontselection.h"

#define FONTSELECTION_LAST_VALUE "gtkdialog-fontselection-last-value"
#define FONTSELECTION_PENDING_VALUE "gtkdialog-fontselection-pending-value"
#define FONTSELECTION_UPDATING "gtkdialog-fontselection-updating"

static void
widget_fontselection_remember(GtkWidget *widget, const gchar *font_name)
{
	g_object_set_data_full(G_OBJECT(widget), FONTSELECTION_LAST_VALUE,
		g_strdup(font_name), g_free);
}

static void
widget_fontselection_emit_if_changed(GtkWidget *widget, AttributeSet *Attr)
{
	gchar *font_name;
	const gchar *previous;

	if (GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), FONTSELECTION_UPDATING)))
		return;

	font_name = gtk_font_selection_get_font_name(GTK_FONT_SELECTION(widget));
	if (font_name == NULL)
		return;
	previous = g_object_get_data(G_OBJECT(widget), FONTSELECTION_LAST_VALUE);
	if (g_strcmp0(previous, font_name) != 0) {
		widget_fontselection_remember(widget, font_name);
		widget_signal_executor(widget, Attr, "font-changed");
	}
	g_free(font_name);
}

static void
widget_fontselection_cursor_changed(GtkTreeView *tree_view, gpointer data)
{
	GtkWidget *widget;

	widget = g_object_get_data(G_OBJECT(tree_view), "gtkdialog-fontselection");
	if (widget != NULL)
		widget_fontselection_emit_if_changed(widget, data);
}

static void
widget_fontselection_size_changed(GtkEditable *editable, gpointer data)
{
	GtkWidget *widget;

	widget = g_object_get_data(G_OBJECT(editable), "gtkdialog-fontselection");
	if (widget != NULL)
		widget_fontselection_emit_if_changed(widget, data);
}

static void
widget_fontselection_connect_child(GtkWidget *child, GtkWidget *selection,
	const gchar *signal_name, GCallback callback, AttributeSet *Attr)
{
	g_object_set_data(G_OBJECT(child), "gtkdialog-fontselection", selection);
	g_signal_connect(G_OBJECT(child), signal_name, callback, Attr);
}

static void
widget_fontselection_connect_changes(GtkWidget *widget, AttributeSet *Attr)
{
	GtkFontSelection *selection;
	gchar *font_name;

	selection = GTK_FONT_SELECTION(widget);
	font_name = gtk_font_selection_get_font_name(selection);
	if (font_name != NULL) {
		widget_fontselection_remember(widget, font_name);
		g_free(font_name);
	}

	widget_fontselection_connect_child(
		gtk_font_selection_get_family_list(selection), widget,
		"cursor-changed", G_CALLBACK(widget_fontselection_cursor_changed), Attr);
	widget_fontselection_connect_child(
		gtk_font_selection_get_face_list(selection), widget,
		"cursor-changed", G_CALLBACK(widget_fontselection_cursor_changed), Attr);
	widget_fontselection_connect_child(
		gtk_font_selection_get_size_list(selection), widget,
		"cursor-changed", G_CALLBACK(widget_fontselection_cursor_changed), Attr);
	widget_fontselection_connect_child(
		gtk_font_selection_get_size_entry(selection), widget,
		"changed", G_CALLBACK(widget_fontselection_size_changed), Attr);
}

static gboolean
widget_fontselection_set_value(variable *var, const gchar *value,
	gboolean emit_signal)
{
	gchar *font_name;
	const gchar *previous;
	gboolean changed;
	gboolean accepted;

	if (!GTK_WIDGET_REALIZED(var->Widget)) {
		g_object_set_data_full(G_OBJECT(var->Widget),
			FONTSELECTION_PENDING_VALUE, g_strdup(value), g_free);
		return TRUE;
	}

	g_object_set_data(G_OBJECT(var->Widget), FONTSELECTION_UPDATING,
		GINT_TO_POINTER(TRUE));
	accepted = gtk_font_selection_set_font_name(
		GTK_FONT_SELECTION(var->Widget), value);
	g_object_set_data(G_OBJECT(var->Widget), FONTSELECTION_UPDATING, NULL);
	if (!accepted) {
		gtkdialog_warning("Invalid fontselection value '%s'; font ignored.",
			value);
		return FALSE;
	}

	font_name = gtk_font_selection_get_font_name(
		GTK_FONT_SELECTION(var->Widget));
	if (font_name == NULL)
		return TRUE;
	previous = g_object_get_data(
		G_OBJECT(var->Widget), FONTSELECTION_LAST_VALUE);
	changed = g_strcmp0(previous, font_name) != 0;
	widget_fontselection_remember(var->Widget, font_name);
	g_free(font_name);
	if (changed && emit_signal)
		widget_signal_executor(var->Widget, var->Attributes, "font-changed");
	return TRUE;
}

static void
widget_fontselection_realized(GtkWidget *widget, gpointer data)
{
	variable *var = data;
	const gchar *pending;
	gchar *value;

	pending = g_object_get_data(
		G_OBJECT(widget), FONTSELECTION_PENDING_VALUE);
	if (pending == NULL)
		return;
	value = g_strdup(pending);
	g_object_set_data(G_OBJECT(widget), FONTSELECTION_PENDING_VALUE, NULL);
	widget_fontselection_set_value(var, value, FALSE);
	g_free(value);
}

static void
widget_fontselection_input_by_command(variable *var, const gchar *command,
	gboolean emit_signal)
{
	FILE *infile;
	gchar *line;

	infile = widget_opencommand((gchar *)command);
	if (infile == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	line = widget_read_line(infile);
	if (line != NULL) {
		widget_fontselection_set_value(var, line, emit_signal);
		g_free(line);
	}
	widget_closecommand(infile, (gchar *)command);
}

static void
widget_fontselection_input_by_file(variable *var, const gchar *filename,
	gboolean emit_signal)
{
	FILE *infile;
	gchar *line;

	infile = fopen(filename, "r");
	if (infile == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}
	line = widget_read_line(infile);
	if (line != NULL) {
		widget_fontselection_set_value(var, line, emit_signal);
		g_free(line);
	}
	fclose(infile);
}

void widget_fontselection_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_fontselection_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	(void)Attr;
	(void)attr;
	(void)Type;
	return gtk_font_selection_new();
}

gchar *widget_fontselection_envvar_construct(GtkWidget *widget)
{
	return gtk_font_selection_get_font_name(GTK_FONT_SELECTION(widget));
}

void widget_fontselection_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void widget_fontselection_refresh(variable *var)
{
	GList *element;
	gchar *act;
	gchar *value;
	gboolean initialised;
	gint width = -1;
	gint height = -1;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act != NULL) {
		if (input_is_shell_command(act))
			widget_fontselection_input_by_command(var, act + 8, initialised);
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, act + 5);
			widget_fontselection_input_by_file(var, act + 5, initialised);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_fontselection_set_value(var,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			FALSE);
	if (var->widget_tag_attr != NULL &&
		(value = get_tag_attribute(var->widget_tag_attr, "font-name"))) {
		widget_fontselection_set_value(var, value, FALSE);
		kill_tag_attribute(var->widget_tag_attr, "font-name");
	}
	if (var->widget_tag_attr != NULL &&
		(value = get_tag_attribute(var->widget_tag_attr, "preview-text"))) {
		gtk_font_selection_set_preview_text(
			GTK_FONT_SELECTION(var->Widget), value);
		kill_tag_attribute(var->widget_tag_attr, "preview-text");
	}
	if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
		width = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_WIDTH), "fontselection width");
	if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
		height = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_HEIGHT), "fontselection height");
	if (width != -1 || height != -1)
		gtk_widget_set_size_request(var->Widget, width, height);
	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
			__func__);
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);

	widget_fontselection_connect_changes(var->Widget, var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "realize",
		G_CALLBACK(widget_fontselection_realized), var);
}

void widget_fontselection_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);
}

void widget_fontselection_save(variable *var)
{
	FILE *outfile;
	GList *element;
	gchar *act;
	gchar *filename = NULL;
	gchar *value;

	act = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (act != NULL) {
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			filename = act + 5;
			break;
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_OUTPUT);
	}
	if (filename == NULL) {
		fprintf(stderr, "%s(): No <output file> directive found.\n", __func__);
		return;
	}

	outfile = fopen(filename, "w");
	if (outfile == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}
	value = widget_fontselection_envvar_construct(var->Widget);
	if (value != NULL) {
		fputs(value, outfile);
		g_free(value);
	}
	widget_close_output(outfile, filename);
}
