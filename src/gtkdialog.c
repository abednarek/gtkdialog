/*
 * gtkdialog.c: Starting up the program.
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2003-2007  László Pere <pipas@linux.pte.hu>
 * Copyright (C) 2011-2012  Thunor <thunorsif@hotmail.com>
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
** $Id: main.c,v 1.1 2004/11/16 20:47:40 root Exp root $
** $Log: main.c,v $
** Revision 1.1  2004/11/16 20:47:40  root
** Initial revision
**
*/

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>
#include <getopt.h>
#include <gtk/gtk.h>

#include "config.h"
#include "gtkdialog.h"
#include "variables.h"
#include "automaton.h"
#include "gtkdialog_parser.h"
#undef DEBUG
#undef WARNING
#include "macros.h"
/*
 * Program argument variables.
 */
gboolean option_version = FALSE;
gboolean option_debug = FALSE;
gchar *option_input_variable = NULL;
gchar *option_input_file = NULL;
gchar *option_include_file = NULL;
gchar *option_event_file = NULL;
gchar *option_geometry = NULL;
gchar *option_space_expand = NULL;
gchar *option_space_fill = NULL;
gchar *option_styles_file = NULL;
gboolean option_input_stdin = FALSE;
gboolean option_no_warning = FALSE;
gboolean option_print_ir = FALSE;
gboolean option_centering = FALSE;

/* Process-wide state shared by the widget implementation modules. */
gint function_signals_block = 0;
gint project_space_expand = -1;
gint project_space_fill = -1;
GtkWidget *lastradiowidget = NULL;
GtkWidget *lasttoolradiowidget = NULL;
GList *accel_groups = NULL;
GList *widget_hide_list = NULL;
GList *widget_show_list = NULL;
gint window_id = 0;

void gtkdialog_warning(const gchar *format, ...)
{
	va_list arguments;

	if (option_no_warning)
		return;

	va_start(arguments, format);
	g_logv(NULL, G_LOG_LEVEL_WARNING, format, arguments);
	va_end(arguments);
}

static gint source = PRG_UNKNOWN;    // Where the program is coming from?
gchar *program_src = NULL;           // The actual program source.
gint charsreaded = 0;                // How much did we red from the source in the memory.
static FILE *sourcefile = NULL;      // The input file handler.
static gchar *program_name = NULL;   // The name of the dialog for messages.

gboolean have_geometry_xy = FALSE;
gboolean have_geometry_dxdy = FALSE;
gint geometry_dx = 0;
gint geometry_dy = 0;
gint geometry_x = 0;
gint geometry_y = 0;

static gboolean
get_geometry_integer(const gchar **cursor, gint *result)
{
	gchar *end;
	gint64 number;

	errno = 0;
	number = g_ascii_strtoll(*cursor, &end, 10);
	if (errno != 0 || end == *cursor || number < G_MININT ||
		number > G_MAXINT)
		return FALSE;

	*cursor = end;
	*result = (gint)number;
	return TRUE;
}

static gboolean
get_geometry_end(const gchar *cursor)
{
	while (g_ascii_isspace(*cursor))
		++cursor;
	return *cursor == '\0';
}

static gboolean 
get_geometry(const char *argument)
{
	const gchar *cursor;
	gint dx, dy, x, y;

	g_return_val_if_fail(argument != NULL, FALSE);

	PIP_DEBUG("geometry: '%s'", argument);

	cursor = argument;
	if (*cursor == '+') {
		++cursor;
		if (get_geometry_integer(&cursor, &x) && *cursor == '+') {
			++cursor;
			if (get_geometry_integer(&cursor, &y) && get_geometry_end(cursor)) {
				PIP_DEBUG("Geometry in form of: +%%d+%%d");
				geometry_x = x;
				geometry_y = y;
				have_geometry_xy = TRUE;
				return TRUE;
			}
		}
	} else if (get_geometry_integer(&cursor, &dx) && *cursor == 'x') {
		++cursor;
		if (get_geometry_integer(&cursor, &dy)) {
			if (get_geometry_end(cursor)) {
				PIP_DEBUG("Geometry in form of: %%dx%%d");
				geometry_dx = dx;
				geometry_dy = dy;
				have_geometry_dxdy = TRUE;
				return TRUE;
			}
			if (*cursor == '+') {
				++cursor;
				if (get_geometry_integer(&cursor, &x) && *cursor == '+') {
					++cursor;
					if (get_geometry_integer(&cursor, &y) &&
						get_geometry_end(cursor)) {
						PIP_DEBUG("Geometry in form of: %%dx%%d+%%d+%%d");
						geometry_dx = dx;
						geometry_dy = dy;
						geometry_x = x;
						geometry_y = y;
						have_geometry_dxdy = TRUE;
						have_geometry_xy = TRUE;
						return TRUE;
					}
				}
			}
		}
	}

	if (!option_no_warning)
		gtkdialog_warning("Invalid geometry value '%s'; option ignored.", argument);
	return FALSE;
}

