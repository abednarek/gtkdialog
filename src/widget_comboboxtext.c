/*
 * widget_comboboxtext.c: 
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
#if HAVE_JSON_GLIB
#include <json-glib/json-glib.h>
#endif

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS
#define COMBOBOX_TREE_JSON_MAX_DEPTH 256

/* Local function prototypes, located at file bottom */
static void widget_comboboxtext_input_by_command(variable *var, char *command);
static void widget_comboboxtext_input_by_file(variable *var, char *filename);
static void widget_comboboxtext_input_by_items(variable *var);
static void widget_comboboxtext_input_json(variable *var,
	const gchar *source, gboolean is_command);
static void widget_comboboxtext_save_json(variable *var,
	const gchar *filename);
static gchar *widget_comboboxtext_get_active_text(GtkWidget *widget);

enum {
	COMBOBOX_RICH_VALUE,
	COMBOBOX_RICH_LABEL,
	COMBOBOX_RICH_PIXBUF,
	COMBOBOX_RICH_MARKUP,
	COMBOBOX_RICH_SENSITIVE,
	COMBOBOX_RICH_FOREGROUND,
	COMBOBOX_RICH_BACKGROUND,
	COMBOBOX_RICH_FONT,
	COMBOBOX_RICH_ENTRY_TEXT,
	COMBOBOX_RICH_STOCK_ID,
	COMBOBOX_RICH_ICON_NAME,
	COMBOBOX_RICH_IMAGE_NAME,
	COMBOBOX_RICH_COLUMNS
};

static gboolean widget_comboboxtext_is_rich(GtkWidget *widget)
{
	return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
		"gtkdialog-comboboxtext-rich"));
}

static gboolean widget_comboboxtext_is_tree(GtkWidget *widget)
{
	return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
		"gtkdialog-comboboxtext-tree"));
}

static gboolean widget_comboboxtext_completion_match_selected(
	GtkEntryCompletion *completion, GtkTreeModel *model,
	GtkTreeIter *iter, gpointer data)
{
	GtkComboBox *combobox = GTK_COMBO_BOX(data);
	gboolean sensitive = TRUE;

	(void)completion;
	if (widget_comboboxtext_is_rich(GTK_WIDGET(combobox)))
		gtk_tree_model_get(model, iter, COMBOBOX_RICH_SENSITIVE,
			&sensitive, -1);
	if (sensitive)
		gtk_combo_box_set_active_iter(combobox, iter);
	return TRUE;
}

static void widget_comboboxtext_rich_cell_data(
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
		COMBOBOX_RICH_LABEL, &label,
		COMBOBOX_RICH_MARKUP, &markup,
		COMBOBOX_RICH_SENSITIVE, &sensitive,
		COMBOBOX_RICH_FOREGROUND, &foreground,
		COMBOBOX_RICH_BACKGROUND, &background,
		COMBOBOX_RICH_FONT, &font,
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

static void widget_comboboxtext_append_rich_at(GtkWidget *widget,
	GtkTreeIter *parent, GtkTreeIter *result,
	const gchar *label, const gchar *value, const gchar *stock_name,
	const gchar *icon_name, const gchar *image_name, gboolean markup,
	gboolean sensitive, const gchar *foreground, const gchar *background,
	const gchar *font, gint position)
{
	GdkPixbuf *pixbuf;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *entry_text = NULL;
	gint icon_size;

	model = gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
	icon_size = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
		"gtkdialog-comboboxtext-icon-size"));
	pixbuf = widget_load_pixbuf(widget, stock_name, icon_name, image_name,
		icon_size);
	if (markup && !pango_parse_markup(label, -1, 0, NULL, &entry_text,
		NULL, NULL))
		entry_text = NULL;
	if (entry_text == NULL)
		entry_text = g_strdup(label);
	if (widget_comboboxtext_is_tree(widget)) {
		if (position < 0)
			gtk_tree_store_append(GTK_TREE_STORE(model), &iter, parent);
		else
			gtk_tree_store_insert(GTK_TREE_STORE(model), &iter, parent,
				position);
		gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
			COMBOBOX_RICH_VALUE, value != NULL ? value : label,
			COMBOBOX_RICH_LABEL, label,
			COMBOBOX_RICH_PIXBUF, pixbuf,
			COMBOBOX_RICH_MARKUP, markup,
			COMBOBOX_RICH_SENSITIVE, sensitive,
			COMBOBOX_RICH_FOREGROUND, foreground,
			COMBOBOX_RICH_BACKGROUND, background,
			COMBOBOX_RICH_FONT, font,
			COMBOBOX_RICH_ENTRY_TEXT, entry_text,
			COMBOBOX_RICH_STOCK_ID, stock_name,
			COMBOBOX_RICH_ICON_NAME, icon_name,
			COMBOBOX_RICH_IMAGE_NAME, image_name,
			-1);
	} else {
		if (position < 0)
			gtk_list_store_append(GTK_LIST_STORE(model), &iter);
		else
			gtk_list_store_insert(GTK_LIST_STORE(model), &iter, position);
		gtk_list_store_set(GTK_LIST_STORE(model), &iter,
			COMBOBOX_RICH_VALUE, value != NULL ? value : label,
			COMBOBOX_RICH_LABEL, label,
			COMBOBOX_RICH_PIXBUF, pixbuf,
			COMBOBOX_RICH_MARKUP, markup,
			COMBOBOX_RICH_SENSITIVE, sensitive,
			COMBOBOX_RICH_FOREGROUND, foreground,
			COMBOBOX_RICH_BACKGROUND, background,
			COMBOBOX_RICH_FONT, font,
			COMBOBOX_RICH_ENTRY_TEXT, entry_text,
			COMBOBOX_RICH_STOCK_ID, stock_name,
			COMBOBOX_RICH_ICON_NAME, icon_name,
			COMBOBOX_RICH_IMAGE_NAME, image_name,
			-1);
	}
	if (result != NULL)
		*result = iter;
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
	g_free(entry_text);
}

static void widget_comboboxtext_append_rich(GtkWidget *widget,
	const gchar *label, const gchar *value, const gchar *stock_name,
	const gchar *icon_name, const gchar *image_name, gboolean markup,
	gboolean sensitive, const gchar *foreground, const gchar *background,
	const gchar *font, gint position)
{
	widget_comboboxtext_append_rich_at(widget, NULL, NULL, label, value,
		stock_name, icon_name, image_name, markup, sensitive, foreground,
		background, font, position);
}

