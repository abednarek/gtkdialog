/*
 * widget_filechooserdialog.c: GTK2 file chooser dialog widget.
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
#include "widget_dialog.h"
#include "widget_filechooserdialog.h"

#define FILECHOOSER_RESPONSE_DATA "gtkdialog-filechooserdialog-response"
#define FILECHOOSER_URI_DATA "gtkdialog-filechooserdialog-uri"
#define FILECHOOSER_PENDING_SELECTION_DATA \
	"gtkdialog-filechooserdialog-pending-selection"
#define FILECHOOSER_PENDING_SOURCE_DATA \
	"gtkdialog-filechooserdialog-pending-source"
#define FILECHOOSER_PENDING_RETRY_LIMIT 10

extern gboolean option_centering;
extern gboolean have_geometry_xy;
extern gboolean have_geometry_dxdy;
extern gint geometry_dx;
extern gint geometry_dy;
extern gint geometry_x;
extern gint geometry_y;

typedef struct {
	GtkWidget *widget;
	gchar *value;
	guint source_id;
	guint attempts;
} FileChooserPendingSelection;

static void widget_filechooserdialog_set_selection(GtkWidget *widget,
	const gchar *value);
static gboolean widget_filechooserdialog_set_selection_internal(GtkWidget *widget,
	const gchar *value, gboolean retrying);

static void widget_filechooserdialog_store_response(GtkWidget *widget,
	gint response)
{
	g_object_set_data_full(G_OBJECT(widget), FILECHOOSER_RESPONSE_DATA,
		widget_dialog_response_name(response), g_free);
}

static gboolean widget_filechooserdialog_set_response(GtkWidget *widget,
	const gchar *value, gboolean emit)
{
	gint response;

	if (!widget_dialog_parse_response(value, &response)) {
		gtkdialog_warning("Invalid file chooser dialog response '%s'; ignored.",
			value != NULL ? value : "");
		return FALSE;
	}
	if (emit)
		gtk_dialog_response(GTK_DIALOG(widget), response);
	else
		widget_filechooserdialog_store_response(widget, response);
	return TRUE;
}

static gboolean widget_filechooserdialog_uses_uri(GtkWidget *widget)
{
	return GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(widget), FILECHOOSER_URI_DATA));
}

static GtkFileChooserAction widget_filechooserdialog_parse_action(
	tag_attr *attributes)
{
	gchar *value;

	if (attributes == NULL)
		return GTK_FILE_CHOOSER_ACTION_OPEN;
	value = get_tag_attribute(attributes, "action");
	if (value == NULL || g_ascii_strcasecmp(value, "open") == 0)
		return GTK_FILE_CHOOSER_ACTION_OPEN;
	if (g_ascii_strcasecmp(value, "save") == 0)
		return GTK_FILE_CHOOSER_ACTION_SAVE;
	if (g_ascii_strcasecmp(value, "select-folder") == 0 ||
		g_ascii_strcasecmp(value, "folder") == 0)
		return GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
	if (g_ascii_strcasecmp(value, "create-folder") == 0 ||
		g_ascii_strcasecmp(value, "new-folder") == 0)
		return GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
	gtkdialog_warning("Invalid file chooser dialog action '%s'; using open.",
		value);
	return GTK_FILE_CHOOSER_ACTION_OPEN;
}

static void widget_filechooserdialog_add_filter(GtkWidget *widget,
	const gchar *name, const gchar *patterns, const gchar *mime_types)
{
	GtkFileFilter *filter;
	gboolean populated = FALSE;
	gchar **items;
	gint index;

	if (patterns == NULL && mime_types == NULL)
		return;
	filter = gtk_file_filter_new();
	if (name != NULL && name[0] != '\0')
		gtk_file_filter_set_name(filter, name);
	if (patterns != NULL) {
		items = g_strsplit_set(patterns, ";,", -1);
		for (index = 0; items[index] != NULL; ++index) {
			g_strstrip(items[index]);
			if (items[index][0] != '\0') {
				gtk_file_filter_add_pattern(filter, items[index]);
				populated = TRUE;
			}
		}
		g_strfreev(items);
	}
	if (mime_types != NULL) {
		items = g_strsplit_set(mime_types, ";,", -1);
		for (index = 0; items[index] != NULL; ++index) {
			g_strstrip(items[index]);
			if (items[index][0] != '\0') {
				gtk_file_filter_add_mime_type(filter, items[index]);
				populated = TRUE;
			}
		}
		g_strfreev(items);
	}
	if (populated)
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(widget), filter);
	else
		g_object_unref(filter);
}

static void widget_filechooserdialog_add_legacy_filters(GtkWidget *widget,
	const gchar *values, gboolean mime_types)
{
	GtkFileFilter *filter;
	gchar **items;
	gint index;

	if (values == NULL)
		return;
	items = g_strsplit(values, "|", -1);
	for (index = 0; items[index] != NULL; ++index) {
		g_strstrip(items[index]);
		if (items[index][0] == '\0')
			continue;
		filter = gtk_file_filter_new();
		gtk_file_filter_set_name(filter, items[index]);
		if (mime_types)
			gtk_file_filter_add_mime_type(filter, items[index]);
		else
			gtk_file_filter_add_pattern(filter, items[index]);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(widget), filter);
	}
	g_strfreev(items);
}

static void widget_filechooserdialog_add_shortcuts(GtkWidget *widget,
	const gchar *values, gboolean uri)
{
	GError *error;
	gchar **items;
	gint index;

	if (values == NULL)
		return;
	items = g_strsplit(values, "|", -1);
	for (index = 0; items[index] != NULL; ++index) {
		g_strstrip(items[index]);
		if (items[index][0] == '\0')
			continue;
		error = NULL;
		if (!(uri
			? gtk_file_chooser_add_shortcut_folder_uri(
				GTK_FILE_CHOOSER(widget), items[index], &error)
			: gtk_file_chooser_add_shortcut_folder(
				GTK_FILE_CHOOSER(widget), items[index], &error))) {
			gtkdialog_warning("Couldn't add file chooser shortcut '%s': %s",
				items[index], error != NULL
					? error->message : "unknown error");
		}
		if (error != NULL)
			g_error_free(error);
	}
	g_strfreev(items);
}

static gboolean widget_filechooserdialog_select_one(GtkWidget *widget,
	const gchar *value, gboolean select)
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(widget);
	gboolean uri = widget_filechooserdialog_uses_uri(widget);

	if (uri)
		return select ? gtk_file_chooser_select_uri(chooser, value)
			: gtk_file_chooser_set_uri(chooser, value);
	return select ? gtk_file_chooser_select_filename(chooser, value)
		: gtk_file_chooser_set_filename(chooser, value);
}

static gchar *widget_filechooserdialog_first_parent(const gchar *value,
	gboolean uri)
{
	gchar *item;
	gchar *line_end;
	gchar *parent;
	gchar *scheme_end;
	gchar *slash;
	gsize length;

	item = g_strdup(value);
	line_end = strchr(item, '\n');
	if (line_end != NULL)
		*line_end = '\0';
	length = strlen(item);
	if (length > 0 && item[length - 1] == '\r')
		item[length - 1] = '\0';
	if (!uri) {
		parent = g_path_get_dirname(item);
		g_free(item);
		return parent;
	}
	slash = strrchr(item, '/');
	if (slash == NULL) {
		g_free(item);
		return NULL;
	}
	scheme_end = strstr(item, "://");
	if (scheme_end != NULL && slash == scheme_end + 3)
		parent = g_strndup(item, slash - item + 1);
	else
		parent = g_strndup(item, slash - item);
	g_free(item);
	return parent;
}

static gboolean widget_filechooserdialog_same_parent(const gchar *value,
	const gchar *parent, gboolean uri)
{
	gchar *value_parent;
	gboolean same;

	value_parent = widget_filechooserdialog_first_parent(value, uri);
	same = value_parent != NULL && strcmp(value_parent, parent) == 0;
	g_free(value_parent);
	return same;
}

static void widget_filechooserdialog_free_files(GSList *files)
{
	GSList *item;

	for (item = files; item != NULL; item = item->next)
		g_free(item->data);
	g_slist_free(files);
}

static void widget_filechooserdialog_pending_widget_destroyed(
	gpointer data, GObject *object)
{
	FileChooserPendingSelection *pending = data;

	(void)object;
	pending->widget = NULL;
}

static void widget_filechooserdialog_pending_free(gpointer data)
{
	FileChooserPendingSelection *pending = data;

	if (pending->widget != NULL) {
		if (GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(pending->widget),
			FILECHOOSER_PENDING_SOURCE_DATA)) == pending->source_id)
			g_object_set_data(G_OBJECT(pending->widget),
				FILECHOOSER_PENDING_SOURCE_DATA, NULL);
		g_object_weak_unref(G_OBJECT(pending->widget),
			widget_filechooserdialog_pending_widget_destroyed, pending);
	}
	g_free(pending->value);
	g_free(pending);
}

static gboolean widget_filechooserdialog_apply_pending(gpointer data)
{
	FileChooserPendingSelection *pending = data;

	if (pending->widget == NULL)
		return FALSE;
	if (widget_filechooserdialog_set_selection_internal(pending->widget,
		pending->value, TRUE))
		return FALSE;
	pending->attempts++;
	if (pending->attempts >= FILECHOOSER_PENDING_RETRY_LIMIT) {
		gtkdialog_warning("File chooser dialog could not apply the requested "
			"multiple selection after %u attempts.", pending->attempts);
		return FALSE;
	}
	return TRUE;
}

static void widget_filechooserdialog_cancel_pending(GtkWidget *widget)
{
	guint source_id;

	source_id = GPOINTER_TO_UINT(g_object_get_data(G_OBJECT(widget),
		FILECHOOSER_PENDING_SOURCE_DATA));
	if (source_id != 0)
		g_source_remove(source_id);
	g_object_set_data(G_OBJECT(widget),
		FILECHOOSER_PENDING_SELECTION_DATA, NULL);
}

static void widget_filechooserdialog_schedule_pending(GtkWidget *widget)
{
	FileChooserPendingSelection *pending;
	const gchar *value;

	value = g_object_get_data(G_OBJECT(widget),
		FILECHOOSER_PENDING_SELECTION_DATA);
	if (value == NULL)
		return;
	pending = g_new(FileChooserPendingSelection, 1);
	pending->widget = widget;
	pending->value = g_strdup(value);
	pending->attempts = 0;
	pending->source_id = 0;
	widget_filechooserdialog_cancel_pending(widget);
	g_object_weak_ref(G_OBJECT(widget),
		widget_filechooserdialog_pending_widget_destroyed, pending);
	/* GTK2 can restore its file-list cursor after map.  Wait for that initial
	 * model update and retry if the requested folder or rows are not ready. */
	pending->source_id = g_timeout_add_full(G_PRIORITY_DEFAULT, 500,
		widget_filechooserdialog_apply_pending, pending,
		widget_filechooserdialog_pending_free);
	g_object_set_data(G_OBJECT(widget), FILECHOOSER_PENDING_SOURCE_DATA,
		GUINT_TO_POINTER(pending->source_id));
}

