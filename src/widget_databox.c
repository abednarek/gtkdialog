/*
 * widget_databox.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <float.h>
#include <math.h>
#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_databox.h"

#if HAVE_GTKDATABOX
#include <gtkdatabox.h>
#include <gtkdatabox_bars.h>
#include <gtkdatabox_grid.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_points.h>
#endif

#define DATABOX_DATA_KEY "gtkdialog-databox-data"
#define DATABOX_REQUESTED_KEY "gtkdialog-databox-requested"
#define DATABOX_MAX_SERIES 64

static const gchar *databox_custom_attributes[] = {
	"databox", "background", "series-types", "series-colors", "series-sizes",
	"show-grid", "grid-color", "grid-horizontal-lines",
	"grid-vertical-lines", "grid-line-style", "grid-line-width",
	"auto-rescale", "rescale-border", "min-x", "max-x", "min-y",
	"max-y", NULL
};

static void widget_databox_kill_custom_attributes(tag_attr *attr)
{
	gint index;

	if (attr == NULL)
		return;
	for (index = 0; databox_custom_attributes[index] != NULL; ++index)
		kill_tag_attribute(attr, databox_custom_attributes[index]);
}

void widget_databox_discard_mode_attributes(tag_attr *attr)
{
	gint index;

	if (attr == NULL)
		return;
	for (index = 0; databox_custom_attributes[index] != NULL; ++index) {
		if (strcmp(databox_custom_attributes[index], "background") != 0)
			kill_tag_attribute(attr, databox_custom_attributes[index]);
	}
}

static gboolean widget_databox_requested(tag_attr *attr)
{
	gchar *value;

	if (attr == NULL)
		return FALSE;
	value = get_tag_attribute(attr, "databox");
	return widget_attribute_is_true(value);
}

#if HAVE_GTKDATABOX

typedef struct {
	guint columns;
	guint length;
	gfloat **values;
	GList *graphs;
	gchar *text;
} DataboxPlot;

typedef struct {
	DataboxPlot *plot;
	gchar *selection;
	gchar *series_types;
	gchar *series_colors;
	gchar *series_sizes;
	gfloat border;
	gfloat min_x;
	gfloat max_x;
	gfloat min_y;
	gfloat max_y;
	gint grid_hlines;
	gint grid_vlines;
	gint grid_style;
	guint grid_width;
	GdkColor grid_color;
	gboolean auto_rescale;
	gboolean explicit_limits;
	gboolean log_scale_x;
	gboolean log_scale_y;
	gboolean show_grid;
	gboolean updating;
} DataboxData;

typedef struct {
	GArray **columns;
	guint column_count;
	GString *text;
	gboolean invalid;
} DataboxBuilder;

static DataboxData *widget_databox_get_data(GtkWidget *widget)
{
	return g_object_get_data(G_OBJECT(widget), DATABOX_DATA_KEY);
}

static void widget_databox_plot_free(DataboxPlot *plot)
{
	GList *graph;
	guint column;

	if (plot == NULL)
		return;
	for (graph = plot->graphs; graph != NULL; graph = graph->next)
		g_object_unref(graph->data);
	g_list_free(plot->graphs);
	for (column = 0; column < plot->columns; ++column)
		g_free(plot->values[column]);
	g_free(plot->values);
	g_free(plot->text);
	g_free(plot);
}

static void widget_databox_data_free(gpointer value)
{
	DataboxData *data = value;

	widget_databox_plot_free(data->plot);
	g_free(data->selection);
	g_free(data->series_types);
	g_free(data->series_colors);
	g_free(data->series_sizes);
	g_free(data);
}

static void widget_databox_destroyed(GtkWidget *widget, DataboxData *data)
{
	gtk_databox_graph_remove_all(GTK_DATABOX(widget));
	widget_databox_plot_free(data->plot);
	data->plot = NULL;
}

static gboolean widget_databox_parse_float(const gchar *text, gfloat *value)
{
	gchar *end;
	gdouble number;

	errno = 0;
	number = g_ascii_strtod(text, &end);
	while (g_ascii_isspace(*end))
		++end;
	if (errno != 0 || end == text || *end != '\0' || !isfinite(number) ||
		number < -FLT_MAX || number > FLT_MAX) {
		gtkdialog_warning("Invalid GtkDatabox numeric value '%s'.", text);
		return FALSE;
	}
	*value = (gfloat)number;
	return TRUE;
}

static void widget_databox_builder_init(DataboxBuilder *builder)
{
	memset(builder, 0, sizeof(*builder));
	builder->text = g_string_new(NULL);
}

static void widget_databox_builder_clear(DataboxBuilder *builder)
{
	guint column;

	for (column = 0; column < builder->column_count; ++column)
		g_array_free(builder->columns[column], TRUE);
	g_free(builder->columns);
	if (builder->text != NULL)
		g_string_free(builder->text, TRUE);
}

static void widget_databox_builder_append(DataboxBuilder *builder,
	const gchar *line)
{
	gchar **fields;
	gchar *copy;
	gfloat value;
	guint column;
	guint count;

	if (builder->invalid)
		return;
	copy = g_strdup(line != NULL ? line : "");
	g_strstrip(copy);
	if (copy[0] == '\0' || copy[0] == '#') {
		g_free(copy);
		return;
	}
	fields = g_strsplit(copy, "|", -1);
	for (count = 0; fields[count] != NULL; ++count)
		;
	if (count < 2 || count > DATABOX_MAX_SERIES + 1) {
		gtkdialog_warning(
			"Invalid GtkDatabox row; expected x and 1..%d y columns.",
			DATABOX_MAX_SERIES);
		builder->invalid = TRUE;
		goto done;
	}
	if (builder->column_count == 0) {
		builder->column_count = count;
		builder->columns = g_new0(GArray *, count);
		for (column = 0; column < count; ++column)
			builder->columns[column] = g_array_new(FALSE, FALSE,
				sizeof(gfloat));
	} else if (count != builder->column_count) {
		gtkdialog_warning(
			"Invalid GtkDatabox row; expected %u columns but found %u.",
			builder->column_count, count);
		builder->invalid = TRUE;
		goto done;
	}
	for (column = 0; column < count; ++column) {
		g_strstrip(fields[column]);
		if (!widget_databox_parse_float(fields[column], &value)) {
			builder->invalid = TRUE;
			goto done;
		}
		g_array_append_val(builder->columns[column], value);
	}
	g_string_append(builder->text, copy);
	g_string_append_c(builder->text, '\n');

done:
	g_strfreev(fields);
	g_free(copy);
}

static gboolean widget_databox_builder_read_stream(DataboxBuilder *builder,
	FILE *stream)
{
	gchar *line;

	while ((line = widget_read_line(stream)) != NULL) {
		widget_databox_builder_append(builder, line);
		g_free(line);
	}
	return !builder->invalid;
}

static gboolean widget_databox_builder_read_file(DataboxBuilder *builder,
	const gchar *filename)
{
	FILE *stream;

	stream = fopen(filename, "r");
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return FALSE;
	}
	widget_databox_builder_read_stream(builder, stream);
	fclose(stream);
	return !builder->invalid;
}

static gboolean widget_databox_builder_read_command(DataboxBuilder *builder,
	const gchar *command)
{
	FILE *stream;

	stream = widget_opencommand(command);
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return FALSE;
	}
	widget_databox_builder_read_stream(builder, stream);
	widget_closecommand(stream, command);
	return !builder->invalid;
}

static DataboxPlot *widget_databox_builder_finish(DataboxBuilder *builder)
{
	DataboxPlot *plot;
	guint column;

	if (builder->invalid)
		return NULL;
	plot = g_new0(DataboxPlot, 1);
	plot->columns = builder->column_count;
	plot->length = builder->column_count > 0 ?
		builder->columns[0]->len : 0;
	plot->values = g_new0(gfloat *, plot->columns);
	for (column = 0; column < plot->columns; ++column) {
		plot->values[column] = (gfloat *)g_array_free(
			builder->columns[column], FALSE);
		builder->columns[column] = NULL;
	}
	plot->text = g_string_free(builder->text, FALSE);
	builder->text = NULL;
	g_free(builder->columns);
	builder->columns = NULL;
	builder->column_count = 0;
	return plot;
}

static gchar *widget_databox_list_value(const gchar *list, guint index,
	const gchar *fallback)
{
	gchar **values;
	gchar *result;
	guint count;

	if (list == NULL || list[0] == '\0')
		return g_strdup(fallback);
	values = g_strsplit(list, "|", -1);
	for (count = 0; values[count] != NULL; ++count)
		;
	if (count == 0)
		result = g_strdup(fallback);
	else {
		result = g_strdup(values[index % count]);
		g_strstrip(result);
		if (result[0] == '\0') {
			g_free(result);
			result = g_strdup(fallback);
		}
	}
	g_strfreev(values);
	return result;
}

static GdkColor widget_databox_series_color(DataboxData *data, guint index)
{
	static const gchar *defaults[] = {
		"#3584e4", "#e66100", "#33a02c", "#c01c28",
		"#9141ac", "#f5c211"
	};
	GdkColor color;
	gchar *value;

	value = widget_databox_list_value(data->series_colors, index,
		defaults[index % G_N_ELEMENTS(defaults)]);
	if (!gdk_color_parse(value, &color)) {
		gtkdialog_warning("Invalid GtkDatabox series colour '%s'; using %s.",
			value, defaults[index % G_N_ELEMENTS(defaults)]);
		gdk_color_parse(defaults[index % G_N_ELEMENTS(defaults)], &color);
	}
	g_free(value);
	return color;
}

static guint widget_databox_series_size(DataboxData *data, guint index)
{
	gchar *value;
	gint size;

	value = widget_databox_list_value(data->series_sizes, index, "1");
	size = widget_parse_bounded_integer(value, 1, 64, 1,
		"GtkDatabox series size");
	g_free(value);
	return (guint)size;
}

static GtkDataboxGraph *widget_databox_series_graph(DataboxData *data,
	DataboxPlot *plot, guint index)
{
	GdkColor color;
	GtkDataboxGraph *graph;
	gchar *type;
	guint size;

	type = widget_databox_list_value(data->series_types, index, "lines");
	color = widget_databox_series_color(data, index);
	size = widget_databox_series_size(data, index);
	if (g_ascii_strcasecmp(type, "points") == 0)
		graph = gtk_databox_points_new(plot->length, plot->values[0],
			plot->values[index + 1], &color, (gint)size);
	else if (g_ascii_strcasecmp(type, "bars") == 0)
		graph = gtk_databox_bars_new(plot->length, plot->values[0],
			plot->values[index + 1], &color, size);
	else {
		if (g_ascii_strcasecmp(type, "lines") != 0)
			gtkdialog_warning(
				"Invalid GtkDatabox series type '%s'; using lines.", type);
		graph = gtk_databox_lines_new(plot->length, plot->values[0],
			plot->values[index + 1], &color, size);
	}
	g_free(type);
	return graph;
}

static void widget_databox_log_limits(DataboxData *data, gboolean x_axis,
	gfloat border, gfloat *minimum, gfloat *maximum)
{
	gboolean have_positive = FALSE;
	gboolean have_nonpositive = FALSE;
	gfloat value;
	gdouble factor;
	guint column;
	guint row;

	for (column = x_axis ? 0 : 1;
		data->plot != NULL &&
		column < (x_axis ? 1 : data->plot->columns); ++column) {
		for (row = 0; row < data->plot->length; ++row) {
			value = data->plot->values[column][row];
			if (value <= 0.0f) {
				have_nonpositive = TRUE;
				continue;
			}
			if (!have_positive) {
				*minimum = value;
				*maximum = value;
				have_positive = TRUE;
			} else {
				*minimum = MIN(*minimum, value);
				*maximum = MAX(*maximum, value);
			}
		}
	}
	if (have_nonpositive)
		gtkdialog_warning(
			"GtkDatabox non-positive %c value ignored for logarithmic automatic limits.",
			x_axis ? 'X' : 'Y');
	if (!have_positive) {
		gtkdialog_warning(
			"GtkDatabox logarithmic %c axis has no positive values; using limits 1..10.",
			x_axis ? 'X' : 'Y');
		*minimum = 1.0f;
		*maximum = 10.0f;
		return;
	}
	if (*minimum == *maximum)
		factor = 1.0 + MAX(border, 0.05f);
	else
		factor = pow((gdouble)*maximum / *minimum, border);
	*minimum = (gfloat)(*minimum / factor);
	*maximum = (gfloat)(*maximum * factor);
}

static void widget_databox_linear_limits(gfloat border, gfloat *minimum,
	gfloat *maximum)
{
	gfloat span = *maximum - *minimum;

	if (span == 0.0f)
		span = *maximum;
	*minimum -= border * span;
	*maximum += border * span;
}

static void widget_databox_set_limits(GtkWidget *widget, DataboxData *data)
{
	GtkDatabox *box = GTK_DATABOX(widget);
	GtkDataboxScaleType scale_x;
	GtkDataboxScaleType scale_y;
	gfloat bottom;
	gfloat left;
	gfloat right;
	gfloat top;

	scale_x = gtk_databox_get_scale_type_x(box);
	scale_y = gtk_databox_get_scale_type_y(box);
	if (data->explicit_limits) {
		left = data->min_x;
		right = data->max_x;
		bottom = data->min_y;
		top = data->max_y;
		if (scale_x != GTK_DATABOX_SCALE_LINEAR && left <= 0.0f) {
			gtkdialog_warning(
				"GtkDatabox logarithmic X limits must be positive; using 1..10.");
			left = 1.0f;
			right = 10.0f;
		}
		if (scale_y != GTK_DATABOX_SCALE_LINEAR && bottom <= 0.0f) {
			gtkdialog_warning(
				"GtkDatabox logarithmic Y limits must be positive; using 1..10.");
			bottom = 1.0f;
			top = 10.0f;
		}
		gtk_databox_set_total_limits(box, left, right, top, bottom);
	} else if (data->auto_rescale) {
		if (scale_x == GTK_DATABOX_SCALE_LINEAR &&
			scale_y == GTK_DATABOX_SCALE_LINEAR) {
			gtk_databox_auto_rescale(box, data->border);
			return;
		}
		if (gtk_databox_calculate_extrema(box, &left, &right, &bottom,
			&top) != 0)
			return;
		if (scale_x != GTK_DATABOX_SCALE_LINEAR)
			widget_databox_log_limits(data, TRUE, data->border,
				&left, &right);
		else
			widget_databox_linear_limits(data->border, &left, &right);
		if (scale_y != GTK_DATABOX_SCALE_LINEAR)
			widget_databox_log_limits(data, FALSE, data->border,
				&bottom, &top);
		else
			widget_databox_linear_limits(data->border, &bottom, &top);
		gtk_databox_set_total_limits(box, left, right, top, bottom);
	}
}

static void widget_databox_prepare_log_scale(GtkWidget *widget,
	DataboxData *data)
{
	gfloat bottom;
	gfloat left;
	gfloat right;
	gfloat top;

	if (!data->log_scale_x && !data->log_scale_y)
		return;
	gtk_databox_get_total_limits(GTK_DATABOX(widget), &left, &right,
		&top, &bottom);
	if (data->log_scale_x)
		widget_databox_log_limits(data, TRUE, data->border, &left, &right);
	if (data->log_scale_y)
		widget_databox_log_limits(data, FALSE, data->border, &bottom, &top);
	gtk_databox_set_total_limits(GTK_DATABOX(widget), left, right, top,
		bottom);
}

static void widget_databox_realized(GtkWidget *widget, DataboxData *data)
{
	/* Generic properties such as scale-type-x and scale-type-y are applied by
	 * the shared realize handler. Refit afterwards so logarithmic scales do not
	 * inherit limits calculated while the widget still used its linear default. */
	data->updating = TRUE;
	widget_databox_set_limits(widget, data);
	data->updating = FALSE;
}