static void widget_comboboxtext_append_plain_rich(
	GtkWidget *widget, const gchar *text, gint position)
{
	widget_comboboxtext_append_rich(widget, text, text, NULL, NULL, NULL,
		FALSE, TRUE, NULL, NULL, NULL, position);
}

static void widget_comboboxtext_remove_all(GtkWidget *widget)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gint rowcount;

	model = gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
	if (widget_comboboxtext_is_rich(widget)) {
		if (widget_comboboxtext_is_tree(widget))
			gtk_tree_store_clear(GTK_TREE_STORE(model));
		else
			gtk_list_store_clear(GTK_LIST_STORE(model));
		return;
	}
	if (!gtk_tree_model_get_iter_first(model, &iter))
		return;
	rowcount = 1;
	while (gtk_tree_model_iter_next(model, &iter))
		rowcount++;
	while (rowcount--)
		gtk_combo_box_remove_text(GTK_COMBO_BOX(widget), rowcount);
}

/* Notes:
 * The comboboxentry widget is near identical to the comboboxtext widget
 * so there's absolutely no point in maintaining another pair of files */

static gchar *widget_comboboxtext_get_active_text(GtkWidget *widget)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *text;

	if (widget_comboboxtext_is_rich(widget)) {
		if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(widget), &iter)) {
			if (GTK_IS_COMBO_BOX_ENTRY(widget))
				return g_strdup(gtk_entry_get_text(GTK_ENTRY(
					gtk_bin_get_child(GTK_BIN(widget)))));
			return g_strdup("");
		}
		model = gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
		gtk_tree_model_get(model, &iter, COMBOBOX_RICH_VALUE, &text, -1);
		return text != NULL ? text : g_strdup("");
	}
	text = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));

	return text ? text : g_strdup("");
}

static gboolean widget_comboboxtext_find_value(GtkTreeModel *model,
	GtkTreeIter *parent, const gchar *value, GtkTreeIter *result)
{
	GtkTreeIter iter;
	gchar *row_value;
	gboolean sensitive;
	gboolean valid;

	valid = parent != NULL ? gtk_tree_model_iter_children(model, &iter, parent) :
		gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		if (gtk_tree_model_iter_has_child(model, &iter)) {
			if (widget_comboboxtext_find_value(model, &iter, value, result))
				return TRUE;
		} else {
			gtk_tree_model_get(model, &iter,
				COMBOBOX_RICH_VALUE, &row_value,
				COMBOBOX_RICH_SENSITIVE, &sensitive, -1);
			if (sensitive && row_value != NULL && strcmp(row_value, value) == 0) {
				*result = iter;
				g_free(row_value);
				return TRUE;
			}
			g_free(row_value);
		}
		valid = gtk_tree_model_iter_next(model, &iter);
	}
	return FALSE;
}

static gboolean widget_comboboxtext_find_first_selectable(
	GtkTreeModel *model, GtkTreeIter *parent, GtkTreeIter *result)
{
	GtkTreeIter iter;
	gboolean sensitive;
	gboolean valid;

	valid = parent != NULL ? gtk_tree_model_iter_children(model, &iter, parent) :
		gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		if (gtk_tree_model_iter_has_child(model, &iter)) {
			if (widget_comboboxtext_find_first_selectable(model, &iter, result))
				return TRUE;
		} else {
			gtk_tree_model_get(model, &iter, COMBOBOX_RICH_SENSITIVE,
				&sensitive, -1);
			if (sensitive) {
				*result = iter;
				return TRUE;
			}
		}
		valid = gtk_tree_model_iter_next(model, &iter);
	}
	return FALSE;
}

static void widget_comboboxtext_select_first(GtkWidget *widget)
{
	GtkTreeIter iter;
	GtkTreeModel *model;

	model = gtk_combo_box_get_model(GTK_COMBO_BOX(widget));
	if (widget_comboboxtext_find_first_selectable(model, NULL, &iter))
		gtk_combo_box_set_active_iter(GTK_COMBO_BOX(widget), &iter);
	else
		gtk_combo_box_set_active(GTK_COMBO_BOX(widget), -1);
}

