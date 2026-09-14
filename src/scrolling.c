/*
 * scrolling.c: helpers for scrollable widgets
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2026       Artur Bednarek <artur@unix.org.pl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <gtk/gtk.h>

#include "scrolling.h"

#define BOTTOM_GRAVITY_DATA "gtkdialog-bottom-gravity"

typedef struct {
	GtkAdjustment *adjustment;
	gulong value_changed_handler;
	gdouble from_bottom;
	gboolean applying;
} BottomGravity;

static gdouble
adjustment_maximum_value(GtkAdjustment *adjustment)
{
	return MAX(adjustment->lower,
		adjustment->upper - adjustment->page_size);
}

static void
bottom_gravity_value_changed(GtkAdjustment *adjustment, gpointer user_data)
{
	BottomGravity *gravity = user_data;

	if (gravity->applying)
		return;
	gravity->from_bottom = MAX(0.0,
		adjustment_maximum_value(adjustment) - adjustment->value);
}

static void
bottom_gravity_size_allocated(GtkWidget *scrolled_window,
	GtkAllocation *allocation, gpointer user_data)
{
	BottomGravity *gravity = user_data;
	gdouble value;

	(void)scrolled_window;
	(void)allocation;
	value = MAX(gravity->adjustment->lower,
		adjustment_maximum_value(gravity->adjustment) -
		gravity->from_bottom);
	gravity->applying = TRUE;
	gtk_adjustment_set_value(gravity->adjustment, value);
	gravity->applying = FALSE;
}

static void
bottom_gravity_destroy(gpointer data)
{
	BottomGravity *gravity = data;

	if (gravity->value_changed_handler != 0)
		g_signal_handler_disconnect(gravity->adjustment,
			gravity->value_changed_handler);
	g_object_unref(gravity->adjustment);
	g_free(gravity);
}

void
scrolling_setup_bottom_gravity(GtkWidget *widget)
{
	GtkAdjustment *adjustment;
	GtkWidget *scrolled_window;
	BottomGravity *gravity;

	g_return_if_fail(GTK_IS_WIDGET(widget));
	scrolled_window = gtk_widget_get_ancestor(widget,
		GTK_TYPE_SCROLLED_WINDOW);
	if (scrolled_window == NULL ||
		g_object_get_data(G_OBJECT(scrolled_window),
			BOTTOM_GRAVITY_DATA) != NULL)
		return;

	adjustment = gtk_scrolled_window_get_vadjustment(
		GTK_SCROLLED_WINDOW(scrolled_window));
	gravity = g_new0(BottomGravity, 1);
	gravity->adjustment = g_object_ref(adjustment);
	gravity->value_changed_handler = g_signal_connect(adjustment,
		"value-changed", G_CALLBACK(bottom_gravity_value_changed), gravity);
	g_signal_connect(scrolled_window, "size-allocate",
		G_CALLBACK(bottom_gravity_size_allocated), gravity);
	g_object_set_data_full(G_OBJECT(scrolled_window), BOTTOM_GRAVITY_DATA,
		gravity, bottom_gravity_destroy);
}
