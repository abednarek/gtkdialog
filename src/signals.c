/*
 * signals.c:
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
#include "config.h"
#include "gtkdialog.h"
#include "actions.h"
#include "attributes.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widget_button.h"
#include "widget_checkbox.h"
#include "widget_colorbutton.h"
#include "widget_comboboxtext.h"
#include "widget_dock.h"
#include "widget_canvas.h"
#include "widget_accellabel.h"
#include "widget_edit.h"
#include "widget_entry.h"
#include "widget_expander.h"
#include "widget_fontbutton.h"
#include "widget_frame.h"
#include "widget_hscale.h"
#include "widget_list.h"
#include "widget_menuitem.h"
#include "widget_notebook.h"
#include "widget_pixmap.h"
#include "widget_radiobutton.h"
#include "widget_recentchooser.h"
#include "widget_statusicon.h"
#include "widget_spinbutton.h"
#include "widget_statusbar.h"
#include "widget_table.h"
#include "widget_terminal.h"
#include "widget_text.h"
#include "widget_timer.h"
#include "widget_tree.h"
#include "widget_window.h"
#include "widgets.h"
#if HAVE_VTE
#include <vte/vte.h>
#endif
#if HAVE_GTKDATABOX
#include <gtkdatabox.h>
#endif
#if HAVE_GTKSHEET
#include <gtksheet/gtksheet.h>
#endif
#if HAVE_SYS_INOTIFY_H
#include <errno.h>
#include <sys/inotify.h>
#endif

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS

/* Local variables */
char *condexpr[] = {
	"active_is_true(", "active_is_false(", "active_is_true (", "active_is_false (",
	"command_is_true(", "command_is_false(", "command_is_true (", "command_is_false (",
	"file_is_true(", "file_is_false(", "file_is_true (", "file_is_false (",
	"sensitive_is_true(", "sensitive_is_false(", "sensitive_is_true (", "sensitive_is_false (",
	"visible_is_true(", "visible_is_false(", "visible_is_true (", "visible_is_false (",
	"variable_is_true(", "variable_is_false(", "variable_is_true (", "variable_is_false ("
};

/* Local function prototypes */
gboolean widget_signal_executor_eval_condition(gchar *condition);

