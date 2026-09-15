/*
 * widget_canvas.c:
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
#include <errno.h>
#include <math.h>
#include <gtk/gtk.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_canvas.h"

#if HAVE_GOOCANVAS
#include <goocanvas.h>
#endif
#if HAVE_JSON_GLIB
#include <json-glib/json-glib.h>
#endif

#define CANVAS_DATA_KEY "gtkdialog-canvas-data"
#define CANVAS_REQUESTED_KEY "gtkdialog-canvas-requested"
#define CANVAS_MIN_SCALE 0.1
#define CANVAS_MAX_SCALE 16.0

static const gchar *canvas_custom_attributes[] = {
	"goocanvas", "background", "canvas-left", "canvas-top",
	"canvas-right", "canvas-bottom", "scale", NULL
};

static gboolean widget_canvas_requested(tag_attr *attr)
{
	gchar *value;

	if (attr == NULL)
		return FALSE;
	value = get_tag_attribute(attr, "goocanvas");
	return widget_attribute_is_true(value);
}

static void widget_canvas_kill_custom_attributes(tag_attr *attr)
{
	gint index;

	if (attr == NULL)
		return;
	for (index = 0; canvas_custom_attributes[index] != NULL; ++index)
		kill_tag_attribute(attr, canvas_custom_attributes[index]);
}

#if HAVE_GOOCANVAS && HAVE_JSON_GLIB

typedef struct {
	GtkWidget *widget;
	AttributeSet *attributes;
	JsonNode *document;
	gchar *selected;
	gdouble initial_scale;
	gdouble scale;
	gdouble left;
	gdouble top;
	gdouble right;
	gdouble bottom;
	GHashTable *building_ids;
} CanvasData;

typedef struct {
	CanvasData *canvas;
	JsonObject *object;
	gdouble last_x;
	gdouble last_y;
	gboolean draggable;
	gboolean dragging;
	gboolean moved;
} CanvasItemData;

static CanvasData *widget_canvas_get_data(GtkWidget *widget)
{
	return g_object_get_data(G_OBJECT(widget), CANVAS_DATA_KEY);
}

static void widget_canvas_data_free(gpointer value)
{
	CanvasData *data = value;

	if (data->document != NULL)
		json_node_free(data->document);
	g_free(data->selected);
	g_free(data);
}

static gboolean widget_canvas_finite_member(JsonObject *object,
	const gchar *name, gdouble fallback, gdouble *value, GError **error)
{
	JsonNode *node;
	gdouble number;

	if (!json_object_has_member(object, name)) {
		*value = fallback;
		return TRUE;
	}
	node = json_object_get_member(object, name);
	if (!JSON_NODE_HOLDS_VALUE(node) ||
		(json_node_get_value_type(node) != G_TYPE_DOUBLE &&
		json_node_get_value_type(node) != G_TYPE_INT64)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas item member '%s' is not a number", name);
		return FALSE;
	}
	number = json_node_get_double(node);
	if (!isfinite(number)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas item member '%s' is not finite", name);
		return FALSE;
	}
	*value = number;
	return TRUE;
}

static gboolean widget_canvas_string_member(JsonObject *object,
	const gchar *name, gboolean required, const gchar **value, GError **error)
{
	JsonNode *node;

	if (!json_object_has_member(object, name)) {
		if (required) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"canvas item has no '%s' member", name);
			return FALSE;
		}
		*value = NULL;
		return TRUE;
	}
	node = json_object_get_member(object, name);
	if (!JSON_NODE_HOLDS_VALUE(node) ||
		json_node_get_value_type(node) != G_TYPE_STRING) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas item member '%s' is not a string", name);
		return FALSE;
	}
	*value = json_node_get_string(node);
	if (required && (*value == NULL || **value == '\0')) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas item member '%s' is empty", name);
		return FALSE;
	}
	return TRUE;
}

static gboolean widget_canvas_boolean_member(JsonObject *object,
	const gchar *name, gboolean fallback, gboolean *value, GError **error)
{
	JsonNode *node;

	if (!json_object_has_member(object, name)) {
		*value = fallback;
		return TRUE;
	}
	node = json_object_get_member(object, name);
	if (!JSON_NODE_HOLDS_VALUE(node) ||
		json_node_get_value_type(node) != G_TYPE_BOOLEAN) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas item member '%s' is not a boolean", name);
		return FALSE;
	}
	*value = json_node_get_boolean(node);
	return TRUE;
}

static gdouble widget_canvas_translation(JsonObject *object,
	const gchar *name)
{
	JsonNode *node;

	if (!json_object_has_member(object, name))
		return 0.0;
	node = json_object_get_member(object, name);
	if (!JSON_NODE_HOLDS_VALUE(node))
		return 0.0;
	return json_node_get_double(node);
}

static gboolean widget_canvas_item_pressed(GooCanvasItem *item,
	GooCanvasItem *target, GdkEventButton *event, gpointer user_data)
{
	CanvasItemData *item_data = user_data;
	const gchar *id;

	(void)target;
	if (event->button != 1)
		return FALSE;
	id = json_object_get_string_member(item_data->object, "id");
	g_free(item_data->canvas->selected);
	item_data->canvas->selected = g_strdup(id);
	item_data->last_x = event->x;
	item_data->last_y = event->y;
	item_data->moved = FALSE;
	if (item_data->draggable) {
		GdkCursor *cursor = gdk_cursor_new(GDK_FLEUR);
		goo_canvas_pointer_grab(GOO_CANVAS(item_data->canvas->widget), item,
			GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK, cursor,
			event->time);
		gdk_cursor_unref(cursor);
		item_data->dragging = TRUE;
	}
	/* The action can clear the scene or close its window, so it must be the
	 * last operation that can access this item's owned metadata. */
	widget_signal_executor(item_data->canvas->widget,
		item_data->canvas->attributes, "item-activated");
	return TRUE;
}

