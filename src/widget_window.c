/*
 * widget_window.c: 
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
#include <errno.h>
#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "tag_attributes.h"

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS

extern gboolean option_centering;
extern gboolean have_geometry_xy;
extern gboolean have_geometry_dxdy;
extern gint geometry_dx;
extern gint geometry_dy;
extern gint geometry_x;
extern gint geometry_y;

/* Local function prototypes, located at file bottom */
static void widget_window_input_by_command(variable *var, char *command);
static void widget_window_input_by_file(variable *var, char *filename);
static void widget_window_input_by_items(variable *var);

static gboolean
widget_plug_embedded_idle(gpointer data)
{
	GtkWidget *widget = data;
	AttributeSet *attributes;

	attributes = g_object_get_data(G_OBJECT(widget), "gtkdialog-attributes");
	if (attributes != NULL)
		widget_signal_executor(widget, attributes, "embedded");
	return FALSE;
}

static void
widget_plug_embedded(GtkPlug *plug, gpointer user_data)
{
	(void)user_data;

	/* gtk_plug_construct() may emit embedded before the root variable and its
	 * owned AttributeSet have been registered.  Run the declared actions once
	 * normal GTK processing begins so general window actions see a complete
	 * program, while preserving their source order. */
	g_idle_add_full(G_PRIORITY_DEFAULT_IDLE, widget_plug_embedded_idle,
		g_object_ref(plug), (GDestroyNotify)g_object_unref);
}

static GdkNativeWindow
widget_plug_parse_socket_id(tag_attr *attr)
{
	gchar *end;
	gchar *value;
	guint64 parsed;

	if (attr == NULL ||
		(value = get_tag_attribute(attr, "socket-id")) == NULL)
		return 0;

	errno = 0;
	parsed = g_ascii_strtoull(value, &end, 10);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == value || *end != '\0' || parsed > G_MAXULONG) {
		gtkdialog_warning("Invalid plug socket-id '%s'; creating an "
			"unattached plug.", value);
		parsed = 0;
	}

	/* socket-id is a constructor argument, not a GObject property. */
	kill_tag_attribute(attr, "socket-id");
	return (GdkNativeWindow)(gulong)parsed;
}

static GtkWindowType widget_window_parse_type(tag_attr *attr)
{
	GtkWindowType window_type = GTK_WINDOW_TOPLEVEL;
	gchar *value;

	if (attr == NULL || (value = get_tag_attribute(attr, "type")) == NULL)
		return window_type;

	if (g_ascii_strcasecmp(value, "toplevel") == 0 ||
		g_ascii_strcasecmp(value, "GTK_WINDOW_TOPLEVEL") == 0)
		window_type = GTK_WINDOW_TOPLEVEL;
	else if (g_ascii_strcasecmp(value, "popup") == 0 ||
		g_ascii_strcasecmp(value, "GTK_WINDOW_POPUP") == 0)
		window_type = GTK_WINDOW_POPUP;
	else
		window_type = (GtkWindowType)widget_parse_bounded_integer(value,
			GTK_WINDOW_TOPLEVEL, GTK_WINDOW_POPUP, GTK_WINDOW_TOPLEVEL,
			"window type");

	/* GtkWindow:type is construct-only and cannot reach the generic pass. */
	kill_tag_attribute(attr, "type");
	return window_type;
}

