/*
 * widget_arrow.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef WIDGET_ARROW_H
#define WIDGET_ARROW_H

GtkWidget *widget_arrow_create(
	AttributeSet *Attr, tag_attr *attr, gint Type);
gchar *widget_arrow_envvar_construct(GtkWidget *widget);

#endif
