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

enum {
	ENTRY_COMPLETION_VALUE,
	ENTRY_COMPLETION_TEXT,
	ENTRY_COMPLETION_ENTRY_TEXT,
	ENTRY_COMPLETION_PIXBUF,
	ENTRY_COMPLETION_MARKUP,
	ENTRY_COMPLETION_SENSITIVE,
	ENTRY_COMPLETION_FOREGROUND,
	ENTRY_COMPLETION_BACKGROUND,
	ENTRY_COMPLETION_FONT,
	ENTRY_COMPLETION_DESCRIPTION,
	ENTRY_COMPLETION_COLUMNS
};

static gboolean widget_entry_completion_is_rich(GtkWidget *widget)
{
	return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
		"gtkdialog-entry-completion-rich"));
}

static void widget_entry_completion_cell_data(
	GtkCellLayout *layout, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gchar *background;
	gchar *font;
	gchar *foreground;
	gchar *label;
	gboolean markup;
	gboolean sensitive;

	(void)layout;
	(void)data;
	gtk_tree_model_get(model, iter,
		ENTRY_COMPLETION_TEXT, &label,
		ENTRY_COMPLETION_MARKUP, &markup,
		ENTRY_COMPLETION_SENSITIVE, &sensitive,
		ENTRY_COMPLETION_FOREGROUND, &foreground,
		ENTRY_COMPLETION_BACKGROUND, &background,
		ENTRY_COMPLETION_FONT, &font,
		-1);
	if (markup)
		g_object_set(G_OBJECT(renderer), "markup", label, NULL);
	else
		g_object_set(G_OBJECT(renderer), "attributes", NULL,
			"text", label, NULL);
	g_object_set(G_OBJECT(renderer),
		"sensitive", sensitive,
		"foreground", foreground,
		"foreground-set", foreground != NULL && foreground[0] != '\0',
		"background", background,
		"background-set", background != NULL && background[0] != '\0',
		"font", font,
		NULL);
	g_free(background);
	g_free(font);
	g_free(foreground);
	g_free(label);
}

static void widget_entry_completion_description_cell_data(
	GtkCellLayout *layout, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gchar *description;
	gboolean sensitive;

	(void)layout;
	(void)data;
	gtk_tree_model_get(model, iter,
		ENTRY_COMPLETION_DESCRIPTION, &description,
		ENTRY_COMPLETION_SENSITIVE, &sensitive,
		-1);
	g_object_set(G_OBJECT(renderer),
		"text", description != NULL ? description : "",
		"sensitive", sensitive,
		NULL);
	g_free(description);
}

static void widget_entry_completion_changed(GtkEditable *editable,
	gpointer data)
{
	GObject *object = G_OBJECT(editable);

	(void)data;
	if (g_object_get_data(object, "gtkdialog-entry-completion-setting") == NULL)
		g_object_set_data_full(object, "gtkdialog-entry-completion-value",
			NULL, NULL);
}

static gboolean widget_entry_completion_match_selected(
	GtkEntryCompletion *completion, GtkTreeModel *model,
	GtkTreeIter *iter, gpointer data)
{
	GtkEntry *entry = GTK_ENTRY(data);
	gchar *entry_text;
	gchar *value;
	gboolean sensitive;

	(void)completion;
	gtk_tree_model_get(model, iter,
		ENTRY_COMPLETION_VALUE, &value,
		ENTRY_COMPLETION_ENTRY_TEXT, &entry_text,
		ENTRY_COMPLETION_SENSITIVE, &sensitive,
		-1);
	if (sensitive) {
		g_object_set_data_full(G_OBJECT(entry),
			"gtkdialog-entry-completion-value", value, g_free);
		value = NULL;
		g_object_set_data(G_OBJECT(entry),
			"gtkdialog-entry-completion-setting", GINT_TO_POINTER(TRUE));
		gtk_entry_set_text(entry, entry_text != NULL ? entry_text : "");
		gtk_editable_set_position(GTK_EDITABLE(entry), -1);
		g_object_set_data(G_OBJECT(entry),
			"gtkdialog-entry-completion-setting", NULL);
	}
	g_free(entry_text);
	g_free(value);
	return TRUE;
}