static void widget_databox_apply_plot(GtkWidget *widget, DataboxPlot *plot)
{
	DataboxData *data;
	GdkColor color;
	GList *graph_item;
	GtkDataboxGraph *graph;
	guint series;

	data = widget_databox_get_data(widget);
	data->updating = TRUE;
	for (series = 0; series + 1 < plot->columns; ++series) {
		graph = widget_databox_series_graph(data, plot, series);
		plot->graphs = g_list_append(plot->graphs, graph);
	}
	gtk_databox_graph_remove_all(GTK_DATABOX(widget));
	widget_databox_plot_free(data->plot);
	data->plot = plot;
	for (graph_item = plot->graphs; graph_item != NULL;
		graph_item = graph_item->next)
		gtk_databox_graph_add(GTK_DATABOX(widget), graph_item->data);
	widget_databox_set_limits(widget, data);
	if (data->show_grid) {
		color = data->grid_color;
		graph = gtk_databox_grid_new(data->grid_hlines,
			data->grid_vlines, &color, data->grid_width);
		gtk_databox_grid_set_line_style(GTK_DATABOX_GRID(graph),
			data->grid_style);
		plot->graphs = g_list_append(plot->graphs, graph);
		gtk_databox_graph_add(GTK_DATABOX(widget), graph);
	}
	g_free(data->selection);
	data->selection = NULL;
	data->updating = FALSE;
	gtk_widget_queue_draw(widget);
}

