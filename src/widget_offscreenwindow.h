/*
 * widget_offscreenwindow.h: GTK2 off-screen window widget.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_OFFSCREENWINDOW_H
#define WIDGET_OFFSCREENWINDOW_H

#include <gtk/gtk.h>

#include "attributes.h"
#include "tag_attributes.h"
#include "variables.h"

GtkWidget *widget_offscreenwindow_create(AttributeSet *attributes,
	tag_attr *tag_attributes, gint type);
gchar *widget_offscreenwindow_envvar_construct(GtkWidget *widget);
void widget_offscreenwindow_refresh(variable *var);
void widget_offscreenwindow_save(variable *var);
void widget_offscreenwindow_clear(variable *var);
void widget_offscreenwindow_removeselected(variable *var);

#endif