static void widget_entry_completion_append(GtkWidget *widget,
	const gchar *text, const gchar *value, const gchar *stock_name,
	const gchar *icon_name, const gchar *image_name, gboolean markup,
	gboolean sensitive, const gchar *foreground, const gchar *background,
	const gchar *font, const gchar *description)
{
	GtkEntryCompletion *completion;
	GdkPixbuf *pixbuf = NULL;
	GtkListStore *store;
	GtkTreeIter iter;
	gchar *entry_text = NULL;
	gint icon_size;

	completion = gtk_entry_get_completion(GTK_ENTRY(widget));
	store = GTK_LIST_STORE(gtk_entry_completion_get_model(completion));
	if (widget_entry_completion_is_rich(widget)) {
		icon_size = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
			"gtkdialog-entry-completion-icon-size"));
		pixbuf = widget_load_pixbuf(widget, stock_name, icon_name, image_name,
			icon_size);
	}
	if (markup && !pango_parse_markup(text, -1, 0, NULL, &entry_text,
		NULL, NULL))
		entry_text = NULL;
	if (entry_text == NULL)
		entry_text = g_strdup(text);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
		ENTRY_COMPLETION_VALUE, value != NULL ? value : entry_text,
		ENTRY_COMPLETION_TEXT, text,
		ENTRY_COMPLETION_ENTRY_TEXT, entry_text,
		ENTRY_COMPLETION_PIXBUF, pixbuf,
		ENTRY_COMPLETION_MARKUP, markup,
		ENTRY_COMPLETION_SENSITIVE, sensitive,
		ENTRY_COMPLETION_FOREGROUND, foreground,
		ENTRY_COMPLETION_BACKGROUND, background,
		ENTRY_COMPLETION_FONT, font,
		ENTRY_COMPLETION_DESCRIPTION, description,
		-1);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
	g_free(entry_text);
}

static void widget_entry_completion_append_plain(GtkWidget *widget,
	const gchar *text)
{
	widget_entry_completion_append(widget, text, text, NULL, NULL, NULL,
		FALSE, TRUE, NULL, NULL, NULL, NULL);
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
	g_object_set_data_full(G_OBJECT(widget),
		"gtkdialog-entry-completion-value", NULL, NULL);
}

static gboolean widget_entry_completion_select_value(GtkWidget *widget,
	const gchar *value)
{
	GtkEntryCompletion *completion;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *entry_text;
	gchar *row_value;
	gboolean sensitive;
	gboolean valid;

	completion = gtk_entry_get_completion(GTK_ENTRY(widget));
	if (completion == NULL || !widget_entry_completion_is_rich(widget))
		return FALSE;
	model = gtk_entry_completion_get_model(completion);
	valid = gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		gtk_tree_model_get(model, &iter,
			ENTRY_COMPLETION_VALUE, &row_value,
			ENTRY_COMPLETION_ENTRY_TEXT, &entry_text,
			ENTRY_COMPLETION_SENSITIVE, &sensitive,
			-1);
		if (sensitive && row_value != NULL && strcmp(row_value, value) == 0) {
			g_object_set_data_full(G_OBJECT(widget),
				"gtkdialog-entry-completion-value", row_value, g_free);
			row_value = NULL;
			g_object_set_data(G_OBJECT(widget),
				"gtkdialog-entry-completion-setting", GINT_TO_POINTER(TRUE));
			gtk_entry_set_text(GTK_ENTRY(widget),
				entry_text != NULL ? entry_text : "");
			gtk_editable_set_position(GTK_EDITABLE(widget), -1);
			g_object_set_data(G_OBJECT(widget),
				"gtkdialog-entry-completion-setting", NULL);
			g_free(entry_text);
			return TRUE;
		}
		g_free(entry_text);
		g_free(row_value);
		valid = gtk_tree_model_iter_next(model, &iter);
	}
	return FALSE;
}