static gboolean widget_databox_parse_optional_float(tag_attr *attr,
	const gchar *name, gfloat *result)
{
	gchar *value;
	gdouble number;

	value = get_tag_attribute(attr, name);
	if (value == NULL)
		return FALSE;
	if (!widget_parse_finite_double(value, &number, name) ||
		number < -FLT_MAX || number > FLT_MAX)
		return FALSE;
	*result = (gfloat)number;
	return TRUE;
}

static gchar *widget_databox_duplicate_attribute(tag_attr *attr,
	const gchar *name)
{
	gchar *value = get_tag_attribute(attr, name);

	return value != NULL ? g_strdup(value) : NULL;
}

static gboolean widget_databox_attribute_is_log_scale(tag_attr *attr,
	const gchar *name)
{
	gboolean is_log;
	gchar *copy;
	gchar *value = get_tag_attribute(attr, name);

	if (value == NULL)
		return FALSE;
	copy = g_strdup(value);
	g_strstrip(copy);
	is_log = g_ascii_strcasecmp(copy, "log") == 0 ||
		g_ascii_strcasecmp(copy, "log2") == 0 ||
		g_ascii_strcasecmp(copy, "GTK_DATABOX_SCALE_LOG") == 0 ||
		g_ascii_strcasecmp(copy, "GTK_DATABOX_SCALE_LOG2") == 0 ||
		strcmp(copy, "1") == 0 || strcmp(copy, "2") == 0;
	g_free(copy);
	return is_log;
}

