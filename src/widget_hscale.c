/*
 * widget_hscale.c: 
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
#include <math.h>
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

/* Local function prototypes, located at file bottom */
static void widget_hscale_input_by_command(variable *var, char *command);
static void widget_hscale_input_by_file(variable *var, char *filename);
static void widget_hscale_input_by_items(variable *var);

static gboolean widget_hscale_parse_mark_value(const gchar *text,
	gdouble *result)
{
	gchar *end;
	gdouble value;
	gboolean converted;

	errno = 0;
	value = g_ascii_strtod(text, &end);
	converted = end != text;
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || !converted || *end != '\0' || !isfinite(value)) {
		gtkdialog_warning("Invalid scale mark value '%s'; mark ignored.", text);
		return FALSE;
	}

	*result = value;
	return TRUE;
}

static gboolean widget_hscale_parse_mark_position(const gchar *text,
	gint *result)
{
	gchar *end;
	gint64 position;
	gboolean converted;

	errno = 0;
	position = g_ascii_strtoll(text, &end, 10);
	converted = end != text;
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || !converted || *end != '\0' ||
		position < GTK_POS_LEFT || position > GTK_POS_BOTTOM) {
		gtkdialog_warning("Invalid scale mark position '%s'; mark ignored.", text);
		return FALSE;
	}

	*result = (gint)position;
	return TRUE;
}

static void widget_hscale_validate_range(gdouble *minimum, gdouble *maximum,
	gdouble *step)
{
	if (*minimum >= *maximum || !isfinite(*maximum - *minimum)) {
		gtkdialog_warning("Invalid scale range; using 0..100.");
		*minimum = 0.0;
		*maximum = 100.0;
	}
	if (*step <= 0.0) {
		gtkdialog_warning("Invalid scale range-step; using 1.");
		*step = 1.0;
	}
}

/* Notes: */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_hscale_clear(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Clear not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/

GtkWidget *widget_hscale_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget        *widget;
	gdouble           range_min = 0;
	gdouble           range_max = 100;
	gdouble           range_step = 1;
	gdouble           range_value = 0;
	gchar            *value;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: These "range-*" names are consistent with the spinbutton widget */
	if (attr) {
		if (!(value = get_tag_attribute(attr, "range-min")))
			value = get_tag_attribute(attr, "scale-min");
		if (value)
			widget_parse_finite_double(value, &range_min, "scale range-min");

		if (!(value = get_tag_attribute(attr, "range-max")))
			value = get_tag_attribute(attr, "scale-max");
		if (value)
			widget_parse_finite_double(value, &range_max, "scale range-max");

		if (!(value = get_tag_attribute(attr, "range-step")))
			value = get_tag_attribute(attr, "scale-step");
		if (value)
			widget_parse_finite_double(value, &range_step, "scale range-step");

		if (!(value = get_tag_attribute(attr, "range-value")))
			value = get_tag_attribute(attr, "scale-value");
		if (value)
			widget_parse_finite_double(value, &range_value, "scale range-value");
	}
	widget_hscale_validate_range(&range_min, &range_max, &range_step);

	if (Type == WIDGET_HSCALE) {
		widget = gtk_hscale_new_with_range(range_min, range_max, range_step);
	} else {
		widget = gtk_vscale_new_with_range(range_min, range_max, range_step);
	}
	gtk_range_set_value(GTK_RANGE(widget), range_value);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_hscale_envvar_construct(GtkWidget *widget)
{
	gchar            *string;
	gdouble           val;
	gint              digits;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	digits = gtk_scale_get_digits(GTK_SCALE(widget));
	val = gtk_range_get_value(GTK_RANGE(widget));
	/* Preserve the historical six-digit fallback above 16 digits. */
	if (digits < 0 || digits > 16)
		digits = 6;
	string = g_strdup_printf("%.*f", digits, val);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_hscale_fileselect(
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

void widget_hscale_refresh(variable *var)
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
			widget_hscale_input_by_command(var, act + 8);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, act + 5);
			}
			widget_hscale_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM)) {
#if GTK_CHECK_VERSION(2,16,0)
		gtk_scale_clear_marks(GTK_SCALE(var->Widget));
#endif
		widget_hscale_input_by_items(var);
	}

	/* Initialise these only once at start-up */
	if (!initialised) {
		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
			fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT)) {
			gdouble default_value;
			const gchar *default_text = attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT);

			if (widget_parse_finite_double(default_text, &default_value,
				"scale default"))
				gtk_range_set_value(GTK_RANGE(var->Widget), default_value);
		}
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
		g_signal_connect(G_OBJECT(var->Widget), "value-changed",
			G_CALLBACK(on_any_widget_value_changed_event), (gpointer)var->Attributes);

	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_hscale_removeselected(variable *var)
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

