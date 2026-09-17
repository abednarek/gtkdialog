/*
 * widget_recentchooser.c:
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
#include "widget_recentchooser.h"

#define RECENT_CHOOSER_MENU_DATA "gtkdialog-recent-chooser-menu"
#define RECENT_CHOOSER_MENU_OWNER_DATA "gtkdialog-recent-chooser-menu-owner"

typedef struct {
	GtkWidget *widget;
	gchar *value;
} RecentChooserPendingSelection;

static GtkWidget *widget_recentchooser_resolve(GtkWidget *widget)
{
	GtkWidget *chooser;

	if (GTK_IS_RECENT_CHOOSER(widget))
		return widget;
	chooser = g_object_get_data(G_OBJECT(widget), RECENT_CHOOSER_MENU_DATA);
	return GTK_IS_RECENT_CHOOSER(chooser) ? chooser : NULL;
}

gboolean widget_recentchooser_is_menu(GtkWidget *widget)
{
	GtkWidget *chooser;

	if (widget == NULL)
		return FALSE;
	chooser = widget_recentchooser_resolve(widget);
	return chooser != NULL && chooser != widget;
}

static GtkWidget *widget_recentchooser_action_source(GtkWidget *chooser)
{
	GtkWidget *owner;

	owner = g_object_get_data(G_OBJECT(chooser),
		RECENT_CHOOSER_MENU_OWNER_DATA);
	return GTK_IS_WIDGET(owner) ? owner : chooser;
}

static void widget_recentchooser_select(
	GtkWidget *widget, const gchar *value)
{
	GError *error;
	gchar **uris;
	gint index;

	if (g_object_get_data(G_OBJECT(widget),
		RECENT_CHOOSER_MENU_OWNER_DATA) != NULL ||
		!GTK_WIDGET_MAPPED(widget)) {
		g_object_set_data_full(G_OBJECT(widget),
			"gtkdialog-recentchooser-pending-selection",
			g_strdup(value != NULL ? value : ""), g_free);
		return;
	}
	gtk_recent_chooser_unselect_all(GTK_RECENT_CHOOSER(widget));
	if (value == NULL || value[0] == '\0')
		return;
	uris = g_strsplit(value, "\n", -1);
	for (index = 0; uris[index] != NULL; ++index) {
		g_strstrip(uris[index]);
		if (uris[index][0] == '\0')
			continue;
		error = NULL;
		if (!gtk_recent_chooser_select_uri(GTK_RECENT_CHOOSER(widget),
			uris[index], &error)) {
			gtkdialog_warning("Recent chooser could not select '%s': %s",
				uris[index], error != NULL ? error->message : "unknown error");
			if (error != NULL)
				g_error_free(error);
		}
	}
	g_strfreev(uris);
}

static gboolean widget_recentchooser_apply_pending(gpointer data)
{
	RecentChooserPendingSelection *pending = data;

	if (pending->widget != NULL)
		widget_recentchooser_select(pending->widget, pending->value);
	return FALSE;
}

static void widget_recentchooser_pending_widget_destroyed(
	gpointer data, GObject *object)
{
	RecentChooserPendingSelection *pending = data;

	(void)object;
	pending->widget = NULL;
}

static void widget_recentchooser_pending_free(gpointer data)
{
	RecentChooserPendingSelection *pending = data;

	if (pending->widget != NULL)
		g_object_weak_unref(G_OBJECT(pending->widget),
			widget_recentchooser_pending_widget_destroyed, pending);
	g_free(pending->value);
	g_free(pending);
}

static void widget_recentchooser_map(GtkWidget *widget, gpointer data)
{
	RecentChooserPendingSelection *pending;
	const gchar *value;

	(void)data;
	value = g_object_get_data(G_OBJECT(widget),
		"gtkdialog-recentchooser-pending-selection");
	if (value != NULL) {
		pending = g_new(RecentChooserPendingSelection, 1);
		pending->widget = widget;
		pending->value = g_strdup(value);
		g_object_weak_ref(G_OBJECT(widget),
			widget_recentchooser_pending_widget_destroyed, pending);
		g_object_set_data(G_OBJECT(widget),
			"gtkdialog-recentchooser-pending-selection", NULL);
		g_idle_add_full(G_PRIORITY_DEFAULT_IDLE,
			widget_recentchooser_apply_pending, pending,
			widget_recentchooser_pending_free);
	}
}

static void widget_recentchooser_add_filter(
	GtkWidget *widget, tag_attr *attr)
{
	GtkRecentFilter *filter;
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
	filter = gtk_recent_filter_new();
	if (name != NULL)
		gtk_recent_filter_set_name(filter, name);
	if (patterns != NULL) {
		items = g_strsplit_set(patterns, ";,", -1);
		for (index = 0; items[index] != NULL; ++index) {
			g_strstrip(items[index]);
			if (items[index][0] != '\0') {
				gtk_recent_filter_add_pattern(filter, items[index]);
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
				gtk_recent_filter_add_mime_type(filter, items[index]);
				populated = TRUE;
			}
		}
		g_strfreev(items);
	}
	if (populated)
		gtk_recent_chooser_add_filter(GTK_RECENT_CHOOSER(widget), filter);
	else
		g_object_unref(filter);
}

static void widget_recentchooser_input_by_command(
	variable *var, const gchar *command)
{
	FILE *input;
	GtkWidget *chooser;
	gchar *contents;

	input = widget_opencommand(command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	contents = widget_read_all(input);
	chooser = widget_recentchooser_resolve(var->Widget);
	if (chooser != NULL)
		widget_recentchooser_select(chooser, contents);
	g_free(contents);
	widget_closecommand(input, command);
}

static void widget_recentchooser_input_by_file(
	variable *var, const gchar *filename)
{
	FILE *input;
	GtkWidget *chooser;
	gchar *contents;

	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}
	contents = widget_read_all(input);
	chooser = widget_recentchooser_resolve(var->Widget);
	if (chooser != NULL)
		widget_recentchooser_select(chooser, contents);
	g_free(contents);
	fclose(input);
}

static void widget_recentchooser_item_activated(
	GtkWidget *widget, AttributeSet *Attr)
{
	/* A menu has no persistent visual selection.  Once GTK activates an item,
	 * its current URI supersedes any logical value supplied by input/default. */
	g_object_set_data(G_OBJECT(widget),
		"gtkdialog-recentchooser-pending-selection", NULL);
	widget_signal_executor(widget_recentchooser_action_source(widget), Attr,
		"item-activated");
}

