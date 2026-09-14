/*
 * widget_paned.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define _GNU_SOURCE
#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "widget_paned.h"
#include "tag_attributes.h"

typedef struct {
	GtkWidget *widget;
	gint position;
} PanedPendingPosition;

static gboolean widget_paned_apply_pending_position_idle(gpointer data)
{
	PanedPendingPosition *pending;

	pending = data;
	gtk_paned_set_position(
		GTK_PANED(pending->widget), pending->position);
	return FALSE;
}

static void widget_paned_pending_position_free(gpointer data)
{
	PanedPendingPosition *pending;

	pending = data;
	g_object_unref(pending->widget);
	g_free(pending);
}

static void widget_paned_apply_pending_position(
	GtkWidget *widget, GtkAllocation *allocation, gpointer user_data)
{
	PanedPendingPosition *pending;
	gint *position;

	(void)allocation;
	(void)user_data;
	position = g_object_steal_data(
		G_OBJECT(widget), "_gtkdialog-paned-position");
	if (position != NULL) {
		pending = g_new(PanedPendingPosition, 1);
		pending->widget = g_object_ref(widget);
		pending->position = *position;
		g_free(position);
		g_idle_add_full(G_PRIORITY_DEFAULT_IDLE,
			widget_paned_apply_pending_position_idle, pending,
			widget_paned_pending_position_free);
	}
}

static gboolean widget_paned_child_flag(tag_attr *attr,
	const gchar *name, gboolean fallback)
{
	gchar *value;
	gboolean result;

	if (attr == NULL)
		return fallback;

	value = get_tag_attribute(attr, name);
	if (value == NULL)
		return fallback;
	if (widget_parse_boolean(value, &result))
		return result;

	gtkdialog_warning("Invalid %s value '%s'; using %s.", name, value,
		fallback ? "true" : "false");
	return fallback;
}

static void widget_paned_set_position_from_text(variable *var,
	const gchar *text, const gchar *description)
{
	gint current;
	gint *pending_position;
	gint position;

	current = gtk_paned_get_position(GTK_PANED(var->Widget));
	position = widget_parse_bounded_integer(text, 0, G_MAXINT, current,
		description);
	gtk_paned_set_position(GTK_PANED(var->Widget), position);
	if (!GTK_WIDGET_REALIZED(var->Widget) &&
		(var->widget_tag_attr == NULL ||
		get_tag_attribute(var->widget_tag_attr, "position") == NULL)) {
		pending_position = g_new(gint, 1);
		*pending_position = position;
		g_object_set_data_full(G_OBJECT(var->Widget),
			"_gtkdialog-paned-position", pending_position, g_free);
	}
}

static void widget_paned_input_by_command(variable *var, const gchar *command)
{
	FILE *stream;
	gchar *line;

	stream = widget_opencommand(command);
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}

	line = widget_read_line(stream);
	if (line != NULL) {
		widget_paned_set_position_from_text(var, line,
			"paned input position");
		g_free(line);
	}
	widget_closecommand(stream, command);
}

static void widget_paned_input_by_file(variable *var, const gchar *filename)
{
	FILE *stream;
	gchar *line;

	stream = fopen(filename, "r");
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}

	line = widget_read_line(stream);
	if (line != NULL) {
		widget_paned_set_position_from_text(var, line,
			"paned input position");
		g_free(line);
	}
	fclose(stream);
}

void widget_paned_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_paned_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;
	stackelement children;
	gboolean resize;
	gboolean shrink;

	(void)Attr;
	children = pop();
	if (children.nwidgets != 2) {
		stackelement_clear(&children);
		g_error("gtkdialog: <%s> requires exactly two direct child widgets.",
			Type == WIDGET_HPANED ? "hpaned" : "vpaned");
	}

	widget = Type == WIDGET_HPANED ? gtk_hpaned_new() : gtk_vpaned_new();
	g_signal_connect(G_OBJECT(widget), "size-allocate",
		G_CALLBACK(widget_paned_apply_pending_position), NULL);

	resize = widget_paned_child_flag(attr, "child1-resize", FALSE);
	shrink = widget_paned_child_flag(attr, "child1-shrink", TRUE);
	gtk_paned_pack1(GTK_PANED(widget), children.widgets[0], resize, shrink);

	resize = widget_paned_child_flag(attr, "child2-resize", TRUE);
	shrink = widget_paned_child_flag(attr, "child2-shrink", TRUE);
	gtk_paned_pack2(GTK_PANED(widget), children.widgets[1], resize, shrink);

	stackelement_clear(&children);
	return widget;
}

gchar *widget_paned_envvar_construct(GtkWidget *widget)
{
	return g_strdup_printf("%d", gtk_paned_get_position(GTK_PANED(widget)));
}

void widget_paned_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void widget_paned_refresh(variable *var)
{
	GList *element;
	gchar *action;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));

	action = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (action != NULL) {
		if (input_is_shell_command(action))
			widget_paned_input_by_command(var, action + 8);
		if (strncasecmp(action, "file:", 5) == 0 && strlen(action) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, action + 5);
			widget_paned_input_by_file(var, action + 5);
		}
		action = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	if (!initialised) {
		if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
			fprintf(stderr,
				"%s(): <label> not implemented for this widget.\n", __func__);
		if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
			fprintf(stderr,
				"%s(): <item> not implemented for this widget.\n", __func__);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			widget_paned_set_position_from_text(var,
				attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
				"paned default position");
		if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
			gtk_widget_set_sensitive(var->Widget, FALSE);
	}
}

void widget_paned_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr,
		"%s(): Removeselected not implemented for this widget.\n", __func__);
}

void widget_paned_save(variable *var)
{
	FILE *stream;
	GList *element;
	gchar *action;
	gchar *filename;

	filename = NULL;
	action = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (action != NULL) {
		if (strncasecmp(action, "file:", 5) == 0 && strlen(action) > 5) {
			filename = action + 5;
			break;
		}
		action = attributeset_get_next(&element, var->Attributes, ATTR_OUTPUT);
	}

	if (filename == NULL) {
		fprintf(stderr, "%s(): No <output file> directive found.\n",
			__func__);
		return;
	}

	stream = fopen(filename, "w");
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}

	fprintf(stream, "%d", gtk_paned_get_position(GTK_PANED(var->Widget)));
	widget_close_output(stream, filename);
}
