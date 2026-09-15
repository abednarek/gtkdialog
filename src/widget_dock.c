/*
 * widget_dock.c:
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
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_dock.h"

#if HAVE_GDL
#include <gdl/gdl.h>
#endif

#define DOCK_DATA_KEY "gtkdialog-dock-data"
#define DOCK_KIND_KEY "gtkdialog-dock-kind"
#define DOCK_ITEM_NAME_KEY "gtkdialog-dock-item-name"
#define DOCK_ITEM_PLACEMENT_KEY "gtkdialog-dock-item-placement"
#define DOCK_ITEM_FLOAT_X_KEY "gtkdialog-dock-item-float-x"
#define DOCK_ITEM_FLOAT_Y_KEY "gtkdialog-dock-item-float-y"
#define DOCK_ITEM_FLOAT_WIDTH_KEY "gtkdialog-dock-item-float-width"
#define DOCK_ITEM_FLOAT_HEIGHT_KEY "gtkdialog-dock-item-float-height"
#define DOCK_ITEM_TARGET_KEY "gtkdialog-dock-item-target"
#define DOCK_ITEM_PARAM_KEY "gtkdialog-dock-item-param"

enum {
	DOCK_KIND_CONTAINER = 1,
	DOCK_KIND_ITEM = 2
};

static const gchar *dock_custom_attributes[] = {
	"switcher-style", "show-dockbar", "dockbar-style",
	"dockbar-orientation", "locked", "default-title", NULL
};

static const gchar *dockitem_custom_attributes[] = {
	"name", "stock-id", "behavior", "position", "floating-x",
	"floating-y", "floating-width", "floating-height", "target",
	"docking-param", NULL
};

static void widget_dock_kill_attributes(tag_attr *attr,
	const gchar *const *names)
{
	gint index;

	if (attr == NULL)
		return;
	for (index = 0; names[index] != NULL; ++index)
		kill_tag_attribute(attr, names[index]);
}

gboolean widget_dock_is_container(GtkWidget *widget)
{
	return widget != NULL && GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), DOCK_KIND_KEY)) == DOCK_KIND_CONTAINER;
}

gboolean widget_dock_is_item(GtkWidget *widget)
{
	return widget != NULL && GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), DOCK_KIND_KEY)) == DOCK_KIND_ITEM;
}

#if HAVE_GDL

typedef struct {
	GtkWidget *dock;
	GdlDockLayout *layout;
} DockData;

static void widget_dock_data_free(gpointer pointer)
{
	DockData *data = pointer;

	g_object_unref(data->layout);
	g_free(data);
}

static gint widget_dock_parse_integer(tag_attr *attr, const gchar *name,
	gint minimum, gint maximum, gint fallback)
{
	gchar *value = attr != NULL ? get_tag_attribute(attr, name) : NULL;

	return value != NULL ? widget_parse_bounded_integer(value, minimum,
		maximum, fallback, name) : fallback;
}

static GdlDockPlacement widget_dock_parse_placement(tag_attr *attr)
{
	gchar *value = attr != NULL ? get_tag_attribute(attr, "position") : NULL;

	if (value == NULL || g_ascii_strcasecmp(value, "center") == 0)
		return GDL_DOCK_CENTER;
	if (g_ascii_strcasecmp(value, "top") == 0)
		return GDL_DOCK_TOP;
	if (g_ascii_strcasecmp(value, "bottom") == 0)
		return GDL_DOCK_BOTTOM;
	if (g_ascii_strcasecmp(value, "left") == 0)
		return GDL_DOCK_LEFT;
	if (g_ascii_strcasecmp(value, "right") == 0)
		return GDL_DOCK_RIGHT;
	if (g_ascii_strcasecmp(value, "floating") == 0)
		return GDL_DOCK_FLOATING;
	gtkdialog_warning("Invalid dockitem position '%s'; using center.", value);
	return GDL_DOCK_CENTER;
}

static GdlDockItemBehavior widget_dock_parse_behavior(tag_attr *attr)
{
	GdlDockItemBehavior behavior = GDL_DOCK_ITEM_BEH_NORMAL;
	gchar **names;
	gchar *value;
	gint index;

	value = attr != NULL ? get_tag_attribute(attr, "behavior") : NULL;
	if (value == NULL || *value == '\0')
		return behavior;
	names = g_strsplit_set(value, "|, ", -1);
	for (index = 0; names[index] != NULL; ++index) {
		const gchar *name = names[index];

		if (*name == '\0' || g_ascii_strcasecmp(name, "normal") == 0)
			continue;
		if (g_ascii_strcasecmp(name, "never-floating") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_NEVER_FLOATING;
		else if (g_ascii_strcasecmp(name, "never-vertical") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_NEVER_VERTICAL;
		else if (g_ascii_strcasecmp(name, "never-horizontal") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_NEVER_HORIZONTAL;
		else if (g_ascii_strcasecmp(name, "locked") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_LOCKED;
		else if (g_ascii_strcasecmp(name, "cant-dock-top") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_CANT_DOCK_TOP;
		else if (g_ascii_strcasecmp(name, "cant-dock-bottom") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_CANT_DOCK_BOTTOM;
		else if (g_ascii_strcasecmp(name, "cant-dock-left") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_CANT_DOCK_LEFT;
		else if (g_ascii_strcasecmp(name, "cant-dock-right") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_CANT_DOCK_RIGHT;
		else if (g_ascii_strcasecmp(name, "cant-dock-center") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_CANT_DOCK_CENTER;
		else if (g_ascii_strcasecmp(name, "cant-close") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_CANT_CLOSE;
		else if (g_ascii_strcasecmp(name, "cant-iconify") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_CANT_ICONIFY;
		else if (g_ascii_strcasecmp(name, "no-grip") == 0)
			behavior |= GDL_DOCK_ITEM_BEH_NO_GRIP;
		else
			gtkdialog_warning("Unknown dockitem behavior '%s'.", name);
	}
	g_strfreev(names);
	return behavior;
}

static GdlSwitcherStyle widget_dock_parse_switcher_style(tag_attr *attr)
{
	gchar *value = attr != NULL
		? get_tag_attribute(attr, "switcher-style") : NULL;

	if (value == NULL || g_ascii_strcasecmp(value, "both") == 0)
		return GDL_SWITCHER_STYLE_BOTH;
	if (g_ascii_strcasecmp(value, "text") == 0)
		return GDL_SWITCHER_STYLE_TEXT;
	if (g_ascii_strcasecmp(value, "icon") == 0)
		return GDL_SWITCHER_STYLE_ICON;
	if (g_ascii_strcasecmp(value, "toolbar") == 0)
		return GDL_SWITCHER_STYLE_TOOLBAR;
	if (g_ascii_strcasecmp(value, "tabs") == 0)
		return GDL_SWITCHER_STYLE_TABS;
	if (g_ascii_strcasecmp(value, "none") == 0)
		return GDL_SWITCHER_STYLE_NONE;
	gtkdialog_warning("Invalid dock switcher-style '%s'; using both.", value);
	return GDL_SWITCHER_STYLE_BOTH;
}

static GdlDockBarStyle widget_dock_parse_bar_style(tag_attr *attr)
{
	gchar *value = attr != NULL
		? get_tag_attribute(attr, "dockbar-style") : NULL;

	if (value == NULL || g_ascii_strcasecmp(value, "auto") == 0)
		return GDL_DOCK_BAR_AUTO;
	if (g_ascii_strcasecmp(value, "icons") == 0)
		return GDL_DOCK_BAR_ICONS;
	if (g_ascii_strcasecmp(value, "text") == 0)
		return GDL_DOCK_BAR_TEXT;
	if (g_ascii_strcasecmp(value, "both") == 0)
		return GDL_DOCK_BAR_BOTH;
	gtkdialog_warning("Invalid dockbar-style '%s'; using auto.", value);
	return GDL_DOCK_BAR_AUTO;
}

static GtkOrientation widget_dock_parse_bar_orientation(tag_attr *attr)
{
	gchar *value = attr != NULL
		? get_tag_attribute(attr, "dockbar-orientation") : NULL;

	if (value == NULL || g_ascii_strcasecmp(value, "vertical") == 0)
		return GTK_ORIENTATION_VERTICAL;
	if (g_ascii_strcasecmp(value, "horizontal") == 0)
		return GTK_ORIENTATION_HORIZONTAL;
	gtkdialog_warning("Invalid dockbar-orientation '%s'; using vertical.",
		value);
	return GTK_ORIENTATION_VERTICAL;
}

static void widget_dock_layout_changed(GdlDock *dock, gpointer user_data)
{
	GtkWidget *widget = GTK_WIDGET(user_data);
	AttributeSet *attributes;

	(void)dock;
	attributes = g_object_get_data(G_OBJECT(widget), "gtkdialog-attributes");
	if (attributes != NULL)
		widget_signal_executor(widget, attributes, "layout-changed");
}

static void widget_dockitem_selected(GdlDockItem *item, gpointer user_data)
{
	AttributeSet *attributes = user_data;

	widget_signal_executor(GTK_WIDGET(item), attributes, "selected");
}

static void widget_dock_add_item(GtkWidget *dock, GtkWidget *item,
	GdlDockPlacement placement)
{
	const gchar *target_name = g_object_get_data(
		G_OBJECT(item), DOCK_ITEM_TARGET_KEY);
	GdlDockItem *target = NULL;

	if (target_name != NULL && *target_name != '\0')
		target = gdl_dock_get_item_by_name(GDL_DOCK(dock), target_name);
	if (placement == GDL_DOCK_FLOATING)
		gdl_dock_add_floating_item(GDL_DOCK(dock), GDL_DOCK_ITEM(item),
			GPOINTER_TO_INT(g_object_get_data(
				G_OBJECT(item), DOCK_ITEM_FLOAT_X_KEY)),
			GPOINTER_TO_INT(g_object_get_data(
				G_OBJECT(item), DOCK_ITEM_FLOAT_Y_KEY)),
			GPOINTER_TO_INT(g_object_get_data(
				G_OBJECT(item), DOCK_ITEM_FLOAT_WIDTH_KEY)),
			GPOINTER_TO_INT(g_object_get_data(
				G_OBJECT(item), DOCK_ITEM_FLOAT_HEIGHT_KEY)));
	else if (target != NULL && target != GDL_DOCK_ITEM(item))
		gdl_dock_item_dock_to(GDL_DOCK_ITEM(item), target, placement,
			GPOINTER_TO_INT(g_object_get_data(
				G_OBJECT(item), DOCK_ITEM_PARAM_KEY)));
	else {
		if (target_name != NULL && *target_name != '\0')
			gtkdialog_warning(
				"Dockitem target '%s' is not attached yet; using the main dock.",
				target_name);
		gdl_dock_add_item(GDL_DOCK(dock), GDL_DOCK_ITEM(item), placement);
	}
}

static gboolean widget_dock_item_target_is_attached(GtkWidget *dock,
	GtkWidget *item, GdlDockPlacement placement)
{
	const gchar *target_name;

	if (placement == GDL_DOCK_FLOATING)
		return TRUE;
	target_name = g_object_get_data(G_OBJECT(item), DOCK_ITEM_TARGET_KEY);
	return target_name == NULL || *target_name == '\0' ||
		gdl_dock_get_item_by_name(GDL_DOCK(dock), target_name) != NULL;
}

#endif

GtkWidget *widget_dockitem_create(AttributeSet *attributes, tag_attr *attr,
	gint type)
{
	static guint generated_name = 0;
	GtkWidget *widget;
	stackelement child;
	gchar generated[64];
	gchar *label;
	gchar *name;
	GList *element;

	(void)type;
	child = pop();
	if (child.nwidgets != 1) {
		stackelement_clear(&child);
		g_error("gtkdialog: <dockitem> requires exactly one direct child widget.");
	}
	label = attributeset_get_first(&element, attributes, ATTR_LABEL);
	name = attr != NULL ? get_tag_attribute(attr, "name") : NULL;
	if (name == NULL || *name == '\0') {
		g_snprintf(generated, sizeof(generated), "gtkdialog-dockitem-%u",
			++generated_name);
		name = generated;
	}
#if HAVE_GDL
	{
		GdlDockItemBehavior behavior = widget_dock_parse_behavior(attr);
		gchar *stock_id = attr != NULL
			? get_tag_attribute(attr, "stock-id") : NULL;
		GdlDockPlacement placement = widget_dock_parse_placement(attr);

		if (stock_id != NULL && *stock_id != '\0')
			widget = gdl_dock_item_new_with_stock(name,
				label != NULL ? label : name, stock_id, behavior);
		else
			widget = gdl_dock_item_new(name,
				label != NULL ? label : name, behavior);
		gtk_container_add(GTK_CONTAINER(widget), child.widgets[0]);
		g_object_set_data(G_OBJECT(widget), DOCK_ITEM_PLACEMENT_KEY,
			GINT_TO_POINTER((gint)placement));
		g_object_set_data(G_OBJECT(widget), DOCK_ITEM_FLOAT_X_KEY,
			GINT_TO_POINTER(widget_dock_parse_integer(attr, "floating-x",
				G_MININT, G_MAXINT, 0)));
		g_object_set_data(G_OBJECT(widget), DOCK_ITEM_FLOAT_Y_KEY,
			GINT_TO_POINTER(widget_dock_parse_integer(attr, "floating-y",
				G_MININT, G_MAXINT, 0)));
		g_object_set_data(G_OBJECT(widget), DOCK_ITEM_FLOAT_WIDTH_KEY,
			GINT_TO_POINTER(widget_dock_parse_integer(attr, "floating-width",
				1, G_MAXINT, 320)));
		g_object_set_data(G_OBJECT(widget), DOCK_ITEM_FLOAT_HEIGHT_KEY,
			GINT_TO_POINTER(widget_dock_parse_integer(attr, "floating-height",
				1, G_MAXINT, 240)));
		g_object_set_data_full(G_OBJECT(widget), DOCK_ITEM_TARGET_KEY,
			g_strdup(attr != NULL ? get_tag_attribute(attr, "target") : NULL),
			g_free);
		g_object_set_data(G_OBJECT(widget), DOCK_ITEM_PARAM_KEY,
			GINT_TO_POINTER(widget_dock_parse_integer(attr, "docking-param",
				0, G_MAXINT, 0)));
		g_signal_connect(widget, "selected",
			G_CALLBACK(widget_dockitem_selected), attributes);
	}
#else
	widget = gtk_frame_new(label != NULL ? label : name);
	gtk_container_add(GTK_CONTAINER(widget), child.widgets[0]);
#endif
	stackelement_clear(&child);
	g_object_set_data(G_OBJECT(widget), DOCK_KIND_KEY,
		GINT_TO_POINTER(DOCK_KIND_ITEM));
	g_object_set_data_full(G_OBJECT(widget), DOCK_ITEM_NAME_KEY,
		g_strdup(name), g_free);
	widget_dock_kill_attributes(attr, dockitem_custom_attributes);
	return widget;
}

GtkWidget *widget_dock_create(AttributeSet *attributes, tag_attr *attr,
	gint type)
{
	GtkWidget *widget;
	stackelement items;
	gint index;

	(void)attributes;
	(void)type;
	items = pop();
	if (items.nwidgets <= 0) {
		stackelement_clear(&items);
		g_error("gtkdialog: <dock> requires at least one dockitem.");
	}
	for (index = 0; index < items.nwidgets; ++index) {
		if (items.widgettypes[index] != WIDGET_DOCKITEM) {
			stackelement_clear(&items);
			g_error("gtkdialog: <dock> accepts only direct <dockitem> children.");
		}
	}
	for (index = 0; index < items.nwidgets; ++index) {
		const gchar *name = g_object_get_data(G_OBJECT(items.widgets[index]),
			DOCK_ITEM_NAME_KEY);
		gint later;

		for (later = index + 1; later < items.nwidgets; ++later) {
			const gchar *later_name = g_object_get_data(
				G_OBJECT(items.widgets[later]), DOCK_ITEM_NAME_KEY);

			if (g_strcmp0(name, later_name) == 0) {
				stackelement_clear(&items);
				g_error("gtkdialog: dockitem name '%s' is not unique.",
					name);
			}
		}
	}
	widget = gtk_hbox_new(FALSE, 0);
	g_object_set_data(G_OBJECT(widget), DOCK_KIND_KEY,
		GINT_TO_POINTER(DOCK_KIND_CONTAINER));
#if HAVE_GDL
	{
		DockData *data = g_new0(DockData, 1);
		GdlDockMaster *master;
		GtkWidget *dockbar;
		gchar *value;
		gboolean show_dockbar = FALSE;
		gboolean *attached;
		guint item_count = (guint)items.nwidgets;
		gint anchor = -1;
		gint remaining;

		data->dock = gdl_dock_new();
		data->layout = gdl_dock_layout_new(GDL_DOCK(data->dock));
		master = GDL_DOCK_OBJECT_GET_MASTER(data->dock);
		g_object_set(master, "switcher-style",
			widget_dock_parse_switcher_style(attr), NULL);
		value = attr != NULL ? get_tag_attribute(attr, "locked") : NULL;
		if (value != NULL)
			g_object_set(master, "locked",
				widget_attribute_is_true(value) ? 1 : 0, NULL);
		value = attr != NULL ? get_tag_attribute(attr, "default-title") : NULL;
		if (value != NULL)
			g_object_set(master, "default-title", value, NULL);
		value = attr != NULL ? get_tag_attribute(attr, "show-dockbar") : NULL;
		if (value != NULL)
			show_dockbar = widget_attribute_is_true(value);
		if (show_dockbar) {
			dockbar = gdl_dock_bar_new(GDL_DOCK(data->dock));
			gdl_dock_bar_set_style(GDL_DOCK_BAR(dockbar),
				widget_dock_parse_bar_style(attr));
			gdl_dock_bar_set_orientation(GDL_DOCK_BAR(dockbar),
				widget_dock_parse_bar_orientation(attr));
			gtk_box_pack_start(GTK_BOX(widget), dockbar, FALSE, FALSE, 0);
			gtk_widget_show(dockbar);
		}
		gtk_box_pack_start(GTK_BOX(widget), data->dock, TRUE, TRUE, 0);
		gtk_widget_show(data->dock);
		/* Make an explicitly centred item the stable main area even when it
		 * appears after side panels in the XML. Remaining placements retain
		 * their document order. */
		for (index = 0; index < items.nwidgets; ++index) {
			GdlDockPlacement placement = (GdlDockPlacement)GPOINTER_TO_INT(
				g_object_get_data(G_OBJECT(items.widgets[index]),
					DOCK_ITEM_PLACEMENT_KEY));

			if (placement == GDL_DOCK_CENTER) {
				anchor = index;
				break;
			}
		}
		if (anchor < 0) {
			for (index = 0; index < items.nwidgets; ++index) {
				GdlDockPlacement placement = (GdlDockPlacement)GPOINTER_TO_INT(
					g_object_get_data(G_OBJECT(items.widgets[index]),
						DOCK_ITEM_PLACEMENT_KEY));

				if (placement != GDL_DOCK_FLOATING) {
					anchor = index;
					break;
				}
			}
		}
		attached = g_new0(gboolean, item_count);
		if (anchor >= 0) {
			widget_dock_add_item(data->dock, items.widgets[anchor],
				(GdlDockPlacement)GPOINTER_TO_INT(g_object_get_data(
					G_OBJECT(items.widgets[anchor]),
					DOCK_ITEM_PLACEMENT_KEY)));
			attached[anchor] = TRUE;
		}
		remaining = items.nwidgets - (anchor >= 0 ? 1 : 0);
		while (remaining > 0) {
			gboolean progressed = FALSE;

			for (index = 0; index < items.nwidgets; ++index) {
				GdlDockPlacement placement;

				if (attached[index])
					continue;
				placement = (GdlDockPlacement)GPOINTER_TO_INT(
					g_object_get_data(G_OBJECT(items.widgets[index]),
						DOCK_ITEM_PLACEMENT_KEY));
				if (!widget_dock_item_target_is_attached(data->dock,
					items.widgets[index], placement))
					continue;
				widget_dock_add_item(data->dock, items.widgets[index], placement);
				attached[index] = TRUE;
				--remaining;
				progressed = TRUE;
			}
			if (progressed)
				continue;
			/* Missing targets or cycles cannot be resolved. Attach those items
			 * to the main dock so the XML still produces visible content. */
			for (index = 0; index < items.nwidgets; ++index) {
				GdlDockPlacement placement;

				if (attached[index])
					continue;
				placement = (GdlDockPlacement)GPOINTER_TO_INT(
					g_object_get_data(G_OBJECT(items.widgets[index]),
						DOCK_ITEM_PLACEMENT_KEY));
				widget_dock_add_item(data->dock, items.widgets[index], placement);
				attached[index] = TRUE;
				--remaining;
			}
		}
		g_free(attached);
		g_signal_connect(data->dock, "layout-changed",
			G_CALLBACK(widget_dock_layout_changed), widget);
		g_object_set_data_full(G_OBJECT(widget), DOCK_DATA_KEY, data,
			widget_dock_data_free);
	}
