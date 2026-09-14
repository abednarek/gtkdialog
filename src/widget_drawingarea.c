/*
 * widget_drawingarea.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026 Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
#include "widget_drawingarea.h"

typedef enum {
	DRAWINGAREA_SCALE_NONE,
	DRAWINGAREA_SCALE_FIT,
	DRAWINGAREA_SCALE_STRETCH
} DrawingAreaScaleMode;

typedef struct {
	GdkPixbuf *pixbuf;
	GdkPixbuf *scaled;
	gchar *source;
	gint scaled_width;
	gint scaled_height;
	gint requested_width;
	gint requested_height;
	DrawingAreaScaleMode scale_mode;
	GdkInterpType interpolation;
} DrawingAreaData;

static DrawingAreaData *widget_drawingarea_get_data(GtkWidget *widget)
{
	return g_object_get_data(G_OBJECT(widget), "gtkdialog-drawingarea-data");
}

static void widget_drawingarea_data_free(gpointer data)
{
	DrawingAreaData *drawing_data = data;

	if (drawing_data->pixbuf != NULL)
		g_object_unref(drawing_data->pixbuf);
	if (drawing_data->scaled != NULL)
		g_object_unref(drawing_data->scaled);
	g_free(drawing_data->source);
	g_free(drawing_data);
}

static void widget_drawingarea_discard_scaled(DrawingAreaData *drawing_data)
{
	if (drawing_data->scaled != NULL) {
		g_object_unref(drawing_data->scaled);
		drawing_data->scaled = NULL;
	}
	drawing_data->scaled_width = 0;
	drawing_data->scaled_height = 0;
}

static gboolean widget_drawingarea_load(GtkWidget *widget,
	const gchar *filename)
{
	DrawingAreaData *drawing_data;
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	gchar *resolved_filename;
	gint height;
	gint width;

	if (filename == NULL || filename[0] == '\0')
		return FALSE;

	resolved_filename = find_pixmap(filename);
	pixbuf = gdk_pixbuf_new_from_file(resolved_filename, &error);
	if (pixbuf == NULL) {
		widget_report_icon_error(resolved_filename, error);
		g_free(resolved_filename);
		return FALSE;
	}
	g_free(resolved_filename);

	drawing_data = widget_drawingarea_get_data(widget);
	if (drawing_data->pixbuf != NULL)
		g_object_unref(drawing_data->pixbuf);
	drawing_data->pixbuf = pixbuf;
	widget_drawingarea_discard_scaled(drawing_data);
	g_free(drawing_data->source);
	drawing_data->source = g_strdup(filename);

	width = drawing_data->requested_width;
	height = drawing_data->requested_height;
	if (width == -1)
		width = gdk_pixbuf_get_width(pixbuf);
	if (height == -1)
		height = gdk_pixbuf_get_height(pixbuf);
	gtk_widget_set_size_request(widget, width, height);
	gtk_widget_queue_draw(widget);
	return TRUE;
}

static void widget_drawingarea_input_by_command(variable *var,
	const gchar *command)
{
	FILE *input;
	gchar *filename;

	input = widget_opencommand(command);
	if (input == NULL) {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
			__func__, command);
		return;
	}
	filename = widget_read_line(input);
	if (filename != NULL) {
		widget_drawingarea_load(var->Widget, filename);
		g_free(filename);
	}
	widget_closecommand(input, command);
}

static void widget_drawingarea_input_by_file(variable *var,
	const gchar *filename)
{
	widget_drawingarea_load(var->Widget, filename);
}

static void widget_drawingarea_parse_scale_mode(DrawingAreaData *drawing_data,
	tag_attr *attr)
{
	gchar *value;

	if (attr == NULL)
		return;
	value = get_tag_attribute(attr, "scale-mode");
	if (value == NULL)
		return;
	if (strcasecmp(value, "none") == 0)
		drawing_data->scale_mode = DRAWINGAREA_SCALE_NONE;
	else if (strcasecmp(value, "fit") == 0)
		drawing_data->scale_mode = DRAWINGAREA_SCALE_FIT;
	else if (strcasecmp(value, "stretch") == 0)
		drawing_data->scale_mode = DRAWINGAREA_SCALE_STRETCH;
	else
		gtkdialog_warning("Invalid drawing area scale-mode '%s'; using none.",
			value);
	kill_tag_attribute(attr, "scale-mode");
}

static void widget_drawingarea_parse_interpolation(
	DrawingAreaData *drawing_data, tag_attr *attr)
{
	gchar *value;

	if (attr == NULL)
		return;
	value = get_tag_attribute(attr, "interpolation");
	if (value == NULL)
		return;
	if (strcasecmp(value, "nearest") == 0)
		drawing_data->interpolation = GDK_INTERP_NEAREST;
	else if (strcasecmp(value, "tiles") == 0)
		drawing_data->interpolation = GDK_INTERP_TILES;
	else if (strcasecmp(value, "bilinear") == 0)
		drawing_data->interpolation = GDK_INTERP_BILINEAR;
	else if (strcasecmp(value, "hyper") == 0)
		drawing_data->interpolation = GDK_INTERP_HYPER;
	else
		gtkdialog_warning("Invalid drawing area interpolation '%s'; using bilinear.",
			value);
	kill_tag_attribute(attr, "interpolation");
}

static void widget_drawingarea_parse_background(GtkWidget *widget,
	tag_attr *attr)
{
	GdkColor color;
	gchar *value;

	if (attr == NULL)
		return;
	value = get_tag_attribute(attr, "background");
	if (value == NULL)
		return;
	if (gdk_color_parse(value, &color))
		gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
	else
		gtkdialog_warning("Invalid drawing area background '%s'; using theme colour.",
			value);
	kill_tag_attribute(attr, "background");
}

static GdkPixbuf *widget_drawingarea_scaled_pixbuf(
	DrawingAreaData *drawing_data, gint width, gint height)
{
	if (width == gdk_pixbuf_get_width(drawing_data->pixbuf) &&
		height == gdk_pixbuf_get_height(drawing_data->pixbuf))
		return drawing_data->pixbuf;
	if (drawing_data->scaled == NULL ||
		drawing_data->scaled_width != width ||
		drawing_data->scaled_height != height) {
		widget_drawingarea_discard_scaled(drawing_data);
		drawing_data->scaled = gdk_pixbuf_scale_simple(drawing_data->pixbuf,
			width, height, drawing_data->interpolation);
		drawing_data->scaled_width = width;
		drawing_data->scaled_height = height;
	}
	return drawing_data->scaled;
}

static gboolean widget_drawingarea_exposed(GtkWidget *widget,
	GdkEventExpose *event, AttributeSet *attributes)
{
	DrawingAreaData *drawing_data;
	GdkPixbuf *display;
	gdouble ratio;
	gint height;
	gint width;
	gint x = 0;
	gint y = 0;

	(void)event;
	drawing_data = widget_drawingarea_get_data(widget);
	if (drawing_data->pixbuf != NULL) {
		width = gdk_pixbuf_get_width(drawing_data->pixbuf);
		height = gdk_pixbuf_get_height(drawing_data->pixbuf);
		if (drawing_data->scale_mode == DRAWINGAREA_SCALE_STRETCH) {
			width = MAX(1, widget->allocation.width);
			height = MAX(1, widget->allocation.height);
		} else if (drawing_data->scale_mode == DRAWINGAREA_SCALE_FIT) {
			ratio = MIN((gdouble)widget->allocation.width / width,
				(gdouble)widget->allocation.height / height);
			width = MAX(1, (gint)(width * ratio + 0.5));
			height = MAX(1, (gint)(height * ratio + 0.5));
			x = (widget->allocation.width - width) / 2;
			y = (widget->allocation.height - height) / 2;
		}
		display = widget_drawingarea_scaled_pixbuf(drawing_data,
			width, height);
		if (display != NULL)
			gdk_draw_pixbuf(widget->window,
				widget->style->fg_gc[GTK_STATE_NORMAL], display,
				0, 0, x, y, width, height, GDK_RGB_DITHER_NORMAL, 0, 0);
	}
	widget_signal_executor(widget, attributes, "expose-event");
	return FALSE;
}

void widget_drawingarea_clear(variable *var)
{
	DrawingAreaData *drawing_data;

	drawing_data = widget_drawingarea_get_data(var->Widget);
	if (drawing_data->pixbuf != NULL) {
		g_object_unref(drawing_data->pixbuf);
		drawing_data->pixbuf = NULL;
	}
	widget_drawingarea_discard_scaled(drawing_data);
	g_free(drawing_data->source);
	drawing_data->source = NULL;
	gtk_widget_queue_draw(var->Widget);
}

GtkWidget *widget_drawingarea_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	DrawingAreaData *drawing_data;
	GList *element;
	GtkWidget *widget;
	gint height = -1;
	gint width = -1;

	(void)Type;
	if (attributeset_is_avail(Attr, ATTR_WIDTH))
		width = widget_parse_size(attributeset_get_first(&element, Attr,
			ATTR_WIDTH), "drawing area width");
	if (attributeset_is_avail(Attr, ATTR_HEIGHT))
		height = widget_parse_size(attributeset_get_first(&element, Attr,
			ATTR_HEIGHT), "drawing area height");

	widget = gtk_drawing_area_new();
	drawing_data = g_new0(DrawingAreaData, 1);
	drawing_data->requested_width = width;
	drawing_data->requested_height = height;
	drawing_data->interpolation = GDK_INTERP_BILINEAR;
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-drawingarea-data",
		drawing_data, widget_drawingarea_data_free);
	widget_drawingarea_parse_scale_mode(drawing_data, attr);
	widget_drawingarea_parse_interpolation(drawing_data, attr);
	widget_drawingarea_parse_background(widget, attr);

	if (width == -1)
		width = 320;
	if (height == -1)
		height = 180;
	gtk_widget_set_size_request(widget, width, height);
	gtk_widget_add_events(widget, GDK_BUTTON_PRESS_MASK |
		GDK_BUTTON_RELEASE_MASK | GDK_ENTER_NOTIFY_MASK |
		GDK_LEAVE_NOTIFY_MASK);
	return widget;
}

gchar *widget_drawingarea_envvar_construct(GtkWidget *widget)
{
	DrawingAreaData *drawing_data;

	drawing_data = widget_drawingarea_get_data(widget);
	return g_strdup(drawing_data->source != NULL ? drawing_data->source : "");
}

void widget_drawingarea_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_drawingarea_load(var->Widget, value);
}

void widget_drawingarea_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_drawingarea_load(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));

	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_drawingarea_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_drawingarea_input_by_file(var, input + 5);
		}
		input = attributeset_get_next(&element,
			var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;

	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
			__func__);
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		fprintf(stderr, "%s(): <item> not implemented for this widget.\n",
			__func__);
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(G_OBJECT(var->Widget), "expose-event",
		G_CALLBACK(widget_drawingarea_exposed), var->Attributes);
}

void widget_drawingarea_save(variable *var)
{
	DrawingAreaData *drawing_data;
	GError *error = NULL;
	GList *element;
	gchar *directive;
	gchar *extension;
	gchar *filename = NULL;
	gchar *format;
	const gchar *save_format;

	directive = attributeset_get_first(&element,
		var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (strncasecmp(directive, "file:", 5) == 0 &&
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
	drawing_data = widget_drawingarea_get_data(var->Widget);
	if (drawing_data->pixbuf == NULL) {
		fprintf(stderr, "%s(): No image to save.\n", __func__);
		return;
	}
	extension = strrchr(filename, '.');
	if (extension == NULL || extension[1] == '\0') {
		gtkdialog_warning("Couldn't determine image format for '%s'.", filename);
		return;
	}
	format = g_ascii_strdown(extension + 1, -1);
	if (strcasecmp(format, "jpg") == 0)
		save_format = "jpeg";
	else
		save_format = format;
	if (!gdk_pixbuf_save(drawing_data->pixbuf, filename, save_format, &error,
		NULL)) {
		gtkdialog_warning("Couldn't save image '%s': %s", filename,
			error != NULL ? error->message : "unknown error");
		if (error != NULL)
			g_error_free(error);
	}
	g_free(format);
}
