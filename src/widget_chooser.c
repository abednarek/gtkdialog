/*
 * widget_chooser.c:
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
#include "widget_chooser.h"

static void widget_chooser_set_filename(
	GtkWidget *widget, const gchar *filename)
{
	if (filename == NULL || filename[0] == '\0')
		gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widget));
	else if (!gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widget),
		filename))
		gtkdialog_warning("Chooser could not select '%s'.", filename);
}

static void widget_chooser_add_filters(
	GtkWidget *widget, const gchar *values, gboolean mime_types)
{
	GtkFileFilter *filter;
	gchar **items;
	gint index;

	if (values == NULL)
		return;
	items = g_strsplit(values, "|", -1);
	for (index = 0; items[index] != NULL; ++index) {
		g_strstrip(items[index]);
		if (items[index][0] == '\0')
			continue;
		filter = gtk_file_filter_new();
		gtk_file_filter_set_name(filter, items[index]);
		if (mime_types)
			gtk_file_filter_add_mime_type(filter, items[index]);
		else
			gtk_file_filter_add_pattern(filter, items[index]);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(widget), filter);
	}
	g_strfreev(items);
}

static void widget_chooser_input_by_command(
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
		widget_chooser_set_filename(var->Widget, line);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_chooser_input_by_file(
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
		widget_chooser_set_filename(var->Widget, line);
		g_free(line);
	}
	fclose(input);
}

static void widget_chooser_file_activated(
	GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "file-activated");
}

static void widget_chooser_selection_changed(
	GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "selection-changed");
}

static void widget_chooser_update_preview(
	GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "update-preview");
}

static void widget_chooser_current_folder_changed(
	GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "current-folder-changed");
}

void widget_chooser_clear(variable *var)
{
	gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(var->Widget));
}

GtkWidget *widget_chooser_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *element;
	GtkWidget *widget;
	gchar *action;
	gchar *signal;

	(void)Type;
	widget = gtk_file_chooser_widget_new(GTK_FILE_CHOOSER_ACTION_OPEN);

	/* Preserve the original chooser-only spelling while using the shared
	 * signal executor and its conditions and action functions. */
	action = attributeset_get_first(&element, Attr, ATTR_ACTION);
	while (action != NULL) {
		signal = attributeset_get_this_tagattr(&element, Attr,
			ATTR_ACTION, "when");
		if (signal != NULL) {
			Attribute *attribute = element->data;

			/* The legacy implementation used when= even if signal= was
			 * also present, so retain that precedence. */
			kill_tag_attribute(attribute->tag_attributes, "signal");
			attributeset_set_this_tagattr(&element, Attr,
				ATTR_ACTION, "signal", signal);
		}
		action = attributeset_get_next(&element, Attr, ATTR_ACTION);
	}

	if (attr != NULL) {
		widget_chooser_add_filters(widget,
			get_tag_attribute(attr, "fs-filters-mime"), TRUE);
		widget_chooser_add_filters(widget,
			get_tag_attribute(attr, "fs-filters"), FALSE);
		kill_tag_attribute(attr, "fs-filters-mime");
		kill_tag_attribute(attr, "fs-filters");
	}
	return widget;
}

gchar *widget_chooser_envvar_construct(GtkWidget *widget)
{
	gchar *filename;

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
	return filename != NULL ? filename : g_strdup("");
}

void widget_chooser_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_chooser_set_filename(var->Widget, value);
}

void widget_chooser_refresh(variable *var)
{
	GList *element;
	gchar *default_file;
	gchar *input;
	gint height = -1;
	gboolean initialised;
	gint width = -1;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_chooser_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_chooser_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT)) {
		const gchar *folder = attributeset_get_first(&element,
			var->Attributes, ATTR_DEFAULT);

		if (folder != NULL && folder[0] != '\0' &&
			!gtk_file_chooser_set_current_folder(
				GTK_FILE_CHOOSER(var->Widget), folder))
			gtkdialog_warning("Chooser could not open folder '%s'.", folder);
	}
	if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
		width = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_WIDTH), "file chooser width");
	if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
		height = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_HEIGHT), "file chooser height");
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

	default_file = var->widget_tag_attr != NULL
		? get_tag_attribute(var->widget_tag_attr, "default-file") : NULL;
	if (default_file != NULL)
		widget_chooser_set_filename(var->Widget, default_file);
	if (var->widget_tag_attr != NULL)
		kill_tag_attribute(var->widget_tag_attr, "default-file");

	g_signal_connect(G_OBJECT(var->Widget), "file-activated",
		G_CALLBACK(widget_chooser_file_activated), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "selection-changed",
		G_CALLBACK(widget_chooser_selection_changed), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "update-preview",
		G_CALLBACK(widget_chooser_update_preview), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "current-folder-changed",
		G_CALLBACK(widget_chooser_current_folder_changed), var->Attributes);
}

void widget_chooser_removeselected(variable *var)
{
	widget_chooser_clear(var);
}

void widget_chooser_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *selection;

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
	selection = widget_chooser_envvar_construct(var->Widget);
	fputs(selection, output);
	g_free(selection);
	widget_close_output(output, filename);
}