static void widget_filechooserdialog_map(GtkWidget *widget, gpointer data)
{
	(void)data;
	widget_filechooserdialog_schedule_pending(widget);
}

static gboolean widget_filechooserdialog_set_selection_internal(GtkWidget *widget,
	const gchar *value, gboolean retrying)
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(widget);
	GtkFileChooserAction action = gtk_file_chooser_get_action(chooser);
	gboolean multiple = gtk_file_chooser_get_select_multiple(chooser);
	gboolean uri = widget_filechooserdialog_uses_uri(widget);
	GHashTable *requested;
	GSList *files;
	GSList *file;
	gchar **items;
	gchar *current_folder;
	gchar *parent;
	gboolean complete;
	guint selected_count;
	gint index;

	if (!retrying)
		widget_filechooserdialog_cancel_pending(widget);
	gtk_file_chooser_unselect_all(chooser);
	if (value == NULL || value[0] == '\0')
		return TRUE;
	if (!multiple || (action != GTK_FILE_CHOOSER_ACTION_OPEN &&
		action != GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER)) {
		gchar *first = g_strdup(value);
		gchar *line_end = strchr(first, '\n');
		gboolean selected;
		gsize length;

		if (line_end != NULL)
			*line_end = '\0';
		length = strlen(first);
		if (length > 0 && first[length - 1] == '\r')
			first[length - 1] = '\0';
		selected = widget_filechooserdialog_select_one(widget, first, FALSE);
		if (!selected &&
			!uri && (action == GTK_FILE_CHOOSER_ACTION_SAVE ||
			action == GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER)) {
			gchar *directory = g_path_get_dirname(first);
			gchar *basename = g_path_get_basename(first);

			if (strcmp(directory, ".") == 0 ||
				gtk_file_chooser_set_current_folder(chooser, directory)) {
				gtk_file_chooser_set_current_name(chooser, basename);
				selected = TRUE;
			}
			g_free(directory);
			g_free(basename);
		}
		if (!selected)
			gtkdialog_warning("File chooser dialog could not select '%s'.",
				first);
		g_free(first);
		return TRUE;
	}
	if (!GTK_WIDGET_MAPPED(widget)) {
		gboolean changed;

		parent = widget_filechooserdialog_first_parent(value, uri);
		if (!retrying)
			g_object_set_data_full(G_OBJECT(widget),
				FILECHOOSER_PENDING_SELECTION_DATA, g_strdup(value), g_free);
		if (parent != NULL) {
			changed = uri
				? gtk_file_chooser_set_current_folder_uri(chooser, parent)
				: gtk_file_chooser_set_current_folder(chooser, parent);
			if (!changed)
				gtkdialog_warning("File chooser dialog could not open folder '%s'.",
					parent);
		}
		g_free(parent);
		return FALSE;
	}
	parent = widget_filechooserdialog_first_parent(value, uri);
	current_folder = uri
		? gtk_file_chooser_get_current_folder_uri(chooser)
		: gtk_file_chooser_get_current_folder(chooser);
	if (parent != NULL && (current_folder == NULL ||
		strcmp(parent, current_folder) != 0)) {
		gboolean changed;

		if (!retrying)
			g_object_set_data_full(G_OBJECT(widget),
				FILECHOOSER_PENDING_SELECTION_DATA, g_strdup(value), g_free);
		changed = uri
			? gtk_file_chooser_set_current_folder_uri(chooser, parent)
			: gtk_file_chooser_set_current_folder(chooser, parent);
		if (!changed) {
			gtkdialog_warning("File chooser dialog could not open folder '%s'.",
				parent);
			g_object_set_data(G_OBJECT(widget),
				FILECHOOSER_PENDING_SELECTION_DATA, NULL);
		} else if (!retrying)
			widget_filechooserdialog_schedule_pending(widget);
		g_free(current_folder);
		g_free(parent);
		return FALSE;
	}
	g_free(current_folder);
	if (parent == NULL) {
		gtkdialog_warning("File chooser dialog selection '%s' has no parent "
			"folder; ignored.", value);
		return TRUE;
	}
	requested = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
	items = g_strsplit(value, "\n", -1);
	for (index = 0; items[index] != NULL; ++index) {
		gsize length = strlen(items[index]);

		if (length > 0 && items[index][length - 1] == '\r')
			items[index][length - 1] = '\0';
		if (items[index][0] == '\0')
			continue;
		if (!widget_filechooserdialog_same_parent(items[index], parent, uri)) {
			gtkdialog_warning("File chooser dialog cannot select '%s' together "
				"with files from folder '%s'; ignored.", items[index], parent);
			continue;
		}
		g_hash_table_insert(requested, g_strdup(items[index]),
			GINT_TO_POINTER(TRUE));
	}
	g_strfreev(items);
	gtk_file_chooser_select_all(chooser);
	files = uri ? gtk_file_chooser_get_uris(chooser)
		: gtk_file_chooser_get_filenames(chooser);
	for (file = files; file != NULL; file = file->next) {
		if (!g_hash_table_lookup(requested, file->data)) {
			if (uri)
				gtk_file_chooser_unselect_uri(chooser, file->data);
			else
				gtk_file_chooser_unselect_filename(chooser, file->data);
		}
	}
	widget_filechooserdialog_free_files(files);
	files = uri ? gtk_file_chooser_get_uris(chooser)
		: gtk_file_chooser_get_filenames(chooser);
	selected_count = g_slist_length(files);
	widget_filechooserdialog_free_files(files);
	if (!retrying && selected_count != g_hash_table_size(requested)) {
		g_object_set_data_full(G_OBJECT(widget),
			FILECHOOSER_PENDING_SELECTION_DATA, g_strdup(value), g_free);
		widget_filechooserdialog_schedule_pending(widget);
	}
	complete = selected_count == g_hash_table_size(requested);
	g_hash_table_destroy(requested);
	g_free(parent);
	return complete;
}

