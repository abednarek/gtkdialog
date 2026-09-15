/*
 * widget_curve.c: GtkCurve widget.
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
#include "widget_curve.h"

#define CURVE_DEFAULT_SAMPLES 32
#define CURVE_MIN_SAMPLES 2
#define CURVE_MAX_SAMPLES 4096
#define CURVE_SAMPLES_KEY "gtkdialog-curve-samples"
#define CURVE_SAMPLES_EXPLICIT_KEY "gtkdialog-curve-samples-explicit"
#define CURVE_PENDING_KEY "gtkdialog-curve-pending"
#define CURVE_ATTRIBUTES_KEY "gtkdialog-curve-attributes"
#define CURVE_TYPE_KEY "gtkdialog-curve-type"

typedef struct {
	GtkWidget *owner;
	GtkCurveType curve_type;
	gboolean curve_type_set;
	gboolean gamma_set;
	gfloat gamma;
	gint vector_length;
	gfloat *vector;
} CurvePending;

static void widget_curve_type_changed(
	GtkWidget *widget, GtkWidget *owner);

static GtkWidget *
widget_curve_inner(GtkWidget *widget)
{
	if (GTK_IS_GAMMA_CURVE(widget))
		return GTK_GAMMA_CURVE(widget)->curve;
	return widget;
}

static void
widget_curve_event_after(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	AttributeSet *attributes;
	GtkWidget *owner;

	(void)widget;
	if (event->type != GDK_BUTTON_RELEASE)
		return;

	/* Keep the widget and its attached AttributeSet alive if an action
	 * synchronously destroys the containing window. */
	owner = data;
	g_object_ref(owner);
	attributes = g_object_get_data(G_OBJECT(owner), CURVE_ATTRIBUTES_KEY);
	widget_signal_executor(owner, attributes, "button-release-event");
	widget_signal_executor(owner, attributes, "changed");
	g_object_unref(owner);
}

static void
widget_curve_pending_destroy(gpointer data)
{
	CurvePending *pending;

	pending = data;
	g_free(pending->vector);
	g_free(pending);
}

static gboolean
widget_curve_configure(GtkWidget *widget, GdkEventConfigure *event,
	gpointer data)
{
	CurvePending *pending;

	(void)event;
	(void)data;
	pending = g_object_get_data(G_OBJECT(widget), CURVE_PENDING_KEY);
	if (pending == NULL)
		return FALSE;
	if (GTK_CURVE(widget)->num_points < CURVE_MIN_SAMPLES ||
		GTK_CURVE(widget)->height < CURVE_MIN_SAMPLES)
		return FALSE;
	if (pending->gamma_set)
		gtk_curve_set_gamma(GTK_CURVE(widget), pending->gamma);
	if (pending->gamma_set && GTK_IS_GAMMA_CURVE(pending->owner))
		GTK_GAMMA_CURVE(pending->owner)->gamma = pending->gamma;
	if (pending->vector != NULL)
		gtk_curve_set_vector(GTK_CURVE(widget), pending->vector_length,
			pending->vector);
	if (pending->curve_type_set)
		gtk_curve_set_curve_type(GTK_CURVE(widget), pending->curve_type);
	g_signal_connect(G_OBJECT(widget), "curve-type-changed",
		G_CALLBACK(widget_curve_type_changed), pending->owner);
	g_object_set_data(G_OBJECT(widget), CURVE_PENDING_KEY, NULL);
	return FALSE;
}

static gint
widget_curve_get_samples(GtkWidget *widget)
{
	gint samples;

	samples = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), CURVE_SAMPLES_KEY));
	return samples >= CURVE_MIN_SAMPLES ? samples : CURVE_DEFAULT_SAMPLES;
}

static void
widget_curve_set_samples(GtkWidget *widget, gint samples)
{
	g_object_set_data(G_OBJECT(widget), CURVE_SAMPLES_KEY,
		GINT_TO_POINTER(samples));
}

static void
widget_curve_resample(const gfloat *input, gint input_count,
	gfloat *output, gint output_count)
{
	gdouble fraction;
	gdouble position;
	gint index;
	gint left;

	for (index = 0; index < output_count; ++index) {
		position = (gdouble)(input_count - 1) * index / (output_count - 1);
		left = (gint)position;
		if (left >= input_count - 1)
			output[index] = input[input_count - 1];
		else {
			fraction = position - left;
			output[index] = input[left] * (1.0 - fraction) +
				input[left + 1] * fraction;
		}
	}
}

