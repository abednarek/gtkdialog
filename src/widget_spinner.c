/*
 * widget_spinner.c:
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
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_spinner.h"

static void widget_spinner_set_active(
	GtkWidget *widget, const gchar *value, const gchar *source)
{
	gboolean active;

	if (!widget_parse_boolean(value, &active)) {
		gtkdialog_warning("Invalid spinner %s value '%s'; ignored.", source,
			value != NULL ? value : "");
		return;
	}
	if (active)
		gtk_spinner_start(GTK_SPINNER(widget));
	else
		gtk_spinner_stop(GTK_SPINNER(widget));
}

static void widget_spinner_input_by_command(
	variable *var, const gchar *command)
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
		widget_spinner_set_active(var->Widget, line, "input");
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_spinner_input_by_file(
	variable *var, const gchar *filename)
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
		widget_spinner_set_active(var->Widget, line, "input");
		g_free(line);
	}
	fclose(input);
}

void widget_spinner_clear(variable *var)
{
	gtk_spinner_stop(GTK_SPINNER(var->Widget));
}

GtkWidget *widget_spinner_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;
	gchar *active = NULL;

	(void)Attr;
	(void)Type;
	widget = gtk_spinner_new();
	if (attr != NULL)
		active = get_tag_attribute(attr, "active");
	if (active != NULL) {
		widget_spinner_set_active(widget, active, "active");
		kill_tag_attribute(attr, "active");
	}
	return widget;
}

gchar *widget_spinner_envvar_construct(GtkWidget *widget)
{
	gboolean active;

	g_object_get(G_OBJECT(widget), "active", &active, NULL);
	return g_strdup(active ? "true" : "false");
}

void widget_spinner_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_spinner_set_active(var->Widget, value, "value");
}

void widget_spinner_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_spinner_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_spinner_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_spinner_set_active(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			"default");
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

void widget_spinner_removeselected(variable *var)
{
	widget_spinner_clear(var);
}

void widget_spinner_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *state;

	directive = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (strncasecmp(directive, "file:", 5) == 0 &&
			strlen(directive) > 5) {
			filename = directive + 5;
			break;
		}
		directive = attributeset_get_next(&element,
			var->Attributes, ATTR_OUTPUT);
	}
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
	state = widget_spinner_envvar_construct(var->Widget);
	fputs(state, output);
	g_free(state);
	widget_close_output(output, filename);
}
