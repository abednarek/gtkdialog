/*
 * widget_offscreenwindow.c: GTK2 off-screen window widget.
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

#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "stack.h"
#include "widgets.h"
#include "widget_offscreenwindow.h"

#if GTK_CHECK_VERSION(2,20,0)
static gboolean widget_offscreenwindow_damaged(GtkWidget *widget,
	GdkEventExpose *event, AttributeSet *attributes)
{
	(void)event;
	widget_signal_executor(widget, attributes, "damage-event");
	return FALSE;
}
#endif

GtkWidget *widget_offscreenwindow_create(AttributeSet *attributes,
	tag_attr *tag_attributes, gint type)
{
#if GTK_CHECK_VERSION(2,20,0)
	GtkWidget *widget;
	stackelement children;
#endif

	(void)attributes;
	(void)tag_attributes;
	(void)type;
#if GTK_CHECK_VERSION(2,20,0)
	widget = gtk_offscreen_window_new();
	children = pop();
	if (children.nwidgets > 0)
		gtk_container_add(GTK_CONTAINER(widget), children.widgets[0]);
	stackelement_clear(&children);
	return widget;
#else
	g_error("The offscreenwindow widget requires GTK+ 2.20 or later.");
	return NULL;
#endif
}

gchar *widget_offscreenwindow_envvar_construct(GtkWidget *widget)
{
	(void)widget;
	/* A top-level variable is a program name, not an exported value. */
	return NULL;
}

void widget_offscreenwindow_refresh(variable *var)
{
#if GTK_CHECK_VERSION(2,20,0)
	GList *element;
	GtkWidget *child;
	gboolean initialised;
	gint height = -1;
	gint width = -1;

	initialised = g_object_get_data(G_OBJECT(var->Widget), "_initialised") !=
		NULL;
	if (!initialised) {
		if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
			width = widget_parse_size(attributeset_get_first(&element,
				var->Attributes, ATTR_WIDTH), "offscreen window width");
		if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
			height = widget_parse_size(attributeset_get_first(&element,
				var->Attributes, ATTR_HEIGHT), "offscreen window height");
		if (width != -1 || height != -1)
			gtk_widget_set_size_request(var->Widget, width, height);
		if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
			gtk_widget_set_sensitive(var->Widget, FALSE);
		g_signal_connect(var->Widget, "damage-event",
			G_CALLBACK(widget_offscreenwindow_damaged), var->Attributes);
		return;
	}

	child = gtk_bin_get_child(GTK_BIN(var->Widget));
	if (child != NULL)
		gtk_widget_queue_draw(child);
	else
		gtk_widget_queue_draw(var->Widget);
#else
	(void)var;
#endif
}

void widget_offscreenwindow_save(variable *var)
{
#if GTK_CHECK_VERSION(2,20,0)
	GError *error = NULL;
	GList *element;
	GdkPixbuf *pixbuf;
	const gchar *directive;
	const gchar *extension;
	const gchar *filename = NULL;
	const gchar *save_format;
	gchar *format;

	directive = attributeset_get_first(&element, var->Attributes,
		ATTR_OUTPUT);
	while (directive != NULL) {
		if (strncasecmp(directive, "file:", 5) == 0 &&
			strlen(directive) > 5) {
			filename = directive + 5;
			break;
		}
		directive = attributeset_get_next(&element, var->Attributes,
			ATTR_OUTPUT);
	}
	if (filename == NULL) {
		gtkdialog_warning("No <output file> directive found for offscreenwindow.");
		return;
	}
	extension = strrchr(filename, '.');
	if (extension == NULL || extension[1] == '\0') {
		gtkdialog_warning("Couldn't determine image format for '%s'.", filename);
		return;
	}
	format = g_ascii_strdown(extension + 1, -1);
	save_format = strcasecmp(format, "jpg") == 0 ? "jpeg" : format;
	pixbuf = gtk_offscreen_window_get_pixbuf(
		GTK_OFFSCREEN_WINDOW(var->Widget));
	if (pixbuf == NULL) {
		gtkdialog_warning("The offscreen window is not ready to save.");
		g_free(format);
		return;
	}
	if (!gdk_pixbuf_save(pixbuf, filename, save_format, &error, NULL)) {
		gtkdialog_warning("Couldn't save offscreen image '%s': %s", filename,
			error != NULL ? error->message : "unknown error");
		if (error != NULL)
			g_error_free(error);
	}
	g_object_unref(pixbuf);
	g_free(format);
#else
	(void)var;
#endif
}

void widget_offscreenwindow_clear(variable *var)
{
	/* The child hierarchy defines the program; clear must not destroy it. */
	(void)var;
}

void widget_offscreenwindow_removeselected(variable *var)
{
	(void)var;
}
