/*
 * size_groups.c:
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
#include "tag_attributes.h"
#include "size_groups.h"

static GHashTable *size_group_registry;

static gboolean size_group_mode_parse(
	const gchar *text, GtkSizeGroupMode *mode)
{
	if (text == NULL || strcasecmp(text, "both") == 0) {
		*mode = GTK_SIZE_GROUP_BOTH;
		return TRUE;
	}
	if (strcasecmp(text, "horizontal") == 0) {
		*mode = GTK_SIZE_GROUP_HORIZONTAL;
		return TRUE;
	}
	if (strcasecmp(text, "vertical") == 0) {
		*mode = GTK_SIZE_GROUP_VERTICAL;
		return TRUE;
	}

	return FALSE;
}

void size_groups_begin(void)
{
	g_assert(size_group_registry == NULL);
	size_group_registry = g_hash_table_new_full(
		g_str_hash, g_str_equal, g_free, g_object_unref);
}

void size_groups_add_widget(GtkWidget *widget, tag_attr *attr)
{
	gchar *group_name;

	if (widget == NULL || attr == NULL)
		return;
	group_name = get_tag_attribute(attr, "size-group");
	if (group_name == NULL || *group_name == '\0')
		return;

	{
		GtkSizeGroup *group;
		GtkSizeGroupMode mode;
		gchar *mode_name;

		mode_name = get_tag_attribute(attr, "size-group-mode");
		if (!size_group_mode_parse(mode_name, &mode)) {
			gtkdialog_warning("Invalid size-group-mode '%s'; using both.",
				mode_name);
			mode = GTK_SIZE_GROUP_BOTH;
		}

		group = g_hash_table_lookup(size_group_registry, group_name);
		if (group == NULL) {
			group = gtk_size_group_new(mode);
			g_hash_table_insert(
				size_group_registry, g_strdup(group_name), group);
		} else if (gtk_size_group_get_mode(group) != mode) {
			gtkdialog_warning("Size group '%s' already uses another mode; "
				"keeping its first mode.", group_name);
		}

		gtk_size_group_add_widget(group, widget);
		g_object_set_data_full(G_OBJECT(widget), "gtkdialog-size-group",
			g_object_ref(group), g_object_unref);
	}
}

void size_groups_end(void)
{
	if (size_group_registry == NULL)
		return;
	g_hash_table_destroy(size_group_registry);
	size_group_registry = NULL;
}
