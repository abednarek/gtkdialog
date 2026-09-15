/*
 * widget_sheet.c:
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
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_sheet.h"

#if HAVE_GTKSHEET
#include <gtksheet/gtksheet.h>
#endif

#define SHEET_DATA_KEY "gtkdialog-sheet-data"
#define SHEET_MAX_COLUMNS 256
#define SHEET_MAX_ROWS 100000

static const gchar *sheet_custom_attributes[] = {
	"columns", "rows", "title", "column-labels", "column-widths",
	"row-titles", "column-titles", "editable", "autoresize-columns",
	"autoresize-rows", "clip-text", "selection-mode", NULL
};

static void widget_sheet_kill_custom_attributes(tag_attr *attr)
{
	gint index;

	if (attr == NULL)
		return;
	for (index = 0; sheet_custom_attributes[index] != NULL; ++index)
		kill_tag_attribute(attr, sheet_custom_attributes[index]);
}

#if HAVE_GTKSHEET

typedef struct {
	guint columns;
	guint populated_rows;
	gboolean updating;
} SheetData;

typedef struct {
	GPtrArray *rows;
	guint columns;
	gboolean invalid;
} SheetBuilder;

static SheetData *widget_sheet_get_data(GtkWidget *widget)
{
	return g_object_get_data(G_OBJECT(widget), SHEET_DATA_KEY);
}

static void widget_sheet_builder_init(SheetBuilder *builder, guint columns)
{
	builder->rows = g_ptr_array_new_with_free_func((GDestroyNotify)g_strfreev);
	builder->columns = columns;
	builder->invalid = FALSE;
}

static void widget_sheet_builder_clear(SheetBuilder *builder)
{
	g_ptr_array_free(builder->rows, TRUE);
}

static void widget_sheet_builder_append(SheetBuilder *builder,
	const gchar *line)
{
	gchar **fields;
	guint count;

	if (builder->invalid)
		return;
	if (builder->rows->len >= SHEET_MAX_ROWS) {
		gtkdialog_warning("GtkSheet input exceeds the %d row limit.",
			SHEET_MAX_ROWS);
		builder->invalid = TRUE;
		return;
	}
	fields = g_strsplit(line != NULL ? line : "", "|", -1);
	for (count = 0; fields[count] != NULL; ++count)
		;
	if (count > builder->columns) {
		gtkdialog_warning(
			"GtkSheet row has %u columns; this sheet has %u.",
			count, builder->columns);
		g_strfreev(fields);
		builder->invalid = TRUE;
		return;
	}
	g_ptr_array_add(builder->rows, fields);
}

static gboolean widget_sheet_builder_read_stream(SheetBuilder *builder,
	FILE *stream)
{
	gchar *line;

	while ((line = widget_read_line(stream)) != NULL) {
		widget_sheet_builder_append(builder, line);
		g_free(line);
	}
	if (ferror(stream)) {
		builder->invalid = TRUE;
		return FALSE;
	}
	return !builder->invalid;
}

static gboolean widget_sheet_builder_read_file(SheetBuilder *builder,
	const gchar *filename)
{
	FILE *stream;
	gboolean valid;

	stream = fopen(filename, "r");
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return FALSE;
	}
	valid = widget_sheet_builder_read_stream(builder, stream);
	fclose(stream);
	return valid;
}

static gboolean widget_sheet_builder_read_command(SheetBuilder *builder,
	const gchar *command)
{
	FILE *stream;
	gboolean valid;
	gint status;

	stream = widget_opencommand(command);
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return FALSE;
	}
	valid = widget_sheet_builder_read_stream(builder, stream);
	status = widget_closecommand(stream, command);
	return valid && status == 0;
}

static void widget_sheet_apply(GtkWidget *widget, SheetBuilder *builder)
{
	SheetData *data = widget_sheet_get_data(widget);
	GtkSheet *sheet = GTK_SHEET(widget);
	guint current_rows;
	guint row;
	guint column;

	current_rows = gtk_sheet_get_rows_count(sheet);
	if (builder->rows->len > current_rows)
		gtk_sheet_add_row(sheet, builder->rows->len - current_rows);
	data->updating = TRUE;
	gtk_sheet_range_clear(sheet, NULL);
	for (row = 0; row < builder->rows->len; ++row) {
		gchar **fields = g_ptr_array_index(builder->rows, row);

		for (column = 0; fields[column] != NULL; ++column)
			gtk_sheet_set_cell_text(sheet, row, column, fields[column]);
	}
	data->populated_rows = builder->rows->len;
	data->updating = FALSE;
}

static gboolean widget_sheet_activate(GtkWidget *widget, gint row, gint column,
	AttributeSet *attributes)
{
	(void)row;
	(void)column;
	widget_signal_executor(widget, attributes, "activate");
	return TRUE;
}

static gboolean widget_sheet_deactivate(GtkWidget *widget, gint row,
	gint column, AttributeSet *attributes)
{
	SheetData *data = widget_sheet_get_data(widget);

	if (!data->updating && row >= 0 && column >= 0)
		data->populated_rows = MAX(data->populated_rows, (guint)row + 1);
	widget_signal_executor(widget, attributes, "deactivate");
	return TRUE;
}

static void widget_sheet_changed(GtkWidget *widget, gint row, gint column,
	AttributeSet *attributes)
{
	SheetData *data = widget_sheet_get_data(widget);

	(void)column;
	if (data->updating)
		return;
	if (row >= 0)
		data->populated_rows = MAX(data->populated_rows, (guint)row + 1);
	widget_signal_executor(widget, attributes, "changed");
}

static void widget_sheet_set_column_labels(GtkSheet *sheet,
	const gchar *labels)
{
	gchar **values;
	guint column;
	guint columns = gtk_sheet_get_columns_count(sheet);

	values = labels != NULL ? g_strsplit(labels, "|", -1) : NULL;
	for (column = 0; column < columns; ++column) {
		gchar generated[32];
		const gchar *title;

		if (values != NULL && values[column] != NULL)
			title = values[column];
		else {
			g_snprintf(generated, sizeof(generated), "%u", column + 1);
			title = generated;
		}
		gtk_sheet_column_button_add_label(sheet, column, title);
		gtk_sheet_set_column_title(sheet, column, title);
	}
	g_strfreev(values);
}

static void widget_sheet_set_column_widths(GtkSheet *sheet,
	const gchar *widths)
{
	gchar **values;
	guint column;
	guint columns;

	if (widths == NULL)
		return;
	columns = gtk_sheet_get_columns_count(sheet);
	values = g_strsplit(widths, "|", -1);
	for (column = 0; column < columns && values[column] != NULL; ++column) {
		gint width = widget_parse_bounded_integer(values[column], 1, 4096,
			80, "sheet column-widths");
		gtk_sheet_set_column_width(sheet, column, width);
	}
	g_strfreev(values);
}

#endif

GtkWidget *widget_sheet_create(AttributeSet *attributes, tag_attr *attr,
	gint type)
{
	GtkWidget *widget;

	(void)attributes;
	(void)type;
#if HAVE_GTKSHEET
	{
		SheetData *data;
		gchar *value;
		GdkColor background;
		gint columns;
		gint rows;

		columns = widget_parse_bounded_integer(
			attr != NULL ? get_tag_attribute(attr, "columns") : NULL,
			1, SHEET_MAX_COLUMNS, 3, "sheet columns");
		rows = widget_parse_bounded_integer(
			attr != NULL ? get_tag_attribute(attr, "rows") : NULL,
			1, SHEET_MAX_ROWS, 10, "sheet rows");
		value = attr != NULL ? get_tag_attribute(attr, "title") : NULL;
		widget = gtk_sheet_new(rows, columns, value != NULL ? value : "");
		value = attr != NULL ? get_tag_attribute(attr, "bgcolor") : NULL;
		if (value != NULL) {
			if (gdk_color_parse(value, &background))
				gtk_sheet_set_background(GTK_SHEET(widget), &background);
			else
				gtkdialog_warning(
					"Invalid sheet bgcolor '%s'; using the default colour.",
					value);
			kill_tag_attribute(attr, "bgcolor");
		}
		data = g_new0(SheetData, 1);
		data->columns = columns;
		g_object_set_data_full(G_OBJECT(widget), SHEET_DATA_KEY, data, g_free);

		widget_sheet_set_column_labels(GTK_SHEET(widget), attr != NULL
			? get_tag_attribute(attr, "column-labels") : NULL);
		widget_sheet_set_column_widths(GTK_SHEET(widget), attr != NULL
			? get_tag_attribute(attr, "column-widths") : NULL);
		value = attr != NULL ? get_tag_attribute(attr, "row-titles") : NULL;
		if (value != NULL && widget_attribute_is_false(value))
			gtk_sheet_hide_row_titles(GTK_SHEET(widget));
		value = attr != NULL ? get_tag_attribute(attr, "column-titles") : NULL;
		if (value != NULL && widget_attribute_is_false(value))
			gtk_sheet_hide_column_titles(GTK_SHEET(widget));
		value = attr != NULL ? get_tag_attribute(attr, "editable") : NULL;
		if (value != NULL && widget_attribute_is_false(value))
			gtk_sheet_set_locked(GTK_SHEET(widget), TRUE);
		value = attr != NULL
			? get_tag_attribute(attr, "autoresize-columns") : NULL;
		if (value != NULL)
			gtk_sheet_set_autoresize_columns(GTK_SHEET(widget),
				widget_attribute_is_true(value));
		value = attr != NULL
			? get_tag_attribute(attr, "autoresize-rows") : NULL;
		if (value != NULL)
			gtk_sheet_set_autoresize_rows(GTK_SHEET(widget),
				widget_attribute_is_true(value));
		value = attr != NULL ? get_tag_attribute(attr, "clip-text") : NULL;
		if (value != NULL)
			gtk_sheet_set_clip_text(GTK_SHEET(widget),
				widget_attribute_is_true(value));
		value = attr != NULL ? get_tag_attribute(attr, "selection-mode") : NULL;
		if (value != NULL)
			gtk_sheet_set_selection_mode(GTK_SHEET(widget),
				widget_parse_selection_mode(value, TRUE,
					"sheet selection-mode"));
	}
#else
	widget = gtk_label_new(
		"GtkSheet 3 support is not available in this build.");
	gtkdialog_warning(
		"Sheet requested, but this build does not include GtkSheet 3 support.");
#endif
	widget_sheet_kill_custom_attributes(attr);
	return widget;
}

void widget_sheet_clear(variable *var)
{
#if HAVE_GTKSHEET
	SheetData *data;

	if (!GTK_IS_SHEET(var->Widget))
		return;
	data = widget_sheet_get_data(var->Widget);
	data->updating = TRUE;
	gtk_sheet_range_clear(GTK_SHEET(var->Widget), NULL);
	data->populated_rows = 0;
	data->updating = FALSE;
#else
	(void)var;
#endif
}

gchar *widget_sheet_envvar_construct(GtkWidget *widget)
{
#if HAVE_GTKSHEET
	gchar *text;
	gint row;
	gint column;

	if (!GTK_IS_SHEET(widget))
		return g_strdup("");
	gtk_sheet_get_active_cell(GTK_SHEET(widget), &row, &column);
	if (row < 0 || column < 0)
		return g_strdup("");
	text = gtk_sheet_cell_get_text(GTK_SHEET(widget), row, column);
	return g_strdup(text != NULL ? text : "");
#else
	(void)widget;
	return g_strdup("");
#endif
}

void widget_sheet_fileselect(variable *var, const char *name,
	const char *value)
{
#if HAVE_GTKSHEET
	SheetBuilder builder;
	gchar **lines;
	guint line;

	(void)name;
	if (!GTK_IS_SHEET(var->Widget))
		return;
	widget_sheet_builder_init(&builder,
		widget_sheet_get_data(var->Widget)->columns);
	lines = g_strsplit(value, "\n", -1);
	for (line = 0; lines[line] != NULL; ++line) {
		if (lines[line + 1] == NULL && lines[line][0] == '\0')
			break;
		widget_sheet_builder_append(&builder, lines[line]);
	}
	g_strfreev(lines);
	if (!builder.invalid)
		widget_sheet_apply(var->Widget, &builder);
	widget_sheet_builder_clear(&builder);
#else
	(void)var;
	(void)name;
	(void)value;
#endif
}

void widget_sheet_refresh(variable *var)
{
#if HAVE_GTKSHEET
	SheetBuilder builder;
	SheetData *data;
	GList *element;
	gboolean initialised;
	gboolean valid = TRUE;
	gchar *input;
	gchar *item;

	if (!GTK_IS_SHEET(var->Widget))
		return;
	data = widget_sheet_get_data(var->Widget);
	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	widget_sheet_builder_init(&builder, data->columns);
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL && valid) {
		if (input_is_shell_command(input))
			valid = widget_sheet_builder_read_command(&builder, input + 8);
		else if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			valid = widget_sheet_builder_read_file(&builder, input + 5);
		} else
			valid = widget_sheet_builder_read_command(&builder, input);
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	item = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (item != NULL && valid) {
		widget_sheet_builder_append(&builder, item);
		valid = !builder.invalid;
		item = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}
	if (valid)
		widget_sheet_apply(var->Widget, &builder);
	widget_sheet_builder_clear(&builder);
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		fprintf(stderr, "%s(): <default> not implemented for sheet.\n",
			__func__);
	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr, "%s(): Use the sheet title attribute instead of <label>.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(var->Widget, "activate",
		G_CALLBACK(widget_sheet_activate), var->Attributes);
	g_signal_connect(var->Widget, "deactivate",
		G_CALLBACK(widget_sheet_deactivate), var->Attributes);
	g_signal_connect(var->Widget, "changed",
		G_CALLBACK(widget_sheet_changed), var->Attributes);
#else
	(void)var;
#endif
}

void widget_sheet_save(variable *var)
{
#if HAVE_GTKSHEET
	SheetData *data;
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	guint row;
	guint column;

	if (!GTK_IS_SHEET(var->Widget))
		return;
	directive = attributeset_get_first(&element,
		var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (g_ascii_strncasecmp(directive, "file:", 5) == 0 &&
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
	data = widget_sheet_get_data(var->Widget);
	for (row = 0; row < data->populated_rows; ++row) {
		for (column = 0; column < data->columns; ++column) {
			gchar *text = gtk_sheet_cell_get_text(
				GTK_SHEET(var->Widget), row, column);
			if (column > 0)
				fputc('|', output);
			if (text != NULL)
				fputs(text, output);
		}
		fputc('\n', output);
	}
	widget_close_output(output, filename);
#else
	(void)var;
#endif
}