static void widget_filechooserdialog_set_selection(GtkWidget *widget,
	const gchar *value)
{
	widget_filechooserdialog_set_selection_internal(widget, value, FALSE);
}

static gchar *widget_filechooserdialog_selection(GtkWidget *widget)
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(widget);
	GSList *files;
	GSList *item;
	GString *selection;
	gboolean uri = widget_filechooserdialog_uses_uri(widget);

	files = uri ? gtk_file_chooser_get_uris(chooser)
		: gtk_file_chooser_get_filenames(chooser);
	selection = g_string_new(NULL);
	for (item = files; item != NULL; item = item->next) {
		if (selection->len != 0)
			g_string_append_c(selection, '\n');
		g_string_append(selection, item->data);
		g_free(item->data);
	}
	g_slist_free(files);
	return g_string_free(selection, FALSE);
}

static gchar *widget_filechooserdialog_current_folder(GtkWidget *widget)
{
	gchar *folder;

	folder = widget_filechooserdialog_uses_uri(widget)
		? gtk_file_chooser_get_current_folder_uri(GTK_FILE_CHOOSER(widget))
		: gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER(widget));
	return folder != NULL ? folder : g_strdup("");
}

static void widget_filechooserdialog_response(GtkDialog *dialog,
	gint response, AttributeSet *attributes)
{
	widget_filechooserdialog_store_response(GTK_WIDGET(dialog), response);
	widget_signal_executor(GTK_WIDGET(dialog), attributes, "response");
}