static void widget_comboboxtext_save_tree_values(GtkTreeModel *model,
	GtkTreeIter *parent, FILE *outfile, gboolean *first)
{
	GtkTreeIter iter;
	gchar *value;
	gboolean valid;

	valid = parent != NULL ? gtk_tree_model_iter_children(model, &iter, parent) :
		gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		if (gtk_tree_model_iter_has_child(model, &iter)) {
			widget_comboboxtext_save_tree_values(model, &iter, outfile, first);
		} else {
			gtk_tree_model_get(model, &iter, COMBOBOX_RICH_VALUE, &value, -1);
			if (!*first)
				fputc('\n', outfile);
			fputs(value != NULL ? value : "", outfile);
			*first = FALSE;
			g_free(value);
		}
		valid = gtk_tree_model_iter_next(model, &iter);
	}
}

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_comboboxtext_clear(variable *var)
{
	gchar            *oldselected;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We'll manage signals ourselves */
	GTKD_FUNCTION_SIGNALS_BLOCK;

	/* Record the currently selected text if any */
	oldselected = widget_comboboxtext_get_active_text(var->Widget);
#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): oldselected='%s'\n", __func__, oldselected);
#endif

	widget_comboboxtext_remove_all(var->Widget);
	/* The comboboxtext functions also manage the comboboxentry:
	 * clear the entry */
	if (var->Type == WIDGET_COMBOBOXENTRY) {
#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): clearing the entry\n", __func__);
#endif
		gtk_entry_set_text(
			GTK_ENTRY(gtk_bin_get_child(GTK_BIN(var->Widget))), "");
	}

	/* We'll manage signals ourselves */
	GTKD_FUNCTION_SIGNALS_UNBLOCK;

	/* The widget will now be empty (entry included if applicable)
	 * and its active index will be -1, so if the recorded text
	 * isn't null then we'll emit a changed signal */
	if (strcmp(oldselected, "")) {
		g_signal_emit_by_name(G_OBJECT(var->Widget), "changed");
#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): emitting 'changed' signal\n", __func__);
#endif
	}
	g_free(oldselected);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/

GtkWidget *widget_comboboxtext_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkCellRenderer *renderer;
	GtkListStore    *store;
	GtkTreeStore    *tree_store;
	GtkWidget        *widget;
	const gchar      *tag_name;
	gchar            *value;
	gboolean          completion_enabled = FALSE;
	gboolean          completion_case_sensitive = FALSE;
	gboolean          rich = FALSE;
	gboolean          tree = FALSE;
	gint              completion_minimum_key_length = 1;
	gint              icon_size = 16;
	WidgetCompletionMatch completion_match = WIDGET_COMPLETION_MATCH_PREFIX;

	(void)Attr;
	tag_name = Type == WIDGET_COMBOBOXENTRY ? "comboboxentry" :
		"comboboxtext";

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (attr != NULL && (value = get_tag_attribute(attr, "item-model"))) {
		if (strcasecmp(value, "rich") == 0)
			rich = TRUE;
		else if (strcasecmp(value, "rich-tree") == 0) {
			rich = TRUE;
			tree = TRUE;
		}
		else if (strcasecmp(value, "text") != 0)
			gtkdialog_warning("Invalid %s item-model '%s'; using text.",
				tag_name, value);
	}
	if (attr != NULL && (value = get_tag_attribute(attr, "icon-size")))
		icon_size = widget_parse_bounded_integer(value, 1, G_MAXINT, 16,
			Type == WIDGET_COMBOBOXENTRY ? "comboboxentry icon-size" :
			"comboboxtext icon-size");
	if (attr != NULL && (value = get_tag_attribute(attr, "completion"))) {
		if (!widget_parse_boolean(value, &completion_enabled)) {
			gtkdialog_warning("Invalid %s completion value '%s'; using false.",
				tag_name, value);
			completion_enabled = FALSE;
		}
	}
	if (attr != NULL && (value = get_tag_attribute(attr,
		"completion-minimum-key-length")))
		completion_minimum_key_length = widget_parse_bounded_integer(value,
			0, G_MAXINT, 1, "comboboxentry completion-minimum-key-length");
	if (attr != NULL) {
		completion_match = widget_parse_completion_match(
			get_tag_attribute(attr, "completion-match"),
			"comboboxentry completion-match");
		completion_case_sensitive = widget_parse_completion_case_sensitive(
			get_tag_attribute(attr, "completion-case-sensitive"),
			"comboboxentry completion-case-sensitive");
	}
	if (completion_enabled && Type != WIDGET_COMBOBOXENTRY) {
		gtkdialog_warning("Completion is available only for comboboxentry; "
			"ignoring it for comboboxtext.");
		completion_enabled = FALSE;
	}
	if (tree && Type != WIDGET_COMBOBOXTEXT) {
		gtkdialog_warning("The rich-tree item model is available only for "
			"comboboxtext; using the text model for comboboxentry.");
		rich = FALSE;
		tree = FALSE;
	}

	if (rich) {
		if (tree) {
			tree_store = gtk_tree_store_new(COMBOBOX_RICH_COLUMNS,
				G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF, G_TYPE_BOOLEAN,
				G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
			widget = gtk_combo_box_new_with_model(GTK_TREE_MODEL(tree_store));
			g_object_unref(tree_store);
			g_object_set_data(G_OBJECT(widget), "gtkdialog-comboboxtext-tree",
				GINT_TO_POINTER(TRUE));
		} else {
			store = gtk_list_store_new(COMBOBOX_RICH_COLUMNS,
				G_TYPE_STRING, G_TYPE_STRING, GDK_TYPE_PIXBUF, G_TYPE_BOOLEAN,
				G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
				G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
			if (Type == WIDGET_COMBOBOXENTRY)
				widget = gtk_combo_box_entry_new_with_model(
					GTK_TREE_MODEL(store), COMBOBOX_RICH_ENTRY_TEXT);
			else
				widget = gtk_combo_box_new_with_model(GTK_TREE_MODEL(store));
			g_object_unref(store);
		}
		g_object_set_data(G_OBJECT(widget), "gtkdialog-comboboxtext-rich",
			GINT_TO_POINTER(TRUE));
		g_object_set_data(G_OBJECT(widget), "gtkdialog-comboboxtext-icon-size",
			GINT_TO_POINTER(icon_size));

		gtk_cell_layout_clear(GTK_CELL_LAYOUT(widget));
		renderer = gtk_cell_renderer_pixbuf_new();
		gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, FALSE);
		gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(widget), renderer,
			"pixbuf", COMBOBOX_RICH_PIXBUF);
		gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(widget), renderer,
			"sensitive", COMBOBOX_RICH_SENSITIVE);
		renderer = gtk_cell_renderer_text_new();
		gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, TRUE);
		gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(widget), renderer,
			widget_comboboxtext_rich_cell_data, NULL, NULL);
	} else if (Type == WIDGET_COMBOBOXTEXT) {
		/* Thunor: gtk_combo_box_new_text() is deprecated but
		 * gtk_combo_box_text_new() and associated functions are
		 * unavailable so I have to use the deprecated functions */

		widget = gtk_combo_box_new_text();
	} else {
		/* Thunor: gtk_combo_box_entry_new_text() is deprectated too:
		 * gtk_combo_box_new_with_entry() is unavailable,
		 * gtk_combo_box_text_new_with_entry() is unavailable,
		 * and my brain hurts badly when I read the API ;) */
		widget = gtk_combo_box_entry_new_text();
	}
	if (completion_enabled) {
		GtkEntryCompletion *completion;
		GtkWidget *entry;

		entry = gtk_bin_get_child(GTK_BIN(widget));
		completion = gtk_entry_completion_new();
		gtk_entry_completion_set_model(completion,
			gtk_combo_box_get_model(GTK_COMBO_BOX(widget)));
		gtk_entry_completion_set_text_column(completion,
			rich ? COMBOBOX_RICH_ENTRY_TEXT : 0);
		widget_entry_completion_set_matching(completion,
			rich ? COMBOBOX_RICH_ENTRY_TEXT : 0, completion_match,
			completion_case_sensitive);
		gtk_entry_completion_set_minimum_key_length(completion,
			completion_minimum_key_length);
		if (rich) {
			gtk_cell_layout_clear(GTK_CELL_LAYOUT(completion));
			renderer = gtk_cell_renderer_pixbuf_new();
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(completion), renderer,
				FALSE);
			gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(completion), renderer,
				"pixbuf", COMBOBOX_RICH_PIXBUF);
			gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(completion), renderer,
				"sensitive", COMBOBOX_RICH_SENSITIVE);
			renderer = gtk_cell_renderer_text_new();
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(completion), renderer,
				TRUE);
			gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(completion),
				renderer, widget_comboboxtext_rich_cell_data, NULL, NULL);
		}
		g_signal_connect(G_OBJECT(completion), "match-selected",
			G_CALLBACK(widget_comboboxtext_completion_match_selected), widget);
		gtk_entry_set_completion(GTK_ENTRY(entry), completion);
		g_object_unref(completion);
	}
	if (attr != NULL) {
		kill_tag_attribute(attr, "item-model");
		kill_tag_attribute(attr, "icon-size");
		kill_tag_attribute(attr, "completion");
		kill_tag_attribute(attr, "completion-minimum-key-length");
		kill_tag_attribute(attr, "completion-match");
		kill_tag_attribute(attr, "completion-case-sensitive");
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_comboboxtext_envvar_construct(GtkWidget *widget)
{
	gchar            *string;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	string = widget_comboboxtext_get_active_text(widget);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_comboboxtext_fileselect(
	variable *var, const char *name, const char *value)
{
	gint              index;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	index = gtk_combo_box_get_active(GTK_COMBO_BOX(var->Widget));

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): index=%i name=%s value=%s\n", __func__, index, name,
		value);
#endif

	if (index < 0) index = 0;
	if (widget_comboboxtext_is_rich(var->Widget))
		widget_comboboxtext_append_plain_rich(var->Widget, value, index);
	else
		gtk_combo_box_insert_text(GTK_COMBO_BOX(var->Widget), index, value);

	gtk_combo_box_set_active(GTK_COMBO_BOX(var->Widget), index);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Refresh                                                             *
 ***********************************************************************/

void widget_comboboxtext_refresh(variable *var)
{
	GList            *element;
	GtkTreeIter       iter;
	GtkTreeModel     *model;
	gchar            *act;
	gchar            *format;
	gchar            *newselected;
	gchar            *oldselected;
	gchar            *string;
	gchar            *text;
	gint              found;
	gint              index;
	gint              initialised = FALSE;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(var->Widget), "_initialised"));

	/* We'll manage signals ourselves */
	GTKD_FUNCTION_SIGNALS_BLOCK;

	/* Record the currently selected text if any */
	oldselected = widget_comboboxtext_get_active_text(var->Widget);
