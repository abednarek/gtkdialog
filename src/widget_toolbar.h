/*
 * widget_toolbar.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_TOOLBAR_H
#define WIDGET_TOOLBAR_H

void widget_toolbar_clear(variable *var);
GtkWidget *widget_toolbar_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
GtkWidget *widget_toolitem_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_toolbar_envvar_construct(GtkWidget *widget);
void widget_toolbar_fileselect(
	variable *var, const char *name, const char *value);
void widget_toolbar_refresh(variable *var);
void widget_toolbar_removeselected(variable *var);
void widget_toolbar_save(variable *var);

#endif
