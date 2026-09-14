/*
 * widget_scrollbar.h: GtkScrollbar widgets.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026       Artur Bednarek <artur@unix.org.pl>
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

#ifndef WIDGET_SCROLLBAR_H
#define WIDGET_SCROLLBAR_H

void widget_scrollbar_clear(variable *var);
GtkWidget *widget_scrollbar_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_scrollbar_envvar_construct(GtkWidget *widget);
void widget_scrollbar_fileselect(
	variable *var, const char *name, const char *value);
void widget_scrollbar_refresh(variable *var);
void widget_scrollbar_removeselected(variable *var);
void widget_scrollbar_save(variable *var);

#endif
