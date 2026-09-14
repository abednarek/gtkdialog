/*
 * widget_aboutdialog.c:
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
#include "tag_attributes.h"
#include "widget_aboutdialog.h"

#if GTK_CHECK_VERSION(2,6,0)
extern gboolean option_centering;
extern gboolean have_geometry_xy;
extern gboolean have_geometry_dxdy;
extern gint geometry_dx;
extern gint geometry_dy;
extern gint geometry_x;
extern gint geometry_y;

static void widget_aboutdialog_set_people(
	GtkWidget *widget, AttributeSet *Attr)
{
	GList *element;
	GPtrArray *artists;
	GPtrArray *authors;
	GPtrArray *documenters;
	gchar *item;
	gchar *role;
	gchar role_attribute[] = "role";

	artists = g_ptr_array_new();
	authors = g_ptr_array_new();
	documenters = g_ptr_array_new();
	item = attributeset_get_first(&element, Attr, ATTR_ITEM);
	while (item != NULL) {
		role = attributeset_get_this_tagattr(&element, Attr, ATTR_ITEM,
			role_attribute);
		if (role == NULL || g_ascii_strcasecmp(role, "author") == 0)
			g_ptr_array_add(authors, item);
		else if (g_ascii_strcasecmp(role, "documenter") == 0)
			g_ptr_array_add(documenters, item);
		else if (g_ascii_strcasecmp(role, "artist") == 0)
			g_ptr_array_add(artists, item);
		else
			gtkdialog_warning("Invalid about dialog item role '%s'; ignored.",
				role);
		item = attributeset_get_next(&element, Attr, ATTR_ITEM);
	}
	if (authors->len != 0) {
		g_ptr_array_add(authors, NULL);
		gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(widget),
			(const gchar **)authors->pdata);
	}
	if (documenters->len != 0) {
		g_ptr_array_add(documenters, NULL);
		gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(widget),
			(const gchar **)documenters->pdata);
	}
	if (artists->len != 0) {
		g_ptr_array_add(artists, NULL);
		gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(widget),
			(const gchar **)artists->pdata);
	}
	g_ptr_array_free(artists, TRUE);
	g_ptr_array_free(authors, TRUE);
	g_ptr_array_free(documenters, TRUE);
}

GtkWidget *widget_aboutdialog_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	const gchar *program_name;
	GtkWidget *widget;

	(void)Type;
	widget = gtk_about_dialog_new();
	program_name = attr != NULL
		? get_tag_attribute(attr, "program-name") : NULL;
	if (program_name == NULL)
		program_name = PACKAGE;
#if GTK_CHECK_VERSION(2,12,0)
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(widget), program_name);
#else
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(widget), program_name);
#endif
	if (attr != NULL)
		kill_tag_attribute(attr, "program-name");
	gtk_window_set_icon_name(GTK_WINDOW(widget), PACKAGE);
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-dialog-response",
		g_strdup(""), g_free);
	widget_aboutdialog_set_people(widget, Attr);
	if (have_geometry_dxdy)
		gtk_widget_set_size_request(widget, geometry_dx, geometry_dy);
	if (have_geometry_xy)
		gtk_window_move(GTK_WINDOW(widget), geometry_x, geometry_y);
	if (option_centering)
		gtk_window_set_position(GTK_WINDOW(widget), GTK_WIN_POS_CENTER_ALWAYS);
	return widget;
}
#endif
