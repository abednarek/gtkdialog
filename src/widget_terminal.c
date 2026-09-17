/*
 * widget_terminal.c: 
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

/* Includes */
#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <errno.h>
#ifndef G_OS_WIN32
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#include "config.h"
#include "gtkdialog.h"
#include "widget_terminal.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "tag_attributes.h"
#if HAVE_VTE
#include <vte/vte.h>
#endif

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS

#define VTE_WARNING "The terminal (VteTerminal) widget requires \
a version of gtkdialog built with libvte."

/* Local functions */
static void widget_terminal_input_by_command(variable *var, char *command);
static void widget_terminal_input_by_file(variable *var, char *filename);
static void widget_terminal_input_by_items(variable *var);
#if HAVE_VTE
static void widget_terminal_set_palette(VteTerminal *terminal,
	const gchar *value)
{
	GdkColor colors[16];
	gchar **parts;
	guint i, count;
	gboolean valid = TRUE;

	parts = g_strsplit(value, ",", -1);
	count = g_strv_length(parts);
	if (count != 8 && count != 16)
		valid = FALSE;
	for (i = 0; valid && i < count; ++i)
		if (!gdk_color_parse(g_strstrip(parts[i]), &colors[i]))
			valid = FALSE;
	if (valid)
		vte_terminal_set_colors(terminal, NULL, NULL, colors, count);
	else
		gtkdialog_warning("Invalid terminal palette; expected 8 or 16 comma-separated colors.");
	g_strfreev(parts);
}

static void widget_terminal_set_scrollback(VteTerminal *terminal,
	const gchar *value)
{
	const gchar *start = value;
	gchar *end;
	gint64 lines;
	gboolean has_digits;

	while (g_ascii_isspace(*start))
		++start;
	errno = 0;
	lines = g_ascii_strtoll(start, &end, 10);
	has_digits = end != start;
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || !has_digits || *end != '\0' ||
		lines < 0 || lines > G_MAXINT) {
		gtkdialog_warning("Invalid terminal scrollback-lines '%s'; ignored.", value);
		return;
	}
	vte_terminal_set_scrollback_lines(terminal, (glong)lines);
}

static void widget_terminal_set_boolean(VteTerminal *terminal,
	tag_attr *attr, const gchar *name)
{
	const gchar *value = get_tag_attribute(attr, name);
	gboolean enabled;

	if (value == NULL)
		return;
	if (!widget_parse_boolean(value, &enabled)) {
		gtkdialog_warning("Invalid terminal %s value '%s'; ignored.", name, value);
		kill_tag_attribute(attr, name);
		return;
	}
	if (strcmp(name, "audible-bell") == 0)
		vte_terminal_set_audible_bell(terminal, enabled);
	else if (strcmp(name, "visible-bell") == 0)
		vte_terminal_set_visible_bell(terminal, enabled);
	else if (strcmp(name, "scroll-on-output") == 0)
		vte_terminal_set_scroll_on_output(terminal, enabled);
	else if (strcmp(name, "scroll-on-keystroke") == 0)
		vte_terminal_set_scroll_on_keystroke(terminal, enabled);
	kill_tag_attribute(attr, name);
}

static gint widget_terminal_pixel_dimension(GtkWidget *widget,
	gint requested_cells, gint default_cells, gboolean horizontal,
	gint *actual_cells);
#endif