static gboolean
get_space_value(const char *argument, gint *targetvar)
{
	if ((strcasecmp(argument, "true") == 0) ||
		(strcasecmp(argument, "yes") == 0) ||
		(strcasecmp(argument, "1") == 0)) {
		*targetvar = TRUE;
		return TRUE;
	} else if ((strcasecmp(argument, "false") == 0) ||
		(strcasecmp(argument, "no") == 0) ||
		(strcasecmp(argument, "0") == 0)) {
		*targetvar = FALSE;
		return TRUE;
	}

	gtkdialog_warning("Invalid packing state '%s'; option ignored.", argument);
	return FALSE;
}

/*
 * FIXME: we maybe should break these options to some groups...
 */
static void
gtkdialog_init(
		int    *argc,
		char ***argv)
{
	GError *error = NULL;
	GOptionContext *context;
	gint parsed_argc = *argc;
	gchar **parsed_argv = *argv;
    static GOptionEntry entries[] = {
	{ 
		"version", 'v', 
		0, G_OPTION_ARG_NONE, &option_version, 
		"Print version and build information and exit.", NULL
	},
	{ 
		"debug", 'd', 
		0, G_OPTION_ARG_NONE, &option_debug, 
		"Debug mode prints the processed characters.", NULL
	},
	{ 
		"program", 'p', 
		0, G_OPTION_ARG_STRING, &option_input_variable, 
		"Get the GUI description from the environment.", "variable"
	},
	{ 
		"file", 'f', 
		0, G_OPTION_ARG_STRING, &option_input_file, 
		"Get the GUI description from a regular file.", "filename"
	},
	{ 
		"include", 'i', 
		0, G_OPTION_ARG_STRING, &option_include_file, 
		"Include the given file when executing.", "filename"
	},
	{ 
		"event-driven", 'e', 
		0, G_OPTION_ARG_STRING, &option_event_file, 
		"Execute the file as an event driven program.", "filename"
	},
	{ 
		"stdin", 's', 
		0, G_OPTION_ARG_NONE, &option_input_stdin, 
		"Get the GUI description from standard input.", NULL
	},
	{ 
		"no-warning", 'w', 
		0, G_OPTION_ARG_NONE, &option_no_warning, 
		"Suppress warning messages.", NULL
	},
	{ 
		"geometry", 'G', 
		G_OPTION_FLAG_IN_MAIN, G_OPTION_ARG_STRING, &option_geometry, 
		"The placement and the size of the window.", "[WIDTHxHEIGHT][+X+Y]"
	},
	{ 
		"center", 'c', 
		0, G_OPTION_ARG_NONE, &option_centering, 
		"Center the windows on the screen.", NULL
	},
	{ 
		"print-ir", '\0', 
		0, G_OPTION_ARG_NONE, &option_print_ir, 
		"Print the internal representation and exit.", NULL
	},
	{ 
		"space-expand", '\0', 
		0, G_OPTION_ARG_STRING, &option_space_expand, 
		"The \"expand\" state for packing all widgets.", "state"
	},
	{ 
		"space-fill", '\0', 
		0, G_OPTION_ARG_STRING, &option_space_fill, 
		"The \"fill\" state for packing all widgets.", "state"
	},
	{ 
		"styles", '\0', 
		0, G_OPTION_ARG_STRING, &option_styles_file, 
		"Load styles in the form of a resource file.", "filename"
	},
	{
		NULL
	}
    };

#ifdef DEBUG
	{
		gint deb;
		PIP_DEBUG("argc: %d", parsed_argc);
		for (deb = 0; deb < parsed_argc; ++deb)
			PIP_DEBUG("  argv[%d] = '%s'", deb, parsed_argv[deb]);
	}
#endif
	/*
	 * Maybe the command line options are coming from the kernel and it did
	 * not slice it.
	 */
	if (parsed_argc > 1 &&
		g_utf8_strchr(parsed_argv[1], -1, ' ') != NULL) {
		GPtrArray *expanded;
		gchar **first_arguments;
		gint first_argc;
		gint optionindex;

		PIP_DEBUG("Must re-slice the options.");
		if (!g_shell_parse_argv(parsed_argv[1], &first_argc,
			&first_arguments, &error))
			g_error("Error splitting command line options: %s", error->message);

		/* Some kernels pass a shebang's optional arguments as one string.
		 * Split only that string: later arguments can be filenames containing
		 * whitespace and must remain intact. */
		expanded = g_ptr_array_new();
		g_ptr_array_add(expanded, g_strdup(parsed_argv[0]));
		for (optionindex = 0; optionindex < first_argc; ++optionindex)
			g_ptr_array_add(expanded, g_strdup(first_arguments[optionindex]));
		for (optionindex = 2; optionindex < parsed_argc; ++optionindex)
			g_ptr_array_add(expanded, g_strdup(parsed_argv[optionindex]));
		g_ptr_array_add(expanded, NULL);
		g_strfreev(first_arguments);

		parsed_argc = expanded->len - 1;
		parsed_argv = (gchar **)g_ptr_array_free(expanded, FALSE);
#ifdef DEBUG
		{
			gint deb;
			PIP_DEBUG("Re sliced...");
			PIP_DEBUG("argc: %d", parsed_argc);
			for (deb = 0; deb < parsed_argc; ++deb)
				PIP_DEBUG("  argv[%d] = '%s'", deb, parsed_argv[deb]);
		}
#endif
	}
	

	context = g_option_context_new(
"\n"
"Create dialog boxes and windows according to the given dialog description.\n"
"For more information try 'info gtkdialog'."
);

	g_option_context_add_main_entries(context, entries, PACKAGE);
	g_option_context_add_group (context, gtk_get_option_group(FALSE));

	if (!g_option_context_parse(context, &parsed_argc, &parsed_argv, &error))
		g_error("Error parsing command line options: %s", 
				error->message);

	g_option_context_free(context);
	*argc = parsed_argc;
	*argv = parsed_argv;
}

