/*
 * widget_messagedialog.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_MESSAGEDIALOG_H
#define WIDGET_MESSAGEDIALOG_H

void widget_messagedialog_clear(variable *var);
GtkWidget *widget_messagedialog_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_messagedialog_envvar_construct(GtkWidget *widget);
void widget_messagedialog_fileselect(
	variable *var, const char *name, const char *value);
void widget_messagedialog_refresh(variable *var);
void widget_messagedialog_removeselected(variable *var);
void widget_messagedialog_save(variable *var);

#endif