/* Notes: */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_terminal_clear(variable *var)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_VTE
	/* This won't result in child-exited being emitted.  Do not replace an
	 * active PTY: VTE would stop associating its terminal with the old child,
	 * so a child which survives the closed PTY could no longer be managed by
	 * this widget. */
	vte_terminal_reset(VTE_TERMINAL(var->Widget), TRUE, TRUE);
	if (GPOINTER_TO_INT(g_object_get_data(G_OBJECT(var->Widget), "_pid")) <= 0)
		widget_terminal_fork_command(var->Widget, var->widget_tag_attr);
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/
GtkWidget *widget_terminal_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget        *widget;
#if HAVE_VTE
	GdkColor          color;
	GList            *element;
	gchar            *value;
	gint              width = -1, height = -1;
	gint              width_cells, height_cells;
	gint              width_pixels, height_pixels;
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_VTE
	/* Read declared directives */
	if (attributeset_is_avail(Attr, ATTR_WIDTH))
		width = widget_parse_size(attributeset_get_first(&element, Attr,
			ATTR_WIDTH), "terminal width");
	if (attributeset_is_avail(Attr, ATTR_HEIGHT))
		height = widget_parse_size(attributeset_get_first(&element, Attr,
			ATTR_HEIGHT), "terminal height");

	widget = vte_terminal_new();
	
	/* Until I added this, none of the functions that set various text
	 * foreground and background colours worked so I'm presuming that
	 * creating a new VteTerminal doesn't create a full palette */
	vte_terminal_set_default_colors(VTE_TERMINAL(widget));

	if (attr) {
		if ((value = get_tag_attribute(attr, "palette"))) {
			widget_terminal_set_palette(VTE_TERMINAL(widget), value);
			kill_tag_attribute(attr, "palette");
		}
		if ((value = get_tag_attribute(attr, "scrollback-lines"))) {
			widget_terminal_set_scrollback(VTE_TERMINAL(widget), value);
			kill_tag_attribute(attr, "scrollback-lines");
		}
		if ((value = get_tag_attribute(attr, "cursor-shape"))) {
			VteTerminalCursorShape shape;

			if (g_ascii_strcasecmp(value, "block") == 0)
				shape = VTE_CURSOR_SHAPE_BLOCK;
			else if (g_ascii_strcasecmp(value, "ibeam") == 0)
				shape = VTE_CURSOR_SHAPE_IBEAM;
			else if (g_ascii_strcasecmp(value, "underline") == 0)
				shape = VTE_CURSOR_SHAPE_UNDERLINE;
			else {
				gtkdialog_warning("Invalid terminal cursor-shape '%s'; ignored.", value);
				shape = VTE_CURSOR_SHAPE_BLOCK;
			}
			if (g_ascii_strcasecmp(value, "block") == 0 ||
				g_ascii_strcasecmp(value, "ibeam") == 0 ||
				g_ascii_strcasecmp(value, "underline") == 0)
				vte_terminal_set_cursor_shape(VTE_TERMINAL(widget), shape);
			kill_tag_attribute(attr, "cursor-shape");
		}
		if ((value = get_tag_attribute(attr, "cursor-blink"))) {
			if (g_ascii_strcasecmp(value, "system") == 0)
				vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(widget),
					VTE_CURSOR_BLINK_SYSTEM);
			else if (g_ascii_strcasecmp(value, "on") == 0)
				vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(widget),
					VTE_CURSOR_BLINK_ON);
			else if (g_ascii_strcasecmp(value, "off") == 0)
				vte_terminal_set_cursor_blink_mode(VTE_TERMINAL(widget),
					VTE_CURSOR_BLINK_OFF);
			else
				gtkdialog_warning("Invalid terminal cursor-blink '%s'; ignored.", value);
			kill_tag_attribute(attr, "cursor-blink");
		}
		widget_terminal_set_boolean(VTE_TERMINAL(widget), attr, "audible-bell");
		widget_terminal_set_boolean(VTE_TERMINAL(widget), attr, "visible-bell");
		widget_terminal_set_boolean(VTE_TERMINAL(widget), attr, "scroll-on-output");
		widget_terminal_set_boolean(VTE_TERMINAL(widget), attr, "scroll-on-keystroke");

		/* The gtk property "font-desc" requires a pointer to a
		 * PangoFontDescription but the application developer can't pass
		 * anything other than a string here, so we'll convert it using
		 * a dedicated gtk function and then kill the tag attribute else
		 * widget_set_tag_attributes() will try to set it later */
		if ((value = get_tag_attribute(attr, "font-desc"))) {
			vte_terminal_set_font_from_string(VTE_TERMINAL(widget), value);
			kill_tag_attribute(attr, "font-desc");
		}

		/* Again, "background-tint-color" requires a pointer to a
		 * GdkColor struct but we can convert a string like "#ff00ff" */
		if ((value = get_tag_attribute(attr, "background-tint-color"))) {
			/* Parse the RGB value to create the necessary GdkColor.
			 * This function doesn't like trailing whitespace so it
			 * needs to be stripped first with g_strstrip() */ 
			if (gdk_color_parse(g_strstrip(value), &color)) {
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s:() valid colour found\n", __func__);
#endif
				vte_terminal_set_background_tint_color(VTE_TERMINAL(widget), &color);
			}
			kill_tag_attribute(attr, "background-tint-color");
		}

		/* Get custom tag attribute "font-name" */
		if ((value = get_tag_attribute(attr, "font-name"))) {
			vte_terminal_set_font_from_string(VTE_TERMINAL(widget), value);
		}

		/* Get custom tag attribute "text-background-color" */
		if ((value = get_tag_attribute(attr, "text-background-color"))) {
			/* Parse the RGB value to create the necessary GdkColor.
			 * This function doesn't like trailing whitespace so it
			 * needs to be stripped first with g_strstrip() */ 
			if (gdk_color_parse(g_strstrip(value), &color)) {
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s:() valid colour found\n", __func__);
#endif
				vte_terminal_set_color_background(VTE_TERMINAL(widget), &color);
			}
		}

		/* Get custom tag attribute "text-foreground-color" */
		if ((value = get_tag_attribute(attr, "text-foreground-color"))) {
			/* Parse the RGB value to create the necessary GdkColor.
			 * This function doesn't like trailing whitespace so it
			 * needs to be stripped first with g_strstrip() */ 
			if (gdk_color_parse(g_strstrip(value), &color)) {
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s:() valid colour found\n", __func__);
#endif
				vte_terminal_set_color_foreground(VTE_TERMINAL(widget), &color);
			}
		}

		/* Get custom tag attribute "bold-foreground-color" */
		if ((value = get_tag_attribute(attr, "bold-foreground-color"))) {
			/* Parse the RGB value to create the necessary GdkColor.
			 * This function doesn't like trailing whitespace so it
			 * needs to be stripped first with g_strstrip() */ 
			if (gdk_color_parse(g_strstrip(value), &color)) {
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s:() valid colour found\n", __func__);
#endif
				vte_terminal_set_color_bold(VTE_TERMINAL(widget), &color);
			}
		}

		/* Get custom tag attribute "dim-foreground-color" */
		if ((value = get_tag_attribute(attr, "dim-foreground-color"))) {
			/* Parse the RGB value to create the necessary GdkColor.
			 * This function doesn't like trailing whitespace so it
			 * needs to be stripped first with g_strstrip() */ 
			if (gdk_color_parse(g_strstrip(value), &color)) {
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s:() valid colour found\n", __func__);
#endif
				vte_terminal_set_color_dim(VTE_TERMINAL(widget), &color);
			}
		}

		/* Get custom tag attribute "cursor-background-color" */
		if ((value = get_tag_attribute(attr, "cursor-background-color"))) {
			/* Parse the RGB value to create the necessary GdkColor.
			 * This function doesn't like trailing whitespace so it
			 * needs to be stripped first with g_strstrip() */ 
			if (gdk_color_parse(g_strstrip(value), &color)) {
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s:() valid colour found\n", __func__);
#endif
				vte_terminal_set_color_cursor(VTE_TERMINAL(widget), &color);
			}
		}

		/* Get custom tag attribute "highlight-background-color" */
		if ((value = get_tag_attribute(attr, "highlight-background-color"))) {
			/* Parse the RGB value to create the necessary GdkColor.
			 * This function doesn't like trailing whitespace so it
			 * needs to be stripped first with g_strstrip() */ 
			if (gdk_color_parse(g_strstrip(value), &color)) {
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s:() valid colour found\n", __func__);
#endif
				vte_terminal_set_color_highlight(VTE_TERMINAL(widget), &color);
			}
		}
	}

	/* GTK2/X11 cannot create native windows larger than G_MAXSHORT pixels.
	 * Convert the terminal's character dimensions once, after its font has
	 * been applied, and retain the safe pixel dimensions for the surrounding
	 * scrolled window. */
	width_pixels = widget_terminal_pixel_dimension(widget, width, 80,
		TRUE, &width_cells);
	height_pixels = widget_terminal_pixel_dimension(widget, height, 25,
		FALSE, &height_cells);
	g_object_set_data(G_OBJECT(widget), "_gtkdialog-terminal-width",
		GINT_TO_POINTER(width_pixels));
	g_object_set_data(G_OBJECT(widget), "_gtkdialog-terminal-height",
		GINT_TO_POINTER(height_pixels));

	/* Set width and height if both supplied */
	if (width != -1 && height != -1)
		vte_terminal_set_size(VTE_TERMINAL(widget), width_cells, height_cells);

	/* And off we go... */
	widget_terminal_fork_command(widget, attr);
