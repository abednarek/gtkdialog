/*
 * widget_windowselector.c: libwnck window-selector widget.
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
#include "widgets.h"
#include "widget_windowselector.h"

#if HAVE_WNCK
#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#define WINDOWSELECTOR_ACTION_SCREEN_KEY \
	"gtkdialog-windowselector-action-screen"
#define WINDOWSELECTOR_READY_KEY "gtkdialog-windowselector-ready"

static WnckScreen *
widget_windowselector_screen(GtkWidget *widget)
{
	GdkScreen *gdk_screen;

	if (!gtk_widget_has_screen(widget))
		return NULL;
	gdk_screen = gtk_widget_get_screen(widget);
	return wnck_screen_get(gdk_screen_get_number(gdk_screen));
}

static void
widget_windowselector_active_window_changed(WnckScreen *screen,
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

static void
widget_windowselector_connect_screen(GtkWidget *widget, WnckScreen *screen)
{
	WnckScreen *previous;

	previous = g_object_get_data(G_OBJECT(widget),
		WINDOWSELECTOR_ACTION_SCREEN_KEY);
	if (previous == screen)
		return;
	if (previous != NULL)
		g_signal_handlers_disconnect_by_func(previous,
			widget_windowselector_active_window_changed, widget);
	if (screen != NULL)
		g_signal_connect_object(screen, "active-window-changed",
			G_CALLBACK(widget_windowselector_active_window_changed), widget, 0);
	g_object_set_data(G_OBJECT(widget), WINDOWSELECTOR_ACTION_SCREEN_KEY,
		screen);
}

static void
widget_windowselector_realize(GtkWidget *widget, gpointer user_data)
{
	variable *var;
	WnckScreen *screen;

	(void)user_data;
	screen = widget_windowselector_screen(widget);
	if (screen != NULL)
		wnck_screen_force_update(screen);
	if (g_object_get_data(G_OBJECT(widget), WINDOWSELECTOR_READY_KEY) != NULL) {
		widget_windowselector_connect_screen(widget, screen);
		return;
	}
	g_object_set_data(G_OBJECT(widget), WINDOWSELECTOR_READY_KEY,
		GINT_TO_POINTER(TRUE));
	var = find_variable_by_widget(widget);
	if (var == NULL)
		return;
	/* The normal construction refresh precedes parenting, when WnckSelector
	 * deliberately has no screen. Repeat that one initial refresh now that
	 * realize has selected the actual Gdk screen. */
	g_object_set_data(G_OBJECT(widget), "_initialised", NULL);
	variables_refresh_widget(var);
}

static gboolean
widget_windowselector_activate_xid(GtkWidget *widget, const gchar *text,
	const gchar *source)
{
	WnckScreen *screen;
	WnckWindow *window;
	gchar *end;
	guint64 xid;

	screen = widget_windowselector_screen(widget);
	if (screen != NULL)
		wnck_screen_force_update(screen);
	errno = 0;
	xid = g_ascii_strtoull(text, &end, 0);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == text || *end != '\0' || xid > G_MAXULONG) {
		gtkdialog_warning("Invalid windowselector XID from %s: '%s'.",
			source, text);
		return FALSE;
	}
	window = wnck_window_get((gulong)xid);
	if (window == NULL) {
		gtkdialog_warning(
			"Windowselector XID from %s is not a managed window: %s.",
			source, text);
		return FALSE;
	}
	wnck_window_activate(window, gtk_get_current_event_time());
	return TRUE;
}
#endif

GtkWidget *
widget_windowselector_create(AttributeSet *attributes,
	tag_attr *tag_attributes, gint type)
{
	GtkWidget *widget;

	(void)attributes;
	(void)tag_attributes;
	(void)type;
#if HAVE_WNCK
	widget = wnck_selector_new();
	g_signal_connect(widget, "realize",
		G_CALLBACK(widget_windowselector_realize), NULL);
#else
	widget = gtk_label_new("libwnck 2 window selector unavailable");
	gtkdialog_warning(
		"Windowselector requested, but this build does not include libwnck 2 support.");
#endif
	return widget;
}

gchar *
widget_windowselector_envvar_construct(GtkWidget *widget)
{
#if HAVE_WNCK
	WnckScreen *screen;
	WnckWindow *window;

	screen = widget_windowselector_screen(widget);
	window = screen != NULL ? wnck_screen_get_active_window(screen) : NULL;
	if (window != NULL)
		return g_strdup_printf("%lu", wnck_window_get_xid(window));
#else
	(void)widget;
#endif
	return g_strdup("");
}

void
widget_windowselector_fileselect(variable *var, const char *name,
	const char *value)
{
	(void)name;
#if HAVE_WNCK
	widget_windowselector_activate_xid(var->Widget, value, "set value");
#else
	(void)var;
	(void)value;
#endif
}

void
widget_windowselector_refresh(variable *var)
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

	screen = widget_windowselector_screen(var->Widget);
	if (screen == NULL)
		return;
	wnck_screen_force_update(screen);
	widget_windowselector_connect_screen(var->Widget, screen);
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
				gtkdialog_warning(
					"Could not open windowselector input '%s': %s.",
					directive + 5, g_strerror(errno));
		}
		if (input != NULL) {
			line = widget_read_line(input);
			if (line != NULL) {
				widget_windowselector_activate_xid(var->Widget, line,
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
		widget_windowselector_activate_xid(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			"default");
#else
	(void)initialised;
#endif
}

void
widget_windowselector_save(variable *var)
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
	value = widget_windowselector_envvar_construct(var->Widget);
	fprintf(output, "%s\n", value);
	g_free(value);
	widget_close_output(output, directive + 5);
}

void
widget_windowselector_clear(variable *var)
{
	(void)var;
}

void
widget_windowselector_removeselected(variable *var)
{
	widget_windowselector_clear(var);
}
