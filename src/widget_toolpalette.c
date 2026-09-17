/*
 * widget_toolpalette.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
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
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_toolpalette.h"

static void widget_toolpalette_store_boolean(GtkWidget *widget,
	tag_attr *attr, const gchar *attribute, const gchar *key)
{
	gchar *text;
	gboolean value;

	if (attr == NULL ||
		(text = get_tag_attribute(attr, (gchar *)attribute)) == NULL)
		return;
	if (widget_parse_boolean(text, &value))
		g_object_set_data(G_OBJECT(widget), key,
			GINT_TO_POINTER(value + 1));
	else
		gtkdialog_warning("Invalid toolitemgroup %s value '%s'; keeping "
			"the GTK+ default.", attribute, text);
	kill_tag_attribute(attr, (gchar *)attribute);
}

static void widget_toolpalette_set_item_packing(
	GtkWidget *group, GtkWidget *item)
{
	const gchar *keys[] = {
		"gtkdialog-toolpalette-homogeneous",
		"gtkdialog-toolpalette-expand",
		"gtkdialog-toolpalette-fill",
		"gtkdialog-toolpalette-new-row"
	};
	const gchar *properties[] = {
		"homogeneous", "expand", "fill", "new-row"
	};
	gpointer data;
	guint index;

	for (index = 0; index < G_N_ELEMENTS(keys); ++index) {
		data = g_object_get_data(G_OBJECT(item), keys[index]);
		if (data != NULL)
			gtk_container_child_set(GTK_CONTAINER(group), item,
				properties[index], GPOINTER_TO_INT(data) - 1, NULL);
	}
}

static gboolean widget_toolpalette_is_group(variable *var)
{
	return var->Type == WIDGET_TOOLITEMGROUP;
}

static void widget_toolpalette_set_group_label(
	variable *var, const gchar *label)
{
	gtk_tool_item_group_set_label(GTK_TOOL_ITEM_GROUP(var->Widget), label);
}

static void widget_toolpalette_input_by_command(
	variable *var, const gchar *command)
{
	FILE *input;
	gchar *line;

	input = widget_opencommand((gchar *)command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	line = widget_read_line(input);
	if (line != NULL) {
		widget_toolpalette_set_group_label(var, line);
		g_free(line);
	}
	widget_closecommand(input, (gchar *)command);
}

static void widget_toolpalette_input_by_file(
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
		widget_toolpalette_set_group_label(var, line);
		g_free(line);
	}
	fclose(input);
}

void widget_toolpalette_clear(variable *var)
{
	if (widget_toolpalette_is_group(var))
		widget_toolpalette_set_group_label(var, "");
	else
		fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
			__func__);
}

GtkWidget *widget_toolpalette_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList *element;
	GtkWidget *widget;
	stackelement children;
	const gchar *label;
	gint n;

	if (Type == WIDGET_TOOLITEMGROUP) {
		label = attributeset_is_avail(Attr, ATTR_LABEL)
			? attributeset_get_first(&element, Attr, ATTR_LABEL) : "";
		widget = gtk_tool_item_group_new(label);
		children = pop();
		for (n = 0; n < children.nwidgets; ++n) {
			if (!GTK_IS_TOOL_ITEM(children.widgets[n]))
				g_error("gtkdialog: <toolitemgroup> accepts only "
					"tool-item direct children.");
			gtk_tool_item_group_insert(GTK_TOOL_ITEM_GROUP(widget),
				GTK_TOOL_ITEM(children.widgets[n]), -1);
			widget_toolpalette_set_item_packing(
				widget, children.widgets[n]);
		}
		stackelement_clear(&children);
		widget_toolpalette_store_boolean(widget, attr, "exclusive",
			"gtkdialog-toolpalette-exclusive");
		widget_toolpalette_store_boolean(widget, attr, "expand",
			"gtkdialog-toolpalette-group-expand");
		return widget;
	}

	widget = gtk_tool_palette_new();
	children = pop();
	for (n = 0; n < children.nwidgets; ++n) {
		if (!GTK_IS_TOOL_ITEM_GROUP(children.widgets[n]))
			g_error("gtkdialog: <toolpalette> accepts only "
				"<toolitemgroup> direct children.");
		gtk_container_add(GTK_CONTAINER(widget), children.widgets[n]);
		if (g_object_get_data(G_OBJECT(children.widgets[n]),
			"gtkdialog-toolpalette-exclusive") != NULL)
			gtk_tool_palette_set_exclusive(GTK_TOOL_PALETTE(widget),
				GTK_TOOL_ITEM_GROUP(children.widgets[n]),
				GPOINTER_TO_INT(g_object_get_data(
					G_OBJECT(children.widgets[n]),
					"gtkdialog-toolpalette-exclusive")) - 1);
		if (g_object_get_data(G_OBJECT(children.widgets[n]),
			"gtkdialog-toolpalette-group-expand") != NULL)
			gtk_tool_palette_set_expand(GTK_TOOL_PALETTE(widget),
				GTK_TOOL_ITEM_GROUP(children.widgets[n]),
				GPOINTER_TO_INT(g_object_get_data(
					G_OBJECT(children.widgets[n]),
					"gtkdialog-toolpalette-group-expand")) - 1);
	}
	stackelement_clear(&children);
	return widget;
}

gchar *widget_toolpalette_envvar_construct(GtkWidget *widget)
{
	if (GTK_IS_TOOL_ITEM_GROUP(widget)) {
		const gchar *label;

		label = gtk_tool_item_group_get_label(GTK_TOOL_ITEM_GROUP(widget));
		return g_strdup(label != NULL ? label : "");
	}
	return g_strdup("");
}

void widget_toolpalette_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	if (widget_toolpalette_is_group(var))
		widget_toolpalette_set_group_label(var, value);
	else
		fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
			__func__);
}

void widget_toolpalette_refresh(variable *var)
{
	GList *element;
	gboolean initialised;
	gint width;
	gint height;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (widget_toolpalette_is_group(var)) {
		gchar *input;

		input = attributeset_get_first(&element,
			var->Attributes, ATTR_INPUT);
		while (input != NULL) {
			if (input_is_shell_command(input))
				widget_toolpalette_input_by_command(var, input + 8);
			else if (strncasecmp(input, "file:", 5) == 0 &&
				strlen(input) > 5) {
				if (!initialised)
					widget_file_monitor_try_create(var, input + 5);
				widget_toolpalette_input_by_file(var, input + 5);
			}
			input = attributeset_get_next(&element,
				var->Attributes, ATTR_INPUT);
		}
	}
	if (initialised)
		return;

	width = -1;
	height = -1;
	if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
		width = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_WIDTH), "tool palette width");
	if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
		height = widget_parse_size_request(attributeset_get_first(&element,
			var->Attributes, ATTR_HEIGHT), "tool palette height");
	if (width != -1 || height != -1)
		gtk_widget_set_size_request(var->Widget, width, height);

	if (widget_toolpalette_is_group(var)) {
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			widget_toolpalette_set_group_label(var,
				attributeset_get_first(&element,
					var->Attributes, ATTR_DEFAULT));
	} else {
		if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
			fprintf(stderr,
				"%s(): <label> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			fprintf(stderr,
				"%s(): <default> not implemented for this widget.\n",
				__func__);
		if (attributeset_is_avail(var->Attributes, ATTR_INPUT))
			fprintf(stderr,
				"%s(): <input> not implemented for this widget.\n",
				__func__);
	}
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr,
			"%s(): <item> not implemented for this widget.\n", __func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
}

void widget_toolpalette_removeselected(variable *var)
{
	widget_toolpalette_clear(var);
}

void widget_toolpalette_save(variable *var)
{
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;

	if (!widget_toolpalette_is_group(var)) {
		fprintf(stderr, "%s(): Save not implemented for this widget.\n",
			__func__);
		return;
	}
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
	{
		const gchar *label;

		label = gtk_tool_item_group_get_label(
			GTK_TOOL_ITEM_GROUP(var->Widget));
		fputs(label != NULL ? label : "", output);
	}
	widget_close_output(output, filename);
}