void widget_hscale_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	gchar            *act;
	gchar            *filename = NULL;
	gdouble           value;
	guint             digits;

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

			digits = gtk_scale_get_digits(GTK_SCALE(var->Widget));
			value = gtk_range_get_value(GTK_RANGE(var->Widget));
			switch (digits) {
				case 0:
					fprintf(outfile, "%.0f", value);
					break;
				case 1:
					fprintf(outfile, "%.1f", value);
					break;
				case 2:
					fprintf(outfile, "%.2f", value);
					break;
				case 3:
					fprintf(outfile, "%.3f", value);
					break;
				case 4:
					fprintf(outfile, "%.4f", value);
					break;
				case 5:
					fprintf(outfile, "%.5f", value);
					break;
				case 6:
					fprintf(outfile, "%.6f", value);
					break;
				case 7:
					fprintf(outfile, "%.7f", value);
					break;
				case 8:
					fprintf(outfile, "%.8f", value);
					break;
				case 9:
					fprintf(outfile, "%.9f", value);
					break;
				case 10:
					fprintf(outfile, "%.10f", value);
					break;
				case 11:
					fprintf(outfile, "%.11f", value);
					break;
				case 12:
					fprintf(outfile, "%.12f", value);
					break;
				case 13:
					fprintf(outfile, "%.13f", value);
					break;
				case 14:
					fprintf(outfile, "%.14f", value);
					break;
				case 15:
					fprintf(outfile, "%.15f", value);
					break;
				case 16:
					fprintf(outfile, "%.16f", value);
					break;
				/* Is there much point going beyond 16? */
				default:
					fprintf(outfile, "%f", value);
					break;
			}

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

static void widget_hscale_input_by_command(variable *var, char *command)
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
		if ((line = widget_read_line(infile))) {
			gdouble value;

			if (widget_parse_finite_double(line, &value,
				"scale command input"))
				gtk_range_set_value(GTK_RANGE(var->Widget), value);
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

static void widget_hscale_input_by_file(variable *var, char *filename)
{
	FILE             *infile;
	gchar            *line;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (infile = fopen(filename, "r")) {
		/* Just one line */
		if ((line = widget_read_line(infile))) {
			gdouble value;

			if (widget_parse_finite_double(line, &value, "scale file input"))
				gtk_range_set_value(GTK_RANGE(var->Widget), value);
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

static void widget_hscale_input_by_items(variable *var)
{
	GList            *element;
	gchar           **parts;
	gchar            *markup;
	gchar            *text;
	gdouble           value;
	gint              position;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if GTK_CHECK_VERSION(2,16,0)

	g_assert(var->Attributes != NULL && var->Widget != NULL);

	text = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (text) {
		parts = g_strsplit(text, "|", 3);
		if (parts[0] != NULL && parts[1] != NULL &&
			widget_hscale_parse_mark_value(g_strstrip(parts[0]), &value) &&
			widget_hscale_parse_mark_position(g_strstrip(parts[1]), &position)) {
			markup = parts[2] != NULL ? parts[2] : "";
#ifdef DEBUG_CONTENT
			printf("%s: value=%.16f position=%i markup='%s'\n",
				__func__, value, position, markup);
#endif
			gtk_scale_add_mark(GTK_SCALE(var->Widget), value, position, markup);
		} else if (parts[0] == NULL || parts[1] == NULL) {
			gtkdialog_warning("Malformed scale mark '%s'; mark ignored.", text);
		}
		g_strfreev(parts);
		text = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
