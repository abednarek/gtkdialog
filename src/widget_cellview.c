/*
 * widget_cellview.c:
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
#include "widgets.h"
#include "tag_attributes.h"
#include "widget_cellview.h"

enum {
	CELLVIEW_PIXBUF,
	CELLVIEW_TEXT,
	CELLVIEW_COLUMNS
};

typedef struct {
	gchar *stock_name;
	gchar *icon_name;
	gchar *image_name;
	gint icon_size;
} CellViewData;

static void widget_cellview_data_free(gpointer data)
{
	CellViewData *cell_data = data;

	g_free(cell_data->stock_name);
	g_free(cell_data->icon_name);
	g_free(cell_data->image_name);
	g_free(cell_data);
}

static gboolean widget_cellview_get_iter(
	GtkWidget *widget, GtkTreeIter *iter)
{
	GtkTreeModel *model;

	model = gtk_cell_view_get_model(GTK_CELL_VIEW(widget));
	return model != NULL && gtk_tree_model_get_iter_first(model, iter);
}

static void widget_cellview_set_text(
	GtkWidget *widget, const gchar *text)
{
	GtkTreeIter iter;

	if (widget_cellview_get_iter(widget, &iter))
		gtk_list_store_set(GTK_LIST_STORE(
			gtk_cell_view_get_model(GTK_CELL_VIEW(widget))), &iter,
			CELLVIEW_TEXT, text != NULL ? text : "", -1);
}

static void widget_cellview_set_pixbuf(
	GtkWidget *widget, GdkPixbuf *pixbuf)
{
	GtkTreeIter iter;

	if (widget_cellview_get_iter(widget, &iter))
		gtk_list_store_set(GTK_LIST_STORE(
			gtk_cell_view_get_model(GTK_CELL_VIEW(widget))), &iter,
			CELLVIEW_PIXBUF, pixbuf, -1);
}

static void widget_cellview_load_static_icon(GtkWidget *widget)
{
	CellViewData *cell_data;
	GdkPixbuf *pixbuf;

	cell_data = g_object_get_data(G_OBJECT(widget),
		"gtkdialog-cellview-data");
	pixbuf = widget_load_pixbuf(widget,
		cell_data->stock_name, cell_data->icon_name, cell_data->image_name,
		cell_data->icon_size);
	widget_cellview_set_pixbuf(widget, pixbuf);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
}

static void widget_cellview_input_by_command(
	variable *var, const gchar *command)
{
	FILE *input;
	gchar *text;

	input = widget_opencommand(command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	text = widget_read_all(input);
	widget_cellview_set_text(var->Widget, text);
	g_free(text);
	widget_closecommand(input, command);
}

static void widget_cellview_input_by_file(
	variable *var, const gchar *filename)
{
	FILE *input;
	gchar *text;

	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}
	text = widget_read_all(input);
	widget_cellview_set_text(var->Widget, text);
	g_free(text);
	fclose(input);
}

void widget_cellview_clear(variable *var)
{
	widget_cellview_set_text(var->Widget, "");
	widget_cellview_set_pixbuf(var->Widget, NULL);
}

GtkWidget *widget_cellview_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GdkPixbuf *pixbuf;
	CellViewData *cell_data;
	GtkCellRenderer *renderer;
	GList *element;
	GtkListStore *store;
	GtkTreeIter iter;
	GtkTreePath *path;
	GtkWidget *widget;
	const gchar *icon_name = NULL;
	const gchar *image_name = NULL;
	const gchar *stock_name = NULL;
	gchar *text;
	gchar *value;
	gboolean use_markup = FALSE;
	gint icon_size = 24;

	(void)Type;
	attributeset_set_if_unset(Attr, ATTR_LABEL, "");
	text = attributeset_get_first(&element, Attr, ATTR_LABEL);
	if (attributeset_is_avail(Attr, ATTR_DEFAULT))
		text = attributeset_get_first(&element, Attr, ATTR_DEFAULT);

	if (attr != NULL) {
		stock_name = get_tag_attribute(attr, "stock");
		icon_name = get_tag_attribute(attr, "icon-name");
		image_name = get_tag_attribute(attr, "image-name");
		if (image_name == NULL)
			image_name = get_tag_attribute(attr, "image-file");
		value = get_tag_attribute(attr, "icon-size");
		if (value != NULL)
			icon_size = widget_parse_bounded_integer(value, 1, G_MAXINT,
				24, "cell view icon-size");
		value = get_tag_attribute(attr, "use-markup");
		if (value != NULL)
			use_markup = widget_attribute_is_true(value);
	}

	widget = gtk_cell_view_new();
	cell_data = g_new0(CellViewData, 1);
	cell_data->stock_name = g_strdup(stock_name);
	cell_data->icon_name = g_strdup(icon_name);
	cell_data->image_name = g_strdup(image_name);
	cell_data->icon_size = icon_size;
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-cellview-data",
		cell_data, widget_cellview_data_free);
	store = gtk_list_store_new(CELLVIEW_COLUMNS,
		GDK_TYPE_PIXBUF, G_TYPE_STRING);
	pixbuf = widget_load_pixbuf(widget, stock_name, icon_name, image_name,
		icon_size);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter,
		CELLVIEW_PIXBUF, pixbuf,
		CELLVIEW_TEXT, text,
		-1);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
	gtk_cell_view_set_model(GTK_CELL_VIEW(widget), GTK_TREE_MODEL(store));
	g_object_unref(store);

	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, FALSE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(widget), renderer,
		"pixbuf", CELLVIEW_PIXBUF);
	renderer = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(widget), renderer, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(widget), renderer,
		use_markup ? "markup" : "text", CELLVIEW_TEXT);

	path = gtk_tree_path_new_first();
	gtk_cell_view_set_displayed_row(GTK_CELL_VIEW(widget), path);
	gtk_tree_path_free(path);

	if (attr != NULL) {
		kill_tag_attribute(attr, "stock");
		kill_tag_attribute(attr, "icon-name");
		kill_tag_attribute(attr, "image-name");
		kill_tag_attribute(attr, "image-file");
		kill_tag_attribute(attr, "icon-size");
		kill_tag_attribute(attr, "use-markup");
	}
	return widget;
}

gchar *widget_cellview_envvar_construct(GtkWidget *widget)
{
	GtkTreeIter iter;
	gchar *text = NULL;

	if (widget_cellview_get_iter(widget, &iter))
		gtk_tree_model_get(gtk_cell_view_get_model(GTK_CELL_VIEW(widget)),
			&iter, CELLVIEW_TEXT, &text, -1);
	return text != NULL ? text : g_strdup("");
}

void widget_cellview_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_cellview_set_text(var->Widget, value);
}

void widget_cellview_refresh(variable *var)
{
	GList *element;
	GList *value_element;
	gchar *input;
	gchar *text;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	text = attributeset_get_first(&value_element,
		var->Attributes, ATTR_LABEL);
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		text = attributeset_get_first(&value_element,
			var->Attributes, ATTR_DEFAULT);
	widget_cellview_set_text(var->Widget, text);
	widget_cellview_load_static_icon(var->Widget);

	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_cellview_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_cellview_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (!initialised &&
		(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
		gtk_widget_set_sensitive(var->Widget, FALSE);
}

void widget_cellview_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *text;

	directive = attributeset_get_first(&element,
		var->Attributes, ATTR_OUTPUT);
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
	text = widget_cellview_envvar_construct(var->Widget);
	fputs(text, output);
	g_free(text);
	widget_close_output(output, filename);
}
