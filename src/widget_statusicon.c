/*
 * widget_statusicon.c:
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
#include "widget_menuitem.h"
#include "widget_statusicon.h"

#define STATUS_ICON_DATA "gtkdialog-status-icon"
#define STATUS_ICON_SOURCE_DATA "gtkdialog-status-icon-source"
#define STATUS_ICON_SIGNALS_DATA "gtkdialog-status-icon-signals"
#define STATUS_ICON_TOOLTIP_DATA "gtkdialog-status-icon-tooltip"

static GtkStatusIcon *widget_statusicon_get(GtkWidget *widget)
{
	gpointer icon;

	if (widget == NULL)
		return NULL;
	icon = g_object_get_data(G_OBJECT(widget), STATUS_ICON_DATA);
	return GTK_IS_STATUS_ICON(icon) ? GTK_STATUS_ICON(icon) : NULL;
}

gboolean widget_statusicon_is_proxy(GtkWidget *widget)
{
	return widget_statusicon_get(widget) != NULL;
}

static void widget_statusicon_set_source(
	GtkWidget *widget, const gchar *source)
{
	GtkStatusIcon *icon;
	gchar *copy;
	gchar *resolved;
	const gchar *value;

	icon = widget_statusicon_get(widget);
	if (icon == NULL)
		return;
	copy = g_strdup(source != NULL ? source : "");
	g_strstrip(copy);
	value = copy;
	if (*value == '\0') {
		gtk_status_icon_set_from_pixbuf(icon, NULL);
	} else if (g_ascii_strncasecmp(value, "stock:", 6) == 0) {
		gtk_status_icon_set_from_stock(icon, value + 6);
	} else if (g_ascii_strncasecmp(value, "file:", 5) == 0) {
		resolved = find_pixmap(value + 5);
		gtk_status_icon_set_from_file(icon, resolved);
		g_free(resolved);
	} else if (g_ascii_strncasecmp(value, "icon:", 5) == 0) {
		gtk_status_icon_set_from_icon_name(icon, value + 5);
	} else {
		gtk_status_icon_set_from_icon_name(icon, value);
	}
	g_object_set_data_full(G_OBJECT(widget), STATUS_ICON_SOURCE_DATA,
		copy, g_free);
}

static void widget_statusicon_set_tooltip(
	GtkStatusIcon *icon, const gchar *text, gboolean markup)
{
	if (markup)
		gtk_status_icon_set_tooltip_markup(icon, text);
	else
		gtk_status_icon_set_tooltip_text(icon, text);
}

static AttributeSet *widget_statusicon_attributes(GtkWidget *widget)
{
	return g_object_get_data(G_OBJECT(widget), "gtkdialog-attributes");
}

static void widget_statusicon_proxy_shown(
	GtkWidget *widget, gpointer data)
{
	(void)widget;
	gtk_status_icon_set_visible(GTK_STATUS_ICON(data), TRUE);
}

static void widget_statusicon_proxy_hidden(
	GtkWidget *widget, gpointer data)
{
	(void)widget;
	gtk_status_icon_set_visible(GTK_STATUS_ICON(data), FALSE);
}

static void widget_statusicon_activated(
	GtkStatusIcon *icon, gpointer data)
{
	GtkWidget *widget = data;
	AttributeSet *Attr;

	(void)icon;
	if (!gtk_widget_get_visible(widget) ||
		!gtk_widget_get_sensitive(widget))
		return;
	Attr = widget_statusicon_attributes(widget);
	if (Attr != NULL)
		widget_signal_executor(widget, Attr, "activate");
}

static gboolean widget_statusicon_button_event(GtkStatusIcon *icon,
	GdkEventButton *event, gpointer data)
{
	GtkWidget *widget = data;
	AttributeSet *Attr;
	gchar *button;
	gchar *ptr_mod;
	gchar *ptr_x;
	gchar *ptr_x_root;
	gchar *ptr_y;
	gchar *ptr_y_root;
	const gchar *signal_name;

	(void)icon;
	if (!gtk_widget_get_visible(widget) ||
		!gtk_widget_get_sensitive(widget))
		return FALSE;
	signal_name = event->type == GDK_BUTTON_RELEASE
		? "button-release-event" : "button-press-event";
	button = g_strdup_printf("%u", event->button);
	ptr_mod = g_strdup_printf("%u", event->state);
	ptr_x = g_strdup_printf("%.0f", event->x);
	ptr_y = g_strdup_printf("%.0f", event->y);
	ptr_x_root = g_strdup_printf("%.0f", event->x_root);
	ptr_y_root = g_strdup_printf("%.0f", event->y_root);
	g_setenv("BUTTON", button, TRUE);
	g_setenv("PTR_BTN", button, TRUE);
	g_setenv("PTR_MOD", ptr_mod, TRUE);
	g_setenv("PTR_X", ptr_x, TRUE);
	g_setenv("PTR_Y", ptr_y, TRUE);
	g_setenv("PTR_X_ROOT", ptr_x_root, TRUE);
	g_setenv("PTR_Y_ROOT", ptr_y_root, TRUE);
	Attr = widget_statusicon_attributes(widget);
	if (Attr != NULL)
		widget_signal_executor(widget, Attr, signal_name);
	g_unsetenv("BUTTON");
	g_unsetenv("PTR_BTN");
	g_unsetenv("PTR_MOD");
	g_unsetenv("PTR_X");
	g_unsetenv("PTR_Y");
	g_unsetenv("PTR_X_ROOT");
	g_unsetenv("PTR_Y_ROOT");
	g_free(button);
	g_free(ptr_mod);
	g_free(ptr_x);
	g_free(ptr_y);
	g_free(ptr_x_root);
	g_free(ptr_y_root);
	return FALSE;
}

static gboolean widget_statusicon_size_changed(GtkStatusIcon *icon,
	gint size, gpointer data)
{
	GtkWidget *widget = data;
	AttributeSet *Attr;

	(void)icon;
	(void)size;
	if (!gtk_widget_get_visible(widget))
		return FALSE;
	Attr = widget_statusicon_attributes(widget);
	if (Attr != NULL)
		widget_signal_executor(widget, Attr, "size-changed");
	return FALSE;
}

static void widget_statusicon_popup_menu(GtkStatusIcon *icon,
	guint button, guint32 activate_time, gpointer data)
{
	GtkWidget *widget = data;
	AttributeSet *Attr;
	const gchar *stored_program;
	gchar *program = NULL;
	gchar *button_value;

	if (!gtk_widget_get_visible(widget) ||
		!gtk_widget_get_sensitive(widget))
		return;
	stored_program = g_object_get_data(G_OBJECT(widget), "context-menu");
	if (stored_program != NULL)
		program = g_strdup(stored_program);
	button_value = g_strdup_printf("%u", button);
	g_setenv("BUTTON", button_value, TRUE);
	g_setenv("PTR_BTN", button_value, TRUE);
	g_object_ref(widget);
	Attr = widget_statusicon_attributes(widget);
	if (Attr != NULL)
		widget_signal_executor(widget, Attr, "popup-menu");
	if (program != NULL && find_variable_by_widget(widget) != NULL)
		widget_menu_popup_context_positioned(widget, program,
			gtk_status_icon_position_menu, icon, button, activate_time);
	g_object_unref(widget);
	g_unsetenv("BUTTON");
	g_unsetenv("PTR_BTN");
	g_free(button_value);
	g_free(program);
}

gboolean widget_statusicon_activate(GtkWidget *widget)
{
	GtkStatusIcon *icon;

	icon = widget_statusicon_get(widget);
	if (icon == NULL)
		return FALSE;
	g_signal_emit_by_name(icon, "activate");
	return TRUE;
}

static void widget_statusicon_input_by_command(
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
		widget_statusicon_set_source(var->Widget, line);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_statusicon_input_by_file(
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
		widget_statusicon_set_source(var->Widget, line);
		g_free(line);
	}
	fclose(input);
}

void widget_statusicon_clear(variable *var)
{
	widget_statusicon_set_source(var->Widget, "");
}

GtkWidget *widget_statusicon_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkStatusIcon *icon;
	GtkWidget *widget;
	gchar *value;
	gchar *source = NULL;
	gboolean blinking;

	(void)Attr;
	(void)Type;
	widget = gtk_alignment_new(0.0, 0.0, 0.0, 0.0);
	gtk_widget_set_size_request(widget, 0, 0);
	icon = gtk_status_icon_new();
	gtk_status_icon_set_visible(icon, FALSE);
	g_object_set_data_full(G_OBJECT(widget), STATUS_ICON_DATA, icon,
		g_object_unref);

	if (attr != NULL) {
		if ((value = get_tag_attribute(attr, "image-name")) != NULL ||
			(value = get_tag_attribute(attr, "image-file")) != NULL ||
			(value = get_tag_attribute(attr, "file")) != NULL)
			source = g_strdup_printf("file:%s", value);
		else if ((value = get_tag_attribute(attr, "stock-id")) != NULL ||
			(value = get_tag_attribute(attr, "stock")) != NULL)
			source = g_strdup_printf("stock:%s", value);
		else if ((value = get_tag_attribute(attr, "icon-name")) != NULL ||
			(value = get_tag_attribute(attr, "icon")) != NULL)
			source = g_strdup_printf("icon:%s", value);
		if ((value = get_tag_attribute(attr, "tooltip-markup")) != NULL) {
			widget_statusicon_set_tooltip(icon, value, TRUE);
			g_object_set_data(G_OBJECT(widget), STATUS_ICON_TOOLTIP_DATA,
				GINT_TO_POINTER(TRUE));
		} else if ((value = get_tag_attribute(attr, "tooltip-text")) != NULL ||
			(value = get_tag_attribute(attr, "tooltip")) != NULL) {
			widget_statusicon_set_tooltip(icon, value, FALSE);
			g_object_set_data(G_OBJECT(widget), STATUS_ICON_TOOLTIP_DATA,
				GINT_TO_POINTER(TRUE));
		}
		if ((value = get_tag_attribute(attr, "blinking")) != NULL &&
			widget_parse_boolean(value, &blinking))
			gtk_status_icon_set_blinking(icon, blinking);
		if ((value = get_tag_attribute(attr, "title")) != NULL)
			gtk_status_icon_set_title(icon, value);
		kill_tag_attribute(attr, "image-name");
		kill_tag_attribute(attr, "image-file");
		kill_tag_attribute(attr, "file");
		kill_tag_attribute(attr, "stock-id");
		kill_tag_attribute(attr, "stock");
		kill_tag_attribute(attr, "icon-name");
		kill_tag_attribute(attr, "icon");
		kill_tag_attribute(attr, "tooltip-markup");
		kill_tag_attribute(attr, "tooltip-text");
		kill_tag_attribute(attr, "tooltip");
		kill_tag_attribute(attr, "blinking");
		kill_tag_attribute(attr, "title");
	}
	widget_statusicon_set_source(widget, source);
	g_free(source);
	return widget;
}

gchar *widget_statusicon_envvar_construct(GtkWidget *widget)
{
	const gchar *source;

	source = g_object_get_data(G_OBJECT(widget), STATUS_ICON_SOURCE_DATA);
	return g_strdup(source != NULL ? source : "");
}

void widget_statusicon_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_statusicon_set_source(var->Widget, value);
}

void widget_statusicon_refresh(variable *var)
{
	GList *element;
	GtkStatusIcon *icon;
	gchar *input;
	gchar *label;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_statusicon_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_statusicon_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_statusicon_set_source(var->Widget,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT));
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	icon = widget_statusicon_get(var->Widget);
	if (icon == NULL)
		return;
	if (g_object_get_data(G_OBJECT(var->Widget),
		STATUS_ICON_TOOLTIP_DATA) == NULL &&
		attributeset_is_avail(var->Attributes, ATTR_LABEL)) {
		label = attributeset_get_first(&element,
			var->Attributes, ATTR_LABEL);
		widget_statusicon_set_tooltip(icon, label, FALSE);
	}
	if (g_object_get_data(G_OBJECT(var->Widget),
		STATUS_ICON_SIGNALS_DATA) == NULL) {
		g_signal_connect(G_OBJECT(var->Widget), "show",
			G_CALLBACK(widget_statusicon_proxy_shown), icon);
		g_signal_connect(G_OBJECT(var->Widget), "hide",
			G_CALLBACK(widget_statusicon_proxy_hidden), icon);
		g_signal_connect(G_OBJECT(icon), "activate",
			G_CALLBACK(widget_statusicon_activated), var->Widget);
		g_signal_connect(G_OBJECT(icon), "button-press-event",
			G_CALLBACK(widget_statusicon_button_event), var->Widget);
		g_signal_connect(G_OBJECT(icon), "button-release-event",
			G_CALLBACK(widget_statusicon_button_event), var->Widget);
		g_signal_connect(G_OBJECT(icon), "popup-menu",
			G_CALLBACK(widget_statusicon_popup_menu), var->Widget);
		g_signal_connect(G_OBJECT(icon), "size-changed",
			G_CALLBACK(widget_statusicon_size_changed), var->Widget);
		g_object_set_data(G_OBJECT(var->Widget), STATUS_ICON_SIGNALS_DATA,
			GINT_TO_POINTER(TRUE));
	}
	gtk_status_icon_set_visible(icon,
		gtk_widget_get_visible(var->Widget));
}

void widget_statusicon_removeselected(variable *var)
{
	widget_statusicon_clear(var);
}

void widget_statusicon_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *source;

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
	source = widget_statusicon_envvar_construct(var->Widget);
	fputs(source, output);
	g_free(source);
	widget_close_output(output, filename);
}