void get_program_from_variable(const gchar *name)
{
	g_assert(name != NULL);

	PIP_DEBUG("Start.");

	gtkdialog_lexer_reset();
	set_program_name(name);
	program_src = g_strdup(g_getenv(name));

	if (program_src == NULL) 
		g_error(
"Gtkdialog: Could not find the dialog description in the environment "
"variable '%s'.", name);

	source = PRG_MEMORY;
}
	
static void
get_program_from_file(const gchar *name)
{
	ssize_t length;
	size_t capacity = 0;
	
	PIP_DEBUG("Start.");
	
	gtkdialog_lexer_reset();
	set_program_name("MAIN_WINDOW");
	
	sourcefile = fopen(name, "r");
	if (!sourcefile) 
		g_error("Error opening file '%s': %s", name, g_strerror(errno));
	
	/* A script may contain a shebang before the dialog description. */
	program_src = NULL;
	length = getline(&program_src, &capacity, sourcefile);
	if (length < 0 && ferror(sourcefile))
		g_error("Error reading file '%s': %s", name, g_strerror(errno));

	if (length < 2 || program_src[0] != '#' || program_src[1] != '!') {
		fclose(sourcefile);
		sourcefile = fopen(name, "r");
		if (!sourcefile) 
			g_error("Error opening file '%s': %s", name, g_strerror(errno));
	}
	free(program_src);
	program_src = NULL;
	
	source = PRG_FILE;
}

