/*
 * widget_ruler.c: GtkRuler widgets.
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
#include "widgets.h"
#include "widget_ruler.h"

static void
widget_ruler_set_position(GtkWidget *widget, const gchar *text,
	const gchar *source)
{
	gdouble position;

	if (widget_parse_finite_double(text, &position, source))
		g_object_set(G_OBJECT(widget), "position", position, NULL);
}

static gchar *
widget_ruler_first_output_filename(AttributeSet *attributes)
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
widget_ruler_input_by_command(variable *var, const gchar *command)
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
		widget_ruler_set_position(var->Widget, line,
			"ruler command input");
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void
widget_ruler_input_by_file(variable *var, const gchar *filename)
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
		widget_ruler_set_position(var->Widget, line, "ruler file input");
		g_free(line);
	}
	fclose(input);
}

void
widget_ruler_clear(variable *var)
{
	gdouble lower;

	g_object_get(G_OBJECT(var->Widget), "lower", &lower, NULL);
	g_object_set(G_OBJECT(var->Widget), "position", lower, NULL);
}

GtkWidget *
widget_ruler_create(AttributeSet *Attr, tag_attr *attr, gint Type)
{
	(void)Attr;
	(void)attr;
	if (Type == WIDGET_HRULER)
		return gtk_hruler_new();
	return gtk_vruler_new();
}

gchar *
widget_ruler_envvar_construct(GtkWidget *widget)
{
	gchar value[G_ASCII_DTOSTR_BUF_SIZE];
	gdouble position;

	g_object_get(G_OBJECT(widget), "position", &position, NULL);
	g_ascii_dtostr(value, sizeof(value), position);
	return g_strdup(value);
}

void
widget_ruler_fileselect(variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void
widget_ruler_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_ruler_input_by_command(var, input + 8);
		else if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_ruler_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}

	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_ruler_set_position(var->Widget,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT), "ruler default");
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
}

void
widget_ruler_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);
}

void
widget_ruler_save(variable *var)
{
	FILE *output;
	gchar *filename;
	gchar *value;

	filename = widget_ruler_first_output_filename(var->Attributes);
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
	value = widget_ruler_envvar_construct(var->Widget);
	fputs(value, output);
	g_free(value);
	widget_close_output(output, filename);
}
