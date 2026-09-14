/*
 * widget_toolbar.c:
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
#include "widget_toolbar.h"
#include "widget_menuitem.h"
#include "signals.h"
#include "tag_attributes.h"

#if GTK_CHECK_VERSION(2,4,0)
static gboolean widget_toolbar_type_is_toggle(gint Type)
{
	return Type == WIDGET_TOGGLETOOLBUTTON ||
		Type == WIDGET_RADIOTOOLBUTTON;
}

static void widget_toolbar_set_active(GtkWidget *widget, const gchar *value)
{
	gint active;

	if (!widget_parse_boolean((gchar *)value, &active))
		active = FALSE;
	gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(widget), active);
}

static void widget_toolbar_input_by_command(variable *var, const gchar *command)
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
		widget_toolbar_set_active(var->Widget, line);
		g_free(line);
	}
	widget_closecommand(input, (gchar *)command);
}

static void widget_toolbar_input_by_file(variable *var, const gchar *filename)
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
		widget_toolbar_set_active(var->Widget, line);
		g_free(line);
	}
	fclose(input);
}

static gboolean widget_toolbar_parse_style(
	const gchar *value, GtkToolbarStyle *style)
{
	if (strcasecmp(value, "icons") == 0)
		*style = GTK_TOOLBAR_ICONS;
	else if (strcasecmp(value, "text") == 0)
		*style = GTK_TOOLBAR_TEXT;
	else if (strcasecmp(value, "both") == 0)
		*style = GTK_TOOLBAR_BOTH;
	else if (strcasecmp(value, "both-horizontal") == 0 ||
		strcasecmp(value, "both-horiz") == 0)
		*style = GTK_TOOLBAR_BOTH_HORIZ;
	else
		return FALSE;

	return TRUE;
}

static gboolean widget_toolbar_parse_orientation(
	const gchar *value, GtkOrientation *orientation)
{
	if (strcasecmp(value, "horizontal") == 0)
		*orientation = GTK_ORIENTATION_HORIZONTAL;
	else if (strcasecmp(value, "vertical") == 0)
		*orientation = GTK_ORIENTATION_VERTICAL;
	else
		return FALSE;

	return TRUE;
}

static void widget_toolitem_set_layout(GtkToolItem *item, tag_attr *attr)
{
	gchar *value;
	gboolean setting;

	if (attr == NULL)
		return;

	value = get_tag_attribute(attr, "expand");
	if (value != NULL) {
		if (widget_parse_boolean(value, &setting)) {
			gtk_tool_item_set_expand(item, setting);
			g_object_set_data(G_OBJECT(item),
				"gtkdialog-toolpalette-expand",
				GINT_TO_POINTER(setting + 1));
		} else
			gtkdialog_warning("Invalid toolitem expand value '%s'; keeping "
				"the GTK+ default.", value);
		kill_tag_attribute(attr, "expand");
	}

	value = get_tag_attribute(attr, "homogeneous");
	if (value != NULL) {
		if (widget_parse_boolean(value, &setting)) {
			gtk_tool_item_set_homogeneous(item, setting);
			g_object_set_data(G_OBJECT(item),
				"gtkdialog-toolpalette-homogeneous",
				GINT_TO_POINTER(setting + 1));
		} else
			gtkdialog_warning("Invalid toolitem homogeneous value '%s'; "
				"keeping the GTK+ default.", value);
		kill_tag_attribute(attr, "homogeneous");
	}

	value = get_tag_attribute(attr, "palette-fill");
	if (value != NULL) {
		if (widget_parse_boolean(value, &setting))
			g_object_set_data(G_OBJECT(item),
				"gtkdialog-toolpalette-fill",
				GINT_TO_POINTER(setting + 1));
		else
			gtkdialog_warning("Invalid toolitem palette-fill value '%s'; "
				"keeping the GTK+ default.", value);
		kill_tag_attribute(attr, "palette-fill");
	}

	value = get_tag_attribute(attr, "palette-new-row");
	if (value != NULL) {
		if (widget_parse_boolean(value, &setting))
			g_object_set_data(G_OBJECT(item),
				"gtkdialog-toolpalette-new-row",
				GINT_TO_POINTER(setting + 1));
		else
			gtkdialog_warning("Invalid toolitem palette-new-row value '%s'; "
				"keeping the GTK+ default.", value);
		kill_tag_attribute(attr, "palette-new-row");
	}
}
#endif

void widget_toolbar_clear(variable *var)
{
#if GTK_CHECK_VERSION(2,4,0)
	if (widget_toolbar_type_is_toggle(var->Type))
		gtk_toggle_tool_button_set_active(
			GTK_TOGGLE_TOOL_BUTTON(var->Widget), FALSE);
	else
#endif
		fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
			__func__);
}

GtkWidget *widget_toolbar_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
#if GTK_CHECK_VERSION(2,4,0)
	GtkOrientation orientation;
	GtkToolbarStyle style;
	GtkWidget *widget;
	stackelement children;
	gchar *value;
	gint n;

	(void)Attr;
	(void)Type;
	widget = gtk_toolbar_new();

	if (attr != NULL &&
		(value = get_tag_attribute(attr, "toolbar-style")) != NULL) {
		if (widget_toolbar_parse_style(value, &style))
			gtk_toolbar_set_style(GTK_TOOLBAR(widget), style);
		else
			gtkdialog_warning("Invalid toolbar-style '%s'; keeping the "
				"GTK+ default.", value);
		kill_tag_attribute(attr, "toolbar-style");
	}
	if (attr != NULL &&
		(value = get_tag_attribute(attr, "orientation")) != NULL) {
		if (widget_toolbar_parse_orientation(value, &orientation))
			gtk_toolbar_set_orientation(GTK_TOOLBAR(widget), orientation);
		else
			gtkdialog_warning("Invalid toolbar orientation '%s'; keeping "
				"horizontal.", value);
		kill_tag_attribute(attr, "orientation");
	}

	children = pop();
	for (n = 0; n < children.nwidgets; ++n) {
		if (!GTK_IS_TOOL_ITEM(children.widgets[n]))
			g_error("gtkdialog: <toolbar> accepts only tool-item direct "
				"children.");
		gtk_toolbar_insert(GTK_TOOLBAR(widget),
			GTK_TOOL_ITEM(children.widgets[n]), -1);
	}
	stackelement_clear(&children);
	return widget;
#else
	(void)Attr;
	(void)attr;
	(void)Type;
	g_error("gtkdialog: <toolbar> requires GTK+ 2.4 or later.");
	return NULL;
#endif
}

GtkWidget *widget_toolitem_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
#if GTK_CHECK_VERSION(2,4,0)
	GList *element;
	const gchar *label;
	GtkToolItem *item;
	stackelement child;

	if (Type == WIDGET_SEPARATORTOOLITEM) {
		item = gtk_separator_tool_item_new();
		widget_toolitem_set_layout(item, attr);
		return GTK_WIDGET(item);
	}
	if (Type == WIDGET_TOOLITEM) {
		child = pop();
		if (child.nwidgets != 1) {
			stackelement_clear(&child);
			g_error("gtkdialog: <toolitem> requires exactly one direct "
				"child widget.");
		}
		item = gtk_tool_item_new();
		gtk_container_add(GTK_CONTAINER(item), child.widgets[0]);
		stackelement_clear(&child);
		widget_toolitem_set_layout(item, attr);
		return GTK_WIDGET(item);
	}

	label = attributeset_is_avail(Attr, ATTR_LABEL)
		? attributeset_get_first(&element, Attr, ATTR_LABEL) : NULL;
	if (Type == WIDGET_TOGGLETOOLBUTTON)
		item = gtk_toggle_tool_button_new();
	else if (Type == WIDGET_RADIOTOOLBUTTON) {
		if (lasttoolradiowidget == NULL) {
			item = gtk_radio_tool_button_new(NULL);
			lasttoolradiowidget = GTK_WIDGET(item);
		} else
			item = gtk_radio_tool_button_new_from_widget(
				GTK_RADIO_TOOL_BUTTON(lasttoolradiowidget));
	} else if (Type == WIDGET_MENUTOOLBUTTON) {
#if GTK_CHECK_VERSION(2,6,0)
		GtkWidget *menu;

		item = gtk_menu_tool_button_new(NULL, label);
		menu = widget_menu_shell_create(NULL);
		/* Keep an explicit construction reference until the tool button's
		 * menu property has taken its own reference. */
		g_object_ref_sink(menu);
		gtk_menu_tool_button_set_menu(GTK_MENU_TOOL_BUTTON(item), menu);
		g_object_unref(menu);
