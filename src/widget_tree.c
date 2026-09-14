/*
 * widget_tree.c: 
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2003-2007  László Pere <pipas@linux.pte.hu>
 * Copyright (C) 2011-2012  Thunor <thunorsif@hotmail.com>
 * Copyright (C) 2026       Artur Bednarek <artur@unix.org.pl>
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
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* Includes */
#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <errno.h>
#include <math.h>

#if GTK_CHECK_VERSION(2,4,0)

#include "config.h"
#if HAVE_JSON_GLIB
#include <json-glib/json-glib.h>
#endif
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "stringman.h"
#include "tag_attributes.h"

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS

#define TREE_JSON_MAX_DEPTH 256

/* Local variables */
typedef enum {
	ColumnPixbuf,
	ColumnIconName,
	ColumnStockId,
	ColumnJsonStyle,
	FirstDataColumn
} treecolumns;

typedef struct {
	gint column;
	gboolean background_set;
	GdkColor *background;
	gfloat xalign;
	gfloat yalign;
	guint xpad;
	guint ypad;
	gboolean text_renderer;
	gboolean foreground_set;
	GdkColor *foreground;
	PangoFontDescription *font_desc;
	PangoStyle font_style;
	gboolean font_style_set;
	gint weight;
	gboolean weight_set;
	PangoUnderline underline;
	gboolean underline_set;
	gboolean strikethrough;
	gboolean strikethrough_set;
	PangoEllipsizeMode ellipsize;
	gboolean ellipsize_set;
	PangoWrapMode wrap_mode;
	gint wrap_width;
	PangoAlignment text_align;
	gboolean text_align_set;
} TreeRendererStyle;

typedef struct {
	GtkWidget *tree_view;
	AttributeSet *attributes;
	gint model_column;
	gboolean radio;
} TreeCellData;

typedef struct {
	const gchar *icon_name;
	const gchar *stock_id;
	gint icon_column;
	gint stock_column;
} TreeInputContext;

typedef struct {
	list_t *background;
	list_t *ellipsize;
	list_t *font;
	list_t *foreground;
	list_t *strikethrough;
	list_t *style;
	list_t *text_align;
	list_t *underline;
	list_t *weight;
	list_t *wrap_mode;
	list_t *wrap_width;
	list_t *xalign;
	list_t *xpad;
	list_t *yalign;
	list_t *ypad;
} TreeColumnStyles;

typedef enum {
	TreeImageIconName,
	TreeImageStockId,
	TreeImageFile
} TreeImageSource;

typedef struct {
	GtkWidget *tree_view;
	gint column;
	gint model_column;
	TreeImageSource source;
	gboolean value_is_source;
	gchar *icon_name;
	gchar *stock_id;
	gchar *filename;
	gint size;
	GHashTable *cache;
	GHashTable *failed;
} TreeImageData;

typedef struct {
	list_t *filename;
	list_t *icon_name;
	list_t *size;
	list_t *source;
	list_t *stock_id;
} TreeImageOptions;

typedef struct {
	list_t *accel_mode;
	list_t *color_height;
	list_t *color_width;
	list_t *progress_orientation;
	list_t *progress_pulse;
	list_t *progress_text;
	list_t *progress_text_xalign;
	list_t *progress_text_yalign;
	list_t *spin_climb_rate;
	list_t *spinner_size;
	list_t *toggle_inconsistent;
} TreeRendererOptions;

typedef struct {
	gint model_column;
	gint width;
	gint height;
	GHashTable *cache;
	GHashTable *failed;
} TreeColorData;

#define TREE_RENDERER_CACHES_DATA "gtkdialog-tree-renderer-caches"

static void widget_tree_renderer_cache_list_free(gpointer data)
{
	g_ptr_array_free(data, TRUE);
}

static void widget_tree_renderer_cache_register(GtkWidget *tree_view,
	GHashTable *cache)
{
	GPtrArray *caches;

	caches = g_object_get_data(G_OBJECT(tree_view),
		TREE_RENDERER_CACHES_DATA);
	if (caches == NULL) {
		caches = g_ptr_array_new();
		g_object_set_data_full(G_OBJECT(tree_view),
			TREE_RENDERER_CACHES_DATA, caches,
			widget_tree_renderer_cache_list_free);
	}
	g_ptr_array_add(caches, cache);
}

static void widget_tree_renderer_caches_clear(GtkWidget *tree_view)
{
	GPtrArray *caches;
	guint index;

	caches = g_object_get_data(G_OBJECT(tree_view),
		TREE_RENDERER_CACHES_DATA);
	if (caches == NULL)
		return;
	for (index = 0; index < caches->len; ++index)
		g_hash_table_remove_all(g_ptr_array_index(caches, index));
}

#if GTK_CHECK_VERSION(2,20,0)
typedef struct {
	GtkWidget *tree_view;
	GtkCellRenderer *renderer;
	guint pulse;
} TreeSpinnerData;
#endif

/* Local function prototypes, located at file bottom */
static GtkTreeStore *widget_tree_create_tree_store(AttributeSet *Attr,
	tag_attr *attr);
static GtkWidget *widget_tree_create_tree_view(AttributeSet *Attr,
	tag_attr *attr, GtkTreeStore *store);
static void widget_tree_input_by_command(variable *var, char *command,
	gint command_or_file, const TreeInputContext *context);
static void widget_tree_input_by_file(variable *var, char *filename,
	const TreeInputContext *context);
static void widget_tree_input_by_items(variable *var,
	const TreeInputContext *context);
static void widget_tree_input_json(variable *var, const gchar *source,
	gboolean is_command, const TreeInputContext *context);
static void widget_tree_save_json(variable *var, const gchar *filename);
static void widget_tree_pixmap_column_cell_layout_function(
	GtkCellLayout *cell_layout, GtkCellRenderer *cell,
	GtkTreeModel *tree_model, GtkTreeIter *iter, GtkTreeView *treeview);
static void widget_tree_apply_json_cell_style(GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter);
gboolean widget_tree_changed_callback(GtkTreeSelection *treeselection,
	variable *var);
gint widget_tree_natcmp(GtkTreeModel *model, GtkTreeIter *a,
	GtkTreeIter *b, gpointer user_data);
gint widget_tree_natcasecmp(GtkTreeModel *model, GtkTreeIter *a,
	GtkTreeIter *b, gpointer user_data);
static gint _widget_tree_natcmp(GtkTreeModel *model, GtkTreeIter *a,
	GtkTreeIter *b, gpointer user_data, gint sensitive);

static gboolean widget_tree_number_is_valid(const gchar *value,
	const gchar *end)
{
	gboolean converted;

	converted = end != value;
	while (g_ascii_isspace(*end))
		++end;
	return errno == 0 && converted && *end == '\0';
}

static list_t *widget_tree_column_values(tag_attr *attr, const gchar *name)
{
	gchar *value;

	if (attr == NULL || (value = get_tag_attribute(attr, name)) == NULL)
		return NULL;
	return linecutter(g_strdup(value), '|');
}

static const gchar *widget_tree_column_value(list_t *values, gint column)
{
	if (values == NULL || column >= values->n_lines ||
		values->line[column][0] == '\0')
		return NULL;
	return values->line[column];
}

static void widget_tree_column_styles_init(TreeColumnStyles *styles,
	tag_attr *attr)
{
	memset(styles, 0, sizeof(*styles));
	styles->background = widget_tree_column_values(attr,
		"column-background");
	styles->ellipsize = widget_tree_column_values(attr,
		"column-ellipsize");
	styles->font = widget_tree_column_values(attr, "column-font");
	styles->foreground = widget_tree_column_values(attr,
		"column-foreground");
	styles->strikethrough = widget_tree_column_values(attr,
		"column-strikethrough");
	styles->style = widget_tree_column_values(attr, "column-style");
	styles->text_align = widget_tree_column_values(attr,
		"column-text-align");
	styles->underline = widget_tree_column_values(attr,
		"column-underline");
	styles->weight = widget_tree_column_values(attr, "column-weight");
	styles->wrap_mode = widget_tree_column_values(attr,
		"column-wrap-mode");
	styles->wrap_width = widget_tree_column_values(attr,
		"column-wrap-width");
	styles->xalign = widget_tree_column_values(attr, "column-xalign");
	styles->xpad = widget_tree_column_values(attr, "column-xpad");
	styles->yalign = widget_tree_column_values(attr, "column-yalign");
	styles->ypad = widget_tree_column_values(attr, "column-ypad");
}

static void widget_tree_column_styles_free(TreeColumnStyles *styles)
{
	list_t_free(styles->background);
	list_t_free(styles->ellipsize);
	list_t_free(styles->font);
	list_t_free(styles->foreground);
	list_t_free(styles->strikethrough);
	list_t_free(styles->style);
	list_t_free(styles->text_align);
	list_t_free(styles->underline);
	list_t_free(styles->weight);
	list_t_free(styles->wrap_mode);
	list_t_free(styles->wrap_width);
	list_t_free(styles->xalign);
	list_t_free(styles->xpad);
	list_t_free(styles->yalign);
	list_t_free(styles->ypad);
}

static void widget_tree_image_options_init(TreeImageOptions *options,
	tag_attr *attr)
{
	memset(options, 0, sizeof(*options));
	options->filename = widget_tree_column_values(attr,
		"column-pixbuf-file");
	options->icon_name = widget_tree_column_values(attr,
		"column-icon-name");
	options->size = widget_tree_column_values(attr, "column-pixbuf-size");
	options->source = widget_tree_column_values(attr,
		"column-pixbuf-source");
	options->stock_id = widget_tree_column_values(attr, "column-stock-id");
}

static void widget_tree_image_options_free(TreeImageOptions *options)
{
	list_t_free(options->filename);
	list_t_free(options->icon_name);
	list_t_free(options->size);
	list_t_free(options->source);
	list_t_free(options->stock_id);
}

static void widget_tree_renderer_options_init(TreeRendererOptions *options,
	tag_attr *attr)
{
	memset(options, 0, sizeof(*options));
	options->accel_mode = widget_tree_column_values(attr,
		"column-accel-mode");
	options->color_height = widget_tree_column_values(attr,
		"column-color-height");
	options->color_width = widget_tree_column_values(attr,
		"column-color-width");
	options->progress_orientation = widget_tree_column_values(attr,
		"column-progress-orientation");
	options->progress_pulse = widget_tree_column_values(attr,
		"column-progress-pulse");
	options->progress_text = widget_tree_column_values(attr,
		"column-progress-text");
	options->progress_text_xalign = widget_tree_column_values(attr,
		"column-progress-text-xalign");
	options->progress_text_yalign = widget_tree_column_values(attr,
		"column-progress-text-yalign");
	options->spin_climb_rate = widget_tree_column_values(attr,
		"column-spin-climb-rate");
	options->spinner_size = widget_tree_column_values(attr,
		"column-spinner-size");
	options->toggle_inconsistent = widget_tree_column_values(attr,
		"column-toggle-inconsistent");
}

static void widget_tree_renderer_options_free(TreeRendererOptions *options)
{
	list_t_free(options->accel_mode);
	list_t_free(options->color_height);
	list_t_free(options->color_width);
	list_t_free(options->progress_orientation);
	list_t_free(options->progress_pulse);
	list_t_free(options->progress_text);
	list_t_free(options->progress_text_xalign);
	list_t_free(options->progress_text_yalign);
	list_t_free(options->spin_climb_rate);
	list_t_free(options->spinner_size);
	list_t_free(options->toggle_inconsistent);
}

static GtkIconSize widget_tree_parse_icon_size(const gchar *value,
	const gchar *attribute)
{
	if (strcasecmp(value, "menu") == 0)
		return GTK_ICON_SIZE_MENU;
	if (strcasecmp(value, "small-toolbar") == 0)
		return GTK_ICON_SIZE_SMALL_TOOLBAR;
	if (strcasecmp(value, "large-toolbar") == 0)
		return GTK_ICON_SIZE_LARGE_TOOLBAR;
	if (strcasecmp(value, "button") == 0)
		return GTK_ICON_SIZE_BUTTON;
	if (strcasecmp(value, "dnd") == 0)
		return GTK_ICON_SIZE_DND;
	if (strcasecmp(value, "dialog") == 0)
		return GTK_ICON_SIZE_DIALOG;
	gtkdialog_warning("Invalid %s value '%s'; using menu.", attribute, value);
	return GTK_ICON_SIZE_MENU;
}

static GtkProgressBarOrientation widget_tree_parse_progress_orientation(
	const gchar *value)
{
	if (strcasecmp(value, "left-to-right") == 0)
		return GTK_PROGRESS_LEFT_TO_RIGHT;
	if (strcasecmp(value, "right-to-left") == 0)
		return GTK_PROGRESS_RIGHT_TO_LEFT;
	if (strcasecmp(value, "bottom-to-top") == 0)
		return GTK_PROGRESS_BOTTOM_TO_TOP;
	if (strcasecmp(value, "top-to-bottom") == 0)
		return GTK_PROGRESS_TOP_TO_BOTTOM;
	gtkdialog_warning("Invalid tree column-progress-orientation value '%s'; "
		"using left-to-right.", value);
	return GTK_PROGRESS_LEFT_TO_RIGHT;
}

static void widget_tree_set_fraction_property(GObject *object,
	const gchar *property, const gchar *value, const gchar *attribute)
{
	gdouble fraction;

	if (!widget_parse_finite_double(value, &fraction, attribute))
		return;
	if (fraction < 0.0 || fraction > 1.0) {
		gtkdialog_warning("%s must be between 0 and 1; ignoring '%s'.",
			attribute, value);
		return;
	}
	g_object_set(object, property, (gfloat)fraction, NULL);
}

static TreeImageSource widget_tree_image_source(const gchar *value)
{
	if (value == NULL || strcasecmp(value, "icon-name") == 0)
		return TreeImageIconName;
	if (strcasecmp(value, "stock-id") == 0)
		return TreeImageStockId;
	if (strcasecmp(value, "file") == 0)
		return TreeImageFile;
	gtkdialog_warning("Invalid tree column-pixbuf-source value '%s'; "
		"using icon-name.", value);
	return TreeImageIconName;
}

static GtkIconSize widget_tree_image_stock_size(gint size)
{
	if (size <= 16)
		return GTK_ICON_SIZE_MENU;
	if (size <= 18)
		return GTK_ICON_SIZE_SMALL_TOOLBAR;
	if (size <= 20)
		return GTK_ICON_SIZE_BUTTON;
	if (size <= 24)
		return GTK_ICON_SIZE_LARGE_TOOLBAR;
	if (size <= 32)
		return GTK_ICON_SIZE_DND;
	return GTK_ICON_SIZE_DIALOG;
}

static gchar *widget_tree_model_string(GtkTreeModel *model, GtkTreeIter *iter,
	gint column)
{
	gdouble double_value;
	gint64 int64_value;
	guint64 uint64_value;
	gchar *value;

	switch (gtk_tree_model_get_column_type(model, column)) {
		case G_TYPE_STRING:
			gtk_tree_model_get(model, iter, column, &value, -1);
			return value;
		case G_TYPE_INT64:
			gtk_tree_model_get(model, iter, column, &int64_value, -1);
			return g_strdup_printf("%" G_GINT64_FORMAT, int64_value);
		case G_TYPE_UINT64:
			gtk_tree_model_get(model, iter, column, &uint64_value, -1);
			return g_strdup_printf("%" G_GUINT64_FORMAT, uint64_value);
		case G_TYPE_DOUBLE:
			gtk_tree_model_get(model, iter, column, &double_value, -1);
			return g_strdup_printf("%f", double_value);
		default:
			return g_strdup("");
	}
}

static GdkPixbuf *widget_tree_color_swatch(TreeColorData *data,
	const gchar *value)
{
	GdkColor color;
	GdkPixbuf *pixbuf;
	guint32 rgba;

	if (value == NULL || *value == '\0')
		return NULL;
	pixbuf = g_hash_table_lookup(data->cache, value);
	if (pixbuf != NULL)
		return pixbuf;
	if (g_hash_table_lookup(data->failed, value) != NULL)
		return NULL;
	if (!gdk_color_parse(value, &color)) {
		gtkdialog_warning("Invalid tree color renderer value '%s'; "
			"showing an empty cell.", value);
		g_hash_table_insert(data->failed, g_strdup(value),
			GINT_TO_POINTER(1));
		return NULL;
	}

	pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8,
		data->width, data->height);
	if (pixbuf == NULL)
		return NULL;
	rgba = ((guint32)(color.red >> 8) << 24) |
		((guint32)(color.green >> 8) << 16) |
		((guint32)(color.blue >> 8) << 8) | 0xff;
	gdk_pixbuf_fill(pixbuf, rgba);
	g_hash_table_insert(data->cache, g_strdup(value), pixbuf);
	return pixbuf;
}

static void widget_tree_color_data_free(gpointer user_data)
{
	TreeColorData *data = user_data;

	g_hash_table_destroy(data->cache);
	g_hash_table_destroy(data->failed);
	g_free(data);
}

static void widget_tree_color_cell_data_function(
	GtkTreeViewColumn *column, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	TreeColorData *data = user_data;
	GdkPixbuf *pixbuf;
	gchar *value;

	(void)column;
	value = widget_tree_model_string(model, iter, data->model_column);
	pixbuf = widget_tree_color_swatch(data, value);
	g_object_set(G_OBJECT(renderer), "pixbuf", pixbuf, NULL);
	widget_tree_apply_json_cell_style(renderer, model, iter);
	g_free(value);
}