#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): oldselected='%s'\n", __func__, oldselected);
#endif

	/* Clear the widget if it has been initialised */
	if (initialised) {
		widget_comboboxtext_remove_all(var->Widget);
		/* The comboboxtext functions also manage the comboboxentry:
		 * clear the entry */
		if (var->Type == WIDGET_COMBOBOXENTRY) {
			gtk_entry_set_text(
				GTK_ENTRY(gtk_bin_get_child(GTK_BIN(var->Widget))), "");
		}
	}

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		format = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_INPUT, "format");
		if (widget_comboboxtext_is_tree(var->Widget) && format != NULL &&
			strcasecmp(format, "json") == 0) {
			if (input_is_shell_command(act))
				widget_comboboxtext_input_json(var, act + 8, TRUE);
			else if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
				if (!initialised)
					widget_file_monitor_try_create(var, act + 5);
				widget_comboboxtext_input_json(var, act + 5, FALSE);
			}
		} else {
			if (input_is_shell_command(act))
				widget_comboboxtext_input_by_command(var, act + 8);
			/* input file stock = "File:", input file = "File:/path/to/file" */
			if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
				if (!initialised) {
					/* Check for file-monitor and create if requested */
					widget_file_monitor_try_create(var, act + 5);
				}
				widget_comboboxtext_input_by_file(var, act + 5);
			}
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_comboboxtext_input_by_items(var);

	/* Select a default item */
	if (var->Type == WIDGET_COMBOBOXTEXT) {
		if (widget_comboboxtext_is_tree(var->Widget))
			widget_comboboxtext_select_first(var->Widget);
		else
			gtk_combo_box_set_active(GTK_COMBO_BOX(var->Widget), 0);
	} else if (var->Type == WIDGET_COMBOBOXENTRY) {
		/* The comboboxtext functions also manage the comboboxentry:
		 * default to the entry */
		gtk_combo_box_set_active(GTK_COMBO_BOX(var->Widget), -1);
	}

	/* We'll manage signals ourselves */
	GTKD_FUNCTION_SIGNALS_UNBLOCK;

	/* Record the currently selected text if any */
	newselected = widget_comboboxtext_get_active_text(var->Widget);
