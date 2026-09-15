/*
 * widget_imageview.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_IMAGEVIEW_H
#define WIDGET_IMAGEVIEW_H

void widget_imageview_clear(variable *var);
GtkWidget *widget_imageview_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_imageview_envvar_construct(GtkWidget *widget);
void widget_imageview_fileselect(
	variable *var, const char *name, const char *value);
void widget_imageview_fit(variable *var);
void widget_imageview_fit_height(variable *var);
void widget_imageview_fit_width(variable *var);
void widget_imageview_refresh(variable *var);
void widget_imageview_save(variable *var);
void widget_imageview_zoom_in(variable *var);
void widget_imageview_zoom_out(variable *var);
void widget_imageview_zoom_reset(variable *var);

#endif
