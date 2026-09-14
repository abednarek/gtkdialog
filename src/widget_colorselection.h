/*
 * widget_colorselection.h: GtkColorSelection widget.
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

#ifndef WIDGET_COLORSELECTION_H
#define WIDGET_COLORSELECTION_H

void widget_colorselection_clear(variable *var);
GtkWidget *widget_colorselection_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_colorselection_envvar_construct(GtkWidget *widget);
void widget_colorselection_fileselect(
	variable *var, const char *name, const char *value);
void widget_colorselection_refresh(variable *var);
void widget_colorselection_removeselected(variable *var);
void widget_colorselection_save(variable *var);

#endif