static void widget_databox_parse_attributes(DataboxData *data,
	tag_attr *attr)
{
	gboolean have_min_x;
	gboolean have_max_x;
	gboolean have_min_y;
	gboolean have_max_y;
	gchar *value;
	gdouble number;

	data->series_types = widget_databox_duplicate_attribute(attr,
		"series-types");
	data->series_colors = widget_databox_duplicate_attribute(attr,
		"series-colors");
	data->series_sizes = widget_databox_duplicate_attribute(attr,
		"series-sizes");
	data->log_scale_x = widget_databox_attribute_is_log_scale(attr,
		"scale-type-x");
	data->log_scale_y = widget_databox_attribute_is_log_scale(attr,
		"scale-type-y");
	data->auto_rescale = TRUE;
	data->border = 0.05f;
	data->show_grid = TRUE;
	data->grid_hlines = 5;
	data->grid_vlines = 5;
	data->grid_style = GTK_DATABOX_GRID_DOTTED_LINES;
	data->grid_width = 1;
	gdk_color_parse("#b0b0b0", &data->grid_color);

	value = get_tag_attribute(attr, "auto-rescale");
	if (value != NULL)
		data->auto_rescale = !widget_attribute_is_false(value);
	value = get_tag_attribute(attr, "show-grid");
	if (value != NULL)
		data->show_grid = !widget_attribute_is_false(value);
	value = get_tag_attribute(attr, "rescale-border");
	if (value != NULL && widget_parse_finite_double(value, &number,
		"GtkDatabox rescale-border")) {
		if (number >= 0.0 && number <= 1.0)
			data->border = (gfloat)number;
		else
			gtkdialog_warning(
				"GtkDatabox rescale-border '%s' is outside 0..1; using 0.05.",
				value);
	}
	value = get_tag_attribute(attr, "grid-horizontal-lines");
	if (value != NULL)
		data->grid_hlines = widget_parse_bounded_integer(value, 0, 100, 5,
			"GtkDatabox grid-horizontal-lines");
	value = get_tag_attribute(attr, "grid-vertical-lines");
	if (value != NULL)
		data->grid_vlines = widget_parse_bounded_integer(value, 0, 100, 5,
			"GtkDatabox grid-vertical-lines");
	value = get_tag_attribute(attr, "grid-line-width");
	if (value != NULL)
		data->grid_width = (guint)widget_parse_bounded_integer(value, 1, 64,
			1, "GtkDatabox grid-line-width");
	value = get_tag_attribute(attr, "grid-color");
	if (value != NULL && !gdk_color_parse(value, &data->grid_color)) {
		gtkdialog_warning("Invalid GtkDatabox grid colour '%s'; using #b0b0b0.",
			value);
		gdk_color_parse("#b0b0b0", &data->grid_color);
	}
	value = get_tag_attribute(attr, "grid-line-style");
	if (value != NULL) {
		if (g_ascii_strcasecmp(value, "dashed") == 0)
			data->grid_style = GTK_DATABOX_GRID_DASHED_LINES;
		else if (g_ascii_strcasecmp(value, "solid") == 0)
			data->grid_style = GTK_DATABOX_GRID_SOLID_LINES;
		else if (g_ascii_strcasecmp(value, "dotted") != 0)
			gtkdialog_warning(
				"Invalid GtkDatabox grid-line-style '%s'; using dotted.", value);
	}
	have_min_x = widget_databox_parse_optional_float(attr, "min-x",
		&data->min_x);
	have_max_x = widget_databox_parse_optional_float(attr, "max-x",
		&data->max_x);
	have_min_y = widget_databox_parse_optional_float(attr, "min-y",
		&data->min_y);
	have_max_y = widget_databox_parse_optional_float(attr, "max-y",
		&data->max_y);
	if (have_min_x || have_max_x || have_min_y || have_max_y) {
		if (have_min_x && have_max_x && have_min_y && have_max_y &&
			data->min_x < data->max_x && data->min_y < data->max_y)
			data->explicit_limits = TRUE;
		else
			gtkdialog_warning(
				"GtkDatabox limits require min-x < max-x and min-y < max-y; using automatic limits.");
	}
}