#else
	/* If libvte support is missing then create a label instead */
	widget = gtk_label_new(VTE_WARNING);
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

#if HAVE_VTE
static gint widget_terminal_pixel_dimension(GtkWidget *widget,
	gint requested_cells, gint default_cells, gboolean horizontal,
	gint *actual_cells)
{
	gint xpad, ypad;
	gint padding;
	gint cells;
	glong cell_extent;
	glong max_cells;
	const gchar *dimension;

	vte_terminal_get_padding(VTE_TERMINAL(widget), &xpad, &ypad);
	padding = horizontal ? xpad : ypad;
	cell_extent = horizontal
		? vte_terminal_get_char_width(VTE_TERMINAL(widget))
		: vte_terminal_get_char_height(VTE_TERMINAL(widget));
	dimension = horizontal ? "width" : "height";

	/* VTE normally reports small positive metrics. Keep malformed metrics
	 * from participating in division or multiplication nevertheless. */
	if (cell_extent <= 0 || padding < 0 || padding >= G_MAXSHORT) {
		gtkdialog_warning("Invalid terminal %s metrics; using automatic sizing.",
			dimension);
		*actual_cells = default_cells;
		return -1;
	}

	max_cells = (G_MAXSHORT - padding) / cell_extent;
	cells = requested_cells == -1 ? default_cells : requested_cells;
	if (cells > max_cells) {
		gtkdialog_warning("Terminal %s value '%d' exceeds the GTK2 window "
			"limit; using %d.", dimension, requested_cells, default_cells);
		cells = MIN((glong)default_cells, max_cells);
	}

	*actual_cells = cells;
	return (gint)(cells * cell_extent + padding);
}
#endif