static void widget_recentchooser_selection_changed(
	GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget_recentchooser_action_source(widget), Attr,
		"selection-changed");
}

void widget_recentchooser_clear(variable *var)
{
	GtkWidget *chooser = widget_recentchooser_resolve(var->Widget);

	if (chooser != NULL)
		widget_recentchooser_select(chooser, "");
}

GtkWidget *widget_recentchooser_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;

	(void)Attr;
	(void)Type;
	widget = gtk_recent_chooser_widget_new();
	g_signal_connect(G_OBJECT(widget), "map",
		G_CALLBACK(widget_recentchooser_map), NULL);
	widget_recentchooser_add_filter(widget, attr);
	if (attr != NULL) {
		kill_tag_attribute(attr, "filter-name");
		kill_tag_attribute(attr, "filter-pattern");
		kill_tag_attribute(attr, "filter-mime-type");
	}
	return widget;
}

static void widget_recentchoosermenu_apply_properties(
	GtkWidget *chooser, tag_attr *attr)
{
	static const gchar *property_names[] = {
		"local-only",
		"show-private",
		"show-not-found",
		"show-tips",
		"show-icons",
		"limit",
		"sort-type",
		"show-numbers"
	};
	tag_attr *properties = NULL;
	gchar *value;
	guint index;

	if (attr == NULL)
		return;
	for (index = 0; index < G_N_ELEMENTS(property_names); ++index) {
		value = get_tag_attribute(attr, property_names[index]);
		if (value == NULL)
			continue;
		if (properties == NULL)
			properties = new_tag_attributeset(
				(gchar *)property_names[index], value);
		else
			add_tag_attribute(properties,
				(gchar *)property_names[index], value);
		/* The logical widget is the owning GtkMenuItem.  Prevent chooser-only
		 * properties from being applied to that wrapper during realization. */
		kill_tag_attribute(attr, property_names[index]);
	}
	widget_set_tag_attributes(chooser, properties);
	tag_attributeset_free(properties);
}