static GtkCellRenderer *widget_tree_create_color_renderer(
	GtkWidget *tree_view, GtkTreeViewColumn *column, gint data_column,
	gint model_column, TreeRendererOptions *options)
{
	GtkCellRenderer *renderer;
	TreeColorData *data;
	const gchar *value;

	data = g_new0(TreeColorData, 1);
	data->model_column = model_column;
	value = widget_tree_column_value(options->color_width, data_column);
	data->width = value ? widget_parse_bounded_integer(value, 1, 512, 36,
		"tree column-color-width") : 36;
	value = widget_tree_column_value(options->color_height, data_column);
	data->height = value ? widget_parse_bounded_integer(value, 1, 512, 14,
		"tree column-color-height") : 14;
	data->cache = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
		(GDestroyNotify)g_object_unref);
	data->failed = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
	widget_tree_renderer_cache_register(tree_view, data->cache);
	widget_tree_renderer_cache_register(tree_view, data->failed);

	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(column, renderer,
		widget_tree_color_cell_data_function, data,
		widget_tree_color_data_free);
	return renderer;
}

#if HAVE_JSON_GLIB
static void widget_tree_json_cell_image(GtkTreeModel *model,
	GtkTreeIter *iter, gint column, gchar **icon_name, gchar **stock_id,
	gchar **filename, gint *size)
{
	GValue value = { 0 };
	JsonArray *cells;
	JsonNode *cell;
	JsonNode *metadata;
	JsonNode *node;
	JsonObject *object;

	gtk_tree_model_get_value(model, iter, ColumnJsonStyle, &value);
	metadata = g_value_get_boxed(&value);
	if (metadata == NULL || !JSON_NODE_HOLDS_OBJECT(metadata))
		goto done;
	object = json_node_get_object(metadata);
	node = json_object_get_member(object, "cells");
	if (node == NULL || !JSON_NODE_HOLDS_ARRAY(node))
		goto done;
	cells = json_node_get_array(node);
	if ((guint)column >= json_array_get_length(cells))
		goto done;
	cell = json_array_get_element(cells, column);
	if (!JSON_NODE_HOLDS_OBJECT(cell))
		goto done;
	object = json_node_get_object(cell);
	if (json_object_has_member(object, "icon-name"))
		*icon_name = g_strdup(json_object_get_string_member(object,
			"icon-name"));
	if (json_object_has_member(object, "stock-id"))
		*stock_id = g_strdup(json_object_get_string_member(object,
			"stock-id"));
	if (json_object_has_member(object, "pixbuf-file"))
		*filename = g_strdup(json_object_get_string_member(object,
			"pixbuf-file"));
	if (json_object_has_member(object, "pixbuf-size"))
		*size = (gint)json_object_get_int_member(object, "pixbuf-size");

done:
	g_value_unset(&value);
}
#else
static void widget_tree_json_cell_image(GtkTreeModel *model,
	GtkTreeIter *iter, gint column, gchar **icon_name, gchar **stock_id,
	gchar **filename, gint *size)
{
	(void)model;
	(void)iter;
	(void)column;
	(void)icon_name;
	(void)stock_id;
	(void)filename;
	(void)size;
}
#endif

static GdkPixbuf *widget_tree_image_load(TreeImageData *data,
	TreeImageSource source, const gchar *value, gint size)
{
	GdkPixbuf *loaded;
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	GtkIconTheme *theme;
	gchar *key;
	gdouble scale;
	gint height;
	gint width;

	if (value == NULL || *value == '\0')
		return NULL;
	key = g_strdup_printf("%d:%d:%s", source, size, value);
	pixbuf = g_hash_table_lookup(data->cache, key);
	if (pixbuf != NULL) {
		g_free(key);
		return pixbuf;
	}
	if (g_hash_table_lookup(data->failed, key) != NULL) {
		g_free(key);
		return NULL;
	}

	if (source == TreeImageIconName) {
		theme = gtk_icon_theme_get_for_screen(
			gtk_widget_get_screen(data->tree_view));
		loaded = gtk_icon_theme_load_icon(theme, value, size, 0, &error);
	} else if (source == TreeImageStockId) {
		loaded = gtk_widget_render_icon(data->tree_view, value,
			widget_tree_image_stock_size(size), NULL);
		if (loaded == NULL)
			error = g_error_new(G_FILE_ERROR, G_FILE_ERROR_FAILED,
				"unknown stock icon");
	} else {
		loaded = gdk_pixbuf_new_from_file(value, &error);
		if (loaded != NULL) {
			width = gdk_pixbuf_get_width(loaded);
			height = gdk_pixbuf_get_height(loaded);
			if (width > size || height > size) {
				scale = MIN((gdouble)size / width,
					(gdouble)size / height);
				pixbuf = gdk_pixbuf_scale_simple(loaded,
					MAX(1, (gint)(width * scale + 0.5)),
					MAX(1, (gint)(height * scale + 0.5)),
					GDK_INTERP_BILINEAR);
				g_object_unref(loaded);
				loaded = pixbuf;
			}
		}
	}
	if (loaded == NULL) {
		gtkdialog_warning("Couldn't load tree cell image '%s': %s", value,
			error ? error->message : "unknown error");
		if (error != NULL)
			g_error_free(error);
		g_hash_table_insert(data->failed, key, GINT_TO_POINTER(1));
		return NULL;
	}
	if (error != NULL)
		g_error_free(error);
	g_hash_table_insert(data->cache, key, loaded);
	return loaded;
}

static void widget_tree_image_data_free(gpointer user_data)
{
	TreeImageData *data = user_data;

	g_free(data->icon_name);
	g_free(data->stock_id);
	g_free(data->filename);
	g_hash_table_destroy(data->cache);
	g_hash_table_destroy(data->failed);
	g_free(data);
}

static void widget_tree_image_cell_data_function(
	GtkTreeViewColumn *column, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	TreeImageData *data = user_data;
	GdkPixbuf *pixbuf;
	TreeImageSource source;
	gchar *filename = NULL;
	gchar *icon_name = NULL;
	gchar *stock_id = NULL;
	gchar *value = NULL;
	gint size;

	(void)column;
	size = data->size;
	widget_tree_json_cell_image(model, iter, data->column, &icon_name,
		&stock_id, &filename, &size);
	if (icon_name == NULL && stock_id == NULL && filename == NULL &&
		data->column == 0 && !data->value_is_source) {
		gtk_tree_model_get(model, iter, ColumnIconName, &icon_name,
			ColumnStockId, &stock_id, -1);
	}
	if (icon_name == NULL && stock_id == NULL && filename == NULL &&
		data->value_is_source) {
		value = widget_tree_model_string(model, iter, data->model_column);
		if (data->source == TreeImageIconName)
			icon_name = value;
		else if (data->source == TreeImageStockId)
			stock_id = value;
		else
			filename = value;
	}
	if (icon_name == NULL && stock_id == NULL && filename == NULL) {
		icon_name = g_strdup(data->icon_name);
		stock_id = g_strdup(data->stock_id);
		filename = g_strdup(data->filename);
	}
	if (icon_name != NULL) {
		source = TreeImageIconName;
		value = icon_name;
	} else if (stock_id != NULL) {
		source = TreeImageStockId;
		value = stock_id;
	} else {
		source = TreeImageFile;
		value = filename;
	}
	pixbuf = widget_tree_image_load(data, source, value, size);
	g_object_set(G_OBJECT(renderer), "pixbuf", pixbuf, NULL);
	widget_tree_apply_json_cell_style(renderer, model, iter);
	g_free(icon_name);
	g_free(stock_id);
	g_free(filename);
}

