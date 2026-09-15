/*
 * widget_pager.c: libwnck workspace-pager widget.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <errno.h>
#include <gtk/gtk.h>

#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_pager.h"

#if HAVE_WNCK
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#endif

#define PAGER_ORIENTATION_KEY "gtkdialog-pager-orientation"
#define PAGER_ROWS_KEY "gtkdialog-pager-rows"

#if !HAVE_WNCK
static const gchar *pager_attributes[] = {
	"screen", "orientation", "rows", "display-mode",
	"show-all-workspaces", "shadow-type", NULL
};

static void
widget_pager_consume_attributes(tag_attr *attributes)
{
	gint index;

	if (attributes == NULL)
		return;
	for (index = 0; pager_attributes[index] != NULL; ++index)
		if (get_tag_attribute(attributes, pager_attributes[index]) != NULL)
			kill_tag_attribute(attributes, pager_attributes[index]);
}
#endif

#if HAVE_WNCK
static WnckScreen *
widget_pager_screen(tag_attr *attributes)
{
	GdkDisplay *display;
	WnckScreen *screen;
	gchar *value;
	gint number;
	gint screens;

	screen = wnck_screen_get_default();
	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, "screen")) == NULL)
		return screen;
	display = gdk_display_get_default();
	screens = display != NULL ? gdk_display_get_n_screens(display) : 1;
	number = widget_parse_bounded_integer(value, 0, MAX(0, screens - 1), 0,
		"pager screen");
	kill_tag_attribute(attributes, "screen");
	return wnck_screen_get(number);
}

static gboolean
widget_pager_boolean(tag_attr *attributes, const gchar *name,
	gboolean fallback)
{
	gboolean result;
	gchar *value;

	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, name)) == NULL)
		return fallback;
	if (!widget_parse_boolean(value, &result)) {
		gtkdialog_warning("Invalid pager %s value '%s'; using %s.",
			name, value, fallback ? "true" : "false");
		result = fallback;
	}
	kill_tag_attribute(attributes, name);
	return result;
}

static WnckPagerDisplayMode
widget_pager_display_mode(tag_attr *attributes)
{
	gchar *value;

	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, "display-mode")) == NULL)
		return WNCK_PAGER_DISPLAY_CONTENT;
	kill_tag_attribute(attributes, "display-mode");
	if (g_ascii_strcasecmp(value, "name") == 0)
		return WNCK_PAGER_DISPLAY_NAME;
	if (g_ascii_strcasecmp(value, "content") != 0)
		gtkdialog_warning(
			"Invalid pager display-mode '%s'; using content.", value);
	return WNCK_PAGER_DISPLAY_CONTENT;
}

static GtkShadowType
widget_pager_shadow_type(tag_attr *attributes)
{
	gchar *value;

	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, "shadow-type")) == NULL)
		return GTK_SHADOW_NONE;
	kill_tag_attribute(attributes, "shadow-type");
	if (g_ascii_strcasecmp(value, "in") == 0)
		return GTK_SHADOW_IN;
	if (g_ascii_strcasecmp(value, "out") == 0)
		return GTK_SHADOW_OUT;
	if (g_ascii_strcasecmp(value, "etched-in") == 0)
		return GTK_SHADOW_ETCHED_IN;
	if (g_ascii_strcasecmp(value, "etched-out") == 0)
		return GTK_SHADOW_ETCHED_OUT;
	if (g_ascii_strcasecmp(value, "none") != 0)
		gtkdialog_warning("Invalid pager shadow-type '%s'; using none.",
			value);
	return GTK_SHADOW_NONE;
}

static void
widget_pager_store_layout(GtkWidget *widget, tag_attr *attributes)
{
	gchar *value;
	gint orientation;
	gint rows;

	if (attributes == NULL)
		return;
	value = get_tag_attribute(attributes, "orientation");
	if (value != NULL) {
		orientation = GTK_ORIENTATION_HORIZONTAL;
		if (g_ascii_strcasecmp(value, "vertical") == 0)
			orientation = GTK_ORIENTATION_VERTICAL;
		else if (g_ascii_strcasecmp(value, "horizontal") != 0)
			gtkdialog_warning(
				"Invalid pager orientation '%s'; using horizontal.", value);
		g_object_set_data(G_OBJECT(widget), PAGER_ORIENTATION_KEY,
			GINT_TO_POINTER(orientation + 1));
		kill_tag_attribute(attributes, "orientation");
	}
	value = get_tag_attribute(attributes, "rows");
	if (value != NULL) {
		rows = widget_parse_bounded_integer(value, 1, G_MAXINT, 1,
			"pager rows");
		g_object_set_data(G_OBJECT(widget), PAGER_ROWS_KEY,
			GINT_TO_POINTER(rows));
		kill_tag_attribute(attributes, "rows");
	}
}

static void
widget_pager_apply_layout(GtkWidget *widget, gpointer user_data)
{
	gpointer orientation_data;
	gpointer rows_data;

	(void)user_data;
	orientation_data = g_object_get_data(G_OBJECT(widget),
		PAGER_ORIENTATION_KEY);
	rows_data = g_object_get_data(G_OBJECT(widget), PAGER_ROWS_KEY);
	if (orientation_data != NULL)
		wnck_pager_set_orientation(WNCK_PAGER(widget),
			GPOINTER_TO_INT(orientation_data) - 1);
	if (rows_data != NULL)
		wnck_pager_set_n_rows(WNCK_PAGER(widget),
			GPOINTER_TO_INT(rows_data));
}

static void
widget_pager_execute_signal(GtkWidget *widget, const gchar *signal)
{
	variable *var;

	var = find_variable_by_widget(widget);
	if (var != NULL && var->Attributes != NULL)
		widget_signal_executor(widget, var->Attributes, signal);
}

static void
widget_pager_active_workspace_changed(WnckScreen *screen,
	WnckWorkspace *previous, GtkWidget *widget)
{
	(void)screen;
	(void)previous;
	widget_pager_execute_signal(widget, "active-workspace-changed");
}

static void
widget_pager_workspace_created(WnckScreen *screen, WnckWorkspace *workspace,
	GtkWidget *widget)
{
	(void)screen;
	(void)workspace;
	widget_pager_execute_signal(widget, "workspace-created");
}

static void
widget_pager_workspace_destroyed(WnckScreen *screen,
	WnckWorkspace *workspace, GtkWidget *widget)
{
	(void)screen;
	(void)workspace;
	widget_pager_execute_signal(widget, "workspace-destroyed");
}

static gboolean
widget_pager_activate_workspace(GtkWidget *widget, const gchar *text,
	const gchar *source)
{
	WnckScreen *screen;
	WnckWorkspace *workspace;
	gchar *end;
	gint64 number;

	errno = 0;
	number = g_ascii_strtoll(text, &end, 10);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == text || *end != '\0' || number < 0 ||
		number > G_MAXINT) {
		gtkdialog_warning("Invalid pager workspace from %s: '%s'.", source,
			text);
		return FALSE;
	}
	screen = g_object_get_data(G_OBJECT(widget), "gtkdialog-wnck-screen");
	if (screen == NULL)
		return FALSE;
	wnck_screen_force_update(screen);
	workspace = wnck_screen_get_workspace(screen, (gint)number);
	if (workspace == NULL) {
		gtkdialog_warning(
			"Pager workspace from %s is outside the available range: %s.",
			source, text);
		return FALSE;
	}
	wnck_workspace_activate(workspace, gtk_get_current_event_time());
	return TRUE;
}
#endif

GtkWidget *
widget_pager_create(AttributeSet *attributes, tag_attr *tag_attributes,
	gint type)
{
	GtkWidget *widget;

	(void)attributes;
	(void)type;
#if HAVE_WNCK
	WnckScreen *screen;

	screen = widget_pager_screen(tag_attributes);
	widget = wnck_pager_new(screen);
	wnck_pager_set_display_mode(WNCK_PAGER(widget),
		widget_pager_display_mode(tag_attributes));
	wnck_pager_set_show_all(WNCK_PAGER(widget),
		widget_pager_boolean(tag_attributes, "show-all-workspaces", TRUE));
	wnck_pager_set_shadow_type(WNCK_PAGER(widget),
		widget_pager_shadow_type(tag_attributes));
	widget_pager_store_layout(widget, tag_attributes);
	g_object_set_data(G_OBJECT(widget), "gtkdialog-wnck-screen", screen);
	g_signal_connect(widget, "realize", G_CALLBACK(widget_pager_apply_layout),
		NULL);
#else
	widget_pager_consume_attributes(tag_attributes);
	widget = gtk_label_new("libwnck 2 workspace pager unavailable");
	gtkdialog_warning(
		"Pager requested, but this build does not include libwnck 2 support.");
#endif
	return widget;
}

gchar *
widget_pager_envvar_construct(GtkWidget *widget)
{
#if HAVE_WNCK
	WnckScreen *screen;
	WnckWorkspace *workspace;

	screen = g_object_get_data(G_OBJECT(widget), "gtkdialog-wnck-screen");
	workspace = screen != NULL ? wnck_screen_get_active_workspace(screen) : NULL;
	if (workspace != NULL)
		return g_strdup_printf("%d", wnck_workspace_get_number(workspace));
#else
	(void)widget;
#endif
	return g_strdup("");
}

void
widget_pager_fileselect(variable *var, const char *name, const char *value)
{
	(void)name;
#if HAVE_WNCK
	widget_pager_activate_workspace(var->Widget, value, "set value");
#else
	(void)var;
	(void)value;
#endif
}

void
widget_pager_refresh(variable *var)
{
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (!initialised &&
		(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
		gtk_widget_set_sensitive(var->Widget, FALSE);
#if HAVE_WNCK
	GList *element;
	WnckScreen *screen;
	gchar *directive;
	gchar *line;
	FILE *input;

	screen = g_object_get_data(G_OBJECT(var->Widget), "gtkdialog-wnck-screen");
	if (screen == NULL)
		return;
	wnck_screen_force_update(screen);
	directive = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (directive != NULL) {
		input = NULL;
		if (input_is_shell_command(directive))
			input = widget_opencommand(directive + 8);
		else if (g_ascii_strncasecmp(directive, "file:", 5) == 0 &&
			strlen(directive) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, directive + 5);
			input = fopen(directive + 5, "r");
			if (input == NULL)
				gtkdialog_warning("Could not open pager input '%s': %s.",
					directive + 5, g_strerror(errno));
		}
		if (input != NULL) {
			line = widget_read_line(input);
			if (line != NULL) {
				widget_pager_activate_workspace(var->Widget, line, "input");
				g_free(line);
			}
			if (input_is_shell_command(directive))
				widget_closecommand(input, directive + 8);
			else
				fclose(input);
		}
		directive = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (!initialised && attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_pager_activate_workspace(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			"default");
	if (!initialised) {
		g_signal_connect_object(screen, "active-workspace-changed",
			G_CALLBACK(widget_pager_active_workspace_changed), var->Widget, 0);
		g_signal_connect_object(screen, "workspace-created",
			G_CALLBACK(widget_pager_workspace_created), var->Widget, 0);
		g_signal_connect_object(screen, "workspace-destroyed",
			G_CALLBACK(widget_pager_workspace_destroyed), var->Widget, 0);
	}
#else
	(void)initialised;
#endif
}

void
widget_pager_save(variable *var)
{
	GList *element;
	gchar *directive;
	gchar *value;
	FILE *output;

	directive = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (directive != NULL &&
		(g_ascii_strncasecmp(directive, "file:", 5) != 0 ||
		strlen(directive) <= 5))
		directive = attributeset_get_next(&element,
			var->Attributes, ATTR_OUTPUT);
	if (directive == NULL) {
		fprintf(stderr, "%s(): No <output file> directive found.\n", __func__);
		return;
	}
	output = fopen(directive + 5, "w");
	if (output == NULL) {
		fprintf(stderr, "%s(): Could not open '%s' for writing.\n", __func__,
			directive + 5);
		return;
	}
	value = widget_pager_envvar_construct(var->Widget);
	fprintf(output, "%s\n", value);
	g_free(value);
	widget_close_output(output, directive + 5);
}

void
widget_pager_clear(variable *var)
{
	(void)var;
}

void
widget_pager_removeselected(variable *var)
{
	widget_pager_clear(var);
}