/***********************************************************************
 * Fork Command                                                        *
 ***********************************************************************/

void widget_terminal_fork_command(GtkWidget *widget, tag_attr *attr)
{
#if HAVE_VTE
	static gchar     *argv0 = "/bin/sh";
	gchar           **argv;
	gchar           **envv;
	gchar            *tagattribute;
	gchar            *value;
	gchar            *working_directory = NULL;
	gsize             vector_size;
	gint              count;
#if VTE_CHECK_VERSION(0,26,0)
	gboolean          retval;
	GError           *error = NULL;
	GPid              pid = 0;
#else
	pid_t             pid = 0;
#endif
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_VTE
	vector_size = attr && attr->n > 0 ? (gsize)attr->n + 2 : 2;
	argv = g_new0(gchar*, vector_size);
	envv = g_new0(gchar*, vector_size);
	argv[0] = argv0;	/* Set a default command otherwise it segfaults */

	if (attr) {
		/* The "current-directory-uri" can only be set when we fork a
		 * command (there's no function for it) so we set it now */
		if ((value = get_tag_attribute(attr, "current-directory-uri")))
			working_directory = value;

		/* Argument and environment vectors are contiguous and terminated by
		 * the zero-filled slot after the last declared attribute. */
		for (count = 0; count <= attr->n; count++) {
			tagattribute = g_strdup_printf("argv%i", count);
			value = get_tag_attribute(attr, tagattribute);
			if (value)
				argv[count] = value;
#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s:() %s=%s\n", __func__, tagattribute,
				value ? value : "(null)");
#endif
			g_free(tagattribute);
			if (value == NULL && count > 0)
				break;
		}
		for (count = 0; count <= attr->n; count++) {
			tagattribute = g_strdup_printf("envv%i", count);
			value = get_tag_attribute(attr, tagattribute);
			if (value)
				envv[count] = value;
#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s:() %s=%s\n", __func__, tagattribute,
				value ? value : "(null)");
#endif
			g_free(tagattribute);
			if (value == NULL)
				break;
		}
	}

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s:() working_directory=%s\n", __func__,
		working_directory ? working_directory : "(null)");
	fprintf(stderr, "%s:() argv=%p *argv=%s argv[0]=%s argv[1]=%s\n",
		__func__, argv, *argv, argv[0], argv[1] ? argv[1] : "(null)");
	fprintf(stderr, "%s:() envv=%p *envv=%s envv[0]=%s envv[1]=%s\n",
		__func__, envv, *envv ? *envv : "(null)",
		envv[0] ? envv[0] : "(null)", envv[1] ? envv[1] : "(null)");
#endif

