/*
 * stringman.h: Special string manipulation functions
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2003-2007  László Pere <pipas@linux.pte.hu>
 * Copyright (C) 2012       Thunor <thunorsif@hotmail.com>
 * Copyright (C) 2026       Artur Bednarek <artur@unix.org.pl>
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
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef STRINGMAN_H
#define STRINGMAN_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "gtkdialog.h"
#include "attributes.h"

typedef struct list_t {
	int n_lines;
	int maxlines;
	char **line;
} list_t;

typedef enum {
	CommandShellCommand      = 0,
	CommandExit              = 1,
	CommandCloseWindow       = 2,
	CommandLaunch            = 3,
	CommandEnable            = 4,
	CommandDisable           = 5,
	CommandShow              = 6,
	CommandHide              = 7,
	CommandActivate          = 8,
	CommandGrabFocus         = 9,
	CommandPresentWindow     = 10,
	CommandRefresh           = 11,
	CommandSave              = 12,
	CommandFileSelect        = 13,
	CommandClear             = 14,
	CommandRemoveSelected    = 15,
	CommandBreak             = 16,
	CommandLoadStyles        = 17,
	CommandInsert            = 18,
	CommandAppend            = 19,
	CommandExpandAll         = 20,
	CommandCollapseAll       = 21,
	CommandZoomIn            = 22,
	CommandZoomOut           = 23,
	CommandZoomReset         = 24,
	CommandFit               = 25,
	CommandFitWidth          = 26,
	CommandFitHeight         = 27,
	CommandUndo              = 28,
	CommandRedo              = 29,
	CommandReparentWidget    = 30,
	CommandAppendTab         = 31,
	CommandRemoveTab         = 32,
	CommandReplaceBox        = 33,
	CommandSelectTab         = 34,
	CommandAppendTabAuto     = 35,
	CommandHangupTerminal    = 36,
	CommandUnknown           = 37
} CommandType;

#ifdef G_OS_WIN32
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
#endif

gboolean input_is_shell_command(const gchar *command);
const gchar *input_get_shell_command(const gchar *command);

void command_get_prefix(const gchar *string, gchar **prefix, gchar **command);
CommandType command_prefix_get_type(const gchar *prefix);
gchar *find_pixmap(const gchar *filename);
list_t *linecutter(char *str, int fs );
void list_t_free(list_t *ptr);
char *str_default_name (int itype);
gint strnatcmp(const gchar *c1, const gchar *c2, gint sensitive);

#endif
