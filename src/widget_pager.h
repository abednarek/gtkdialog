/*
 * widget_pager.h: libwnck workspace-pager widget.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_PAGER_H
#define WIDGET_PAGER_H

#include <gtk/gtk.h>

#include "attributes.h"
#include "tag_attributes.h"
#include "variables.h"

GtkWidget *widget_pager_create(AttributeSet *attributes,
	tag_attr *tag_attributes, gint type);
gchar *widget_pager_envvar_construct(GtkWidget *widget);
void widget_pager_fileselect(variable *var, const char *name,
	const char *value);
void widget_pager_refresh(variable *var);
void widget_pager_save(variable *var);
void widget_pager_clear(variable *var);
void widget_pager_removeselected(variable *var);

#endif
