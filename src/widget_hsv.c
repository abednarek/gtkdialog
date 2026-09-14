/*
 * widget_hsv.c: GtkHSV widget.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE
#include <gtk/gtk.h>

#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_hsv.h"

static gboolean
widget_hsv_parse_value(const gchar *text, gdouble *hue,
	gdouble *saturation, gdouble *value, const gchar *source)
{
	gchar **fields;
	gboolean valid;
	gint count;

	fields = g_strsplit(text, "|", 4);
	for (count = 0; fields[count] != NULL; ++count)
		;
	valid = count == 3 &&
		widget_parse_finite_double(g_strstrip(fields[0]), hue, source) &&
		widget_parse_finite_double(g_strstrip(fields[1]), saturation, source) &&
		widget_parse_finite_double(g_strstrip(fields[2]), value, source) &&
		*hue >= 0.0 && *hue <= 1.0 &&
		*saturation >= 0.0 && *saturation <= 1.0 &&
		*value >= 0.0 && *value <= 1.0;
	if (!valid)
		gtkdialog_warning(
			"Invalid %s value '%s'; expected H|S|V in the 0..1 range.",
			source, text);
	g_strfreev(fields);
	return valid;
}

static void
widget_hsv_set_value(GtkWidget *widget, const gchar *text,
	const gchar *source)
{
	gdouble hue;
	gdouble saturation;
	gdouble value;

	if (widget_hsv_parse_value(text, &hue, &saturation, &value, source))
		gtk_hsv_set_color(GTK_HSV(widget), hue, saturation, value);
}

static gchar *
widget_hsv_first_output_filename(AttributeSet *attributes)
{
	GList *element;
	gchar *directive;

	directive = attributeset_get_first(&element, attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (g_ascii_strncasecmp(directive, "file:", 5) == 0 &&
			strlen(directive) > 5)
			return directive + 5;
		directive = attributeset_get_next(&element, attributes, ATTR_OUTPUT);
	}
	return NULL;
}

static void
widget_hsv_input_by_command(variable *var, const gchar *command)
{
	FILE *input;
	gchar *line;

	input = widget_opencommand(command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	line = widget_read_line(input);
	if (line != NULL) {
		widget_hsv_set_value(var->Widget, line, "hsv command input");
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void
widget_hsv_input_by_file(variable *var, const gchar *filename)
{
	FILE *input;
	gchar *line;

	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}
	line = widget_read_line(input);
	if (line != NULL) {
		widget_hsv_set_value(var->Widget, line, "hsv file input");
		g_free(line);
	}
	fclose(input);
}

static void
widget_hsv_changed(GtkWidget *widget, AttributeSet *attributes)
{
	widget_signal_executor(widget, attributes, "changed");
}

void
widget_hsv_clear(variable *var)
{
	gtk_hsv_set_color(GTK_HSV(var->Widget), 0.0, 0.0, 1.0);
}

GtkWidget *
widget_hsv_create(AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;
	gchar *text;
	gint size;
	gint ring_width;

	(void)Attr;
	(void)Type;
	widget = gtk_hsv_new();
	gtk_hsv_get_metrics(GTK_HSV(widget), &size, &ring_width);
	if (attr != NULL) {
		if ((text = get_tag_attribute(attr, "hsv-size")) != NULL) {
			size = widget_parse_nonnegative_integer(text, size, "hsv-size");
			kill_tag_attribute(attr, "hsv-size");
		}
		if ((text = get_tag_attribute(attr, "ring-width")) != NULL) {
			ring_width = widget_parse_nonnegative_integer(
				text, ring_width, "ring-width");
			kill_tag_attribute(attr, "ring-width");
		}
	}
	if (size > 0 && ring_width > 0 && ring_width <= (size - 1) / 2)
		gtk_hsv_set_metrics(GTK_HSV(widget), size, ring_width);
	else
		gtkdialog_warning(
			"Invalid hsv metrics; hsv-size must exceed twice ring-width.");
	return widget;
}

gchar *
widget_hsv_envvar_construct(GtkWidget *widget)
{
	gchar hue[G_ASCII_DTOSTR_BUF_SIZE];
	gchar saturation[G_ASCII_DTOSTR_BUF_SIZE];
	gchar value[G_ASCII_DTOSTR_BUF_SIZE];
	gdouble h;
	gdouble s;
	gdouble v;

	gtk_hsv_get_color(GTK_HSV(widget), &h, &s, &v);
	g_ascii_dtostr(hue, sizeof(hue), h);
	g_ascii_dtostr(saturation, sizeof(saturation), s);
	g_ascii_dtostr(value, sizeof(value), v);
	return g_strdup_printf("%s|%s|%s", hue, saturation, value);
}

void
widget_hsv_fileselect(variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void
widget_hsv_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_hsv_input_by_command(var, input + 8);
		else if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_hsv_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}

	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_hsv_set_value(var->Widget,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT), "hsv default");
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

	g_signal_connect(G_OBJECT(var->Widget), "changed",
		G_CALLBACK(widget_hsv_changed), var->Attributes);
}

void
widget_hsv_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);
}

void
widget_hsv_save(variable *var)
{
	FILE *output;
	gchar *filename;
	gchar *value;

	filename = widget_hsv_first_output_filename(var->Attributes);
	if (filename == NULL) {
		fprintf(stderr, "%s(): No <output file> directive found.\n",
			__func__);
		return;
	}
	output = fopen(filename, "w");
	if (output == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}
	value = widget_hsv_envvar_construct(var->Widget);
	fputs(value, output);
	g_free(value);
	widget_close_output(output, filename);
}
