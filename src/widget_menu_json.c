/*
 * widget_menu_json.c: Optional structured GTK2 menu input.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "widgets.h"
#include "widget_menu_json.h"

#if HAVE_JSON_GLIB
#include <json-glib/json-glib.h>

#define JSON_MENU_ITEM_KEY "gtkdialog-json-menu-item"

static gboolean menu_json_string(JsonObject *object, const gchar *name,
	const gchar **value, GError **error)
{
	JsonNode *node = json_object_get_member(object, name);

	*value = NULL;
	if (node == NULL)
		return TRUE;
	if (!JSON_NODE_HOLDS_VALUE(node) ||
		json_node_get_value_type(node) != G_TYPE_STRING) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu item '%s' must be a string", name);
		return FALSE;
	}
	*value = json_node_get_string(node);
	return TRUE;
}

static gboolean menu_json_boolean(JsonObject *object, const gchar *name,
	gboolean fallback, gboolean *value, GError **error)
{
	JsonNode *node = json_object_get_member(object, name);

	*value = fallback;
	if (node == NULL)
		return TRUE;
	if (!JSON_NODE_HOLDS_VALUE(node) ||
		json_node_get_value_type(node) != G_TYPE_BOOLEAN) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu item '%s' must be a boolean", name);
		return FALSE;
	}
	*value = json_node_get_boolean(node);
	return TRUE;
}

static void menu_json_discard(GtkWidget *widget)
{
	gtk_widget_destroy(widget);
	g_object_unref(widget);
}

static GtkWidget *menu_json_build_item(JsonNode *node, guint depth,
	GError **error)
{
	JsonObject *object;
	JsonNode *children_node, *actions_node;
	JsonArray *children, *actions;
	GtkWidget *item = NULL, *child, *submenu, *label_widget, *image;
	AttributeSet *action_set = NULL;
	GdkColor color;
	PangoFontDescription *font_desc;
	const gchar *type, *label, *stock, *icon;
	const gchar *foreground, *font, *tooltip, *action;
	gboolean markup, sensitive, visible;
	guint i;

	if (depth > 32 || !JSON_NODE_HOLDS_OBJECT(node)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu nesting exceeds 32 levels or an item is not an object");
		return NULL;
	}
	object = json_node_get_object(node);
	if (!menu_json_string(object, "type", &type, error) ||
		!menu_json_string(object, "label", &label, error) ||
		!menu_json_string(object, "stock", &stock, error) ||
		!menu_json_string(object, "icon-name", &icon, error) ||
		!menu_json_string(object, "foreground", &foreground, error) ||
		!menu_json_string(object, "font", &font, error) ||
		!menu_json_string(object, "tooltip", &tooltip, error) ||
		!menu_json_string(object, "action", &action, error) ||
		!menu_json_boolean(object, "markup", FALSE, &markup, error) ||
		!menu_json_boolean(object, "sensitive", TRUE, &sensitive, error) ||
		!menu_json_boolean(object, "visible", TRUE, &visible, error))
		return NULL;

	children_node = json_object_get_member(object, "items");
	if (children_node != NULL && !JSON_NODE_HOLDS_ARRAY(children_node)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu item 'items' must be an array");
		return NULL;
	}
	actions_node = json_object_get_member(object, "actions");
	if (actions_node != NULL && !JSON_NODE_HOLDS_ARRAY(actions_node)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu item 'actions' must be an array");
		return NULL;
	}
	if (type == NULL)
		type = children_node != NULL ? "menu" : "item";
	if (g_strcmp0(type, "separator") == 0) {
		if (children_node != NULL || action != NULL || actions_node != NULL) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"menu separator cannot have items or actions");
			return NULL;
		}
		item = gtk_separator_menu_item_new();
	} else if (g_strcmp0(type, "item") == 0 ||
		g_strcmp0(type, "menu") == 0) {
		if (label == NULL) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"menu item requires a label");
			return NULL;
		}
		if (g_strcmp0(type, "menu") == 0 && children_node == NULL) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"menu submenu requires an items array");
			return NULL;
		}
		if (stock != NULL || icon != NULL)
			item = gtk_image_menu_item_new_with_label(label);
		else
			item = gtk_menu_item_new_with_label(label);
	} else {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"unknown menu item type '%s'", type);
		return NULL;
	}
	g_object_ref_sink(item);

	if (label != NULL && !GTK_IS_SEPARATOR_MENU_ITEM(item)) {
		label_widget = gtk_bin_get_child(GTK_BIN(item));
		if (markup) {
			if (!pango_parse_markup(label, -1, 0, NULL, NULL, NULL, error))
				goto fail;
			gtk_label_set_markup(GTK_LABEL(label_widget), label);
		}
		if (foreground != NULL) {
			if (!gdk_color_parse(foreground, &color)) {
				g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
					"invalid menu foreground '%s'", foreground);
				goto fail;
			}
			gtk_widget_modify_fg(label_widget, GTK_STATE_NORMAL, &color);
		}
		if (font != NULL) {
			font_desc = pango_font_description_from_string(font);
			gtk_widget_modify_font(label_widget, font_desc);
			pango_font_description_free(font_desc);
		}
		if (stock != NULL || icon != NULL) {
			if (stock != NULL)
				image = gtk_image_new_from_stock(stock, GTK_ICON_SIZE_MENU);
			else
				image = gtk_image_new_from_icon_name(icon, GTK_ICON_SIZE_MENU);
			gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item), image);
		}
		if (tooltip != NULL)
			gtk_widget_set_tooltip_text(item, tooltip);
	}

	if (children_node != NULL) {
		children = json_node_get_array(children_node);
		submenu = gtk_menu_new();
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
		for (i = 0; i < json_array_get_length(children); ++i) {
			child = menu_json_build_item(json_array_get_element(children, i),
				depth + 1, error);
			if (child == NULL)
				goto fail;
			gtk_menu_shell_append(GTK_MENU_SHELL(submenu), child);
			g_object_unref(child);
		}
	}
	if (action != NULL || actions_node != NULL) {
		action_set = attributeset_new();
		if (action != NULL)
			attributeset_insert(action_set, ATTR_ACTION, action);
		if (actions_node != NULL) {
			actions = json_node_get_array(actions_node);
			for (i = 0; i < json_array_get_length(actions); ++i) {
				JsonNode *action_node = json_array_get_element(actions, i);
				if (!JSON_NODE_HOLDS_VALUE(action_node) ||
					json_node_get_value_type(action_node) != G_TYPE_STRING) {
					g_set_error(error, G_MARKUP_ERROR,
						G_MARKUP_ERROR_INVALID_CONTENT,
						"menu actions must contain strings");
					attributeset_destroy(action_set);
					goto fail;
				}
				attributeset_insert(action_set, ATTR_ACTION,
					json_node_get_string(action_node));
			}
		}
		g_object_set_data_full(G_OBJECT(item), "gtkdialog-json-menu-actions",
			action_set, attributeset_destroy);
		g_signal_connect(item, "activate",
			G_CALLBACK(on_any_widget_activate_event), action_set);
	}
	gtk_widget_set_sensitive(item, sensitive);
	if (!visible) {
		gtk_widget_set_no_show_all(item, TRUE);
		gtk_widget_hide(item);
	}
	return item;

fail:
	menu_json_discard(item);
	return NULL;
}

static gboolean menu_json_read_items(const gchar *filename, GPtrArray *items,
	GError **error)
{
	JsonParser *parser;
	JsonNode *root, *member;
	JsonObject *document;
	JsonArray *array;
	gchar *contents;
	GtkWidget *item;
	guint i;

	if (!g_file_get_contents(filename, &contents, NULL, error))
		return FALSE;
	parser = json_parser_new();
	if (!json_parser_load_from_data(parser, contents, -1, error)) {
		g_free(contents);
		g_object_unref(parser);
		return FALSE;
	}
	g_free(contents);
	root = json_parser_get_root(parser);
	if (root == NULL || !JSON_NODE_HOLDS_OBJECT(root)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu JSON root must be an object");
		goto fail;
	}
	document = json_node_get_object(root);
	member = json_object_get_member(document, "version");
	if (member == NULL || !JSON_NODE_HOLDS_VALUE(member) ||
		json_node_get_value_type(member) != G_TYPE_INT64 ||
		json_node_get_int(member) != 1) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu JSON version must be the integer 1");
		goto fail;
	}
	member = json_object_get_member(document, "items");
	if (member == NULL || !JSON_NODE_HOLDS_ARRAY(member)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"menu JSON root requires an items array");
		goto fail;
	}
	array = json_node_get_array(member);
	for (i = 0; i < json_array_get_length(array); ++i) {
		item = menu_json_build_item(json_array_get_element(array, i), 0, error);
		if (item == NULL)
			goto fail;
		g_ptr_array_add(items, item);
	}
	g_object_unref(parser);
	return TRUE;

fail:
	g_object_unref(parser);
	return FALSE;
}
#endif

void widget_menu_json_refresh(variable *var, GtkWidget *shell,
	const gchar *source, gboolean initialised)
{
#if HAVE_JSON_GLIB
	GPtrArray *items;
	GList *children, *element;
	GError *error = NULL;
	guint i;

	if (!GTK_IS_MENU_SHELL(shell) ||
		g_ascii_strncasecmp(source, "file:", 5) != 0 || source[5] == '\0') {
		gtkdialog_warning("Menu JSON input requires an input file.");
		return;
	}
	if (!initialised)
		widget_file_monitor_try_create(var, (gchar *)source + 5);
	items = g_ptr_array_new();
	if (!menu_json_read_items(source + 5, items, &error)) {
		gtkdialog_warning("Couldn't load menu JSON input '%s': %s",
			source + 5, error->message);
		g_error_free(error);
		for (i = 0; i < items->len; ++i)
			menu_json_discard(g_ptr_array_index(items, i));
		g_ptr_array_free(items, TRUE);
		return;
	}
	children = gtk_container_get_children(GTK_CONTAINER(shell));
	for (element = children; element != NULL; element = element->next) {
		GtkWidget *child = element->data;
		if (g_object_get_data(G_OBJECT(child), JSON_MENU_ITEM_KEY) != NULL)
			gtk_widget_destroy(child);
	}
	g_list_free(children);
	for (i = 0; i < items->len; ++i) {
		GtkWidget *item = g_ptr_array_index(items, i);
		g_object_set_data(G_OBJECT(item), JSON_MENU_ITEM_KEY,
			GINT_TO_POINTER(1));
		gtk_menu_shell_append(GTK_MENU_SHELL(shell), item);
		gtk_widget_show_all(item);
		g_object_unref(item);
	}
	g_ptr_array_free(items, TRUE);
#else
	(void)var;
	(void)shell;
	(void)source;
	(void)initialised;
	gtkdialog_warning("Menu JSON input requires JSON-GLib support.");
#endif
}
