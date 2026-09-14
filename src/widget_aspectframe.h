/*
 * widget_aspectframe.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_ASPECTFRAME_H
#define WIDGET_ASPECTFRAME_H

void widget_aspectframe_clear(variable *var);
GtkWidget *widget_aspectframe_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_aspectframe_envvar_construct(GtkWidget *widget);
void widget_aspectframe_fileselect(
	variable *var, const char *name, const char *value);
void widget_aspectframe_refresh(variable *var);
void widget_aspectframe_removeselected(variable *var);
void widget_aspectframe_save(variable *var);

#endif
