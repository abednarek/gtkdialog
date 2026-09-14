/*
 * size_groups.h:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef SIZE_GROUPS_H
#define SIZE_GROUPS_H

void size_groups_begin(void);
void size_groups_add_widget(GtkWidget *widget, tag_attr *attr);
void size_groups_end(void);

#endif