static gboolean widget_canvas_item_moved(GooCanvasItem *item,
	GooCanvasItem *target, GdkEventMotion *event, gpointer user_data)
{
	CanvasItemData *item_data = user_data;
	gdouble dx;
	gdouble dy;

	(void)target;
	if (!item_data->dragging)
		return FALSE;
	dx = event->x - item_data->last_x;
	dy = event->y - item_data->last_y;
	if (dx != 0.0 || dy != 0.0) {
		goo_canvas_item_translate(item, dx, dy);
		json_object_set_double_member(item_data->object, "translate-x",
			widget_canvas_translation(item_data->object, "translate-x") + dx);
		json_object_set_double_member(item_data->object, "translate-y",
			widget_canvas_translation(item_data->object, "translate-y") + dy);
		item_data->last_x = event->x;
		item_data->last_y = event->y;
		item_data->moved = TRUE;
	}
	return TRUE;
}

static gboolean widget_canvas_item_released(GooCanvasItem *item,
	GooCanvasItem *target, GdkEventButton *event, gpointer user_data)
{
	CanvasItemData *item_data = user_data;

	(void)target;
	if (event->button != 1 || !item_data->dragging)
		return FALSE;
	goo_canvas_pointer_ungrab(GOO_CANVAS(item_data->canvas->widget), item,
		event->time);
	item_data->dragging = FALSE;
	if (item_data->moved)
		widget_signal_executor(item_data->canvas->widget,
			item_data->canvas->attributes, "item-moved");
	return TRUE;
}

static void widget_canvas_apply_property(GooCanvasItem *item,
	JsonObject *object, const gchar *json_name, const gchar *property_name,
	GType expected_type, GError **error)
{
	JsonNode *node;
	GParamSpec *spec;

	if (*error != NULL || !json_object_has_member(object, json_name))
		return;
	spec = g_object_class_find_property(G_OBJECT_GET_CLASS(item), property_name);
	if (spec == NULL)
		return;
	node = json_object_get_member(object, json_name);
	if (!JSON_NODE_HOLDS_VALUE(node) ||
		json_node_get_value_type(node) != expected_type) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas item member '%s' has the wrong type", json_name);
		return;
	}
	if (expected_type == G_TYPE_STRING)
		g_object_set(G_OBJECT(item), property_name,
			json_node_get_string(node), NULL);
	else if (expected_type == G_TYPE_BOOLEAN)
		g_object_set(G_OBJECT(item), property_name,
			json_node_get_boolean(node), NULL);
}

