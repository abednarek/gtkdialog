/*
 * widget_arrow.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "tag_attributes.h"
#include "widget_arrow.h"

GtkWidget *widget_arrow_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	(void)Attr;
	(void)attr;
	(void)Type;
	return gtk_arrow_new(GTK_ARROW_RIGHT, GTK_SHADOW_NONE);
}

gchar *widget_arrow_envvar_construct(GtkWidget *widget)
{
	(void)widget;
	return g_strdup("");
}