#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): newselected='%s'\n", __func__, newselected);
#endif
	/* If the before and after selected items are different then
	 * emit a changed signal */
	if (strcmp(oldselected, newselected)) {
#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): emitting 'changed' signal\n", __func__);
#endif
		g_signal_emit_by_name(G_OBJECT(var->Widget), "changed");
	}
	g_free(oldselected);
	g_free(newselected);

	/* Initialise these only once at start-up */
	if (!initialised) {
		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
			fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT)) {
			string = attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT);
			model = gtk_combo_box_get_model(GTK_COMBO_BOX(var->Widget));
			if (widget_comboboxtext_is_tree(var->Widget)) {
				if (widget_comboboxtext_find_value(model, NULL, string, &iter))
					gtk_combo_box_set_active_iter(GTK_COMBO_BOX(var->Widget), &iter);
			} else if (gtk_tree_model_get_iter_first(model, &iter)) {
				index = 0;
				found = FALSE;
				do {
					gtk_tree_model_get(model, &iter,
						widget_comboboxtext_is_rich(var->Widget)
							? COMBOBOX_RICH_VALUE : 0,
						&text, -1);
#ifdef DEBUG_CONTENT
					fprintf(stderr, "%s(): string=%s text=%s\n", __func__,
						string, text);
#endif
					if (strcmp(string, text) == 0) {
						gtk_combo_box_set_active(GTK_COMBO_BOX(var->Widget),
							index);
						g_free(text);
						found = TRUE;
						break;
					}
					g_free(text);
					index++;
				} while (gtk_tree_model_iter_next(model, &iter));
				/* The comboboxtext functions also manage the comboboxentry:
				 * if default text not found then set it as default entry text */
				if (var->Type == WIDGET_COMBOBOXENTRY) {
					if (!found) {
						gtk_entry_set_text(GTK_ENTRY(
							gtk_bin_get_child(GTK_BIN(var->Widget))), string);
					}
				}
			}
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
		g_signal_connect(G_OBJECT(var->Widget), "changed", 
			G_CALLBACK(on_any_widget_changed_event),
			(gpointer)var->Attributes);
		/* The comboboxtext functions also manage the comboboxentry */
		if (var->Type == WIDGET_COMBOBOXENTRY) {
			/* Connect to the activate signal of the child entry widget */
			g_signal_connect(G_OBJECT(gtk_bin_get_child(
				GTK_BIN(var->Widget))), "activate",
				G_CALLBACK(on_any_widget_activate_event),
				(gpointer)var->Attributes);
			/* Connect to the button-press/release signals of the child
			 * entry widget */
			g_signal_connect(G_OBJECT(gtk_bin_get_child(
				GTK_BIN(var->Widget))), "button-press-event",
				G_CALLBACK(on_any_widget_button_pressed),
				(gpointer)var->Attributes);
			g_signal_connect(G_OBJECT(gtk_bin_get_child(
				GTK_BIN(var->Widget))), "button-release-event",
				G_CALLBACK(on_any_widget_button_released),
				(gpointer)var->Attributes);
		}
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_comboboxtext_removeselected(variable *var)
{
	GtkTreeIter       iter;
	gchar            *newselected;
	gchar            *oldselected;
	gint              index;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Thunor: We'll manage signals ourselves */
	GTKD_FUNCTION_SIGNALS_BLOCK;

	/* Record the currently selected text if any */
	oldselected = widget_comboboxtext_get_active_text(var->Widget);
#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): oldselected='%s'\n", __func__, oldselected);
#endif
	/* Delete the selected item */
	index = gtk_combo_box_get_active(GTK_COMBO_BOX(var->Widget));
	if (widget_comboboxtext_is_tree(var->Widget)) {
		if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(var->Widget), &iter))
			gtk_tree_store_remove(GTK_TREE_STORE(gtk_combo_box_get_model(
				GTK_COMBO_BOX(var->Widget))), &iter);
	} else if (index >= 0) {
		if (widget_comboboxtext_is_rich(var->Widget)) {
			if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(var->Widget), &iter))
				gtk_list_store_remove(GTK_LIST_STORE(gtk_combo_box_get_model(
					GTK_COMBO_BOX(var->Widget))), &iter);
		} else {
			gtk_combo_box_remove_text(GTK_COMBO_BOX(var->Widget), index);
		}
	}
	/* The comboboxtext functions also manage the comboboxentry:
	 * clear the entry */
	if (var->Type == WIDGET_COMBOBOXENTRY) {
		gtk_entry_set_text(
			GTK_ENTRY(gtk_bin_get_child(GTK_BIN(var->Widget))), "");
	}
	/* Auto-select the previous item rather than leaving it empty */
	if (widget_comboboxtext_is_tree(var->Widget)) {
		widget_comboboxtext_select_first(var->Widget);
	} else {
		if (index > 0) index--; else index = 0;
		gtk_combo_box_set_active(GTK_COMBO_BOX(var->Widget), index);
	}

	/* Thunor: We'll manage signals ourselves */
	GTKD_FUNCTION_SIGNALS_UNBLOCK;

	/* Record the currently selected text if any */
	newselected = widget_comboboxtext_get_active_text(var->Widget);
#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): newselected='%s'\n", __func__, newselected);
#endif
	/* If the before and after selected items are different then
	 * emit a changed signal */
	if (strcmp(oldselected, newselected)) {
#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): emitting 'changed' signal\n", __func__);
#endif
		g_signal_emit_by_name(G_OBJECT(var->Widget), "changed");
	}
	g_free(oldselected);
	g_free(newselected);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_comboboxtext_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	GtkTreeModel     *model;
	GtkTreeIter       iter;
	gchar            *act;
	gchar            *filename = NULL;
	gchar            *format;
	gchar            *text;
	gboolean          first;
	gboolean          json_format = FALSE;
	gint              index;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Require an explicit output path; never overwrite an input file as an
	 * implicit fallback. */
	act = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (act) {
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			filename = act + 5;
			format = attributeset_get_this_tagattr(&element, var->Attributes,
				ATTR_OUTPUT, "format");
			json_format = format != NULL && strcasecmp(format, "json") == 0;
			break;
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_OUTPUT);
	}
#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): filename=%s\n", __func__,
		filename ? filename : "(null)");