static gboolean widget_canvas_apply_common(GooCanvasItem *item,
	JsonObject *object, CanvasData *data, GError **error)
{
	CanvasItemData *item_data;
	const gchar *id = NULL;
	gboolean draggable;
	gboolean visible;
	gdouble line_width;
	gdouble opacity;
	gdouble rotate;
	gdouble scale;
	gdouble translate_x;
	gdouble translate_y;

	if (!widget_canvas_string_member(object, "id", FALSE, &id, error) ||
		!widget_canvas_boolean_member(object, "visible", TRUE, &visible,
			error) ||
		!widget_canvas_boolean_member(object, "draggable", FALSE,
			&draggable, error) ||
		!widget_canvas_finite_member(object, "line-width", 1.0,
			&line_width, error) ||
		!widget_canvas_finite_member(object, "opacity", 1.0, &opacity,
			error) ||
		!widget_canvas_finite_member(object, "translate-x", 0.0,
			&translate_x, error) ||
		!widget_canvas_finite_member(object, "translate-y", 0.0,
			&translate_y, error) ||
		!widget_canvas_finite_member(object, "scale", 1.0, &scale,
			error) ||
		!widget_canvas_finite_member(object, "rotation", 0.0, &rotate,
			error))
		return FALSE;
	if (line_width < 0.0 || opacity < 0.0 || opacity > 1.0 || scale <= 0.0) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas line-width, opacity or scale is outside its valid range");
		return FALSE;
	}
	widget_canvas_apply_property(item, object, "fill-color", "fill-color",
		G_TYPE_STRING, error);
	widget_canvas_apply_property(item, object, "stroke-color", "stroke-color",
		G_TYPE_STRING, error);
	widget_canvas_apply_property(item, object, "font", "font",
		G_TYPE_STRING, error);
	widget_canvas_apply_property(item, object, "tooltip", "tooltip",
		G_TYPE_STRING, error);
	if (*error != NULL)
		return FALSE;
	if (g_object_class_find_property(G_OBJECT_GET_CLASS(item), "line-width"))
		g_object_set(G_OBJECT(item), "line-width", line_width, NULL);
	if (g_object_class_find_property(G_OBJECT_GET_CLASS(item), "visibility"))
		g_object_set(G_OBJECT(item), "visibility", visible ?
			GOO_CANVAS_ITEM_VISIBLE : GOO_CANVAS_ITEM_HIDDEN, NULL);
	if (g_object_class_find_property(G_OBJECT_GET_CLASS(item), "opacity"))
		g_object_set(G_OBJECT(item), "opacity", (guint)(opacity * 255.0 + 0.5),
			NULL);
	goo_canvas_item_set_simple_transform(item, translate_x, translate_y,
		scale, rotate);
	if (id != NULL && *id != '\0') {
		if (g_hash_table_lookup(data->building_ids, id) != NULL) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"duplicate canvas item id '%s'", id);
			return FALSE;
		}
		g_hash_table_insert(data->building_ids, g_strdup(id),
			GINT_TO_POINTER(TRUE));
		item_data = g_new0(CanvasItemData, 1);
		item_data->canvas = data;
		item_data->object = object;
		item_data->draggable = draggable;
		g_object_set_data_full(G_OBJECT(item), "gtkdialog-canvas-item-data",
			item_data, g_free);
		g_signal_connect(G_OBJECT(item), "button-press-event",
			G_CALLBACK(widget_canvas_item_pressed), item_data);
		g_signal_connect(G_OBJECT(item), "motion-notify-event",
			G_CALLBACK(widget_canvas_item_moved), item_data);
		g_signal_connect(G_OBJECT(item), "button-release-event",
			G_CALLBACK(widget_canvas_item_released), item_data);
	}
	return TRUE;
}

static GtkAnchorType widget_canvas_anchor(const gchar *value,
	GError **error)
{
	static const gchar *names[] = { "center", "north", "north-west",
		"north-east", "south", "south-west", "south-east", "west",
		"east", NULL };
	static const GtkAnchorType values[] = { GTK_ANCHOR_CENTER, GTK_ANCHOR_N,
		GTK_ANCHOR_NW, GTK_ANCHOR_NE, GTK_ANCHOR_S, GTK_ANCHOR_SW,
		GTK_ANCHOR_SE, GTK_ANCHOR_W, GTK_ANCHOR_E };
	gint index;

	if (value == NULL)
		return GTK_ANCHOR_NW;
	for (index = 0; names[index] != NULL; ++index)
		if (g_ascii_strcasecmp(value, names[index]) == 0)
			return values[index];
	g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
		"invalid canvas text anchor '%s'", value);
	return GTK_ANCHOR_NW;
}