static GtkCellRenderer *widget_tree_create_image_renderer(GtkWidget *tree_view,
	GtkTreeViewColumn *column, gint data_column, gint model_column,
	TreeImageOptions *options, gboolean value_is_source)
{
	GtkCellRenderer *renderer;
	TreeImageData *data;
	const gchar *value;

	data = g_new0(TreeImageData, 1);
	data->tree_view = tree_view;
	data->column = data_column;
	data->model_column = model_column;
	data->value_is_source = value_is_source;
	data->source = widget_tree_image_source(widget_tree_column_value(
		options->source, data_column));
	value = widget_tree_column_value(options->size, data_column);
	data->size = value ? widget_parse_bounded_integer(value, 1, 512, 16,
		"tree column-pixbuf-size") : 16;
	data->icon_name = g_strdup(widget_tree_column_value(options->icon_name,
		data_column));
	data->stock_id = g_strdup(widget_tree_column_value(options->stock_id,
		data_column));
	data->filename = g_strdup(widget_tree_column_value(options->filename,
		data_column));
	data->cache = g_hash_table_new_full(g_str_hash, g_str_equal, g_free,
		(GDestroyNotify)g_object_unref);
	data->failed = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
	widget_tree_renderer_cache_register(tree_view, data->cache);
	widget_tree_renderer_cache_register(tree_view, data->failed);

	renderer = gtk_cell_renderer_pixbuf_new();
	gtk_tree_view_column_pack_start(column, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(column, renderer,
		widget_tree_image_cell_data_function, data,
		widget_tree_image_data_free);
	return renderer;
}

static gboolean widget_tree_parse_alignment(const gchar *value,
	PangoAlignment *alignment)
{
	if (strcasecmp(value, "left") == 0) {
		*alignment = PANGO_ALIGN_LEFT;
	} else if (strcasecmp(value, "center") == 0) {
		*alignment = PANGO_ALIGN_CENTER;
	} else if (strcasecmp(value, "right") == 0) {
		*alignment = PANGO_ALIGN_RIGHT;
	} else {
		gtkdialog_warning("Invalid tree column-text-align value '%s'; "
			"ignoring it.", value);
		return FALSE;
	}
	return TRUE;
}

static gboolean widget_tree_parse_ellipsize(const gchar *value,
	PangoEllipsizeMode *ellipsize)
{
	if (strcasecmp(value, "none") == 0) {
		*ellipsize = PANGO_ELLIPSIZE_NONE;
	} else if (strcasecmp(value, "start") == 0) {
		*ellipsize = PANGO_ELLIPSIZE_START;
	} else if (strcasecmp(value, "middle") == 0) {
		*ellipsize = PANGO_ELLIPSIZE_MIDDLE;
	} else if (strcasecmp(value, "end") == 0) {
		*ellipsize = PANGO_ELLIPSIZE_END;
	} else {
		gtkdialog_warning("Invalid tree column-ellipsize value '%s'; "
			"ignoring it.", value);
		return FALSE;
	}
	return TRUE;
}

static gboolean widget_tree_parse_font_style(const gchar *value,
	PangoStyle *style)
{
	if (strcasecmp(value, "normal") == 0) {
		*style = PANGO_STYLE_NORMAL;
	} else if (strcasecmp(value, "oblique") == 0) {
		*style = PANGO_STYLE_OBLIQUE;
	} else if (strcasecmp(value, "italic") == 0) {
		*style = PANGO_STYLE_ITALIC;
	} else {
		gtkdialog_warning("Invalid tree column-style value '%s'; ignoring it.",
			value);
		return FALSE;
	}
	return TRUE;
}

static gboolean widget_tree_parse_underline(const gchar *value,
	PangoUnderline *underline)
{
	if (strcasecmp(value, "none") == 0) {
		*underline = PANGO_UNDERLINE_NONE;
	} else if (strcasecmp(value, "single") == 0) {
		*underline = PANGO_UNDERLINE_SINGLE;
	} else if (strcasecmp(value, "double") == 0) {
		*underline = PANGO_UNDERLINE_DOUBLE;
	} else if (strcasecmp(value, "low") == 0) {
		*underline = PANGO_UNDERLINE_LOW;
	} else if (strcasecmp(value, "error") == 0) {
		*underline = PANGO_UNDERLINE_ERROR;
	} else {
		gtkdialog_warning("Invalid tree column-underline value '%s'; "
			"ignoring it.", value);
		return FALSE;
	}
	return TRUE;
}

static gboolean widget_tree_parse_wrap_mode(const gchar *value,
	PangoWrapMode *wrap_mode)
{
	if (strcasecmp(value, "word") == 0) {
		*wrap_mode = PANGO_WRAP_WORD;
	} else if (strcasecmp(value, "char") == 0) {
		*wrap_mode = PANGO_WRAP_CHAR;
	} else if (strcasecmp(value, "word-char") == 0) {
		*wrap_mode = PANGO_WRAP_WORD_CHAR;
	} else {
		gtkdialog_warning("Invalid tree column-wrap-mode value '%s'; "
			"ignoring it.", value);
		return FALSE;
	}
	return TRUE;
}

static gint widget_tree_parse_weight(const gchar *value)
{
	if (strcasecmp(value, "normal") == 0)
		return PANGO_WEIGHT_NORMAL;
	if (strcasecmp(value, "bold") == 0)
		return PANGO_WEIGHT_BOLD;
	if (strcasecmp(value, "light") == 0)
		return PANGO_WEIGHT_LIGHT;
	return widget_parse_bounded_integer(value, 1, 1000,
		PANGO_WEIGHT_NORMAL, "tree column-weight");
}

static void widget_tree_apply_column_style(GtkCellRenderer *renderer,
	TreeColumnStyles *styles, gint column)
{
	const gchar *value;
	gdouble alignment;
	GdkColor color;
	gint padding;
	PangoAlignment text_alignment;
	PangoEllipsizeMode ellipsize;
	PangoStyle font_style;
	PangoUnderline underline;
	PangoWrapMode wrap_mode;

	value = widget_tree_column_value(styles->background, column);
	if (value != NULL) {
		if (gdk_color_parse(value, &color))
			g_object_set(G_OBJECT(renderer), "cell-background", value, NULL);
		else
			gtkdialog_warning("Invalid tree column-background value '%s'; "
				"ignoring it.", value);
	}
	value = widget_tree_column_value(styles->xalign, column);
	if (value != NULL && widget_parse_finite_double(value, &alignment,
		"tree column-xalign")) {
		if (alignment >= 0.0 && alignment <= 1.0)
			g_object_set(G_OBJECT(renderer), "xalign", (gfloat)alignment, NULL);
		else
			gtkdialog_warning("Tree column-xalign must be between 0 and 1; "
				"ignoring '%s'.", value);
	}
	value = widget_tree_column_value(styles->yalign, column);
	if (value != NULL && widget_parse_finite_double(value, &alignment,
		"tree column-yalign")) {
		if (alignment >= 0.0 && alignment <= 1.0)
			g_object_set(G_OBJECT(renderer), "yalign", (gfloat)alignment, NULL);
		else
			gtkdialog_warning("Tree column-yalign must be between 0 and 1; "
				"ignoring '%s'.", value);
	}
	value = widget_tree_column_value(styles->xpad, column);
	if (value != NULL) {
		padding = widget_parse_bounded_integer(value, 0, G_MAXINT, 0,
			"tree column-xpad");
		g_object_set(G_OBJECT(renderer), "xpad", (guint)padding, NULL);
	}
	value = widget_tree_column_value(styles->ypad, column);
	if (value != NULL) {
		padding = widget_parse_bounded_integer(value, 0, G_MAXINT, 0,
			"tree column-ypad");
		g_object_set(G_OBJECT(renderer), "ypad", (guint)padding, NULL);
	}

	if (!GTK_IS_CELL_RENDERER_TEXT(renderer))
		return;
	value = widget_tree_column_value(styles->foreground, column);
	if (value != NULL) {
		if (gdk_color_parse(value, &color))
			g_object_set(G_OBJECT(renderer), "foreground", value, NULL);
		else
			gtkdialog_warning("Invalid tree column-foreground value '%s'; "
				"ignoring it.", value);
	}
	value = widget_tree_column_value(styles->font, column);
	if (value != NULL)
		g_object_set(G_OBJECT(renderer), "font", value, NULL);
	value = widget_tree_column_value(styles->weight, column);
	if (value != NULL)
		g_object_set(G_OBJECT(renderer), "weight",
			widget_tree_parse_weight(value), NULL);
	value = widget_tree_column_value(styles->style, column);
	if (value != NULL && widget_tree_parse_font_style(value, &font_style))
		g_object_set(G_OBJECT(renderer), "style", font_style, NULL);
	value = widget_tree_column_value(styles->underline, column);
	if (value != NULL && widget_tree_parse_underline(value, &underline))
		g_object_set(G_OBJECT(renderer), "underline", underline, NULL);
	value = widget_tree_column_value(styles->strikethrough, column);
	if (value != NULL)
		g_object_set(G_OBJECT(renderer), "strikethrough",
			widget_attribute_is_true(value), NULL);
#if GTK_CHECK_VERSION(2,6,0)
	value = widget_tree_column_value(styles->ellipsize, column);
	if (value != NULL && widget_tree_parse_ellipsize(value, &ellipsize))
		g_object_set(G_OBJECT(renderer), "ellipsize", ellipsize, NULL);
#endif
#if GTK_CHECK_VERSION(2,8,0)
	value = widget_tree_column_value(styles->wrap_mode, column);
	if (value != NULL && widget_tree_parse_wrap_mode(value, &wrap_mode))
		g_object_set(G_OBJECT(renderer), "wrap-mode", wrap_mode, NULL);
	value = widget_tree_column_value(styles->wrap_width, column);
	if (value != NULL) {
		padding = widget_parse_bounded_integer(value, -1, G_MAXINT, -1,
			"tree column-wrap-width");
		g_object_set(G_OBJECT(renderer), "wrap-width", padding, NULL);
	}
#endif
#if GTK_CHECK_VERSION(2,10,0)
	value = widget_tree_column_value(styles->text_align, column);
	if (value != NULL &&
		widget_tree_parse_alignment(value, &text_alignment))
		g_object_set(G_OBJECT(renderer), "alignment", text_alignment, NULL);
#endif
}

#if HAVE_JSON_GLIB
static void widget_tree_renderer_style_free(gpointer user_data)
{
	TreeRendererStyle *style = user_data;

	if (style->background != NULL)
		gdk_color_free(style->background);
	if (style->foreground != NULL)
		gdk_color_free(style->foreground);
	if (style->font_desc != NULL)
		pango_font_description_free(style->font_desc);
	g_free(style);
}

static TreeRendererStyle *widget_tree_renderer_style_new(
	GtkCellRenderer *renderer, gint column)
{
	TreeRendererStyle *style;

	style = g_new0(TreeRendererStyle, 1);
	style->column = column;
	style->text_renderer = GTK_IS_CELL_RENDERER_TEXT(renderer);
	g_object_get(G_OBJECT(renderer),
		"cell-background-set", &style->background_set,
		"cell-background-gdk", &style->background,
		"xalign", &style->xalign,
		"yalign", &style->yalign,
		"xpad", &style->xpad,
		"ypad", &style->ypad, NULL);
	if (style->text_renderer) {
		g_object_get(G_OBJECT(renderer),
			"foreground-set", &style->foreground_set,
			"foreground-gdk", &style->foreground,
			"font-desc", &style->font_desc,
			"style", &style->font_style,
			"style-set", &style->font_style_set,
			"weight", &style->weight,
			"weight-set", &style->weight_set,
			"underline", &style->underline,
			"underline-set", &style->underline_set,
			"strikethrough", &style->strikethrough,
			"strikethrough-set", &style->strikethrough_set, NULL);
#if GTK_CHECK_VERSION(2,6,0)
		g_object_get(G_OBJECT(renderer),
			"ellipsize", &style->ellipsize,
			"ellipsize-set", &style->ellipsize_set, NULL);
#endif
#if GTK_CHECK_VERSION(2,8,0)
		g_object_get(G_OBJECT(renderer),
			"wrap-mode", &style->wrap_mode,
			"wrap-width", &style->wrap_width, NULL);
#endif
#if GTK_CHECK_VERSION(2,10,0)
		g_object_get(G_OBJECT(renderer),
			"alignment", &style->text_align,
			"align-set", &style->text_align_set, NULL);
#endif
	}
	return style;
}

static void widget_tree_restore_renderer_style(GtkCellRenderer *renderer,
	TreeRendererStyle *style)
{
	g_object_set(G_OBJECT(renderer),
		"xalign", style->xalign,
		"yalign", style->yalign,
		"xpad", style->xpad,
		"ypad", style->ypad, NULL);
	if (style->background_set)
		g_object_set(G_OBJECT(renderer), "cell-background-gdk",
			style->background, NULL);
	else
		g_object_set(G_OBJECT(renderer), "cell-background-set", FALSE, NULL);
	if (!style->text_renderer)
		return;

	if (style->foreground_set)
		g_object_set(G_OBJECT(renderer), "foreground-gdk",
			style->foreground, NULL);
	else
		g_object_set(G_OBJECT(renderer), "foreground-set", FALSE, NULL);
	g_object_set(G_OBJECT(renderer), "font-desc", style->font_desc,
		"style", style->font_style,
		"style-set", style->font_style_set,
		"weight", style->weight,
		"weight-set", style->weight_set,
		"underline", style->underline,
		"underline-set", style->underline_set,
		"strikethrough", style->strikethrough,
		"strikethrough-set", style->strikethrough_set, NULL);
#if GTK_CHECK_VERSION(2,6,0)
	g_object_set(G_OBJECT(renderer),
		"ellipsize", style->ellipsize,
		"ellipsize-set", style->ellipsize_set, NULL);
#endif
#if GTK_CHECK_VERSION(2,8,0)
	g_object_set(G_OBJECT(renderer),
		"wrap-mode", style->wrap_mode,
		"wrap-width", style->wrap_width, NULL);
#endif
#if GTK_CHECK_VERSION(2,10,0)
	g_object_set(G_OBJECT(renderer),
		"alignment", style->text_align,
		"align-set", style->text_align_set, NULL);
#endif
}

static void widget_tree_apply_json_style_object(GtkCellRenderer *renderer,
	TreeRendererStyle *base, JsonObject *object)
{
	JsonNode *node;
	const gchar *string;
	PangoAlignment text_alignment;
	PangoEllipsizeMode ellipsize;
	PangoStyle font_style;
	PangoUnderline underline;
	PangoWrapMode wrap_mode;

	if (json_object_has_member(object, "background"))
		g_object_set(G_OBJECT(renderer), "cell-background",
			json_object_get_string_member(object, "background"), NULL);
	if (json_object_has_member(object, "xalign"))
		g_object_set(G_OBJECT(renderer), "xalign", (gfloat)
			json_object_get_double_member(object, "xalign"), NULL);
	if (json_object_has_member(object, "yalign"))
		g_object_set(G_OBJECT(renderer), "yalign", (gfloat)
			json_object_get_double_member(object, "yalign"), NULL);
	if (json_object_has_member(object, "xpad"))
		g_object_set(G_OBJECT(renderer), "xpad", (guint)
			json_object_get_int_member(object, "xpad"), NULL);
	if (json_object_has_member(object, "ypad"))
		g_object_set(G_OBJECT(renderer), "ypad", (guint)
			json_object_get_int_member(object, "ypad"), NULL);
	if (!base->text_renderer)
		return;

	if (json_object_has_member(object, "foreground"))
		g_object_set(G_OBJECT(renderer), "foreground",
			json_object_get_string_member(object, "foreground"), NULL);
	if (json_object_has_member(object, "font"))
		g_object_set(G_OBJECT(renderer), "font",
			json_object_get_string_member(object, "font"), NULL);
	if (json_object_has_member(object, "weight")) {
		node = json_object_get_member(object, "weight");
		if (json_node_get_value_type(node) == G_TYPE_STRING) {
			string = json_node_get_string(node);
			g_object_set(G_OBJECT(renderer), "weight",
				widget_tree_parse_weight(string), NULL);
		} else {
			g_object_set(G_OBJECT(renderer), "weight",
				(gint)json_node_get_int(node), NULL);
		}
	}
	if (json_object_has_member(object, "style")) {
		string = json_object_get_string_member(object, "style");
		if (widget_tree_parse_font_style(string, &font_style))
			g_object_set(G_OBJECT(renderer), "style", font_style, NULL);
	}
	if (json_object_has_member(object, "underline")) {
		string = json_object_get_string_member(object, "underline");
		if (widget_tree_parse_underline(string, &underline))
			g_object_set(G_OBJECT(renderer), "underline", underline, NULL);
	}
	if (json_object_has_member(object, "strikethrough"))
		g_object_set(G_OBJECT(renderer), "strikethrough",
			json_object_get_boolean_member(object, "strikethrough"), NULL);
#if GTK_CHECK_VERSION(2,6,0)
	if (json_object_has_member(object, "ellipsize")) {
		string = json_object_get_string_member(object, "ellipsize");
		if (widget_tree_parse_ellipsize(string, &ellipsize))
			g_object_set(G_OBJECT(renderer), "ellipsize", ellipsize, NULL);
	}
#endif
#if GTK_CHECK_VERSION(2,8,0)
	if (json_object_has_member(object, "wrap-mode")) {
		string = json_object_get_string_member(object, "wrap-mode");
		if (widget_tree_parse_wrap_mode(string, &wrap_mode))
			g_object_set(G_OBJECT(renderer), "wrap-mode", wrap_mode, NULL);
	}
	if (json_object_has_member(object, "wrap-width"))
		g_object_set(G_OBJECT(renderer), "wrap-width",
			(gint)json_object_get_int_member(object, "wrap-width"), NULL);
#endif
#if GTK_CHECK_VERSION(2,10,0)
	if (json_object_has_member(object, "text-align")) {
		string = json_object_get_string_member(object, "text-align");
		if (widget_tree_parse_alignment(string, &text_alignment))
			g_object_set(G_OBJECT(renderer), "alignment", text_alignment, NULL);
	}
#endif
}

static gboolean widget_tree_iter_is_selected(GtkWidget *tree_view,
	GtkTreeModel *model, GtkTreeIter *iter)
{
	GtkTreePath *path;
	GtkTreeSelection *selection;
	gboolean selected;

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
	path = gtk_tree_model_get_path(model, iter);
	selected = gtk_tree_selection_path_is_selected(selection, path);
	gtk_tree_path_free(path);
	return selected;
}

static void widget_tree_apply_json_cell_style(GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter)
{
	GValue value = { 0 };
	GtkWidget *tree_view;
	JsonArray *cells;
	JsonNode *cell;
	JsonNode *metadata;
	JsonNode *node;
	JsonObject *object;
	TreeRendererStyle *base;

	base = g_object_get_data(G_OBJECT(renderer),
		"_gtkdialog-tree-renderer-style");
	if (base == NULL)
		return;
	widget_tree_restore_renderer_style(renderer, base);
	gtk_tree_model_get_value(model, iter, ColumnJsonStyle, &value);
	metadata = g_value_get_boxed(&value);
	if (metadata != NULL && JSON_NODE_HOLDS_OBJECT(metadata)) {
		object = json_node_get_object(metadata);
		node = json_object_get_member(object, "style");
		if (node != NULL && JSON_NODE_HOLDS_OBJECT(node))
			widget_tree_apply_json_style_object(renderer, base,
				json_node_get_object(node));
		node = json_object_get_member(object, "cells");
		if (node != NULL && JSON_NODE_HOLDS_ARRAY(node)) {
			cells = json_node_get_array(node);
			if ((guint)base->column < json_array_get_length(cells)) {
				cell = json_array_get_element(cells, base->column);
				if (JSON_NODE_HOLDS_OBJECT(cell))
					widget_tree_apply_json_style_object(renderer, base,
						json_node_get_object(cell));
			}
		}
	}
	tree_view = g_object_get_data(G_OBJECT(renderer),
		"_gtkdialog-tree-view");
	if (tree_view != NULL &&
		widget_tree_iter_is_selected(tree_view, model, iter)) {
		g_object_set(G_OBJECT(renderer), "cell-background-set", FALSE, NULL);
		if (base->text_renderer)
			g_object_set(G_OBJECT(renderer), "foreground-set", FALSE, NULL);
	}
	g_value_unset(&value);
}

static void widget_tree_style_cell_data_function(
	GtkCellLayout *cell_layout, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	(void)cell_layout;
	(void)user_data;
	widget_tree_apply_json_cell_style(renderer, model, iter);
}

static void widget_tree_attach_json_style(GtkCellRenderer *renderer,
	GtkWidget *tree_view, gint column, gboolean own_data_function)
{
	g_object_set_data_full(G_OBJECT(renderer),
		"_gtkdialog-tree-renderer-style",
		widget_tree_renderer_style_new(renderer, column),
		widget_tree_renderer_style_free);
	g_object_set_data(G_OBJECT(renderer), "_gtkdialog-tree-view", tree_view);
	if (own_data_function)
		gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(
			gtk_tree_view_get_column(GTK_TREE_VIEW(tree_view), column)),
			renderer, widget_tree_style_cell_data_function, NULL, NULL);
}
#else
static void widget_tree_apply_json_cell_style(GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter)
{
	(void)renderer;
	(void)model;
	(void)iter;
}

static void widget_tree_attach_json_style(GtkCellRenderer *renderer,
	GtkWidget *tree_view, gint column, gboolean own_data_function)
{
	(void)renderer;
	(void)tree_view;
	(void)column;
	(void)own_data_function;
}
#endif

static void widget_tree_set_column_value(GtkTreeStore *store,
	GtkTreeIter *iter, gint column, const gchar *value)
{
	const gchar *start;
	gchar *end;
	gdouble double_value;
	gint64 int64_value;
	guint64 uint64_value;
	GType type;

	type = gtk_tree_model_get_column_type(GTK_TREE_MODEL(store), column);
	switch (type) {
		case G_TYPE_STRING:
			gtk_tree_store_set(store, iter, column, value, -1);
			break;
		case G_TYPE_INT64:
			errno = 0;
			int64_value = g_ascii_strtoll(value, &end, 0);
			if (!widget_tree_number_is_valid(value, end)) {
				gtkdialog_warning("Invalid tree int64 data '%s'; using 0.", value);
				int64_value = 0;
			}
			gtk_tree_store_set(store, iter, column, int64_value, -1);
			break;
		case G_TYPE_UINT64:
			start = value;
			while (g_ascii_isspace(*start))
				++start;
			errno = 0;
			uint64_value = g_ascii_strtoull(value, &end, 0);
			if (*start == '-' || !widget_tree_number_is_valid(value, end)) {
				gtkdialog_warning("Invalid tree uint64 data '%s'; using 0.", value);
				uint64_value = 0;
			}
			gtk_tree_store_set(store, iter, column, uint64_value, -1);
			break;
		case G_TYPE_DOUBLE:
			errno = 0;
			double_value = g_ascii_strtod(value, &end);
			if (!widget_tree_number_is_valid(value, end) ||
				!isfinite(double_value)) {
				gtkdialog_warning("Invalid tree double data '%s'; using 0.", value);
				double_value = 0.0;
			}
			gtk_tree_store_set(store, iter, column, double_value, -1);
			break;
		default:
			fprintf(stderr, "%s(): Unsupported column-type %"
				G_GSIZE_FORMAT "\n", __func__, type);
	}
}

static gboolean widget_tree_get_column_boolean(GtkTreeModel *model,
	GtkTreeIter *iter, gint column)
{
	gchar *string_value;
	gdouble double_value;
	gint64 int64_value;
	guint64 uint64_value;
	gboolean result;

	switch (gtk_tree_model_get_column_type(model, column)) {
		case G_TYPE_STRING:
			gtk_tree_model_get(model, iter, column, &string_value, -1);
			result = widget_attribute_is_true(string_value);
			g_free(string_value);
			return result;
		case G_TYPE_INT64:
			gtk_tree_model_get(model, iter, column, &int64_value, -1);
			return int64_value != 0;
		case G_TYPE_UINT64:
			gtk_tree_model_get(model, iter, column, &uint64_value, -1);
			return uint64_value != 0;
		case G_TYPE_DOUBLE:
			gtk_tree_model_get(model, iter, column, &double_value, -1);
			return double_value != 0.0;
		default:
			return FALSE;
	}
}

static void widget_tree_set_column_boolean(GtkTreeStore *store,
	GtkTreeIter *iter, gint column, gboolean value)
{
	switch (gtk_tree_model_get_column_type(GTK_TREE_MODEL(store), column)) {
		case G_TYPE_STRING:
			gtk_tree_store_set(store, iter, column,
				value ? "true" : "false", -1);
			break;
		case G_TYPE_INT64:
			gtk_tree_store_set(store, iter, column, (gint64)value, -1);
			break;
		case G_TYPE_UINT64:
			gtk_tree_store_set(store, iter, column, (guint64)value, -1);
			break;
		case G_TYPE_DOUBLE:
			gtk_tree_store_set(store, iter, column, (gdouble)value, -1);
			break;
		default:
			break;
	}
}

static TreeCellData *widget_tree_cell_data_new(GtkWidget *tree_view,
	AttributeSet *attributes, gint model_column)
{
	TreeCellData *data;

	data = g_new(TreeCellData, 1);
	data->tree_view = tree_view;
	data->attributes = attributes;
	data->model_column = model_column;
	data->radio = FALSE;
	return data;
}

static void widget_tree_text_edited_callback(GtkCellRendererText *renderer,
	gchar *path, gchar *new_text, TreeCellData *data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;

	(void)renderer;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(data->tree_view));
	if (!gtk_tree_model_get_iter_from_string(model, &iter, path))
		return;

	widget_tree_set_column_value(GTK_TREE_STORE(model), &iter,
		data->model_column, new_text);
	widget_signal_executor(data->tree_view, data->attributes, "cell-edited");
}

static void widget_tree_enable_text_editing(GtkCellRenderer *renderer,
	GtkWidget *tree_view, AttributeSet *attributes, gint model_column)
{
	TreeCellData *data;

	data = widget_tree_cell_data_new(tree_view, attributes, model_column);
	g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
	g_object_set_data_full(G_OBJECT(renderer),
		"_gtkdialog-tree-cell-data", data, g_free);
	g_signal_connect(G_OBJECT(renderer), "edited",
		G_CALLBACK(widget_tree_text_edited_callback), data);
}

#if GTK_CHECK_VERSION(2,10,0)
static gchar *widget_tree_accel_to_gtk_name(const gchar *accelerator)
{
	GString *name;
	gchar **parts;
	gint index;
	gint n_parts;

	if (accelerator == NULL || strchr(accelerator, '<') != NULL)
		return g_strdup(accelerator);
	parts = g_strsplit(accelerator, "+", -1);
	n_parts = g_strv_length(parts);
	if (n_parts < 2) {
		g_strfreev(parts);
		return g_strdup(accelerator);
	}

	name = g_string_new(NULL);
	for (index = 0; index < n_parts - 1; ++index)
		g_string_append_printf(name, "<%s>", parts[index]);
	g_string_append(name, parts[n_parts - 1]);
	g_strfreev(parts);
	return g_string_free(name, FALSE);
}

static gchar *widget_tree_accel_from_gtk_name(const gchar *gtk_name)
{
	GString *name;
	const gchar *end;
	const gchar *position;

	name = g_string_new(NULL);
	position = gtk_name;
	while (*position != '\0') {
		if (*position == '<' && (end = strchr(position, '>')) != NULL) {
			g_string_append_len(name, position + 1,
				(gssize)(end - position - 1));
			g_string_append_c(name, '+');
			position = end + 1;
		} else {
			g_string_append_c(name, *position);
			++position;
		}
	}
	return g_string_free(name, FALSE);
}