#endif

	/* If we have a valid filename then open it and dump the
	 * widget's data to it carefully not leaving a newline so
	 * that it can be read back in again */
	if (filename) {
		if (widget_comboboxtext_is_tree(var->Widget) && json_format) {
			widget_comboboxtext_save_json(var, filename);
			return;
		}
		if ((outfile = fopen(filename, "w"))) {
			if (widget_comboboxtext_is_tree(var->Widget)) {
				first = TRUE;
				model = gtk_combo_box_get_model(GTK_COMBO_BOX(var->Widget));
				widget_comboboxtext_save_tree_values(model, NULL, outfile, &first);
				widget_close_output(outfile, filename);
				return;
			}
			index = 0;
			/* The comboboxtext functions also manage the comboboxentry:
			 * save the entry if the active index is -1 and the entry
			 * isn't empty */
			if ((var->Type == WIDGET_COMBOBOXENTRY) &&
				(gtk_combo_box_get_active(GTK_COMBO_BOX(var->Widget)) == -1)) {
				text = widget_comboboxtext_get_active_text(var->Widget);
				if (text[0] != '\0') {
					fprintf(outfile, "%s", text);
					index++;
				}
				g_free(text);
			}
			model = gtk_combo_box_get_model(GTK_COMBO_BOX(var->Widget));
			if (gtk_tree_model_get_iter_first(model, &iter)) {
				do {
					gtk_tree_model_get(model, &iter,
						widget_comboboxtext_is_rich(var->Widget)
							? COMBOBOX_RICH_VALUE : 0,
						&text, -1);
					if (index) {
						fprintf(outfile, "\n%s", text);
					} else {
						fprintf(outfile, "%s", text);
						index++;
					}
					g_free(text);
				} while (gtk_tree_model_iter_next(model, &iter));
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

static void widget_comboboxtext_input_by_command(variable *var, char *command)
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
		while ((line = widget_read_line(infile)) != NULL) {
			if (widget_comboboxtext_is_rich(var->Widget))
				widget_comboboxtext_append_plain_rich(var->Widget, line, -1);
			else
				gtk_combo_box_append_text(GTK_COMBO_BOX(var->Widget), line);
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

static void widget_comboboxtext_input_by_file(variable *var, char *filename)
{
	FILE             *infile;
	gchar            *line;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if ((infile = fopen(filename, "r"))) {
		while ((line = widget_read_line(infile)) != NULL) {
			if (widget_comboboxtext_is_rich(var->Widget))
				widget_comboboxtext_append_plain_rich(var->Widget, line, -1);
			else
				gtk_combo_box_append_text(GTK_COMBO_BOX(var->Widget), line);
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

static void widget_comboboxtext_input_by_items(variable *var)
{
	GList            *element;
	gchar            *text;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	text = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (text) {
		if (widget_comboboxtext_is_rich(var->Widget)) {
			gchar *background;
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
				gtkdialog_warning("Invalid comboboxtext item markup value '%s'; "
					"using false.", markup_text);
			sensitive_text = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "sensitive");
			if (sensitive_text != NULL &&
				!widget_parse_boolean(sensitive_text, &sensitive)) {
				gtkdialog_warning("Invalid comboboxtext item sensitive value '%s'; "
					"using true.", sensitive_text);
				sensitive = TRUE;
			}
			foreground = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "foreground");
			background = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "background");
			font = attributeset_get_this_tagattr(&element, var->Attributes,
				ATTR_ITEM, "font");
			widget_comboboxtext_append_rich(var->Widget, text, value,
				stock_name, icon_name, image_name, markup, sensitive,
				foreground, background, font, -1);
		} else {
			gtk_combo_box_append_text(GTK_COMBO_BOX(var->Widget), text);
		}
		text = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

#if HAVE_JSON_GLIB
static gboolean widget_comboboxtext_json_value_is_scalar(JsonNode *node)
{
	GType type;

	if (JSON_NODE_HOLDS_NULL(node))
		return TRUE;
	if (!JSON_NODE_HOLDS_VALUE(node))
		return FALSE;
	type = json_node_get_value_type(node);
	return type == G_TYPE_STRING || type == G_TYPE_INT64 ||
		type == G_TYPE_DOUBLE || type == G_TYPE_BOOLEAN;
}

static gchar *widget_comboboxtext_json_scalar_to_string(JsonNode *node)
{
	gchar buffer[G_ASCII_DTOSTR_BUF_SIZE];
	GType type;

	if (JSON_NODE_HOLDS_NULL(node))
		return g_strdup("");
	type = json_node_get_value_type(node);
	if (type == G_TYPE_STRING)
		return g_strdup(json_node_get_string(node));
	if (type == G_TYPE_INT64)
		return g_strdup_printf("%" G_GINT64_FORMAT, json_node_get_int(node));
	if (type == G_TYPE_DOUBLE)
		return g_strdup(g_ascii_dtostr(buffer, sizeof(buffer),
			json_node_get_double(node)));
	if (type == G_TYPE_BOOLEAN)
		return g_strdup(json_node_get_boolean(node) ? "true" : "false");
	return g_strdup("");
}

static gboolean widget_comboboxtext_json_validate_string_member(
	JsonObject *object, const gchar *name, guint row, guint depth,
	GError **error)
{
	JsonNode *member;

	member = json_object_get_member(object, name);
	if (member == NULL)
		return TRUE;
	if (JSON_NODE_HOLDS_VALUE(member) &&
		json_node_get_value_type(member) == G_TYPE_STRING)
		return TRUE;
	g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
		"comboboxtext JSON %s in row %u at depth %u is not a string",
		name, row, depth);
	return FALSE;
}

static gboolean widget_comboboxtext_json_validate_rows(JsonArray *rows,
	guint depth, GError **error)
{
	static const gchar *style_names[] = {
		"foreground", "background", "font", "pixbuf-file", NULL
	};
	JsonArray *children;
	JsonArray *values;
	JsonNode *member;
	JsonNode *row_node;
	JsonObject *row;
	JsonObject *style;
	guint index;
	guint name_index;
	guint value_index;

	if (depth > COMBOBOX_TREE_JSON_MAX_DEPTH) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"comboboxtext JSON exceeds the maximum nesting depth of %u",
			COMBOBOX_TREE_JSON_MAX_DEPTH);
		return FALSE;
	}

	for (index = 0; index < json_array_get_length(rows); ++index) {
		row_node = json_array_get_element(rows, index);
		if (!JSON_NODE_HOLDS_OBJECT(row_node)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"comboboxtext JSON row %u at depth %u is not an object",
				index, depth);
			return FALSE;
		}
		row = json_node_get_object(row_node);
		member = json_object_get_member(row, "values");
		if (member == NULL || !JSON_NODE_HOLDS_ARRAY(member)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"comboboxtext JSON row %u at depth %u has no values array",
				index, depth);
			return FALSE;
		}
		values = json_node_get_array(member);
		if (json_array_get_length(values) < 1 ||
			json_array_get_length(values) > 2) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"comboboxtext JSON values in row %u at depth %u must contain one or two values",
				index, depth);
			return FALSE;
		}
		for (value_index = 0; value_index < json_array_get_length(values);
			++value_index) {
			if (!widget_comboboxtext_json_value_is_scalar(
				json_array_get_element(values, value_index))) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"comboboxtext JSON value %u in row %u at depth %u is not scalar",
					value_index, index, depth);
				return FALSE;
			}
		}
		if (!widget_comboboxtext_json_validate_string_member(row,
			"stock-id", index, depth, error) ||
			!widget_comboboxtext_json_validate_string_member(row,
				"icon-name", index, depth, error))
			return FALSE;
		if (json_object_has_member(row, "stock-id") &&
			json_object_has_member(row, "icon-name")) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"comboboxtext JSON row %u at depth %u has both stock-id and icon-name",
				index, depth);
			return FALSE;
		}
		member = json_object_get_member(row, "markup");
		if (member != NULL && (!JSON_NODE_HOLDS_VALUE(member) ||
			json_node_get_value_type(member) != G_TYPE_BOOLEAN)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"comboboxtext JSON markup in row %u at depth %u is not Boolean",
				index, depth);
			return FALSE;
		}
		member = json_object_get_member(row, "sensitive");
		if (member != NULL && (!JSON_NODE_HOLDS_VALUE(member) ||
			json_node_get_value_type(member) != G_TYPE_BOOLEAN)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"comboboxtext JSON sensitive in row %u at depth %u is not Boolean",
				index, depth);
			return FALSE;
		}
		member = json_object_get_member(row, "style");
		if (member != NULL) {
			if (!JSON_NODE_HOLDS_OBJECT(member)) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"comboboxtext JSON style in row %u at depth %u is not an object",
					index, depth);
				return FALSE;
			}
			style = json_node_get_object(member);
			for (name_index = 0; style_names[name_index] != NULL;
				++name_index) {
				if (!widget_comboboxtext_json_validate_string_member(style,
					style_names[name_index], index, depth, error))
					return FALSE;
			}
		}
		member = json_object_get_member(row, "children");
		if (member != NULL) {
			if (!JSON_NODE_HOLDS_ARRAY(member)) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"comboboxtext JSON children in row %u at depth %u is not an array",
					index, depth);
				return FALSE;
			}
			children = json_node_get_array(member);
			if (!widget_comboboxtext_json_validate_rows(children, depth + 1,
				error))
				return FALSE;
		}
	}
	return TRUE;
}