#else
	for (index = 0; index < items.nwidgets; ++index)
		gtk_box_pack_start(GTK_BOX(widget), items.widgets[index], TRUE, TRUE, 0);
	gtkdialog_warning(
		"Dock requested, but this build does not include GDL 2 support.");
#endif
	stackelement_clear(&items);
	widget_dock_kill_attributes(attr, dock_custom_attributes);
	return widget;
}

void widget_dock_clear(variable *var)
{
	(void)var;
	fprintf(stderr, "%s(): Clear not implemented for this widget.\n",
		__func__);
}

gchar *widget_dock_envvar_construct(GtkWidget *widget)
{
	if (widget_dock_is_item(widget)) {
		const gchar *name = g_object_get_data(
			G_OBJECT(widget), DOCK_ITEM_NAME_KEY);

		return g_strdup(name != NULL ? name : "");
	}
#if HAVE_GDL
	if (widget_dock_is_container(widget)) {
		DockData *data = g_object_get_data(G_OBJECT(widget), DOCK_DATA_KEY);

		return g_strdup(data != NULL &&
			gdl_dock_layout_is_dirty(data->layout) ? "true" : "false");
	}
#endif
	return g_strdup("");
}

void widget_dock_fileselect(variable *var, const char *name,
	const char *value)
{
	(void)var;
	(void)name;
	(void)value;
	fprintf(stderr, "%s(): Set-value not implemented for this widget.\n",
		__func__);
}

