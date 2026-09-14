/*
 * widget_hsv.h: GtkHSV widget.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_HSV_H
#define WIDGET_HSV_H

void widget_hsv_clear(variable *var);
GtkWidget *widget_hsv_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_hsv_envvar_construct(GtkWidget *widget);
void widget_hsv_fileselect(
	variable *var, const char *name, const char *value);
void widget_hsv_refresh(variable *var);
void widget_hsv_removeselected(variable *var);
void widget_hsv_save(variable *var);

#endif