static void widget_databox_selection(GtkWidget *widget,
	GtkDataboxValueRectangle *selection, AttributeSet *attributes,
	const gchar *signal_name)
{
	DataboxData *data = widget_databox_get_data(widget);

	g_free(data->selection);
	data->selection = g_strdup_printf("%.9g|%.9g|%.9g|%.9g",
		selection->x1, selection->y1, selection->x2, selection->y2);
	widget_signal_executor(widget, attributes, signal_name);
}

static void widget_databox_selection_started(GtkWidget *widget,
	GtkDataboxValueRectangle *selection, AttributeSet *attributes)
{
	widget_databox_selection(widget, selection, attributes,
		"selection-started");
}

static void widget_databox_selection_changed(GtkWidget *widget,
	GtkDataboxValueRectangle *selection, AttributeSet *attributes)
{
	widget_databox_selection(widget, selection, attributes,
		"selection-changed");
}

static void widget_databox_selection_finalized(GtkWidget *widget,
	GtkDataboxValueRectangle *selection, AttributeSet *attributes)
{
	widget_databox_selection(widget, selection, attributes,
		"selection-finalized");
}

static void widget_databox_selection_canceled(GtkWidget *widget,
	AttributeSet *attributes)
{
	DataboxData *data = widget_databox_get_data(widget);

	g_free(data->selection);
	data->selection = NULL;
	widget_signal_executor(widget, attributes, "selection-canceled");
}

