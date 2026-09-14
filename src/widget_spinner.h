/*
 * widget_spinner.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_SPINNER_H
#define WIDGET_SPINNER_H

void widget_spinner_clear(variable *var);
GtkWidget *widget_spinner_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_spinner_envvar_construct(GtkWidget *widget);
void widget_spinner_fileselect(
	variable *var, const char *name, const char *value);
void widget_spinner_refresh(variable *var);
void widget_spinner_removeselected(variable *var);
void widget_spinner_save(variable *var);

#endif