GtkWidget *widget_recentchoosermenu_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *chooser;
	GtkWidget *item;

	(void)Type;
	attributeset_set_if_unset(Attr, ATTR_LABEL, "Recent files");
	item = widget_menuitem_create(Attr, attr, WIDGET_MENUITEM);
	chooser = gtk_recent_chooser_menu_new();
	widget_recentchoosermenu_apply_properties(chooser, attr);
	widget_recentchooser_add_filter(chooser, attr);
	if (attr != NULL) {
		kill_tag_attribute(attr, "filter-name");
		kill_tag_attribute(attr, "filter-pattern");
		kill_tag_attribute(attr, "filter-mime-type");
	}
	g_object_set_data(G_OBJECT(item), RECENT_CHOOSER_MENU_DATA, chooser);
	g_object_set_data(G_OBJECT(chooser), RECENT_CHOOSER_MENU_OWNER_DATA, item);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), chooser);
	widget_visibility_list_add(chooser, NULL);
	return item;
}

gchar *widget_recentchooser_envvar_construct(GtkWidget *widget)
{
	GtkWidget *chooser;
	GString *selection;
	gchar *uri;
	gchar **uris;
	gsize index;
	gsize length;

	chooser = widget_recentchooser_resolve(widget);
	if (chooser == NULL)
		return g_strdup("");
	if (widget_recentchooser_is_menu(widget)) {
		uri = g_object_get_data(G_OBJECT(chooser),
			"gtkdialog-recentchooser-pending-selection");
		if (uri != NULL)
			return g_strdup(uri);
		uri = gtk_recent_chooser_get_current_uri(
			GTK_RECENT_CHOOSER(chooser));
		return uri != NULL ? uri : g_strdup("");
	}
	uris = gtk_recent_chooser_get_uris(GTK_RECENT_CHOOSER(chooser), &length);
	selection = g_string_new(NULL);
	for (index = 0; index < length; ++index) {
		if (index != 0)
			g_string_append_c(selection, '\n');
		g_string_append(selection, uris[index]);
	}
	g_strfreev(uris);
	return g_string_free(selection, FALSE);
}

void widget_recentchooser_fileselect(
	variable *var, const char *name, const char *value)
{
	GtkWidget *chooser;

	(void)name;
	chooser = widget_recentchooser_resolve(var->Widget);
	if (chooser != NULL)
		widget_recentchooser_select(chooser, value);
}

void widget_recentchooser_refresh(variable *var)
{
	GList *element;
	GtkWidget *chooser;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	chooser = widget_recentchooser_resolve(var->Widget);
	if (chooser == NULL)
		return;
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_recentchooser_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_recentchooser_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_recentchooser_select(chooser,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT));
	if (!widget_recentchooser_is_menu(var->Widget) &&
		attributeset_is_avail(var->Attributes, ATTR_LABEL))
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
	g_signal_connect(G_OBJECT(chooser), "item-activated",
		G_CALLBACK(widget_recentchooser_item_activated), var->Attributes);
	g_signal_connect(G_OBJECT(chooser), "selection-changed",
		G_CALLBACK(widget_recentchooser_selection_changed), var->Attributes);
}

void widget_recentchooser_removeselected(variable *var)
{
	widget_recentchooser_clear(var);
}

void widget_recentchooser_save(variable *var)
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
	selection = widget_recentchooser_envvar_construct(var->Widget);
	fputs(selection, output);
	g_free(selection);
	widget_close_output(output, filename);
}