static void widget_tree_accel_cell_data_function(
	GtkTreeViewColumn *column, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	GdkModifierType modifiers;
	gchar *accelerator;
	gchar *gtk_name;
	gint model_column;
	guint key;

	(void)column;
	model_column = GPOINTER_TO_INT(user_data);
	gtk_tree_model_get(model, iter, model_column, &accelerator, -1);
	key = 0;
	modifiers = 0;
	gtk_name = widget_tree_accel_to_gtk_name(accelerator);
	if (gtk_name != NULL)
		gtk_accelerator_parse(gtk_name, &key, &modifiers);
	g_object_set(G_OBJECT(renderer), "accel-key", key,
		"accel-mods", modifiers, "keycode", 0, NULL);
	widget_tree_apply_json_cell_style(renderer, model, iter);
	g_free(gtk_name);
	g_free(accelerator);
}

static void widget_tree_accel_edited_callback(GtkCellRendererAccel *renderer,
	const gchar *path, guint key, GdkModifierType modifiers,
	guint hardware_keycode, TreeCellData *data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *accelerator;
	gchar *gtk_name;

	(void)renderer;
	(void)hardware_keycode;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(data->tree_view));
	if (!gtk_tree_model_get_iter_from_string(model, &iter, path))
		return;

	accelerator = gtk_accelerator_name(key, modifiers);
	gtk_name = widget_tree_accel_from_gtk_name(accelerator);
	widget_tree_set_column_value(GTK_TREE_STORE(model), &iter,
		data->model_column, gtk_name);
	g_free(gtk_name);
	g_free(accelerator);
	widget_signal_executor(data->tree_view, data->attributes, "accel-edited");
}

static void widget_tree_accel_cleared_callback(GtkCellRendererAccel *renderer,
	const gchar *path, TreeCellData *data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;

	(void)renderer;
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(data->tree_view));
	if (!gtk_tree_model_get_iter_from_string(model, &iter, path))
		return;

	widget_tree_set_column_value(GTK_TREE_STORE(model), &iter,
		data->model_column, "");
	widget_signal_executor(data->tree_view, data->attributes, "accel-cleared");
}

static void widget_tree_enable_accel(GtkCellRenderer *renderer,
	GtkWidget *tree_view, AttributeSet *attributes, gint model_column)
{
	TreeCellData *data;

	data = widget_tree_cell_data_new(tree_view, attributes, model_column);
	g_object_set(G_OBJECT(renderer), "editable", TRUE, NULL);
	g_object_set_data_full(G_OBJECT(renderer),
		"_gtkdialog-tree-cell-data", data, g_free);
	g_signal_connect(G_OBJECT(renderer), "accel-edited",
		G_CALLBACK(widget_tree_accel_edited_callback), data);
	g_signal_connect(G_OBJECT(renderer), "accel-cleared",
		G_CALLBACK(widget_tree_accel_cleared_callback), data);
}
#endif

#if GTK_CHECK_VERSION(2,10,0)
static gdouble widget_tree_get_spin_double(list_t *values, gint column,
	gdouble fallback, const gchar *attribute)
{
	gdouble value;

	if (values == NULL || column >= values->n_lines ||
		values->line[column][0] == '\0')
		return fallback;
	value = fallback;
	widget_parse_finite_double(values->line[column], &value, attribute);
	return value;
}
#endif

static gboolean widget_tree_get_combo_item_column(GList **element,
	AttributeSet *attributes, gint ncolumns, gint *column, gboolean warn)
{
	gchar combo_column_name[] = "combo-column";
	gchar *end;
	gchar *value;
	gint64 parsed_column;

	value = attributeset_get_this_tagattr(element, attributes, ATTR_ITEM,
		combo_column_name);
	if (value == NULL)
		return FALSE;

	errno = 0;
	parsed_column = g_ascii_strtoll(value, &end, 10);
	if (!widget_tree_number_is_valid(value, end) ||
		parsed_column < 0 || parsed_column >= ncolumns) {
		*column = -1;
		if (warn)
			gtkdialog_warning("Invalid tree item combo-column value '%s'; "
				"ignoring the combo option.", value);
	} else {
		*column = (gint)parsed_column;
	}
	return TRUE;
}

#if GTK_CHECK_VERSION(2,6,0)
static GtkListStore *widget_tree_create_combo_store(AttributeSet *attributes,
	gint combo_column, gint ncolumns)
{
	GList *element;
	GtkListStore *store;
	GtkTreeIter iter;
	gchar *item;
	gint item_column;

	store = gtk_list_store_new(1, G_TYPE_STRING);
	item = attributeset_get_first(&element, attributes, ATTR_ITEM);
	while (item != NULL) {
		if (widget_tree_get_combo_item_column(&element, attributes, ncolumns,
			&item_column, FALSE) && item_column == combo_column) {
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter, 0, item, -1);
		}
		item = attributeset_get_next(&element, attributes, ATTR_ITEM);
	}

	return store;
}
#endif

static void widget_tree_toggle_cell_data_function(
	GtkTreeViewColumn *column, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	gint model_column;

	(void)column;
	model_column = GPOINTER_TO_INT(user_data);
	g_object_set(G_OBJECT(renderer), "active",
		widget_tree_get_column_boolean(model, iter, model_column), NULL);
	widget_tree_apply_json_cell_style(renderer, model, iter);
}

static void widget_tree_toggle_callback(GtkCellRendererToggle *renderer,
	gchar *path, TreeCellData *data)
{
	GtkTreeIter iter;
	GtkTreeIter parent;
	GtkTreeIter sibling;
	GtkTreeModel *model;
	GtkTreeSelection *selection;
	GtkWidget *tree_view;
	AttributeSet *attributes;

	(void)renderer;
	tree_view = data->tree_view;
	attributes = data->attributes;
	g_object_ref(tree_view);
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
	if (!gtk_tree_model_get_iter_from_string(model, &iter, path))
		goto done;

	if (data->radio) {
		if (widget_tree_get_column_boolean(model, &iter, data->model_column))
			goto done;
		if (gtk_tree_model_iter_parent(model, &parent, &iter))
			gtk_tree_model_iter_children(model, &sibling, &parent);
		else
			gtk_tree_model_get_iter_first(model, &sibling);
		do {
			widget_tree_set_column_boolean(GTK_TREE_STORE(model), &sibling,
				data->model_column, FALSE);
		} while (gtk_tree_model_iter_next(model, &sibling));
		widget_tree_set_column_boolean(GTK_TREE_STORE(model), &iter,
			data->model_column, TRUE);
	} else {
		widget_tree_set_column_boolean(GTK_TREE_STORE(model), &iter,
			data->model_column,
			!widget_tree_get_column_boolean(model, &iter,
				data->model_column));
	}
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
	if (gtk_tree_selection_get_mode(selection) != GTK_SELECTION_NONE &&
		!gtk_tree_selection_iter_is_selected(selection, &iter))
		gtk_tree_selection_select_iter(selection, &iter);
	widget_signal_executor(tree_view, attributes, "cell-toggled");

done:
	g_object_unref(tree_view);
}

static void widget_tree_enable_toggle(GtkCellRenderer *renderer,
	GtkWidget *tree_view, AttributeSet *attributes, gint model_column,
	gboolean radio)
{
	TreeCellData *data;

	data = widget_tree_cell_data_new(tree_view, attributes, model_column);
	data->radio = radio;
	g_object_set(G_OBJECT(renderer), "activatable", TRUE, NULL);
	g_object_set_data_full(G_OBJECT(renderer),
		"_gtkdialog-tree-cell-data", data, g_free);
	g_signal_connect(G_OBJECT(renderer), "toggled",
		G_CALLBACK(widget_tree_toggle_callback), data);
}

#if GTK_CHECK_VERSION(2,20,0)
static void widget_tree_spinner_cell_data_function(
	GtkTreeViewColumn *column, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	gint model_column;

	(void)column;
	model_column = GPOINTER_TO_INT(user_data);
	g_object_set(G_OBJECT(renderer), "active",
		widget_tree_get_column_boolean(model, iter, model_column), NULL);
	widget_tree_apply_json_cell_style(renderer, model, iter);
}

static gboolean widget_tree_spinner_pulse(gpointer user_data)
{
	TreeSpinnerData *data = user_data;

	++data->pulse;
	g_object_set(G_OBJECT(data->renderer), "pulse", data->pulse, NULL);
	gtk_widget_queue_draw(data->tree_view);
	return TRUE;
}

static void widget_tree_spinner_stop(GtkWidget *tree_view, gpointer user_data)
{
	(void)tree_view;
	g_source_remove(GPOINTER_TO_UINT(user_data));
}

static void widget_tree_enable_spinner(GtkCellRenderer *renderer,
	GtkWidget *tree_view)
{
	TreeSpinnerData *data;
	guint source_id;

	data = g_new0(TreeSpinnerData, 1);
	data->tree_view = tree_view;
	data->renderer = renderer;
	source_id = g_timeout_add_full(G_PRIORITY_DEFAULT, 100,
		widget_tree_spinner_pulse, data, g_free);
	g_signal_connect(G_OBJECT(tree_view), "destroy",
		G_CALLBACK(widget_tree_spinner_stop), GUINT_TO_POINTER(source_id));
}
#endif

#if GTK_CHECK_VERSION(2,6,0)
static gint widget_tree_get_column_progress(GtkTreeModel *model,
	GtkTreeIter *iter, gint column)
{
	gchar *end;
	gchar *string_value;
	gdouble value;
	gint64 int64_value;
	guint64 uint64_value;

	switch (gtk_tree_model_get_column_type(model, column)) {
		case G_TYPE_STRING:
			gtk_tree_model_get(model, iter, column, &string_value, -1);
			if (string_value == NULL)
				return 0;
			errno = 0;
			value = g_ascii_strtod(string_value, &end);
			if (!widget_tree_number_is_valid(string_value, end) ||
				!isfinite(value))
				value = 0.0;
			g_free(string_value);
			break;
		case G_TYPE_INT64:
			gtk_tree_model_get(model, iter, column, &int64_value, -1);
			if (int64_value <= 0)
				return 0;
			if (int64_value >= 100)
				return 100;
			return (gint)int64_value;
		case G_TYPE_UINT64:
			gtk_tree_model_get(model, iter, column, &uint64_value, -1);
			if (uint64_value >= 100)
				return 100;
			return (gint)uint64_value;
		case G_TYPE_DOUBLE:
			gtk_tree_model_get(model, iter, column, &value, -1);
			break;
		default:
			return 0;
	}

	if (!isfinite(value) || value <= 0.0)
		return 0;
	if (value >= 100.0)
		return 100;
	return (gint)(value + 0.5);
}

static void widget_tree_progress_cell_data_function(
	GtkTreeViewColumn *column, GtkCellRenderer *renderer,
	GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data)
{
	gint model_column;

	(void)column;
	model_column = GPOINTER_TO_INT(user_data);
	g_object_set(G_OBJECT(renderer), "value",
		widget_tree_get_column_progress(model, iter, model_column), NULL);
	widget_tree_apply_json_cell_style(renderer, model, iter);
}
#endif

static void widget_tree_path_free(gpointer data, gpointer user_data)
{
	(void)user_data;
	gtk_tree_path_free(data);
}

/* Notes:
 * I want to note that g_object_get_data is being used quite a bit here
 * where normally get_tag_attribute would be used instead.
 * 
 * I've tidied this widget up considerably and even fixed the problematic
 * pixmap rendering because I want to leave it in a usable stable state
 * but I don't recommend spending any more time on it as it's possibly
 * using the wrong GtkTreeStore model when GtkListStore might've been
 * the more suitable one. This would have to be tested though */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_tree_clear(variable *var)
{
	GtkTreeModel     *model;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We drop all the lines here */
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget));
	widget_tree_renderer_caches_clear(var->Widget);
	gtk_tree_store_clear(GTK_TREE_STORE(model));
	/* Reset the current sort column to unsorted ascending */
	gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(model),
		GTK_TREE_SORTABLE_UNSORTED_SORT_COLUMN_ID, GTK_SORT_ASCENDING);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/
/* Handling TreeView widgets.
 * 
 * The model always has the following columns:
 *   0) "pixbuf"     GdkPixbuf
 *   1) "icon-name"  gchar array
 *   2) "stock-id"   gchar array
 * 
 * We create the GtkTreeStore in:
 *   widget_tree_create_tree_store
 * We create the GtkTreeView in:
 *   widget_tree_create_tree_view */

GtkWidget *widget_tree_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkTreeSelection  *selection;
	GtkTreeStore      *store;
	GtkWidget         *widget;
	gchar             *value;	
	GtkSelectionMode   selectionmode;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We need a label so set a default if one wasn't declared */
	if (!attributeset_is_avail(Attr, ATTR_LABEL))
		attributeset_set_if_unset(Attr, ATTR_LABEL, "Column 0");

	/* Creating the tree store to hold the data and the tree view to
	 * represent it */
	store = widget_tree_create_tree_store(Attr, attr);
	widget = widget_tree_create_tree_view(Attr, attr, store);
	/* GtkTreeView owns a reference to its model. */
	g_object_unref(store);

	/* Thunor: Now we deal with setting the selection mode. The default
	 * is GTK_SELECTION_SINGLE so we'll leave that alone and only set a
	 * different mode if the user requests it */
	if (attr) {
		if ((value = get_tag_attribute(attr, "selection-mode"))) {
			/* Get a pointer to the selection object and set the requested mode */
			selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
			selectionmode = widget_parse_selection_mode(value, TRUE,
				"tree selection-mode");
			gtk_tree_selection_set_mode(selection, selectionmode);
		}
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_tree_envvar_construct(GtkWidget *widget)
{
	GList             *selectedrows, *row;
	GString          *selected_values;
	GtkTreeIter        iter;
	GtkTreeModel      *model;
	GtkTreePath       *path;
	GtkTreeSelection  *selection;
	GType              coltype;
	gchar             *string;
	gchar             *text;
	gdouble            valdouble;
	gint               column;
	gint               index;
	gint               initialrow;
	gint               selectionmode;
	gint64             valint64;
	guint64            valuint64;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Searching the selected row */
	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
	selectionmode = gtk_tree_selection_get_mode(selection);
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): widget=%p selectionmode=%i\n", __func__,
		widget, selectionmode);
	fprintf(stderr, "%s(): widget=%p selected row count=%i\n", __func__,
		widget, gtk_tree_selection_count_selected_rows(selection));
#endif

	if (selectionmode == GTK_SELECTION_NONE) {
		string = g_strdup("");	/* Nothing is selected */
	} else if (selectionmode == GTK_SELECTION_MULTIPLE) {
		/* Thunor: New code to return data from multiple selected rows.
		 * I need to document how it works (for my benefit at least)
		 * and what the user should expect to get from it:
		 * If gtk_tree_selection_count_selected_rows returns 0
		 * then we simply return a new empty string, otherwise
		 * gtk_tree_selection_get_selected_rows gives us a GList of
		 * GtkTreePaths (with a documented empty definition!). Then
		 * gtk_tree_model_get_iter will convert the mysterious
		 * GtkTreePath to a usable GtkTreeIter and then
		 * gtk_tree_model_get reads the data from the column.
		 * The cast GTK_TREE_PATH threw up an undefined reference
		 * warning so I used (GtkTreePath*) instead */
		/* Get exported-column (custom) */
		if ((text = g_object_get_data(G_OBJECT(widget), "exported-column"))) {
			index = widget_parse_column_index(text,
				gtk_tree_model_get_n_columns(model) - FirstDataColumn,
				"tree exported-column") + FirstDataColumn;
		} else {
			index = FirstDataColumn;
		}
		selected_values = g_string_new(NULL);
		if (gtk_tree_selection_count_selected_rows(selection)) {
			selectedrows = gtk_tree_selection_get_selected_rows(selection,
				&model);
			coltype = gtk_tree_model_get_column_type(GTK_TREE_MODEL(model),
				index);
			initialrow = TRUE;
			row = selectedrows;
			while (row) {
				path = (GtkTreePath*)(row->data);
				gtk_tree_model_get_iter(model, &iter, path);
				switch (coltype) {
					case G_TYPE_STRING:
						gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
							index, &string, -1);
						break;
					case G_TYPE_INT64:
						gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
							index, &valint64, -1);
						string = g_strdup_printf("%" G_GINT64_FORMAT, valint64);
						break;
					case G_TYPE_UINT64:
						gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
							index, &valuint64, -1);
						string = g_strdup_printf("%" G_GUINT64_FORMAT, valuint64);
						break;
					case G_TYPE_DOUBLE:
						gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
							index, &valdouble, -1);
						string = g_strdup_printf("%f", valdouble);
						break;
					default:
						fprintf(stderr, "%s(): Unsupported column-type %"
							G_GSIZE_FORMAT "\n", __func__, coltype);
						string = g_strdup("");
				}
				if (!initialrow)
					g_string_append_c(selected_values, '\n');
				g_string_append(selected_values, string);
				initialrow = FALSE;
				g_free(string);
				row = row->next;
			}
			/* The GtkTreePaths and the GList should be freed now */
			g_list_foreach(selectedrows, widget_tree_path_free, NULL);
			g_list_free(selectedrows);
		}

#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): selected_values=%s\n", __func__,
			selected_values->str);