static gfloat
widget_curve_linear_endpoint(GtkCurve *curve)
{
	gfloat previous_x;
	gfloat previous_y;
	gfloat x;
	gfloat y;
	gfloat value;
	gboolean have_previous;
	gint index;

	previous_x = curve->min_x - 1.0;
	previous_y = curve->min_y;
	have_previous = FALSE;
	for (index = 0; index < curve->num_ctlpoints; ++index) {
		x = curve->ctlpoint[index][0];
		y = curve->ctlpoint[index][1];
		if (x <= previous_x)
			continue;
		if (x == curve->max_x)
			return CLAMP(y, curve->min_y, curve->max_y);
		if (x > curve->max_x) {
			if (!have_previous || previous_x > curve->max_x)
				return curve->min_y;
			value = previous_y + (curve->max_x - previous_x) *
				(y - previous_y) / (x - previous_x);
			return CLAMP(value, curve->min_y, curve->max_y);
		}
		previous_x = x;
		previous_y = y;
		have_previous = TRUE;
	}
	return curve->min_y;
}

static gboolean
widget_curve_set_value(GtkWidget *widget, const gchar *text,
	const gchar *source)
{
	gchar **fields;
	gfloat *vector;
	gdouble number;
	gboolean valid;
	gint count;
	gint index;
	gint curve_type;
	CurvePending *pending;
	GtkWidget *curve;

	curve = widget_curve_inner(widget);

	fields = g_strsplit(text, "|", -1);
	for (count = 0; fields[count] != NULL; ++count)
		;
	if (count < CURVE_MIN_SAMPLES || count > CURVE_MAX_SAMPLES) {
		gtkdialog_warning(
			"Invalid %s value; expected 2..4096 pipe-separated samples.",
			source);
		g_strfreev(fields);
		return FALSE;
	}

	vector = g_new(gfloat, count);
	valid = TRUE;
	for (index = 0; index < count; ++index) {
		if (!widget_parse_finite_double(g_strstrip(fields[index]),
			&number, source)) {
			valid = FALSE;
			break;
		}
		if (number < GTK_CURVE(curve)->min_y ||
			number > GTK_CURVE(curve)->max_y) {
			gtkdialog_warning(
				"Invalid %s sample '%s'; expected a value in the %g..%g range.",
				source, fields[index], GTK_CURVE(curve)->min_y,
				GTK_CURVE(curve)->max_y);
			valid = FALSE;
			break;
		}
		vector[index] = (gfloat)number;
	}
	if (valid) {
		if (GTK_CURVE(curve)->num_points >= CURVE_MIN_SAMPLES) {
			gtk_curve_set_vector(GTK_CURVE(curve), count, vector);
			curve_type = GPOINTER_TO_INT(g_object_get_data(
				G_OBJECT(widget), CURVE_TYPE_KEY));
			if (curve_type > 0)
				gtk_curve_set_curve_type(GTK_CURVE(curve),
					(GtkCurveType)(curve_type - 1));
		} else {
			pending = g_object_get_data(G_OBJECT(curve), CURVE_PENDING_KEY);
			if (pending != NULL) {
				g_free(pending->vector);
				pending->vector = vector;
				pending->vector_length = count;
				vector = NULL;
			}
		}
		if (!GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
			CURVE_SAMPLES_EXPLICIT_KEY)))
			widget_curve_set_samples(widget, count);
	}
	g_free(vector);
	g_strfreev(fields);
	return valid;
}

static gchar *
widget_curve_first_output_filename(AttributeSet *attributes)
{
	GList *element;
	gchar *directive;

	directive = attributeset_get_first(&element, attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (g_ascii_strncasecmp(directive, "file:", 5) == 0 &&
			strlen(directive) > 5)
			return directive + 5;
		directive = attributeset_get_next(&element, attributes, ATTR_OUTPUT);
	}
	return NULL;
}