static void widget_filechooserdialog_file_activated(GtkWidget *widget,
	AttributeSet *attributes)
{
	widget_signal_executor(widget, attributes, "file-activated");
}

static void widget_filechooserdialog_selection_changed(GtkWidget *widget,
	AttributeSet *attributes)
{
	widget_filechooserdialog_schedule_pending(widget);
	widget_signal_executor(widget, attributes, "selection-changed");
}

static void widget_filechooserdialog_update_preview(GtkWidget *widget,
	AttributeSet *attributes)
{
	widget_signal_executor(widget, attributes, "update-preview");
}

static void widget_filechooserdialog_current_folder_changed(GtkWidget *widget,
	AttributeSet *attributes)
{
	widget_filechooserdialog_schedule_pending(widget);
	widget_signal_executor(widget, attributes, "current-folder-changed");
}

GtkWidget *widget_filechooserdialog_create(AttributeSet *attributes,
	tag_attr *tag_attributes, gint type)
{
	GtkFileChooserAction action;
	GtkWidget *widget;
	gchar *accept_label;
	gchar *cancel_label;
	gchar *selection_format;
	gchar *title;
	gchar *value;
	gboolean boolean;

	(void)attributes;
	(void)type;
	action = widget_filechooserdialog_parse_action(tag_attributes);
	title = tag_attributes != NULL
		? get_tag_attribute(tag_attributes, "title") : NULL;
	accept_label = tag_attributes != NULL
		? get_tag_attribute(tag_attributes, "accept-label") : NULL;
	cancel_label = tag_attributes != NULL
		? get_tag_attribute(tag_attributes, "cancel-label") : NULL;
	if (accept_label == NULL)
		accept_label = action == GTK_FILE_CHOOSER_ACTION_SAVE
			? GTK_STOCK_SAVE : GTK_STOCK_OPEN;
	if (cancel_label == NULL)
		cancel_label = GTK_STOCK_CANCEL;
	widget = gtk_file_chooser_dialog_new(title != NULL ? title : PACKAGE,
		NULL, action, cancel_label, GTK_RESPONSE_CANCEL,
		accept_label, GTK_RESPONSE_ACCEPT, NULL);
	gtk_dialog_set_default_response(GTK_DIALOG(widget), GTK_RESPONSE_ACCEPT);
	g_object_set_data_full(G_OBJECT(widget), FILECHOOSER_RESPONSE_DATA,
		g_strdup(""), g_free);
	g_signal_connect(G_OBJECT(widget), "map",
		G_CALLBACK(widget_filechooserdialog_map), NULL);

	selection_format = tag_attributes != NULL
		? get_tag_attribute(tag_attributes, "selection-format") : NULL;
	if (selection_format != NULL &&
		g_ascii_strcasecmp(selection_format, "uri") == 0) {
		g_object_set_data(G_OBJECT(widget), FILECHOOSER_URI_DATA,
			GINT_TO_POINTER(TRUE));
	} else if (selection_format != NULL &&
		g_ascii_strcasecmp(selection_format, "filename") != 0) {
		gtkdialog_warning("Invalid file chooser selection-format '%s'; "
			"using filename.", selection_format);
	}

	if (tag_attributes != NULL &&
		(value = get_tag_attribute(tag_attributes, "select-multiple")) != NULL &&
		widget_parse_boolean(value, &boolean))
		gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(widget), boolean);
	if (tag_attributes != NULL &&
		(value = get_tag_attribute(tag_attributes, "local-only")) != NULL &&
		widget_parse_boolean(value, &boolean))
		gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(widget), boolean);
	if (tag_attributes != NULL &&
		(value = get_tag_attribute(tag_attributes, "current-folder")) != NULL &&
		value[0] != '\0' && !gtk_file_chooser_set_current_folder(
			GTK_FILE_CHOOSER(widget), value))
		gtkdialog_warning("File chooser dialog could not open folder '%s'.",
			value);
	if (tag_attributes != NULL &&
		(value = get_tag_attribute(tag_attributes,
			"current-folder-uri")) != NULL && value[0] != '\0' &&
		!gtk_file_chooser_set_current_folder_uri(
			GTK_FILE_CHOOSER(widget), value))
		gtkdialog_warning("File chooser dialog could not open folder URI '%s'.",
			value);
	if (tag_attributes != NULL &&
		(value = get_tag_attribute(tag_attributes, "current-name")) != NULL)
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(widget), value);

	if (tag_attributes != NULL) {
		widget_filechooserdialog_add_filter(widget,
			get_tag_attribute(tag_attributes, "filter-name"),
			get_tag_attribute(tag_attributes, "filter-pattern"),
			get_tag_attribute(tag_attributes, "filter-mime-type"));
		widget_filechooserdialog_add_legacy_filters(widget,
			get_tag_attribute(tag_attributes, "fs-filters"), FALSE);
		widget_filechooserdialog_add_legacy_filters(widget,
			get_tag_attribute(tag_attributes, "fs-filters-mime"), TRUE);
		widget_filechooserdialog_add_shortcuts(widget,
			get_tag_attribute(tag_attributes, "shortcut-folders"), FALSE);
		widget_filechooserdialog_add_shortcuts(widget,
			get_tag_attribute(tag_attributes, "shortcut-folder-uris"), TRUE);

		kill_tag_attribute(tag_attributes, "action");
		kill_tag_attribute(tag_attributes, "accept-label");
		kill_tag_attribute(tag_attributes, "cancel-label");
		kill_tag_attribute(tag_attributes, "selection-format");
		kill_tag_attribute(tag_attributes, "current-folder");
		kill_tag_attribute(tag_attributes, "current-folder-uri");
		kill_tag_attribute(tag_attributes, "current-name");
		kill_tag_attribute(tag_attributes, "filter-name");
		kill_tag_attribute(tag_attributes, "filter-pattern");
		kill_tag_attribute(tag_attributes, "filter-mime-type");
		kill_tag_attribute(tag_attributes, "fs-filters");
		kill_tag_attribute(tag_attributes, "fs-filters-mime");
		kill_tag_attribute(tag_attributes, "shortcut-folders");
		kill_tag_attribute(tag_attributes, "shortcut-folder-uris");
	}
	if (have_geometry_dxdy)
		gtk_widget_set_size_request(widget, geometry_dx, geometry_dy);
	if (have_geometry_xy)
		gtk_window_move(GTK_WINDOW(widget), geometry_x, geometry_y);
	if (option_centering)
		gtk_window_set_position(GTK_WINDOW(widget), GTK_WIN_POS_CENTER_ALWAYS);
	return widget;
}

