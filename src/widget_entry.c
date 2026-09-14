/*
 * widget_entry.c: 
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
static void widget_entry_input_by_command(
	variable *var, char *command, gboolean completion_source);
static void widget_entry_input_by_file(
	variable *var, char *filename, gboolean completion_source);
static void widget_entry_input_by_items(variable *var);

#if GTK_CHECK_VERSION(2,4,0)
enum {
	ENTRY_COMPLETION_TEXT,
	ENTRY_COMPLETION_COLUMNS
};

static void widget_entry_completion_append(GtkWidget *widget,
	const gchar *text)
{
	GtkEntryCompletion *completion;
	GtkListStore *store;
	GtkTreeIter iter;

	completion = gtk_entry_get_completion(GTK_ENTRY(widget));
	store = GTK_LIST_STORE(gtk_entry_completion_get_model(completion));
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, ENTRY_COMPLETION_TEXT, text, -1);
}

static gboolean widget_entry_input_is_completion(
	GList **element, variable *var)
{
	gchar *value;
	gboolean completion_source;

	value = attributeset_get_this_tagattr(element, var->Attributes,
		ATTR_INPUT, (gchar *)"completion");
	if (value == NULL)
		return FALSE;
	if (!widget_parse_boolean(value, &completion_source)) {
		gtkdialog_warning("Invalid entry input completion value '%s'; "
			"using false.", value);
		return FALSE;
	}
	return completion_source;
}

static void widget_entry_completion_clear(GtkWidget *widget)
{
	GtkEntryCompletion *completion;
	GtkTreeModel *model;

	completion = gtk_entry_get_completion(GTK_ENTRY(widget));
	if (completion == NULL)
		return;
	model = gtk_entry_completion_get_model(completion);
	gtk_list_store_clear(GTK_LIST_STORE(model));
}
#endif

/* Notes: */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_entry_clear(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	gtk_entry_set_text(GTK_ENTRY(var->Widget), "");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/

GtkWidget *widget_entry_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget        *widget;
	gchar            *value;
	gboolean          completion_enabled = FALSE;
	gint              minimum_key_length = 1;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	widget = gtk_entry_new();
	if (attr != NULL) {
		value = get_tag_attribute(attr, "completion");
		if (value != NULL &&
			!widget_parse_boolean(value, &completion_enabled)) {
			gtkdialog_warning("Invalid entry completion value '%s'; "
				"using false.", value);
			completion_enabled = FALSE;
		}
		value = get_tag_attribute(attr, "completion-minimum-key-length");
		if (value != NULL)
			minimum_key_length = widget_parse_bounded_integer(value, 0,
				G_MAXINT, 1, "entry completion-minimum-key-length");
		kill_tag_attribute(attr, "completion");
		kill_tag_attribute(attr, "completion-minimum-key-length");
	}
#if GTK_CHECK_VERSION(2,4,0)
	if (completion_enabled) {
		GtkEntryCompletion *completion;
		GtkListStore *store;

		store = gtk_list_store_new(ENTRY_COMPLETION_COLUMNS, G_TYPE_STRING);
		completion = gtk_entry_completion_new();
		gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));
		g_object_unref(store);
		gtk_entry_completion_set_text_column(completion,
			ENTRY_COMPLETION_TEXT);
		gtk_entry_completion_set_minimum_key_length(completion,
			minimum_key_length);
		gtk_entry_set_completion(GTK_ENTRY(widget), completion);
		g_object_unref(completion);
	}
#else
	if (completion_enabled)
		gtkdialog_warning("Entry completion requires GTK+ 2.4 or later.");
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_entry_envvar_construct(GtkWidget *widget)
{
	gchar            *string;
	gchar            *text;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	text = (gchar*)gtk_entry_get_text(GTK_ENTRY(widget));

	string = g_strdup(text);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_entry_fileselect(
	variable *var, const char *name, const char *value)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	gtk_entry_set_text(GTK_ENTRY(var->Widget), value);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Refresh                                                             *
 ***********************************************************************/

void widget_entry_refresh(variable *var)
{
	GList            *element;
	gchar            *act;
	gint              initialised = FALSE;
	gboolean          completion_source;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(var->Widget), "_initialised"));

#if GTK_CHECK_VERSION(2,4,0)
	widget_entry_completion_clear(var->Widget);
#endif

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		completion_source = FALSE;
#if GTK_CHECK_VERSION(2,4,0)
		if (gtk_entry_get_completion(GTK_ENTRY(var->Widget)) != NULL)
			completion_source = widget_entry_input_is_completion(
				&element, var);
