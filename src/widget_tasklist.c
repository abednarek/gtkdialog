/*
 * widget_tasklist.c: libwnck task-list widget.
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
#include "widget_tasklist.h"

#if HAVE_WNCK
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#endif

#if !HAVE_WNCK
static const gchar *tasklist_attributes[] = {
	"screen", "grouping", "grouping-limit", "include-all-workspaces",
	"switch-workspace-on-unminimize", "button-relief", "minimum-width",
	"minimum-height", NULL
};

static void
widget_tasklist_consume_attributes(tag_attr *attributes)
{
	gint index;

	if (attributes == NULL)
		return;
	for (index = 0; tasklist_attributes[index] != NULL; ++index)
		if (get_tag_attribute(attributes, tasklist_attributes[index]) != NULL)
			kill_tag_attribute(attributes, tasklist_attributes[index]);
}
#endif

#if HAVE_WNCK
static WnckScreen *
widget_tasklist_screen(tag_attr *attributes)
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
		"tasklist screen");
	kill_tag_attribute(attributes, "screen");
	return wnck_screen_get(number);
}

static gboolean
widget_tasklist_boolean(tag_attr *attributes, const gchar *name,
	gboolean fallback)
{
	gboolean result;
	gchar *value;

	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, name)) == NULL)
		return fallback;
	if (!widget_parse_boolean(value, &result)) {
		gtkdialog_warning("Invalid tasklist %s value '%s'; using %s.",
			name, value, fallback ? "true" : "false");
		result = fallback;
	}
	kill_tag_attribute(attributes, name);
	return result;
}

static WnckTasklistGroupingType
widget_tasklist_grouping(tag_attr *attributes)
{
	gchar *value;

	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, "grouping")) == NULL)
		return WNCK_TASKLIST_AUTO_GROUP;
	kill_tag_attribute(attributes, "grouping");
	if (g_ascii_strcasecmp(value, "never") == 0)
		return WNCK_TASKLIST_NEVER_GROUP;
	if (g_ascii_strcasecmp(value, "always") == 0)
		return WNCK_TASKLIST_ALWAYS_GROUP;
	if (g_ascii_strcasecmp(value, "auto") != 0)
		gtkdialog_warning("Invalid tasklist grouping '%s'; using auto.", value);
	return WNCK_TASKLIST_AUTO_GROUP;
}

static GtkReliefStyle
widget_tasklist_relief(tag_attr *attributes)
{
	gchar *value;

	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, "button-relief")) == NULL)
		return GTK_RELIEF_NORMAL;
	kill_tag_attribute(attributes, "button-relief");
	if (g_ascii_strcasecmp(value, "none") == 0)
		return GTK_RELIEF_NONE;
	if (g_ascii_strcasecmp(value, "half") == 0)
		return GTK_RELIEF_HALF;
	if (g_ascii_strcasecmp(value, "normal") != 0)
		gtkdialog_warning(
			"Invalid tasklist button-relief '%s'; using normal.", value);
	return GTK_RELIEF_NORMAL;
}

static void
widget_tasklist_set_optional_size(WnckTasklist *tasklist,
	tag_attr *attributes, const gchar *name, gboolean width)
{
	gchar *value;
	gint size;

	if (attributes == NULL ||
		(value = get_tag_attribute(attributes, name)) == NULL)
		return;
	size = widget_parse_nonnegative_integer(value, 0, name);
	kill_tag_attribute(attributes, name);
	if (width)
		wnck_tasklist_set_minimum_width(tasklist, size);
	else
		wnck_tasklist_set_minimum_height(tasklist, size);
}

static void
widget_tasklist_active_window_changed(WnckScreen *screen,
	WnckWindow *previous, GtkWidget *widget)
{
	variable *var;

	(void)screen;
	(void)previous;
	var = find_variable_by_widget(widget);
	if (var != NULL && var->Attributes != NULL)
		widget_signal_executor(widget, var->Attributes,
			"active-window-changed");
}

static gboolean
widget_tasklist_activate_xid(GtkWidget *widget, const gchar *text,
	const gchar *source)
{
	WnckWindow *window;
	gchar *end;
	guint64 xid;

	(void)widget;
	errno = 0;
	xid = g_ascii_strtoull(text, &end, 0);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == text || *end != '\0' || xid > G_MAXULONG) {
		gtkdialog_warning("Invalid tasklist XID from %s: '%s'.", source,
			text);
		return FALSE;
	}
	window = wnck_window_get((gulong)xid);
	if (window == NULL) {
		gtkdialog_warning("Tasklist XID from %s is not a managed window: %s.",
			source, text);
		return FALSE;
	}
	wnck_window_activate(window, gtk_get_current_event_time());
	return TRUE;
}
#endif

GtkWidget *
widget_tasklist_create(AttributeSet *attributes, tag_attr *tag_attributes,
	gint type)
{
	GtkWidget *widget;

	(void)attributes;
	(void)type;
#if HAVE_WNCK
	WnckScreen *screen;
	WnckTasklist *tasklist;
	gchar *value;
	gint grouping_limit;

	screen = widget_tasklist_screen(tag_attributes);
	widget = wnck_tasklist_new(screen);
	tasklist = WNCK_TASKLIST(widget);
	wnck_tasklist_set_grouping(tasklist,
		widget_tasklist_grouping(tag_attributes));
	if (tag_attributes != NULL &&
		(value = get_tag_attribute(tag_attributes, "grouping-limit")) != NULL) {
		grouping_limit = widget_parse_nonnegative_integer(value, 0,
			"tasklist grouping-limit");
		wnck_tasklist_set_grouping_limit(tasklist, grouping_limit);
		kill_tag_attribute(tag_attributes, "grouping-limit");
	}
	wnck_tasklist_set_include_all_workspaces(tasklist,
		widget_tasklist_boolean(tag_attributes, "include-all-workspaces",
			FALSE));
	wnck_tasklist_set_switch_workspace_on_unminimize(tasklist,
		widget_tasklist_boolean(tag_attributes,
			"switch-workspace-on-unminimize", TRUE));
	wnck_tasklist_set_button_relief(tasklist,
		widget_tasklist_relief(tag_attributes));
	widget_tasklist_set_optional_size(tasklist, tag_attributes,
		"minimum-width", TRUE);
	widget_tasklist_set_optional_size(tasklist, tag_attributes,
		"minimum-height", FALSE);
	g_object_set_data(G_OBJECT(widget), "gtkdialog-wnck-screen", screen);
#else
	widget_tasklist_consume_attributes(tag_attributes);
	widget = gtk_label_new("libwnck 2 task list unavailable");
	gtkdialog_warning(
		"Tasklist requested, but this build does not include libwnck 2 support.");
#endif
	return widget;
}

gchar *
widget_tasklist_envvar_construct(GtkWidget *widget)
{
#if HAVE_WNCK
	WnckScreen *screen;
	WnckWindow *window;

	screen = g_object_get_data(G_OBJECT(widget), "gtkdialog-wnck-screen");
	window = screen != NULL ? wnck_screen_get_active_window(screen) : NULL;
	if (window != NULL)
		return g_strdup_printf("%lu", wnck_window_get_xid(window));
#else
	(void)widget;
#endif
	return g_strdup("");
}

void
widget_tasklist_fileselect(variable *var, const char *name, const char *value)
{
	(void)name;
#if HAVE_WNCK
	widget_tasklist_activate_xid(var->Widget, value, "set value");
#else
	(void)var;
	(void)value;
#endif
}

void
widget_tasklist_refresh(variable *var)
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
				gtkdialog_warning("Could not open tasklist input '%s': %s.",
					directive + 5, g_strerror(errno));
		}
		if (input != NULL) {
			line = widget_read_line(input);
			if (line != NULL) {
				widget_tasklist_activate_xid(var->Widget, line,
					"input");
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
		widget_tasklist_activate_xid(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			"default");
	if (!initialised)
		g_signal_connect_object(screen, "active-window-changed",
			G_CALLBACK(widget_tasklist_active_window_changed), var->Widget, 0);
#else
	(void)initialised;
#endif
}

void
widget_tasklist_save(variable *var)
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
	value = widget_tasklist_envvar_construct(var->Widget);
	fprintf(output, "%s\n", value);
	g_free(value);
	widget_close_output(output, directive + 5);
}

void
widget_tasklist_clear(variable *var)
{
	(void)var;
}

void
widget_tasklist_removeselected(variable *var)
{
	widget_tasklist_clear(var);
}