gchar *widget_filechooserdialog_envvar_construct(GtkWidget *widget)
{
	const gchar *response;

	response = g_object_get_data(G_OBJECT(widget), FILECHOOSER_RESPONSE_DATA);
	return g_strdup(response != NULL ? response : "");
}

void widget_filechooserdialog_fileselect(variable *var, const char *name,
	const char *value)
{
	(void)name;
	widget_filechooserdialog_set_selection(var->Widget, value);
}

static void widget_filechooserdialog_input(variable *var,
	FILE *stream, gboolean response, gboolean emit)
{
	gchar *value;

	value = response ? widget_read_line(stream) : widget_read_all(stream);
	if (value == NULL)
		return;
	if (response)
		widget_filechooserdialog_set_response(var->Widget, value, emit);
	else
		widget_filechooserdialog_set_selection(var->Widget, value);
	g_free(value);
}

static void widget_filechooserdialog_input_by_command(variable *var,
	const gchar *command, gboolean response, gboolean emit)
{
	FILE *input;

	input = widget_opencommand(command);
	if (input == NULL) {
		gtkdialog_warning("Couldn't run file chooser input '%s'.", command);
		return;
	}
	widget_filechooserdialog_input(var, input, response, emit);
	widget_closecommand(input, command);
}

static void widget_filechooserdialog_input_by_file(variable *var,
	const gchar *filename, gboolean response, gboolean emit)
{
	FILE *input;

	input = fopen(filename, "r");
	if (input == NULL) {
		gtkdialog_warning("Couldn't open file chooser input '%s'.", filename);
		return;
	}
	widget_filechooserdialog_input(var, input, response, emit);
	fclose(input);
}