static void
get_program_from_stdin(void)
{
	/* Moose / Debian 03_stdin patch: changed from PRG_MEMORY */
	gtkdialog_lexer_reset();
	source = PRG_STDIN;
	PIP_DEBUG("Start.");
}

static void 
print_version_exit(int exitcode)
{
	gint extralibs = 0;

	PIP_DEBUG("Start.");

	printf("%s version %s %s (C) 2003-2007 Laszlo Pere, "
		"2011-2012 Thunor, 2026 Artur Bednarek\n",
		PACKAGE_NAME, PACKAGE_VERSION, BUILD_DETAILS);
	printf("Built with support for: ");
	printf("GTK+ 2"); extralibs++;
#if HAVE_JSON_GLIB
	if (extralibs) printf(", ");
	printf("JSON-GLib"); extralibs++;
#endif
#if HAVE_VTE
	if (extralibs) printf(", ");
	printf("VTE"); extralibs++;
#endif
	if (!extralibs) printf("none");
	printf(".\n");

	exit(exitcode);
}

int 
getnextchar(void)
{
	int c = EOF;

	//PIP_DEBUG("Start.");

	/*
	 * This is not too GTKish
	 */
	if (source == PRG_MEMORY) {
		/* Avoid strlen() here: the lexer consumes this buffer one character
		 * at a time, so rescanning it would make parsing quadratic. */
		c = (int) program_src[charsreaded];
		if (c == '\0')
			c = EOF;
		++charsreaded;
		goto we_have_the_char;
	}
	
	if (source == PRG_STDIN) {
		c = getchar();
		goto we_have_the_char;
	}
	
	if (source == PRG_FILE) {
		c = (int) getc(sourcefile);
		goto we_have_the_char;
	}
	
we_have_the_char:
	/*
	 * FIXME: This is a very very simple debug function.
	 */
	//PIP_DEBUG("Returning: '%c'", c);
	if (option_debug)
		printf("%c", c);

	return c;
}

void
reset_program_source(void) 
{
	if (source == PRG_MEMORY) {
		g_free(program_src);
		program_src = NULL;
	} else if (source == PRG_FILE && sourcefile != NULL) {
		fclose(sourcefile);
		sourcefile = NULL;
	}

	source = PRG_UNKNOWN;
	charsreaded = 0;

	PIP_DEBUG("");
}

void
set_program_name(const gchar *name)
{

	PIP_DEBUG("name: '%s'", name);

	g_free(program_name);
	program_name = g_strdup(name);
}

gchar *
get_program_name(void)
{

	PIP_DEBUG("");

	if (program_name != NULL) 
		return program_name;
	else
/*		return "Unknown"; */
		return "";	/* Thunor: A null string is better */
}

void load_styles_file(gchar *filename)
{
	/* Currently this works at the start but not at runtime temp temp */
	gtk_rc_parse(filename);
}

