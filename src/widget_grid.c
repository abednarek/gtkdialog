/*
 * widget_grid.c:
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

#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "widget_grid.h"
#include "tag_attributes.h"

#define GRID_MAX_ATTACH 65535

static tag_attr *widget_grid_child_attributes(GtkWidget *widget)
{
	variable *var;

	while (widget != NULL) {
		var = find_variable_by_widget(widget);
		if (var != NULL)
			return var->widget_tag_attr;
		if (!GTK_IS_BIN(widget))
			break;
		widget = gtk_bin_get_child(GTK_BIN(widget));
	}

	return NULL;
}

static guint widget_grid_take_integer(tag_attr *attr, const gchar *name,
	guint minimum, guint maximum, guint fallback)
{
	gchar *value;
	guint result;

	if (attr == NULL || (value = get_tag_attribute(attr, name)) == NULL)
		return fallback;
	result = (guint)widget_parse_bounded_integer(value, (gint)minimum,
		(gint)maximum, (gint)fallback, name);
	kill_tag_attribute(attr, name);
	return result;
}

static gboolean widget_grid_parse_attach_options(const gchar *value,
	GtkAttachOptions *options)
{
	gchar **parts;
	gchar **part;
	gboolean found;
	gboolean none;

	*options = 0;
	found = FALSE;
	none = FALSE;
	parts = g_strsplit_set(value, "|, \t", -1);
	for (part = parts; *part != NULL; ++part) {
		if (**part == '\0')
			continue;
		found = TRUE;
		if (g_ascii_strcasecmp(*part, "expand") == 0)
			*options |= GTK_EXPAND;
		else if (g_ascii_strcasecmp(*part, "shrink") == 0)
			*options |= GTK_SHRINK;
		else if (g_ascii_strcasecmp(*part, "fill") == 0)
			*options |= GTK_FILL;
		else if (g_ascii_strcasecmp(*part, "none") == 0)
			none = TRUE;
		else {
			g_strfreev(parts);
			return FALSE;
		}
	}
	g_strfreev(parts);
	return found && !(none && *options != 0);
}

static GtkAttachOptions widget_grid_take_attach_options(tag_attr *attr,
	const gchar *name)
{
	GtkAttachOptions options;
	gchar *value;

	options = GTK_EXPAND | GTK_FILL;
	if (attr == NULL || (value = get_tag_attribute(attr, name)) == NULL)
		return options;
	if (!widget_grid_parse_attach_options(value, &options)) {
		gtkdialog_warning("Invalid grid %s value '%s'; using expand|fill.",
			name, value);
		options = GTK_EXPAND | GTK_FILL;
	}
	kill_tag_attribute(attr, name);
	return options;
}

void widget_grid_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_grid_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkAttachOptions xoptions;
	GtkAttachOptions yoptions;
	GtkWidget *widget;
	stackelement children;
	tag_attr *child_attr;
	guint column;
	guint columns;
	guint column_span;
	guint row;
	guint rows;
	guint row_span;
	guint xpadding;
	guint ypadding;
	gint n;

	(void)Attr;
	(void)Type;
	rows = widget_grid_take_integer(attr, "rows", 1,
		GRID_MAX_ATTACH, 1);
	columns = widget_grid_take_integer(attr, "columns", 1,
		GRID_MAX_ATTACH, 1);
	widget = gtk_table_new(rows, columns, FALSE);

	children = pop();
	for (n = 0; n < children.nwidgets; ++n) {
		if ((guint)n / columns >= GRID_MAX_ATTACH) {
			stackelement_clear(&children);
			g_error("gtkdialog: <grid> exceeds the GTK+ 2 attachment limit.");
		}
		child_attr = widget_grid_child_attributes(children.widgets[n]);
		column = widget_grid_take_integer(child_attr, "column", 0,
			GRID_MAX_ATTACH - 1, (guint)n % columns);
		row = widget_grid_take_integer(child_attr, "row", 0,
			GRID_MAX_ATTACH - 1, (guint)n / columns);
		column_span = widget_grid_take_integer(child_attr, "column-span", 1,
			GRID_MAX_ATTACH, 1);
		row_span = widget_grid_take_integer(child_attr, "row-span", 1,
			GRID_MAX_ATTACH, 1);
		if (column_span > GRID_MAX_ATTACH - column) {
			gtkdialog_warning("Grid column-span exceeds the GTK+ 2 limit; "
				"using %u.", GRID_MAX_ATTACH - column);
			column_span = GRID_MAX_ATTACH - column;
		}
		if (row_span > GRID_MAX_ATTACH - row) {
			gtkdialog_warning("Grid row-span exceeds the GTK+ 2 limit; "
				"using %u.", GRID_MAX_ATTACH - row);
			row_span = GRID_MAX_ATTACH - row;
		}
		xoptions = widget_grid_take_attach_options(child_attr, "x-options");
		yoptions = widget_grid_take_attach_options(child_attr, "y-options");
		xpadding = widget_grid_take_integer(child_attr, "x-padding", 0,
			GRID_MAX_ATTACH, 0);
		ypadding = widget_grid_take_integer(child_attr, "y-padding", 0,
			GRID_MAX_ATTACH, 0);
		gtk_table_attach(GTK_TABLE(widget), children.widgets[n],
			column, column + column_span, row, row + row_span,
			xoptions, yoptions, xpadding, ypadding);
	}
	stackelement_clear(&children);
	return widget;
}

gchar *widget_grid_envvar_construct(GtkWidget *widget)
{
	(void)widget;
	return g_strdup("");
}

void widget_grid_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void widget_grid_refresh(variable *var)
{
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr,
			"%s(): <label> not implemented for this widget.\n", __func__);
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr,
			"%s(): <item> not implemented for this widget.\n", __func__);
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		fprintf(stderr,
			"%s(): <default> not implemented for this widget.\n", __func__);
	if (attributeset_is_avail(var->Attributes, ATTR_INPUT))
		fprintf(stderr,
			"%s(): <input> not implemented for this widget.\n", __func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
}

void widget_grid_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr,
		"%s(): Removeselected not implemented for this widget.\n", __func__);
}

void widget_grid_save(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Save not implemented for this widget.\n",
		__func__);
}
