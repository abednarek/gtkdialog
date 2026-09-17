/*
 * widget_filechooserbutton.c:
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
#include "widget_filechooserbutton.h"

static void widget_filechooserbutton_set_filename(
	GtkWidget *widget, const gchar *filename)
{
	if (filename == NULL || filename[0] == '\0')
		gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(widget));
	else if (!gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(widget),
		filename))
		gtkdialog_warning("File chooser button could not select '%s'.",
			filename);
}

static GtkFileChooserAction widget_filechooserbutton_parse_action(
	tag_attr *attr)
{
	gchar *value;

	if (attr == NULL)
		return GTK_FILE_CHOOSER_ACTION_OPEN;
	value = get_tag_attribute(attr, "action");
	if (value == NULL)
		return GTK_FILE_CHOOSER_ACTION_OPEN;
	if (strcasecmp(value, "open") == 0)
		return GTK_FILE_CHOOSER_ACTION_OPEN;
	if (strcasecmp(value, "select-folder") == 0 ||
		strcasecmp(value, "folder") == 0)
		return GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
	gtkdialog_warning("Invalid file chooser button action '%s'; using open.",
		value);
	return GTK_FILE_CHOOSER_ACTION_OPEN;
}

static void widget_filechooserbutton_add_filter(
	GtkWidget *widget, tag_attr *attr)
{
	GtkFileFilter *filter;
	gboolean populated = FALSE;
	gchar **items;
	gchar *mime_types;
	gchar *name;
	gchar *patterns;
	gint index;

	if (attr == NULL)
		return;
	name = get_tag_attribute(attr, "filter-name");
	patterns = get_tag_attribute(attr, "filter-pattern");
	mime_types = get_tag_attribute(attr, "filter-mime-type");
	if (patterns == NULL && mime_types == NULL)
		return;

	filter = gtk_file_filter_new();
	if (name != NULL)
		gtk_file_filter_set_name(filter, name);
	if (patterns != NULL) {
		items = g_strsplit_set(patterns, ";,", -1);
		for (index = 0; items[index] != NULL; ++index) {
			g_strstrip(items[index]);
			if (items[index][0] != '\0') {
				gtk_file_filter_add_pattern(filter, items[index]);
				populated = TRUE;
			}
		}
		g_strfreev(items);
	}
	if (mime_types != NULL) {
		items = g_strsplit_set(mime_types, ";,", -1);
		for (index = 0; items[index] != NULL; ++index) {
			g_strstrip(items[index]);
			if (items[index][0] != '\0') {
				gtk_file_filter_add_mime_type(filter, items[index]);
				populated = TRUE;
			}
		}
		g_strfreev(items);
	}
	if (populated)
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(widget), filter);
	else
		g_object_unref(filter);
}

static void widget_filechooserbutton_input_by_command(
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
		widget_filechooserbutton_set_filename(var->Widget, line);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_filechooserbutton_input_by_file(
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
		widget_filechooserbutton_set_filename(var->Widget, line);
		g_free(line);
	}
	fclose(input);
}

static void widget_filechooserbutton_file_set(
	GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "file-set");
}

void widget_filechooserbutton_clear(variable *var)
{
	gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(var->Widget));
}

GtkWidget *widget_filechooserbutton_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkFileChooserAction action;
	GtkWidget *widget;
	gchar *current_folder = NULL;
	gchar *title = NULL;

	(void)Attr;
	(void)Type;
	action = widget_filechooserbutton_parse_action(attr);
	if (attr != NULL) {
		title = get_tag_attribute(attr, "title");
		current_folder = get_tag_attribute(attr, "current-folder");
	}
	widget = gtk_file_chooser_button_new(
		title != NULL ? title : "Select a file", action);
	widget_filechooserbutton_add_filter(widget, attr);
	if (current_folder != NULL && current_folder[0] != '\0' &&
		!gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(widget),
			current_folder))
		gtkdialog_warning("File chooser button could not open folder '%s'.",
			current_folder);
	if (attr != NULL) {
		kill_tag_attribute(attr, "action");
		kill_tag_attribute(attr, "title");
		kill_tag_attribute(attr, "current-folder");
		kill_tag_attribute(attr, "filter-name");
		kill_tag_attribute(attr, "filter-pattern");
		kill_tag_attribute(attr, "filter-mime-type");
	}
	return widget;
}

gchar *widget_filechooserbutton_envvar_construct(GtkWidget *widget)
{
	gchar *filename;

	filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));
	return filename != NULL ? filename : g_strdup("");
}

void widget_filechooserbutton_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_filechooserbutton_set_filename(var->Widget, value);
}

void widget_filechooserbutton_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_filechooserbutton_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_filechooserbutton_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_filechooserbutton_set_filename(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
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

	g_signal_connect(G_OBJECT(var->Widget), "file-set",
		G_CALLBACK(widget_filechooserbutton_file_set), var->Attributes);
}

void widget_filechooserbutton_removeselected(variable *var)
{
	widget_filechooserbutton_clear(var);
}

void widget_filechooserbutton_save(variable *var)
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
	selection = widget_filechooserbutton_envvar_construct(var->Widget);
	fputs(selection, output);
	g_free(selection);
	widget_close_output(output, filename);
}
