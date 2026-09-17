/*
 * widget_assistant.c:
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
#include "stack.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_assistant.h"

extern gboolean option_centering;
extern gboolean have_geometry_xy;
extern gboolean have_geometry_dxdy;
extern gint geometry_dx;
extern gint geometry_dy;
extern gint geometry_x;
extern gint geometry_y;

static gboolean widget_assistant_parse_page_type(
	const gchar *value, GtkAssistantPageType *type)
{
	if (g_ascii_strcasecmp(value, "content") == 0)
		*type = GTK_ASSISTANT_PAGE_CONTENT;
	else if (g_ascii_strcasecmp(value, "intro") == 0)
		*type = GTK_ASSISTANT_PAGE_INTRO;
	else if (g_ascii_strcasecmp(value, "confirm") == 0)
		*type = GTK_ASSISTANT_PAGE_CONFIRM;
	else if (g_ascii_strcasecmp(value, "summary") == 0)
		*type = GTK_ASSISTANT_PAGE_SUMMARY;
	else if (g_ascii_strcasecmp(value, "progress") == 0)
		*type = GTK_ASSISTANT_PAGE_PROGRESS;
	else
		return FALSE;
	return TRUE;
}

static void widget_assistant_set_page(GtkWidget *widget, const gchar *value)
{
	gint pages;
	gint page;

	pages = gtk_assistant_get_n_pages(GTK_ASSISTANT(widget));
	if (pages == 0)
		return;
	page = widget_parse_bounded_integer(value, 0, pages - 1, 0,
		"assistant page");
	gtk_assistant_set_current_page(GTK_ASSISTANT(widget), page);
}

static void widget_assistant_input_by_command(
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
		widget_assistant_set_page(var->Widget, line);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_assistant_input_by_file(
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
		widget_assistant_set_page(var->Widget, line);
		g_free(line);
	}
	fclose(input);
}

static void widget_assistant_apply(GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "apply");
}

static void widget_assistant_cancel(GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "cancel");
}

static void widget_assistant_close(GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "close");
}

static void widget_assistant_prepare(
	GtkWidget *widget, GtkWidget *page, AttributeSet *Attr)
{
	(void)page;
	widget_signal_executor(widget, Attr, "prepare");
}

void widget_assistant_clear(variable *var)
{
	if (gtk_assistant_get_n_pages(GTK_ASSISTANT(var->Widget)) > 0)
		gtk_assistant_set_current_page(GTK_ASSISTANT(var->Widget), 0);
}

GtkWidget *widget_assistant_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *element;
	GList *accel_group;
	GtkAssistantPageType page_type;
	GtkWidget *widget;
	stackelement pages;
	variable *page_var;
	gchar *title;
	gchar *value;
	gboolean complete;
	gint n;

	(void)attr;
	(void)Type;
	widget = gtk_assistant_new();
	attributeset_set_if_unset(Attr, ATTR_LABEL, PACKAGE);
	title = attributeset_get_first(&element, Attr, ATTR_LABEL);
	gtk_window_set_title(GTK_WINDOW(widget), title);
	gtk_window_set_icon_name(GTK_WINDOW(widget), PACKAGE);
	gtk_container_set_border_width(GTK_CONTAINER(widget), 5);
	if (have_geometry_dxdy)
		gtk_widget_set_size_request(widget, geometry_dx, geometry_dy);
	if (have_geometry_xy)
		gtk_window_move(GTK_WINDOW(widget), geometry_x, geometry_y);
	if (option_centering)
		gtk_window_set_position(GTK_WINDOW(widget), GTK_WIN_POS_CENTER_ALWAYS);

	pages = pop();
	for (n = 0; n < pages.nwidgets; ++n) {
		gtk_assistant_append_page(GTK_ASSISTANT(widget), pages.widgets[n]);
		page_var = find_variable_by_widget(pages.widgets[n]);
		if (page_var == NULL || page_var->widget_tag_attr == NULL)
			continue;
		value = get_tag_attribute(page_var->widget_tag_attr, "page-title");
		if (value != NULL) {
			gtk_assistant_set_page_title(GTK_ASSISTANT(widget),
				pages.widgets[n], value);
			kill_tag_attribute(page_var->widget_tag_attr, "page-title");
		}
		value = get_tag_attribute(page_var->widget_tag_attr, "page-type");
		if (value != NULL) {
			if (widget_assistant_parse_page_type(value, &page_type))
				gtk_assistant_set_page_type(GTK_ASSISTANT(widget),
					pages.widgets[n], page_type);
			else
				gtkdialog_warning("Invalid assistant page-type '%s'; using "
					"content.", value);
			kill_tag_attribute(page_var->widget_tag_attr, "page-type");
		}
		value = get_tag_attribute(page_var->widget_tag_attr, "complete");
		if (value != NULL) {
			if (widget_parse_boolean(value, &complete))
				gtk_assistant_set_page_complete(GTK_ASSISTANT(widget),
					pages.widgets[n], complete);
			else
				gtkdialog_warning("Invalid assistant complete value '%s'; "
					"using false.", value);
			kill_tag_attribute(page_var->widget_tag_attr, "complete");
		}
	}
	stackelement_clear(&pages);
	accel_group = g_list_first(accel_groups);
	while (accel_group != NULL) {
		gtk_window_add_accel_group(GTK_WINDOW(widget),
			GTK_ACCEL_GROUP(accel_group->data));
		g_object_unref(accel_group->data);
		accel_group = accel_group->next;
	}
	g_list_free(accel_groups);
	accel_groups = NULL;
	return widget;
}

gchar *widget_assistant_envvar_construct(GtkWidget *widget)
{
	return g_strdup_printf("%d",
		gtk_assistant_get_current_page(GTK_ASSISTANT(widget)));
}

void widget_assistant_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_assistant_set_page(var->Widget, value);
}

void widget_assistant_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_assistant_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_assistant_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_assistant_set_page(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(G_OBJECT(var->Widget), "apply",
		G_CALLBACK(widget_assistant_apply), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "cancel",
		G_CALLBACK(widget_assistant_cancel), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "close",
		G_CALLBACK(widget_assistant_close), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "prepare",
		G_CALLBACK(widget_assistant_prepare), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "delete-event",
		G_CALLBACK(window_delete_event_handler), NULL);
}

void widget_assistant_removeselected(variable *var)
{
	widget_assistant_clear(var);
}

void widget_assistant_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *page;

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
	page = widget_assistant_envvar_construct(var->Widget);
	fputs(page, output);
	g_free(page);
	widget_close_output(output, filename);
}
