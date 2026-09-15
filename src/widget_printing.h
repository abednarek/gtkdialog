/*
 * widget_printing.h: shared GTK2 printing helpers.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef WIDGET_PRINTING_H
#define WIDGET_PRINTING_H

#include <gtk/gtk.h>

#include "tag_attributes.h"

#if GTK_CHECK_VERSION(2,10,0)
void widget_printing_apply_page_attributes(GtkPageSetup *setup,
	tag_attr *attributes);
GtkPageSetup *widget_printing_load_page_setup(const gchar *filename);
GtkPrintSettings *widget_printing_load_print_settings(const gchar *filename);
gboolean widget_printing_save_print_settings(GtkPrintSettings *settings,
	const gchar *filename, GError **error);
#endif

#endif
