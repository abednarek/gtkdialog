/*
 * widget_linkbutton.c:
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
#include "widget_linkbutton.h"

static void widget_linkbutton_set_uri(GtkWidget *widget, const gchar *uri)
{
	gtk_link_button_set_uri(GTK_LINK_BUTTON(widget), uri != NULL ? uri : "");
}

static void widget_linkbutton_input_by_command(
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
		widget_linkbutton_set_uri(var->Widget, line);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_linkbutton_input_by_file(
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
		widget_linkbutton_set_uri(var->Widget, line);
		g_free(line);
	}
	fclose(input);
}

void widget_linkbutton_clear(variable *var)
{
	widget_linkbutton_set_uri(var->Widget, "");
}

GtkWidget *widget_linkbutton_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *element;
	GtkWidget *widget;
	gchar *label = NULL;
	gchar *uri = NULL;

	(void)Type;
	if (attr != NULL)
		uri = get_tag_attribute(attr, "uri");
	if (uri == NULL)
		gtkdialog_warning("Link button has no uri attribute; using an empty URI.");
	if (attributeset_is_avail(Attr, ATTR_LABEL))
		label = attributeset_get_first(&element, Attr, ATTR_LABEL);

	if (label != NULL)
		widget = gtk_link_button_new_with_label(uri != NULL ? uri : "", label);
	else
		widget = gtk_link_button_new(uri != NULL ? uri : "");
	if (uri != NULL)
		kill_tag_attribute(attr, "uri");
	return widget;
}

gchar *widget_linkbutton_envvar_construct(GtkWidget *widget)
{
	const gchar *uri;

	uri = gtk_link_button_get_uri(GTK_LINK_BUTTON(widget));
	return g_strdup(uri != NULL ? uri : "");
}

void widget_linkbutton_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_linkbutton_set_uri(var->Widget, value);
}

void widget_linkbutton_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_linkbutton_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_linkbutton_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_linkbutton_set_uri(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);

	g_signal_connect(G_OBJECT(var->Widget), "clicked",
		G_CALLBACK(button_clicked_attr), var->Attributes);
}

void widget_linkbutton_removeselected(variable *var)
{
	widget_linkbutton_clear(var);
}

void widget_linkbutton_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *uri;

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
	uri = widget_linkbutton_envvar_construct(var->Widget);
	fputs(uri, output);
	g_free(uri);
	widget_close_output(output, filename);
}
