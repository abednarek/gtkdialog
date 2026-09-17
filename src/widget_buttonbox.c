/*
 * widget_buttonbox.c:
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
#include "widgets.h"
#include "widget_buttonbox.h"
#include "tag_attributes.h"

static gboolean widget_buttonbox_parse_layout(
	const gchar *value, GtkButtonBoxStyle *layout)
{
	if (strcasecmp(value, "default") == 0)
		*layout = GTK_BUTTONBOX_DEFAULT_STYLE;
	else if (strcasecmp(value, "spread") == 0)
		*layout = GTK_BUTTONBOX_SPREAD;
	else if (strcasecmp(value, "edge") == 0)
		*layout = GTK_BUTTONBOX_EDGE;
	else if (strcasecmp(value, "start") == 0)
		*layout = GTK_BUTTONBOX_START;
	else if (strcasecmp(value, "end") == 0)
		*layout = GTK_BUTTONBOX_END;
	else if (strcasecmp(value, "center") == 0)
		*layout = GTK_BUTTONBOX_CENTER;
	else
		return FALSE;

	return TRUE;
}

void widget_buttonbox_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_buttonbox_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkButtonBoxStyle layout;
	GtkWidget *widget;
	stackelement children;
	variable *child_var;
	gchar *value;
	gboolean secondary;
	gint n;

	(void)Attr;
	widget = Type == WIDGET_HBUTTONBOX
		? gtk_hbutton_box_new() : gtk_vbutton_box_new();

	if (attr != NULL &&
		(value = get_tag_attribute(attr, "layout-style")) != NULL) {
		if (widget_buttonbox_parse_layout(value, &layout))
			gtk_button_box_set_layout(GTK_BUTTON_BOX(widget), layout);
		else
			gtkdialog_warning("Invalid button box layout-style '%s'; "
				"keeping the GTK+ default.", value);
		kill_tag_attribute(attr, "layout-style");
	}

	children = pop();
	for (n = 0; n < children.nwidgets; ++n) {
		gtk_container_add(GTK_CONTAINER(widget), children.widgets[n]);
		child_var = find_variable_by_widget(children.widgets[n]);
		if (child_var == NULL || child_var->widget_tag_attr == NULL)
			continue;

		value = get_tag_attribute(child_var->widget_tag_attr, "secondary");
		if (value == NULL)
			continue;
		if (widget_parse_boolean(value, &secondary))
			gtk_button_box_set_child_secondary(GTK_BUTTON_BOX(widget),
				children.widgets[n], secondary);
		else
			gtkdialog_warning("Invalid button box secondary value '%s'; "
				"using false.", value);
		kill_tag_attribute(child_var->widget_tag_attr, "secondary");
	}
	stackelement_clear(&children);

	return widget;
}

gchar *widget_buttonbox_envvar_construct(GtkWidget *widget)
{
	(void)widget;
	return g_strdup("");
}

void widget_buttonbox_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void widget_buttonbox_refresh(variable *var)
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

void widget_buttonbox_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr,
		"%s(): Removeselected not implemented for this widget.\n", __func__);
}

void widget_buttonbox_save(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Save not implemented for this widget.\n",
		__func__);
}
