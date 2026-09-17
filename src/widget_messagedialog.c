/*
 * widget_messagedialog.c:
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
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_messagedialog.h"

typedef struct {
	const gchar *name;
	gint value;
} WidgetMessageDialogResponse;

static const WidgetMessageDialogResponse widget_messagedialog_responses[] = {
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

static gboolean widget_messagedialog_parse_response(
	const gchar *text, gint *response)
{
	char *end;
	long number;
	guint index;

	if (text == NULL || text[0] == '\0')
		return FALSE;
	for (index = 0;
		index < G_N_ELEMENTS(widget_messagedialog_responses); ++index) {
		if (g_ascii_strcasecmp(text,
			widget_messagedialog_responses[index].name) == 0) {
			*response = widget_messagedialog_responses[index].value;
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

static gchar *widget_messagedialog_response_name(gint response)
{
	guint index;

	for (index = 0;
		index < G_N_ELEMENTS(widget_messagedialog_responses); ++index) {
		if (response == widget_messagedialog_responses[index].value)
			return g_strdup(widget_messagedialog_responses[index].name);
	}
	return g_strdup_printf("%d", response);
}

static void widget_messagedialog_store_response(
	GtkWidget *widget, gint response)
{
	g_object_set_data_full(G_OBJECT(widget),
		"gtkdialog-messagedialog-response",
		widget_messagedialog_response_name(response), g_free);
}

static void widget_messagedialog_response(
	GtkDialog *dialog, gint response, AttributeSet *Attr)
{
	widget_messagedialog_store_response(GTK_WIDGET(dialog), response);
	widget_signal_executor(GTK_WIDGET(dialog), Attr, "response");
}

static gboolean widget_messagedialog_set_response(
	GtkWidget *widget, const gchar *value, gboolean emit)
{
	gint response;

	if (!widget_messagedialog_parse_response(value, &response)) {
		gtkdialog_warning("Invalid message dialog response '%s'; ignored.",
			value != NULL ? value : "");
		return FALSE;
	}
	if (emit)
		gtk_dialog_response(GTK_DIALOG(widget), response);
	else
		widget_messagedialog_store_response(widget, response);
	return TRUE;
}

static gboolean widget_messagedialog_parse_message_type(
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

static gboolean widget_messagedialog_parse_buttons(
	const gchar *value, GtkButtonsType *buttons)
{
	if (g_ascii_strcasecmp(value, "none") == 0)
		*buttons = GTK_BUTTONS_NONE;
	else if (g_ascii_strcasecmp(value, "ok") == 0)
		*buttons = GTK_BUTTONS_OK;
	else if (g_ascii_strcasecmp(value, "close") == 0)
		*buttons = GTK_BUTTONS_CLOSE;
	else if (g_ascii_strcasecmp(value, "cancel") == 0)
		*buttons = GTK_BUTTONS_CANCEL;
	else if (g_ascii_strcasecmp(value, "yes-no") == 0)
		*buttons = GTK_BUTTONS_YES_NO;
	else if (g_ascii_strcasecmp(value, "ok-cancel") == 0)
		*buttons = GTK_BUTTONS_OK_CANCEL;
	else
		return FALSE;
	return TRUE;
}

static void widget_messagedialog_input_by_command(
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
		widget_messagedialog_set_response(var->Widget, line, emit);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_messagedialog_input_by_file(
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
		widget_messagedialog_set_response(var->Widget, line, emit);
		g_free(line);
	}
	fclose(input);
}

void widget_messagedialog_clear(variable *var)
{
	g_object_set_data_full(G_OBJECT(var->Widget),
		"gtkdialog-messagedialog-response", g_strdup(""), g_free);
}

GtkWidget *widget_messagedialog_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *element;
	GtkButtonsType buttons = GTK_BUTTONS_OK;
	GtkMessageType message_type = GTK_MESSAGE_INFO;
	GtkWidget *widget;
	const gchar *primary;
	gchar *secondary = NULL;
	gchar *value;
	gint response;

	(void)Type;
	if (attr != NULL &&
		(value = get_tag_attribute(attr, "message-type")) != NULL) {
		if (!widget_messagedialog_parse_message_type(value, &message_type))
			gtkdialog_warning("Invalid message dialog message-type '%s'; "
				"using info.", value);
		kill_tag_attribute(attr, "message-type");
	}
	if (attr != NULL &&
		(value = get_tag_attribute(attr, "buttons")) != NULL) {
		if (!widget_messagedialog_parse_buttons(value, &buttons))
			gtkdialog_warning("Invalid message dialog buttons value '%s'; "
				"using ok.", value);
		kill_tag_attribute(attr, "buttons");
	}
	if (attr != NULL &&
		(value = get_tag_attribute(attr, "secondary-text")) != NULL) {
		secondary = g_strdup(value);
		kill_tag_attribute(attr, "secondary-text");
	}

	primary = attributeset_is_avail(Attr, ATTR_LABEL)
		? attributeset_get_first(&element, Attr, ATTR_LABEL) : "";
	widget = gtk_message_dialog_new(NULL, 0, message_type, buttons,
		"%s", primary);
	gtk_window_set_title(GTK_WINDOW(widget), PACKAGE);
	gtk_window_set_icon_name(GTK_WINDOW(widget), PACKAGE);
	g_object_set_data_full(G_OBJECT(widget),
		"gtkdialog-messagedialog-response", g_strdup(""), g_free);
	if (secondary != NULL)
		gtk_message_dialog_format_secondary_text(
			GTK_MESSAGE_DIALOG(widget), "%s", secondary);
	g_free(secondary);

	if (attr != NULL &&
		(value = get_tag_attribute(attr, "default-response")) != NULL) {
		if (widget_messagedialog_parse_response(value, &response))
			gtk_dialog_set_default_response(GTK_DIALOG(widget), response);
		else
			gtkdialog_warning("Invalid message dialog default-response '%s'; "
				"ignored.", value);
		kill_tag_attribute(attr, "default-response");
	}
	if (have_geometry_dxdy)
		gtk_widget_set_size_request(widget, geometry_dx, geometry_dy);
	if (have_geometry_xy)
		gtk_window_move(GTK_WINDOW(widget), geometry_x, geometry_y);
	if (option_centering)
		gtk_window_set_position(GTK_WINDOW(widget), GTK_WIN_POS_CENTER_ALWAYS);
	return widget;
}

gchar *widget_messagedialog_envvar_construct(GtkWidget *widget)
{
	const gchar *value;

	value = g_object_get_data(G_OBJECT(widget),
		"gtkdialog-messagedialog-response");
	return g_strdup(value != NULL ? value : "");
}

void widget_messagedialog_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_messagedialog_set_response(var->Widget, value, TRUE);
}

void widget_messagedialog_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_messagedialog_input_by_command(var, input + 8, initialised);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_messagedialog_input_by_file(
				var, input + 5, initialised);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_messagedialog_set_response(var->Widget,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT), FALSE);
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(G_OBJECT(var->Widget), "response",
		G_CALLBACK(widget_messagedialog_response), var->Attributes);
}

void widget_messagedialog_removeselected(variable *var)
{
	widget_messagedialog_clear(var);
}

void widget_messagedialog_save(variable *var)
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
	response = widget_messagedialog_envvar_construct(var->Widget);
	fputs(response, output);
	g_free(response);
	widget_close_output(output, filename);
}
