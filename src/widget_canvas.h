/*
 * widget_canvas.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_CANVAS_H
#define WIDGET_CANVAS_H

#include <gtk/gtk.h>
#include "attributes.h"
#include "variables.h"

GtkWidget *widget_canvas_create(AttributeSet *attributes, tag_attr *attr,
	gint width, gint height);
gboolean widget_canvas_is_requested(GtkWidget *widget);
gboolean widget_canvas_is_widget(GtkWidget *widget);
void widget_canvas_clear(variable *var);
gchar *widget_canvas_envvar_construct(GtkWidget *widget);
void widget_canvas_refresh(variable *var);
void widget_canvas_save(variable *var);
void widget_canvas_set_value(variable *var, const gchar *value);
void widget_canvas_zoom_in(variable *var);
void widget_canvas_zoom_out(variable *var);
void widget_canvas_zoom_reset(variable *var);

#endif