static GooCanvasItem *widget_canvas_build_item(CanvasData *data,
	GooCanvasItem *parent, JsonObject *object, GError **error);

static gboolean widget_canvas_build_children(CanvasData *data,
	GooCanvasItem *parent, JsonObject *object, GError **error)
{
	JsonArray *children;
	JsonNode *node;
	guint index;

	if (!json_object_has_member(object, "children"))
		return TRUE;
	node = json_object_get_member(object, "children");
	if (!JSON_NODE_HOLDS_ARRAY(node)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas group children is not an array");
		return FALSE;
	}
	children = json_node_get_array(node);
	for (index = 0; index < json_array_get_length(children); ++index) {
		node = json_array_get_element(children, index);
		if (!JSON_NODE_HOLDS_OBJECT(node)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"canvas child %u is not an object", index);
			return FALSE;
		}
		if (widget_canvas_build_item(data, parent,
			json_node_get_object(node), error) == NULL)
			return FALSE;
	}
	return TRUE;
}

static GooCanvasItem *widget_canvas_build_polyline(GooCanvasItem *parent,
	JsonObject *object, GError **error)
{
	GooCanvasItem *item;
	GooCanvasPoints *points;
	JsonArray *array;
	JsonNode *node;
	gboolean closed;
	gdouble value;
	guint index;
	guint length;

	if (!json_object_has_member(object, "points")) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas polyline has no 'points' member");
		return NULL;
	}
	node = json_object_get_member(object, "points");
	if (!JSON_NODE_HOLDS_ARRAY(node)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas polyline points is not an array");
		return NULL;
	}
	array = json_node_get_array(node);
	length = json_array_get_length(array);
	if (length < 4 || length % 2 != 0) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas polyline needs an even array of at least four numbers");
		return NULL;
	}
	points = goo_canvas_points_new(length / 2);
	for (index = 0; index < length; ++index) {
		node = json_array_get_element(array, index);
		if (!JSON_NODE_HOLDS_VALUE(node) ||
			(json_node_get_value_type(node) != G_TYPE_DOUBLE &&
			json_node_get_value_type(node) != G_TYPE_INT64)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"canvas polyline coordinate %u is not a number", index);
			goo_canvas_points_unref(points);
			return NULL;
		}
		value = json_node_get_double(node);
		if (!isfinite(value)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"canvas polyline coordinate %u is not finite", index);
			goo_canvas_points_unref(points);
			return NULL;
		}
		points->coords[index] = value;
	}
	if (!widget_canvas_boolean_member(object, "closed", FALSE, &closed,
		error)) {
		goo_canvas_points_unref(points);
		return NULL;
	}
	item = goo_canvas_polyline_new(parent, closed, 0, "points", points, NULL);
	goo_canvas_points_unref(points);
	return item;
}