#if VTE_CHECK_VERSION(0,26,0)
	retval = (vte_terminal_fork_command_full(VTE_TERMINAL(widget),
		VTE_PTY_DEFAULT,
		working_directory,
		argv,
		envv,
		G_SPAWN_SEARCH_PATH,
		NULL,
		NULL,
		&pid,
		&error));
	if (!retval) {
		fprintf(stderr, "%s(): vte_terminal_fork_command_full(): %s\n",
			__func__, error ? error->message : "unknown error");
		if (error)
			g_error_free(error);
	}
#else
	pid = (vte_terminal_fork_command(VTE_TERMINAL(widget),
		argv[0],
		argv,
		envv,
		working_directory,
		TRUE,
		TRUE,
		TRUE));
	if (pid == -1)
		fprintf(stderr, "%s(): vte_terminal_fork_command(): %s\n",
			__func__, "error");
#endif

	/* Store "pid" as a piece of widget data (recreated if exists)  */
	g_object_set_data(G_OBJECT(widget), "_pid", GINT_TO_POINTER(pid));
	g_free(argv);
	g_free(envv);

#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

gboolean widget_terminal_hangup(GtkWidget *widget)
{
#if HAVE_VTE && !defined(G_OS_WIN32)
	pid_t pid;

	if (!VTE_IS_TERMINAL(widget))
		return FALSE;
	pid = (pid_t)GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget), "_pid"));
	if (pid <= 0)
		return TRUE;
	if (kill(pid, SIGHUP) == 0 || errno == ESRCH)
		return TRUE;
	gtkdialog_warning("Cannot hang up terminal child %ld: %s",
		(long)pid, g_strerror(errno));
	return FALSE;
#else
	(void)widget;
	gtkdialog_warning("hangupterminal requires VTE support.");
	return FALSE;
#endif
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_terminal_envvar_construct(GtkWidget *widget)
{
	gchar            *string;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_VTE
	string = g_strdup_printf("%i", GPOINTER_TO_INT(
		g_object_get_data(G_OBJECT(widget), "_pid")));
#else
	string = g_strdup("");
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_terminal_fileselect(
	variable *var, const char *name, const char *value)
{

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Refresh                                                             *
 ***********************************************************************/
void widget_terminal_refresh(variable *var)
{
	GList            *element;
	gchar            *act;
	gint              initialised = FALSE;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(var->Widget), "_initialised"));

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		if (input_is_shell_command(act))
			widget_terminal_input_by_command(var, act + 8);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, act + 5);
			}
			widget_terminal_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_terminal_input_by_items(var);

	/* Initialise these only once at start-up */
	if (!initialised) {
		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
			fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			fprintf(stderr, "%s(): <default> not implemented for this widget.\n",
				__func__);
		if ((attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled")) ||	/* Deprecated */
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
			gtk_widget_set_sensitive(var->Widget, FALSE);

		/* Connect signals */
#if HAVE_VTE
		g_signal_connect(G_OBJECT(var->Widget), "child-exited",
			G_CALLBACK(on_any_widget_child_exited_event), (gpointer)var->Attributes);
#endif

	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_terminal_removeselected(variable *var)
{

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_terminal_save(variable *var)
{

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Save not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Command                                                    *
 ***********************************************************************/

static void widget_terminal_input_by_command(variable *var, char *command)
{
#if HAVE_VTE
	FILE             *infile;
	gchar            *text = NULL;
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): command: '%s'\n", __func__, command);
#endif

#if HAVE_VTE
	/* Opening pipe for reading... */
	if ((infile = widget_opencommand(command))) {
		text = widget_read_all(infile);

		vte_terminal_feed_child(VTE_TERMINAL(var->Widget), text, strlen(text));

		g_free(text);
		/* Close the file */
		widget_closecommand(infile, command);
	} else {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n", __func__,
			command);
	}
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by File                                                       *
 ***********************************************************************/

static void widget_terminal_input_by_file(variable *var, char *filename)
{
#if HAVE_VTE
	FILE             *infile;
	gchar            *text = NULL;
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_VTE
	if ((infile = fopen(filename, "r"))) {
		text = widget_read_all(infile);

		vte_terminal_feed_child(VTE_TERMINAL(var->Widget), text, strlen(text));

		g_free(text);
		/* Close the file */
		fclose(infile);
	} else {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n", __func__,
			filename);
	}
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Items                                                      *
 ***********************************************************************/

static void widget_terminal_input_by_items(variable *var)
{

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): <item> not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
