/*
 * widget_databox.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_DATABOX_H
#define WIDGET_DATABOX_H

GtkWidget *widget_databox_create(AttributeSet *attributes, tag_attr *attr,
	gint width, gint height);
gboolean widget_databox_is_requested(GtkWidget *widget);
gboolean widget_databox_is_widget(GtkWidget *widget);
void widget_databox_clear(variable *var);
gchar *widget_databox_envvar_construct(GtkWidget *widget);
void widget_databox_refresh(variable *var);
void widget_databox_save(variable *var);
void widget_databox_set_value(variable *var, const gchar *value);
void widget_databox_zoom_in(variable *var);
void widget_databox_zoom_out(variable *var);
void widget_databox_zoom_reset(variable *var);

#endif