static GooCanvasItem *widget_canvas_build_item(CanvasData *data,
	GooCanvasItem *parent, JsonObject *object, GError **error)
{
	GdkPixbuf *pixbuf;
	GooCanvasItem *item = NULL;
	GtkAnchorType anchor;
	const gchar *anchor_name = NULL;
	const gchar *filename = NULL;
	const gchar *text = NULL;
	const gchar *type = NULL;
	gchar *resolved;
	gdouble a;
	gdouble b;
	gdouble c;
	gdouble d;
	GError *pixbuf_error = NULL;

	if (!widget_canvas_string_member(object, "type", TRUE, &type, error))
		return NULL;
	if (g_ascii_strcasecmp(type, "group") == 0) {
		item = goo_canvas_group_new(parent, NULL);
	} else if (g_ascii_strcasecmp(type, "rect") == 0) {
		if (!widget_canvas_finite_member(object, "x", 0, &a, error) ||
			!widget_canvas_finite_member(object, "y", 0, &b, error) ||
			!widget_canvas_finite_member(object, "width", 0, &c, error) ||
			!widget_canvas_finite_member(object, "height", 0, &d, error))
			return NULL;
		item = goo_canvas_rect_new(parent, a, b, c, d, NULL);
	} else if (g_ascii_strcasecmp(type, "ellipse") == 0) {
		if (!widget_canvas_finite_member(object, "center-x", 0, &a, error) ||
			!widget_canvas_finite_member(object, "center-y", 0, &b, error) ||
			!widget_canvas_finite_member(object, "radius-x", 0, &c, error) ||
			!widget_canvas_finite_member(object, "radius-y", 0, &d, error))
			return NULL;
		item = goo_canvas_ellipse_new(parent, a, b, c, d, NULL);
	} else if (g_ascii_strcasecmp(type, "text") == 0) {
		if (!widget_canvas_string_member(object, "text", TRUE, &text, error) ||
			!widget_canvas_string_member(object, "anchor", FALSE,
				&anchor_name, error) ||
			!widget_canvas_finite_member(object, "x", 0, &a, error) ||
			!widget_canvas_finite_member(object, "y", 0, &b, error) ||
			!widget_canvas_finite_member(object, "width", -1, &c, error))
			return NULL;
		anchor = widget_canvas_anchor(anchor_name, error);
		if (*error != NULL)
			return NULL;
		item = goo_canvas_text_new(parent, text, a, b, c, anchor, NULL);
	} else if (g_ascii_strcasecmp(type, "image") == 0) {
		if (!widget_canvas_string_member(object, "file", TRUE, &filename,
			error) ||
			!widget_canvas_finite_member(object, "x", 0, &a, error) ||
			!widget_canvas_finite_member(object, "y", 0, &b, error))
			return NULL;
		resolved = find_pixmap(filename);
		pixbuf = gdk_pixbuf_new_from_file(resolved, &pixbuf_error);
		g_free(resolved);
		if (pixbuf == NULL) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"could not load canvas image '%s': %s", filename,
				pixbuf_error != NULL ? pixbuf_error->message : "unknown error");
			if (pixbuf_error != NULL)
				g_error_free(pixbuf_error);
			return NULL;
		}
		item = goo_canvas_image_new(parent, pixbuf, a, b, NULL);
		g_object_unref(pixbuf);
	} else if (g_ascii_strcasecmp(type, "path") == 0) {
		if (!widget_canvas_string_member(object, "data", TRUE, &text, error))
			return NULL;
		item = goo_canvas_path_new(parent, text, NULL);
	} else if (g_ascii_strcasecmp(type, "polyline") == 0) {
		item = widget_canvas_build_polyline(parent, object, error);
	} else {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"unknown canvas item type '%s'", type);
		return NULL;
	}
	if (item == NULL || !widget_canvas_apply_common(item, object, data, error))
		return NULL;
	if (g_ascii_strcasecmp(type, "group") == 0 &&
		!widget_canvas_build_children(data, item, object, error))
		return NULL;
	return item;
}

static gboolean widget_canvas_validate_document(JsonNode *root,
	JsonArray **items, GError **error)
{
	JsonNode *member;
	JsonObject *document;

	if (root == NULL || !JSON_NODE_HOLDS_OBJECT(root)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas JSON document root is not an object");
		return FALSE;
	}
	document = json_node_get_object(root);
	member = json_object_get_member(document, "version");
	if (member == NULL || !JSON_NODE_HOLDS_VALUE(member) ||
		json_node_get_value_type(member) != G_TYPE_INT64 ||
		json_node_get_int(member) != 1) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas JSON document version must be the integer 1");
		return FALSE;
	}
	member = json_object_get_member(document, "items");
	if (member == NULL || !JSON_NODE_HOLDS_ARRAY(member)) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"canvas JSON document has no items array");
		return FALSE;
	}
	*items = json_node_get_array(member);
	return TRUE;
}

