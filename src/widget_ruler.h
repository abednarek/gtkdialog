/*
 * widget_ruler.h: GtkRuler widgets.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_RULER_H
#define WIDGET_RULER_H

void widget_ruler_clear(variable *var);
GtkWidget *widget_ruler_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_ruler_envvar_construct(GtkWidget *widget);
void widget_ruler_fileselect(
	variable *var, const char *name, const char *value);
void widget_ruler_refresh(variable *var);
void widget_ruler_removeselected(variable *var);
void widget_ruler_save(variable *var);

#endif
