/*
 * widget_dialog.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "stack.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_dialog.h"

typedef struct {
	const gchar *name;
	gint value;
} WidgetDialogResponse;

static const WidgetDialogResponse widget_dialog_responses[] = {
	{ "none", GTK_RESPONSE_NONE },
	{ "reject", GTK_RESPONSE_REJECT },
	{ "accept", GTK_RESPONSE_ACCEPT },
	{ "delete-event", GTK_RESPONSE_DELETE_EVENT },
	{ "ok", GTK_RESPONSE_OK },
	{ "cancel", GTK_RESPONSE_CANCEL },
	{ "close", GTK_RESPONSE_CLOSE },
	{ "yes", GTK_RESPONSE_YES },
	{ "no", GTK_RESPONSE_NO },
	{ "apply", GTK_RESPONSE_APPLY },
	{ "help", GTK_RESPONSE_HELP }
};

extern gboolean option_centering;
extern gboolean have_geometry_xy;
extern gboolean have_geometry_dxdy;
extern gint geometry_dx;
extern gint geometry_dy;
extern gint geometry_x;
extern gint geometry_y;

gboolean widget_dialog_parse_response(const gchar *text, gint *response)
{
	char *end;
	long number;
	guint index;

	if (text == NULL || text[0] == '\0')
		return FALSE;
	for (index = 0; index < G_N_ELEMENTS(widget_dialog_responses); ++index) {
		if (g_ascii_strcasecmp(text, widget_dialog_responses[index].name) == 0) {
			*response = widget_dialog_responses[index].value;
			return TRUE;
		}
	}
	errno = 0;
	number = strtol(text, &end, 10);
	if (errno != 0 || end == text || *end != '\0' ||
		number < G_MININT || number > G_MAXINT)
		return FALSE;
	*response = (gint)number;
	return TRUE;
}

gchar *widget_dialog_response_name(gint response)
{
	guint index;

	for (index = 0; index < G_N_ELEMENTS(widget_dialog_responses); ++index) {
		if (response == widget_dialog_responses[index].value)
			return g_strdup(widget_dialog_responses[index].name);
	}
	return g_strdup_printf("%d", response);
}

static void widget_dialog_store_response(GtkWidget *widget, gint response)
{
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-dialog-response",
		widget_dialog_response_name(response), g_free);
}

static void widget_dialog_response(
	GtkDialog *dialog, gint response, AttributeSet *Attr)
{
	widget_dialog_store_response(GTK_WIDGET(dialog), response);
	widget_signal_executor(GTK_WIDGET(dialog), Attr, "response");
}

static gboolean widget_dialog_set_response(
	GtkWidget *widget, const gchar *value, gboolean emit)
{
	gint response;

	if (!widget_dialog_parse_response(value, &response)) {
		gtkdialog_warning("Invalid dialog response '%s'; ignored.",
			value != NULL ? value : "");
		return FALSE;
	}
	if (emit)
		gtk_dialog_response(GTK_DIALOG(widget), response);
	else
		widget_dialog_store_response(widget, response);
	return TRUE;
}

static void widget_dialog_input_by_command(
	variable *var, const gchar *command, gboolean emit)
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
		widget_dialog_set_response(var->Widget, line, emit);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_dialog_input_by_file(
	variable *var, const gchar *filename, gboolean emit)
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
		widget_dialog_set_response(var->Widget, line, emit);
		g_free(line);
	}
	fclose(input);
}

void widget_dialog_clear(variable *var)
{
	g_object_set_data_full(G_OBJECT(var->Widget),
		"gtkdialog-dialog-response", g_strdup(""), g_free);
}

GtkWidget *widget_dialog_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *accel_group;
	GtkWidget *content_area;
	GtkWidget *widget;
	stackelement children;
	variable *child_var;
	gboolean default_response_set = FALSE;
	gchar *value;
	gint default_response = GTK_RESPONSE_NONE;
	gint n;
	gint response;

	(void)Attr;
	(void)Type;
	widget = gtk_dialog_new();
	gtk_window_set_title(GTK_WINDOW(widget), PACKAGE);
	gtk_window_set_icon_name(GTK_WINDOW(widget), PACKAGE);
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-dialog-response",
		g_strdup(""), g_free);

	if (attr != NULL &&
		(value = get_tag_attribute(attr, "default-response")) != NULL) {
		if (widget_dialog_parse_response(value, &default_response))
			default_response_set = TRUE;
		else
			gtkdialog_warning("Invalid dialog default-response '%s'; ignored.",
				value);
		kill_tag_attribute(attr, "default-response");
	}
	if (have_geometry_dxdy)
		gtk_widget_set_size_request(widget, geometry_dx, geometry_dy);
	if (have_geometry_xy)
		gtk_window_move(GTK_WINDOW(widget), geometry_x, geometry_y);
	if (option_centering)
		gtk_window_set_position(GTK_WINDOW(widget), GTK_WIN_POS_CENTER_ALWAYS);

	content_area = gtk_dialog_get_content_area(GTK_DIALOG(widget));
	children = pop();
	for (n = 0; n < children.nwidgets; ++n) {
		child_var = find_variable_by_widget(children.widgets[n]);
		value = child_var != NULL && child_var->widget_tag_attr != NULL
			? get_tag_attribute(child_var->widget_tag_attr, "response") : NULL;
		if (value == NULL) {
			gtk_container_add(GTK_CONTAINER(content_area), children.widgets[n]);
			continue;
		}
		if (!GTK_IS_BUTTON(children.widgets[n]) ||
			!widget_dialog_parse_response(value, &response)) {
			gtkdialog_warning("Invalid dialog child response '%s'; child kept "
				"in the content area.", value);
			gtk_container_add(GTK_CONTAINER(content_area), children.widgets[n]);
		} else {
			GTK_WIDGET_SET_FLAGS(children.widgets[n], GTK_CAN_DEFAULT);
			gtk_dialog_add_action_widget(GTK_DIALOG(widget),
				children.widgets[n], response);
		}
		kill_tag_attribute(child_var->widget_tag_attr, "response");
	}
	stackelement_clear(&children);
	if (default_response_set)
		gtk_dialog_set_default_response(GTK_DIALOG(widget), default_response);

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

gchar *widget_dialog_envvar_construct(GtkWidget *widget)
{
	const gchar *value;

	value = g_object_get_data(G_OBJECT(widget), "gtkdialog-dialog-response");
	return g_strdup(value != NULL ? value : "");
}

void widget_dialog_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_dialog_set_response(var->Widget, value, TRUE);
}

void widget_dialog_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_dialog_input_by_command(var, input + 8, initialised);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_dialog_input_by_file(var, input + 5, initialised);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_dialog_set_response(var->Widget,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT), FALSE);
	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
			__func__);
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM)
#if GTK_CHECK_VERSION(2,6,0)
		&& !GTK_IS_ABOUT_DIALOG(var->Widget)
#endif
	)
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(G_OBJECT(var->Widget), "response",
		G_CALLBACK(widget_dialog_response), var->Attributes);
}

void widget_dialog_removeselected(variable *var)
{
	widget_dialog_clear(var);
}

void widget_dialog_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *response;

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
	response = widget_dialog_envvar_construct(var->Widget);
	fputs(response, output);
	g_free(response);
	widget_close_output(output, filename);
}
