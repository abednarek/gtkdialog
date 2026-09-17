/*
 * widget_recentchooser.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_RECENTCHOOSER_H
#define WIDGET_RECENTCHOOSER_H

void widget_recentchooser_clear(variable *var);
GtkWidget *widget_recentchooser_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
GtkWidget *widget_recentchoosermenu_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gboolean widget_recentchooser_is_menu(GtkWidget *widget);
gchar *widget_recentchooser_envvar_construct(GtkWidget *widget);
void widget_recentchooser_fileselect(
	variable *var, const char *name, const char *value);
void widget_recentchooser_refresh(variable *var);
void widget_recentchooser_removeselected(variable *var);
void widget_recentchooser_save(variable *var);

#endif
