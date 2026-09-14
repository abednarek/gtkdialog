/*
 * stringman.c: Special string maniplutations.
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

/* 
**
** $Id: stringman.c,v 1.1 2004/11/16 20:43:31 root Exp root $
** $Log: stringman.c,v $
** Revision 1.1  2004/11/16 20:43:31  root
** Initial revision
**
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <gtk/gtk.h>

#include "gtkdialog.h"
#include "stringman.h"
#include "widgets.h"

#define IN_TEXT         1
#define IN_SEPARATOR    2

#ifdef G_OS_WIN32
ssize_t
getline(char **lineptr, size_t *n, FILE *stream)
{
	gtkdialog_warning("%s(): Unimplemented", __func__);
	return 0;
}
#endif

gboolean
input_is_shell_command(const gchar *command)
{
	if (command == NULL)
		return FALSE;
	return strncasecmp(command, "command:", 8) == 0;
}

const gchar *
input_get_shell_command(const gchar *command)
{
	if (input_is_shell_command(command))
		return command + 8;
	else
		return NULL;
}

/* Thunor: These need to match CommandType as defined in stringman.h */

static gchar *prefixes[] = {
	"command",
	"exit",
	"closewindow",
	"launch",
	"enable",
	"disable",
	"show",
	"hide",
	"activate",
	"grabfocus",
	"presentwindow",
	"refresh",
	"save",
	"fileselect",
	"clear",
	"removeselected",
	"break",
	"loadstyles",
	"insert",
	"append",
	"expandall",
	"collapseall",
	NULL
};

CommandType
command_prefix_get_type(const gchar *prefix) 
{
	gint n;
	for (n = 0; prefixes[n] != NULL; ++n) 
		if (g_ascii_strcasecmp(prefix, prefixes[n]) == 0)
			return (CommandType) n;

	return CommandUnknown;
}

void
command_get_prefix(const gchar *string, 
		gchar **prefix,
		gchar **command)
{
	gchar *separator;
	gchar *tmp;
	gint   n;

	g_assert(string != NULL && prefix != NULL && command != NULL);
	separator = g_utf8_strchr(string, -1, ':');
	if (separator == NULL) {
		*prefix = g_strdup("command");
		*command = g_strchug(g_strdup(string));
		return;
	}
	
	n = g_utf8_pointer_to_offset(string, separator);
	tmp = g_strndup(string, n);

	for (n = 0; prefixes[n] != NULL; ++n) 
		if (g_ascii_strcasecmp(tmp, prefixes[n]) == 0) {
			*prefix = tmp;
			*command = g_strchug(g_strdup(separator + 1));
			return;
		}
	
	/*
	** We found the separator, but the prefix is invalid, so it is a shell
	** command with a ':' in it.
	*/
	g_free(tmp);
	*prefix = g_strdup("command");
	*command = g_strchug(g_strdup(string));
}

/* Thunor: I saw what technosaurus was doing with GTKDIALOG_PIXMAP_PATH
 * at http://www.murga-linux.com/puppy/viewtopic.php?p=632867#632867
 * and thought it a very good platform independent idea to implement */

/* Return a newly allocated path. The caller must free it with g_free(). */
gchar *find_pixmap(const gchar *filename)
{
	gchar *fullpath;
	gchar **folders;
	gchar *gtkdialog_pixmap_path;
	gint count;

	if (access(filename, R_OK) == 0)
		return g_strdup(filename);

	/* If the GTKDIALOG_PIXMAP_PATH envvar exists then attempt to find
	 * the file in any one of a number of user defined locations */
	if ((gtkdialog_pixmap_path = getenv("GTKDIALOG_PIXMAP_PATH"))) {

		folders = g_strsplit(gtkdialog_pixmap_path, ":", -1);

#ifdef DEBUG
		fprintf(stderr, "%s(): gtkdialog_pixmap_path=\"%s\"\n",
			__func__, gtkdialog_pixmap_path);
		count = 0;
		while (folders[count]) {
			fprintf(stderr, "%s(): folders[%i]=\"%s\"\n", __func__,
				count, folders[count]);
			count++;
		}
#endif

		count = 0;
		while (folders[count]) {
			fullpath = g_build_filename(folders[count], filename, NULL);

#ifdef DEBUG
			fprintf(stderr, "%s(): Finding \"%s\"\n", __func__,
				fullpath);
#endif

			if (access(fullpath, R_OK) == 0) {
				g_strfreev(folders);
				return fullpath;
			}

			g_free(fullpath);
			count++;
		}

		g_strfreev(folders);
	}

#ifdef DEBUG
	fprintf(stderr, "%s(): No readable image found for \"%s\"\n",
		__func__, filename);
#endif

	return g_strdup("");
}


/* Cut an owned string into substrings with the given field separator.
 * The returned list owns all strings and must be freed with list_t_free(). */
