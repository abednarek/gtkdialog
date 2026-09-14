/*
 * widget_scrollbar.c: GtkScrollbar widgets.
 * Gtkdialog - A small utility for fast and easy GUI building.
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
 */

#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <math.h>

#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_scrollbar.h"

static gboolean
widget_scrollbar_parse_value(const gchar *text, gdouble *value,
	const gchar *source)
{
	return widget_parse_finite_double(text, value, source);
}

static void
widget_scrollbar_set_value(GtkWidget *widget, const gchar *text,
	const gchar *source)
{
	gdouble value;

	if (widget_scrollbar_parse_value(text, &value, source))
		gtk_range_set_value(GTK_RANGE(widget), value);
}

static gchar *
widget_scrollbar_first_output_filename(AttributeSet *attributes)
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
widget_scrollbar_input_by_command(variable *var, const gchar *command)
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
		widget_scrollbar_set_value(var->Widget, line,
			"scrollbar command input");
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void
widget_scrollbar_input_by_file(variable *var, const gchar *filename)
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
		widget_scrollbar_set_value(var->Widget, line,
			"scrollbar file input");
		g_free(line);
	}
	fclose(input);
}

static void
widget_scrollbar_adjust_bounds(GtkRange *range, gdouble value,
	AttributeSet *attributes)
{
	(void)value;
	widget_signal_executor(GTK_WIDGET(range), attributes, "adjust-bounds");
}

static gboolean
widget_scrollbar_change_value(GtkRange *range, GtkScrollType scroll,
	gdouble value, AttributeSet *attributes)
{
	(void)scroll;
	(void)value;
	widget_signal_executor(GTK_WIDGET(range), attributes, "change-value");
	return FALSE;
}

static void
widget_scrollbar_move_slider(GtkRange *range, GtkScrollType scroll,
	AttributeSet *attributes)
{
	(void)scroll;
	widget_signal_executor(GTK_WIDGET(range), attributes, "move-slider");
}

void
widget_scrollbar_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *
widget_scrollbar_create(AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkAdjustment *adjustment;
	GtkWidget *widget;
	gchar *text;
	gdouble lower = 0.0;
	gdouble upper = 100.0;
	gdouble step = 1.0;
	gdouble page = 10.0;
	gdouble page_size = 0.0;
	gdouble value = 0.0;
	gdouble span;

	(void)Attr;
	if (attr != NULL) {
		if ((text = get_tag_attribute(attr, "range-min")) != NULL)
			widget_scrollbar_parse_value(text, &lower,
				"scrollbar range-min");
		if ((text = get_tag_attribute(attr, "range-max")) != NULL)
			widget_scrollbar_parse_value(text, &upper,
				"scrollbar range-max");
		if ((text = get_tag_attribute(attr, "range-step")) != NULL)
			widget_scrollbar_parse_value(text, &step,
				"scrollbar range-step");
		if ((text = get_tag_attribute(attr, "range-page")) != NULL)
			widget_scrollbar_parse_value(text, &page,
				"scrollbar range-page");
		if ((text = get_tag_attribute(attr, "range-page-size")) != NULL)
			widget_scrollbar_parse_value(text, &page_size,
				"scrollbar range-page-size");
		if ((text = get_tag_attribute(attr, "range-value")) != NULL)
			widget_scrollbar_parse_value(text, &value,
				"scrollbar range-value");
	}

	span = upper - lower;
	if (lower >= upper || !isfinite(span)) {
		gtkdialog_warning("Invalid scrollbar range; using 0..100.");
		lower = 0.0;
		upper = 100.0;
		span = 100.0;
	}
	if (step <= 0.0) {
		gtkdialog_warning("Invalid scrollbar range-step; using 1.");
		step = 1.0;
	}
	if (page <= 0.0) {
		gtkdialog_warning("Invalid scrollbar range-page; using 10.");
		page = 10.0;
	}
	if (page_size < 0.0 || page_size > span) {
		gtkdialog_warning("Invalid scrollbar range-page-size; using 0.");
		page_size = 0.0;
	}

	adjustment = GTK_ADJUSTMENT(gtk_adjustment_new(value, lower, upper,
		step, page, page_size));
	if (Type == WIDGET_HSCROLLBAR)
		widget = gtk_hscrollbar_new(adjustment);
	else
		widget = gtk_vscrollbar_new(adjustment);
	return widget;
}

gchar *
widget_scrollbar_envvar_construct(GtkWidget *widget)
{
	gchar value[G_ASCII_DTOSTR_BUF_SIZE];

	g_ascii_dtostr(value, sizeof(value),
		gtk_range_get_value(GTK_RANGE(widget)));
	return g_strdup(value);
}

void
widget_scrollbar_fileselect(variable *var, const char *name,
	const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void
widget_scrollbar_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_scrollbar_input_by_command(var, input + 8);
		else if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_scrollbar_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}

	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_scrollbar_set_value(var->Widget,
			attributeset_get_first(&element,
				var->Attributes, ATTR_DEFAULT),
			"scrollbar default");
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

	g_signal_connect(G_OBJECT(var->Widget), "value-changed",
		G_CALLBACK(on_any_widget_value_changed_event), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "adjust-bounds",
		G_CALLBACK(widget_scrollbar_adjust_bounds), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "change-value",
		G_CALLBACK(widget_scrollbar_change_value), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "move-slider",
		G_CALLBACK(widget_scrollbar_move_slider), var->Attributes);
}

void
widget_scrollbar_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);
}

void
widget_scrollbar_save(variable *var)
{
	FILE *output;
	gchar *filename;
	gchar *value;

	filename = widget_scrollbar_first_output_filename(var->Attributes);
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
	value = widget_scrollbar_envvar_construct(var->Widget);
	fputs(value, output);
	g_free(value);
	widget_close_output(output, filename);
}