static void widget_databox_zoomed(GtkWidget *widget,
	AttributeSet *attributes)
{
	DataboxData *data = widget_databox_get_data(widget);

	if (data->updating)
		return;
	widget_signal_executor(widget, attributes, "zoomed");
}

#endif

GtkWidget *widget_databox_create(AttributeSet *attributes, tag_attr *attr,
	gint width, gint height)
{
	GdkColor background;
	gboolean have_background = FALSE;
	gchar *background_value;
	GtkWidget *widget;

	(void)attributes;
	if (!widget_databox_requested(attr))
		return NULL;
	background_value = get_tag_attribute(attr, "background");
	if (background_value != NULL) {
		have_background = gdk_color_parse(background_value, &background);
		if (!have_background)
			gtkdialog_warning(
				"Invalid GtkDatabox background '%s'; using theme colour.",
				background_value);
	}
#if HAVE_GTKDATABOX
	{
		DataboxData *data;

		widget = gtk_databox_new();
		data = g_new0(DataboxData, 1);
		widget_databox_parse_attributes(data, attr);
		widget_databox_kill_custom_attributes(attr);
		g_object_set_data_full(G_OBJECT(widget), DATABOX_DATA_KEY, data,
			widget_databox_data_free);
		g_signal_connect(G_OBJECT(widget), "destroy",
			G_CALLBACK(widget_databox_destroyed), data);
		g_signal_connect(G_OBJECT(widget), "realize",
			G_CALLBACK(widget_databox_prepare_log_scale), data);
		g_signal_connect_after(G_OBJECT(widget), "realize",
			G_CALLBACK(widget_databox_realized), data);
	}
#else
	widget_databox_kill_custom_attributes(attr);
	widget = gtk_drawing_area_new();
	gtkdialog_warning(
		"GtkDatabox drawing area requested, but this build does not include GtkDatabox 0.9 support.");
#endif
	g_object_set_data(G_OBJECT(widget), DATABOX_REQUESTED_KEY,
		GINT_TO_POINTER(TRUE));
	if (have_background)
		gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &background);
	gtk_widget_set_size_request(widget, width == -1 ? 480 : width,
		height == -1 ? 260 : height);
	return widget;
}