#else
		g_error("gtkdialog: <menutoolbutton> requires GTK+ 2.6 or later.");
		return NULL;
#endif
	} else
		item = gtk_tool_button_new(NULL, label);

	if (label != NULL && Type != WIDGET_TOOLBUTTON)
		gtk_tool_button_set_label(GTK_TOOL_BUTTON(item), label);
	widget_toolitem_set_layout(item, attr);
	return GTK_WIDGET(item);
#else
	(void)Attr;
	(void)attr;
	(void)Type;
	g_error("gtkdialog: tool items require GTK+ 2.4 or later.");
	return NULL;
#endif
}

gchar *widget_toolbar_envvar_construct(GtkWidget *widget)
{
#if GTK_CHECK_VERSION(2,4,0)
	if (GTK_IS_TOGGLE_TOOL_BUTTON(widget))
		return g_strdup(gtk_toggle_tool_button_get_active(
			GTK_TOGGLE_TOOL_BUTTON(widget)) ? "true" : "false");
#else
	(void)widget;
#endif
	return g_strdup("");
}

void widget_toolbar_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
#if GTK_CHECK_VERSION(2,4,0)
	if (widget_toolbar_type_is_toggle(var->Type))
		widget_toolbar_set_active(var->Widget, value);
	else
#endif
		fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n",
			__func__);
}

