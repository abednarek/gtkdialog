/*
 * widget_alignment.c:
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
#include "widget_alignment.h"
#include "tag_attributes.h"

void widget_alignment_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

GtkWidget *widget_alignment_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget *widget;
	stackelement child;

	(void)Attr;
	(void)attr;
	(void)Type;
	child = pop();
	if (child.nwidgets != 1) {
		stackelement_clear(&child);
		g_error("gtkdialog: <alignment> requires exactly one direct "
			"child widget.");
	}

	widget = gtk_alignment_new(0.5, 0.5, 1.0, 1.0);
	gtk_container_add(GTK_CONTAINER(widget), child.widgets[0]);
	stackelement_clear(&child);
	return widget;
}

gchar *widget_alignment_envvar_construct(GtkWidget *widget)
{
	(void)widget;
	return g_strdup("");
}

void widget_alignment_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
		__func__);
}

void widget_alignment_refresh(variable *var)
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

void widget_alignment_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr,
		"%s(): Removeselected not implemented for this widget.\n", __func__);
}

void widget_alignment_save(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Save not implemented for this widget.\n",
		__func__);
}