#endif

		string = g_string_free(selected_values, FALSE);
	} else {
		/* GTK_SELECTION_SINGLE and GTK_SELECTION_BROWSE return at most
		 * one row.  The iterator is only initialized when this succeeds. */
		if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
			/* Get exported-column (custom) */
			if ((text = g_object_get_data(G_OBJECT(widget), "exported-column"))) {
				index = widget_parse_column_index(text,
					gtk_tree_model_get_n_columns(model) - FirstDataColumn,
					"tree exported-column") + FirstDataColumn;
			} else {
				index = FirstDataColumn;
			}
			coltype = gtk_tree_model_get_column_type(GTK_TREE_MODEL(model),
				index);
			switch (coltype) {
				case G_TYPE_STRING:
					gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
						index, &string, -1);
					break;
				case G_TYPE_INT64:
					gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
						index, &valint64, -1);
					string = g_strdup_printf("%" G_GINT64_FORMAT, valint64);
					break;
				case G_TYPE_UINT64:
					gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
						index, &valuint64, -1);
					string = g_strdup_printf("%" G_GUINT64_FORMAT, valuint64);
					break;
				case G_TYPE_DOUBLE:
					gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
						index, &valdouble, -1);
					string = g_strdup_printf("%f", valdouble);
					break;
				default:
					fprintf(stderr, "%s(): Unsupported column-type %"
						G_GSIZE_FORMAT "\n", __func__, coltype);
					string = g_strdup("");
			}
		} else {
			string = g_strdup("");
		}
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_tree_fileselect(
	variable *var, const char *name, const char *value)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Fileselect not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Refresh                                                             *
 ***********************************************************************/

void widget_tree_refresh(variable *var)
{
	GList            *element;
	GtkTreeIter       iter;
	GtkTreeSelection *selection;
	GtkTreeModel     *model;
	gchar            *act;
	gchar            *tmp;
	gchar            *value;
	gint              index;
	gint              initialised = FALSE;
	gint              n_columns;
	gint              selected_row;
	gint              selectionmode;
	gint              sort_column;
	gint              sort_type;
	TreeInputContext  context = { NULL, NULL, -1, -1 };

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Get default icon from the custom tag attributes if available */
	if (var->widget_tag_attr) {
		if ((value = get_tag_attribute(var->widget_tag_attr, "stock")) ||
			(value = get_tag_attribute(var->widget_tag_attr, "stock-id")))
			context.stock_id = value;
		if ((value = get_tag_attribute(var->widget_tag_attr, "icon")) ||
			(value = get_tag_attribute(var->widget_tag_attr, "icon-name")))
			context.icon_name = value;
	}

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(var->Widget), "_initialised"));

	/* We drop all the lines here */
	/* Thunor: I'd like to stop doing this but some applications (pbackup,
	 * pcd, petget, pfind, pmusic, pprocess, psip) are refreshing without
	 * clearing first so I'll leave it as is. It's no big deal but giving
	 * the application developer the choice of clearing could make refresh
	 * behave as though it's appending data which is much more useful */
	widget_tree_clear(var);
	model = gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget));
	n_columns = gtk_tree_model_get_n_columns(model) - FirstDataColumn;

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		value = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_INPUT, "format");
		/* Get stock-column from the input tag if available */
		if ((tmp = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_INPUT, "stock-column")) ||
			(tmp = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_INPUT, "stock_column"))) {
			context.stock_column = widget_parse_bounded_integer(tmp, 0,
				n_columns - 1, -1, "tree input stock-column");
		} else {
			context.stock_column = -1;
		}
		/* Get icon-column from the input tag if available */
		if ((tmp = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_INPUT, "icon-column")) ||
			(tmp = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_INPUT, "icon_column"))) {
			context.icon_column = widget_parse_bounded_integer(tmp, 0,
				n_columns - 1, -1, "tree input icon-column");
		} else {
			context.icon_column = -1;
		}

		if (value && strcasecmp(value, "json") == 0) {
			if (input_is_shell_command(act)) {
				widget_tree_input_json(var, act + 8, TRUE, &context);
			} else if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
				if (!initialised)
					widget_file_monitor_try_create(var, act + 5);
				widget_tree_input_json(var, act + 5, FALSE, &context);
			} else {
				widget_tree_input_json(var, act, TRUE, &context);
			}
		} else if (input_is_shell_command(act)) {
#ifdef DEBUG_CONTENT
			printf("%s(): 2: command='%s'\n", __func__, act);
#endif
			widget_tree_input_by_command(var, act + 8, TRUE, &context);
		} else if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
		/* input file stock = "File:", input file = "File:/path/to/file" */
#ifdef DEBUG_CONTENT
			printf("%s(): 1: command='%s'\n", __func__, act);
#endif
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, act + 5);
			}
			widget_tree_input_by_file(var, act + 5, &context);
		} else {
#ifdef DEBUG_CONTENT
			printf("%s(): 3: command='%s'\n", __func__, act);
#endif
			/* Thunor: These are shell commands without the "Command:",
			 * but why is it missing? token_store_with_argument_attr()
			 * manages that. Whoever wrote the widget_tree_refresh()
			 * function knew this which is why an else clause exists here.
			 * 
			 * I think I've found the answer but I'm not going to mess
			 * about with it right now. It looks as though input isn't
			 * set-up properly in the parser which might uncover other
			 * issues when corrected so I'll mark it temp temp
			 * 
			 * [UPDATE] 2012-11-16
			 * I have a suspicion that the recently replaced custom tree
			 * signal handler may have been responsible for the above in
			 * which case it won't be a possible issue anymore.
			 */
			widget_tree_input_by_command(var, act, TRUE, &context);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_tree_input_by_items(var, &context);

	/* Initialise these only once at start-up */
	if (!initialised) {
		/* Apply directives */
		if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
			fprintf(stderr, "%s(): <default> not implemented for this widget.\n",
				__func__);
		if ((attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled")) ||	/* Deprecated */
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no")) ||
			(attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0")))
			gtk_widget_set_sensitive(var->Widget, FALSE);

		/* Connect signals */
		/* The default signal */
		g_signal_connect(G_OBJECT(var->Widget), "row-activated",
			G_CALLBACK(on_any_widget_row_activated_event), (gpointer)var->Attributes);
		/* This was also connected-up but I don't know if anyone is using it.
		 * This signal is always one selection behind which connect-after
		 * doesn't fix so it's of questionable worth */
		g_signal_connect(G_OBJECT(var->Widget), "cursor-changed",
			G_CALLBACK(on_any_widget_cursor_changed_event), (gpointer)var->Attributes);
		/* This signal originates from the selection object which is useful
		 * but we need to route it through a local callback first */
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(var->Widget));
		g_signal_connect(selection, "changed",
			G_CALLBACK(widget_tree_changed_callback), (gpointer)var);
	}

	if (var->widget_tag_attr) {
		/* Get auto-sort (custom) */
		if ((value = get_tag_attribute(var->widget_tag_attr, "auto-sort")) &&
			widget_attribute_is_true(value)) {
			/* Get sort-column (custom) */
			sort_column = widget_parse_column_index(
				get_tag_attribute(var->widget_tag_attr, "sort-column"),
				gtk_tree_model_get_n_columns(model) - FirstDataColumn,
				"tree sort-column");
			/* Get sort-type (custom) */
			sort_type = widget_parse_sort_type(
				get_tag_attribute(var->widget_tag_attr, "sort-type"),
				"tree sort-type");
			/* Sort! */
			gtk_tree_sortable_set_sort_column_id(GTK_TREE_SORTABLE(model),
				sort_column + FirstDataColumn, sort_type);
		}
		/* Get selected-row (custom) */
		if ((value = get_tag_attribute(var->widget_tag_attr, "selected-row"))) {
			selected_row = widget_parse_nonnegative_integer(value, -1,
				"tree selected-row");
			if (selected_row >= 0) {
				selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(var->Widget));
				selectionmode = gtk_tree_selection_get_mode(selection);
				if (selectionmode != GTK_SELECTION_NONE) {
					model = gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget));
					gtk_tree_model_get_iter_first(model, &iter);
					index = 0;
					while (gtk_tree_store_iter_is_valid(GTK_TREE_STORE(model), &iter)) {
						if (selected_row == index) {
							gtk_tree_selection_select_iter(selection, &iter);
							break;
						}
						if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(model), &iter))
							break;
						index++;
					}
				}
			}
		}
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_tree_removeselected(variable *var)
{
	GList             *rowreferences = NULL;
	GList             *selectedrows, *row;
	GtkTreeIter        iter;
	GtkTreeModel      *model;
	GtkTreePath       *path;
	GtkTreeSelection  *selection;
	gint               selectionmode;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(var->Widget));
	selectionmode = gtk_tree_selection_get_mode(selection);
	if (selectionmode == GTK_SELECTION_NONE) {
		/* Nothing to do */
	} else if (selectionmode == GTK_SELECTION_MULTIPLE) {
		/* Thunor: New code to delete multiple selected rows.
		 * http://scentric.net/tutorial/sec-treemodel-rowref.html
		 * is a good place to learn about paths and row references.
		 * This is based on the code I added to widget_get_text_value.
		 * We get the list of selected rows as normal but we can't
		 * iterate through it and delete them one by one because the
		 * paths to the rows change, but we can convert those paths
		 * to GtkTreeRowReferences which we'll store in another list.
		 * GtkTreeRowReferences watch for row changes and maintain
		 * themselves so we can delete them one by one without
		 * worrying about the others becoming out-of-date. */
		if (gtk_tree_selection_count_selected_rows(selection)) {
			selectedrows = gtk_tree_selection_get_selected_rows(selection, &model);
			row = selectedrows;
			while (row) {
				rowreferences = g_list_prepend(rowreferences,
					gtk_tree_row_reference_new(model, (GtkTreePath*)(row->data)));
				row = row->next;
			}
			/* Prepending is constant-time; restore the original removal order. */
			rowreferences = g_list_reverse(rowreferences);
			/* The GtkTreePaths and the GList should be freed now */
			g_list_foreach(selectedrows, widget_tree_path_free, NULL);
			g_list_free(selectedrows);
			row = rowreferences;
			while (row) {
				path = gtk_tree_row_reference_get_path(
					(GtkTreeRowReference*)row->data);
				if (path != NULL && gtk_tree_model_get_iter(model, &iter, path))
					gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
				if (path != NULL)
					gtk_tree_path_free(path);
				gtk_tree_row_reference_free(
					(GtkTreeRowReference*)row->data);
				row = row->next;
			}
			/* The row references were released above; free their list. */
			g_list_free(rowreferences);
		}
	} else {
		/* Thunor: Below is the original code that handles the
		 * default GTK_SELECTION_SINGLE mode and it's quite happy
		 * dealing with GTK_SELECTION_BROWSE too.
		 * 
		 * It didn't check that something is actually selected
		 * but since gtk_tree_selection_get_selected returns
		 * true if something is then making a simple modification
		 * seemed to be the sensible thing to do. Now there's no
		 * Gtk-CRITICAL message appearing in the terminal */
		if (gtk_tree_selection_get_selected(selection, &model, &iter))
			gtk_tree_store_remove(GTK_TREE_STORE(model), &iter);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_tree_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	GtkTreeIter       iter;
	GtkTreeModel     *model;
	GType             coltype;
	gdouble           valdouble;
	gchar            *act;
	gchar            *filename = NULL;
	gchar            *format;
	gchar            *string;
	gboolean           json_format = FALSE;
	gint              column, columnmax;
	gint              initialrow = TRUE;
	gint64            valint64;
	guint64           valuint64;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We'll use the output file filename if available */
	act = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (act) {
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			filename = act + 5;
			format = attributeset_get_this_tagattr(&element, var->Attributes,
				ATTR_OUTPUT, "format");
			json_format = format && strcasecmp(format, "json") == 0;
			break;
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_OUTPUT);
	}

	/* If we have a valid filename then open it and dump the
	 * widget's data to it */
	if (filename) {
		if (json_format) {
			widget_tree_save_json(var, filename);
			return;
		}
		if ((outfile = fopen(filename, "w"))) {

			model = gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget));
			columnmax = gtk_tree_model_get_n_columns(GTK_TREE_MODEL(model));

			gtk_tree_model_get_iter_first(model, &iter);
			while (gtk_tree_store_iter_is_valid(GTK_TREE_STORE(model), &iter)) {
				if (initialrow) {
					initialrow = FALSE;
				} else {
					fputc('\n', outfile);
				}
				for (column = ColumnIconName; column < columnmax; column++) {
					if (column == ColumnJsonStyle)
						continue;

					coltype = gtk_tree_model_get_column_type
						(GTK_TREE_MODEL(model), column);
					switch (coltype) {
						case G_TYPE_STRING:
							gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
								column, &string, -1);
							break;
						case G_TYPE_INT64:
							gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
								column, &valint64, -1);
							string = g_strdup_printf("%" G_GINT64_FORMAT, valint64);
							break;
						case G_TYPE_UINT64:
							gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
								column, &valuint64, -1);
							string = g_strdup_printf("%" G_GUINT64_FORMAT, valuint64);
							break;
						case G_TYPE_DOUBLE:
							gtk_tree_model_get(GTK_TREE_MODEL(model), &iter,
								column, &valdouble, -1);
							string = g_strdup_printf("%f", valdouble);
							break;
						default:
							fprintf(stderr, "%s(): Unsupported column-type %"
								G_GSIZE_FORMAT "\n", __func__, coltype);
							string = g_strdup("");
					}
					if (column != ColumnIconName)
						fputc('|', outfile);
					fputs(string ? string : "", outfile);
					g_free(string);
				}

				if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(model), &iter)) break;
			}

			widget_close_output(outfile, filename);
		} else {
			fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
				__func__, filename);
		}
	} else {
		fprintf(stderr, "%s(): No <output file> directive found.\n", __func__);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create GtkTreeStore                                                 *
 ***********************************************************************/
/* Creating tree store from <label></label> tags */

static GtkTreeStore *widget_tree_create_tree_store(AttributeSet *Attr,
	tag_attr *attr)
{
	GList            *element;
	GtkTreeStore     *treestore;
	GType            *types;
	gchar            *label;
	gchar            *value;
	gint              index;
	gint              ncolumns;
	gint              user_type;
	list_t           *columns = NULL;
	list_t           *column_type = NULL;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We create as many columns as the label tag suggests */
	label = g_strdup(attributeset_get_first(&element, Attr, ATTR_LABEL));
	columns = linecutter(label, '|');
	ncolumns = columns->n_lines;

	/* Set type for image columns */
	types = g_new(GType, ncolumns + FirstDataColumn);
	types[ColumnPixbuf]   = G_TYPE_POINTER;
	types[ColumnIconName] = G_TYPE_STRING;
	types[ColumnStockId]  = G_TYPE_STRING;
#if HAVE_JSON_GLIB
	types[ColumnJsonStyle] = JSON_TYPE_NODE;
#else
	types[ColumnJsonStyle] = G_TYPE_POINTER;
#endif

	if (attr) {
		/* Get column-type (custom) */
		if ((value = get_tag_attribute(attr, "column-type"))) {
			column_type = linecutter(g_strdup(value), '|');
		}
	}

	/* Set type for user columns */
	for (index = FirstDataColumn; index < ncolumns + FirstDataColumn; ++index) {
		/* Set a default type */
		types[index] = G_TYPE_STRING;
		/* Set a user requested type? */
		if (column_type && column_type->n_lines > index - FirstDataColumn) {
			if (strcasecmp(column_type->line[index - FirstDataColumn],
				"int64") == 0) {
				user_type = G_TYPE_INT64;
			} else if (strcasecmp(column_type->line[index - FirstDataColumn],
				"uint64") == 0) {
				user_type = G_TYPE_UINT64;
			} else if (strcasecmp(column_type->line[index - FirstDataColumn],
				"double") == 0) {
				user_type = G_TYPE_DOUBLE;
			} else {
				user_type = G_TYPE_STRING;
			}
#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s(): column_type=%p column_type->n_lines=%i \
index-FirstDataColumn=%i user_type=%i\n", __func__, column_type,
				column_type->n_lines, index - FirstDataColumn, user_type);
#endif
			types[index] = user_type;
		}
	}

	treestore = gtk_tree_store_newv(ncolumns + FirstDataColumn, types);
	g_free(types);

	/* Free linecutter memory */
	if (columns) list_t_free(columns);
	if (column_type) list_t_free(column_type);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return treestore;
}

/***********************************************************************
 * Create GtkTreeView                                                  *
 ***********************************************************************/

