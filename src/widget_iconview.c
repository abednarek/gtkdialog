/*
 * widget_iconview.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE
#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "stringman.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_iconview.h"

enum {
	ICONVIEW_PIXBUF,
	ICONVIEW_TEXT,
	ICONVIEW_PLAIN_TEXT,
	ICONVIEW_VALUE,
	ICONVIEW_MARKUP,
	ICONVIEW_FOREGROUND,
	ICONVIEW_BACKGROUND,
	ICONVIEW_FONT,
	ICONVIEW_DESCRIPTION,
	ICONVIEW_TOOLTIP,
	ICONVIEW_COLUMNS
};

typedef enum {
	ICONVIEW_INPUT_TEXT,
	ICONVIEW_INPUT_IMAGE
} IconViewInputMode;

static void widget_iconview_tree_path_free(gpointer data, gpointer user_data)
{
	(void)user_data;
	gtk_tree_path_free(data);
}

static gboolean widget_iconview_is_rich(GtkWidget *widget)
{
	return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
		"gtkdialog-iconview-rich"));
}

static void widget_iconview_text_cell_data(
	GtkCellLayout *layout, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gchar *background;
	gchar *font;
	gchar *foreground;
	gchar *text;
	gboolean markup;

	(void)layout;
	(void)data;
	gtk_tree_model_get(model, iter,
		ICONVIEW_TEXT, &text,
		ICONVIEW_MARKUP, &markup,
		ICONVIEW_FOREGROUND, &foreground,
		ICONVIEW_BACKGROUND, &background,
		ICONVIEW_FONT, &font,
		-1);
	if (markup)
		g_object_set(G_OBJECT(renderer), "markup", text, NULL);
	else
		g_object_set(G_OBJECT(renderer), "attributes", NULL,
			"text", text, NULL);
	g_object_set(G_OBJECT(renderer),
		"foreground", foreground,
		"foreground-set", foreground != NULL && foreground[0] != '\0',
		"background", background,
		"background-set", background != NULL && background[0] != '\0',
		"font", font,
		NULL);
	g_free(background);
	g_free(font);
	g_free(foreground);
	g_free(text);
}

static void widget_iconview_description_cell_data(
	GtkCellLayout *layout, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer data)
{
	gchar *description;

	(void)layout;
	(void)data;
	gtk_tree_model_get(model, iter,
		ICONVIEW_DESCRIPTION, &description,
		-1);
	g_object_set(G_OBJECT(renderer),
		"text", description != NULL ? description : "",
		"visible", description != NULL && description[0] != '\0',
		NULL);
	g_free(description);
}

static void widget_iconview_append(GtkWidget *widget, const gchar *text,
	const gchar *value, const gchar *stock_name, const gchar *icon_name,
	const gchar *image_name, gboolean markup, const gchar *foreground,
	const gchar *background, const gchar *font, const gchar *description,
	const gchar *tooltip)
{
	GdkPixbuf *pixbuf;
	GtkListStore *store;
	GtkTreeIter iter;
	gchar *plain_text = NULL;
	gint icon_size;

	store = GTK_LIST_STORE(gtk_icon_view_get_model(GTK_ICON_VIEW(widget)));
	icon_size = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), "gtkdialog-icon-size"));
	pixbuf = widget_load_pixbuf(widget, stock_name, icon_name, image_name,
		icon_size);
	if (markup && !pango_parse_markup(text, -1, 0, NULL, &plain_text,
		NULL, NULL)) {
		gtkdialog_warning("Invalid icon view item markup; displaying it "
			"as plain text.");
		markup = FALSE;
	}
	if (plain_text == NULL)
		plain_text = g_strdup(text);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
		ICONVIEW_PIXBUF, pixbuf,
		ICONVIEW_TEXT, text,
		ICONVIEW_PLAIN_TEXT, plain_text,
		ICONVIEW_VALUE, value != NULL ? value : plain_text,
		ICONVIEW_MARKUP, markup,
		ICONVIEW_FOREGROUND, foreground,
		ICONVIEW_BACKGROUND, background,
		ICONVIEW_FONT, font,
		ICONVIEW_DESCRIPTION, description,
		ICONVIEW_TOOLTIP, tooltip,
		-1);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
	g_free(plain_text);
}

static void widget_iconview_append_plain(GtkWidget *widget,
	const gchar *text, const gchar *image_name)
{
	widget_iconview_append(widget, text, text, NULL, NULL, image_name,
		FALSE, NULL, NULL, NULL, NULL, NULL);
}

static void widget_iconview_populate_stream(
	variable *var, FILE *input)
{
	gchar *line;
	IconViewInputMode input_mode;

	input_mode = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "gtkdialog-iconview-input-mode"));

	while ((line = widget_read_line(input)) != NULL) {
		widget_iconview_append_plain(var->Widget, line,
			input_mode == ICONVIEW_INPUT_IMAGE ? line : NULL);
		g_free(line);
	}
}

static void widget_iconview_input_by_command(
	variable *var, const gchar *command)
{
	FILE *input;

	input = widget_opencommand(command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	widget_iconview_populate_stream(var, input);
	widget_closecommand(input, command);
}

static void widget_iconview_input_by_file(
	variable *var, const gchar *filename)
{
	FILE *input;

	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}
	widget_iconview_populate_stream(var, input);
	fclose(input);
}

static void widget_iconview_input_by_items(variable *var)
{
	GList *element;
	gchar *icon_name;
	gchar *image_name;
	gchar *stock_name;
	gchar *text;

	text = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (text != NULL) {
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
			var->Attributes, ATTR_ITEM, "image-name");
		if (image_name == NULL)
			image_name = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "image-file");
		if (widget_iconview_is_rich(var->Widget)) {
			gchar *background;
			gchar *description;
			gchar *font;
			gchar *foreground;
			gchar *markup_text;
			gchar *tooltip;
			gchar *value;
			gboolean markup = FALSE;

			value = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "value");
			markup_text = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "markup");
			if (markup_text != NULL &&
				!widget_parse_boolean(markup_text, &markup))
				gtkdialog_warning("Invalid icon view item markup value '%s'; "
					"using false.", markup_text);
			foreground = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "foreground");
			background = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "background");
			font = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "font");
			description = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "description");
			tooltip = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_ITEM, "tooltip");
			widget_iconview_append(var->Widget, text, value,
				stock_name, icon_name, image_name, markup, foreground,
				background, font, description, tooltip);
		} else {
			widget_iconview_append(var->Widget, text, text,
				stock_name, icon_name, image_name, FALSE, NULL, NULL,
				NULL, NULL, NULL);
		}
		text = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}
}

static void widget_iconview_select_value(
	GtkWidget *widget, const gchar *value)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *text;
	gboolean valid;

	gtk_icon_view_unselect_all(GTK_ICON_VIEW(widget));
	if (value == NULL || value[0] == '\0')
		return;
	model = gtk_icon_view_get_model(GTK_ICON_VIEW(widget));
	valid = gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		gtk_tree_model_get(model, &iter,
			widget_iconview_is_rich(widget) ? ICONVIEW_VALUE : ICONVIEW_TEXT,
			&text, -1);
		if (strcmp(text, value) == 0) {
			GtkTreePath *path = gtk_tree_model_get_path(model, &iter);

			gtk_icon_view_select_path(GTK_ICON_VIEW(widget), path);
			gtk_icon_view_set_cursor(GTK_ICON_VIEW(widget), path, NULL, FALSE);
			gtk_tree_path_free(path);
			g_free(text);
			return;
		}
		g_free(text);
		valid = gtk_tree_model_iter_next(model, &iter);
	}
}

static void widget_iconview_item_activated(
	GtkWidget *widget, GtkTreePath *path, AttributeSet *Attr)
{
	(void)path;
	widget_signal_executor(widget, Attr, "item-activated");
}

void widget_iconview_clear(variable *var)
{
	GtkTreeModel *model;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(var->Widget));
	gtk_list_store_clear(GTK_LIST_STORE(model));
}

GtkWidget *widget_iconview_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkListStore *store;
	GtkSelectionMode mode = GTK_SELECTION_SINGLE;
	GtkWidget *widget;
	gchar *value;
	gboolean rich = FALSE;
	gboolean rich_show_pixbuf = TRUE;
	gboolean rich_show_text = TRUE;
	gint icon_size = 48;
	IconViewInputMode input_mode = ICONVIEW_INPUT_TEXT;

	(void)Attr;
	(void)Type;
	store = gtk_list_store_new(ICONVIEW_COLUMNS,
		GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
		G_TYPE_BOOLEAN, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
		G_TYPE_STRING, G_TYPE_STRING);
	widget = gtk_icon_view_new_with_model(GTK_TREE_MODEL(store));
	g_object_unref(store);
	if (attr != NULL) {
		value = get_tag_attribute(attr, "item-model");
		if (value != NULL) {
			if (strcasecmp(value, "rich") == 0)
				rich = TRUE;
			else if (strcasecmp(value, "text") != 0)
				gtkdialog_warning("Invalid icon view item-model value '%s'; "
					"using text.", value);
		}
		value = get_tag_attribute(attr, "selection-mode");
		if (value != NULL)
			mode = widget_parse_selection_mode(value, TRUE,
				"icon view selection-mode");
		value = get_tag_attribute(attr, "icon-size");
		if (value != NULL)
			icon_size = widget_parse_bounded_integer(value, 1, G_MAXINT,
				48, "icon view icon-size");
		value = get_tag_attribute(attr, "input-mode");
		if (value != NULL) {
			if (strcasecmp(value, "image") == 0)
				input_mode = ICONVIEW_INPUT_IMAGE;
			else if (strcasecmp(value, "text") != 0)
				gtkdialog_warning("Invalid icon view input-mode value '%s'; "
					"using text.", value);
		}
		if (rich) {
			value = get_tag_attribute(attr, "pixbuf-column");
			if (value != NULL)
				rich_show_pixbuf = widget_parse_bounded_integer(value, -1,
					G_MAXINT, ICONVIEW_PIXBUF,
					"rich icon view pixbuf-column") >= 0;
			value = get_tag_attribute(attr, "text-column");
			if (value != NULL)
				rich_show_text = widget_parse_bounded_integer(value, -1,
					G_MAXINT, ICONVIEW_TEXT,
					"rich icon view text-column") >= 0;
			kill_tag_attribute(attr, "pixbuf-column");
			kill_tag_attribute(attr, "text-column");
		}
		kill_tag_attribute(attr, "item-model");
		kill_tag_attribute(attr, "selection-mode");
		kill_tag_attribute(attr, "icon-size");
		kill_tag_attribute(attr, "input-mode");
	}
	if (rich) {
		GtkCellRenderer *renderer;

		if (rich_show_pixbuf) {
			renderer = gtk_cell_renderer_pixbuf_new();
			g_object_set(G_OBJECT(renderer), "xalign", 0.5, NULL);
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, FALSE);
			gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(widget), renderer,
				"pixbuf", ICONVIEW_PIXBUF);
		}
		if (rich_show_text) {
			renderer = gtk_cell_renderer_text_new();
			g_object_set(G_OBJECT(renderer), "xalign", 0.5, NULL);
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, TRUE);
			gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(widget), renderer,
				widget_iconview_text_cell_data, NULL, NULL);
			renderer = gtk_cell_renderer_text_new();
			g_object_set(G_OBJECT(renderer),
				"xalign", 0.5,
				"foreground", "#666666",
				"font", "Sans 8",
				NULL);
			gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, FALSE);
			gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(widget), renderer,
				widget_iconview_description_cell_data, NULL, NULL);
		}
		gtk_icon_view_set_tooltip_column(GTK_ICON_VIEW(widget),
			ICONVIEW_TOOLTIP);
		g_object_set_data(G_OBJECT(widget), "gtkdialog-iconview-rich",
			GINT_TO_POINTER(TRUE));
	} else {
		gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(widget), ICONVIEW_PIXBUF);
		gtk_icon_view_set_text_column(GTK_ICON_VIEW(widget), ICONVIEW_TEXT);
	}
	gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(widget), mode);
	g_object_set_data(G_OBJECT(widget), "gtkdialog-icon-size",
		GINT_TO_POINTER(icon_size));
	g_object_set_data(G_OBJECT(widget), "gtkdialog-iconview-input-mode",
		GINT_TO_POINTER(input_mode));
	return widget;
}

gchar *widget_iconview_envvar_construct(GtkWidget *widget)
{
	GList *element;
	GList *paths;
	GString *values;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *text;
	gboolean first = TRUE;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(widget));
	paths = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(widget));
	values = g_string_new(NULL);
	for (element = paths; element != NULL; element = element->next) {
		if (!gtk_tree_model_get_iter(model, &iter, element->data))
			continue;
		gtk_tree_model_get(model, &iter,
			widget_iconview_is_rich(widget) ? ICONVIEW_VALUE : ICONVIEW_TEXT,
			&text, -1);
		if (!first)
			g_string_append_c(values, '\n');
		g_string_append(values, text);
		first = FALSE;
		g_free(text);
	}
	g_list_foreach(paths, widget_iconview_tree_path_free, NULL);
	g_list_free(paths);
	return g_string_free(values, FALSE);
}

void widget_iconview_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_iconview_select_value(var->Widget, value);
}

void widget_iconview_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	widget_iconview_clear(var);
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_iconview_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_iconview_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	widget_iconview_input_by_items(var);
	if (!initialised) {
		if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
			attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
			gtk_widget_set_sensitive(var->Widget, FALSE);
		g_signal_connect(G_OBJECT(var->Widget), "item-activated",
			G_CALLBACK(widget_iconview_item_activated), var->Attributes);
		g_signal_connect(G_OBJECT(var->Widget), "selection-changed",
			G_CALLBACK(on_any_widget_selection_changed_event), var->Attributes);
	}
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_iconview_select_value(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
}

void widget_iconview_removeselected(variable *var)
{
	GList *element;
	GList *paths;
	GList *references = NULL;
	GtkTreeModel *model;

	model = gtk_icon_view_get_model(GTK_ICON_VIEW(var->Widget));
	paths = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(var->Widget));
	for (element = paths; element != NULL; element = element->next)
		references = g_list_prepend(references,
			gtk_tree_row_reference_new(model, element->data));
	g_list_foreach(paths, widget_iconview_tree_path_free, NULL);
	g_list_free(paths);
	for (element = references; element != NULL; element = element->next) {
		GtkTreePath *path;
		GtkTreeIter iter;

		path = gtk_tree_row_reference_get_path(element->data);
		if (path != NULL && gtk_tree_model_get_iter(model, &iter, path))
			gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
		if (path != NULL)
			gtk_tree_path_free(path);
		gtk_tree_row_reference_free(element->data);
	}
	g_list_free(references);
}

void widget_iconview_save(variable *var)
{
	FILE *output;
	GList *element;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *directive;
	gchar *filename = NULL;
	gchar *text;
	gboolean first = TRUE;
	gboolean valid;

	directive = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (strncasecmp(directive, "file:", 5) == 0 &&
			strlen(directive) > 5) {
			filename = directive + 5;
			break;
		}
		directive = attributeset_get_next(&element,
			var->Attributes, ATTR_OUTPUT);
	}
	if (filename == NULL) {
		fprintf(stderr, "%s(): No <output file> directive found.\n",
			__func__);
		return;
	}
	output = fopen(filename, "w");
	if (output == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}
	model = gtk_icon_view_get_model(GTK_ICON_VIEW(var->Widget));
	valid = gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		gtk_tree_model_get(model, &iter,
			widget_iconview_is_rich(var->Widget) ? ICONVIEW_VALUE : ICONVIEW_TEXT,
			&text, -1);
		if (!first)
			fputc('\n', output);
		fputs(text, output);
		first = FALSE;
		g_free(text);
		valid = gtk_tree_model_iter_next(model, &iter);
	}
	widget_close_output(output, filename);
}