/* Notes: */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_window_clear(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Well, it can't be null because something changes it to
	 * "Unnamed Window", and since the default is PACKAGE when
	 * no title has been declared I'll clear it to that */

	gtk_window_set_title(GTK_WINDOW(var->Widget), PACKAGE);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/
GtkWidget *widget_window_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	gchar            *resolved_image;
	gchar            *value;
	GError           *error = NULL;
	GList            *accel_group = NULL;
	GList            *element;
	gint              border_width;
	GtkWindowType     window_type;
	GtkWidget        *widget;
	stackelement      s;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Create the window widget */
	if (Type == WIDGET_PLUG) {
		GdkNativeWindow socket_id = widget_plug_parse_socket_id(attr);

		widget = g_object_new(GTK_TYPE_PLUG, NULL);
		g_signal_connect(widget, "embedded",
			G_CALLBACK(widget_plug_embedded), Attr);
		gtk_plug_construct(GTK_PLUG(widget), socket_id);
	} else {
		window_type = widget_window_parse_type(attr);
		widget = gtk_window_new(window_type);
	}

	/* Set a default window title */
	attributeset_set_if_unset(Attr, ATTR_LABEL, PACKAGE);
	gtk_window_set_title(GTK_WINDOW(widget), 
		attributeset_get_first(&element, Attr, ATTR_LABEL));

	/* Set a default title bar theme icon */
	gtk_window_set_icon_name(GTK_WINDOW(widget), PACKAGE);

	/* If requested set a title bar image by filename */
	if (attr) {
		if ((value = get_tag_attribute(attr, "image-name"))) {
			resolved_image = find_pixmap(value);
			if (!gtk_window_set_icon_from_file(GTK_WINDOW(widget),
				resolved_image, &error))
				widget_report_icon_error(value, error);
			g_free(resolved_image);
		}
	}

	/* Set a default border width */
	border_width = 5;
	if (attr && (value = get_tag_attribute(attr, "margin")))	/* Deprecated */
		border_width = widget_parse_nonnegative_integer(value, 5,
			"window margin");
	gtk_container_set_border_width(GTK_CONTAINER(widget), border_width);

	/* If we have geometry given in the command line, we set that */
	if (have_geometry_dxdy)
		gtk_widget_set_size_request(widget, geometry_dx, geometry_dy);
	if (have_geometry_xy)
		gtk_window_move(GTK_WINDOW(widget), geometry_x, geometry_y);
	if (option_centering)
		gtk_window_set_position(GTK_WINDOW(widget),
			GTK_WIN_POS_CENTER_ALWAYS);

	/* Pop the widgets that the window will contain and add them */
	s = pop();
	if (s.nwidgets > 0)
		gtk_container_add(GTK_CONTAINER(widget), s.widgets[0]);
	stackelement_clear(&s);

	/* Thunor: Each menu created will have an accelerator group
	 * for its menuitems which will require adding to the window */
	if (accel_groups) {
		accel_group = g_list_first(accel_groups);
		while (accel_group) {
			gtk_window_add_accel_group(GTK_WINDOW(widget),
				GTK_ACCEL_GROUP(accel_group->data));
#ifdef DEBUG
			fprintf(stderr, "%s: Adding accel_group=%p to window\n",
				__func__, accel_group->data);
#endif
			/* The menu and window now hold their own references. */
			g_object_unref(accel_group->data);
			accel_group = accel_group->next;
		}
		g_list_free(accel_groups);
		accel_groups = NULL;
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_window_envvar_construct(GtkWidget *widget)
{
	gchar            *string;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: Variables are exported before a window is launched which
	 * can be a problem if launching from a launched window because
	 * launched windows are required to contain a variable that matches
	 * the name of the program's envvar and this results in the program
	 * being overwritten with the window title ;) It took me a day to
	 * find this issue because the error was being reported by the parser
	 * but the problem was located in action_launchwindow. So window
	 * variables can't be exported, they must be reserved for scripts.
	 * 
	 * string = g_strdup(gtk_window_get_title(GTK_WINDOW(widget))); */
	string = NULL;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_window_fileselect(
	variable *var, const char *name, const char *value)
{
	gchar            *var1;
	gint              var2;

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
void widget_window_refresh(variable *var)
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
			widget_window_input_by_command(var, act + 8);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, act + 5);
			}
			widget_window_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_window_input_by_items(var);

	/* Initialise these only once at start-up */
	if (!initialised) {
		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			fprintf(stderr, "%s(): <default> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
			fprintf(stderr, "%s(): <height> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
			fprintf(stderr, "%s(): <width> not implemented for this widget.\n",
				__func__);
		if ((attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled")) ||	/* Deprecated */
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
			gtk_widget_set_sensitive(var->Widget, FALSE);

		/* Connect signals */
		g_signal_connect(G_OBJECT(var->Widget), "delete-event",
			G_CALLBACK(window_delete_event_handler), NULL);

	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_window_removeselected(variable *var)
{
	gchar            *var1;
	gint              var2;

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

void widget_window_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	gchar            *act;
	gchar            *filename = NULL;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We'll use the output file filename if available */
	act = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (act) {
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			filename = act + 5;
			break;
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_OUTPUT);
	}

	/* If we have a valid filename then open it and dump the
	 * widget's data to it */
	if (filename) {
		if ((outfile = fopen(filename, "w"))) {
			fprintf(outfile, "%s", gtk_window_get_title(GTK_WINDOW(var->Widget)));
			/* Close the file */
			widget_close_output(outfile, filename);
		} else {
			fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
				__func__, filename);
		}
	} else {
		fprintf(stderr, "%s(): No <output file> directive found.\n", __func__);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Command                                                    *
 ***********************************************************************/

static void widget_window_input_by_command(variable *var, char *command)
{
	FILE             *infile;
	gchar            *line;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): command: '%s'\n", __func__, command);
#endif

	/* Opening pipe for reading... */
	if (infile = widget_opencommand(command)) {
		/* Just one line */
		if ((line = widget_read_line(infile)) != NULL) {
			gtk_window_set_title(GTK_WINDOW(var->Widget), line);
			g_free(line);
		}
		/* Close the file */
		widget_closecommand(infile, command);
	} else {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n", __func__,
			command);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by File                                                       *
 ***********************************************************************/

static void widget_window_input_by_file(variable *var, char *filename)
{
	FILE             *infile;
	gchar            *line;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (infile = fopen(filename, "r")) {
		/* Just one line */
		if ((line = widget_read_line(infile)) != NULL) {
			gtk_window_set_title(GTK_WINDOW(var->Widget), line);
			g_free(line);
		}
		/* Close the file */
		fclose(infile);
	} else {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n", __func__,
			filename);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Items                                                      *
 ***********************************************************************/

static void widget_window_input_by_items(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): <item> not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
