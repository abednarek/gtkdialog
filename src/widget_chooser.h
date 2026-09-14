/*
 * widget_chooser.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_CHOOSER_H
#define WIDGET_CHOOSER_H

void widget_chooser_clear(variable *var);
GtkWidget *widget_chooser_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_chooser_envvar_construct(GtkWidget *widget);
void widget_chooser_fileselect(
	variable *var, const char *name, const char *value);
void widget_chooser_refresh(variable *var);
void widget_chooser_removeselected(variable *var);
void widget_chooser_save(variable *var);

#endif
