/*
 * widget_infobar.c:
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
#include "widget_infobar.h"

typedef struct {
	const gchar *name;
	gint value;
} WidgetInfobarResponse;

static const WidgetInfobarResponse widget_infobar_responses[] = {
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

static gboolean widget_infobar_parse_response(
	const gchar *text, gint *response)
{
	char *end;
	long number;
	guint index;

	if (text == NULL || text[0] == '\0')
		return FALSE;
	for (index = 0; index < G_N_ELEMENTS(widget_infobar_responses); ++index) {
		if (g_ascii_strcasecmp(text,
			widget_infobar_responses[index].name) == 0) {
			*response = widget_infobar_responses[index].value;
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

static gchar *widget_infobar_response_name(gint response)
{
	guint index;

	for (index = 0; index < G_N_ELEMENTS(widget_infobar_responses); ++index) {
		if (response == widget_infobar_responses[index].value)
			return g_strdup(widget_infobar_responses[index].name);
	}
	return g_strdup_printf("%d", response);
}

static void widget_infobar_store_response(GtkWidget *widget, gint response)
{
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-infobar-response",
		widget_infobar_response_name(response), g_free);
}

static void widget_infobar_response(
	GtkInfoBar *bar, gint response, AttributeSet *Attr)
{
	widget_infobar_store_response(GTK_WIDGET(bar), response);
	widget_signal_executor(GTK_WIDGET(bar), Attr, "response");
}

static gboolean widget_infobar_set_response(
	GtkWidget *widget, const gchar *value, gboolean emit)
{
	gint response;

	if (!widget_infobar_parse_response(value, &response)) {
		gtkdialog_warning("Invalid info bar response '%s'; ignored.",
			value != NULL ? value : "");
		return FALSE;
	}
	if (emit)
		gtk_info_bar_response(GTK_INFO_BAR(widget), response);
	else
		widget_infobar_store_response(widget, response);
	return TRUE;
}

static gboolean widget_infobar_parse_message_type(
	const gchar *value, GtkMessageType *type)
{
	if (g_ascii_strcasecmp(value, "info") == 0)
		*type = GTK_MESSAGE_INFO;
	else if (g_ascii_strcasecmp(value, "warning") == 0)
		*type = GTK_MESSAGE_WARNING;
	else if (g_ascii_strcasecmp(value, "question") == 0)
		*type = GTK_MESSAGE_QUESTION;
	else if (g_ascii_strcasecmp(value, "error") == 0)
		*type = GTK_MESSAGE_ERROR;
	else if (g_ascii_strcasecmp(value, "other") == 0)
		*type = GTK_MESSAGE_OTHER;
	else
		return FALSE;
	return TRUE;
}

static void widget_infobar_read_response(
	variable *var, const gchar *value, gboolean emit)
{
	widget_infobar_set_response(var->Widget, value, emit);
}

static void widget_infobar_input_by_command(
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
		widget_infobar_read_response(var, line, emit);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_infobar_input_by_file(
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
		widget_infobar_read_response(var, line, emit);
		g_free(line);
	}
	fclose(input);
}

void widget_infobar_clear(variable *var)
{
	g_object_set_data_full(G_OBJECT(var->Widget),
		"gtkdialog-infobar-response", g_strdup(""), g_free);
}

GtkWidget *widget_infobar_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkMessageType message_type;
	GtkWidget *content_area;
	GtkWidget *widget;
	stackelement children;
	variable *child_var;
	gchar *value;
	gint n;
	gint response;

	(void)Attr;
	(void)Type;
	widget = gtk_info_bar_new();
	content_area = gtk_info_bar_get_content_area(GTK_INFO_BAR(widget));
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-infobar-response",
		g_strdup(""), g_free);

	if (attr != NULL &&
		(value = get_tag_attribute(attr, "message-type")) != NULL) {
		if (widget_infobar_parse_message_type(value, &message_type))
			gtk_info_bar_set_message_type(GTK_INFO_BAR(widget), message_type);
		else
			gtkdialog_warning("Invalid info bar message-type '%s'; using info.",
				value);
		kill_tag_attribute(attr, "message-type");
	}
	if (attr != NULL &&
		(value = get_tag_attribute(attr, "default-response")) != NULL) {
		if (widget_infobar_parse_response(value, &response))
			gtk_info_bar_set_default_response(GTK_INFO_BAR(widget), response);
		else
			gtkdialog_warning("Invalid info bar default-response '%s'; ignored.",
				value);
		kill_tag_attribute(attr, "default-response");
	}

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
			!widget_infobar_parse_response(value, &response)) {
			gtkdialog_warning("Invalid info bar child response '%s'; child "
				"kept in the content area.", value);
			gtk_container_add(GTK_CONTAINER(content_area), children.widgets[n]);
		} else {
			gtk_info_bar_add_action_widget(GTK_INFO_BAR(widget),
				children.widgets[n], response);
		}
		kill_tag_attribute(child_var->widget_tag_attr, "response");
	}
	stackelement_clear(&children);
	return widget;
}

gchar *widget_infobar_envvar_construct(GtkWidget *widget)
{
	const gchar *value;

	value = g_object_get_data(G_OBJECT(widget), "gtkdialog-infobar-response");
	return g_strdup(value != NULL ? value : "");
}

void widget_infobar_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_infobar_set_response(var->Widget, value, TRUE);
}

void widget_infobar_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_infobar_input_by_command(var, input + 8, initialised);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_infobar_input_by_file(var, input + 5, initialised);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_infobar_set_response(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			FALSE);
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
	g_signal_connect(G_OBJECT(var->Widget), "response",
		G_CALLBACK(widget_infobar_response), var->Attributes);
}

void widget_infobar_removeselected(variable *var)
{
	widget_infobar_clear(var);
}

void widget_infobar_save(variable *var)
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
	response = widget_infobar_envvar_construct(var->Widget);
	fputs(response, output);
	g_free(response);
	widget_close_output(output, filename);
}