gboolean widget_databox_is_requested(GtkWidget *widget)
{
	return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
		DATABOX_REQUESTED_KEY));
}

gboolean widget_databox_is_widget(GtkWidget *widget)
{
#if HAVE_GTKDATABOX
	return GTK_IS_DATABOX(widget);
#else
	(void)widget;
	return FALSE;
#endif
}

void widget_databox_clear(variable *var)
{
#if HAVE_GTKDATABOX
	DataboxData *data;

	if (!GTK_IS_DATABOX(var->Widget))
		return;
	data = widget_databox_get_data(var->Widget);
	gtk_databox_graph_remove_all(GTK_DATABOX(var->Widget));
	widget_databox_plot_free(data->plot);
	data->plot = NULL;
	g_free(data->selection);
	data->selection = NULL;
	gtk_widget_queue_draw(var->Widget);
#else
	(void)var;
#endif
}

gchar *widget_databox_envvar_construct(GtkWidget *widget)
{
#if HAVE_GTKDATABOX
	DataboxData *data;

	if (!GTK_IS_DATABOX(widget))
		return g_strdup("");
	data = widget_databox_get_data(widget);
	return g_strdup(data->selection != NULL ? data->selection : "");
#else
	(void)widget;
	return g_strdup("");
#endif
}

void widget_databox_set_value(variable *var, const gchar *value)
{
#if HAVE_GTKDATABOX
	DataboxBuilder builder;
	DataboxPlot *plot;
	gchar **lines;
	guint line;

	if (!GTK_IS_DATABOX(var->Widget))
		return;
	widget_databox_builder_init(&builder);
	lines = g_strsplit(value, "\n", -1);
	for (line = 0; lines[line] != NULL; ++line)
		widget_databox_builder_append(&builder, lines[line]);
	g_strfreev(lines);
	plot = widget_databox_builder_finish(&builder);
	if (plot != NULL)
		widget_databox_apply_plot(var->Widget, plot);
	widget_databox_builder_clear(&builder);
#else
	(void)var;
	(void)value;
#endif
}