list_t *linecutter(char *str, int fs)
{
	list_t *parts;
	gchar separator[2];
	
	g_assert(str != NULL);
#ifdef DEBUG
	g_message("%s(%s, %c)", __func__, str, fs);
#endif

	separator[0] = (gchar)fs;
	separator[1] = '\0';

	parts = g_new(list_t, 1);
	parts->line = g_strsplit(str, separator, -1);
	parts->n_lines = g_strv_length(parts->line);
	parts->maxlines = parts->n_lines;
	g_free(str);

	return parts;
}

/* Free a list returned by linecutter(). */

void list_t_free(list_t *ptr)
{
	if (ptr != NULL) {
		g_strfreev(ptr->line);
		g_free(ptr);
	}
}

/*
** This function will give a default name for widgets, so every
** widget can get a variable name.
*/
char *str_default_name(int itype)
{
	static int serial = 0;
	char *type;

	++serial;
	type = widgets_to_str(itype);
	return g_strdup_printf("%s%03d", type, serial);
}

/***********************************************************************
 * strnatcmp                                                           *
 ***********************************************************************/

static gint strnatcmp_numbers(const gchar **c1, const gchar **c2)
{
	const gchar       *end1 = *c1;
	const gchar       *end2 = *c2;
	const gchar       *significant1;
	const gchar       *significant2;
	gsize              length1;
	gsize              length2;
	gint               retval;

	while (*end1 >= '0' && *end1 <= '9') end1++;
	while (*end2 >= '0' && *end2 <= '9') end2++;

	significant1 = *c1;
	significant2 = *c2;
	while (significant1 < end1 && *significant1 == '0') significant1++;
	while (significant2 < end2 && *significant2 == '0') significant2++;
	length1 = end1 - significant1;
	length2 = end2 - significant2;

	if (length1 < length2) {
		retval = -1;
	} else if (length1 > length2) {
		retval = 1;
	} else if (length1 == 0) {
		retval = 0;
	} else {
		retval = memcmp(significant1, significant2, length1);
		if (retval < 0) retval = -1;
		if (retval > 0) retval = 1;
	}

	*c1 = end1;
	*c2 = end2;
	return retval;
}

gint strnatcmp(const gchar *c1, const gchar *c2, gint sensitive)
{
	gint              c1val;
	gint              c2val;
	gint              retval;

	if (c1 == c2) {		/* Deals with identical pointers, including NULL */
		retval = 0;
	} else if (!c1) {
		retval = -1;
	} else if (!c2) {
		retval = 1;
	} else if (!*c1 && !*c2) {
		retval = 0;
	} else if (!*c1) {
		retval = -1;
	} else if (!*c2) {
		retval = 1;
	} else {
		/* There's definitely something different to compare */
		while (TRUE) {

			/* Ignore whitespace */
			while ((*c1 == ' ' || *c1 == '\t') && *(c1 + 1)) c1++;
			while ((*c2 == ' ' || *c2 == '\t') && *(c2 + 1)) c2++;

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): c1='%s' c2='%s'\n", __func__, c1, c2);
#endif

			/* Compare complete decimal runs without converting them to an
			 * integer. Conversion overflowed for long generated identifiers. */
			if ((*c1 >= '0' && *c1 <= '9') &&
				(*c2 >= '0' && *c2 <= '9')) {
				retval = strnatcmp_numbers(&c1, &c2);
				if (retval != 0)
					break;
				c1val = c2val = 0;
			} else if (*c1 >= '0' && *c1 <= '9') {
				retval = 1;
				break;
			} else if (*c2 >= '0' && *c2 <= '9') {
				retval = -1;
				break;
			} else {
				/* Evaluate single chars */
				if (sensitive) {
					c1val = (guchar)*c1;
				} else {
					/* tolower() determines what is upper
					 * and lower case dependent upon locale */
					c1val = tolower((guchar)*c1);
				}
				c1++;
				if (sensitive) {
					c2val = (guchar)*c2;
				} else {
					c2val = tolower((guchar)*c2);
				}
				c2++;
			}

			/* At this point both c1 and c2 will be pointing to the next
			 * character to process which could be the terminating zero */

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): c1val=%i c2val=%i c1='%s' c2='%s'\n", __func__, c1val, c2val, c1, c2);
#endif

			/* Not equal? */
			if (c1val < c2val) {
				retval = -1;
				break;
			} else if (c1val > c2val) {
				retval = 1;
				break;
			}

			/* End of data? */
			if (!*c1 && !*c2) {
				retval = 0;
				break;
			} else if (!*c1) {
				retval = -1;
				break;
			} else if (!*c2) {
				retval = 1;
				break;
			}
		}
	}

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): retval=%i\n", __func__, retval);
#endif

	return retval;
}