void widget_dock_refresh(variable *var)
{
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (!initialised &&
		(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		 attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
		gtk_widget_set_sensitive(var->Widget, FALSE);
#if HAVE_GDL
	DockData *data;
	GList *element;
	gchar *input;

	if (!widget_dock_is_container(var->Widget))
		return;
	data = g_object_get_data(G_OBJECT(var->Widget), DOCK_DATA_KEY);
	if (data == NULL)
		return;
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			if (!gdl_dock_layout_load_from_file(data->layout, input + 5) ||
				!gdl_dock_layout_load_layout(data->layout, NULL))
				gtkdialog_warning("Couldn't load GDL layout from '%s'.",
					input + 5);
		} else
			gtkdialog_warning(
				"Dock input requires an <input file> directive.");
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
#else
	(void)initialised;
#endif
}

void widget_dock_save(variable *var)
{
#if HAVE_GDL
	DockData *data;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;

	if (!widget_dock_is_container(var->Widget))
		return;
	data = g_object_get_data(G_OBJECT(var->Widget), DOCK_DATA_KEY);
	if (data == NULL)
		return;
	directive = attributeset_get_first(&element,
		var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (g_ascii_strncasecmp(directive, "file:", 5) == 0 &&
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
	gdl_dock_layout_save_layout(data->layout, NULL);
	if (!gdl_dock_layout_save_to_file(data->layout, filename))
		gtkdialog_warning("Couldn't save GDL layout to '%s'.", filename);
#else
	(void)var;
#endif
}