static GtkWidget *widget_tree_create_tree_view(AttributeSet *Attr,
	tag_attr *attr, GtkTreeStore *store)
{
	GList             *element;
	GtkCellRenderer   *renderer;
	GtkTreeViewColumn *column;
	GtkWidget         *tree_view;
	gchar             *headline = NULL;
	gchar             *value;
#if GTK_CHECK_VERSION(2,6,0)
	GtkListStore      *combo_store;
#endif
	gboolean            column_is_editable;
	gboolean            column_is_accel;
	gboolean            column_is_combo;
	gboolean            column_is_color;
	gboolean            column_is_icon_text;
	gboolean            column_is_markup;
	gboolean            column_is_pixbuf;
	gboolean            column_is_radio;
#if GTK_CHECK_VERSION(2,6,0)
	gboolean            column_combo_has_entry;
#endif
	gboolean            column_is_progress;
	gboolean            column_is_spin;
	gboolean            column_is_spinner;
	gboolean            column_is_toggle;
	gint               index;
	gint               function;
#if GTK_CHECK_VERSION(2,10,0)
	gdouble             spin_maximum;
	gdouble             spin_minimum;
	gdouble             spin_step;
	gint                spin_digits;
	GtkObject          *spin_adjustment;
#endif
	GtkCellRenderer   *image_renderer;
	list_t            *column_sizing = NULL;
	list_t            *column_resizeable = NULL;
	list_t            *column_header_active = NULL;
	list_t            *column_editable = NULL;
	list_t            *column_combo_has_entry_values = NULL;
	list_t            *column_renderer = NULL;
	list_t            *column_spin_digits = NULL;
	list_t            *column_spin_max = NULL;
	list_t            *column_spin_min = NULL;
	list_t            *column_spin_step = NULL;
	list_t            *column_visible = NULL;
	list_t            *column_sort_function = NULL;
	list_t            *columns = NULL;
	TreeColumnStyles    column_styles;
	TreeImageOptions    image_options;
	TreeRendererOptions renderer_options;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	headline = g_strdup(attributeset_get_first(&element, Attr, ATTR_LABEL));
	columns = linecutter(headline, '|');
	widget_tree_column_styles_init(&column_styles, attr);
	widget_tree_image_options_init(&image_options, attr);
	widget_tree_renderer_options_init(&renderer_options, attr);

	if (attr) {
		/* Get column-sizing (custom) */
		if ((value = get_tag_attribute(attr, "column-sizing")))
			column_sizing = linecutter(g_strdup(value), '|');
		/* Get column-resizeable (custom) */
		if ((value = get_tag_attribute(attr, "column-resizeable")))
			column_resizeable = linecutter(g_strdup(value), '|');
		/* Get column-header-active (custom) */
		if ((value = get_tag_attribute(attr, "column-header-active")))
			column_header_active = linecutter(g_strdup(value), '|');
		/* Get column-editable (custom) */
		if ((value = get_tag_attribute(attr, "column-editable")))
			column_editable = linecutter(g_strdup(value), '|');
		/* Get column-combo-has-entry (custom) */
		if ((value = get_tag_attribute(attr, "column-combo-has-entry")))
			column_combo_has_entry_values = linecutter(g_strdup(value), '|');
		/* Get column-renderer (custom) */
		if ((value = get_tag_attribute(attr, "column-renderer")))
			column_renderer = linecutter(g_strdup(value), '|');
		/* Get spin renderer settings (custom) */
		if ((value = get_tag_attribute(attr, "column-spin-min")))
			column_spin_min = linecutter(g_strdup(value), '|');
		if ((value = get_tag_attribute(attr, "column-spin-max")))
			column_spin_max = linecutter(g_strdup(value), '|');
		if ((value = get_tag_attribute(attr, "column-spin-step")))
			column_spin_step = linecutter(g_strdup(value), '|');
		if ((value = get_tag_attribute(attr, "column-spin-digits")))
			column_spin_digits = linecutter(g_strdup(value), '|');
		/* Get column-visible (custom) */
		if ((value = get_tag_attribute(attr, "column-visible")))
			column_visible = linecutter(g_strdup(value), '|');
		/* Get column-sort-function (custom) */
		if ((value = get_tag_attribute(attr, "column-sort-function")))
			column_sort_function = linecutter(g_strdup(value), '|');
	}

	/* We create the TreeView here */
	tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

	/* Remove the default indentation in column 0 */
	gtk_tree_view_set_show_expanders(GTK_TREE_VIEW(tree_view), FALSE);

	/* Create the columns */
	for (index = 0; index < columns->n_lines; ++index) {
		column = gtk_tree_view_column_new();
		gtk_tree_view_column_set_title(column, columns->line[index]);
		gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
		gtk_tree_view_column_set_sort_column_id(column, index + FirstDataColumn);
		column_is_editable = column_editable &&
			index < column_editable->n_lines &&
			widget_attribute_is_true(column_editable->line[index]);
		column_is_radio = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "radio") == 0;
		column_is_toggle = column_is_radio || (column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "toggle") == 0);
		column_is_color = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "color") == 0;
		if (column_is_color &&
			gtk_tree_model_get_column_type(GTK_TREE_MODEL(store),
				index + FirstDataColumn) != G_TYPE_STRING) {
			gtkdialog_warning("Tree color renderer requires a string column; "
				"using text instead.");
			column_is_color = FALSE;
		}
		column_is_pixbuf = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "pixbuf") == 0;
		column_is_icon_text = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "icon-text") == 0;
		column_is_markup = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "markup") == 0;
		if (column_is_markup &&
			gtk_tree_model_get_column_type(GTK_TREE_MODEL(store),
				index + FirstDataColumn) != G_TYPE_STRING) {
			gtkdialog_warning("Tree markup renderer requires a string column; "
				"using text instead.");
			column_is_markup = FALSE;
		}
#if GTK_CHECK_VERSION(2,6,0)
		column_is_combo = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "combo") == 0;
		column_combo_has_entry = column_combo_has_entry_values &&
			index < column_combo_has_entry_values->n_lines &&
			widget_attribute_is_true(
				column_combo_has_entry_values->line[index]);
		column_is_progress = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "progress") == 0;
#else
		column_is_combo = FALSE;
		column_is_progress = FALSE;
#endif
#if GTK_CHECK_VERSION(2,10,0)
		column_is_accel = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "accel") == 0;
		if (column_is_accel &&
			gtk_tree_model_get_column_type(GTK_TREE_MODEL(store),
				index + FirstDataColumn) != G_TYPE_STRING) {
			gtkdialog_warning("Tree accel renderer requires a string column; "
				"using text instead.");
			column_is_accel = FALSE;
		}
		column_is_spin = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "spin") == 0;
#else
		column_is_accel = FALSE;
		column_is_spin = FALSE;
#endif
#if GTK_CHECK_VERSION(2,20,0)
		column_is_spinner = column_renderer &&
			index < column_renderer->n_lines &&
			strcasecmp(column_renderer->line[index], "spinner") == 0;
#else
		column_is_spinner = FALSE;
#endif

		if (index == 0 && !column_is_color && !column_is_pixbuf &&
			!column_is_icon_text) {
			/* The first column includes the optional row icon renderer. */
			renderer = gtk_cell_renderer_pixbuf_new();
			gtk_tree_view_column_pack_start(column, renderer, FALSE);
			gtk_cell_layout_set_cell_data_func(GTK_CELL_LAYOUT(column),
				renderer,
				(GtkCellLayoutDataFunc)widget_tree_pixmap_column_cell_layout_function,
				tree_view, NULL);
		}
		if (column_is_color) {
			renderer = widget_tree_create_color_renderer(tree_view, column,
				index, index + FirstDataColumn, &renderer_options);
		} else if (column_is_pixbuf) {
			renderer = widget_tree_create_image_renderer(tree_view, column,
				index, index + FirstDataColumn, &image_options, TRUE);
		} else if (column_is_icon_text) {
			image_renderer = widget_tree_create_image_renderer(tree_view, column,
				index, index + FirstDataColumn, &image_options, FALSE);
			widget_tree_apply_column_style(image_renderer, &column_styles,
				index);
			widget_tree_attach_json_style(image_renderer, tree_view, index,
				FALSE);
			renderer = gtk_cell_renderer_text_new();
			gtk_tree_view_column_pack_start(column, renderer, TRUE);
			gtk_tree_view_column_add_attribute(column, renderer, "text",
				index + FirstDataColumn);
			if (column_is_editable)
				widget_tree_enable_text_editing(renderer, tree_view, Attr,
					index + FirstDataColumn);
		} else if (column_is_markup) {
			renderer = gtk_cell_renderer_text_new();
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_add_attribute(column, renderer, "markup",
				index + FirstDataColumn);
			if (column_is_editable)
				widget_tree_enable_text_editing(renderer, tree_view, Attr,
					index + FirstDataColumn);
		} else if (column_is_progress) {
#if GTK_CHECK_VERSION(2,6,0)
			renderer = gtk_cell_renderer_progress_new();
			value = (gchar *)widget_tree_column_value(
				renderer_options.progress_text, index);
			if (value != NULL)
				g_object_set(G_OBJECT(renderer), "text", value, NULL);
			value = (gchar *)widget_tree_column_value(
				renderer_options.progress_orientation, index);
			if (value != NULL)
				g_object_set(G_OBJECT(renderer), "orientation",
					widget_tree_parse_progress_orientation(value), NULL);
			value = (gchar *)widget_tree_column_value(
				renderer_options.progress_pulse, index);
			if (value != NULL)
				g_object_set(G_OBJECT(renderer), "pulse",
					widget_parse_bounded_integer(value, -1, G_MAXINT, -1,
						"tree column-progress-pulse"), NULL);
			value = (gchar *)widget_tree_column_value(
				renderer_options.progress_text_xalign, index);
			if (value != NULL)
				widget_tree_set_fraction_property(G_OBJECT(renderer),
					"text-xalign", value,
					"tree column-progress-text-xalign");
			value = (gchar *)widget_tree_column_value(
				renderer_options.progress_text_yalign, index);
			if (value != NULL)
				widget_tree_set_fraction_property(G_OBJECT(renderer),
					"text-yalign", value,
					"tree column-progress-text-yalign");
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_set_cell_data_func(column, renderer,
				widget_tree_progress_cell_data_function,
				GINT_TO_POINTER(index + FirstDataColumn), NULL);
#endif
		} else if (column_is_toggle) {
			renderer = gtk_cell_renderer_toggle_new();
			g_object_set(G_OBJECT(renderer), "activatable", FALSE,
				"radio", column_is_radio, NULL);
			value = (gchar *)widget_tree_column_value(
				renderer_options.toggle_inconsistent, index);
			if (value != NULL)
				g_object_set(G_OBJECT(renderer), "inconsistent",
					widget_attribute_is_true(value), NULL);
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_set_cell_data_func(column, renderer,
				widget_tree_toggle_cell_data_function,
				GINT_TO_POINTER(index + FirstDataColumn), NULL);
			if (column_is_editable)
				widget_tree_enable_toggle(renderer, tree_view, Attr,
					index + FirstDataColumn, column_is_radio);
		} else if (column_is_combo) {
#if GTK_CHECK_VERSION(2,6,0)
			combo_store = widget_tree_create_combo_store(Attr, index,
				columns->n_lines);
			renderer = gtk_cell_renderer_combo_new();
			g_object_set(G_OBJECT(renderer), "model", combo_store,
				"text-column", 0, "has-entry", column_combo_has_entry, NULL);
			/* The renderer owns its model reference and the store copies text. */
			g_object_unref(combo_store);
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_add_attribute(column, renderer, "text",
				index + FirstDataColumn);
			if (column_is_editable)
				widget_tree_enable_text_editing(renderer, tree_view, Attr,
					index + FirstDataColumn);
#endif
		} else if (column_is_accel) {
#if GTK_CHECK_VERSION(2,10,0)
			renderer = gtk_cell_renderer_accel_new();
			value = (gchar *)widget_tree_column_value(
				renderer_options.accel_mode, index);
			if (value != NULL) {
				if (strcasecmp(value, "gtk") == 0)
					g_object_set(G_OBJECT(renderer), "accel-mode",
						GTK_CELL_RENDERER_ACCEL_MODE_GTK, NULL);
				else if (strcasecmp(value, "other") == 0)
					g_object_set(G_OBJECT(renderer), "accel-mode",
						GTK_CELL_RENDERER_ACCEL_MODE_OTHER, NULL);
				else
					gtkdialog_warning("Invalid tree column-accel-mode value "
						"'%s'; using gtk.", value);
			}
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_set_cell_data_func(column, renderer,
				widget_tree_accel_cell_data_function,
				GINT_TO_POINTER(index + FirstDataColumn), NULL);
			if (column_is_editable)
				widget_tree_enable_accel(renderer, tree_view, Attr,
					index + FirstDataColumn);
#endif
		} else if (column_is_spin) {
#if GTK_CHECK_VERSION(2,10,0)
			spin_minimum = widget_tree_get_spin_double(column_spin_min, index,
				0.0, "tree column-spin-min");
			spin_maximum = widget_tree_get_spin_double(column_spin_max, index,
				100.0, "tree column-spin-max");
			spin_step = widget_tree_get_spin_double(column_spin_step, index,
				1.0, "tree column-spin-step");
			if (spin_minimum >= spin_maximum ||
				!isfinite(spin_maximum - spin_minimum)) {
				gtkdialog_warning("Invalid tree spin range; using 0..100.");
				spin_minimum = 0.0;
				spin_maximum = 100.0;
			}
			if (spin_step <= 0.0) {
				gtkdialog_warning("Invalid tree column-spin-step; using 1.");
				spin_step = 1.0;
			}
			spin_digits = 0;
			if (column_spin_digits && index < column_spin_digits->n_lines &&
				column_spin_digits->line[index][0] != '\0')
				spin_digits = widget_parse_bounded_integer(
					column_spin_digits->line[index], 0, 20, 0,
					"tree column-spin-digits");
			spin_adjustment = gtk_adjustment_new(spin_minimum, spin_minimum,
				spin_maximum, spin_step, spin_step, 0.0);
			/* Give the renderer its own reference, then drop the local one. */
			g_object_ref_sink(spin_adjustment);
			renderer = gtk_cell_renderer_spin_new();
			g_object_set(G_OBJECT(renderer), "adjustment", spin_adjustment,
				"digits", spin_digits, NULL);
			value = (gchar *)widget_tree_column_value(
				renderer_options.spin_climb_rate, index);
			if (value != NULL && widget_parse_finite_double(value,
				&spin_step, "tree column-spin-climb-rate")) {
				if (spin_step >= 0.0)
					g_object_set(G_OBJECT(renderer), "climb-rate",
						spin_step, NULL);
				else
					gtkdialog_warning("Tree column-spin-climb-rate cannot be "
						"negative; ignoring '%s'.", value);
			}
			g_object_unref(spin_adjustment);
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_add_attribute(column, renderer, "text",
				index + FirstDataColumn);
			if (column_is_editable)
				widget_tree_enable_text_editing(renderer, tree_view, Attr,
					index + FirstDataColumn);
#endif
		} else if (column_is_spinner) {
#if GTK_CHECK_VERSION(2,20,0)
			renderer = gtk_cell_renderer_spinner_new();
			value = (gchar *)widget_tree_column_value(
				renderer_options.spinner_size, index);
			if (value != NULL)
				g_object_set(G_OBJECT(renderer), "size",
					widget_tree_parse_icon_size(value,
						"tree column-spinner-size"), NULL);
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_set_cell_data_func(column, renderer,
				widget_tree_spinner_cell_data_function,
				GINT_TO_POINTER(index + FirstDataColumn), NULL);
			widget_tree_enable_spinner(renderer, tree_view);
#endif
		} else {
			renderer = gtk_cell_renderer_text_new();
			gtk_tree_view_column_pack_start(column, renderer, index != 0);
			gtk_tree_view_column_add_attribute(column, renderer, "text",
				index + FirstDataColumn);
			if (column_is_editable)
				widget_tree_enable_text_editing(renderer, tree_view, Attr,
					index + FirstDataColumn);
		}
		widget_tree_apply_column_style(renderer, &column_styles, index);
		widget_tree_attach_json_style(renderer, tree_view, index,
			!column_is_progress && !column_is_toggle && !column_is_accel &&
			!column_is_spinner && !column_is_color && !column_is_pixbuf);
		/* Resize mode? (the default is to grow) */
		if (column_sizing && index < column_sizing->n_lines) {
			function = widget_parse_bounded_integer(column_sizing->line[index],
				GTK_TREE_VIEW_COLUMN_GROW_ONLY, G_MAXINT,
				GTK_TREE_VIEW_COLUMN_GROW_ONLY, "tree column-sizing");
			if (function == GTK_TREE_VIEW_COLUMN_GROW_ONLY) {
				gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_GROW_ONLY);
			} else if (function == GTK_TREE_VIEW_COLUMN_AUTOSIZE) {
				gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
			} else {
				gtk_tree_view_column_set_sizing(column, GTK_TREE_VIEW_COLUMN_FIXED);
				gtk_tree_view_column_set_fixed_width(column, function);
			}
		}
		/* Resizeable column? (gtkdialog's default is resizeable) */
		gtk_tree_view_column_set_resizable(column, TRUE);
		if (column_resizeable && index < column_resizeable->n_lines) {
			if ((strcasecmp(column_resizeable->line[index], "false") == 0) ||
				(strcasecmp(column_resizeable->line[index], "no") == 0) ||
				(strcasecmp(column_resizeable->line[index], "0") == 0)) {
				gtk_tree_view_column_set_resizable(column, FALSE);
			}
		}
		/* Deactivate column header? (the default is active) */
		if (column_header_active && index < column_header_active->n_lines) {
			if ((strcasecmp(column_header_active->line[index], "false") == 0) ||
				(strcasecmp(column_header_active->line[index], "no") == 0) ||
				(strcasecmp(column_header_active->line[index], "0") == 0)) {
				gtk_tree_view_column_set_clickable(column, FALSE);
			}
		}
		/* Hide column? (the default is shown) */
		if (column_visible && index < column_visible->n_lines) {
			if ((strcasecmp(column_visible->line[index], "false") == 0) ||
				(strcasecmp(column_visible->line[index], "no") == 0) ||
				(strcasecmp(column_visible->line[index], "0") == 0)) {
				gtk_tree_view_column_set_visible(column, FALSE);
			}
		}
		/* Set custom sort function for column? */
		if (column_sort_function && index < column_sort_function->n_lines) {
			function = widget_parse_bounded_integer(
				column_sort_function->line[index], 0, 2, 0,
				"tree column-sort-function");
			if (function == 1 || function == 2) {
				if ((gtk_tree_model_get_column_type(GTK_TREE_MODEL(store),
					index + FirstDataColumn) == G_TYPE_STRING)) {
					if (function == 1) {
						gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store),
							index + FirstDataColumn, widget_tree_natcmp,
							GINT_TO_POINTER(index + FirstDataColumn), NULL);
					} else {
						gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(store),
							index + FirstDataColumn, widget_tree_natcasecmp,
							GINT_TO_POINTER(index + FirstDataColumn), NULL);
					}
				} else {
					fprintf(stderr, "%s(): column-sort-function: natural \
sorting is compatible only with columns of type string.\n", __func__);
				}
			}
		}
	}

	/* Free linecutter memory */
	if (column_sizing) list_t_free(column_sizing);
	if (column_resizeable) list_t_free(column_resizeable);
	if (column_header_active) list_t_free(column_header_active);
	if (column_editable) list_t_free(column_editable);
	if (column_combo_has_entry_values)
		list_t_free(column_combo_has_entry_values);
	if (column_renderer) list_t_free(column_renderer);
	if (column_spin_digits) list_t_free(column_spin_digits);
	if (column_spin_max) list_t_free(column_spin_max);
	if (column_spin_min) list_t_free(column_spin_min);
	if (column_spin_step) list_t_free(column_spin_step);
	if (column_visible) list_t_free(column_visible);
	if (column_sort_function) list_t_free(column_sort_function);
	if (columns) list_t_free(columns);
	widget_tree_column_styles_free(&column_styles);
	widget_tree_image_options_free(&image_options);
	widget_tree_renderer_options_free(&renderer_options);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return tree_view;
}

