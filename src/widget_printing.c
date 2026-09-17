/*
 * widget_printing.c: shared GTK2 printing helpers.
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

#include "gtkdialog.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_printing.h"

static GtkPageOrientation widget_printing_orientation(const gchar *value,
	gboolean *valid)
{
	*valid = TRUE;
	if (g_ascii_strcasecmp(value, "portrait") == 0)
		return GTK_PAGE_ORIENTATION_PORTRAIT;
	if (g_ascii_strcasecmp(value, "landscape") == 0)
		return GTK_PAGE_ORIENTATION_LANDSCAPE;
	if (g_ascii_strcasecmp(value, "reverse-portrait") == 0)
		return GTK_PAGE_ORIENTATION_REVERSE_PORTRAIT;
	if (g_ascii_strcasecmp(value, "reverse-landscape") == 0)
		return GTK_PAGE_ORIENTATION_REVERSE_LANDSCAPE;
	*valid = FALSE;
	return GTK_PAGE_ORIENTATION_PORTRAIT;
}

static gboolean widget_printing_get_nonnegative(tag_attr *attributes,
	const gchar *name, gdouble *result)
{
	gchar *value;

	value = get_tag_attribute(attributes, name);
	if (value == NULL)
		return FALSE;
	if (!widget_parse_finite_double(value, result, name))
		return FALSE;
	if (*result < 0.0) {
		gtkdialog_warning("Invalid %s value '%s'; ignored.", name, value);
		return FALSE;
	}
	return TRUE;
}

void widget_printing_apply_page_attributes(GtkPageSetup *setup,
	tag_attr *attributes)
{
	GtkPageOrientation orientation;
	GtkPaperSize *paper;
	gchar *height_text;
	gchar *name;
	gchar *orientation_text;
	gchar *width_text;
	gboolean height_valid;
	gboolean orientation_valid;
	gboolean width_valid;
	gdouble height = 0.0;
	gdouble margin;
	gdouble width = 0.0;

	if (attributes == NULL)
		return;
	name = get_tag_attribute(attributes, "paper-name");
	width_text = get_tag_attribute(attributes, "paper-width");
	height_text = get_tag_attribute(attributes, "paper-height");
	width_valid = width_text != NULL &&
		widget_parse_finite_double(width_text, &width, "paper-width") &&
		width > 0.0;
	height_valid = height_text != NULL &&
		widget_parse_finite_double(height_text, &height, "paper-height") &&
		height > 0.0;
	if ((width_text != NULL || height_text != NULL) &&
		!(width_valid && height_valid)) {
		gtkdialog_warning("Custom paper requires positive paper-width and "
			"paper-height values; using the named or default paper size.");
	}
	if (width_valid && height_valid) {
		const gchar *custom_name = name != NULL && name[0] != '\0'
			? name : "gtkdialog-custom";

		paper = gtk_paper_size_new_custom(custom_name, custom_name,
			width, height, GTK_UNIT_MM);
		gtk_page_setup_set_paper_size(setup, paper);
		gtk_paper_size_free(paper);
	} else if (name != NULL && name[0] != '\0') {
		paper = gtk_paper_size_new(name);
		gtk_page_setup_set_paper_size_and_default_margins(setup, paper);
		gtk_paper_size_free(paper);
	}

	orientation_text = get_tag_attribute(attributes, "orientation");
	if (orientation_text != NULL) {
		orientation = widget_printing_orientation(orientation_text,
			&orientation_valid);
		if (orientation_valid)
			gtk_page_setup_set_orientation(setup, orientation);
		else
			gtkdialog_warning("Invalid page orientation '%s'; ignored.",
				orientation_text);
	}
	if (widget_printing_get_nonnegative(attributes, "top-margin", &margin))
		gtk_page_setup_set_top_margin(setup, margin, GTK_UNIT_MM);
	if (widget_printing_get_nonnegative(attributes, "bottom-margin", &margin))
		gtk_page_setup_set_bottom_margin(setup, margin, GTK_UNIT_MM);
	if (widget_printing_get_nonnegative(attributes, "left-margin", &margin))
		gtk_page_setup_set_left_margin(setup, margin, GTK_UNIT_MM);
	if (widget_printing_get_nonnegative(attributes, "right-margin", &margin))
		gtk_page_setup_set_right_margin(setup, margin, GTK_UNIT_MM);

	kill_tag_attribute(attributes, "paper-name");
	kill_tag_attribute(attributes, "paper-width");
	kill_tag_attribute(attributes, "paper-height");
	kill_tag_attribute(attributes, "orientation");
	kill_tag_attribute(attributes, "top-margin");
	kill_tag_attribute(attributes, "bottom-margin");
	kill_tag_attribute(attributes, "left-margin");
	kill_tag_attribute(attributes, "right-margin");
}

GtkPageSetup *widget_printing_load_page_setup(const gchar *filename)
{
	GError *error = NULL;
	GtkPageSetup *setup;

	setup = gtk_page_setup_new_from_file(filename, &error);
	if (setup == NULL) {
		gtkdialog_warning("Couldn't load page setup '%s': %s", filename,
			error != NULL ? error->message : "unknown error");
		if (error != NULL)
			g_error_free(error);
	}
	return setup;
}

GtkPrintSettings *widget_printing_load_print_settings(const gchar *filename)
{
	GError *error = NULL;
	GtkPrintSettings *settings;

	settings = gtk_print_settings_new_from_file(filename, &error);
	if (settings == NULL) {
		gtkdialog_warning("Couldn't load print settings '%s': %s", filename,
			error != NULL ? error->message : "unknown error");
		if (error != NULL)
			g_error_free(error);
	}
	return settings;
}

static void widget_printing_count_setting(const gchar *key,
	const gchar *value, gpointer data)
{
	guint *count = data;

	(void)key;
	(void)value;
	++*count;
}

gboolean widget_printing_save_print_settings(GtkPrintSettings *settings,
	const gchar *filename, GError **error)
{
	gboolean empty;
	gboolean saved;
	guint count = 0;

	gtk_print_settings_foreach(settings, widget_printing_count_setting, &count);
	empty = count == 0;
	if (empty)
		gtk_print_settings_set(settings, "gtkdialog-empty-settings", "true");
	saved = gtk_print_settings_to_file(settings, filename, error);
	if (empty)
		gtk_print_settings_unset(settings, "gtkdialog-empty-settings");
	return saved;
}