static void widget_comboboxtext_json_append_rows(variable *var,
	JsonArray *rows, GtkTreeIter *parent)
{
	GtkTreeIter iter;
	JsonArray *children;
	JsonArray *values;
	JsonNode *member;
	JsonObject *row;
	JsonObject *style = NULL;
	const gchar *background = NULL;
	const gchar *font = NULL;
	const gchar *foreground = NULL;
	const gchar *icon_name = NULL;
	const gchar *image_name = NULL;
	const gchar *stock_name = NULL;
	gchar *label;
	gchar *value;
	gboolean markup;
	gboolean sensitive;
	guint index;

	for (index = 0; index < json_array_get_length(rows); ++index) {
		row = json_node_get_object(json_array_get_element(rows, index));
		values = json_object_get_array_member(row, "values");
		label = widget_comboboxtext_json_scalar_to_string(
			json_array_get_element(values, 0));
		value = json_array_get_length(values) > 1 ?
			widget_comboboxtext_json_scalar_to_string(
				json_array_get_element(values, 1)) : g_strdup(label);
		stock_name = json_object_has_member(row, "stock-id") ?
			json_object_get_string_member(row, "stock-id") : NULL;
		icon_name = json_object_has_member(row, "icon-name") ?
			json_object_get_string_member(row, "icon-name") : NULL;
		markup = json_object_has_member(row, "markup") &&
			json_object_get_boolean_member(row, "markup");
		sensitive = !json_object_has_member(row, "sensitive") ||
			json_object_get_boolean_member(row, "sensitive");
		member = json_object_get_member(row, "style");
		if (member != NULL) {
			style = json_node_get_object(member);
			foreground = json_object_has_member(style, "foreground") ?
				json_object_get_string_member(style, "foreground") : NULL;
			background = json_object_has_member(style, "background") ?
				json_object_get_string_member(style, "background") : NULL;
			font = json_object_has_member(style, "font") ?
				json_object_get_string_member(style, "font") : NULL;
			image_name = json_object_has_member(style, "pixbuf-file") ?
				json_object_get_string_member(style, "pixbuf-file") : NULL;
		} else {
			style = NULL;
			foreground = background = font = image_name = NULL;
		}
		widget_comboboxtext_append_rich_at(var->Widget, parent, &iter,
			label, value, stock_name, icon_name, image_name, markup, sensitive,
			foreground, background, font, -1);
		g_free(label);
		g_free(value);
		member = json_object_get_member(row, "children");
		if (member != NULL) {
			children = json_node_get_array(member);
			widget_comboboxtext_json_append_rows(var, children, &iter);
		}
	}
}

static gchar *widget_comboboxtext_json_read_stream(FILE *stream)
{
	GString *contents;
	gchar buffer[4096];
	size_t count;

	contents = g_string_new(NULL);
	while ((count = fread(buffer, 1, sizeof(buffer), stream)) > 0)
		g_string_append_len(contents, buffer, count);
	if (ferror(stream)) {
		g_string_free(contents, TRUE);
		return NULL;
	}
	return g_string_free(contents, FALSE);
}
#endif

static void widget_comboboxtext_input_json(variable *var,
	const gchar *source, gboolean is_command)
{
#if HAVE_JSON_GLIB
	FILE *stream;
	GError *error = NULL;
	JsonArray *rows;
	JsonNode *member;
	JsonNode *root;
	JsonObject *document;
	JsonParser *parser;
	gchar *contents;

	stream = is_command ? widget_opencommand((gchar *)source) : fopen(source, "r");
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, source);
		return;
	}
	contents = widget_comboboxtext_json_read_stream(stream);
	if (is_command)
		widget_closecommand(stream, (gchar *)source);
	else
		fclose(stream);
	if (contents == NULL) {
		gtkdialog_warning("Couldn't read comboboxtext JSON input '%s'.", source);
		return;
	}
	parser = json_parser_new();
	if (!json_parser_load_from_data(parser, contents, -1, &error)) {
		gtkdialog_warning("Couldn't parse comboboxtext JSON input '%s': %s",
			source, error->message);
		g_error_free(error);
		g_object_unref(parser);
		g_free(contents);
		return;
	}
	g_free(contents);
	root = json_parser_get_root(parser);
	if (root == NULL || !JSON_NODE_HOLDS_OBJECT(root)) {
		g_set_error(&error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"comboboxtext JSON document root is not an object");
	} else {
		document = json_node_get_object(root);
		member = json_object_get_member(document, "version");
		if (member == NULL || !JSON_NODE_HOLDS_VALUE(member) ||
			json_node_get_value_type(member) != G_TYPE_INT64 ||
			json_node_get_int(member) != 1) {
			g_set_error(&error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"comboboxtext JSON document version must be the integer 1");
		} else {
			member = json_object_get_member(document, "rows");
			if (member == NULL || !JSON_NODE_HOLDS_ARRAY(member)) {
				g_set_error(&error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"comboboxtext JSON document has no rows array");
			} else {
				rows = json_node_get_array(member);
				if (widget_comboboxtext_json_validate_rows(rows, 1, &error))
					widget_comboboxtext_json_append_rows(var, rows, NULL);
			}
		}
	}
	if (error != NULL) {
		gtkdialog_warning("Invalid comboboxtext JSON input '%s': %s",
			source, error->message);
		g_error_free(error);
	}
	g_object_unref(parser);
