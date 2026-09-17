/*
 * widget_menu_json.h: Optional JSON input for GTK2 menu shells.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef WIDGET_MENU_JSON_H
#define WIDGET_MENU_JSON_H

#include "variables.h"

void widget_menu_json_refresh(variable *var, GtkWidget *shell,
	const gchar *source, gboolean initialised);

#endif