#if HAVE_JSON_GLIB
static gchar *widget_tree_json_scalar_to_string(JsonNode *node,
	GType column_type);

static gboolean widget_tree_json_value_is_scalar(JsonNode *node)
{
	GType type;

	if (JSON_NODE_HOLDS_NULL(node))
		return TRUE;
	if (!JSON_NODE_HOLDS_VALUE(node))
		return FALSE;
	type = json_node_get_value_type(node);
	return type == G_TYPE_STRING || type == G_TYPE_INT64 ||
		type == G_TYPE_DOUBLE || type == G_TYPE_BOOLEAN;
}

static gboolean widget_tree_json_value_matches_column(JsonNode *node,
	GType column_type)
{
	const gchar *start;
	gchar *end;
	gchar *value;
	gdouble double_value;
	gboolean valid;

	if (JSON_NODE_HOLDS_NULL(node) || column_type == G_TYPE_STRING)
		return TRUE;
	value = widget_tree_json_scalar_to_string(node, column_type);
	start = value;
	while (g_ascii_isspace(*start))
		++start;
	errno = 0;
	if (column_type == G_TYPE_INT64) {
		(void)g_ascii_strtoll(value, &end, 0);
		valid = widget_tree_number_is_valid(value, end);
	} else if (column_type == G_TYPE_UINT64) {
		(void)g_ascii_strtoull(value, &end, 0);
		valid = *start != '-' && widget_tree_number_is_valid(value, end);
	} else if (column_type == G_TYPE_DOUBLE) {
		double_value = g_ascii_strtod(value, &end);
		valid = widget_tree_number_is_valid(value, end) &&
			isfinite(double_value);
	} else {
		valid = FALSE;
	}
	g_free(value);
	return valid;
}

static const gchar *widget_tree_json_column_type_name(GType column_type)
{
	if (column_type == G_TYPE_STRING)
		return "string";
	if (column_type == G_TYPE_INT64)
		return "int64";
	if (column_type == G_TYPE_UINT64)
		return "uint64";
	if (column_type == G_TYPE_DOUBLE)
		return "double";
	return "unsupported";
}

static gboolean widget_tree_json_style_enum_is_valid(const gchar *name,
	const gchar *value)
{
	if (strcmp(name, "style") == 0)
		return strcasecmp(value, "normal") == 0 ||
			strcasecmp(value, "oblique") == 0 ||
			strcasecmp(value, "italic") == 0;
	if (strcmp(name, "underline") == 0)
		return strcasecmp(value, "none") == 0 ||
			strcasecmp(value, "single") == 0 ||
			strcasecmp(value, "double") == 0 ||
			strcasecmp(value, "low") == 0 ||
			strcasecmp(value, "error") == 0;
	if (strcmp(name, "ellipsize") == 0)
		return strcasecmp(value, "none") == 0 ||
			strcasecmp(value, "start") == 0 ||
			strcasecmp(value, "middle") == 0 ||
			strcasecmp(value, "end") == 0;
	if (strcmp(name, "wrap-mode") == 0)
		return strcasecmp(value, "word") == 0 ||
			strcasecmp(value, "char") == 0 ||
			strcasecmp(value, "word-char") == 0;
	if (strcmp(name, "text-align") == 0)
		return strcasecmp(value, "left") == 0 ||
			strcasecmp(value, "center") == 0 ||
			strcasecmp(value, "right") == 0;
	return TRUE;
}

static gboolean widget_tree_json_validate_style(JsonObject *style,
	const gchar *context, GError **error)
{
	static const gchar *string_names[] = {
		"background", "foreground", "font", "style", "underline",
		"ellipsize", "wrap-mode", "text-align", "icon-name",
		"stock-id", "pixbuf-file", NULL
	};
	static const gchar *number_names[] = { "xalign", "yalign", NULL };
	static const gchar *integer_names[] = {
		"xpad", "ypad", "wrap-width", "pixbuf-size", NULL
	};
	GdkColor colour;
	gdouble number;
	gint64 integer;
	JsonNode *node;
	GType type;
	gint index;
	const gchar *value;

	for (index = 0; string_names[index] != NULL; ++index) {
		if (!json_object_has_member(style, string_names[index]))
			continue;
		node = json_object_get_member(style, string_names[index]);
		if (!JSON_NODE_HOLDS_VALUE(node) ||
			json_node_get_value_type(node) != G_TYPE_STRING) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON %s %s is not a string", context,
				string_names[index]);
			return FALSE;
		}
		value = json_node_get_string(node);
		if ((strcmp(string_names[index], "background") == 0 ||
			strcmp(string_names[index], "foreground") == 0) &&
			!gdk_color_parse(value, &colour)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON %s %s is not a valid colour", context,
				string_names[index]);
			return FALSE;
		}
		if (!widget_tree_json_style_enum_is_valid(string_names[index],
			value)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON %s %s has invalid value '%s'", context,
				string_names[index], value);
			return FALSE;
		}
	}
	if (json_object_has_member(style, "weight")) {
		node = json_object_get_member(style, "weight");
		type = JSON_NODE_HOLDS_VALUE(node) ?
			json_node_get_value_type(node) : G_TYPE_INVALID;
		if (type == G_TYPE_STRING) {
			value = json_node_get_string(node);
			if (strcasecmp(value, "light") != 0 &&
				strcasecmp(value, "normal") != 0 &&
				strcasecmp(value, "bold") != 0)
				type = G_TYPE_INVALID;
		} else if (type != G_TYPE_INT64 || json_node_get_int(node) < 1 ||
			json_node_get_int(node) > 1000) {
			type = G_TYPE_INVALID;
		}
		if (type == G_TYPE_INVALID) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON %s weight is invalid", context);
			return FALSE;
		}
	}
	if (json_object_has_member(style, "strikethrough")) {
		node = json_object_get_member(style, "strikethrough");
		if (!JSON_NODE_HOLDS_VALUE(node) ||
			json_node_get_value_type(node) != G_TYPE_BOOLEAN) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON %s strikethrough is not Boolean", context);
			return FALSE;
		}
	}
	for (index = 0; number_names[index] != NULL; ++index) {
		if (!json_object_has_member(style, number_names[index]))
			continue;
		node = json_object_get_member(style, number_names[index]);
		type = JSON_NODE_HOLDS_VALUE(node) ?
			json_node_get_value_type(node) : G_TYPE_INVALID;
		if (type != G_TYPE_INT64 && type != G_TYPE_DOUBLE) {
			number = -1.0;
		} else {
			number = json_node_get_double(node);
		}
		if (!isfinite(number) || number < 0.0 || number > 1.0) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON %s %s is outside 0..1", context,
				number_names[index]);
			return FALSE;
		}
	}
	for (index = 0; integer_names[index] != NULL; ++index) {
		if (!json_object_has_member(style, integer_names[index]))
			continue;
		node = json_object_get_member(style, integer_names[index]);
		integer = JSON_NODE_HOLDS_VALUE(node) &&
			json_node_get_value_type(node) == G_TYPE_INT64 ?
			json_node_get_int(node) : G_MININT64;
		if (integer < (strcmp(integer_names[index], "wrap-width") == 0 ?
			-1 : strcmp(integer_names[index], "pixbuf-size") == 0 ? 1 : 0) ||
			integer > (strcmp(integer_names[index], "pixbuf-size") == 0 ?
			512 : G_MAXINT)) {
			g_set_error(error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON %s %s is outside its supported range", context,
				integer_names[index]);
			return FALSE;
		}
	}
	if ((json_object_has_member(style, "icon-name") ? 1 : 0) +
		(json_object_has_member(style, "stock-id") ? 1 : 0) +
		(json_object_has_member(style, "pixbuf-file") ? 1 : 0) > 1) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"tree JSON %s has more than one cell image source", context);
		return FALSE;
	}
	return TRUE;
}

static gboolean widget_tree_json_validate_rows(JsonArray *rows,
	GtkTreeModel *model, guint depth, GError **error)
{
	JsonArray *cells;
	JsonArray *children;
	JsonArray *values;
	JsonNode *cell;
	JsonNode *member;
	JsonNode *row_node;
	JsonObject *row;
	gchar context[96];
	GType column_type;
	guint index;
	guint ncolumns;
	guint value_index;

	ncolumns = gtk_tree_model_get_n_columns(model) - FirstDataColumn;

	if (depth > TREE_JSON_MAX_DEPTH) {
		g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"tree JSON exceeds the maximum nesting depth of %u",
			TREE_JSON_MAX_DEPTH);
		return FALSE;
	}

	for (index = 0; index < json_array_get_length(rows); ++index) {
		row_node = json_array_get_element(rows, index);
		if (!JSON_NODE_HOLDS_OBJECT(row_node)) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON row %u at depth %u is not an object", index, depth);
			return FALSE;
		}
		row = json_node_get_object(row_node);
		member = json_object_get_member(row, "values");
		if (!member || !JSON_NODE_HOLDS_ARRAY(member)) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON row %u at depth %u has no values array", index, depth);
			return FALSE;
		}
		values = json_node_get_array(member);
		if (json_array_get_length(values) > ncolumns) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON row %u at depth %u has more values than columns",
				index, depth);
			return FALSE;
		}
		for (value_index = 0; value_index < json_array_get_length(values);
			++value_index) {
			member = json_array_get_element(values, value_index);
			if (!widget_tree_json_value_is_scalar(member)) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"tree JSON value %u in row %u at depth %u is not scalar",
					value_index, index, depth);
				return FALSE;
			}
			column_type = gtk_tree_model_get_column_type(model,
				FirstDataColumn + value_index);
			if (!widget_tree_json_value_matches_column(member, column_type)) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"tree JSON value %u in row %u at depth %u is invalid for "
					"column-type %s", value_index, index, depth,
					widget_tree_json_column_type_name(column_type));
				return FALSE;
			}
		}

		member = json_object_get_member(row, "style");
		if (member != NULL) {
			if (!JSON_NODE_HOLDS_OBJECT(member)) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"tree JSON style in row %u at depth %u is not an object",
					index, depth);
				return FALSE;
			}
			g_snprintf(context, sizeof(context), "row %u at depth %u style",
				index, depth);
			if (!widget_tree_json_validate_style(json_node_get_object(member),
				context, error))
				return FALSE;
		}
		member = json_object_get_member(row, "cells");
		if (member != NULL) {
			if (!JSON_NODE_HOLDS_ARRAY(member)) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"tree JSON cells in row %u at depth %u is not an array",
					index, depth);
				return FALSE;
			}
			cells = json_node_get_array(member);
			if (json_array_get_length(cells) > ncolumns) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"tree JSON cells in row %u at depth %u exceed columns",
					index, depth);
				return FALSE;
			}
			for (value_index = 0;
				value_index < json_array_get_length(cells); ++value_index) {
				cell = json_array_get_element(cells, value_index);
				if (JSON_NODE_HOLDS_NULL(cell))
					continue;
				if (!JSON_NODE_HOLDS_OBJECT(cell)) {
					g_set_error(error, G_MARKUP_ERROR,
						G_MARKUP_ERROR_INVALID_CONTENT,
						"tree JSON cell %u in row %u at depth %u is not an object or null",
						value_index, index, depth);
					return FALSE;
				}
				g_snprintf(context, sizeof(context),
					"cell %u in row %u at depth %u", value_index, index,
					depth);
				if (!widget_tree_json_validate_style(
					json_node_get_object(cell), context, error))
					return FALSE;
			}
		}

		member = json_object_get_member(row, "icon-name");
		if (member && (!JSON_NODE_HOLDS_VALUE(member) ||
			json_node_get_value_type(member) != G_TYPE_STRING)) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON icon-name in row %u at depth %u is not a string",
				index, depth);
			return FALSE;
		}
		member = json_object_get_member(row, "stock-id");
		if (member && (!JSON_NODE_HOLDS_VALUE(member) ||
			json_node_get_value_type(member) != G_TYPE_STRING)) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON stock-id in row %u at depth %u is not a string",
				index, depth);
			return FALSE;
		}
		if (json_object_has_member(row, "icon-name") &&
			json_object_has_member(row, "stock-id")) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON row %u at depth %u has both icon-name and stock-id",
				index, depth);
			return FALSE;
		}
		member = json_object_get_member(row, "expanded");
		if (member && (!JSON_NODE_HOLDS_VALUE(member) ||
			json_node_get_value_type(member) != G_TYPE_BOOLEAN)) {
			g_set_error(error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON expanded in row %u at depth %u is not Boolean",
				index, depth);
			return FALSE;
		}
		member = json_object_get_member(row, "children");
		if (member) {
			if (!JSON_NODE_HOLDS_ARRAY(member)) {
				g_set_error(error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"tree JSON children in row %u at depth %u is not an array",
					index, depth);
				return FALSE;
			}
			children = json_node_get_array(member);
			if (!widget_tree_json_validate_rows(children, model, depth + 1,
				error))
				return FALSE;
		}
	}
	return TRUE;
}

static gchar *widget_tree_json_scalar_to_string(JsonNode *node,
	GType column_type)
{
	gchar buffer[G_ASCII_DTOSTR_BUF_SIZE];
	GType type;

	if (JSON_NODE_HOLDS_NULL(node))
		return g_strdup(column_type == G_TYPE_STRING ? "" : "0");
	type = json_node_get_value_type(node);
	if (type == G_TYPE_STRING)
		return g_strdup(json_node_get_string(node));
	if (type == G_TYPE_INT64)
		return g_strdup_printf("%" G_GINT64_FORMAT, json_node_get_int(node));
	if (type == G_TYPE_DOUBLE)
		return g_strdup(g_ascii_dtostr(buffer, sizeof(buffer),
			json_node_get_double(node)));
	if (type == G_TYPE_BOOLEAN)
		return g_strdup(json_node_get_boolean(node) ?
			(column_type == G_TYPE_STRING ? "true" : "1") :
			(column_type == G_TYPE_STRING ? "false" : "0"));
	return g_strdup("");
}

static void widget_tree_json_append_rows(variable *var, JsonArray *rows,
	GtkTreeIter *parent, const TreeInputContext *context,
	GList **expanded_rows)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkTreePath *path;
	GtkTreeRowReference *row_reference;
	JsonArray *children;
	JsonArray *values;
	JsonNode *member;
	JsonNode *metadata_node;
	JsonObject *row;
	JsonObject *metadata;
	const gchar *string;
	gchar *value;
	guint index;
	guint value_index;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget));
	for (index = 0; index < json_array_get_length(rows); ++index) {
		row = json_node_get_object(json_array_get_element(rows, index));
		gtk_tree_store_append(GTK_TREE_STORE(model), &iter, parent);
		metadata = json_object_new();
		member = json_object_get_member(row, "style");
		if (member != NULL)
			json_object_set_member(metadata, "style", json_node_copy(member));
		member = json_object_get_member(row, "cells");
		if (member != NULL)
			json_object_set_member(metadata, "cells", json_node_copy(member));
		if (json_object_get_size(metadata) > 0) {
			metadata_node = json_node_new(JSON_NODE_OBJECT);
			json_node_take_object(metadata_node, metadata);
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnJsonStyle, metadata_node, -1);
			json_node_free(metadata_node);
		} else {
			json_object_unref(metadata);
		}

		if (json_object_has_member(row, "stock-id")) {
			string = json_object_get_string_member(row, "stock-id");
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnStockId, string, -1);
		} else if (json_object_has_member(row, "icon-name")) {
			string = json_object_get_string_member(row, "icon-name");
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnIconName, string, -1);
		} else if (context->stock_id) {
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnStockId, context->stock_id, -1);
		} else if (context->icon_name) {
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnIconName, context->icon_name, -1);
		}

		values = json_object_get_array_member(row, "values");
		for (value_index = 0; value_index < json_array_get_length(values);
			++value_index) {
			value = widget_tree_json_scalar_to_string(
				json_array_get_element(values, value_index),
				gtk_tree_model_get_column_type(model,
					FirstDataColumn + value_index));
			widget_tree_set_column_value(GTK_TREE_STORE(model), &iter,
				FirstDataColumn + value_index, value);
			g_free(value);
		}

		if (json_object_has_member(row, "expanded") &&
			json_object_get_boolean_member(row, "expanded")) {
			path = gtk_tree_model_get_path(model, &iter);
			row_reference = gtk_tree_row_reference_new(model, path);
			gtk_tree_path_free(path);
			if (row_reference)
				*expanded_rows = g_list_prepend(*expanded_rows, row_reference);
		}
		member = json_object_get_member(row, "children");
		if (member) {
			children = json_node_get_array(member);
			widget_tree_json_append_rows(var, children, &iter, context,
				expanded_rows);
		}
	}
}

