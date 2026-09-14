/*
 * widget_fixed.c:
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
#include "widget_fixed.h"
#include "tag_attributes.h"

static tag_attr *widget_fixed_child_attributes(GtkWidget *widget)
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

static gint widget_fixed_take_coordinate(tag_attr *attr, const gchar *name)
{
	gchar *value;
	gint result;

	if (attr == NULL || (value = get_tag_attribute(attr, name)) == NULL)
		return 0;
	result = widget_parse_bounded_integer(value, G_MININT, G_MAXINT, 0,
		name);
	kill_tag_attribute(attr, name);
	return result;
}

void widget_fixed_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_fixed_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;
	stackelement children;
	gint n;
	tag_attr *child_attr;
	gint x;
	gint y;

	(void)Attr;
	(void)attr;
	(void)Type;
	widget = gtk_fixed_new();
	children = pop();
	for (n = 0; n < children.nwidgets; ++n) {
		child_attr = widget_fixed_child_attributes(children.widgets[n]);
		x = widget_fixed_take_coordinate(child_attr, "x");
		y = widget_fixed_take_coordinate(child_attr, "y");
		gtk_fixed_put(GTK_FIXED(widget), children.widgets[n], x, y);
	}
	stackelement_clear(&children);
	return widget;
}

gchar *widget_fixed_envvar_construct(GtkWidget *widget)
{
	(void)widget;
	return g_strdup("");
}

void widget_fixed_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void widget_fixed_refresh(variable *var)
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

void widget_fixed_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr,
		"%s(): Removeselected not implemented for this widget.\n", __func__);
}

void widget_fixed_save(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Save not implemented for this widget.\n",
		__func__);
}