#endif
		if (input_is_shell_command(act))
			widget_entry_input_by_command(var, act + 8, completion_source);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		else if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, act + 5);
			}
			widget_entry_input_by_file(var, act + 5, completion_source);
		} else if (completion_source)
			widget_entry_input_by_command(var, act, TRUE);
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_entry_input_by_items(var);

	/* Initialise these only once at start-up */
	if (!initialised) {
		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
			fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT)) {
			/* Thunor: This is all original code moved across when refactoring */
			gtk_entry_set_text(GTK_ENTRY(var->Widget), attributeset_get_first(
				&element, var->Attributes, ATTR_DEFAULT));
		}
		if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT) &&
			attributeset_is_avail(var->Attributes, ATTR_WIDTH)) {
			/* Thunor: This is all original code moved across when refactoring */
			gtk_widget_set_size_request(var->Widget,
				widget_parse_size_request(attributeset_get_first(&element,
					var->Attributes, ATTR_WIDTH), "entry width"),
				widget_parse_size_request(attributeset_get_first(&element,
					var->Attributes, ATTR_HEIGHT), "entry height"));
		}
		if ((attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled")) ||	/* Deprecated */
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
			gtk_widget_set_sensitive(var->Widget, FALSE);
		if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "password")) {
			/* Thunor: This is all original code moved across when refactoring */
			gtk_entry_set_visibility(GTK_ENTRY(var->Widget), FALSE);
		}

		/* Connect signals */
		/* Thunor: This is all original code moved across when refactoring */
		g_signal_connect(G_OBJECT(var->Widget), "changed", 
			G_CALLBACK(on_any_widget_changed_event), (gpointer)var->Attributes);
		g_signal_connect(G_OBJECT(var->Widget), "activate",
			G_CALLBACK(on_any_widget_activate_event), (gpointer)var->Attributes);
#if GTK_CHECK_VERSION(2,16,0)
		/* Despite what the GTK+ 2 Reference Manual says, I found
		 * these to be activatable by default. They will actually
		 * be prefixed with either primary- or secondary- for use
		 * within action directives */
		g_signal_connect(G_OBJECT(var->Widget), "icon-press",
			G_CALLBACK(on_any_widget_icon_press_event), (gpointer)var->Attributes);
		g_signal_connect(G_OBJECT(var->Widget), "icon-release",
			G_CALLBACK(on_any_widget_icon_release_event), (gpointer)var->Attributes);
#endif

	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_entry_removeselected(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	gtk_entry_set_text(GTK_ENTRY(var->Widget), "");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_entry_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	gchar            *act;
	gchar            *filename = NULL;
	const gchar      *text;

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

			/* Thunor: This is all original code moved across when refactoring */
			text = gtk_entry_get_text(GTK_ENTRY(var->Widget));
			fprintf(outfile, "%s", text);

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

static void widget_entry_input_by_command(
	variable *var, char *command, gboolean completion_source)
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
		if (completion_source) {
#if GTK_CHECK_VERSION(2,4,0)
			while ((line = widget_read_line(infile)) != NULL) {
				widget_entry_completion_append(var->Widget, line);
				g_free(line);
			}
#endif
		} else if ((line = widget_read_line(infile)) != NULL) {
			/* Thunor: This is all original code moved across when refactoring */
			gtk_entry_set_text(GTK_ENTRY(var->Widget), (const gchar*)line);
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

static void widget_entry_input_by_file(
	variable *var, char *filename, gboolean completion_source)
{
	FILE             *infile;
	gchar            *line;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (infile = fopen(filename, "r")) {
		if (completion_source) {
#if GTK_CHECK_VERSION(2,4,0)
			while ((line = widget_read_line(infile)) != NULL) {
				widget_entry_completion_append(var->Widget, line);
				g_free(line);
			}
#endif
		} else if ((line = widget_read_line(infile)) != NULL) {
			/* Thunor: This is all original code moved across when refactoring */
			gtk_entry_set_text(GTK_ENTRY(var->Widget), line);
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

static void widget_entry_input_by_items(variable *var)
{
	GList            *element;
	gchar            *text;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if GTK_CHECK_VERSION(2,4,0)
	if (gtk_entry_get_completion(GTK_ENTRY(var->Widget)) != NULL) {
		text = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
		while (text != NULL) {
			widget_entry_completion_append(var->Widget, text);
			text = attributeset_get_next(&element,
				var->Attributes, ATTR_ITEM);
		}
	} else
#endif
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
