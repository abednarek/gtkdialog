/*
 * widget_linkbutton.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_LINKBUTTON_H
#define WIDGET_LINKBUTTON_H

void widget_linkbutton_clear(variable *var);
GtkWidget *widget_linkbutton_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_linkbutton_envvar_construct(GtkWidget *widget);
void widget_linkbutton_fileselect(
	variable *var, const char *name, const char *value);
void widget_linkbutton_refresh(variable *var);
void widget_linkbutton_removeselected(variable *var);
void widget_linkbutton_save(variable *var);

#endif