void widget_filechooserdialog_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;
	gboolean response;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		response = widget_attribute_is_true(
			attributeset_get_this_tagattr(&element, var->Attributes,
				ATTR_INPUT, "response"));
		if (input_is_shell_command(input))
			widget_filechooserdialog_input_by_command(var, input + 8,
				response, initialised);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_filechooserdialog_input_by_file(var, input + 5,
				response, initialised);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_filechooserdialog_set_selection(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));
	if (var->widget_tag_attr != NULL) {
		gchar *default_file = get_tag_attribute(
			var->widget_tag_attr, "default-file");

		if (default_file != NULL)
			widget_filechooserdialog_set_selection(var->Widget, default_file);
		kill_tag_attribute(var->widget_tag_attr, "default-file");
	}
	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		gtkdialog_warning("<label> is not used by filechooserdialog.");
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		gtkdialog_warning("<item> is not used by filechooserdialog.");
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(G_OBJECT(var->Widget), "response",
		G_CALLBACK(widget_filechooserdialog_response), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "file-activated",
		G_CALLBACK(widget_filechooserdialog_file_activated), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "selection-changed",
		G_CALLBACK(widget_filechooserdialog_selection_changed), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "update-preview",
		G_CALLBACK(widget_filechooserdialog_update_preview), var->Attributes);
	g_signal_connect(G_OBJECT(var->Widget), "current-folder-changed",
		G_CALLBACK(widget_filechooserdialog_current_folder_changed),
		var->Attributes);
}