int 
main(int argc, char *argv[])
{
	/* Initialise global variables */
	GTKD_FUNCTION_SIGNALS_RESET;
	lastradiowidget = NULL;
	lasttoolradiowidget = NULL;
	accel_groups = NULL;
	widget_hide_list = widget_show_list = NULL;
	window_id = 0;
	if (atexit(variables_free_all) != 0)
		gtkdialog_warning("Couldn't register variable registry cleanup.");

	setlocale(LC_ALL, "");
	/*
	 * Processing command line arguments.
	 */
	gtkdialog_init(&argc, &argv);
	
	if (option_version)
		print_version_exit(EXIT_SUCCESS);
	
	if (option_geometry != NULL) 
		get_geometry(option_geometry);

	/* Thunor: I've added these to control/override the widget
	 * packing expand and fill states at the project level */
	project_space_expand = -1;
	if (option_space_expand)
		get_space_value(option_space_expand, &project_space_expand);
	project_space_fill = -1;
	if (option_space_fill)
		get_space_value(option_space_fill, &project_space_fill);

	if (option_input_stdin) {
		get_program_from_stdin();
		goto gtkdialog_initialized;
	}
	/*
	 * If we have an event driven program we recall the gtkdialog with
	 * a source file included into the environment.
	 */
#ifdef DEBUG
	fprintf(stderr, "%s(): argv[0]='%s'\n", __func__, argv[0]);
#endif
	if (option_event_file != NULL) {
		GError *spawn_error = NULL;
		GPtrArray *event_argv;
		gchar **event_argv_data;
		gint wait_status;
		gboolean spawned;

		event_argv = g_ptr_array_new();
		g_ptr_array_add(event_argv, g_strdup("/bin/sh"));
		g_ptr_array_add(event_argv, g_strdup("-c"));
		g_ptr_array_add(event_argv,
			g_strdup(". \"$1\"; shift; exec \"$@\""));
		g_ptr_array_add(event_argv, g_strdup("gtkdialog-event"));
		g_ptr_array_add(event_argv, g_strdup(option_event_file));
		g_ptr_array_add(event_argv, g_strdup(argv[0]));
		if (option_debug)
			g_ptr_array_add(event_argv, g_strdup("--debug"));
		if (option_no_warning)
			g_ptr_array_add(event_argv, g_strdup("--no-warning"));
		if (option_centering)
			g_ptr_array_add(event_argv, g_strdup("--center"));
		if (option_print_ir)
			g_ptr_array_add(event_argv, g_strdup("--print-ir"));
		if (have_geometry_dxdy || have_geometry_xy) {
			g_ptr_array_add(event_argv, g_strdup("--geometry"));
			g_ptr_array_add(event_argv, g_strdup(option_geometry));
		}
		if (project_space_expand != -1) {
			g_ptr_array_add(event_argv, g_strdup("--space-expand"));
			g_ptr_array_add(event_argv, g_strdup(
				project_space_expand ? "true" : "false"));
		}
		if (project_space_fill != -1) {
			g_ptr_array_add(event_argv, g_strdup("--space-fill"));
			g_ptr_array_add(event_argv, g_strdup(
				project_space_fill ? "true" : "false"));
		}
		if (option_styles_file != NULL) {
			g_ptr_array_add(event_argv, g_strdup("--styles"));
			g_ptr_array_add(event_argv, g_strdup(option_styles_file));
		}
		if (option_input_variable != NULL) {
			g_ptr_array_add(event_argv, g_strdup("--program"));
			g_ptr_array_add(event_argv, g_strdup(option_input_variable));
		}
		if (option_input_file != NULL) {
			g_ptr_array_add(event_argv, g_strdup("--file"));
			g_ptr_array_add(event_argv, g_strdup(option_input_file));
		}
		g_ptr_array_add(event_argv, g_strdup("--include"));
		g_ptr_array_add(event_argv, g_strdup(option_event_file));
		g_ptr_array_add(event_argv, NULL);
		event_argv_data = (gchar **)g_ptr_array_free(event_argv, FALSE);

		spawned = g_spawn_sync(NULL, event_argv_data, NULL, 0,
			NULL, NULL, NULL, NULL, &wait_status, &spawn_error);
		g_strfreev(event_argv_data);
		if (!spawned) {
			g_error("Couldn't start event-driven dialog: %s",
				spawn_error->message);
		}
		if (!g_spawn_check_exit_status(wait_status, &spawn_error)) {
			gtkdialog_warning("Event-driven dialog failed: %s", spawn_error->message);
			g_error_free(spawn_error);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	
	if (option_input_file != NULL) {
		get_program_from_file(option_input_file);
		goto gtkdialog_initialized;
	}
	
	if (option_input_variable != NULL) {
		get_program_from_variable(option_input_variable);
		goto gtkdialog_initialized;
	}
	
	get_program_from_variable("MAIN_DIALOG");

gtkdialog_initialized:
	gtk_init(&argc, &argv);
	
	if (option_styles_file)
		load_styles_file(option_styles_file);

	gtkdialog_parse();
}