static gboolean widget_canvas_apply_document(GtkWidget *widget,
	JsonNode *document, const gchar *source)
{
	CanvasData *data;
	GooCanvasItem *root;
	JsonArray *items;
	JsonNode *node;
	GError *error = NULL;
	guint index;

	data = widget_canvas_get_data(widget);
	if (!widget_canvas_validate_document(document, &items, &error))
		goto invalid;
	data->building_ids = g_hash_table_new_full(g_str_hash, g_str_equal,
		g_free, NULL);
	root = goo_canvas_group_new(NULL, NULL);
	for (index = 0; index < json_array_get_length(items); ++index) {
		node = json_array_get_element(items, index);
		if (!JSON_NODE_HOLDS_OBJECT(node)) {
			g_set_error(&error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"canvas item %u is not an object", index);
			break;
		}
		if (widget_canvas_build_item(data, root,
			json_node_get_object(node), &error) == NULL)
			break;
	}
	if (error != NULL) {
		g_object_unref(root);
		g_hash_table_destroy(data->building_ids);
		data->building_ids = NULL;
		goto invalid;
	}
	g_hash_table_destroy(data->building_ids);
	data->building_ids = NULL;
	goo_canvas_set_root_item(GOO_CANVAS(widget), root);
	g_object_unref(root);
	if (data->document != NULL)
		json_node_free(data->document);
	data->document = document;
	g_free(data->selected);
	data->selected = NULL;
	return TRUE;

invalid:
	gtkdialog_warning("Invalid canvas JSON%s%s: %s", source ? " from " : "",
		source ? source : "", error != NULL ? error->message : "unknown error");
	if (error != NULL)
		g_error_free(error);
	return FALSE;
}

static JsonNode *widget_canvas_parse_document(const gchar *contents,
	const gchar *source)
{
	JsonParser *parser;
	JsonNode *document = NULL;
	GError *error = NULL;

	parser = json_parser_new();
	if (json_parser_load_from_data(parser, contents, -1, &error))
		document = json_node_copy(json_parser_get_root(parser));
	else {
		gtkdialog_warning("Couldn't parse canvas JSON%s%s: %s",
			source ? " from " : "", source ? source : "", error->message);
		g_error_free(error);
	}
	g_object_unref(parser);
	return document;
}

static gchar *widget_canvas_read_stream(FILE *stream)
{
	return widget_read_all(stream);
}

static JsonNode *widget_canvas_read_input(const gchar *source,
	gboolean command)
{
	FILE *stream;
	gchar *contents;
	JsonNode *document;

	stream = command ? widget_opencommand(source) : fopen(source, "r");
	if (stream == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, source);
		return NULL;
	}
	contents = widget_canvas_read_stream(stream);
	if (command)
		widget_closecommand(stream, source);
	else
		fclose(stream);
	if (contents == NULL) {
		gtkdialog_warning("Couldn't read canvas JSON from '%s'.", source);
		return NULL;
	}
	document = widget_canvas_parse_document(contents, source);
	g_free(contents);
	return document;
}

static gboolean widget_canvas_append_document(JsonBuilder *builder,
	JsonNode *document, const gchar *source)
{
	JsonArray *items;
	GError *error = NULL;
	guint index;

	if (!widget_canvas_validate_document(document, &items, &error)) {
		gtkdialog_warning("Invalid canvas JSON from %s: %s", source,
			error->message);
		g_error_free(error);
		return FALSE;
	}
	for (index = 0; index < json_array_get_length(items); ++index)
		json_builder_add_value(builder,
			json_node_copy(json_array_get_element(items, index)));
	return TRUE;
}

static gboolean widget_canvas_append_item(JsonBuilder *builder,
	const gchar *contents)
{
	JsonParser *parser;
	JsonNode *root;
	GError *error = NULL;
	gboolean valid = FALSE;

	parser = json_parser_new();
	if (!json_parser_load_from_data(parser, contents, -1, &error)) {
		gtkdialog_warning("Couldn't parse canvas item JSON: %s", error->message);
		g_error_free(error);
	} else {
		root = json_parser_get_root(parser);
		if (root != NULL && JSON_NODE_HOLDS_OBJECT(root)) {
			json_builder_add_value(builder, json_node_copy(root));
			valid = TRUE;
		} else {
			gtkdialog_warning("Canvas <item> JSON is not an object.");
		}
	}
	g_object_unref(parser);
	return valid;
}

static void widget_canvas_parse_attribute_double(tag_attr *attr,
	const gchar *name, gdouble *value)
{
	gchar *text;
	gdouble parsed;

	if (attr == NULL || (text = get_tag_attribute(attr, name)) == NULL)
		return;
	if (widget_parse_finite_double(text, &parsed, name))
		*value = parsed;
}

#endif