static gchar *widget_tree_json_read_stream(FILE *stream)
{
	GString *contents;
	gchar buffer[4096];
	size_t count;

	contents = g_string_new(NULL);
	while ((count = fread(buffer, 1, sizeof(buffer), stream)) > 0)
		g_string_append_len(contents, buffer, count);
	if (ferror(stream)) {
		g_string_free(contents, TRUE);
		return NULL;
	}
	return g_string_free(contents, FALSE);
}
#endif

static void widget_tree_input_json(variable *var, const gchar *source,
	gboolean is_command, const TreeInputContext *context)
{
#if HAVE_JSON_GLIB
	FILE *stream;
	GError *error = NULL;
	JsonArray *rows;
	JsonNode *member;
	JsonNode *root;
	JsonObject *document;
	JsonParser *parser;
	GtkTreePath *expanded_path;
	GList *expanded_rows = NULL;
	GList *row;
	gchar *contents;

	stream = is_command ? widget_opencommand((gchar *)source) : fopen(source, "r");
	if (!stream) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, source);
		return;
	}
	contents = widget_tree_json_read_stream(stream);
	if (is_command)
		widget_closecommand(stream, (gchar *)source);
	else
		fclose(stream);
	if (!contents) {
		gtkdialog_warning("Couldn't read tree JSON input '%s'.", source);
		return;
	}

	parser = json_parser_new();
	if (!json_parser_load_from_data(parser, contents, -1, &error)) {
		gtkdialog_warning("Couldn't parse tree JSON input '%s': %s",
			source, error->message);
		g_error_free(error);
		g_object_unref(parser);
		g_free(contents);
		return;
	}
	g_free(contents);

	root = json_parser_get_root(parser);
	if (!root || !JSON_NODE_HOLDS_OBJECT(root)) {
		g_set_error(&error, G_MARKUP_ERROR, G_MARKUP_ERROR_INVALID_CONTENT,
			"tree JSON document root is not an object");
	} else {
		document = json_node_get_object(root);
		member = json_object_get_member(document, "version");
		if (!member || !JSON_NODE_HOLDS_VALUE(member) ||
			json_node_get_value_type(member) != G_TYPE_INT64 ||
			json_node_get_int(member) != 1) {
			g_set_error(&error, G_MARKUP_ERROR,
				G_MARKUP_ERROR_INVALID_CONTENT,
				"tree JSON document version must be the integer 1");
		} else {
			member = json_object_get_member(document, "rows");
			if (!member || !JSON_NODE_HOLDS_ARRAY(member)) {
				g_set_error(&error, G_MARKUP_ERROR,
					G_MARKUP_ERROR_INVALID_CONTENT,
					"tree JSON document has no rows array");
			} else {
				rows = json_node_get_array(member);
				if (widget_tree_json_validate_rows(rows,
					gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget)), 1,
					&error)) {
					widget_tree_json_append_rows(var, rows, NULL, context,
						&expanded_rows);
					expanded_rows = g_list_reverse(expanded_rows);
					for (row = expanded_rows; row; row = row->next) {
						expanded_path = gtk_tree_row_reference_get_path(
							row->data);
						if (expanded_path) {
							gtk_tree_view_expand_to_path(
								GTK_TREE_VIEW(var->Widget), expanded_path);
							gtk_tree_path_free(expanded_path);
						}
						gtk_tree_row_reference_free(row->data);
					}
					g_list_free(expanded_rows);
				}
			}
		}
	}
	if (error) {
		gtkdialog_warning("Invalid tree JSON input '%s': %s",
			source, error->message);
		g_error_free(error);
	}
	g_object_unref(parser);
#else
	(void)var;
	(void)is_command;
	(void)context;
	gtkdialog_warning("Tree JSON input '%s' requires JSON-GLib support.", source);
#endif
}

#if HAVE_JSON_GLIB
static void widget_tree_json_build_rows(JsonBuilder *builder,
	GtkTreeView *tree_view, GtkTreeIter *parent)
{
	GValue style_value = { 0 };
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkTreePath *path;
	GType type;
	gboolean valid;
	gchar *string;
	gdouble double_value;
	gint column;
	gint columnmax;
	gint64 int64_value;
	JsonNode *member;
	JsonNode *metadata;
	guint64 uint64_value;

	model = gtk_tree_view_get_model(tree_view);
	valid = parent ? gtk_tree_model_iter_children(model, &iter, parent) :
		gtk_tree_model_get_iter_first(model, &iter);
	while (valid) {
		json_builder_begin_object(builder);

		gtk_tree_model_get(model, &iter, ColumnIconName, &string, -1);
		if (string && *string) {
			json_builder_set_member_name(builder, "icon-name");
			json_builder_add_string_value(builder, string);
		}
		g_free(string);
		gtk_tree_model_get(model, &iter, ColumnStockId, &string, -1);
		if (string && *string) {
			json_builder_set_member_name(builder, "stock-id");
			json_builder_add_string_value(builder, string);
		}
		g_free(string);
		gtk_tree_model_get_value(model, &iter, ColumnJsonStyle, &style_value);
		metadata = g_value_get_boxed(&style_value);
		if (metadata != NULL && JSON_NODE_HOLDS_OBJECT(metadata)) {
			member = json_object_get_member(json_node_get_object(metadata),
				"style");
			if (member != NULL) {
				json_builder_set_member_name(builder, "style");
				json_builder_add_value(builder, json_node_copy(member));
			}
			member = json_object_get_member(json_node_get_object(metadata),
				"cells");
			if (member != NULL) {
				json_builder_set_member_name(builder, "cells");
				json_builder_add_value(builder, json_node_copy(member));
			}
		}
		g_value_unset(&style_value);

		json_builder_set_member_name(builder, "values");
		json_builder_begin_array(builder);
		columnmax = gtk_tree_model_get_n_columns(model);
		for (column = FirstDataColumn; column < columnmax; ++column) {
			type = gtk_tree_model_get_column_type(model, column);
			switch (type) {
				case G_TYPE_STRING:
					gtk_tree_model_get(model, &iter, column, &string, -1);
					json_builder_add_string_value(builder, string ? string : "");
					g_free(string);
					break;
				case G_TYPE_INT64:
					gtk_tree_model_get(model, &iter, column, &int64_value, -1);
					json_builder_add_int_value(builder, int64_value);
					break;
				case G_TYPE_UINT64:
					gtk_tree_model_get(model, &iter, column, &uint64_value, -1);
					string = g_strdup_printf("%" G_GUINT64_FORMAT, uint64_value);
					json_builder_add_string_value(builder, string);
					g_free(string);
					break;
				case G_TYPE_DOUBLE:
					gtk_tree_model_get(model, &iter, column, &double_value, -1);
					json_builder_add_double_value(builder, double_value);
					break;
				default:
					json_builder_add_string_value(builder, "");
			}
		}
		json_builder_end_array(builder);

		path = gtk_tree_model_get_path(model, &iter);
		json_builder_set_member_name(builder, "expanded");
		json_builder_add_boolean_value(builder,
			gtk_tree_view_row_expanded(tree_view, path));
		gtk_tree_path_free(path);

		if (gtk_tree_model_iter_has_child(model, &iter)) {
			json_builder_set_member_name(builder, "children");
			json_builder_begin_array(builder);
			widget_tree_json_build_rows(builder, tree_view, &iter);
			json_builder_end_array(builder);
		}
		json_builder_end_object(builder);
		valid = gtk_tree_model_iter_next(model, &iter);
	}
}
#endif

static void widget_tree_save_json(variable *var, const gchar *filename)
{
#if HAVE_JSON_GLIB
	FILE *outfile;
	JsonBuilder *builder;
	JsonGenerator *generator;
	JsonNode *root;
	gchar *data;

	outfile = fopen(filename, "w");
	if (!outfile) {
		fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
			__func__, filename);
		return;
	}
	builder = json_builder_new();
	json_builder_begin_object(builder);
	json_builder_set_member_name(builder, "version");
	json_builder_add_int_value(builder, 1);
	json_builder_set_member_name(builder, "rows");
	json_builder_begin_array(builder);
	widget_tree_json_build_rows(builder, GTK_TREE_VIEW(var->Widget), NULL);
	json_builder_end_array(builder);
	json_builder_end_object(builder);

	root = json_builder_get_root(builder);
	generator = json_generator_new();
	json_generator_set_root(generator, root);
	json_generator_set_pretty(generator, TRUE);
	data = json_generator_to_data(generator, NULL);
	fputs(data, outfile);
	fputc('\n', outfile);
	widget_close_output(outfile, filename);

	g_free(data);
	g_object_unref(generator);
	json_node_free(root);
	g_object_unref(builder);
#else
	(void)var;
	gtkdialog_warning("Tree JSON output '%s' requires JSON-GLib support.",
		filename);
#endif
}

/***********************************************************************
 * Input by Command                                                    *
 ***********************************************************************/
static void widget_tree_input_by_command(variable *var, char *filename,
	gint command_or_file, const TreeInputContext *context)
{
	FILE             *infile;
	GtkTreeIter       iter;
	GtkTreeModel     *model;
	gchar           **columns;
	gchar            *oneline;
	gint              hiddencolumns;
	gint              n, ncolumns;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (command_or_file) {
		infile = widget_opencommand(filename);
	} else {
		infile = fopen(filename, "r");
	}

	if (infile) {

		model = gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget));

		ncolumns = gtk_tree_model_get_n_columns(model) - FirstDataColumn;

		while ((oneline = widget_read_line(infile)) != NULL) {
			hiddencolumns = 0;

			columns = g_strsplit(oneline, "|", -1);

			gtk_tree_store_append(GTK_TREE_STORE(model), &iter, NULL);

			if (context->stock_id) {
				/* If a default stock-id was declared then set it here */
				gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
					ColumnStockId, context->stock_id, -1);
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s(): <tree stock-id='%s'>\n", __func__,
					context->stock_id);
#endif
			} else if (context->icon_name) {
				/* If a default icon-name was declared then set it here */
				gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
					ColumnIconName, context->icon_name, -1);
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s(): <tree icon-name='%s'>\n", __func__,
					context->icon_name);
#endif
			}

			for (n = 0; columns[n] != NULL; ++n) {

				if (n - hiddencolumns >= ncolumns) break;
				if (n == context->stock_column) {
					g_strstrip(columns[n]);	/* Required */
					gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
						ColumnStockId, columns[n], -1);
					gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
						ColumnIconName, NULL, -1);
#ifdef DEBUG_CONTENT
					fprintf(stderr, "%s(): <input stock-column=%i>\n",
						__func__, context->stock_column);
#endif
					++hiddencolumns;
				} else if (n == context->icon_column) {
					g_strstrip(columns[n]);	/* Required */
					gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
						ColumnStockId, NULL, -1);
					gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
						ColumnIconName, columns[n], -1);
#ifdef DEBUG_CONTENT
					fprintf(stderr, "%s(): <input icon-column=%i>\n",
						__func__, context->icon_column);
#endif
					++hiddencolumns;
				} else {
					widget_tree_set_column_value(GTK_TREE_STORE(model), &iter,
						n + FirstDataColumn - hiddencolumns, columns[n]);
				}
			}
			g_strfreev(columns);
			g_free(oneline);
		}

		if (command_or_file) {
			widget_closecommand(infile, filename);
		} else {
			fclose(infile);
		}

	} else {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, filename);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by File                                                       *
 ***********************************************************************/

static void widget_tree_input_by_file(variable *var, char *filename,
	const TreeInputContext *context)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_tree_input_by_command(var, filename, FALSE, context);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Items                                                      *
 ***********************************************************************/
static void widget_tree_input_by_items(variable *var,
	const TreeInputContext *context)
{
	GList            *element;
	GtkTreeIter       iter;
	GtkTreeModel     *model;
	gchar            *act;
	gchar           **columns;
	gchar            *value;
	gint              n, ncolumns;
	gint              combo_column;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(var->Widget));

	ncolumns = gtk_tree_model_get_n_columns(model) - FirstDataColumn;

	/* act points to the original string so we can't remove [CR]LFs
	 * although it's not going to be likely within <item>"..."</item> */
	act = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (act != NULL) {
		if (widget_tree_get_combo_item_column(&element, var->Attributes,
			ncolumns, &combo_column, TRUE)) {
			act = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
			continue;
		}
		gtk_tree_store_append(GTK_TREE_STORE(model), &iter, NULL);

		if ((value = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_ITEM, "stock")) ||
			(value = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_ITEM, "stock-id")) ||
			(value = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_ITEM, "stock_id"))) {
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnStockId, value, -1);
#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s(): <item stock-id='%s'>\n", __func__, value);
#endif
		} else if ((value = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_ITEM, "icon")) ||
			(value = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_ITEM, "icon-name")) ||
			(value = attributeset_get_this_tagattr(&element, var->Attributes,
			ATTR_ITEM, "icon_name"))) {
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnIconName, value, -1);
#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s(): <item icon-name='%s'>\n", __func__, value);
#endif
		} else if (context->stock_id) {
			/* If a default stock-id was declared then set it here */
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnStockId, context->stock_id, -1);
#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s(): <tree stock-id='%s'>\n", __func__, context->stock_id);
#endif
		} else if (context->icon_name) {
			/* If a default icon-name was declared then set it here */
			gtk_tree_store_set(GTK_TREE_STORE(model), &iter,
				ColumnIconName, context->icon_name, -1);
#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s(): <tree icon-name='%s'>\n", __func__, context->icon_name);
#endif
		}

		columns = g_strsplit(act, "|", -1);
		for (n = 0; columns[n] != NULL; ++n) {
			if (n >= ncolumns)
				break;
			widget_tree_set_column_value(GTK_TREE_STORE(model), &iter,
				n + FirstDataColumn, columns[n]);
		}
		g_strfreev(columns);

		act = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Pixmap Column Cell Layout Function                                  *
 ***********************************************************************/
/* This is a callback */

static void widget_tree_pixmap_column_cell_layout_function(
	GtkCellLayout *cell_layout, GtkCellRenderer *cell,
	GtkTreeModel *tree_model, GtkTreeIter *iter, GtkTreeView *treeview)
{
	gchar            *icon_name;
	gchar            *stock_id;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	gtk_tree_model_get(tree_model, iter, ColumnStockId, &stock_id,
		ColumnIconName, &icon_name, -1);

	if (stock_id != NULL) {
#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): This row has a stock_id: '%s'\n",
			__func__, stock_id);
#endif
		g_object_set(G_OBJECT(cell), "stock-id", stock_id, NULL);
		g_object_set(G_OBJECT(cell), "icon-name", NULL, NULL);
	} else if (icon_name != NULL) {
#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): This row has an icon_name: '%s'\n",
			__func__, icon_name);
#endif
		g_object_set(G_OBJECT(cell), "stock-id", NULL, NULL);
		g_object_set(G_OBJECT(cell), "icon-name", icon_name, NULL);
	} else {
#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): This row has no icon set.\n", __func__);
#endif
		g_object_set(G_OBJECT(cell), "stock-id", NULL, NULL);
		g_object_set(G_OBJECT(cell), "icon-name", NULL, NULL);
	}
	g_free(stock_id);
	g_free(icon_name);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Changed Callback                                                    *
 ***********************************************************************/

gboolean widget_tree_changed_callback(GtkTreeSelection *treeselection,
	variable *var)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): treeselection=%p var->Widget=%p\n", __func__,
		treeselection, var->Widget);
#endif

	/* Pass the correct var->Widget which will be the GtkTreeView */
	on_any_widget_changed_event(var->Widget, var->Attributes);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return TRUE;
}

/***********************************************************************
 * Natural Compare                                                     *
 ***********************************************************************/

gint widget_tree_natcmp(GtkTreeModel *model, GtkTreeIter *a,
	GtkTreeIter *b, gpointer user_data)
{
	return _widget_tree_natcmp(model, a, b, user_data, TRUE);
}

gint widget_tree_natcasecmp(GtkTreeModel *model, GtkTreeIter *a,
	GtkTreeIter *b, gpointer user_data)
{
	return _widget_tree_natcmp(model, a, b, user_data, FALSE);
}

static gint _widget_tree_natcmp(GtkTreeModel *model, GtkTreeIter *a,
	GtkTreeIter *b, gpointer user_data, gint sensitive)
{
	gchar            *r1 = NULL;
	gchar            *r2 = NULL;
	gint              retval;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	gtk_tree_model_get(model, a, GPOINTER_TO_INT(user_data), &r1, -1);
	gtk_tree_model_get(model, b, GPOINTER_TO_INT(user_data), &r2, -1);

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): r1=\"%s\" r2=\"%s\"\n", __func__, r1, r2);
#endif

	retval = strnatcmp(r1, r2, sensitive);
	g_free(r1);
	g_free(r2);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return retval;
}

#endif