void widget_filechooserdialog_save(variable *var)
{
	GError *error;
	GList *element;
	gchar *contents;
	gchar *current_folder;
	gchar *directive;
	gchar *response_output;
	gchar *folder_output;
	gboolean found = FALSE;

	directive = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (strncasecmp(directive, "file:", 5) == 0 && strlen(directive) > 5) {
			found = TRUE;
			response_output = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_OUTPUT, "response");
			folder_output = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_OUTPUT, "current-folder");
			if (widget_attribute_is_true(response_output) &&
				widget_attribute_is_true(folder_output)) {
				gtkdialog_warning("File chooser output '%s' cannot be both response "
					"and current folder; ignored.", directive + 5);
			} else {
				if (widget_attribute_is_true(response_output))
					contents = widget_filechooserdialog_envvar_construct(
						var->Widget);
				else if (widget_attribute_is_true(folder_output)) {
					current_folder = widget_filechooserdialog_current_folder(
						var->Widget);
					contents = current_folder;
				} else
					contents = widget_filechooserdialog_selection(var->Widget);
				error = NULL;
				if (!g_file_set_contents(directive + 5, contents, -1, &error))
					gtkdialog_warning("Couldn't save file chooser output '%s': %s",
						directive + 5, error != NULL
							? error->message : "unknown error");
				if (error != NULL)
					g_error_free(error);
				g_free(contents);
			}
		}
		directive = attributeset_get_next(&element,
			var->Attributes, ATTR_OUTPUT);
	}
	if (!found)
		gtkdialog_warning("No <output file> directive found for "
			"filechooserdialog.");
}

void widget_filechooserdialog_clear(variable *var)
{
	widget_filechooserdialog_cancel_pending(var->Widget);
	gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(var->Widget));
	if (gtk_file_chooser_get_action(GTK_FILE_CHOOSER(var->Widget)) ==
		GTK_FILE_CHOOSER_ACTION_SAVE ||
		gtk_file_chooser_get_action(GTK_FILE_CHOOSER(var->Widget)) ==
		GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER)
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(var->Widget), "");
	g_object_set_data_full(G_OBJECT(var->Widget), FILECHOOSER_RESPONSE_DATA,
		g_strdup(""), g_free);
}

void widget_filechooserdialog_removeselected(variable *var)
{
	widget_filechooserdialog_cancel_pending(var->Widget);
	gtk_file_chooser_unselect_all(GTK_FILE_CHOOSER(var->Widget));
}