GtkWidget *widget_canvas_create(AttributeSet *attributes, tag_attr *attr,
	gint width, gint height)
{
	GdkColor background;
	gboolean have_background = FALSE;
	gchar *background_value;
	GtkWidget *widget;

	(void)attributes;
	if (!widget_canvas_requested(attr))
		return NULL;
	background_value = get_tag_attribute(attr, "background");
	if (background_value != NULL) {
		have_background = gdk_color_parse(background_value, &background);
		if (!have_background)
			gtkdialog_warning("Invalid GooCanvas background '%s'; using theme colour.",
				background_value);
	}
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	{
		CanvasData *data;

		widget = goo_canvas_new();
		data = g_new0(CanvasData, 1);
		data->widget = widget;
		data->attributes = attributes;
		data->left = 0.0;
		data->top = 0.0;
		data->right = 640.0;
		data->bottom = 400.0;
		data->initial_scale = 1.0;
		widget_canvas_parse_attribute_double(attr, "canvas-left", &data->left);
		widget_canvas_parse_attribute_double(attr, "canvas-top", &data->top);
		widget_canvas_parse_attribute_double(attr, "canvas-right", &data->right);
		widget_canvas_parse_attribute_double(attr, "canvas-bottom", &data->bottom);
		widget_canvas_parse_attribute_double(attr, "scale", &data->initial_scale);
		if (data->right <= data->left || data->bottom <= data->top) {
			gtkdialog_warning("Invalid GooCanvas bounds; using 0,0,640,400.");
			data->left = data->top = 0.0;
			data->right = 640.0;
			data->bottom = 400.0;
		}
		if (data->initial_scale < CANVAS_MIN_SCALE ||
			data->initial_scale > CANVAS_MAX_SCALE) {
			gtkdialog_warning("Invalid GooCanvas scale; using 1.0.");
			data->initial_scale = 1.0;
		}
		data->scale = data->initial_scale;
		goo_canvas_set_bounds(GOO_CANVAS(widget), data->left, data->top,
			data->right, data->bottom);
		goo_canvas_set_scale(GOO_CANVAS(widget), data->scale);
		g_object_set_data_full(G_OBJECT(widget), CANVAS_DATA_KEY, data,
			widget_canvas_data_free);
	}
#else
	widget = gtk_drawing_area_new();
	gtkdialog_warning(
		"GooCanvas drawing area requested, but this build does not include both GooCanvas 1 and JSON-GLib support.");
#endif
	widget_canvas_kill_custom_attributes(attr);
	g_object_set_data(G_OBJECT(widget), CANVAS_REQUESTED_KEY,
		GINT_TO_POINTER(TRUE));
	if (have_background)
		gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &background);
	gtk_widget_set_size_request(widget, width == -1 ? 480 : width,
		height == -1 ? 300 : height);
	return widget;
}

gboolean widget_canvas_is_requested(GtkWidget *widget)
{
	return widget != NULL && GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), CANVAS_REQUESTED_KEY));
}

gboolean widget_canvas_is_widget(GtkWidget *widget)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	return widget != NULL && GOO_IS_CANVAS(widget);
#else
	(void)widget;
	return FALSE;
#endif
}

void widget_canvas_clear(variable *var)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	CanvasData *data;
	GooCanvasItem *root;

	if (!widget_canvas_is_widget(var->Widget))
		return;
	data = widget_canvas_get_data(var->Widget);
	root = goo_canvas_group_new(NULL, NULL);
	goo_canvas_set_root_item(GOO_CANVAS(var->Widget), root);
	g_object_unref(root);
	if (data->document != NULL) {
		json_node_free(data->document);
		data->document = NULL;
	}
	g_free(data->selected);
	data->selected = NULL;
#else
	(void)var;
#endif
}

gchar *widget_canvas_envvar_construct(GtkWidget *widget)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	CanvasData *data;

	if (!widget_canvas_is_widget(widget))
		return g_strdup("");
	data = widget_canvas_get_data(widget);
	return g_strdup(data->selected != NULL ? data->selected : "");
#else
	(void)widget;
	return g_strdup("");
#endif
}

void widget_canvas_set_value(variable *var, const gchar *value)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	JsonNode *document;

	if (!widget_canvas_is_widget(var->Widget))
		return;
	document = widget_canvas_parse_document(value, NULL);
	if (document != NULL &&
		!widget_canvas_apply_document(var->Widget, document, NULL))
		json_node_free(document);
