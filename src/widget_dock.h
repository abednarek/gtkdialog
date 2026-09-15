/*
 * widget_dock.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_DOCK_H
#define WIDGET_DOCK_H

void widget_dock_clear(variable *var);
GtkWidget *widget_dock_create(
	AttributeSet *attributes, tag_attr *attr, gint type);
GtkWidget *widget_dockitem_create(
	AttributeSet *attributes, tag_attr *attr, gint type);
gchar *widget_dock_envvar_construct(GtkWidget *widget);
void widget_dock_fileselect(
	variable *var, const char *name, const char *value);
gboolean widget_dock_is_container(GtkWidget *widget);
gboolean widget_dock_is_item(GtkWidget *widget);
void widget_dock_refresh(variable *var);
void widget_dock_save(variable *var);

#endif
