/*
 * widget_colorselection.c: GtkColorSelection widget.
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
#include <errno.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "widget_colorselection.h"

static gboolean
widget_colorselection_parse_alpha(const gchar *value, guint *result)
{
	gchar *end;
	gint64 alpha;

	errno = 0;
	alpha = g_ascii_strtoll(value, &end, 10);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == value || *end != '\0' ||
		alpha < 0 || alpha > G_MAXUINT16) {
		gtkdialog_warning(
			"Invalid colorselection alpha value '%s'; alpha ignored.",
			value);
		return FALSE;
	}

	*result = (guint)alpha;
	return TRUE;
}

static void
widget_colorselection_set_value(GtkColorSelection *selection,
	const gchar *value)
{
	GdkColor color;
	guint alpha;
	list_t *values;

	values = linecutter(g_strdup(value), '|');
	if (values->n_lines > 0 &&
		gdk_color_parse(g_strstrip(values->line[0]), &color))
		gtk_color_selection_set_current_color(selection, &color);
	if (values->n_lines > 1 &&
		widget_colorselection_parse_alpha(values->line[1], &alpha))
		gtk_color_selection_set_current_alpha(selection, alpha);
	list_t_free(values);
}

static void
widget_colorselection_input_by_command(variable *var, const gchar *command)
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
		widget_colorselection_set_value(
			GTK_COLOR_SELECTION(var->Widget), line);
		g_free(line);
	}
	widget_closecommand(infile, (gchar *)command);
}

static void
widget_colorselection_input_by_file(variable *var, const gchar *filename)
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
		widget_colorselection_set_value(
			GTK_COLOR_SELECTION(var->Widget), line);
		g_free(line);
	}
	fclose(infile);
}

static void
widget_colorselection_changed(GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "color-changed");
}

void widget_colorselection_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_colorselection_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	(void)Attr;
	(void)attr;
	(void)Type;
	return gtk_color_selection_new();
}

gchar *widget_colorselection_envvar_construct(GtkWidget *widget)
{
	GdkColor color;
	GtkColorSelection *selection;

	selection = GTK_COLOR_SELECTION(widget);
	gtk_color_selection_get_current_color(selection, &color);
	if (gtk_color_selection_get_has_opacity_control(selection))
		return g_strdup_printf("#%02x%02x%02x|%u",
			(color.red + 257 / 2) / 257,
			(color.green + 257 / 2) / 257,
			(color.blue + 257 / 2) / 257,
			gtk_color_selection_get_current_alpha(selection));

	return g_strdup_printf("#%02x%02x%02x",
		(color.red + 257 / 2) / 257,
		(color.green + 257 / 2) / 257,
		(color.blue + 257 / 2) / 257);
}

void widget_colorselection_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void widget_colorselection_refresh(variable *var)
{
	GList *element;
	gchar *act;
	gboolean initialised;
	gint width = -1;
	gint height = -1;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act != NULL) {
		if (input_is_shell_command(act))
			widget_colorselection_input_by_command(var, act + 8);
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, act + 5);
			widget_colorselection_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_colorselection_set_value(GTK_COLOR_SELECTION(var->Widget),
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
	if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
		width = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_WIDTH), "colorselection width");
	if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
		height = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_HEIGHT), "colorselection height");
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

	g_signal_connect(G_OBJECT(var->Widget), "color-changed",
		G_CALLBACK(widget_colorselection_changed), var->Attributes);
}

void widget_colorselection_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);
}

void widget_colorselection_save(variable *var)
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
		fprintf(stderr, "%s(): No <output file> directive found.\n",
			__func__);
		return;
	}

	outfile = fopen(filename, "w");
	if (outfile == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}
	value = widget_colorselection_envvar_construct(var->Widget);
	fputs(value, outfile);
	g_free(value);
	widget_close_output(outfile, filename);
}
