/*
 * widget_imageview.c:
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
#include "widget_imageview.h"

typedef enum {
	IMAGEVIEW_SCALE_MANUAL,
	IMAGEVIEW_SCALE_FIT,
	IMAGEVIEW_SCALE_FIT_WIDTH,
	IMAGEVIEW_SCALE_FIT_HEIGHT
} ImageViewScaleMode;

typedef struct {
	GdkPixbuf *pixbuf;
	gchar *source;
	gdouble zoom;
	gdouble zoom_step;
	gdouble displayed_zoom;
	gint displayed_width;
	gint displayed_height;
	gint requested_width;
	gint requested_height;
	guint pan_button;
	gboolean dragging;
	gboolean wheel_zoom;
	gdouble drag_x;
	gdouble drag_y;
	gdouble drag_hvalue;
	gdouble drag_vvalue;
	ImageViewScaleMode scale_mode;
	GdkInterpType interpolation;
} ImageViewData;

#define IMAGEVIEW_MIN_ZOOM 0.01
#define IMAGEVIEW_MAX_ZOOM 32.0

static ImageViewData *widget_imageview_get_data(GtkWidget *widget)
{
	return g_object_get_data(G_OBJECT(widget), "gtkdialog-imageview-data");
}

static void widget_imageview_data_free(gpointer data)
{
	ImageViewData *view_data = data;

	if (view_data->pixbuf != NULL)
		g_object_unref(view_data->pixbuf);
	g_free(view_data->source);
	g_free(view_data);
}

static GtkScrolledWindow *widget_imageview_get_scrolled_window(
	GtkWidget *widget)
{
	GtkWidget *parent = gtk_widget_get_parent(widget);

	while (parent != NULL && !GTK_IS_SCROLLED_WINDOW(parent))
		parent = gtk_widget_get_parent(parent);
	return parent != NULL ? GTK_SCROLLED_WINDOW(parent) : NULL;
}

static void widget_imageview_get_viewport_size(GtkWidget *widget,
	gint *width, gint *height)
{
	GtkWidget *parent = gtk_widget_get_parent(widget);

	if (parent != NULL && GTK_IS_VIEWPORT(parent)) {
		*width = MAX(1, parent->allocation.width);
		*height = MAX(1, parent->allocation.height);
	} else {
		*width = MAX(1, widget->allocation.width);
		*height = MAX(1, widget->allocation.height);
	}
}

static gint widget_imageview_scaled_dimension(gint dimension, gdouble zoom)
{
	gdouble scaled = dimension * zoom;

	if (scaled > G_MAXINT)
		return G_MAXINT;
	return MAX(1, (gint)(scaled + 0.5));
}

static void widget_imageview_update_geometry(GtkWidget *widget)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);
	gboolean changed;
	gdouble height_zoom;
	gdouble previous_zoom;
	gdouble width_zoom;
	gdouble zoom;
	gint height;
	gint image_height;
	gint image_width;
	gint previous_height;
	gint previous_width;
	gint request_height;
	gint request_width;
	gint width;

	previous_zoom = view_data->displayed_zoom;
	previous_width = view_data->displayed_width;
	previous_height = view_data->displayed_height;

	if (view_data->pixbuf == NULL) {
		view_data->displayed_width = 0;
		view_data->displayed_height = 0;
		view_data->displayed_zoom = view_data->zoom;
		request_width = 1;
		request_height = 1;
		goto apply_request;
	}

	image_width = gdk_pixbuf_get_width(view_data->pixbuf);
	image_height = gdk_pixbuf_get_height(view_data->pixbuf);
	widget_imageview_get_viewport_size(widget, &width, &height);
	width_zoom = (gdouble)width / image_width;
	height_zoom = (gdouble)height / image_height;

	switch (view_data->scale_mode) {
	case IMAGEVIEW_SCALE_FIT:
		zoom = MIN(width_zoom, height_zoom);
		break;
	case IMAGEVIEW_SCALE_FIT_WIDTH:
		zoom = width_zoom;
		break;
	case IMAGEVIEW_SCALE_FIT_HEIGHT:
		zoom = height_zoom;
		break;
	default:
		zoom = view_data->zoom;
		break;
	}
	zoom = CLAMP(zoom, IMAGEVIEW_MIN_ZOOM, IMAGEVIEW_MAX_ZOOM);
	view_data->displayed_zoom = zoom;
	view_data->displayed_width = widget_imageview_scaled_dimension(
		image_width, zoom);
	view_data->displayed_height = widget_imageview_scaled_dimension(
		image_height, zoom);

	switch (view_data->scale_mode) {
	case IMAGEVIEW_SCALE_FIT:
		request_width = 1;
		request_height = 1;
		break;
	case IMAGEVIEW_SCALE_FIT_WIDTH:
		request_width = 1;
		request_height = view_data->displayed_height;
		break;
	case IMAGEVIEW_SCALE_FIT_HEIGHT:
		request_width = view_data->displayed_width;
		request_height = 1;
		break;
	default:
		request_width = view_data->displayed_width;
		request_height = view_data->displayed_height;
		break;
	}

apply_request:
	changed = previous_zoom != view_data->displayed_zoom ||
		previous_width != view_data->displayed_width ||
		previous_height != view_data->displayed_height;
	if (request_width != view_data->requested_width ||
		request_height != view_data->requested_height) {
		view_data->requested_width = request_width;
		view_data->requested_height = request_height;
		gtk_widget_set_size_request(widget, request_width, request_height);
		changed = TRUE;
	}
	if (changed)
		gtk_widget_queue_draw(widget);
}

static void widget_imageview_emit_zoom_changed(GtkWidget *widget)
{
	variable *var = find_variable_by_widget(widget);

	if (var != NULL)
		widget_signal_executor(widget, var->Attributes, "zoom-changed");
}

static void widget_imageview_set_manual_zoom(GtkWidget *widget, gdouble zoom)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);

	view_data->scale_mode = IMAGEVIEW_SCALE_MANUAL;
	view_data->zoom = CLAMP(zoom, IMAGEVIEW_MIN_ZOOM, IMAGEVIEW_MAX_ZOOM);
	widget_imageview_update_geometry(widget);
	widget_imageview_emit_zoom_changed(widget);
}

static void widget_imageview_set_scale_mode(GtkWidget *widget,
	ImageViewScaleMode scale_mode)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);

	view_data->scale_mode = scale_mode;
	widget_imageview_update_geometry(widget);
	widget_imageview_emit_zoom_changed(widget);
}

static gboolean widget_imageview_load(GtkWidget *widget,
	const gchar *filename)
{
	ImageViewData *view_data;
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	gchar *resolved_filename;

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

	view_data = widget_imageview_get_data(widget);
	if (view_data->pixbuf != NULL)
		g_object_unref(view_data->pixbuf);
	view_data->pixbuf = pixbuf;
	g_free(view_data->source);
	view_data->source = g_strdup(filename);
	widget_imageview_update_geometry(widget);
	return TRUE;
}

static void widget_imageview_input_by_command(variable *var,
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
		widget_imageview_load(var->Widget, filename);
		g_free(filename);
	}
	widget_closecommand(input, command);
}

static void widget_imageview_parse_scale_mode(ImageViewData *view_data,
	tag_attr *attr)
{
	gchar *value;

	if (attr == NULL)
		return;
	value = get_tag_attribute(attr, "scale-mode");
	if (value == NULL)
		return;
	if (strcasecmp(value, "none") == 0 ||
		strcasecmp(value, "manual") == 0)
		view_data->scale_mode = IMAGEVIEW_SCALE_MANUAL;
	else if (strcasecmp(value, "fit") == 0)
		view_data->scale_mode = IMAGEVIEW_SCALE_FIT;
	else if (strcasecmp(value, "fit-width") == 0)
		view_data->scale_mode = IMAGEVIEW_SCALE_FIT_WIDTH;
	else if (strcasecmp(value, "fit-height") == 0)
		view_data->scale_mode = IMAGEVIEW_SCALE_FIT_HEIGHT;
	else
		gtkdialog_warning("Invalid image view scale-mode '%s'; using none.",
			value);
	kill_tag_attribute(attr, "scale-mode");
}

static void widget_imageview_parse_interpolation(ImageViewData *view_data,
	tag_attr *attr)
{
	gchar *value;

	if (attr == NULL)
		return;
	value = get_tag_attribute(attr, "interpolation");
	if (value == NULL)
		return;
	if (strcasecmp(value, "nearest") == 0)
		view_data->interpolation = GDK_INTERP_NEAREST;
	else if (strcasecmp(value, "tiles") == 0)
		view_data->interpolation = GDK_INTERP_TILES;
	else if (strcasecmp(value, "bilinear") == 0)
		view_data->interpolation = GDK_INTERP_BILINEAR;
	else if (strcasecmp(value, "hyper") == 0)
		view_data->interpolation = GDK_INTERP_HYPER;
	else
		gtkdialog_warning("Invalid image view interpolation '%s'; using bilinear.",
			value);
	kill_tag_attribute(attr, "interpolation");
}

static void widget_imageview_parse_double(gdouble *destination,
	tag_attr *attr, const gchar *name, gdouble minimum, gdouble maximum)
{
	gdouble parsed;
	gchar *value;

	if (attr == NULL)
		return;
	value = get_tag_attribute(attr, name);
	if (value == NULL)
		return;
	if (!widget_parse_finite_double(value, &parsed, name)) {
		kill_tag_attribute(attr, name);
		return;
	}
	if (parsed >= minimum && parsed <= maximum)
		*destination = parsed;
	else
		gtkdialog_warning("Invalid image view %s '%s'; using %.2f.",
			name, value, *destination);
	kill_tag_attribute(attr, name);
}

static void widget_imageview_parse_options(GtkWidget *widget,
	ImageViewData *view_data, tag_attr *attr)
{
	GdkColor color;
	gchar *value;
	gint button;
	gboolean boolean_value;

	widget_imageview_parse_scale_mode(view_data, attr);
	widget_imageview_parse_interpolation(view_data, attr);
	widget_imageview_parse_double(&view_data->zoom, attr, "zoom",
		IMAGEVIEW_MIN_ZOOM, IMAGEVIEW_MAX_ZOOM);
	widget_imageview_parse_double(&view_data->zoom_step, attr, "zoom-step",
		1.01, 10.0);
	if (attr == NULL)
		return;

	value = get_tag_attribute(attr, "pan-button");
	if (value != NULL) {
		button = widget_parse_bounded_integer(value, 1, 5, 2,
			"image view pan-button");
		view_data->pan_button = (guint)button;
		kill_tag_attribute(attr, "pan-button");
	}
	value = get_tag_attribute(attr, "wheel-zoom");
	if (value != NULL) {
		if (widget_parse_boolean(value, &boolean_value))
			view_data->wheel_zoom = boolean_value;
		else
			gtkdialog_warning("Invalid image view wheel-zoom '%s'; using true.",
				value);
		kill_tag_attribute(attr, "wheel-zoom");
	}
	value = get_tag_attribute(attr, "background");
	if (value != NULL) {
		if (gdk_color_parse(value, &color))
			gtk_widget_modify_bg(widget, GTK_STATE_NORMAL, &color);
		else
			gtkdialog_warning("Invalid image view background '%s'; using theme colour.",
				value);
		kill_tag_attribute(attr, "background");
	}
}

static gboolean widget_imageview_exposed(GtkWidget *widget,
	GdkEventExpose *event, AttributeSet *attributes)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);
	GdkPixbuf *rendered;
	gint height;
	gint image_x;
	gint image_y;
	gint width;
	gint x;
	gint y;

	widget_imageview_update_geometry(widget);
	if (view_data->pixbuf != NULL) {
		image_x = MAX(0,
			(widget->allocation.width - view_data->displayed_width) / 2);
		image_y = MAX(0,
			(widget->allocation.height - view_data->displayed_height) / 2);
		x = MAX(event->area.x, image_x);
		y = MAX(event->area.y, image_y);
		width = MIN(event->area.x + event->area.width,
			image_x + view_data->displayed_width) - x;
		height = MIN(event->area.y + event->area.height,
			image_y + view_data->displayed_height) - y;
		if (width > 0 && height > 0) {
			rendered = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8,
				width, height);
			if (rendered != NULL) {
				gdk_pixbuf_scale(view_data->pixbuf, rendered,
					0, 0, width, height,
					image_x - x, image_y - y,
					view_data->displayed_zoom,
					view_data->displayed_zoom,
					view_data->interpolation);
				gdk_draw_pixbuf(widget->window,
					widget->style->fg_gc[GTK_STATE_NORMAL], rendered,
					0, 0, x, y, width, height,
					GDK_RGB_DITHER_NORMAL, 0, 0);
				g_object_unref(rendered);
			}
		}
	}
	widget_signal_executor(widget, attributes, "expose-event");
	return FALSE;
}

static gboolean widget_imageview_button_pressed(GtkWidget *widget,
	GdkEventButton *event, gpointer user_data)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);
	GtkScrolledWindow *scrolled;
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;

	(void)user_data;
	if (event->button != view_data->pan_button)
		return FALSE;
	scrolled = widget_imageview_get_scrolled_window(widget);
	if (scrolled == NULL)
		return FALSE;
	hadjustment = gtk_scrolled_window_get_hadjustment(scrolled);
	vadjustment = gtk_scrolled_window_get_vadjustment(scrolled);
	view_data->dragging = TRUE;
	view_data->drag_x = event->x_root;
	view_data->drag_y = event->y_root;
	view_data->drag_hvalue = hadjustment->value;
	view_data->drag_vvalue = vadjustment->value;
	return FALSE;
}

static gboolean widget_imageview_button_released(GtkWidget *widget,
	GdkEventButton *event, gpointer user_data)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);

	(void)user_data;
	if (event->button == view_data->pan_button)
		view_data->dragging = FALSE;
	return FALSE;
}

static gboolean widget_imageview_motion(GtkWidget *widget,
	GdkEventMotion *event, gpointer user_data)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);
	GdkModifierType button_mask;
	GtkScrolledWindow *scrolled;
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;
	gdouble value;

	(void)user_data;
	if (!view_data->dragging)
		return FALSE;
	switch (view_data->pan_button) {
	case 1:
		button_mask = GDK_BUTTON1_MASK;
		break;
	case 2:
		button_mask = GDK_BUTTON2_MASK;
		break;
	case 3:
		button_mask = GDK_BUTTON3_MASK;
		break;
	case 4:
		button_mask = GDK_BUTTON4_MASK;
		break;
	default:
		button_mask = GDK_BUTTON5_MASK;
		break;
	}
	if ((event->state & button_mask) == 0) {
		view_data->dragging = FALSE;
		return FALSE;
	}
	scrolled = widget_imageview_get_scrolled_window(widget);
	if (scrolled == NULL)
		return FALSE;
	hadjustment = gtk_scrolled_window_get_hadjustment(scrolled);
	vadjustment = gtk_scrolled_window_get_vadjustment(scrolled);
	value = view_data->drag_hvalue - (event->x_root - view_data->drag_x);
	gtk_adjustment_set_value(hadjustment, CLAMP(value, hadjustment->lower,
		MAX(hadjustment->lower, hadjustment->upper - hadjustment->page_size)));
	value = view_data->drag_vvalue - (event->y_root - view_data->drag_y);
	gtk_adjustment_set_value(vadjustment, CLAMP(value, vadjustment->lower,
		MAX(vadjustment->lower, vadjustment->upper - vadjustment->page_size)));
	return TRUE;
}

static gboolean widget_imageview_scrolled(GtkWidget *widget,
	GdkEventScroll *event, gpointer user_data)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);
	gdouble zoom;

	(void)user_data;
	if (!view_data->wheel_zoom || (event->state & GDK_CONTROL_MASK) == 0)
		return FALSE;
	zoom = view_data->displayed_zoom;
	if (event->direction == GDK_SCROLL_UP)
		zoom *= view_data->zoom_step;
	else if (event->direction == GDK_SCROLL_DOWN)
		zoom /= view_data->zoom_step;
	else
		return FALSE;
	widget_imageview_set_manual_zoom(widget, zoom);
	return TRUE;
}

static void widget_imageview_viewport_allocated(GtkWidget *viewport,
	GtkAllocation *allocation, GtkWidget *widget)
{
	(void)viewport;
	(void)allocation;
	widget_imageview_update_geometry(widget);
}

void widget_imageview_clear(variable *var)
{
	ImageViewData *view_data = widget_imageview_get_data(var->Widget);

	if (view_data->pixbuf != NULL) {
		g_object_unref(view_data->pixbuf);
		view_data->pixbuf = NULL;
	}
	g_free(view_data->source);
	view_data->source = NULL;
	widget_imageview_update_geometry(var->Widget);
}

GtkWidget *widget_imageview_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	ImageViewData *view_data;
	GtkWidget *widget;

	(void)Attr;
	(void)Type;
	widget = gtk_drawing_area_new();
	view_data = g_new0(ImageViewData, 1);
	view_data->zoom = 1.0;
	view_data->zoom_step = 1.25;
	view_data->displayed_zoom = 1.0;
	view_data->requested_width = -1;
	view_data->requested_height = -1;
	view_data->pan_button = 2;
	view_data->wheel_zoom = TRUE;
	view_data->interpolation = GDK_INTERP_BILINEAR;
	g_object_set_data_full(G_OBJECT(widget), "gtkdialog-imageview-data",
		view_data, widget_imageview_data_free);
	widget_imageview_parse_options(widget, view_data, attr);
	gtk_widget_set_size_request(widget, 1, 1);
	gtk_widget_add_events(widget, GDK_BUTTON_PRESS_MASK |
		GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK |
		GDK_SCROLL_MASK | GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK);
	g_signal_connect(G_OBJECT(widget), "button-press-event",
		G_CALLBACK(widget_imageview_button_pressed), NULL);
	g_signal_connect(G_OBJECT(widget), "button-release-event",
		G_CALLBACK(widget_imageview_button_released), NULL);
	g_signal_connect(G_OBJECT(widget), "motion-notify-event",
		G_CALLBACK(widget_imageview_motion), NULL);
	g_signal_connect(G_OBJECT(widget), "scroll-event",
		G_CALLBACK(widget_imageview_scrolled), NULL);
	return widget;
}

gchar *widget_imageview_envvar_construct(GtkWidget *widget)
{
	ImageViewData *view_data = widget_imageview_get_data(widget);

	return g_strdup(view_data->source != NULL ? view_data->source : "");
}

void widget_imageview_fileselect(
	variable *var, const char *name, const char *value)
{
	(void)name;
	widget_imageview_load(var->Widget, value);
}

void widget_imageview_fit(variable *var)
{
	widget_imageview_set_scale_mode(var->Widget, IMAGEVIEW_SCALE_FIT);
}

void widget_imageview_fit_height(variable *var)
{
	widget_imageview_set_scale_mode(var->Widget, IMAGEVIEW_SCALE_FIT_HEIGHT);
}

void widget_imageview_fit_width(variable *var)
{
	widget_imageview_set_scale_mode(var->Widget, IMAGEVIEW_SCALE_FIT_WIDTH);
}

void widget_imageview_refresh(variable *var)
{
	GList *element;
	gchar *input;
	gboolean initialised;
	GtkWidget *parent;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_imageview_load(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT));

	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_imageview_input_by_command(var, input + 8);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_imageview_load(var->Widget, input + 5);
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
		G_CALLBACK(widget_imageview_exposed), var->Attributes);
	parent = gtk_widget_get_parent(var->Widget);
	if (parent != NULL)
		g_signal_connect_object(G_OBJECT(parent), "size-allocate",
			G_CALLBACK(widget_imageview_viewport_allocated),
			G_OBJECT(var->Widget), 0);
	widget_imageview_update_geometry(var->Widget);
}

void widget_imageview_save(variable *var)
{
	ImageViewData *view_data;
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
	view_data = widget_imageview_get_data(var->Widget);
	if (view_data->pixbuf == NULL) {
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
	if (!gdk_pixbuf_save(view_data->pixbuf, filename, save_format, &error,
		NULL)) {
		gtkdialog_warning("Couldn't save image '%s': %s", filename,
			error != NULL ? error->message : "unknown error");
		if (error != NULL)
			g_error_free(error);
	}
	g_free(format);
}

void widget_imageview_zoom_in(variable *var)
{
	ImageViewData *view_data = widget_imageview_get_data(var->Widget);

	widget_imageview_set_manual_zoom(var->Widget,
		view_data->displayed_zoom * view_data->zoom_step);
}

void widget_imageview_zoom_out(variable *var)
{
	ImageViewData *view_data = widget_imageview_get_data(var->Widget);

	widget_imageview_set_manual_zoom(var->Widget,
		view_data->displayed_zoom / view_data->zoom_step);
}

void widget_imageview_zoom_reset(variable *var)
{
	widget_imageview_set_manual_zoom(var->Widget, 1.0);
}
