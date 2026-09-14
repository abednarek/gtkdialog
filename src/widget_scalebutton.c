/*
 * widget_scalebutton.c:
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
#include <math.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_scalebutton.h"

#if GTK_CHECK_VERSION(2,12,0)
static void widget_scalebutton_validate_range(
	gdouble *minimum, gdouble *maximum, gdouble *step)
{
	if (*minimum >= *maximum || !isfinite(*maximum - *minimum)) {
		gtkdialog_warning("Invalid scale button range; using 0..100.");
		*minimum = 0.0;
		*maximum = 100.0;
	}
	if (*step <= 0.0) {
		gtkdialog_warning("Invalid scale button range-step; using 1.");
		*step = 1.0;
	}
}

static gchar **widget_scalebutton_parse_icons(const gchar *value)
{
	gchar **icons;
	gint input;
	gint output = 0;

	if (value == NULL)
		return NULL;
	icons = g_strsplit_set(value, ";,", -1);
	for (input = 0; icons[input] != NULL; ++input) {
		g_strstrip(icons[input]);
		if (icons[input][0] == '\0') {
			g_free(icons[input]);
			icons[input] = NULL;
			continue;
		}
		if (output != input) {
			icons[output] = icons[input];
			icons[input] = NULL;
		}
		++output;
	}
	icons[output] = NULL;
	if (output == 0) {
		g_strfreev(icons);
		return NULL;
	}
	return icons;
}

static void widget_scalebutton_set_value(
	GtkWidget *widget, const gchar *text, const gchar *source)
{
	gdouble value;

	if (widget_parse_finite_double(text, &value, source))
		gtk_scale_button_set_value(GTK_SCALE_BUTTON(widget), value);
}

static void widget_scalebutton_input_by_command(
	variable *var, const gchar *command)
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
		widget_scalebutton_set_value(var->Widget, line,
			"scale button command input");
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_scalebutton_input_by_file(
	variable *var, const gchar *filename)
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
		widget_scalebutton_set_value(var->Widget, line,
			"scale button file input");
		g_free(line);
	}
	fclose(input);
}

static void widget_scalebutton_value_changed(
	GtkWidget *widget, gdouble value, AttributeSet *Attr)
{
	(void)value;
	widget_signal_executor(widget, Attr, "value-changed");
}

void widget_scalebutton_clear(variable *var)
{
	GtkAdjustment *adjustment;

	adjustment = gtk_scale_button_get_adjustment(
		GTK_SCALE_BUTTON(var->Widget));
	gtk_scale_button_set_value(GTK_SCALE_BUTTON(var->Widget),
		adjustment->lower);
}

GtkWidget *widget_scalebutton_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkIconSize icon_size = GTK_ICON_SIZE_SMALL_TOOLBAR;
	GtkWidget *widget;
	gchar **icons = NULL;
	gchar *value;
	gdouble initial = 0.0;
	gdouble maximum = 100.0;
	gdouble minimum = 0.0;
	gdouble step = 1.0;
	gint digits = 2;

	(void)Attr;
	if (attr != NULL) {
		value = get_tag_attribute(attr, "range-min");
		if (value == NULL)
			value = get_tag_attribute(attr, "scale-min");
		if (value != NULL)
			widget_parse_finite_double(value, &minimum,
				"scale button range-min");
		value = get_tag_attribute(attr, "range-max");
		if (value == NULL)
			value = get_tag_attribute(attr, "scale-max");
		if (value != NULL)
			widget_parse_finite_double(value, &maximum,
				"scale button range-max");
		value = get_tag_attribute(attr, "range-step");
		if (value == NULL)
			value = get_tag_attribute(attr, "scale-step");
		if (value != NULL)
			widget_parse_finite_double(value, &step,
				"scale button range-step");
		value = get_tag_attribute(attr, "range-value");
		if (value == NULL)
			value = get_tag_attribute(attr, "scale-value");
		if (value != NULL)
			widget_parse_finite_double(value, &initial,
				"scale button range-value");
		value = get_tag_attribute(attr, "digits");
		if (value != NULL)
			digits = widget_parse_bounded_integer(value, 0, 16, 2,
				"scale button digits");
		value = get_tag_attribute(attr, "icon-size");
		if (value != NULL)
			icon_size = widget_parse_stock_icon_size(value,
				GTK_ICON_SIZE_SMALL_TOOLBAR, "scale button icon-size");
		icons = widget_scalebutton_parse_icons(
			get_tag_attribute(attr, "icons"));
	}

	if (Type == WIDGET_VOLUMEBUTTON) {
		widget = gtk_volume_button_new();
	} else {
		widget_scalebutton_validate_range(&minimum, &maximum, &step);
		widget = gtk_scale_button_new(icon_size,
			minimum, maximum, step, (const gchar **)icons);
	}
	if (icons != NULL) {
		if (Type == WIDGET_VOLUMEBUTTON)
			gtk_scale_button_set_icons(GTK_SCALE_BUTTON(widget),
				(const gchar **)icons);
		g_strfreev(icons);
	}
	gtk_scale_button_set_value(GTK_SCALE_BUTTON(widget), initial);
	g_object_set_data(G_OBJECT(widget), "gtkdialog-digits",
		GINT_TO_POINTER(digits + 1));
	if (attr != NULL) {
		kill_tag_attribute(attr, "range-min");
		kill_tag_attribute(attr, "range-max");
		kill_tag_attribute(attr, "range-step");
		kill_tag_attribute(attr, "range-value");
		kill_tag_attribute(attr, "scale-min");
		kill_tag_attribute(attr, "scale-max");
		kill_tag_attribute(attr, "scale-step");
		kill_tag_attribute(attr, "scale-value");
		kill_tag_attribute(attr, "digits");
		kill_tag_attribute(attr, "icon-size");
		kill_tag_attribute(attr, "icons");
	}
	return widget;
}

gchar *widget_scalebutton_envvar_construct(GtkWidget *widget)
{
	gdouble value;
	gint digits;

	digits = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), "gtkdialog-digits")) - 1;
	if (digits < 0 || digits > 16)
		digits = 2;
	value = gtk_scale_button_get_value(GTK_SCALE_BUTTON(widget));
	return g_strdup_printf("%.*f", digits, value);
}

void widget_scalebutton_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_scalebutton_set_value(var->Widget, value,
		"scale button selected value");
}

void widget_scalebutton_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_scalebutton_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_scalebutton_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_scalebutton_set_value(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			"scale button default");
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
		G_CALLBACK(widget_scalebutton_value_changed), var->Attributes);
}

void widget_scalebutton_removeselected(variable *var)
{
	widget_scalebutton_clear(var);
}

void widget_scalebutton_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	gchar *value;

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
	value = widget_scalebutton_envvar_construct(var->Widget);
	fputs(value, output);
	g_free(value);
	widget_close_output(output, filename);
}
#endif
