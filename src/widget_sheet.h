/*
 * widget_sheet.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_SHEET_H
#define WIDGET_SHEET_H

void widget_sheet_clear(variable *var);
GtkWidget *widget_sheet_create(
	AttributeSet *attributes, tag_attr *attr, gint type);
gchar *widget_sheet_envvar_construct(GtkWidget *widget);
void widget_sheet_fileselect(
	variable *var, const char *name, const char *value);
void widget_sheet_refresh(variable *var);
void widget_sheet_save(variable *var);

#endif