#else
	(void)var;
	(void)is_command;
	gtkdialog_warning("Comboboxtext JSON input '%s' requires JSON-GLib support.",
		source);
#endif
}

#if HAVE_JSON_GLIB
static void widget_comboboxtext_json_build_rows(JsonBuilder *builder,
	GtkTreeModel *model, GtkTreeIter *parent)
{
	GtkTreeIter iter;
	gchar *background;
	gchar *font;
	gchar *foreground;
	gchar *icon_name;
	gchar *image_name;
	gchar *label;
	gchar *stock_name;
	gchar *value;
	gboolean markup;
	gboolean sensitive;
	gboolean valid;

	valid = parent != NULL ? gtk_tree_model_iter_children(model, &iter, parent) :
		gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		gtk_tree_model_get(model, &iter,
			COMBOBOX_RICH_VALUE, &value,
			COMBOBOX_RICH_LABEL, &label,
			COMBOBOX_RICH_MARKUP, &markup,
			COMBOBOX_RICH_SENSITIVE, &sensitive,
			COMBOBOX_RICH_FOREGROUND, &foreground,
			COMBOBOX_RICH_BACKGROUND, &background,
			COMBOBOX_RICH_FONT, &font,
			COMBOBOX_RICH_STOCK_ID, &stock_name,
			COMBOBOX_RICH_ICON_NAME, &icon_name,
			COMBOBOX_RICH_IMAGE_NAME, &image_name, -1);
		json_builder_begin_object(builder);
		if (stock_name != NULL && stock_name[0] != '\0') {
			json_builder_set_member_name(builder, "stock-id");
			json_builder_add_string_value(builder, stock_name);
		} else if (icon_name != NULL && icon_name[0] != '\0') {
			json_builder_set_member_name(builder, "icon-name");
			json_builder_add_string_value(builder, icon_name);
		}
		json_builder_set_member_name(builder, "values");
		json_builder_begin_array(builder);
		json_builder_add_string_value(builder, label != NULL ? label : "");
		json_builder_add_string_value(builder, value != NULL ? value : "");
		json_builder_end_array(builder);
		if (markup) {
			json_builder_set_member_name(builder, "markup");
			json_builder_add_boolean_value(builder, TRUE);
		}
		if (!sensitive) {
			json_builder_set_member_name(builder, "sensitive");
			json_builder_add_boolean_value(builder, FALSE);
		}
		if ((foreground != NULL && foreground[0] != '\0') ||
			(background != NULL && background[0] != '\0') ||
			(font != NULL && font[0] != '\0') ||
			((stock_name == NULL || stock_name[0] == '\0') &&
			 (icon_name == NULL || icon_name[0] == '\0') &&
			 image_name != NULL && image_name[0] != '\0')) {
			json_builder_set_member_name(builder, "style");
			json_builder_begin_object(builder);
			if (foreground != NULL && foreground[0] != '\0') {
				json_builder_set_member_name(builder, "foreground");
				json_builder_add_string_value(builder, foreground);
			}
			if (background != NULL && background[0] != '\0') {
				json_builder_set_member_name(builder, "background");
				json_builder_add_string_value(builder, background);
			}
			if (font != NULL && font[0] != '\0') {
				json_builder_set_member_name(builder, "font");
				json_builder_add_string_value(builder, font);
			}
			if ((stock_name == NULL || stock_name[0] == '\0') &&
				(icon_name == NULL || icon_name[0] == '\0') &&
				image_name != NULL && image_name[0] != '\0') {
				json_builder_set_member_name(builder, "pixbuf-file");
				json_builder_add_string_value(builder, image_name);
			}
			json_builder_end_object(builder);
		}
		if (gtk_tree_model_iter_has_child(model, &iter)) {
			json_builder_set_member_name(builder, "children");
			json_builder_begin_array(builder);
			widget_comboboxtext_json_build_rows(builder, model, &iter);
			json_builder_end_array(builder);
		}
		json_builder_end_object(builder);
		g_free(background);
		g_free(font);
		g_free(foreground);
		g_free(icon_name);
		g_free(image_name);
		g_free(label);
		g_free(stock_name);
		g_free(value);
		valid = gtk_tree_model_iter_next(model, &iter);
	}
}
#endif

static void widget_comboboxtext_save_json(variable *var,
	const gchar *filename)
{
#if HAVE_JSON_GLIB
	FILE *outfile;
	JsonBuilder *builder;
	JsonGenerator *generator;
	JsonNode *root;
	gchar *data;

	outfile = fopen(filename, "w");
	if (outfile == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}
	builder = json_builder_new();
	json_builder_begin_object(builder);
	json_builder_set_member_name(builder, "version");
	json_builder_add_int_value(builder, 1);
	json_builder_set_member_name(builder, "rows");
	json_builder_begin_array(builder);
	widget_comboboxtext_json_build_rows(builder,
		gtk_combo_box_get_model(GTK_COMBO_BOX(var->Widget)), NULL);
	json_builder_end_array(builder);
	json_builder_end_object(builder);
	root = json_builder_get_root(builder);
	generator = json_generator_new();
	json_generator_set_root(generator, root);
	json_generator_set_pretty(generator, TRUE);
	data = json_generator_to_data(generator, NULL);
	fputs(data, outfile);
	fputc('\n', outfile);
	widget_close_output(outfile, filename);
	g_free(data);
	g_object_unref(generator);
	json_node_free(root);
	g_object_unref(builder);
#else
	(void)var;
	gtkdialog_warning("Comboboxtext JSON output '%s' requires JSON-GLib support.",
		filename);
#endif
}
