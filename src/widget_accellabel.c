/*
 * widget_accellabel.c:
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
#include "variables.h"
#include "widgets.h"
#include "widget_accellabel.h"

#define ACCEL_WIDGET_DATA "gtkdialog-accel-widget"
#define ACCEL_WIDGET_BOUND_DATA "gtkdialog-accel-widget-bound"

static void widget_accellabel_set_text(GtkWidget *widget, const gchar *text)
{
	if (gtk_label_get_use_markup(GTK_LABEL(widget)))
		gtk_label_set_markup(GTK_LABEL(widget), text != NULL ? text : "");
	else
		gtk_label_set_text(GTK_LABEL(widget), text != NULL ? text : "");
}

static void widget_accellabel_bind(GtkWidget *widget, gboolean warn)
{
	const gchar *target_name;
	variable *target;

	if (g_object_get_data(G_OBJECT(widget), ACCEL_WIDGET_BOUND_DATA) != NULL)
		return;
	target_name = g_object_get_data(G_OBJECT(widget), ACCEL_WIDGET_DATA);
	if (target_name == NULL || *target_name == '\0')
		return;
	target = variables_get_by_name(target_name);
	if (target == NULL || target->Widget == NULL ||
		target->Widget == widget) {
		if (warn)
			fprintf(stderr, "%s(): Couldn't bind accel-widget '%s'.\n",
				__func__, target_name);
		return;
	}
	gtk_accel_label_set_accel_widget(GTK_ACCEL_LABEL(widget), target->Widget);
	gtk_accel_label_refetch(GTK_ACCEL_LABEL(widget));
	g_object_set_data(G_OBJECT(widget), ACCEL_WIDGET_BOUND_DATA,
		GINT_TO_POINTER(TRUE));
}

static void widget_accellabel_realized(GtkWidget *widget, gpointer data)
{
	(void)data;
	widget_accellabel_bind(widget, TRUE);
}

static void widget_accellabel_input_by_command(
	variable *var, const gchar *command)
{
	FILE *input;
	gchar *text;

	input = widget_opencommand(command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	text = widget_read_all(input);
	widget_accellabel_set_text(var->Widget, text);
	g_free(text);
	widget_closecommand(input, command);
}

static void widget_accellabel_input_by_file(
	variable *var, const gchar *filename)
{
	FILE *input;
	gchar *text;

	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}
	text = widget_read_all(input);
	widget_accellabel_set_text(var->Widget, text);
	g_free(text);
	fclose(input);
}

void widget_accellabel_clear(variable *var)
{
	widget_accellabel_set_text(var->Widget, "");
}

GtkWidget *widget_accellabel_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *element;
	GtkWidget *widget;
	gchar *target_name;
	gchar *value;

	(void)Type;
	attributeset_set_if_unset(Attr, ATTR_LABEL, "");
	value = attributeset_get_first(&element, Attr, ATTR_LABEL);
	if (attributeset_is_avail(Attr, ATTR_DEFAULT))
		value = attributeset_get_first(&element, Attr, ATTR_DEFAULT);
	widget = gtk_accel_label_new(value);
	target_name = attr != NULL
		? get_tag_attribute(attr, "accel-widget") : NULL;
	if (target_name != NULL) {
		g_object_set_data_full(G_OBJECT(widget), ACCEL_WIDGET_DATA,
			g_strdup(target_name), g_free);
		kill_tag_attribute(attr, "accel-widget");
		g_signal_connect(G_OBJECT(widget), "realize",
			G_CALLBACK(widget_accellabel_realized), NULL);
	}
	return widget;
}

gchar *widget_accellabel_envvar_construct(GtkWidget *widget)
{
	return g_strdup(gtk_label_get_text(GTK_LABEL(widget)));
}

void widget_accellabel_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_accellabel_set_text(var->Widget, value);
}

void widget_accellabel_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gchar *value;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (!initialised) {
		value = attributeset_get_first(&element,
			var->Attributes, ATTR_LABEL);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			value = attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT);
		widget_accellabel_set_text(var->Widget, value);
	}
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_accellabel_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_accellabel_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	widget_accellabel_bind(var->Widget,
		GTK_WIDGET_REALIZED(var->Widget));
}

void widget_accellabel_removeselected(variable *var)
{
	widget_accellabel_clear(var);
}

void widget_accellabel_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *text;

	directive = attributeset_get_first(&element,
		var->Attributes, ATTR_OUTPUT);
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
	text = widget_accellabel_envvar_construct(var->Widget);
	fputs(text, output);
	g_free(text);
	widget_close_output(output, filename);
}