void widget_toolbar_refresh(variable *var)
{
	GList *element;
	gchar *input;
#if GTK_CHECK_VERSION(2,4,0)
	gchar *value;
#endif
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
#if GTK_CHECK_VERSION(2,4,0)
		if (widget_toolbar_type_is_toggle(var->Type)) {
			if (input_is_shell_command(input))
				widget_toolbar_input_by_command(var, input + 8);
			else if (strncasecmp(input, "file:", 5) == 0 &&
				strlen(input) > 5) {
				if (!initialised)
					widget_file_monitor_try_create(var, input + 5);
				widget_toolbar_input_by_file(var, input + 5);
			}
		} else
#endif
			fprintf(stderr,
				"%s(): <input> not implemented for this widget.\n",
				__func__);
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;

	if ((var->Type == WIDGET_SEPARATORTOOLITEM ||
		var->Type == WIDGET_TOOLITEM) &&
		attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr,
			"%s(): <label> not implemented for this widget.\n", __func__);
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr,
			"%s(): <item> not implemented for this widget.\n", __func__);
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT)) {
#if GTK_CHECK_VERSION(2,4,0)
		if (widget_toolbar_type_is_toggle(var->Type)) {
			value = attributeset_get_first(&element, var->Attributes,
				ATTR_DEFAULT);
			widget_toolbar_set_active(var->Widget, value);
		} else
#endif
			fprintf(stderr,
				"%s(): <default> not implemented for this widget.\n",
				__func__);
	}
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);

	if (var->Type == WIDGET_TOOLBUTTON ||
		var->Type == WIDGET_MENUTOOLBUTTON)
		g_signal_connect(G_OBJECT(var->Widget), "clicked",
			G_CALLBACK(button_clicked_attr), (gpointer)var->Attributes);
#if GTK_CHECK_VERSION(2,4,0)
	else if (widget_toolbar_type_is_toggle(var->Type))
		g_signal_connect(G_OBJECT(var->Widget), "toggled",
			G_CALLBACK(on_any_widget_toggled_event),
			(gpointer)var->Attributes);
#endif
}

void widget_toolbar_removeselected(variable *var)
{
	(void)var;
	fprintf(stderr,
		"%s(): Removeselected not implemented for this widget.\n", __func__);
}

void widget_toolbar_save(variable *var)
{
#if GTK_CHECK_VERSION(2,4,0)
	FILE *output;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;

	if (!widget_toolbar_type_is_toggle(var->Type)) {
		fprintf(stderr, "%s(): Save not implemented for this widget.\n",
			__func__);
		return;
	}
	directive = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (strncasecmp(directive, "file:", 5) == 0 &&
			strlen(directive) > 5) {
			filename = directive + 5;
			break;
		}
		directive = attributeset_get_next(&element, var->Attributes,
			ATTR_OUTPUT);
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
	fputs(gtk_toggle_tool_button_get_active(
		GTK_TOGGLE_TOOL_BUTTON(var->Widget)) ? "true" : "false", output);
	widget_close_output(output, filename);
#else
	(void)var;
	fprintf(stderr, "%s(): Save not implemented for this widget.\n",
		__func__);
#endif
}
