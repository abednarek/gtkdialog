/*
 * widget_calendar.c:
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
#include "widget_calendar.h"

static gboolean widget_calendar_set_date(GtkWidget *widget, const gchar *value)
{
	const gchar *end;
	const gchar *start;
	guint day, month, year;

	start = value;
	if (start != NULL) {
		while (g_ascii_isspace(*start))
			++start;
		end = start + strlen(start);
		while (end > start && g_ascii_isspace(end[-1]))
			--end;
	}
	if (start == NULL || end - start != 10 ||
		!g_ascii_isdigit(start[0]) || !g_ascii_isdigit(start[1]) ||
		!g_ascii_isdigit(start[2]) || !g_ascii_isdigit(start[3]) ||
		start[4] != '-' ||
		!g_ascii_isdigit(start[5]) || !g_ascii_isdigit(start[6]) ||
		start[7] != '-' ||
		!g_ascii_isdigit(start[8]) || !g_ascii_isdigit(start[9])) {
		gtkdialog_warning("Invalid calendar date '%s'; expected YYYY-MM-DD.",
			value != NULL ? value : "");
		return FALSE;
	}

	year = (start[0] - '0') * 1000 + (start[1] - '0') * 100 +
		(start[2] - '0') * 10 + start[3] - '0';
	month = (start[5] - '0') * 10 + start[6] - '0';
	day = (start[8] - '0') * 10 + start[9] - '0';
	if (month < 1 || month > 12 ||
		day < 1 || day > 31 ||
		!g_date_valid_dmy((GDateDay)day, (GDateMonth)month,
			(GDateYear)year)) {
		gtkdialog_warning("Invalid calendar date '%s'; expected YYYY-MM-DD.",
			value != NULL ? value : "");
		return FALSE;
	}

	gtk_calendar_select_month(GTK_CALENDAR(widget), month - 1, year);
	gtk_calendar_select_day(GTK_CALENDAR(widget), day);
	return TRUE;
}

static void widget_calendar_apply_display_option(GtkWidget *widget,
	tag_attr *attr, const gchar *name, GtkCalendarDisplayOptions option)
{
	GtkCalendarDisplayOptions options;
	gboolean enabled;
	gchar *value;

	if (attr == NULL)
		return;
	value = get_tag_attribute(attr, name);
	if (value == NULL)
		return;
	if (!widget_parse_boolean(value, &enabled)) {
		gtkdialog_warning("Invalid %s value '%s'; option ignored.", name,
			value);
		kill_tag_attribute(attr, name);
		return;
	}

	options = gtk_calendar_get_display_options(GTK_CALENDAR(widget));
	if (enabled)
		options |= option;
	else
		options &= ~option;
	gtk_calendar_set_display_options(GTK_CALENDAR(widget), options);
	kill_tag_attribute(attr, name);
}

static void widget_calendar_input_by_command(
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
		widget_calendar_set_date(var->Widget, line);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_calendar_input_by_file(
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
		widget_calendar_set_date(var->Widget, line);
		g_free(line);
	}
	fclose(input);
}

void widget_calendar_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_calendar_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;

	(void)Attr;
	(void)Type;
	widget = gtk_calendar_new();
	widget_calendar_apply_display_option(widget, attr, "show-heading",
		GTK_CALENDAR_SHOW_HEADING);
	widget_calendar_apply_display_option(widget, attr, "show-day-names",
		GTK_CALENDAR_SHOW_DAY_NAMES);
	widget_calendar_apply_display_option(widget, attr, "no-month-change",
		GTK_CALENDAR_NO_MONTH_CHANGE);
	widget_calendar_apply_display_option(widget, attr, "show-week-numbers",
		GTK_CALENDAR_SHOW_WEEK_NUMBERS);
	return widget;
}

gchar *widget_calendar_envvar_construct(GtkWidget *widget)
{
	guint day, month, year;

	gtk_calendar_get_date(GTK_CALENDAR(widget), &year, &month, &day);
	return g_strdup_printf("%04u-%02u-%02u", year, month + 1, day);
}

void widget_calendar_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_calendar_set_date(var->Widget, value);
}

void widget_calendar_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_calendar_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_calendar_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_calendar_set_date(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
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

	g_signal_connect(G_OBJECT(var->Widget), "day-selected",
		G_CALLBACK(on_any_widget_day_selected_event), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "day-selected-double-click",
		G_CALLBACK(on_any_widget_day_selected_double_click_event),
		var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "month-changed",
		G_CALLBACK(on_any_widget_month_changed_event), var->Attributes);
}

void widget_calendar_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);
}

void widget_calendar_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *date;
	gchar *directive;
	gchar *filename = NULL;

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
	date = widget_calendar_envvar_construct(var->Widget);
	fputs(date, output);
	g_free(date);
	widget_close_output(output, filename);
}