#else
	(void)var;
	(void)value;
#endif
}

void widget_canvas_refresh(variable *var)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	JsonBuilder *builder;
	JsonNode *combined;
	JsonNode *document;
	GList *element;
	gchar *input;
	gchar *item;
	gboolean initialised;
	gboolean valid = TRUE;

	if (!widget_canvas_is_widget(var->Widget))
		return;
	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	builder = json_builder_new();
	json_builder_begin_object(builder);
	json_builder_set_member_name(builder, "version");
	json_builder_add_int_value(builder, 1);
	json_builder_set_member_name(builder, "items");
	json_builder_begin_array(builder);
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT)) {
		item = attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT);
		document = widget_canvas_parse_document(item, "<default>");
		if (document == NULL || !widget_canvas_append_document(builder,
			document, "<default>"))
			valid = FALSE;
		if (document != NULL)
			json_node_free(document);
	}
	item = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (item != NULL && valid) {
		valid = widget_canvas_append_item(builder, item);
		item = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL && valid) {
		document = NULL;
		if (input_is_shell_command(input))
			document = widget_canvas_read_input(input + 8, TRUE);
		else if (g_ascii_strncasecmp(input, "file:", 5) == 0 &&
			strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			document = widget_canvas_read_input(input + 5, FALSE);
		}
		if (document == NULL || !widget_canvas_append_document(builder,
			document, input))
			valid = FALSE;
		if (document != NULL)
			json_node_free(document);
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	json_builder_end_array(builder);
	json_builder_end_object(builder);
	combined = json_builder_get_root(builder);
	if (valid) {
		combined = json_node_copy(combined);
		if (!widget_canvas_apply_document(var->Widget, combined, "refresh"))
			json_node_free(combined);
	}
	g_object_unref(builder);
	if (!initialised &&
		(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
		gtk_widget_set_sensitive(var->Widget, FALSE);
#else
	(void)var;
#endif
}

void widget_canvas_save(variable *var)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	CanvasData *data;
	JsonGenerator *generator;
	GList *element;
	gchar *directive;
	gchar *filename = NULL;
	GError *error = NULL;

	if (!widget_canvas_is_widget(var->Widget))
		return;
	directive = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
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
		fprintf(stderr, "%s(): No <output file> directive found.\n", __func__);
		return;
	}
	data = widget_canvas_get_data(var->Widget);
	if (data->document == NULL) {
		fprintf(stderr, "%s(): No canvas scene to save.\n", __func__);
		return;
	}
	generator = json_generator_new();
	json_generator_set_root(generator, data->document);
	json_generator_set_pretty(generator, TRUE);
	if (!json_generator_to_file(generator, filename, &error)) {
		gtkdialog_warning("Couldn't save canvas JSON '%s': %s", filename,
			error->message);
		g_error_free(error);
	}
	g_object_unref(generator);
#else
	(void)var;
#endif
}

void widget_canvas_zoom_in(variable *var)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	CanvasData *data;

	if (!widget_canvas_is_widget(var->Widget))
		return;
	data = widget_canvas_get_data(var->Widget);
	data->scale = MIN(CANVAS_MAX_SCALE, data->scale * 1.25);
	goo_canvas_set_scale(GOO_CANVAS(var->Widget), data->scale);
#else
	(void)var;
#endif
}

void widget_canvas_zoom_out(variable *var)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	CanvasData *data;

	if (!widget_canvas_is_widget(var->Widget))
		return;
	data = widget_canvas_get_data(var->Widget);
	data->scale = MAX(CANVAS_MIN_SCALE, data->scale / 1.25);
	goo_canvas_set_scale(GOO_CANVAS(var->Widget), data->scale);
#else
	(void)var;
#endif
}

void widget_canvas_zoom_reset(variable *var)
{
#if HAVE_GOOCANVAS && HAVE_JSON_GLIB
	CanvasData *data;

	if (!widget_canvas_is_widget(var->Widget))
		return;
	data = widget_canvas_get_data(var->Widget);
	data->scale = data->initial_scale;
	goo_canvas_set_scale(GOO_CANVAS(var->Widget), data->scale);
#else
	(void)var;
#endif
}