/* Notes: */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_entry_clear(variable *var)
{

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
	gboolean          completion_case_sensitive = FALSE;
	gboolean          completion_rich = FALSE;
	gint              completion_icon_size = 16;
	gint              minimum_key_length = 1;
	WidgetCompletionMatch completion_match = WIDGET_COMPLETION_MATCH_PREFIX;

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
		value = get_tag_attribute(attr, "completion-model");
		if (value != NULL) {
			if (strcasecmp(value, "rich") == 0)
				completion_rich = TRUE;
			else if (strcasecmp(value, "text") != 0)
				gtkdialog_warning("Invalid entry completion-model '%s'; "
					"using text.", value);
		}
		value = get_tag_attribute(attr, "completion-icon-size");
		if (value != NULL)
			completion_icon_size = widget_parse_bounded_integer(value, 1,
				G_MAXINT, 16, "entry completion-icon-size");
		completion_match = widget_parse_completion_match(
			get_tag_attribute(attr, "completion-match"),
			"entry completion-match");
		completion_case_sensitive = widget_parse_completion_case_sensitive(
			get_tag_attribute(attr, "completion-case-sensitive"),
			"entry completion-case-sensitive");
		kill_tag_attribute(attr, "completion");
		kill_tag_attribute(attr, "completion-minimum-key-length");
		kill_tag_attribute(attr, "completion-model");
		kill_tag_attribute(attr, "completion-icon-size");
		kill_tag_attribute(attr, "completion-match");
		kill_tag_attribute(attr, "completion-case-sensitive");
	}
	if (completion_enabled) {
		GtkCellRenderer *renderer;
		GtkEntryCompletion *completion;
		GtkListStore *store;

		store = gtk_list_store_new(ENTRY_COMPLETION_COLUMNS,
			G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF,
			G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING,
			G_TYPE_STRING, G_TYPE_STRING);
		completion = gtk_entry_completion_new();
		gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));
		g_object_unref(store);
		gtk_entry_completion_set_text_column(completion,
			completion_rich ? ENTRY_COMPLETION_ENTRY_TEXT :
			ENTRY_COMPLETION_TEXT);
		widget_entry_completion_set_matching(completion,
			completion_rich ? ENTRY_COMPLETION_ENTRY_TEXT :
			ENTRY_COMPLETION_TEXT, completion_match,
			completion_case_sensitive);
		gtk_entry_completion_set_minimum_key_length(completion,
			minimum_key_length);
		if (completion_rich) {
			gtk_cell_layout_clear(GTK_CELL_LAYOUT(completion));
			renderer = gtk_cell_renderer_pixbuf_new();
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(completion), renderer,
				FALSE);
			gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(completion), renderer,
				"pixbuf", ENTRY_COMPLETION_PIXBUF);
			gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(completion), renderer,
				"sensitive", ENTRY_COMPLETION_SENSITIVE);
			renderer = gtk_cell_renderer_text_new();
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(completion), renderer,
				TRUE);
			gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(completion),
				renderer, widget_entry_completion_cell_data, NULL, NULL);
			renderer = gtk_cell_renderer_text_new();
			g_object_set(G_OBJECT(renderer), "foreground", "#666666", NULL);
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(completion), renderer,
				FALSE);
			gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(completion),
				renderer, widget_entry_completion_description_cell_data,
				NULL, NULL);
			g_signal_connect(G_OBJECT(completion), "match-selected",
				G_CALLBACK(widget_entry_completion_match_selected), widget);
			g_signal_connect(G_OBJECT(widget), "changed",
				G_CALLBACK(widget_entry_completion_changed), NULL);
			g_object_set_data(G_OBJECT(widget),
				"gtkdialog-entry-completion-rich", GINT_TO_POINTER(TRUE));
			g_object_set_data(G_OBJECT(widget),
				"gtkdialog-entry-completion-icon-size",
				GINT_TO_POINTER(completion_icon_size));
		}
		gtk_entry_set_completion(GTK_ENTRY(widget), completion);
		g_object_unref(completion);
	}

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
	const gchar      *completion_value;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	completion_value = g_object_get_data(G_OBJECT(widget),
		"gtkdialog-entry-completion-value");
	if (completion_value != NULL) {
		string = g_strdup(completion_value);
	} else {
		/* Thunor: This is all original code moved across when refactoring */
		text = (gchar*)gtk_entry_get_text(GTK_ENTRY(widget));
		string = g_strdup(text);
	}

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

	widget_entry_completion_clear(var->Widget);

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		completion_source = FALSE;
		if (gtk_entry_get_completion(GTK_ENTRY(var->Widget)) != NULL)
			completion_source = widget_entry_input_is_completion(
				&element, var);
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
			act = attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT);
			if (!widget_entry_completion_select_value(var->Widget, act))
				gtk_entry_set_text(GTK_ENTRY(var->Widget), act);
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
		/* Despite what the GTK+ 2 Reference Manual says, I found
		 * these to be activatable by default. They will actually
		 * be prefixed with either primary- or secondary- for use
		 * within action directives */
		g_signal_connect(G_OBJECT(var->Widget), "icon-press",
			G_CALLBACK(on_any_widget_icon_press_event), (gpointer)var->Attributes);
		g_signal_connect(G_OBJECT(var->Widget), "icon-release",
			G_CALLBACK(on_any_widget_icon_release_event), (gpointer)var->Attributes);

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
			text = widget_entry_envvar_construct(var->Widget);
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
	if ((infile = widget_opencommand(command))) {
		if (completion_source) {
			while ((line = widget_read_line(infile)) != NULL) {
				widget_entry_completion_append_plain(var->Widget, line);
				g_free(line);
			}
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

	if ((infile = fopen(filename, "r"))) {
		if (completion_source) {
			while ((line = widget_read_line(infile)) != NULL) {
				widget_entry_completion_append_plain(var->Widget, line);
				g_free(line);
			}
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

	if (gtk_entry_get_completion(GTK_ENTRY(var->Widget)) != NULL) {
		text = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
		while (text != NULL) {
			if (widget_entry_completion_is_rich(var->Widget)) {
				gchar *background;
				gchar *description;
				gchar *font;
				gchar *foreground;
				gchar *icon_name;
				gchar *image_name;
				gchar *markup_text;
				gchar *sensitive_text;
				gchar *stock_name;
				gchar *value;
				gboolean markup = FALSE;
				gboolean sensitive = TRUE;

				value = attributeset_get_this_tagattr(&element, var->Attributes,
					ATTR_ITEM, "value");
				stock_name = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "stock");
				if (stock_name == NULL)
					stock_name = attributeset_get_this_tagattr(&element,
						var->Attributes, ATTR_ITEM, "stock-id");
				icon_name = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "icon-name");
				if (icon_name == NULL)
					icon_name = attributeset_get_this_tagattr(&element,
						var->Attributes, ATTR_ITEM, "icon");
				image_name = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "image-file");
				if (image_name == NULL)
					image_name = attributeset_get_this_tagattr(&element,
						var->Attributes, ATTR_ITEM, "image-name");
				markup_text = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "markup");
				if (markup_text != NULL &&
					!widget_parse_boolean(markup_text, &markup))
					gtkdialog_warning("Invalid entry completion item markup "
						"value '%s'; using false.", markup_text);
				sensitive_text = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "sensitive");
				if (sensitive_text != NULL &&
					!widget_parse_boolean(sensitive_text, &sensitive)) {
					gtkdialog_warning("Invalid entry completion item sensitive "
						"value '%s'; using true.", sensitive_text);
					sensitive = TRUE;
				}
				foreground = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "foreground");
				background = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "background");
				font = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "font");
				description = attributeset_get_this_tagattr(&element,
					var->Attributes, ATTR_ITEM, "description");
				widget_entry_completion_append(var->Widget, text, value,
					stock_name, icon_name, image_name, markup, sensitive,
					foreground, background, font, description);
			} else {
				widget_entry_completion_append_plain(var->Widget, text);
			}
			text = attributeset_get_next(&element,
				var->Attributes, ATTR_ITEM);
		}
	} else
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
