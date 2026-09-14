/*
 * widget_aspectframe.c:
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
#include "widget_aspectframe.h"

static gfloat widget_aspectframe_parse_alignment(
	const gchar *text, gfloat fallback, const gchar *name)
{
	gdouble value;

	if (text == NULL)
		return fallback;
	if (!widget_parse_finite_double(text, &value, name) ||
		value < 0.0 || value > 1.0) {
		gtkdialog_warning("Invalid aspect frame %s value '%s'; using %.1f.",
			name, text, fallback);
		return fallback;
	}
	return (gfloat)value;
}

static gfloat widget_aspectframe_parse_ratio(const gchar *text)
{
	gdouble value;

	if (text == NULL)
		return 1.0;
	if (!widget_parse_finite_double(text, &value, "aspect frame ratio") ||
		value <= 0.0 || value > G_MAXFLOAT) {
		gtkdialog_warning("Invalid aspect frame ratio '%s'; using 1.", text);
		return 1.0;
	}
	return (gfloat)value;
}

static gboolean widget_aspectframe_parse_obey_child(const gchar *text)
{
	gboolean value;

	if (text == NULL)
		return TRUE;
	if (!widget_parse_boolean(text, &value)) {
		gtkdialog_warning("Invalid aspect frame obey-child value '%s'; "
			"using true.", text);
		return TRUE;
	}
	return value;
}

static void widget_aspectframe_input_by_command(
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
		gtk_frame_set_label(GTK_FRAME(var->Widget), line);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_aspectframe_input_by_file(
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
		gtk_frame_set_label(GTK_FRAME(var->Widget), line);
		g_free(line);
	}
	fclose(input);
}

void widget_aspectframe_clear(variable *var)
{
	gtk_frame_set_label(GTK_FRAME(var->Widget), "");
}

GtkWidget *widget_aspectframe_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *element;
	GtkWidget *widget;
	stackelement child;
	gchar *label;
	gfloat ratio;
	gfloat xalign;
	gfloat yalign;
	gboolean obey_child;

	(void)Type;
	child = pop();
	if (child.nwidgets != 1) {
		stackelement_clear(&child);
		g_error("gtkdialog: <aspectframe> requires exactly one direct "
			"child widget.");
	}
	attributeset_set_if_unset(Attr, ATTR_LABEL, "");
	label = attributeset_get_first(&element, Attr, ATTR_LABEL);
	xalign = widget_aspectframe_parse_alignment(attr == NULL ? NULL :
		get_tag_attribute(attr, "xalign"), 0.5, "xalign");
	yalign = widget_aspectframe_parse_alignment(attr == NULL ? NULL :
		get_tag_attribute(attr, "yalign"), 0.5, "yalign");
	ratio = widget_aspectframe_parse_ratio(attr == NULL ? NULL :
		get_tag_attribute(attr, "ratio"));
	obey_child = widget_aspectframe_parse_obey_child(attr == NULL ? NULL :
		get_tag_attribute(attr, "obey-child"));
	widget = gtk_aspect_frame_new(label, xalign, yalign, ratio, obey_child);
	gtk_container_add(GTK_CONTAINER(widget), child.widgets[0]);
	stackelement_clear(&child);
	if (attr != NULL) {
		kill_tag_attribute(attr, "xalign");
		kill_tag_attribute(attr, "yalign");
		kill_tag_attribute(attr, "ratio");
		kill_tag_attribute(attr, "obey-child");
	}
	return widget;
}

gchar *widget_aspectframe_envvar_construct(GtkWidget *widget)
{
	const gchar *label;

	label = gtk_frame_get_label(GTK_FRAME(widget));
	return g_strdup(label != NULL ? label : "");
}

void widget_aspectframe_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	gtk_frame_set_label(GTK_FRAME(var->Widget), value);
}

void widget_aspectframe_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_aspectframe_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_aspectframe_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		gtk_frame_set_label(GTK_FRAME(var->Widget),
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
}

void widget_aspectframe_removeselected(variable *var)
{
	widget_aspectframe_clear(var);
}

void widget_aspectframe_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;

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
	fputs(gtk_frame_get_label(GTK_FRAME(var->Widget)), output);
	widget_close_output(output, filename);
}