void widget_databox_refresh(variable *var)
{
#if HAVE_GTKDATABOX
	DataboxBuilder builder;
	DataboxPlot *plot;
	GList *element;
	gboolean initialised;
	gboolean valid = TRUE;
	gchar *input;
	gchar *item;

	if (!GTK_IS_DATABOX(var->Widget))
		return;
	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	widget_databox_builder_init(&builder);
	item = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (item != NULL) {
		widget_databox_builder_append(&builder, item);
		item = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL && valid) {
		if (input_is_shell_command(input))
			valid = widget_databox_builder_read_command(&builder, input + 8);
		else if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			valid = widget_databox_builder_read_file(&builder, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (valid) {
		plot = widget_databox_builder_finish(&builder);
		if (plot != NULL)
			widget_databox_apply_plot(var->Widget, plot);
	}
	widget_databox_builder_clear(&builder);
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		fprintf(stderr, "%s(): <default> not implemented for GtkDatabox.\n",
			__func__);
	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr, "%s(): <label> not implemented for GtkDatabox.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(G_OBJECT(var->Widget), "selection-started",
		G_CALLBACK(widget_databox_selection_started), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "selection-changed",
		G_CALLBACK(widget_databox_selection_changed), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "selection-finalized",
		G_CALLBACK(widget_databox_selection_finalized), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "selection-canceled",
		G_CALLBACK(widget_databox_selection_canceled), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "zoomed",
		G_CALLBACK(widget_databox_zoomed), var->Attributes);
#else
	(void)var;
#endif
}

void widget_databox_save(variable *var)
{
#if HAVE_GTKDATABOX
	DataboxData *data;
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;

	if (!GTK_IS_DATABOX(var->Widget))
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
	data = widget_databox_get_data(var->Widget);
	output = fopen(filename, "w");
	if (output == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}
	if (data->plot != NULL && data->plot->text != NULL)
		fputs(data->plot->text, output);
	widget_close_output(output, filename);
#else
	(void)var;
#endif
}

void widget_databox_zoom_in(variable *var)
{
#if HAVE_GTKDATABOX
	if (GTK_IS_DATABOX(var->Widget))
		gtk_databox_zoom_to_selection(GTK_DATABOX(var->Widget));
#else
	(void)var;
#endif
}

void widget_databox_zoom_out(variable *var)
{
#if HAVE_GTKDATABOX
	if (GTK_IS_DATABOX(var->Widget))
		gtk_databox_zoom_out(GTK_DATABOX(var->Widget));
#else
	(void)var;
#endif
}

void widget_databox_zoom_reset(variable *var)
{
#if HAVE_GTKDATABOX
	if (GTK_IS_DATABOX(var->Widget))
		gtk_databox_zoom_home(GTK_DATABOX(var->Widget));
#else
	(void)var;
#endif
}
