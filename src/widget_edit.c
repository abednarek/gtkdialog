/*
 * widget_edit.c: 
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
#include "scrolling.h"
#if HAVE_GTKSPELL
#include <gtkspell/gtkspell.h>
#endif
#if HAVE_GTKSOURCEVIEW
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcelanguagemanager.h>
#include <gtksourceview/gtksourcestyleschememanager.h>
#endif

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS

/* Local function prototypes, located at file bottom */
static void widget_edit_input_by_command(variable *var, char *command);
static void widget_edit_input_by_file(variable *var, char *filename);
static void widget_edit_input_by_items(variable *var);
static void widget_edit_setup_spell_check(GtkWidget *widget, tag_attr *attr);
static GtkWidget *widget_edit_create_text_view(tag_attr *attr);

/* Notes: */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_edit_clear(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	gtk_text_buffer_set_text(gtk_text_view_get_buffer(
		GTK_TEXT_VIEW(var->Widget)), "", 0);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/

static void widget_edit_setup_spell_check(GtkWidget *widget, tag_attr *attr)
{
	gchar *enabled;
	gchar *language;

	if (attr == NULL)
		return;

	enabled = get_tag_attribute(attr, "spell-check");
	language = get_tag_attribute(attr, "spell-language");
	if (widget_attribute_is_true(enabled)) {
#if HAVE_GTKSPELL
		GError *error = NULL;

		if (gtkspell_new_attach(GTK_TEXT_VIEW(widget),
			language != NULL && language[0] != '\0' ? language : NULL,
			&error) == NULL) {
			gtkdialog_warning("Spell checking could not be enabled%s%s: %s.",
				language != NULL && language[0] != '\0' ? " for " : "",
				language != NULL && language[0] != '\0' ? language : "",
				error != NULL ? error->message : "unknown GtkSpell error");
			g_clear_error(&error);
		}
#else
		gtkdialog_warning("Spell checking was requested, but this build "
			"does not include GtkSpell 2 support.");
#endif
	}

	/* These are gtkdialog extensions, not GtkTextView properties. */
	kill_tag_attribute(attr, "spell-check");
	kill_tag_attribute(attr, "spell-language");
}

static void widget_edit_kill_source_attributes(tag_attr *attr)
{
	static const gchar *names[] = {
		"sourceview",
		"language",
		"style-scheme",
		"highlight-syntax",
		"highlight-matching-brackets",
		"max-undo-levels"
	};
	guint index;

	for (index = 0; index < G_N_ELEMENTS(names); ++index)
		kill_tag_attribute(attr, names[index]);
}

static GtkWidget *widget_edit_create_text_view(tag_attr *attr)
{
	GtkWidget *widget;
	gchar *enabled;

	if (attr == NULL)
		return gtk_text_view_new();

	enabled = get_tag_attribute(attr, "sourceview");
	if (!widget_attribute_is_true(enabled)) {
		kill_tag_attribute(attr, "sourceview");
		return gtk_text_view_new();
	}

#if HAVE_GTKSOURCEVIEW
	{
		GtkSourceBuffer *buffer;
		GtkSourceLanguage *language;
		GtkSourceLanguageManager *language_manager;
		GtkSourceStyleScheme *scheme;
		GtkSourceStyleSchemeManager *scheme_manager;
		gchar *end;
		gchar *value;
		gint64 undo_levels;

		buffer = gtk_source_buffer_new(NULL);
		widget = gtk_source_view_new_with_buffer(buffer);

		value = get_tag_attribute(attr, "language");
		if (value != NULL && value[0] != '\0') {
			language_manager = gtk_source_language_manager_get_default();
			language = gtk_source_language_manager_get_language(
				language_manager, value);
			if (language != NULL)
				gtk_source_buffer_set_language(buffer, language);
			else
				gtkdialog_warning("GtkSourceView language '%s' was not found.",
					value);
		}

		value = get_tag_attribute(attr, "style-scheme");
		if (value != NULL && value[0] != '\0') {
			scheme_manager = gtk_source_style_scheme_manager_get_default();
			scheme = gtk_source_style_scheme_manager_get_scheme(
				scheme_manager, value);
			if (scheme != NULL)
				gtk_source_buffer_set_style_scheme(buffer, scheme);
			else
				gtkdialog_warning("GtkSourceView style scheme '%s' was not found.",
					value);
		}

		value = get_tag_attribute(attr, "highlight-syntax");
		if (value != NULL)
			gtk_source_buffer_set_highlight_syntax(buffer,
				widget_attribute_is_true(value));
		value = get_tag_attribute(attr, "highlight-matching-brackets");
		if (value != NULL)
			gtk_source_buffer_set_highlight_matching_brackets(buffer,
				widget_attribute_is_true(value));
		value = get_tag_attribute(attr, "max-undo-levels");
		if (value != NULL) {
			errno = 0;
			undo_levels = g_ascii_strtoll(value, &end, 10);
			while (g_ascii_isspace(*end))
				++end;
			if (errno == 0 && end != value && *end == '\0' &&
				undo_levels >= -1 && undo_levels <= G_MAXINT)
				gtk_source_buffer_set_max_undo_levels(buffer,
					(gint)undo_levels);
			else
				gtkdialog_warning("Invalid GtkSourceView max-undo-levels value "
					"'%s'.", value);
		}

		g_object_unref(buffer);
	}
#else
	widget = gtk_text_view_new();
	gtkdialog_warning("A source editor was requested, but this build does not "
		"include GtkSourceView 2 support.");
#endif

	widget_edit_kill_source_attributes(attr);
	return widget;
}

GtkWidget *widget_edit_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget        *widget;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
#if GTK_CHECK_VERSION(2, 4, 0)

	widget = widget_edit_create_text_view(attr);
	widget_edit_setup_spell_check(widget, attr);

#else

	yyerror_simple("Editor widget is not supported by"
		"this version of GTK+, you need at"
		"least GTK+ 2.4.0\n");

#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_edit_envvar_construct(GtkWidget *widget)
{
	GtkTextBuffer    *text_buffer;
	GtkTextIter       start, end;		
	gchar            *string;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
	gtk_text_buffer_get_start_iter(text_buffer, &start);
	gtk_text_buffer_get_end_iter(text_buffer, &end);
	/* This function returns an allocated string so no need to strdup */
	string = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_edit_fileselect(
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

void widget_edit_refresh(variable *var)
{
	GList            *element;
	GtkTextBuffer    *text_buffer;
	gchar            *act;
	gchar            *value;
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
			widget_edit_input_by_command(var, act + 8);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, act + 5);
			}
			widget_edit_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_edit_input_by_items(var);

	/* Initialise these only once at start-up */
	if (!initialised) {
		if (var->widget_tag_attr &&
			(value = get_tag_attribute(var->widget_tag_attr,
				"scroll-to-bottom")) &&
			widget_attribute_is_true(value))
			scrolling_setup_bottom_gravity(var->Widget);

		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
			fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT)) {
			/* Thunor: This is all original code moved across when refactoring */
			text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(var->Widget));
			gtk_text_buffer_set_text(text_buffer, attributeset_get_first(
				&element, var->Attributes, ATTR_DEFAULT), -1);
		}
		if ((attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled")) ||	/* Deprecated */
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
			gtk_widget_set_sensitive(var->Widget, FALSE);

		/* Connect signals */

	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_edit_removeselected(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: This is all original code moved across when refactoring */
	gtk_text_buffer_delete_selection(gtk_text_view_get_buffer(
		GTK_TEXT_VIEW(var->Widget)), FALSE, TRUE);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_edit_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	GtkTextBuffer    *buffer;
	GtkTextIter       start, end;
	gchar            *act;
	gchar            *filename = NULL;
	gchar            *text;

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
			buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(var->Widget));
			gtk_text_buffer_get_start_iter(buffer, &start);
			gtk_text_buffer_get_end_iter(buffer, &end);
			text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
			fprintf(outfile, "%s", text);
			g_free(text);

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

static void widget_edit_input_by_command(variable *var, char *command)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): <input> not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by File                                                       *
 ***********************************************************************/

static void widget_edit_input_by_file(variable *var, char *filename)
{
	GtkTextBuffer    *buffer;
	GError           *error = NULL;
	gchar            *filebuffer = NULL;
	gsize             length = 0;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (!g_file_get_contents(filename, &filebuffer, &length, &error)) {
		fprintf(stderr, "%s(): Couldn't read '%s': %s.\n", __func__,
			filename, error->message);
		g_error_free(error);
		return;
	}

	if (length > G_MAXINT) {
		fprintf(stderr, "%s(): File '%s' is too large for a GtkTextBuffer.\n",
			__func__, filename);
		g_free(filebuffer);
		return;
	}

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(var->Widget));
	gtk_text_buffer_set_text(buffer, filebuffer, (gint)length);
	g_free(filebuffer);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Items                                                      *
 ***********************************************************************/

static void widget_edit_input_by_items(variable *var)
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