/* Notes: */

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void button_clicked_attr(GtkWidget *button, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(button, Attr, "clicked");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void button_entered_attr(GtkWidget *button, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(button, Attr, "enter");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void button_leaved_attr(GtkWidget *button, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(button, Attr, "leave");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void button_pressed_attr(GtkWidget *button, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(button, Attr, "pressed");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void button_released_attr(GtkWidget *button, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(button, Attr, "released");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_activate_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "activate");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

static void widget_context_menu_select_item(GtkWidget *widget,
	GdkEventButton *event)
{
	if (GTK_IS_TREE_VIEW(widget)) {
		GtkTreePath *path = NULL;
		GtkTreeSelection *selection;
		GtkSelectionMode mode;

		if (!gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget),
			(gint)event->x, (gint)event->y, &path, NULL, NULL, NULL))
			return;
		selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
		mode = gtk_tree_selection_get_mode(selection);
		if (!gtk_tree_selection_path_is_selected(selection, path)) {
			if (mode == GTK_SELECTION_MULTIPLE)
				gtk_tree_selection_unselect_all(selection);
			gtk_tree_selection_select_path(selection, path);
		}
		gtk_tree_path_free(path);
		return;
	}

#if GTK_CHECK_VERSION(2,6,0)
	if (GTK_IS_ICON_VIEW(widget)) {
		GtkTreePath *path = gtk_icon_view_get_path_at_pos(
			GTK_ICON_VIEW(widget), (gint)event->x, (gint)event->y);

		if (path == NULL)
			return;
		if (!gtk_icon_view_path_is_selected(GTK_ICON_VIEW(widget), path)) {
			if (gtk_icon_view_get_selection_mode(GTK_ICON_VIEW(widget)) ==
				GTK_SELECTION_MULTIPLE)
				gtk_icon_view_unselect_all(GTK_ICON_VIEW(widget));
			gtk_icon_view_select_path(GTK_ICON_VIEW(widget), path);
		}
		gtk_tree_path_free(path);
		return;
	}
#endif

	if (GTK_IS_CLIST(widget)) {
		gint row;
		gint column;

		if (gtk_clist_get_selection_info(GTK_CLIST(widget),
			(gint)event->x, (gint)event->y, &row, &column)) {
			if (GTK_CLIST(widget)->selection_mode == GTK_SELECTION_MULTIPLE &&
				g_list_find(GTK_CLIST(widget)->selection,
					GINT_TO_POINTER(row)) == NULL)
				gtk_clist_unselect_all(GTK_CLIST(widget));
			gtk_clist_select_row(GTK_CLIST(widget), row, column);
		}
	}
}

gboolean on_any_widget_popup_menu(GtkWidget *widget, AttributeSet *Attr)
{
	const gchar *program_name;

	(void)Attr;
	program_name = g_object_get_data(G_OBJECT(widget), "context-menu");
	if (program_name == NULL || *program_name == '\0')
		return FALSE;

	return widget_menu_popup_context(widget, program_name, 0,
		gtk_get_current_event_time());
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_context_event(GtkWidget *widget, GdkEvent *event,
	AttributeSet *Attr)
{
	if (event != NULL && event->type == GDK_BUTTON_PRESS &&
		((GdkEventButton *)event)->button == 3 &&
		g_object_get_data(G_OBJECT(widget), "context-menu") != NULL) {
		on_any_widget_button_pressed(widget, (GdkEventButton *)event, Attr);
		/* The generic event signal precedes button-press-event. Consume this
		 * explicitly configured context click even when menu construction fails,
		 * otherwise the existing action and warning are emitted a second time. */
		return TRUE;
	}

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_button_pressed(GtkWidget *widget,
	GdkEventButton *event, AttributeSet *Attr)
{
	const gchar *stored_program;
	gchar *context_program = NULL;
	gboolean context_shown = FALSE;
	gchar *ptrx = g_strdup_printf("%.0f", event->x);
	gchar *ptry = g_strdup_printf("%.0f", event->y);
	gchar *ptrmod = g_strdup_printf("%u", event->state);
	gchar *button = g_strdup_printf("%u", event->button);
	gchar *ptrbtn = g_strdup_printf("%u", event->button);
	gchar *ptrxroot = g_strdup_printf("%.0f", event->x_root);
	gchar *ptryroot = g_strdup_printf("%.0f", event->y_root);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	g_setenv("PTR_X", ptrx, TRUE);
	g_setenv("PTR_Y", ptry, TRUE);
	g_setenv("PTR_MOD", ptrmod, TRUE);
	g_setenv("BUTTON", button, TRUE);	/* Deprecated */
	g_setenv("PTR_BTN", ptrbtn, TRUE);
	g_setenv("PTR_X_ROOT", ptrxroot, TRUE);
	g_setenv("PTR_Y_ROOT", ptryroot, TRUE);
	if (event->button == 3 &&
		(stored_program = g_object_get_data(G_OBJECT(widget),
			"context-menu")) != NULL && *stored_program != '\0') {
		context_program = g_strdup(stored_program);
		widget_context_menu_select_item(widget, event);
		g_object_ref(widget);
	}

	widget_signal_executor(widget, Attr, "button-press-event");
	if (context_program != NULL && find_variable_by_widget(widget) != NULL)
		context_shown = widget_menu_popup_context(widget, context_program,
			event->button, event->time);

	g_unsetenv("PTR_X");
	g_unsetenv("PTR_Y");
	g_unsetenv("PTR_MOD");
	g_unsetenv("BUTTON");
	g_unsetenv("PTR_BTN");
	g_unsetenv("PTR_X_ROOT");
	g_unsetenv("PTR_Y_ROOT");
	g_free(ptrx);
	g_free(ptry);
	g_free(ptrmod);
	g_free(button);
	g_free(ptrbtn);
	g_free(ptrxroot);
	g_free(ptryroot);
	if (context_program != NULL) {
		g_object_unref(widget);
		g_free(context_program);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return context_shown;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_button_released(GtkWidget *widget,
	GdkEventButton *event, AttributeSet *Attr)
{
	gchar *ptrx = g_strdup_printf("%.0f", event->x);
	gchar *ptry = g_strdup_printf("%.0f", event->y);
	gchar *ptrmod = g_strdup_printf("%u", event->state);
	gchar *button = g_strdup_printf("%u", event->button);
	gchar *ptrbtn = g_strdup_printf("%u", event->button);
	gchar *ptrxroot = g_strdup_printf("%.0f", event->x_root);
	gchar *ptryroot = g_strdup_printf("%.0f", event->y_root);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	g_setenv("PTR_X", ptrx, TRUE);
	g_setenv("PTR_Y", ptry, TRUE);
	g_setenv("PTR_MOD", ptrmod, TRUE);
	g_setenv("BUTTON", button, TRUE);	/* Deprecated */
	g_setenv("PTR_BTN", ptrbtn, TRUE);
	g_setenv("PTR_X_ROOT", ptrxroot, TRUE);
	g_setenv("PTR_Y_ROOT", ptryroot, TRUE);

	widget_signal_executor(widget, Attr, "button-release-event");

	g_unsetenv("PTR_X");
	g_unsetenv("PTR_Y");
	g_unsetenv("PTR_MOD");
	g_unsetenv("BUTTON");
	g_unsetenv("PTR_BTN");
	g_unsetenv("PTR_X_ROOT");
	g_unsetenv("PTR_Y_ROOT");
	g_free(ptrx);
	g_free(ptry);
	g_free(ptrmod);
	g_free(button);
	g_free(ptrbtn);
	g_free(ptrxroot);
	g_free(ptryroot);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_changed_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "changed");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

void on_any_widget_day_selected_event(GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "day-selected");
}

void on_any_widget_day_selected_double_click_event(
	GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "day-selected-double-click");
}

void on_any_widget_month_changed_event(GtkWidget *widget, AttributeSet *Attr)
{
	widget_signal_executor(widget, Attr, "month-changed");
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_color_set_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "color-set");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_font_set_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "font-set");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_configure_event(GtkWidget *widget,
	GdkEventConfigure *event, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "configure-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_delete_event(GtkWidget *widget, GdkEvent *event,
	AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "delete-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_destroy_event(GtkWidget *widget, GdkEvent *event,
	AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "destroy-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_enter_notify_event(GtkWidget *widget,
	GdkEventCrossing *event, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "enter-notify-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_focus_in_event(GtkWidget *widget,
	GdkEventFocus *event, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "focus-in-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_focus_out_event(GtkWidget *widget,
	GdkEventFocus *event, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "focus-out-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_hide(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "hide");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

#if GTK_CHECK_VERSION(2,16,0)
/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_icon_press_event(GtkWidget *widget,
	GtkEntryIconPosition pos, GdkEvent *event, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (pos == GTK_ENTRY_ICON_PRIMARY)
		widget_signal_executor(widget, Attr, "primary-icon-press");
	else
		widget_signal_executor(widget, Attr, "secondary-icon-press");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_icon_release_event(GtkWidget *widget,
	GtkEntryIconPosition pos, GdkEvent *event, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (pos == GTK_ENTRY_ICON_PRIMARY)
		widget_signal_executor(widget, Attr, "primary-icon-release");
	else
		widget_signal_executor(widget, Attr, "secondary-icon-release");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
#endif

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_child_exited_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_VTE
	if (VTE_IS_TERMINAL(widget)) {
		/* The pid will now be invalid so set widget's variable to 0 */
		g_object_set_data(G_OBJECT(widget), "_pid", GINT_TO_POINTER(0));
	}
#endif

	widget_signal_executor(widget, Attr, "child-exited");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_key_press_event(GtkWidget *widget,
	GdkEventKey *event, AttributeSet *Attr)
{
	gchar *keyval = g_strdup_printf("0x%03x", event->keyval);
	const gchar *keysym_name = gdk_keyval_name(event->keyval);
	gchar *keysym = g_strdup(keysym_name ? keysym_name : "");
	gchar *keyuni = g_strdup_printf("%lc", gdk_keyval_to_unicode(event->keyval));
	gchar *keymod = g_strdup_printf("%u", event->state);
	gchar *keyraw = g_strdup_printf("0x%x", event->hardware_keycode);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	g_setenv("KEY_VAL", keyval, TRUE);
	g_setenv("KEY_SYM", keysym, TRUE);
	g_setenv("KEY_UNI", keyuni, TRUE);
	g_setenv("KEY_MOD", keymod, TRUE);
	g_setenv("KEY_RAW", keyraw, TRUE);

	widget_signal_executor(widget, Attr, "key-press-event");

	g_unsetenv("KEY_VAL");
	g_unsetenv("KEY_SYM");
	g_unsetenv("KEY_UNI");
	g_unsetenv("KEY_MOD");
	g_unsetenv("KEY_RAW");
	g_free(keyval);
	g_free(keysym);
	g_free(keyuni);
	g_free(keymod);
	g_free(keyraw);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_key_release_event(GtkWidget*widget,
	GdkEventKey *event, AttributeSet *Attr)
{
	gchar *keyval = g_strdup_printf("0x%03x", event->keyval);
	const gchar *keysym_name = gdk_keyval_name(event->keyval);
	gchar *keysym = g_strdup(keysym_name ? keysym_name : "");
	gchar *keyuni = g_strdup_printf("%lc", gdk_keyval_to_unicode(event->keyval));
	gchar *keymod = g_strdup_printf("%u", event->state);
	gchar *keyraw = g_strdup_printf("0x%x", event->hardware_keycode);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	g_setenv("KEY_VAL", keyval, TRUE);
	g_setenv("KEY_SYM", keysym, TRUE);
	g_setenv("KEY_UNI", keyuni, TRUE);
	g_setenv("KEY_MOD", keymod, TRUE);
	g_setenv("KEY_RAW", keyraw, TRUE);

	widget_signal_executor(widget, Attr, "key-release-event");

	g_unsetenv("KEY_VAL");
	g_unsetenv("KEY_SYM");
	g_unsetenv("KEY_UNI");
	g_unsetenv("KEY_MOD");
	g_unsetenv("KEY_RAW");
	g_free(keyval);
	g_free(keysym);
	g_free(keyuni);
	g_free(keymod);
	g_free(keyraw);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_leave_notify_event(GtkWidget *widget,
	GdkEventCrossing *event, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "leave-notify-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_map_event(GtkWidget *widget, GdkEvent *event,
	AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "map-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_realized(GtkWidget *widget, tag_attr *tag_attributes)
{
	variable         *var = find_variable_by_widget(widget);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): widget=%p  tag_attributes=%p\n", __func__,
		widget, tag_attributes);
#endif

	/* Menu and tool-item properties are applied before their parent container
	 * computes its first size requisition. */
	if (!GTK_IS_MENU(widget) && !GTK_IS_MENU_ITEM(widget) &&
		!GTK_IS_TOOL_ITEM(widget))
		widget_set_tag_attributes(widget, tag_attributes);

	/* We don't have an AttributeSet coming in, so retrieve it through the
	 * widget index. This remains unambiguous when variable names repeat. */
	if (var) {
		widget_signal_executor(widget, var->Attributes, "realize");
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_show(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "show");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_toggled_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "toggled");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean on_any_widget_unmap_event(GtkWidget *widget, GdkEvent *event,
	AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "unmap-event");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_value_changed_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "value-changed");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_select_row_event(GtkWidget *widget, gint row,
	gint column, GdkEvent *event, gpointer Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): row=%i column=%i\n", __func__, row, column);
#endif

	widget_signal_executor(widget, Attr, "select-row");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_selection_changed_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "selection-changed");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_row_activated_event(GtkWidget *widget,
	GtkTreePath *path, GtkTreeViewColumn *column, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "row-activated");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void on_any_widget_cursor_changed_event(GtkWidget *widget, AttributeSet *Attr)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_signal_executor(widget, Attr, "cursor-changed");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* Events occur in this sequence when a file is recreated using rox:
 * 
 * G_FILE_MONITOR_EVENT_DELETED (2)
 * G_FILE_MONITOR_EVENT_CREATED (3)
 * G_FILE_MONITOR_EVENT_CHANGED (0)
 * G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT (1)
 * G_FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED (4)
 * G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT (1)
 * 
 * And from rxvt:
 * 
 * G_FILE_MONITOR_EVENT_CHANGED (0)
 * G_FILE_MONITOR_EVENT_CHANGES_DONE_HINT (1)
 * 
 * Thanks and credit go to technosaurus for the inotify code.
 */

#if HAVE_SYS_INOTIFY_H
typedef struct {
	gint fd;
	gint wd;
	gint input_tag;
} WidgetInotifyMonitor;

static void widget_inotify_monitor_free(gpointer data)
{
	WidgetInotifyMonitor *monitor = data;

	if (monitor == NULL)
		return;
	if (monitor->input_tag > 0)
		gdk_input_remove(monitor->input_tag);
	if (monitor->wd >= 0)
		inotify_rm_watch(monitor->fd, monitor->wd);
	if (monitor->fd >= 0)
		close(monitor->fd);
	g_free(monitor);
}

static void widget_inotify_monitor_drain(gint source)
{
	/* A read buffer must be large enough for the event header and its
	 * optional name. A header-sized buffer can fail with EINVAL. */
	gchar buffer[4096];
	ssize_t bytes_read;

	do {
		bytes_read = read(source, buffer, sizeof(buffer));
	} while (bytes_read < 0 && errno == EINTR);

	if (bytes_read < 0 && errno != EAGAIN)
		fprintf(stderr, "%s(): Couldn't read file monitor: %s.\n",
			__func__, g_strerror(errno));
}

void on_any_widget_file_changed_event(gpointer data, gint source,
	GdkInputCondition condition)
#else
static gboolean widget_file_monitor_event_matches(GFileMonitor *monitor,
	GFile *file, GFileMonitorEvent event_type)
{
	GFile *target;

	if (event_type != G_FILE_MONITOR_EVENT_CHANGED &&
		event_type != G_FILE_MONITOR_EVENT_CREATED)
		return FALSE;

	target = g_object_get_data(G_OBJECT(monitor),
		"gtkdialog-file-monitor-target");
	return target != NULL && file != NULL && g_file_equal(target, file);
}

void on_any_widget_file_changed_event(GFileMonitor *monitor, GFile *file,
	GFile *other_file, GFileMonitorEvent event_type, variable *var)
#endif
{
#if HAVE_SYS_INOTIFY_H
	variable         *var = (variable*)data;
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_SYS_INOTIFY_H
	/* Just clearing, don't care the type */
	widget_inotify_monitor_drain(source);

#else
#ifdef DEBUG_CONTENT
	gchar *path = g_file_get_path(file);

	fprintf(stderr, "%s(): event_type=%i filename=%s\n", __func__,
		event_type, path ? path : "(non-local)");
	g_free(path);
#endif

	if (widget_file_monitor_event_matches(monitor, file, event_type))
#endif
	widget_signal_executor(var->Widget, var->Attributes, "file-changed");

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

#if HAVE_SYS_INOTIFY_H
void on_any_widget_auto_refresh_event(gpointer data, gint source,
	GdkInputCondition condition)
#else
void on_any_widget_auto_refresh_event(GFileMonitor *monitor, GFile *file,
	GFile *other_file, GFileMonitorEvent event_type, variable *var)
#endif
{
#if HAVE_SYS_INOTIFY_H
	variable         *var = (variable*)data;
#endif

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#if HAVE_SYS_INOTIFY_H
	/* Just clearing, don't care the type */
	widget_inotify_monitor_drain(source);

#else
	if (widget_file_monitor_event_matches(monitor, file, event_type))
#endif
	switch (var->Type) {
		case WIDGET_ACCELLABEL:
		case WIDGET_ABOUTDIALOG:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
		case WIDGET_ASSISTANT:
		case WIDGET_ASPECTFRAME:
		case WIDGET_BUTTON:
		case WIDGET_CALENDAR:
		case WIDGET_CELLVIEW:
		case WIDGET_CHECKBOX:
		case WIDGET_COLORBUTTON:
		case WIDGET_COLORSELECTION:
		case WIDGET_DIALOG:
		case WIDGET_DRAWINGAREA:
		case WIDGET_DOCK:
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
		case WIDGET_EDIT:
		case WIDGET_ENTRY:
		case WIDGET_EXPANDER:
		case WIDGET_CHOOSER:
		case WIDGET_FILECHOOSERBUTTON:
		case WIDGET_FILECHOOSERDIALOG:
		case WIDGET_FONTBUTTON:
		case WIDGET_FONTSELECTION:
		case WIDGET_FRAME:
		case WIDGET_HSCALE:
		case WIDGET_HSCROLLBAR:
		case WIDGET_HSV:
		case WIDGET_TASKLIST:
		case WIDGET_PAGER:
		case WIDGET_WINDOWSELECTOR:
		case WIDGET_CURVE:
		case WIDGET_HRULER:
		case WIDGET_VRULER:
		case WIDGET_ICONVIEW:
		case WIDGET_INFOBAR:
		case WIDGET_VSCALE:
		case WIDGET_VSCROLLBAR:
		case WIDGET_LIST:
		case WIDGET_LINKBUTTON:
		case WIDGET_MESSAGEDIALOG:
		case WIDGET_MENUITEM:
		case WIDGET_MENU:
		case WIDGET_POPUPMENU:
		case WIDGET_NOTEBOOK:
		case WIDGET_PIXMAP:
		case WIDGET_RADIOBUTTON:
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_RECENTCHOOSER:
		case WIDGET_SCALEBUTTON:
		case WIDGET_SPINBUTTON:
		case WIDGET_SPINNER:
		case WIDGET_STATUSBAR:
		case WIDGET_STATUSICON:
		case WIDGET_TABLE:
		case WIDGET_TERMINAL:
		case WIDGET_TEXT:
		case WIDGET_TIMER:
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_TREE:
#endif
		case WIDGET_VOLUMEBUTTON:
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			variables_refresh_widget(var);
			break;
		default:
			fprintf(stderr, "%s(): Unhandled widget type.\n", __func__);
			break;
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean window_delete_event_handler(GtkWidget *widget, GtkWidget *event,
	gpointer data)
{
	variable         *var;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	var = find_variable_by_widget(widget);
	if (var) {
		variables_drop_by_window_id(NULL, var->window_id);
	} else {
		/* Every live widget should have a direct registry entry, including
		 * widgets whose variable names are duplicated. Missing a window here
		 * indicates an internal registry error. */
		fprintf(stderr, "%s(): Window widget with NULL variable detected.\n",
			__func__);
		exit(EXIT_FAILURE);
	}

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): variables_count_widgets()=%i\n", __func__,
		variables_count_widgets());
#endif

	if (variables_count_widgets() == 0) {
		printf("EXIT=\"abort\"\n");

#ifdef DEBUG_CONTENT
		fprintf(stderr, "%s(): Calling exit(EXIT_SUCCESS)\n", __func__);
#endif

		exit(EXIT_SUCCESS);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return FALSE;
}

/***********************************************************************
 * Widget Signal Executor                                              *
 ***********************************************************************/
/* Thunor: With the work I've been doing I've called this function from
 * signal callbacks and let this iterate through the actions, manage the
 * default widget signal and the "if true/false" if applicable. I think
 * that originally each individual callback dealt with this but as the
 * number of widgets increases it makes sense to do everything here */

static gboolean widget_signal_executor_eval_active_action(gchar **command,
	gint is_active, gchar *condition)
{
	gboolean active_matches = TRUE;

	if (strncasecmp(*command, "if true ", 8) == 0) {
		*command += 8;
		active_matches = is_active != 0;
	} else if (strncasecmp(*command, "if false ", 9) == 0) {
		*command += 9;
		active_matches = is_active == 0;
	}

	/* The explicit condition historically applied last and replaced the
	 * legacy prefix result.  Existing dialogs can depend on that precedence. */
	if (condition != NULL)
		return widget_signal_executor_eval_condition(condition);
	return active_matches;
}

void widget_signal_executor(GtkWidget *widget, AttributeSet *Attr,
	const gchar *signal_name)
{
	GList            *element;
	gchar            *command;
	gchar            *condition;
	gchar            *function;
	gchar            *signal;
	gint              execute, is_active;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): Executing signal '%s' on widget %p\n",
		__func__, signal_name, widget);
#endif

	/* closewindow can destroy the window containing this widget while later
	 * synchronous actions still have to run.  Keep the originating object and
	 * its attached AttributeSet alive until the complete sequence finishes. */
	g_object_ref(widget);

	command = attributeset_get_first(&element, Attr, ATTR_ACTION);
	while (command) {
		execute = FALSE;
		is_active = -1;

		function = attributeset_get_this_tagattr(&element, Attr, ATTR_ACTION, "function");
		if (function == NULL) {
			/* function has replaced type in 0.8.3 so type is now deprecated */
			function = attributeset_get_this_tagattr(&element, Attr, ATTR_ACTION, "type");
		}
		signal = attributeset_get_this_tagattr(&element, Attr, ATTR_ACTION, "signal");
		condition = attributeset_get_this_tagattr(&element, Attr, ATTR_ACTION, "condition");

		if (signal && g_ascii_strcasecmp(signal, signal_name) == 0) {
			/***********************************************************
			 * This manages <action signal="type">                     *
			 * i.e the specified signal                                *
			 ***********************************************************/

#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s(): command=%s function=%s signal=%s signal_name=%s\n",
				__func__, command, function, signal != NULL ? signal : "(default)",
				signal_name);
#endif

			/* There's a class hierarchy to be aware of here */
/* GtkWidget--->GtkContainer--->GtkBin--->GtkButton--->GtkToggleButton */
			if (GTK_IS_TOGGLE_BUTTON(widget)) {
				is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
#if GTK_CHECK_VERSION(2,4,0)
/* GtkWidget--->GtkContainer--->GtkBin--->GtkToolItem--->GtkToolButton--->GtkToggleToolButton */
			} else if (GTK_IS_TOGGLE_TOOL_BUTTON(widget)) {
				is_active = gtk_toggle_tool_button_get_active(
					GTK_TOGGLE_TOOL_BUTTON(widget));
#endif
/* GtkWidget--->GtkContainer--->GtkBin--->GtkExpander */
			} else if (GTK_IS_EXPANDER(widget)) {
				is_active = gtk_expander_get_expanded(GTK_EXPANDER(widget));
/* GtkWidget--->GtkContainer--->GtkBin--->GtkItem--->GtkMenuItem--->GtkCheckMenuItem */
			} else if (GTK_IS_CHECK_MENU_ITEM(widget)) {
				is_active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
			} else {
				execute = widget_signal_executor_eval_condition(condition);
			}

			/* Deal with toggle widgets here as they can have a conditional
			 * prefix within the action in addition to a possible condition
			 * tag attribute, which historically is applied last. */
			if (is_active > -1)
				execute = widget_signal_executor_eval_active_action(&command,
					is_active, condition);

		} else if (signal == NULL) {
			/***********************************************************
			 * This manages <action> and <action function="type">      *
			 * i.e. the default widget signal                          *
			 ***********************************************************/

#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s(): command=%s function=%s signal=%s signal_name=%s\n",
				__func__, command, function, "(default)", signal_name);
#endif

			/* There's a class hierarchy to be aware of here */
/* GtkWidget--->GtkContainer--->GtkBin--->GtkButton--->GtkToggleButton */
			if (widget_statusicon_is_proxy(widget)) {
				if (strcasecmp(signal_name, "activate") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (widget_canvas_is_requested(widget)) {
				if (strcasecmp(signal_name, "item-activated") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#if HAVE_GTKDATABOX
			} else if (GTK_IS_DATABOX(widget)) {
				if (strcasecmp(signal_name, "selection-finalized") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#endif
#if HAVE_GTKSHEET
			} else if (GTK_IS_SHEET(widget)) {
				if (strcasecmp(signal_name, "deactivate") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#endif
			} else if (widget_dock_is_container(widget)) {
				if (strcasecmp(signal_name, "layout-changed") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (widget_dock_is_item(widget)) {
				if (strcasecmp(signal_name, "selected") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (GTK_IS_TOGGLE_BUTTON(widget)) {
				if (strcasecmp(signal_name, "toggled") == 0) {
					is_active = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
				}
#if GTK_CHECK_VERSION(2,12,0)
			} else if (GTK_IS_SCALE_BUTTON(widget)) {
				if (strcasecmp(signal_name, "value-changed") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#endif
#if GTK_CHECK_VERSION(2,10,0)
			} else if (GTK_IS_ASSISTANT(widget)) {
				if (strcasecmp(signal_name, "apply") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (widget_recentchooser_is_menu(widget)) {
				if (strcasecmp(signal_name, "item-activated") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (GTK_IS_RECENT_CHOOSER(widget)) {
				if (strcasecmp(signal_name, "item-activated") == 0 ||
					strcasecmp(signal_name, "selection-changed") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#endif
#if GTK_CHECK_VERSION(2,4,0)
/* GtkWidget--->GtkContainer--->GtkBin--->GtkToolItem--->GtkToolButton--->GtkToggleToolButton */
			} else if (GTK_IS_TOGGLE_TOOL_BUTTON(widget)) {
				if (strcasecmp(signal_name, "toggled") == 0) {
					is_active = gtk_toggle_tool_button_get_active(
						GTK_TOGGLE_TOOL_BUTTON(widget));
				}
#endif
			} else if (GTK_IS_COLOR_SELECTION(widget)) {
				if (strcasecmp(signal_name, "color-changed") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
			} else if (GTK_IS_CURVE(widget) || GTK_IS_GAMMA_CURVE(widget)) {
				if (strcasecmp(signal_name, "changed") == 0)
					execute = widget_signal_executor_eval_condition(condition);
/* GtkWidget--->GtkContainer--->GtkBin--->GtkButton--->GtkColorButton */
			} else if (GTK_IS_COLOR_BUTTON(widget)) {
				if (strcasecmp(signal_name, "color-set") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkContainer--->GtkBin--->GtkButton--->GtkFontButton */
			} else if (GTK_IS_FONT_BUTTON(widget)) {
				if (strcasecmp(signal_name, "font-set") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
			} else if (GTK_IS_FONT_SELECTION(widget)) {
				if (strcasecmp(signal_name, "font-changed") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkContainer--->GtkBin--->GtkButton */
			} else if (GTK_IS_BUTTON(widget)) {
				if (strcasecmp(signal_name, "clicked") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
#if GTK_CHECK_VERSION(2,4,0)
/* GtkWidget--->GtkContainer--->GtkBin--->GtkToolItem--->GtkToolButton */
			} else if (GTK_IS_TOOL_BUTTON(widget)) {
				if (strcasecmp(signal_name, "clicked") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
#endif
#if GTK_CHECK_VERSION(2,4,0)
			} else if (GTK_IS_FILE_CHOOSER_WIDGET(widget)) {
				if (strcasecmp(signal_name, "file-activated") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#endif
#if GTK_CHECK_VERSION(2,6,0)
/* GtkWidget--->GtkContainer--->GtkBox--->GtkHBox--->GtkFileChooserButton */
			} else if (GTK_IS_FILE_CHOOSER_BUTTON(widget)) {
				if (strcasecmp(signal_name, "file-set") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
			} else if (GTK_IS_ICON_VIEW(widget)) {
				if (strcasecmp(signal_name, "item-activated") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#endif
			} else if (GTK_IS_CALENDAR(widget)) {
				if (strcasecmp(signal_name, "day-selected") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (GTK_IS_DIALOG(widget)) {
				if (strcasecmp(signal_name, "response") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (GTK_IS_PLUG(widget)) {
				if (strcasecmp(signal_name, "embedded") == 0)
					execute = widget_signal_executor_eval_condition(condition);
			} else if (GTK_IS_SOCKET(widget)) {
				if (strcasecmp(signal_name, "plug-added") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#if GTK_CHECK_VERSION(2,18,0)
			} else if (GTK_IS_INFO_BAR(widget)) {
				if (strcasecmp(signal_name, "response") == 0)
					execute = widget_signal_executor_eval_condition(condition);
#endif
/* GtkWidget--->GtkContainer--->GtkBin--->GtkExpander */
			} else if (GTK_IS_EXPANDER(widget)) {
				if (strcasecmp(signal_name, "activate") == 0) {
					is_active = gtk_expander_get_expanded(GTK_EXPANDER(widget));
				}
/* GtkWidget--->GtkContainer--->GtkBin--->GtkItem--->GtkMenuItem--->GtkCheckMenuItem */
			} else if (GTK_IS_CHECK_MENU_ITEM(widget)) {
				if (strcasecmp(signal_name, "toggled") == 0) {
					is_active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget));
				}
/* GtkWidget--->GtkContainer--->GtkBin--->GtkItem--->GtkMenuItem */
			} else if (GTK_IS_MENU_ITEM(widget)) {
				if (strcasecmp(signal_name, "activate") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkContainer--->GtkBox--->GtkHBox--->GtkCombo */
			} else if (GTK_IS_COMBO_BOX(widget)) {
				if (strcasecmp(signal_name, "changed") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkContainer--->GtkCList */
			} else if (GTK_IS_CLIST(widget)) {
				if (strcasecmp(signal_name, "select-row") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkContainer--->GtkList */
			} else if (GTK_IS_LIST(widget)) {
				if (strcasecmp(signal_name, "selection-changed") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkContainer--->GtkTreeView */
			} else if (GTK_IS_TREE_VIEW(widget)) {
				if (strcasecmp(signal_name, "row-activated") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkEntry-->GtkSpinButton */
			} else if (GTK_IS_SPIN_BUTTON(widget)) {
				if (strcasecmp(signal_name, "value-changed") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkEntry */
			} else if (GTK_IS_ENTRY(widget)) {
				if (strcasecmp(signal_name, "changed") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkMisc--->GtkLabel */
			} else if (GTK_IS_LABEL(widget)) {
				/* A GtkLabel that ticks is a timer */
				if (strcasecmp(signal_name, "tick") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkProgress--->GtkProgressBar */
			} else if (GTK_IS_PROGRESS_BAR(widget)) {
				if (strcasecmp(signal_name, "time-out") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
/* GtkWidget--->GtkRange */
			} else if (GTK_IS_RANGE(widget)) {
				if (strcasecmp(signal_name, "value-changed") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
#if HAVE_VTE
/* GtkWidget--->VteTerminal */
			} else if (VTE_IS_TERMINAL(widget)) {
				if (strcasecmp(signal_name, "child-exited") == 0) {
					execute = widget_signal_executor_eval_condition(condition);
				}
#endif
			}

			/* Deal with toggle widgets here as they can have a conditional
			 * prefix within the action in addition to a possible condition
			 * tag attribute, which historically is applied last. */
			if (is_active > -1)
				execute = widget_signal_executor_eval_active_action(&command,
					is_active, condition);

		}

		if (execute)
			if (execute_action(widget, command, function) == 2)
				break;
		command = attributeset_get_next(&element, Attr, ATTR_ACTION);

	}
	g_object_unref(widget);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Widget Signal Executor - Evaluate Condition                         *
 ***********************************************************************/
/* On entry: condition is a pointer to the action's condition
 *                   tag attribute which could be null
 *  On exit: returns true if condition was null
 *           returns true if condition wasn't null but was unknown
 *           returns true if condition evaluated as such else false */

gboolean widget_signal_executor_eval_condition(gchar *condition)
{
	#define           TYPE_CONDFUNC_UNKNOWN 0
	#define           TYPE_CONDFUNC_ACTIVE 1
	#define           TYPE_CONDFUNC_COMMAND 2
	#define           TYPE_CONDFUNC_FILE 3
	#define           TYPE_CONDFUNC_SENSITIVE 4
	#define           TYPE_CONDFUNC_VISIBLE 5
	#define           TYPE_CONDFUNC_VARIABLE 6
	FILE             *infile;
	gchar            *argument = NULL;
	gchar            *argument_end;
	gchar            *read_cursor;
	gchar            *write_cursor;
	gchar            *line = NULL;
	gchar            *value = NULL;
	gint              condfunc = TYPE_CONDFUNC_UNKNOWN;
	gint              count;
	gint              not;
	gint              retval = FALSE;
	gint              state = -1;
	variable         *var;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "\n%s(): Entering.\n", __func__);
	fprintf(stderr, "%s():    condition=%s\n", __func__,
		condition != NULL ? condition : "(none)");
#endif

	if (condition != NULL) {

		/* Try and find one of the expressions within the condition */
		for (count = 0; count < TYPE_CONDFUNC_VARIABLE * 4; count++) {
			if (strncasecmp(condition, condexpr[count],
				strlen(condexpr[count])) == 0) {
				condfunc = count / 4 + TYPE_CONDFUNC_ACTIVE;
				not = count % 2;
				break;
			}
		}

		if (condfunc != TYPE_CONDFUNC_UNKNOWN) {

			/* Extract the argument without imposing an arbitrary size limit. */
			condition = strchr(condition, '(');
			argument_end = condition == NULL ? NULL : strrchr(condition + 1, ')');
			if (condition == NULL)
				return FALSE;
			condition++;
			/* Historical gtkdialog used the final ')' when present and
			 * otherwise treated the rest of the text as the argument. */
			argument = argument_end != NULL
				? g_strndup(condition, argument_end - condition)
				: g_strdup(condition);

			/* Fix any escaped double-quotes if present */
			read_cursor = argument;
			write_cursor = argument;
			while (*read_cursor != '\0') {
				if (read_cursor[0] == '\\' && read_cursor[1] == '"')
					read_cursor++;
				*write_cursor++ = *read_cursor++;
			}
			*write_cursor = '\0';

			/* Remove enveloping spaces if present */
			g_strstrip(argument);

			/* Remove enveloping double-quotes if present */
			if (strlen(argument) >= 2 && argument[0] == '"' &&
				argument[strlen(argument) - 1] == '"') {
				argument[0] = ' ';
				argument[strlen(argument)- 1] = ' ';
				g_strstrip(argument);
			}

			/***********************************************************
			 * if active()                                             *
			 ***********************************************************/
			if (condfunc == TYPE_CONDFUNC_ACTIVE) {

				if (variables_is_avail_by_name(argument)) {
					var = variables_get_by_name(argument);

					/* There's a class hierarchy to be aware of here */
/* GtkWidget--->GtkContainer--->GtkBin--->GtkButton--->GtkToggleButton */
					if (GTK_IS_TOGGLE_BUTTON(var->Widget)) {
						state = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(var->Widget));
#if GTK_CHECK_VERSION(2,4,0)
/* GtkWidget--->GtkContainer--->GtkBin--->GtkToolItem--->GtkToolButton--->GtkToggleToolButton */
					} else if (GTK_IS_TOGGLE_TOOL_BUTTON(var->Widget)) {
						state = gtk_toggle_tool_button_get_active(
							GTK_TOGGLE_TOOL_BUTTON(var->Widget));
#endif
#if GTK_CHECK_VERSION(2,20,0)
/* GtkWidget--->GtkSpinner */
					} else if (GTK_IS_SPINNER(var->Widget)) {
						g_object_get(G_OBJECT(var->Widget), "active", &state, NULL);
#endif
/* GtkWidget--->GtkContainer--->GtkBin--->GtkExpander */
					} else if (GTK_IS_EXPANDER(var->Widget)) {
						state = gtk_expander_get_expanded(GTK_EXPANDER(var->Widget));
/* GtkWidget--->GtkContainer--->GtkBin--->GtkItem--->GtkMenuItem--->GtkCheckMenuItem */
					} else if (GTK_IS_CHECK_MENU_ITEM(var->Widget)) {
						state = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(var->Widget));
					} else {
						state = -1;
					}

					if (not) {
						if (state == 0) {
							retval = TRUE;
						}
					} else {
						if (state == 1) {
							retval = TRUE;
						}
					}
				}

			/***********************************************************
			 * if command()                                            *
			 ***********************************************************/
			} else if (condfunc == TYPE_CONDFUNC_COMMAND) {

				/* Do as action_shellcommand() does */
				variables_export_all();

				/* Opening pipe for reading... */
				if ((infile = widget_opencommand(argument))) {
					line = widget_read_line(infile);
					if (line == NULL)
						line = g_strdup("");

					if (widget_parse_boolean(line, &state) &&
						(not ? !state : state))
						retval = TRUE;

					/* Close the file */
					widget_closecommand(infile, argument);
					g_free(line);
					line = NULL;
				} else {
					fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n", __func__,
						argument);
				}

			/***********************************************************
			 * if file()                                               *
			 ***********************************************************/
			} else if (condfunc == TYPE_CONDFUNC_FILE) {

				if ((infile = fopen(argument, "r"))) {
					line = widget_read_line(infile);
					if (line == NULL)
						line = g_strdup("");

					if (widget_parse_boolean(line, &state) &&
						(not ? !state : state))
						retval = TRUE;

					/* Close the file */
					fclose(infile);
					g_free(line);
					line = NULL;
				} else {
					fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n", __func__,
						argument);
				}

			/***********************************************************
			 * if sensitive()                                          *
			 ***********************************************************/
			} else if (condfunc == TYPE_CONDFUNC_SENSITIVE) {

				if (variables_is_avail_by_name(argument)) {
					var = variables_get_by_name(argument);

					state = gtk_widget_get_sensitive(var->Widget);

					if (not) {
						if (state == 0) {
							retval = TRUE;
						}
					} else {
						if (state == 1) {
							retval = TRUE;
						}
					}
				}

			/***********************************************************
			 * if visible()                                            *
			 ***********************************************************/
			} else if (condfunc == TYPE_CONDFUNC_VISIBLE) {

				if (variables_is_avail_by_name(argument)) {
					var = variables_get_by_name(argument);

					state = gtk_widget_get_visible(var->Widget);

					if (not) {
						if (state == 0) {
							retval = TRUE;
						}
					} else {
						if (state == 1) {
							retval = TRUE;
						}
					}
				}

			/***********************************************************
			 * if variable()                                           *
			 ***********************************************************/
			} else if (condfunc == TYPE_CONDFUNC_VARIABLE) {

				if (variables_is_avail_by_name(argument)) {
					var = variables_get_by_name(argument);
					value = widget_get_text_value(var->Widget, var->Type);
					if (widget_parse_boolean(value, &state) &&
						(not ? !state : state))
						retval = TRUE;
					g_free(value);
					value = NULL;
				}

			}

#ifdef DEBUG_CONTENT
			fprintf(stderr, "%s():        argument=%s\n", __func__, argument);
			fprintf(stderr, "%s():        not=%i\n", __func__, not);
			fprintf(stderr, "%s():        line=%s\n", __func__,
				line ? line : "(null)");
			fprintf(stderr, "%s():        state=%i\n", __func__, state);
			fprintf(stderr, "%s():        retval=%i\n", __func__, retval);
#endif
		} else {
			fprintf(stderr, "%s(): Unknown expression '%s'\n", __func__,
				condition);
			/* Unknown conditions were historically ignored. */
			retval = TRUE;
		}
	} else {
		/* No condition so ignore it */
		retval = TRUE;
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s():    retval=%i\n", __func__, retval);
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
	g_free(argument);

	return retval;
}
#undef DEBUG_TRANSITS
#undef DEBUG_CONTENT

/***********************************************************************
 * Widget File Monitor Try Create                                      *
 ***********************************************************************/
/* If the tag attribute file-monitor is true then attempt to create a
 * file monitor and emit a signal on change.
 * 
 * If the tag attribute auto-refresh is true then attempt to create a
 * file monitor and connect directly to the widget's refresh function.
 * 
 * The monitor object is attached to the widget as a piece of data
 * with a unique sequential name starting at "_monitor0" which will be
 * used later to cancel the monitor when/if the widget is dropped.
 * 
 * Thanks and credit go to technosaurus for the inotify code.
 */

#if HAVE_SYS_INOTIFY_H
void widget_file_monitor_try_create(variable *var, gchar *filename)
{
	WidgetInotifyMonitor *monitor;
	gchar             name[32];
	gchar            *value;
	gint              count;
	gint              fd, wd;
	gint              index = 0;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Is this file going to be monitored? */
	if (var->widget_tag_attr) {

		for (count = 0; count < 2; count++) {

			/* Get file-monitor (custom) or auto-refresh (custom) */
			if (((!count && (value = get_tag_attribute(var->widget_tag_attr,
				"file-monitor"))) ||
				(count && (value = get_tag_attribute(var->widget_tag_attr,
				"auto-refresh")))) && widget_attribute_is_true(value)) {

				if ((fd = inotify_init()) != -1) {

					if ((wd = inotify_add_watch(fd, (gchar*)filename,
						IN_MODIFY)) != -1) {
#ifdef DEBUG_CONTENT
						fprintf(stderr, "%s(): fd=%i wd=%i\n", __func__,
							fd, wd);
#endif
						/* Generate unique name */
						while (TRUE) {
							g_snprintf(name, sizeof(name), "_monitor%i", index);
							if (!(g_object_get_data(G_OBJECT(var->Widget),
								name))) break;
							index++;
						}
#ifdef DEBUG_CONTENT
						fprintf(stderr, "%s(): name=%s\n", __func__, name);
#endif
						monitor = g_new0(WidgetInotifyMonitor, 1);
						monitor->fd = fd;
						monitor->wd = wd;

						if (!count) {
							/* Connect to the "changed" signal which will reach
							 * the application as the "file-changed" signal */
							monitor->input_tag = gdk_input_add(fd, GDK_INPUT_READ,
								on_any_widget_file_changed_event, (gpointer)var);
						} else {
							/* Connect to the "changed" signal which will call
							 * the widget's refresh function directly without
							 * being routed through gtkdialog's signal handling
							 * system and without emitting a signal (it's faster) */
							monitor->input_tag = gdk_input_add(fd, GDK_INPUT_READ,
								on_any_widget_auto_refresh_event, (gpointer)var);
						}
						g_object_set_data_full(G_OBJECT(var->Widget), name,
							monitor, widget_inotify_monitor_free);

					} else {
						if (fd != -1) close(fd);
					}
				}
				if (fd == -1 || wd == -1)
					fprintf(stderr, "%s(): Couldn't create file monitor for '%s'.\n",
						__func__, filename);
			}
		}
	}
}

#else
void widget_file_monitor_try_create(variable *var, gchar *filename)
{
	GError           *error;
	GFile            *directory;
	GFile            *file;
	GFileMonitor     *monitor;
	gchar             name[32];
	gchar            *resolved_filename;
	gchar            *value;
	gint              count;
	gint              index = 0;
	gint              rate_limit;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Is this file going to be monitored? */
	if (var->widget_tag_attr) {

		for (count = 0; count < 2; count++) {

			/* Get file-monitor (custom) or auto-refresh (custom) */
			if (((!count && (value = get_tag_attribute(var->widget_tag_attr,
				"file-monitor"))) ||
				(count && (value = get_tag_attribute(var->widget_tag_attr,
				"auto-refresh")))) && widget_attribute_is_true(value)) {

				/* Use the search result when the file already exists, but keep
				 * the requested path when it does not so its parent can still be
				 * monitored for a later creation. */
				resolved_filename = find_pixmap(filename);
				file = g_file_new_for_path(resolved_filename[0] != '\0'
					? resolved_filename : filename);
				g_free(resolved_filename);
				monitor = NULL;
				if (file) {

					error = NULL;
					directory = g_file_get_parent(file);
					if (directory) {
						monitor = g_file_monitor_directory(directory,
							G_FILE_MONITOR_NONE, FALSE, &error);
						g_object_unref(directory);
					}

#ifdef DEBUG_CONTENT
					fprintf(stderr, "%s(): file=%p monitor=%p\n", __func__,
						file, monitor);
#endif

					if (monitor) {
						/* Watching the parent keeps the monitor alive when an editor
						 * atomically replaces the target with rename(2). Retain the
						 * target so unrelated directory events can be ignored. */
						g_object_set_data_full(G_OBJECT(monitor),
							"gtkdialog-file-monitor-target", g_object_ref(file),
							(GDestroyNotify)g_object_unref);

						/* Get rate-limit (custom) */
						if ((value = get_tag_attribute(var->widget_tag_attr,
							"rate-limit"))) {
							rate_limit = widget_parse_nonnegative_integer(value,
								-1, "file monitor rate-limit");
							if (rate_limit >= 0)
								g_file_monitor_set_rate_limit(monitor, rate_limit);
						}

						/* Generate unique name */
						while (TRUE) {
							g_snprintf(name, sizeof(name), "_monitor%i", index++);
							if (!(g_object_get_data(G_OBJECT(var->Widget), name)))
								break;
						}

#ifdef DEBUG_CONTENT
						fprintf(stderr, "%s(): name=%s\n", __func__, name);
#endif

						/* Store monitor as a piece of widget data */
						g_object_set_data_full(G_OBJECT(var->Widget), name,
							monitor, (GDestroyNotify)g_object_unref);

						if (!count) {
							/* Connect to the "changed" signal which will reach
							 * the application as the "file-changed" signal */
							g_signal_connect(monitor, "changed",
								G_CALLBACK(on_any_widget_file_changed_event),
								(gpointer)var);
						} else {
							/* Connect to the "changed" signal which will call
							 * the widget's refresh function directly without
							 * being routed through gtkdialog's signal handling
							 * system and without emitting a signal (it's faster) */
							g_signal_connect(monitor, "changed",
								G_CALLBACK(on_any_widget_auto_refresh_event),
								(gpointer)var);
						}

					} else if (error) {
						fprintf(stderr, "%s(): %s\n", __func__, error->message);
						g_error_free(error);
					}
					g_object_unref(file);
				}
				if (!file || !monitor)
					fprintf(stderr, "%s(): Couldn't create file monitor for '%s'.\n",
						__func__, filename);
			}
		}
	}
}
#endif
