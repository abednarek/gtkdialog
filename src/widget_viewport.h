/*
 * widget_viewport.h: GtkViewport widget.
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

#ifndef WIDGET_VIEWPORT_H
#define WIDGET_VIEWPORT_H

void widget_viewport_clear(variable *var);
GtkWidget *widget_viewport_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_viewport_envvar_construct(GtkWidget *widget);
void widget_viewport_fileselect(
	variable *var, const char *name, const char *value);
void widget_viewport_refresh(variable *var);
void widget_viewport_removeselected(variable *var);
void widget_viewport_save(variable *var);

#endif