static void
widget_curve_input_by_command(variable *var, const gchar *command)
{
	FILE *input;
	gchar *line;

	input = widget_opencommand(command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	line = widget_read_line(input);
	if (line != NULL) {
		widget_curve_set_value(var->Widget, line, "curve command input");
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void
widget_curve_input_by_file(variable *var, const gchar *filename)
{
	FILE *input;
	gchar *line;

	input = fopen(filename, "r");
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
		return;
	}
	line = widget_read_line(input);
	if (line != NULL) {
		widget_curve_set_value(var->Widget, line, "curve file input");
		g_free(line);
	}
	fclose(input);
}

static GtkCurveType
widget_curve_parse_type(const gchar *value, GtkCurveType fallback)
{
	gchar *copy;
	GtkCurveType result;

	copy = g_strdup(value);
	g_strstrip(copy);
	if (g_ascii_strcasecmp(copy, "linear") == 0)
		result = GTK_CURVE_TYPE_LINEAR;
	else if (g_ascii_strcasecmp(copy, "spline") == 0)
		result = GTK_CURVE_TYPE_SPLINE;
	else if (g_ascii_strcasecmp(copy, "free") == 0)
		result = GTK_CURVE_TYPE_FREE;
	else
		result = (GtkCurveType)widget_parse_bounded_integer(
			copy, GTK_CURVE_TYPE_LINEAR, GTK_CURVE_TYPE_FREE,
			fallback, "curve-type");
	g_free(copy);
	return result;
}

static void
widget_curve_type_changed(GtkWidget *widget, GtkWidget *owner)
{
	AttributeSet *attributes;

	(void)widget;
	g_object_ref(owner);
	attributes = g_object_get_data(G_OBJECT(owner), CURVE_ATTRIBUTES_KEY);
	widget_signal_executor(owner, attributes, "curve-type-changed");
	g_object_unref(owner);
}

void
widget_curve_clear(variable *var)
{
	CurvePending *pending;
	GtkWidget *curve;

	curve = widget_curve_inner(var->Widget);
	pending = g_object_get_data(G_OBJECT(curve), CURVE_PENDING_KEY);
	if (pending != NULL) {
		g_free(pending->vector);
		pending->vector = NULL;
		pending->vector_length = 0;
		pending->gamma_set = FALSE;
	}
	gtk_curve_reset(GTK_CURVE(curve));
}

GtkWidget *
widget_curve_create(AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;
	GtkWidget *curve;
	gchar *text;
	gdouble min_x;
	gdouble max_x;
	gdouble min_y;
	gdouble max_y;
	gdouble value;
	gboolean valid_range;
	gint samples;
	GtkCurveType curve_type;
	CurvePending *pending;
	gboolean controls;

	(void)Type;
	controls = FALSE;
	if (attr != NULL &&
		(text = get_tag_attribute(attr, "controls")) != NULL) {
		if (!widget_parse_boolean(text, &controls))
			gtkdialog_warning(
				"Invalid controls value '%s'; using false.", text);
		kill_tag_attribute(attr, "controls");
	}
	widget = controls ? gtk_gamma_curve_new() : gtk_curve_new();
	curve = widget_curve_inner(widget);
	pending = g_new0(CurvePending, 1);
	pending->owner = widget;
	min_x = GTK_CURVE(curve)->min_x;
	max_x = GTK_CURVE(curve)->max_x;
	min_y = GTK_CURVE(curve)->min_y;
	max_y = GTK_CURVE(curve)->max_y;
	curve_type = GTK_CURVE(curve)->curve_type;
	samples = CURVE_DEFAULT_SAMPLES;
	valid_range = TRUE;

	if (attr != NULL) {
		if ((text = get_tag_attribute(attr, "curve-type")) != NULL) {
			curve_type = widget_curve_parse_type(text, curve_type);
			pending->curve_type_set = TRUE;
			g_object_set_data(G_OBJECT(widget), CURVE_TYPE_KEY,
				GINT_TO_POINTER(curve_type + 1));
			kill_tag_attribute(attr, "curve-type");
		}
		if ((text = get_tag_attribute(attr, "min-x")) != NULL) {
			valid_range &= widget_parse_finite_double(text, &min_x, "min-x");
			kill_tag_attribute(attr, "min-x");
		}
		if ((text = get_tag_attribute(attr, "max-x")) != NULL) {
			valid_range &= widget_parse_finite_double(text, &max_x, "max-x");
			kill_tag_attribute(attr, "max-x");
		}
		if ((text = get_tag_attribute(attr, "min-y")) != NULL) {
			valid_range &= widget_parse_finite_double(text, &min_y, "min-y");
			kill_tag_attribute(attr, "min-y");
		}
		if ((text = get_tag_attribute(attr, "max-y")) != NULL) {
			valid_range &= widget_parse_finite_double(text, &max_y, "max-y");
			kill_tag_attribute(attr, "max-y");
		}
		if ((text = get_tag_attribute(attr, "samples")) != NULL) {
			samples = widget_parse_bounded_integer(text,
				CURVE_MIN_SAMPLES, CURVE_MAX_SAMPLES,
				CURVE_DEFAULT_SAMPLES, "samples");
			g_object_set_data(G_OBJECT(widget),
				CURVE_SAMPLES_EXPLICIT_KEY, GINT_TO_POINTER(1));
			kill_tag_attribute(attr, "samples");
		}
	}

	if (valid_range && min_x < max_x && min_y < max_y)
		gtk_curve_set_range(GTK_CURVE(curve), min_x, max_x, min_y, max_y);
	else if (valid_range)
		gtkdialog_warning(
			"Invalid curve range; min-x/min-y must be less than max-x/max-y.");
	widget_curve_set_samples(widget, samples);
	gtk_widget_set_size_request(curve, 256, 160);
	pending->curve_type = curve_type;

	if (attr != NULL &&
		(text = get_tag_attribute(attr, "gamma")) != NULL) {
		if (widget_parse_finite_double(text, &value, "gamma")) {
			if (value > 0.0) {
				pending->gamma_set = TRUE;
				pending->gamma = (gfloat)value;
			}
			else
				gtkdialog_warning(
					"Invalid gamma value '%s'; expected a positive number.",
					text);
		}
		kill_tag_attribute(attr, "gamma");
	}
	g_object_set_data_full(G_OBJECT(curve), CURVE_PENDING_KEY, pending,
		widget_curve_pending_destroy);
	g_object_set_data(G_OBJECT(widget), CURVE_ATTRIBUTES_KEY, Attr);
	g_signal_connect_after(G_OBJECT(curve), "configure-event",
		G_CALLBACK(widget_curve_configure), NULL);
	g_signal_connect(G_OBJECT(curve), "event-after",
		G_CALLBACK(widget_curve_event_after), widget);
	return widget;
}

gchar *
widget_curve_envvar_construct(GtkWidget *widget)
{
	gchar number[G_ASCII_DTOSTR_BUF_SIZE];
	gfloat *points;
	gfloat *vector;
	GString *value;
	gint samples;
	gint point_count;
	gint index;
	CurvePending *pending;
	GtkWidget *curve;

	curve = widget_curve_inner(widget);
	samples = widget_curve_get_samples(widget);
	vector = g_new(gfloat, samples);
	pending = g_object_get_data(G_OBJECT(curve), CURVE_PENDING_KEY);
	point_count = GTK_CURVE(curve)->num_points;
	if (pending != NULL && pending->vector != NULL)
		widget_curve_resample(pending->vector, pending->vector_length,
			vector, samples);
	else if (point_count >= CURVE_MIN_SAMPLES &&
		GTK_CURVE(curve)->curve_type == GTK_CURVE_TYPE_FREE) {
		points = g_new(gfloat, point_count);
		gtk_curve_get_vector(GTK_CURVE(curve), point_count, points);
		widget_curve_resample(points, point_count, vector, samples);
		g_free(points);
	} else if (point_count >= CURVE_MIN_SAMPLES) {
		gtk_curve_get_vector(GTK_CURVE(curve), samples, vector);
		if (GTK_CURVE(curve)->curve_type == GTK_CURVE_TYPE_LINEAR)
			vector[samples - 1] =
				widget_curve_linear_endpoint(GTK_CURVE(curve));
	} else
		gtk_curve_get_vector(GTK_CURVE(curve), samples, vector);
	value = g_string_new(NULL);
	for (index = 0; index < samples; ++index) {
		if (index > 0)
			g_string_append_c(value, '|');
		g_ascii_dtostr(number, sizeof(number), vector[index]);
		g_string_append(value, number);
	}
	g_free(vector);
	return g_string_free(value, FALSE);
}

void
widget_curve_fileselect(variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void
widget_curve_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_curve_input_by_command(var, input + 8);
		else if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_curve_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}

	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_curve_set_value(var->Widget,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT), "curve default");
	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
			__func__);
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
}

void
widget_curve_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);
}

void
widget_curve_save(variable *var)
{
	FILE *output;
	gchar *filename;
	gchar *value;

	filename = widget_curve_first_output_filename(var->Attributes);
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
	value = widget_curve_envvar_construct(var->Widget);
	fputs(value, output);
	g_free(value);
	widget_close_output(output, filename);
}
