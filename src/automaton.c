/*
 * automaton.c: An automaton executing the program, creating widgets. 
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

/*
**
** $Id: automaton.c,v 1.3 2004/11/25 18:26:48 pipas Exp pipas $
** $Log: automaton.c,v $
** Revision 1.3  2004/11/25 18:26:48  pipas
** Menu seems to be much more better, and the actions are just the same.
**
** Revision 1.2  2004/11/18 16:45:52  root
**   The button now can have a pixmap and a label simultaneously.
**
** Revision 1.1  2004/11/16 20:45:22  root
** Initial revision
**
*/

#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <errno.h>
#include <math.h>

#include "config.h"
#include "gtkdialog.h"
#include "widgets.h"
#include "automaton.h"
#include "stringman.h"
#include "actions.h"
#include "attributes.h"
#include "widgets.h"
#include "size_groups.h"
#include "widget_accellabel.h"
#include "widget_arrow.h"
#include "widget_aboutdialog.h"
#include "widget_alignment.h"
#include "widget_assistant.h"
#include "widget_aspectframe.h"
#include "widget_button.h"
#include "widget_buttonbox.h"
#include "widget_calendar.h"
#include "widget_cellview.h"
#include "widget_checkbox.h"
#include "widget_chooser.h"
#include "widget_colorbutton.h"
#include "widget_colorselection.h"
#include "widget_combobox.h"
#include "widget_comboboxtext.h"
#include "widget_curve.h"
#include "widget_dialog.h"
#include "widget_dock.h"
#include "widget_drawingarea.h"
#include "widget_imageview.h"
#include "widget_edit.h"
#include "widget_entry.h"
#include "widget_eventbox.h"
#include "widget_expander.h"
#include "widget_filechooserbutton.h"
#include "widget_filechooserdialog.h"
#include "widget_fixed.h"
#include "widget_fontbutton.h"
#include "widget_fontselection.h"
#include "widget_frame.h"
#include "widget_grid.h"
#include "widget_handlebox.h"
#include "widget_hbox.h"
#include "widget_hscale.h"
#include "widget_hseparator.h"
#include "widget_hsv.h"
#include "widget_ruler.h"
#include "widget_iconview.h"
#include "widget_infobar.h"
#include "widget_layout.h"
#include "widget_linkbutton.h"
#include "widget_list.h"
#include "widget_messagedialog.h"
#include "widget_menubar.h"
#include "widget_menuitem.h"
#include "widget_notebook.h"
#include "widget_paned.h"
#include "widget_pixmap.h"
#include "widget_progressbar.h"
#include "widget_radiobutton.h"
#include "widget_recentchooser.h"
#include "widget_scalebutton.h"
#include "widget_scrollbar.h"
#include "widget_scrolledwindow.h"
#include "widget_sheet.h"
#include "widget_socket.h"
#include "widget_spinbutton.h"
#include "widget_spinner.h"
#include "widget_statusbar.h"
#include "widget_statusicon.h"
#include "widget_table.h"
#include "widget_tasklist.h"
#include "widget_pager.h"
#include "widget_windowselector.h"
#include "widget_offscreenwindow.h"
#include "widget_pagesetupdialog.h"
#include "widget_printdialog.h"
#include "widget_terminal.h"
#include "widget_text.h"
#include "widget_timer.h"
#include "widget_toolbar.h"
#include "widget_toolpalette.h"
#include "widget_tree.h"
#include "widget_vbox.h"
#include "widget_viewport.h"
#include "widget_window.h"
#include "signals.h"
#include "tag_attributes.h"

#undef DEBUG
#undef WARNING
#include "macros.h"

#if HAVE_VTE
#include <vte/vte.h>
#endif

#undef TOOLTIPS

extern gboolean option_no_warning;

instruction *program = NULL;
int instruction_counter = 0;		/* The first available memory cell */
size_t memory_counter = 0;			/* The size of program memory */

typedef struct {
	GtkWidget *container;
	gint target_window_id;
	gchar *instance;
	gchar *label;
	gboolean replace_box;
	gboolean destroyed;
	gulong destroy_handler;
} FragmentTarget;

static FragmentTarget *fragment_target = NULL;
static GHashTable *live_template_instances = NULL;
static gboolean fragment_attach_succeeded = FALSE;

typedef struct {
	GtkWidget *host;
	GSList *groups;
} FragmentAccelerators;

static void fragment_accelerators_free(gpointer data)
{
	FragmentAccelerators *accelerators = data;
	GSList *element;

	if (accelerators->host != NULL) {
		if (!(GTK_OBJECT_FLAGS(accelerators->host) & GTK_IN_DESTRUCTION)) {
			for (element = accelerators->groups; element != NULL;
				element = element->next)
				gtk_window_remove_accel_group(GTK_WINDOW(accelerators->host),
					GTK_ACCEL_GROUP(element->data));
		}
		g_object_remove_weak_pointer(G_OBJECT(accelerators->host),
			(gpointer *)&accelerators->host);
	}
	for (element = accelerators->groups; element != NULL;
		element = element->next)
		g_object_unref(element->data);
	g_slist_free(accelerators->groups);
	g_free(accelerators);
}

static void fragment_target_destroyed(GtkWidget *widget, gpointer data)
{
	(void)widget;
	((FragmentTarget *)data)->destroyed = TRUE;
}

gboolean program_fragment_instance_is_live(const gchar *instance)
{
	return live_template_instances != NULL &&
		g_hash_table_lookup(live_template_instances, instance) != NULL;
}

gboolean program_remove_tab_instance(const gchar *instance,
	GtkWidget *source, gboolean *source_removed)
{
	GtkWidget *page = live_template_instances == NULL ? NULL :
		g_hash_table_lookup(live_template_instances, instance);

	*source_removed = FALSE;
	if (page == NULL ||
		!GTK_IS_NOTEBOOK(gtk_widget_get_parent(page)))
		return FALSE;
	if (GTK_IS_WIDGET(source))
		*source_removed = source == page ||
			gtk_widget_is_ancestor(source, page);
	gtk_widget_destroy(page);
	return TRUE;
}

gboolean program_select_tab_instance(const gchar *instance)
{
	GtkWidget *page = live_template_instances == NULL ? NULL :
		g_hash_table_lookup(live_template_instances, instance);
	GtkWidget *notebook;
	gint index;

	if (page == NULL)
		return FALSE;
	notebook = gtk_widget_get_parent(page);
	if (!GTK_IS_NOTEBOOK(notebook))
		return FALSE;
	index = gtk_notebook_page_num(GTK_NOTEBOOK(notebook), page);
	if (index < 0)
		return FALSE;
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), index);
	return TRUE;
}

gboolean program_fragment_template_active(void)
{
	return fragment_target != NULL;
}

gboolean program_fragment_was_attached(void)
{
	return fragment_attach_succeeded;
}

gboolean program_set_tab_target(GtkWidget *notebook, gint target_window_id,
	const gchar *instance, const gchar *label)
{
	if (fragment_target != NULL || !GTK_IS_NOTEBOOK(notebook) ||
		program_fragment_instance_is_live(instance))
		return FALSE;
	fragment_target = g_new0(FragmentTarget, 1);
	fragment_attach_succeeded = FALSE;
	fragment_target->container = g_object_ref(notebook);
	fragment_target->target_window_id = target_window_id;
	fragment_target->instance = g_strdup(instance);
	fragment_target->label = g_strdup(label);
	fragment_target->destroy_handler = g_signal_connect(notebook, "destroy",
		G_CALLBACK(fragment_target_destroyed), fragment_target);
	return TRUE;
}

gboolean program_set_box_target(GtkWidget *box, gint target_window_id,
	const gchar *instance)
{
	GtkWidget *previous;

	if (fragment_target != NULL || !GTK_IS_BOX(box))
		return FALSE;
	previous = live_template_instances == NULL ? NULL :
		g_hash_table_lookup(live_template_instances, instance);
	/* Rebuilding a current child with the same instance is safe: its old
	 * variable mappings are dropped before the new child is attached. */
	if (previous != NULL && gtk_widget_get_parent(previous) != box)
		return FALSE;
	fragment_target = g_new0(FragmentTarget, 1);
	fragment_attach_succeeded = FALSE;
	fragment_target->container = g_object_ref(box);
	fragment_target->target_window_id = target_window_id;
	fragment_target->instance = g_strdup(instance);
	fragment_target->replace_box = TRUE;
	fragment_target->destroy_handler = g_signal_connect(box, "destroy",
		G_CALLBACK(fragment_target_destroyed), fragment_target);
	return TRUE;
}

void program_clear_fragment_target(void)
{
	if (fragment_target == NULL)
		return;
	if (!fragment_target->destroyed && g_signal_handler_is_connected(
		fragment_target->container, fragment_target->destroy_handler))
		g_signal_handler_disconnect(fragment_target->container,
			fragment_target->destroy_handler);
	g_object_unref(fragment_target->container);
	g_free(fragment_target->instance);
	g_free(fragment_target->label);
	g_free(fragment_target);
	fragment_target = NULL;
}

static void fragment_page_destroyed(GtkWidget *page, gpointer data)
{
	const gchar *instance = g_object_get_data(G_OBJECT(page),
		"gtkdialog-fragment-instance");

	(void)data;
	variables_drop_subtree(page);
	if (live_template_instances != NULL && instance != NULL) {
		g_hash_table_remove(live_template_instances, instance);
		if (g_hash_table_size(live_template_instances) == 0) {
			g_hash_table_destroy(live_template_instances);
			live_template_instances = NULL;
		}
	}
}

static void fragment_tab_close_clicked(GtkButton *button, gpointer data)
{
	GtkWidget *page = data;
	gchar *instance = g_strdup(g_object_get_data(G_OBJECT(button),
		"gtkdialog-tab-close-instance"));
	gchar *action = g_strdup(g_object_get_data(G_OBJECT(button),
		"gtkdialog-tab-close-action"));

	/* The action may itself remove the page (and this button). Keep copies
	 * of its arguments and never act on a newly created same-name page. */
	g_object_ref(page);
	if (instance != NULL && action != NULL && *action != '\0')
		execute_action(GTK_WIDGET(button), action, NULL);
	if (instance == NULL && GTK_IS_NOTEBOOK(gtk_widget_get_parent(page))) {
		variables_drop_subtree(page);
		gtk_widget_destroy(page);
	} else if (instance != NULL && live_template_instances != NULL &&
		g_hash_table_lookup(live_template_instances, instance) == page)
		gtk_widget_destroy(page);
	g_object_unref(page);
	g_free(action);
	g_free(instance);
}

GtkWidget *program_notebook_tab_label(GtkWidget *page, const gchar *label,
	const gchar *instance, const gchar *close_enabled,
	const gchar *close_action)
{
	GtkWidget *tab_label;
	GtkWidget *button;
	GtkWidget *image;
	gchar **pieces;
	gchar *expanded;

	if (close_enabled == NULL || !widget_attribute_is_true(close_enabled)) {
		tab_label = gtk_label_new(label);
		gtk_widget_show(tab_label);
		return tab_label;
	}

	tab_label = gtk_hbox_new(FALSE, 4);
	gtk_box_pack_start(GTK_BOX(tab_label), gtk_label_new(label),
		FALSE, FALSE, 0);
	button = gtk_button_new();
	gtk_button_set_relief(GTK_BUTTON(button), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click(GTK_BUTTON(button), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(button), 0);
	gtk_widget_set_tooltip_text(button, "Close tab");
	image = gtk_image_new_from_stock(GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
	gtk_container_add(GTK_CONTAINER(button), image);
	gtk_box_pack_end(GTK_BOX(tab_label), button, FALSE, FALSE, 0);
	if (instance != NULL)
		g_object_set_data_full(G_OBJECT(button),
			"gtkdialog-tab-close-instance", g_strdup(instance), g_free);
	if (instance != NULL && close_action != NULL) {
		pieces = g_strsplit(close_action, "@INSTANCE@", -1);
		expanded = g_strjoinv(instance, pieces);
		g_strfreev(pieces);
		g_object_set_data_full(G_OBJECT(button), "gtkdialog-tab-close-action",
			expanded, g_free);
	}
	g_signal_connect(button, "clicked",
		G_CALLBACK(fragment_tab_close_clicked), page);
	gtk_widget_show_all(tab_label);
	return tab_label;
}

static void program_fragment_box_packing(GtkWidget *fragment,
	gboolean *expand, gboolean *fill)
{
	GtkWidget *logical = fragment;
	variable *var = find_variable_by_widget(logical);
	variable *box_var = find_variable_by_widget(fragment_target->container);
	int type;
	int space_expand = project_space_expand;
	int space_fill = project_space_fill;
	const gchar *value;
	gboolean default_pack;

	while (var == NULL && GTK_IS_BIN(logical)) {
		logical = gtk_bin_get_child(GTK_BIN(logical));
		if (logical == NULL)
			break;
		var = find_variable_by_widget(logical);
	}
	type = var == NULL ? 0 : var->Type;
	default_pack = fragment != logical ||
		type == WIDGET_ALIGNMENT || type == WIDGET_EDIT ||
		type == WIDGET_FIXED || type == WIDGET_FRAME ||
		type == WIDGET_GRID || type == WIDGET_HPANED ||
		type == WIDGET_VPANED || type == WIDGET_SCROLLEDW ||
		type == WIDGET_SCROLLEDWINDOW ||
		type == WIDGET_HANDLEBOX || type == WIDGET_TOOLPALETTE;
	if (box_var != NULL && box_var->Type == WIDGET_HBOX &&
		type == WIDGET_ENTRY)
		default_pack = TRUE;
	if (box_var != NULL && box_var->widget_tag_attr != NULL) {
		value = get_tag_attribute(box_var->widget_tag_attr, "space-expand");
		if (value != NULL)
			space_expand = widget_attribute_is_true(value);
		value = get_tag_attribute(box_var->widget_tag_attr, "space-fill");
		if (value != NULL)
			space_fill = widget_attribute_is_true(value);
	}
	*expand = space_expand == -1 ? default_pack : space_expand;
	*fill = space_fill == -1 ? default_pack : space_fill;
	if (var != NULL && var->widget_tag_attr != NULL) {
		value = get_tag_attribute(var->widget_tag_attr, "space-expand");
		if (value != NULL)
			*expand = widget_attribute_is_true(value);
		value = get_tag_attribute(var->widget_tag_attr, "space-fill");
		if (value != NULL)
			*fill = widget_attribute_is_true(value);
	}
}

static GtkWidget *program_attach_fragment(GtkWidget *root,
	gint staged_window_id)
{
	GtkWidget *page = NULL;
	GtkWidget *tab_label;
	GtkWidget *host;
	variable *root_var;
	FragmentAccelerators *accelerators;
	GSList *element;
	GList *children;
	GList *child;
	gboolean expand;
	gboolean fill;

	accelerators = g_new0(FragmentAccelerators, 1);
	for (element = gtk_accel_groups_from_object(G_OBJECT(root));
		element != NULL; element = element->next)
		accelerators->groups = g_slist_prepend(accelerators->groups,
			g_object_ref(element->data));
	accelerators->groups = g_slist_reverse(accelerators->groups);

	if (GTK_IS_WINDOW(root))
		page = gtk_bin_get_child(GTK_BIN(root));
	if (page != NULL)
		g_object_ref(page);
	if (page != NULL)
		gtk_container_remove(GTK_CONTAINER(root), page);
	root_var = find_variable_by_widget(root);
	if (root_var != NULL)
		variables_drop_by_window_id(root_var, staged_window_id);
	gtk_widget_destroy(root);

	if (page == NULL || fragment_target->destroyed) {
		gtkdialog_warning("Template must contain one widget tree and a live "
			"destination container.");
		if (page != NULL) {
			variables_drop_subtree(page);
			gtk_widget_destroy(page);
			g_object_unref(page);
		}
		fragment_accelerators_free(accelerators);
		return NULL;
	}
	if (fragment_target->replace_box) {
		/* The new subtree is fully built before any old child is touched.
		 * Dropping each old subtree first cancels its timers and monitors. */
		children = gtk_container_get_children(
			GTK_CONTAINER(fragment_target->container));
		for (child = children; child != NULL; child = child->next) {
			if (fragment_target->destroyed)
				break;
			variables_drop_subtree(GTK_WIDGET(child->data));
			gtk_widget_destroy(GTK_WIDGET(child->data));
		}
		g_list_free(children);
		if (fragment_target->destroyed) {
			variables_drop_subtree(page);
			gtk_widget_destroy(page);
			g_object_unref(page);
			fragment_accelerators_free(accelerators);
			return NULL;
		}
	}

	variables_reassign_subtree(page, staged_window_id,
		fragment_target->target_window_id);
	if (live_template_instances == NULL)
		live_template_instances = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, NULL);
	g_object_set_data_full(G_OBJECT(page), "gtkdialog-fragment-instance",
		g_strdup(fragment_target->instance), g_free);
	host = gtk_widget_get_toplevel(fragment_target->container);
	if (GTK_IS_WINDOW(host) && accelerators->groups != NULL) {
		accelerators->host = host;
		g_object_add_weak_pointer(G_OBJECT(host),
			(gpointer *)&accelerators->host);
		for (element = accelerators->groups; element != NULL;
			element = element->next)
			gtk_window_add_accel_group(GTK_WINDOW(host),
				GTK_ACCEL_GROUP(element->data));
	}
	g_object_set_data_full(G_OBJECT(page), "gtkdialog-fragment-accelerators",
		accelerators, fragment_accelerators_free);
	g_signal_connect(page, "destroy", G_CALLBACK(fragment_page_destroyed), NULL);
	g_hash_table_insert(live_template_instances,
		g_strdup(fragment_target->instance),
		page);
	if (fragment_target->replace_box) {
		program_fragment_box_packing(page, &expand, &fill);
		if (GTK_IS_HBOX(fragment_target->container))
			gtk_box_pack_end(GTK_BOX(fragment_target->container), page,
				expand, fill, 0);
		else
			gtk_box_pack_start(GTK_BOX(fragment_target->container), page,
				expand, fill, 0);
	} else {
		tag_attr *page_attributes = g_object_get_data(G_OBJECT(page),
			"gtkdialog-tag-attributes");
		tag_attr *notebook_attributes = g_object_get_data(
			G_OBJECT(fragment_target->container),
			"gtkdialog-tag-attributes");
		const gchar *close_enabled = g_object_get_data(
			G_OBJECT(fragment_target->container), "tab-close-buttons");
		const gchar *close_action = page_attributes == NULL ? NULL :
			get_tag_attribute(page_attributes, "tab-close-action");

		if (close_enabled == NULL && notebook_attributes != NULL)
			close_enabled = get_tag_attribute(notebook_attributes,
				"tab-close-buttons");
		if (close_action == NULL)
			close_action = g_object_get_data(
				G_OBJECT(fragment_target->container), "tab-close-action");
		if (close_action == NULL && notebook_attributes != NULL)
			close_action = get_tag_attribute(notebook_attributes,
				"tab-close-action");
		tab_label = program_notebook_tab_label(page,
			fragment_target->label, fragment_target->instance,
			close_enabled, close_action);
		gtk_notebook_append_page(GTK_NOTEBOOK(fragment_target->container), page,
			tab_label);
	}
	fragment_attach_succeeded = TRUE;
	return page;
}

/* This records the last window widget created */
GtkWidget *window = NULL;

/* Widget-stack depths saved by active imperative if/while bodies. */
static GArray *imperative_block_depths = NULL;
typedef struct {
	GtkWidget *tool_button;
} RadioGroupState;

/* Radio tool buttons saved while entering nested container scopes.  Classic
 * radiobuttons retain gtkdialog's historical post-order grouping contract. */
static GArray *radio_group_stack = NULL;

/*
 * Static function definitions.
 */
static int 
instruction_execute_push(token Token, 
		AttributeSet *Attr, 
		tag_attr *tag_attributes);

static gboolean imperative_parse_number(const gchar *text, gdouble *value);
static void imperative_push_value(const gchar *value);
static void imperative_push_number(gdouble value);
static gboolean imperative_pop_number(gdouble *value);

void print_command(instruction command)
{
    token Token;
    char *Argument;
    int Instr_Code;
    int Widget_Type;
    int Attribute_Number;

    Token = command.command;
    Argument = command.argument;

    Instr_Code = Token & COMMAND;
    Widget_Type = Token & WIDGET_TYPE;
    Attribute_Number = Token & ATTRIBUTE;

    switch (Instr_Code) {
    case PUSH:
	printf("push");
	switch (Widget_Type) {
		case WIDGET_ABOUTDIALOG:
			printf("(new aboutdialog())");
			break;
		case WIDGET_ALIGNMENT:
			printf("(new alignment(pop()))");
			break;
		case WIDGET_ASSISTANT:
			printf("(new assistant(pop()))");
			break;
		case WIDGET_ASPECTFRAME:
			printf("(new aspectframe(pop()))");
			break;
		case WIDGET_CANCELBUTTON:
			printf("(new cancelbutton())");
			break;
		case WIDGET_HELPBUTTON:
			printf("(new helpbutton())");
			break;
		case WIDGET_NOBUTTON:
			printf("(new nobutton())");
			break;
		case WIDGET_OKBUTTON:
			printf("(new okbutton())");
			break;
		case WIDGET_YESBUTTON:
			printf("(new yesbutton())");
			break;
		case WIDGET_BUTTON:
			printf("(new button())");
			break;
		case WIDGET_CHECKBOX:
			printf("(new checkbox())");
			break;
		case WIDGET_COLORBUTTON:
			printf("(new colorbutton())");
			break;
		case WIDGET_COLORSELECTION:
			printf("(new colorselection())");
			break;
		case WIDGET_DIALOG:
			printf("(new dialog(pop()))");
			break;
		case WIDGET_COMBOBOX:
			printf("(new combobox())");
			break;
		case WIDGET_COMBOBOXENTRY:
			printf("(new comboboxentry())");
			break;
		case WIDGET_COMBOBOXTEXT:
			printf("(new comboboxtext())");
			break;
		case WIDGET_EDIT:
			printf("(new edit)");
			break;
		case WIDGET_ENTRY:
			printf("(new entry())");
			break;
		case WIDGET_EVENTBOX:
			printf("(new eventbox())");
			break;
		case WIDGET_EXPANDER:
			printf("(new expander())");
			break;
		case WIDGET_FILECHOOSERBUTTON:
			printf("(new filechooserbutton())");
			break;
		case WIDGET_FIXED:
			printf("(new fixed(pop()))");
			break;
		case WIDGET_FONTBUTTON:
			printf("(new fontbutton())");
			break;
		case WIDGET_FONTSELECTION:
			printf("(new fontselection())");
			break;
		case WIDGET_ACCELLABEL:
			printf("(new accellabel())");
			break;
		case WIDGET_ARROW:
			printf("(new arrow())");
			break;
		case WIDGET_HSV:
			printf("(new hsv())");
			break;
		case WIDGET_TASKLIST:
			printf("(new tasklist())");
			break;
		case WIDGET_PAGER:
			printf("(new pager())");
			break;
		case WIDGET_WINDOWSELECTOR:
			printf("(new windowselector())");
			break;
		case WIDGET_OFFSCREENWINDOW:
			printf("(new offscreenwindow(pop()))");
			break;
		case WIDGET_PAGESETUPDIALOG:
			printf("(new pagesetupdialog())");
			break;
		case WIDGET_PRINTDIALOG:
			printf("(new printdialog())");
			break;
		case WIDGET_FILECHOOSERDIALOG:
			printf("(new filechooserdialog())");
			break;
		case WIDGET_CURVE:
			printf("(new curve())");
			break;
		case WIDGET_HRULER:
			printf("(new hruler())");
			break;
		case WIDGET_VRULER:
			printf("(new vruler())");
			break;
		case WIDGET_PLUG:
			printf("(new plug(pop()))");
			break;
		case WIDGET_FRAME:
			printf("(new frame(pop()))");
			break;
		case WIDGET_GRID:
			printf("(new grid(pop()))");
			break;
		case WIDGET_HBOX:
			printf("(new hbox(pop()))");
			break;
		case WIDGET_HBUTTONBOX:
			printf("(new hbuttonbox(pop()))");
			break;
		case WIDGET_HPANED:
			printf("(new hpaned(pop()))");
			break;
		case WIDGET_HSCALE:
			printf("(new hscale())");
			break;
		case WIDGET_HSCROLLBAR:
			printf("(new hscrollbar())");
			break;
		case WIDGET_HSEPARATOR:
			printf("(new hseparator())");
			break;
		case WIDGET_ICONVIEW:
			printf("(new iconview())");
			break;
		case WIDGET_CELLVIEW:
			printf("(new cellview())");
			break;
		case WIDGET_DRAWINGAREA:
			printf("(new drawingarea())");
			break;
		case WIDGET_SHEET:
			printf("(new sheet())");
			break;
		case WIDGET_DOCK:
			printf("(new dock(pop()))");
			break;
		case WIDGET_DOCKITEM:
			printf("(new dockitem(pop()))");
			break;
		case WIDGET_IMAGEVIEW:
			printf("(new imageview())");
			break;
		case WIDGET_SOCKET:
			printf("(new socket())");
			break;
		case WIDGET_INFOBAR:
			printf("(new infobar(pop()))");
			break;
		case WIDGET_LAYOUT:
			printf("(new layout(pop()))");
			break;
		case WIDGET_LIST:
			printf("(new list())");
			break;
		case WIDGET_MENU:
			printf("(new menu(pop()))");
			break;
		case WIDGET_POPUPMENU:
			printf("(new popupmenu(pop()))");
			break;
		case WIDGET_MENUBAR:
			printf("(new menubar(pop()))");
			break;
		case WIDGET_MENUITEM:
			printf("(new menuitem())");
			break;
		case WIDGET_MENUITEMSEPARATOR:
			printf("(new menuitemseparator())");
			break;
		case WIDGET_MESSAGEDIALOG:
			printf("(new messagedialog())");
			break;
		case WIDGET_NOTEBOOK:
			printf("(new notebook(pop()))");
			break;
		case WIDGET_PIXMAP:
			printf("(new pixmap())");
			break;
		case WIDGET_PROGRESSBAR:
			printf("(new progressbar())");
			break;
		case WIDGET_RADIOBUTTON:
			printf("(new radiobutton())");
			break;
		case WIDGET_RECENTCHOOSER:
			printf("(new recentchooser())");
			break;
		case WIDGET_RECENTCHOOSERMENU:
			printf("(new recentchoosermenu())");
			break;
		case WIDGET_STATUSICON:
			printf("(new statusicon())");
			break;
		case WIDGET_SCALEBUTTON:
			printf("(new scalebutton())");
			break;
		case WIDGET_SCROLLEDWINDOW:
			printf("(new scrolledwindow(pop()))");
			break;
		case WIDGET_HANDLEBOX:
			printf("(new handlebox(pop()))");
			break;
		case WIDGET_SPINBUTTON:
			printf("(new spinbutton())");
			break;
		case WIDGET_SEPARATORTOOLITEM:
			printf("(new separatortoolitem())");
			break;
		case WIDGET_STATUSBAR:
			printf("(new statusbar())");
			break;
		case WIDGET_TABLE:
			printf("(new table())");
			break;
		case WIDGET_TERMINAL:
			printf("(new terminal())");
			break;
		case WIDGET_TEXT:
			printf("(new text())");
			break;
		case WIDGET_TIMER:
			printf("(new timer())");
			break;
		case WIDGET_TOOLBAR:
			printf("(new toolbar(pop()))");
			break;
		case WIDGET_TOOLBUTTON:
			printf("(new toolbutton())");
			break;
		case WIDGET_TOOLITEM:
			printf("(new toolitem(pop()))");
			break;
		case WIDGET_TOOLPALETTE:
			printf("(new toolpalette(pop()))");
			break;
		case WIDGET_TOOLITEMGROUP:
			printf("(new toolitemgroup(pop()))");
			break;
		case WIDGET_TOGGLETOOLBUTTON:
			printf("(new toggletoolbutton())");
			break;
		case WIDGET_RADIOTOOLBUTTON:
			printf("(new radiotoolbutton())");
			break;
		case WIDGET_MENUTOOLBUTTON:
			printf("(new menutoolbutton(pop()))");
			break;
		case WIDGET_CALENDAR:
			printf("(new calendar())");
			break;
		case WIDGET_LINKBUTTON:
			printf("(new linkbutton())");
			break;
		case WIDGET_SPINNER:
			printf("(new spinner())");
			break;
		case WIDGET_TOGGLEBUTTON:
			printf("(new togglebutton())");
			break;
		case WIDGET_TREE:
			printf("(new tree())");
			break;
		case WIDGET_VBOX:
			printf("(new vbox(pop()))");
			break;
		case WIDGET_VOLUMEBUTTON:
			printf("(new volumebutton())");
			break;
		case WIDGET_VBUTTONBOX:
			printf("(new vbuttonbox(pop()))");
			break;
		case WIDGET_VPANED:
			printf("(new vpaned(pop()))");
			break;
		case WIDGET_VIEWPORT:
			printf("(new viewport(pop()))");
			break;
		case WIDGET_VSCALE:
			printf("(new vscale())");
			break;
		case WIDGET_VSCROLLBAR:
			printf("(new vscrollbar())");
			break;
		case WIDGET_VSEPARATOR:
			printf("(new vseparator())");
			break;
		case WIDGET_WINDOW:
			printf("(new window(pop()))");
			break;


	case WIDGET_SCROLLEDW:
	    printf("(new scrolledwindow(pop()))");
	    break;
	case WIDGET_GVIM:
	    printf("(new gvim())");
	    break;
	case WIDGET_CHOOSER:
	    printf("(new chooser())");
	    break;
	default:
	    printf("(Unknown Widget: %d)", Widget_Type);
	}
	break;
    case IMASSG:
        printf("%s := pop()", Argument);
	break;
    case IMPUSH:
	printf("push");
	switch (Widget_Type) {
	case VARIABLE_NAME:
	    printf("(variable '%s')", Argument);
	    break;
	case CONST_NUMBER:
	    printf("(number '%s')", Argument);
	    break;
	case OP_ADD:
	    printf("(pop() + pop())");
	    break;
	case OP_SUBST:
	    printf("(pop() - pop())");
	    break;
	case OP_MULT:
	    printf("(pop() * pop())");
	    break;
	case OP_DIV:
	    printf("(pop() / pop())");
	    break;
	case REL_EQ:
	    printf("(pop() = pop())");
	    break;
	case REL_NE:
	    printf("(pop() != pop())");
	    break;

	default:
	    printf("(Unknown Parameter: %d)", Widget_Type);
	}
	break;
    case SUM:
	printf("push(pop() + pop())");
	break;
    case SHOW:
	printf("show");
	break;
    case SET:
	printf("%s := '%s'", attribute_name(Attribute_Number), Argument);
	break;
    case IFNGOTO:
	printf("if not pop() goto %d", command.ival);
	break;
	case GOTO:
	printf("goto %d", command.ival);
	break;
	case IMMERGE:
		printf("merge imperative widgets");
		break;
	case RGROUP_PUSH:
		printf("enter radio group scope");
		break;
	case RGROUP_POP:
		printf("leave radio group scope");
		break;
    default:
	printf(" %s(): unknown instruction: '%d' ", __func__, Instr_Code);
    }

    printf("\n");
    fflush(stdout);
}

void
print_program()
{
	gint pc;
	instruction command;
	
	for (pc = 0; pc < instruction_counter; ++pc) {
		command = program[pc];
		g_printf("%5d  ", pc);
		print_command(command);
	}
}

/*
** This function will run the program in memory.
*/
void run_program()
{
	static int        is_launched = 0;
	int               pc, q;
	stackelement      s;
	variable         *var;
	gchar            *progname;
	GtkWidget        *root_widget;
	GtkWidget        *fragment_root;
	gboolean          popup_program;
	gboolean          is_fragment_template = fragment_target != NULL;

	PIP_DEBUG("Program starting.");

	window_id++;	/* Generate a new unique window id */
	/* Implicit radio groups are local to one parsed window.  A launched
	 * window must not inherit the last radio widget from an earlier one. */
	lastradiowidget = NULL;
	lasttoolradiowidget = NULL;

	/*
	 * This is the main loop which creates the widgets, runs the program.
	 */
	imperative_block_depths = g_array_new(FALSE, FALSE, sizeof(size_t));
	radio_group_stack = g_array_new(FALSE, FALSE, sizeof(RadioGroupState));
	size_groups_begin();
	for (pc = 0; pc < instruction_counter; ) {
		int next_pc = instruction_execute(program[pc]);

		if (next_pc < 0)
			++pc;
		else if (next_pc >= instruction_counter)
			g_error("Imperative jump outside the program.");
		else
			pc = next_pc;
	}
	if (imperative_block_depths->len != 0)
		g_error("Unfinished imperative block.");
	g_array_free(imperative_block_depths, TRUE);
	imperative_block_depths = NULL;
	if (radio_group_stack->len != 0)
		g_error("Unfinished radio group scope.");
	g_array_free(radio_group_stack, TRUE);
	radio_group_stack = NULL;
	size_groups_end();

	PIP_DEBUG("Program ended.");

	/*
	 * When we executed the program, there is only one widget in
	 * the stack which is a window.
	 */
	s = pop();
	if (s.nwidgets != 1)
		g_error("A program must create exactly one top-level widget.");
	root_widget = s.widgets[0];
	if (is_fragment_template) {
		/* The parser has compiled a private window around a widget tree.
		 * Transfer its child before visibility processing can show that
		 * staging window or connect it to the running application. */
		g_object_ref(root_widget);
		stackelement_clear(&s);
		fragment_root = program_attach_fragment(root_widget, window_id);
		g_object_unref(root_widget);
		widget_show_all();
		if (fragment_root != NULL) {
			variables_initialize_subtree(fragment_root);
			g_object_unref(fragment_root);
		}
		window = NULL;
		program_clear_fragment_target();
		goto cleanup_program;
	}
	/* A synchronous show action may destroy its complete top-level before
	 * widget_show_all() returns. Keep the type and popup finalisation target
	 * alive without changing the historical action order. */
	g_object_ref(root_widget);
	widget_show_all();

	PIP_DEBUG("Top-level widget: %p.", root_widget);
	stackelement_clear(&s);

	//
	// Now, all of the widgets are realized, so we can initialize
	// them as needed.
	//
	variables_initialize_all();
	popup_program = widget_menu_popup_finish(root_widget, window_id);
	if (!popup_program && !GTK_IS_WINDOW(root_widget))
		g_error("A program root must be a window, dialog or <popupmenu>.");
	g_object_unref(root_widget);

	/* Thunor: This verifies that later launched windows contain a variable
	 * that matches the exported shell variable which is a requirement.
	 * I can't enforce it since there will be applications out there that
	 * might break, but I can dump a warning to the terminal */
	if (is_launched++ && !popup_program) {

		progname = get_program_name();
		var = variables_get_by_name(progname);

#ifdef DEBUG
		fprintf(stderr, "%s(): program_name=%s variable=%p\n",
			__func__, progname, var);
#endif
		if (var == NULL) {
			gtkdialog_warning("The recently launched window \"%s\" is missing \
an equivalently named variable directive <variable>%s</variable> which \
is a requirement of the launch action.", progname, progname);
		}

	}

cleanup_program:
	for (q = 0; q < instruction_counter; q++) {
		g_free(program[q].argument);
		program[q].argument = NULL;
		tag_attributeset_free(program[q].tag_attributes);
		program[q].tag_attributes = NULL;
	}
	instruction_counter = 0;
	reset_program_source();

	/* A launch action parses and builds another window while the original
	 * GTK loop is already running. Reuse that loop instead of nesting one
	 * more level for every close/relaunch cycle. */
	if (!is_fragment_template && gtk_main_level() == 0) {
#ifdef DEBUG
		fprintf(stderr, "%s(): Calling gtk_main()\n", __func__);
#endif
		gtk_main();
	}
}

void print_token(token Token)
{
	int Instr_Code;
	int Widget_Type;
	int Attribute_Number;
	Instr_Code = Token & COMMAND;
	Widget_Type = Token & WIDGET_TYPE;
	Attribute_Number = Token & ATTRIBUTE;

	switch (Instr_Code) {
		case PUSH:
			printf("push");
			break;
		case SUM:
			printf("push(pop() + pop())");
			goto finalize;
		case SET:
			printf("SET %s", 
					attribute_name(Attribute_Number));
			goto finalize;
		case RGROUP_PUSH:
			printf("enter radio group scope");
			goto finalize;
		case RGROUP_POP:
			printf("leave radio group scope");
			goto finalize;
		case EMPTY_WIDGETS:
			printf("push empty widget list");
			goto finalize;
		default:
			printf(" %s(): unknown instruction: '%d' ", __func__, Instr_Code);
	}

	switch (Widget_Type) {
		case WIDGET_ABOUTDIALOG:
			printf("(ABOUTDIALOG)");
			break;
		case WIDGET_ALIGNMENT:
			printf("(ALIGNMENT)");
			break;
		case WIDGET_ASSISTANT:
			printf("(ASSISTANT)");
			break;
		case WIDGET_ASPECTFRAME:
			printf("(ASPECTFRAME)");
			break;
		case WIDGET_CANCELBUTTON:
			printf("(CANCELBUTTON)");
			break;
		case WIDGET_HELPBUTTON:
			printf("(HELPBUTTON)");
			break;
		case WIDGET_NOBUTTON:
			printf("(NOBUTTON)");
			break;
		case WIDGET_OKBUTTON:
			printf("(OKBUTTON)");
			break;
		case WIDGET_YESBUTTON:
			printf("(YESBUTTON)");
			break;
		case WIDGET_BUTTON:
			printf("(BUTTON)");
			break;
		case WIDGET_CHECKBOX:
			printf("(CHECKBOX)");
			break;
		case WIDGET_COLORBUTTON:
			printf("(COLORBUTTON)");
			break;
		case WIDGET_COLORSELECTION:
			printf("(COLORSELECTION)");
			break;
		case WIDGET_DIALOG:
			printf("(DIALOG)");
			break;
		case WIDGET_COMBOBOX:
			printf("(COMBOBOX)");
			break;
		case WIDGET_COMBOBOXENTRY:
			printf("(COMBOBOXENTRY)");
			break;
		case WIDGET_COMBOBOXTEXT:
			printf("(COMBOBOXTEXT)");
			break;
		case WIDGET_EDIT:
			printf("(EDIT)");
			break;
		case WIDGET_ENTRY:
			printf("(ENTRY)");
			break;
		case WIDGET_EVENTBOX:
			printf("(EVENTBOX)");
			break;
		case WIDGET_EXPANDER:
			printf("(EXPANDER)");
			break;
		case WIDGET_FILECHOOSERBUTTON:
			printf("(FILECHOOSERBUTTON)");
			break;
		case WIDGET_FIXED:
			printf("(FIXED)");
			break;
		case WIDGET_FONTBUTTON:
			printf("(FONTBUTTON)");
			break;
		case WIDGET_FONTSELECTION:
			printf("(FONTSELECTION)");
			break;
		case WIDGET_ACCELLABEL:
			printf("(ACCELLABEL)");
			break;
		case WIDGET_ARROW:
			printf("(ARROW)");
			break;
		case WIDGET_HSV:
			printf("(HSV)");
			break;
		case WIDGET_TASKLIST:
			printf("(TASKLIST)");
			break;
		case WIDGET_PAGER:
			printf("(PAGER)");
			break;
		case WIDGET_WINDOWSELECTOR:
			printf("(WINDOWSELECTOR)");
			break;
		case WIDGET_OFFSCREENWINDOW:
			printf("(OFFSCREENWINDOW)");
			break;
		case WIDGET_PAGESETUPDIALOG:
			printf("(PAGESETUPDIALOG)");
			break;
		case WIDGET_PRINTDIALOG:
			printf("(PRINTDIALOG)");
			break;
		case WIDGET_FILECHOOSERDIALOG:
			printf("(FILECHOOSERDIALOG)");
			break;
		case WIDGET_CURVE:
			printf("(CURVE)");
			break;
		case WIDGET_HRULER:
			printf("(HRULER)");
			break;
		case WIDGET_VRULER:
			printf("(VRULER)");
			break;
		case WIDGET_PLUG:
			printf("(PLUG)");
			break;
		case WIDGET_FRAME:
			printf("(FRAME)");
			break;
		case WIDGET_GRID:
			printf("(GRID)");
			break;
		case WIDGET_HBOX:
			printf("(HBOX)");
			break;
		case WIDGET_HPANED:
			printf("(HPANED)");
			break;
		case WIDGET_HSCALE:
			printf("(HSCALE)");
			break;
		case WIDGET_HSCROLLBAR:
			printf("(HSCROLLBAR)");
			break;
		case WIDGET_HSEPARATOR:
			printf("(HSEPARATOR)");
			break;
		case WIDGET_ICONVIEW:
			printf("(ICONVIEW)");
			break;
		case WIDGET_CELLVIEW:
			printf("(CELLVIEW)");
			break;
		case WIDGET_DRAWINGAREA:
			printf("(DRAWINGAREA)");
			break;
		case WIDGET_SHEET:
			printf("(SHEET)");
			break;
		case WIDGET_DOCK:
			printf("(DOCK)");
			break;
		case WIDGET_DOCKITEM:
			printf("(DOCKITEM)");
			break;
		case WIDGET_IMAGEVIEW:
			printf("(IMAGEVIEW)");
			break;
		case WIDGET_SOCKET:
			printf("(SOCKET)");
			break;
		case WIDGET_INFOBAR:
			printf("(INFOBAR)");
			break;
		case WIDGET_LAYOUT:
			printf("(LAYOUT)");
			break;
		case WIDGET_LIST:
			printf("(LIST)");
			break;
		case WIDGET_MENU:
			printf("(MENU)");
			break;
		case WIDGET_POPUPMENU:
			printf("(POPUPMENU)");
			break;
		case WIDGET_MENUBAR:
			printf("(MENUBAR)");
			break;
		case WIDGET_MENUITEM:
			printf("(MENUITEM)");
			break;
		case WIDGET_MESSAGEDIALOG:
			printf("(MESSAGEDIALOG)");
			break;
		case WIDGET_MENUITEMSEPARATOR:
			printf("(MENUITEMSEPARATOR)");
			break;
		case WIDGET_NOTEBOOK:
			printf("(NOTEBOOK)");
			break;
		case WIDGET_PIXMAP:
			printf("(PIXMAP)");
			break;
		case WIDGET_PROGRESSBAR:
			printf("(PROGRESSBAR)");
			break;
		case WIDGET_RADIOBUTTON:
			printf("(RADIOBUTTON)");
			break;
		case WIDGET_RECENTCHOOSER:
			printf("(RECENTCHOOSER)");
			break;
		case WIDGET_RECENTCHOOSERMENU:
			printf("(RECENTCHOOSERMENU)");
			break;
		case WIDGET_STATUSICON:
			printf("(STATUSICON)");
			break;
		case WIDGET_SCALEBUTTON:
			printf("(SCALEBUTTON)");
			break;
		case WIDGET_SCROLLEDWINDOW:
			printf("(SCROLLEDWINDOW)");
			break;
		case WIDGET_HANDLEBOX:
			printf("(HANDLEBOX)");
			break;
		case WIDGET_SPINBUTTON:
			printf("(SPINBUTTON)");
			break;
		case WIDGET_STATUSBAR:
			printf("(STATUSBAR)");
			break;
		case WIDGET_TABLE:
			printf("(TABLE)");
			break;
		case WIDGET_TERMINAL:
			printf("(TERMINAL)");
			break;
		case WIDGET_TEXT:
			printf("(TEXT)");
			break;
		case WIDGET_TIMER:
			printf("(TIMER)");
			break;
		case WIDGET_TOGGLEBUTTON:
			printf("(TOGGLEBUTTON)");
			break;
		case WIDGET_TOOLITEM:
			printf("(TOOLITEM)");
			break;
		case WIDGET_TOOLPALETTE:
			printf("(TOOLPALETTE)");
			break;
		case WIDGET_TOOLITEMGROUP:
			printf("(TOOLITEMGROUP)");
			break;
		case WIDGET_TOGGLETOOLBUTTON:
			printf("(TOGGLETOOLBUTTON)");
			break;
		case WIDGET_RADIOTOOLBUTTON:
			printf("(RADIOTOOLBUTTON)");
			break;
		case WIDGET_MENUTOOLBUTTON:
			printf("(MENUTOOLBUTTON)");
			break;
		case WIDGET_CALENDAR:
			printf("(CALENDAR)");
			break;
		case WIDGET_LINKBUTTON:
			printf("(LINKBUTTON)");
			break;
		case WIDGET_SPINNER:
			printf("(SPINNER)");
			break;
		case WIDGET_TREE:
			printf("(TREE)");
			break;
		case WIDGET_VBOX:
			printf("(VBOX)");
			break;
		case WIDGET_VOLUMEBUTTON:
			printf("(VOLUMEBUTTON)");
			break;
		case WIDGET_VPANED:
			printf("(VPANED)");
			break;
		case WIDGET_VIEWPORT:
			printf("(VIEWPORT)");
			break;
		case WIDGET_VSCALE:
			printf("(VSCALE)");
			break;
		case WIDGET_VSCROLLBAR:
			printf("(VSCROLLBAR)");
			break;
		case WIDGET_VSEPARATOR:
			printf("(VSEPARATOR)");
			break;
		case WIDGET_WINDOW:
			printf("(WINDOW)");
			break;


	case WIDGET_SCROLLEDW:
		printf("(SCROLLEDW)");
		break;
	case WIDGET_GVIM:
		printf("(GVIM)");
		break;
	case WIDGET_CHOOSER:
		printf("(CHOOSER)");
		break;
	default:
		printf("Unknown Widget (%d)", Widget_Type);
	}
	
finalize:
	printf("\n");
	fflush(stdout);
}


static gboolean
imperative_parse_number(const gchar *text, gdouble *value)
{
	gchar *end;
	gdouble parsed;

	if (text == NULL || *text == '\0')
		return FALSE;
	errno = 0;
	parsed = g_ascii_strtod(text, &end);
	if (errno != 0 || end == text || *end != '\0' || !isfinite(parsed))
		return FALSE;

	*value = parsed;
	return TRUE;
}

static void
imperative_push_value(const gchar *value)
{
	stackelement element = { 0 };

	element.cval = g_strdup(value);
	push(element);
}

static void
imperative_push_number(gdouble value)
{
	gchar buffer[G_ASCII_DTOSTR_BUF_SIZE];

	g_ascii_dtostr(buffer, sizeof(buffer), value);
	imperative_push_value(buffer);
}

static gboolean
imperative_pop_number(gdouble *value)
{
	stackelement element = pop();
	gboolean valid = imperative_parse_number(element.cval, value);

	if (!valid)
		gtkdialog_warning("Invalid imperative numeric value '%s'; using 0.",
			element.cval != NULL ? element.cval : "");
	stackelement_clear(&element);
	if (!valid)
		*value = 0.0;
	return valid;
}

int 
instruction_execute(instruction command)
{
	GList *element;
	token Token;
	char *Argument;
	int Instr_Code;
	int Widget_Type;
	int Attribute_Number;
	static AttributeSet *Attr = NULL;

	Token = command.command;
	Argument = command.argument;

	Instr_Code = Token & COMMAND;
	Widget_Type = Token & WIDGET_TYPE;
	Attribute_Number = Token & ATTRIBUTE;


	PIP_DEBUG("Executing token %08x with argument '%s'.", Token, Argument);
	PIP_DEBUG("   inst:%08x, widgt:%08x, argn:%08x\n",
		Instr_Code, Widget_Type, Attribute_Number);
#ifdef DEBUG
	print_token(Token);
#endif
	
	if (Attr == NULL)
		Attr = attributeset_new();

	switch (Instr_Code) {
	case PUSH:
		/* These instructions will push widgets into the stack, some
		 ** of them will remove elements from it.
		 */
		instruction_execute_push(Token, Attr,
			tag_attributeset_copy(command.tag_attributes));
		variables_refresh(attributeset_get_first(&element, Attr, ATTR_VARIABLE));
		Attr = NULL;
		break;
	case SET:
		/* 
		 ** SET instructions stores strings in the attributes.
		 */
		attributeset_insert_with_tagattrs(Attr, 
				    Attribute_Number, 
				    Argument,
				    tag_attributeset_copy(command.tag_attributes));
		break;
	case SUM:
		/*
		 ** This instruction will add the to uppermost stack element and
		 ** push the result to the stack, so the upper element will 
		 ** contain all the widgets.
		 */
		{
			stackelement source = pop();
			stackelement destination = pop();

			stackelement_append(&destination, &source);
			push(destination);
		}
		break;
	case EMPTY_WIDGETS:
		{
			stackelement empty = { 0 };
			push(empty);
		}
		break;
	/*
	 * The imperative instructions.
	 */
	case IMASSG:
		{
		stackelement s;
		s = pop();
		if (Argument == NULL || s.cval == NULL)
			gtkdialog_warning("Invalid imperative assignment.");
		else if (!g_setenv(Argument, s.cval, TRUE))
			gtkdialog_warning("Couldn't assign imperative variable '%s'.",
				Argument);
		stackelement_clear(&s);
		}
		break;
	case IMPUSH:
		switch (Widget_Type) {
			case VARIABLE_NAME:
				{
				const gchar *value = g_getenv(Argument);

				if (value == NULL) {
					gtkdialog_warning("Undefined imperative variable '%s'; using 0.",
						Argument);
					value = "0";
				}
				imperative_push_value(value);
			    	}
			    break;
			case CONST_NUMBER:
				imperative_push_value(Argument);
			    break;
			case OP_ADD:
			case OP_SUBST:
			case OP_MULT:
			case OP_DIV:
			case REL_EQ:
			case REL_NE:
				{
				gdouble left;
				gdouble right;
				gdouble result = 0.0;

				imperative_pop_number(&right);
				imperative_pop_number(&left);
				switch (Widget_Type) {
				case OP_ADD:
					result = left + right;
					break;
				case OP_SUBST:
					result = left - right;
					break;
				case OP_MULT:
					result = left * right;
					break;
				case OP_DIV:
					if (right == 0.0)
						gtkdialog_warning("Division by zero in imperative expression; using 0.");
					else
						result = left / right;
					break;
				case REL_EQ:
					result = left == right;
					break;
				case REL_NE:
					result = left != right;
					break;
				}
				if (!isfinite(result)) {
					gtkdialog_warning("Non-finite imperative result; using 0.");
					result = 0.0;
				}
				imperative_push_number(result);
				}
				break;
			default:
				gtkdialog_warning("Unknown imperative operator: %d.",
					Widget_Type);
			}
		break;
	case SHOW:
		show_without_pop(window);
		break;
	case IFNGOTO:
		{
		gdouble condition;

		imperative_pop_number(&condition);
		if (condition == 0.0) {
			if (stack_depth() == 0) {
				stackelement empty = { 0 };
				push(empty);
			}
			return command.ival;
		}
		if (imperative_block_depths == NULL)
			g_error("Imperative condition executed outside a program.");
		{
			size_t depth = stack_depth();
			g_array_append_val(imperative_block_depths, depth);
		}
		}
		break;
	case GOTO:
		return command.ival;
	case IMMERGE:
		if (imperative_block_depths == NULL ||
			imperative_block_depths->len == 0)
			g_error("Imperative block stack underflow.");
		{
			guint last = imperative_block_depths->len - 1;
			size_t depth = g_array_index(imperative_block_depths,
				size_t, last);

			g_array_set_size(imperative_block_depths, last);
			stack_merge_since(depth);
		}
		break;
	case RGROUP_PUSH:
	{
		RadioGroupState state;

		if (radio_group_stack == NULL)
			g_error("Radio group scope entered outside a program.");
		state.tool_button = lasttoolradiowidget;
		g_array_append_val(radio_group_stack, state);
		lasttoolradiowidget = NULL;
		break;
	}
	case RGROUP_POP:
	{
		RadioGroupState state;

		if (radio_group_stack == NULL || radio_group_stack->len == 0)
			g_error("Radio group scope underflow.");
		state = g_array_index(radio_group_stack, RadioGroupState,
			radio_group_stack->len - 1);
		lasttoolradiowidget = state.tool_button;
		g_array_set_size(radio_group_stack,
			radio_group_stack->len - 1);
		break;
	}
	default:
		if (!option_no_warning)
			gtkdialog_warning("%s(): Unknown instruction.", __func__);
	}

	PIP_DEBUG("Instruction executed.");
	return -1;
}

/***********************************************************************
 * Put in the Scrolled Window                                          *
 ***********************************************************************/
/* Places the widget into a scrolled window */

static GtkWidget *put_in_the_scrolled_window(GtkWidget *widget,
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList            *element;
	GtkWidget        *parent;
	GtkWidget        *scrolledwindow;
	gchar            *value;
	gint              width = -1;
	gint              height = -1;
	gint              hscrollbar_policy = GTK_POLICY_AUTOMATIC;
	gint              vscrollbar_policy = GTK_POLICY_AUTOMATIC;

	g_assert(widget != NULL);

	/* Create the scrolled window */
	scrolledwindow = gtk_scrolled_window_new(NULL, NULL);

	/* Set the scrollbar policy for the scrollbars */
	/* Get scrollbar policy from custom tag attributes */
	if (attr) {
		if ((value = get_tag_attribute(attr, "hscrollbar-policy")))
			hscrollbar_policy = widget_parse_bounded_integer(value,
				GTK_POLICY_ALWAYS, GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC,
				"hscrollbar-policy");
		if ((value = get_tag_attribute(attr, "vscrollbar-policy")))
			vscrollbar_policy = widget_parse_bounded_integer(value,
				GTK_POLICY_ALWAYS, GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC,
				"vscrollbar-policy");
	}
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow), 
		hscrollbar_policy, vscrollbar_policy);

	/* Get dimensions from directives */
	if (attributeset_is_avail(Attr, ATTR_WIDTH))
		width = Type == WIDGET_TERMINAL
			? widget_parse_size(attributeset_get_first(&element, Attr,
				ATTR_WIDTH), "terminal width")
			: widget_parse_size_request(attributeset_get_first(&element, Attr,
				ATTR_WIDTH), "scrolled window width");
	if (attributeset_is_avail(Attr, ATTR_HEIGHT))
		height = Type == WIDGET_TERMINAL
			? widget_parse_size(attributeset_get_first(&element, Attr,
				ATTR_HEIGHT), "terminal height")
			: widget_parse_size_request(attributeset_get_first(&element, Attr,
				ATTR_HEIGHT), "scrolled window height");

	/* Set scrolled window size */
	switch (Type) {
		case WIDGET_HBOX:
		case WIDGET_VBOX:
		case WIDGET_TEXT:
			/* Get dimensions from custom tag attributes */
			if (attr) {
				if ((value = get_tag_attribute(attr, "width")))
					width = widget_parse_size_request(value, "scrolled box width");
				if ((value = get_tag_attribute(attr, "height")))
					height = widget_parse_size_request(value, "scrolled box height");
			}
			/* Set some defaults */
			if (width == -1) width = 200;
			if (height == -1) height = 100;
			/* Set the size */
			gtk_widget_set_size_request(scrolledwindow, width, height);
			/* Pack the widget */
			gtk_scrolled_window_add_with_viewport(
				GTK_SCROLLED_WINDOW(scrolledwindow), widget);
			break;
		case WIDGET_TERMINAL:
#if HAVE_VTE
			/* The terminal constructor applies the font before safely
			 * converting character dimensions to GTK pixel dimensions. */
			width = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
				"_gtkdialog-terminal-width"));
			height = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
				"_gtkdialog-terminal-height"));
			/* Set the size */
			gtk_widget_set_usize(scrolledwindow, width, height);
			/* Pack the widget */
			gtk_container_add(GTK_CONTAINER(scrolledwindow), widget);
			break;
#endif
		default:
			/* Set some defaults */
			if (width == -1) width = 200;
			if (height == -1) height = 100;
			/* Set the size */
			gtk_widget_set_size_request(scrolledwindow, width, height);
			/* Pack the widget */
			if (Type == WIDGET_LIST || Type == WIDGET_TERMINAL ||
				Type == WIDGET_IMAGEVIEW) {
				gtk_scrolled_window_add_with_viewport(
					GTK_SCROLLED_WINDOW(scrolledwindow), widget);
			} else {
				gtk_container_add(GTK_CONTAINER(scrolledwindow), widget);
			}
			break;
	}

	/* The default shadow-type for the scrolledwindow is GTK_SHADOW_NONE
	 * but for the viewport it's GTK_SHADOW_IN which should be modifiable */
	/* Get shadow-type */
	if (attr) {
		if ((value = get_tag_attribute(attr, "shadow-type"))) {
			parent = gtk_widget_get_parent(widget);
			if (GTK_IS_VIEWPORT(parent)) {
				gtk_viewport_set_shadow_type(GTK_VIEWPORT(parent),
					widget_parse_bounded_integer(value, GTK_SHADOW_NONE,
						GTK_SHADOW_ETCHED_OUT, GTK_SHADOW_IN, "shadow-type"));
			}
		}
	}
	return scrolledwindow;
}

#define GVIM_WINDOW_DATA "gtkdialog-gvim-window"

static void
gvim_position_window(GtkWidget *placeholder)
{
	GtkWidget *popup;
	gint x, y;

	popup = g_object_get_data(G_OBJECT(placeholder), GVIM_WINDOW_DATA);
	if (popup == NULL || !GTK_WIDGET_REALIZED(placeholder) ||
		placeholder->window == NULL)
		return;

	gdk_window_get_origin(placeholder->window, &x, &y);
	gtk_window_move(GTK_WINDOW(popup), x, y);
	gtk_window_resize(GTK_WINDOW(popup),
		MAX(1, placeholder->allocation.width),
		MAX(1, placeholder->allocation.height));
}

static gboolean
gvim_parent_configured(GtkWidget *parent, GdkEventConfigure *event,
	gpointer user_data)
{
	(void)parent;
	(void)event;
	gvim_position_window(GTK_WIDGET(user_data));
	return FALSE;
}

static void
gvim_placeholder_realized(GtkWidget *placeholder, gpointer user_data)
{
	GtkWidget *popup = GTK_WIDGET(user_data);
	GtkWidget *toplevel = gtk_widget_get_toplevel(placeholder);

	if (GTK_IS_WINDOW(toplevel))
		g_signal_connect_object(toplevel, "configure-event",
			G_CALLBACK(gvim_parent_configured), placeholder, 0);
	gvim_position_window(placeholder);
	gtk_widget_show(popup);
}

static void
gvim_placeholder_unrealized(GtkWidget *placeholder, gpointer user_data)
{
	(void)placeholder;
	gtk_widget_hide(GTK_WIDGET(user_data));
}

static void
gvim_placeholder_destroyed(GtkWidget *placeholder, gpointer user_data)
{
	GtkWidget *popup = GTK_WIDGET(user_data);

	g_object_set_data(G_OBJECT(placeholder), GVIM_WINDOW_DATA, NULL);
	gtk_widget_destroy(popup);
}

static GtkWidget *
create_gvim(AttributeSet * Attr)
{
	GtkWidget *Window = gtk_window_new(GTK_WINDOW_POPUP);
	GtkWidget *socket = gtk_socket_new();
	GtkWidget *fake = gtk_drawing_area_new();
	GdkNativeWindow socket_id_value;
	char socket_id[32];

	gtk_widget_set_events(fake, GDK_ALL_EVENTS_MASK);
	gtk_widget_set_extension_events (fake,
			GDK_EXTENSION_EVENTS_ALL);
	gtk_widget_set_size_request(Window, 350, 150);
	gtk_widget_set_size_request(fake, 350, 150);
	
	gtk_container_add (GTK_CONTAINER (Window), socket);
	gtk_widget_show (socket);
	gtk_widget_realize(Window);
	gtk_widget_realize (socket);
	socket_id_value = gtk_socket_get_id(GTK_SOCKET(socket));
	g_snprintf(socket_id, sizeof(socket_id), "%lu", (gulong)socket_id_value);
	attributeset_set_if_unset(Attr, ATTR_SOCKET, socket_id);

	g_object_set_data(G_OBJECT(fake), GVIM_WINDOW_DATA, Window);
	g_signal_connect(fake, "realize",
		G_CALLBACK(gvim_placeholder_realized), Window);
	g_signal_connect(fake, "unrealize",
		G_CALLBACK(gvim_placeholder_unrealized), Window);
	g_signal_connect(fake, "destroy",
		G_CALLBACK(gvim_placeholder_destroyed), Window);
	return fake;
}

static gboolean
attributeset_destroy_idle(gpointer data)
{
	AttributeSet *set = data;
	Attribute *attribute;
	GList *element;
	int index;

	if (set == NULL)
		return FALSE;

	for (index = 0; index < set->n_attr; ++index) {
		for (element = set->attr[index]; element != NULL;
			element = element->next) {
			attribute = element->data;
			g_free(attribute->text);
			tag_attributeset_free(attribute->tag_attributes);
			g_free(attribute);
		}
		g_list_free(set->attr[index]);
	}

	g_free(set->attr);
	g_free(set);
	return FALSE;
}

void
attributeset_destroy(gpointer data)
{
	/* An action can destroy its own window while its signal callback is still
	 * iterating over this set.  Release it when control returns to GTK. */
	g_idle_add(attributeset_destroy_idle, data);
}

static gboolean
tag_attributeset_destroy_idle(gpointer data)
{
	tag_attributeset_free(data);
	return FALSE;
}

static void
tag_attributeset_destroy(gpointer data)
{
	g_idle_add(tag_attributeset_destroy_idle, data);
}

static gint 
instruction_execute_push(
		token          Token, 
		AttributeSet  *Attr,
		tag_attr      *tag_attributes)
{
	GList            *element;
	GtkWidget        *scrolled_window = NULL;
	GtkWidget        *Widget = NULL;
	gchar            *value;
	gint              Widget_Type;

	PIP_DEBUG("token: %d", Token);
	
	Widget_Type = Token & WIDGET_TYPE;

	switch (Widget_Type) {
		case WIDGET_ABOUTDIALOG:
			Widget = window = widget_aboutdialog_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_ALIGNMENT:
			Widget = widget_alignment_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_ASSISTANT:
			Widget = window = widget_assistant_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_ASPECTFRAME:
			Widget = widget_aspectframe_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_CANCELBUTTON:
		case WIDGET_HELPBUTTON:
		case WIDGET_NOBUTTON:
		case WIDGET_OKBUTTON:
		case WIDGET_YESBUTTON:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_BUTTON:
			Widget = widget_button_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_CALENDAR:
			Widget = widget_calendar_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_CHECKBOX:
			Widget = widget_checkbox_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_COLORBUTTON:
			Widget = widget_colorbutton_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_COLORSELECTION:
			Widget = widget_colorselection_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_DIALOG:
			Widget = window = widget_dialog_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_COMBOBOX:
			Widget = widget_combobox_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
			Widget = widget_comboboxtext_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_EDIT:
			Widget = widget_edit_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);
			break;
		case WIDGET_ENTRY:
			Widget = widget_entry_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_EVENTBOX:
			Widget = widget_eventbox_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_EXPANDER:
			Widget = widget_expander_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_FILECHOOSERBUTTON:
			Widget = widget_filechooserbutton_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_FIXED:
			Widget = widget_fixed_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_FONTBUTTON:
			Widget = widget_fontbutton_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_FONTSELECTION:
			Widget = widget_fontselection_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_ACCELLABEL:
			Widget = widget_accellabel_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_ARROW:
			Widget = widget_arrow_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_FRAME:
			Widget = widget_frame_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_GRID:
			Widget = widget_grid_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_HBOX:
			Widget = widget_hbox_create(Attr, tag_attributes, Widget_Type);
			/* Thunor: If the custom attribute "scrollable" is true
			 * then place the hbox inside a GtkScrolledWindow */
			if (tag_attributes &&
				(value = get_tag_attribute(tag_attributes, "scrollable")) &&
				widget_attribute_is_true(value)) {
				scrolled_window = put_in_the_scrolled_window(Widget, Attr,
					tag_attributes, Widget_Type);
				push_widget(scrolled_window, WIDGET_SCROLLEDW);
			} else {
				push_widget(Widget, Widget_Type);
			}
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_HBUTTONBOX:
		case WIDGET_VBUTTONBOX:
			Widget = widget_buttonbox_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_HSV:
			Widget = widget_hsv_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_TASKLIST:
			Widget = widget_tasklist_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_PAGER:
			Widget = widget_pager_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_WINDOWSELECTOR:
			Widget = widget_windowselector_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_OFFSCREENWINDOW:
			Widget = window = widget_offscreenwindow_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_PAGESETUPDIALOG:
			Widget = window = widget_pagesetupdialog_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_PRINTDIALOG:
			Widget = window = widget_printdialog_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_FILECHOOSERDIALOG:
			Widget = window = widget_filechooserdialog_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_CURVE:
			Widget = widget_curve_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_HRULER:
		case WIDGET_VRULER:
			Widget = widget_ruler_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_HSCALE:
		case WIDGET_VSCALE:
			Widget = widget_hscale_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_HSCROLLBAR:
		case WIDGET_VSCROLLBAR:
			Widget = widget_scrollbar_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_HSEPARATOR:
		case WIDGET_VSEPARATOR:
			Widget = widget_hseparator_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_INFOBAR:
			Widget = widget_infobar_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_LAYOUT:
			Widget = widget_layout_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);
			break;
		case WIDGET_ICONVIEW:
			Widget = widget_iconview_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);
			break;
		case WIDGET_CELLVIEW:
			Widget = widget_cellview_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_DRAWINGAREA:
			Widget = widget_drawingarea_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_SHEET:
			Widget = widget_sheet_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);
			break;
		case WIDGET_DOCKITEM:
			Widget = widget_dockitem_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_DOCK:
			Widget = widget_dock_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_IMAGEVIEW:
			Widget = widget_imageview_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);
			break;
		case WIDGET_SOCKET:
			Widget = widget_socket_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_LIST:
			Widget = widget_list_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);		
			break;
		case WIDGET_LINKBUTTON:
			Widget = widget_linkbutton_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_MENU:
			Widget = widget_menu_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_POPUPMENU:
			Widget = widget_popupmenu_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			lastradiowidget = NULL;
			break;
		case WIDGET_MENUBAR:
			Widget = widget_menubar_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_MENUITEMSEPARATOR:
		case WIDGET_MENUITEM:
			Widget = widget_menuitem_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			/* Creating a non radiobutton menuitem widget closes any open group */
			if (!(GTK_IS_RADIO_MENU_ITEM(Widget))) lastradiowidget = NULL;
			break;
		case WIDGET_MESSAGEDIALOG:
			Widget = window = widget_messagedialog_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_NOTEBOOK:
			Widget = widget_notebook_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_HPANED:
		case WIDGET_VPANED:
			Widget = widget_paned_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_PIXMAP:
			Widget = widget_pixmap_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_PROGRESSBAR:
			Widget = widget_progressbar_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_RADIOBUTTON:
			Widget = widget_radiobutton_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_RECENTCHOOSER:
			Widget = widget_recentchooser_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_RECENTCHOOSERMENU:
			Widget = widget_recentchoosermenu_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			lastradiowidget = NULL;
			break;
		case WIDGET_STATUSICON:
			Widget = widget_statusicon_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_SCALEBUTTON:
		case WIDGET_VOLUMEBUTTON:
			Widget = widget_scalebutton_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_SCROLLEDWINDOW:
			Widget = widget_scrolledwindow_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_VIEWPORT:
			Widget = widget_viewport_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_HANDLEBOX:
			Widget = widget_handlebox_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_SPINBUTTON:
			Widget = widget_spinbutton_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_SPINNER:
			Widget = widget_spinner_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
			Widget = widget_toolitem_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_STATUSBAR:
			Widget = widget_statusbar_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_TABLE:
			Widget = widget_table_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);		
			break;
		case WIDGET_TERMINAL:
			Widget = widget_terminal_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);
			break;
		case WIDGET_TEXT:
			Widget = widget_text_create(Attr, tag_attributes, Widget_Type);
			if (tag_attributes &&
				(value = get_tag_attribute(tag_attributes, "scrollable")) &&
				widget_attribute_is_true(value)) {
				scrolled_window = put_in_the_scrolled_window(Widget, Attr,
					tag_attributes, Widget_Type);
				push_widget(scrolled_window, WIDGET_SCROLLEDW);
			} else {
				push_widget(Widget, Widget_Type);
			}
			break;
		case WIDGET_TIMER:
			Widget = widget_timer_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_TOOLBAR:
			Widget = widget_toolbar_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_TOOLITEMGROUP:
		case WIDGET_TOOLPALETTE:
			Widget = widget_toolpalette_create(
				Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;
		case WIDGET_TREE:
			Widget = widget_tree_create(Attr, tag_attributes, Widget_Type);
			scrolled_window = put_in_the_scrolled_window(Widget, Attr,
				tag_attributes, Widget_Type);
			push_widget(scrolled_window, WIDGET_SCROLLEDW);
			break;
		case WIDGET_VBOX:
			Widget = widget_vbox_create(Attr, tag_attributes, Widget_Type);
			/* Thunor: If the custom attribute "scrollable" is true
			 * then place the vbox inside a GtkScrolledWindow */
			if (tag_attributes &&
				(value = get_tag_attribute(tag_attributes, "scrollable")) &&
				widget_attribute_is_true(value)) {
				scrolled_window = put_in_the_scrolled_window(Widget, Attr,
					tag_attributes, Widget_Type);
				push_widget(scrolled_window, WIDGET_SCROLLEDW);
			} else {
				push_widget(Widget, Widget_Type);
			}
			/* Creating this widget historically closes any open group. */
			lastradiowidget = NULL;
			break;
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			Widget = window = widget_window_create(Attr, tag_attributes, Widget_Type);
			push_widget(Widget, Widget_Type);
			break;



	case WIDGET_CHOOSER:
		Widget = widget_chooser_create(Attr, tag_attributes, Widget_Type);
		push_widget(Widget, Widget_Type);
		break;
		
	case WIDGET_GVIM:
		Widget = create_gvim(Attr);
		push_widget(Widget, Widget_Type);
		break;
		
	default:
		if (!option_no_warning)
		gtkdialog_warning("%s(): Unknown widget type.", __func__);
	}
	
	PIP_DEBUG("Widget created: %p", Widget);
	size_groups_add_widget(
		scrolled_window != NULL ? scrolled_window : Widget, tag_attributes);
	
	/*
	 ** Now we create a new variable or refresh the old one for this
	 ** new widget.
	 */
	variables_new_with_widget(Attr, tag_attributes, Widget, Widget_Type);

	variables_set_attributes(attributeset_get_first(&element, Attr,
		ATTR_VARIABLE), Attr);

	/* Thunor: At this point Widget and possibly scrolled_window will
	 * be non-NULL and they require registering in a list (actually two)
	 * for later hiding or showing in run_program() */
	widget_visibility_list_add(Widget, tag_attributes);
	widget_visibility_list_add(scrolled_window, tag_attributes);

	/* Popup menus and tool items need layout properties before their parent
	 * shell or toolbar computes its initial requisition. */
	switch (Widget_Type) {
		case WIDGET_MENU:
		case WIDGET_POPUPMENU:
		case WIDGET_MENUITEM:
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_MENUTOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
		case WIDGET_DOCKITEM:
			widget_set_tag_attributes(Widget, tag_attributes);
			break;
		default:
			break;
	}

	/*
	 * This is where we set the properties and connect signals 
	 * for the widget.
	 */
	//widget_set_tag_attributes(Widget, tag_attributes);
	g_signal_connect(G_OBJECT(Widget), "realize",
		G_CALLBACK(on_any_widget_realized), (gpointer)tag_attributes);

	/* Thunor: This is in addition to the widget specific signals that
	 * are generally connected within each widget's refresh function at
	 * start-up. */
	widget_connect_signals(Widget, Attr);

	/* Signal callbacks and variable refreshes borrow both descriptions for
	 * the complete widget lifetime.  Let the widget release the parser-owned
	 * data after those callbacks can no longer run. */
	g_object_set_data_full(G_OBJECT(Widget), "gtkdialog-attributes", Attr,
		attributeset_destroy);
	g_object_set_data_full(G_OBJECT(Widget), "gtkdialog-tag-attributes",
		tag_attributes, tag_attributeset_destroy);

	PIP_DEBUG("Variable created.");

	return 0;
}


void _more_memory_needed(void)
{
	void *memory = NULL;

	if (program == NULL) {
		memory_counter = 128;
		memory = malloc(memory_counter * sizeof(instruction));
	} else {
		if (memory_counter > G_MAXINT / 2 ||
			memory_counter > G_MAXSIZE / 2 / sizeof(instruction)) {
			fprintf(stderr, "%s: instruction memory size overflow.\n", __func__);
			exit(EXIT_FAILURE);
		}
		memory_counter *= 2;
		memory =
		    realloc(program, memory_counter * sizeof(instruction));
	}

	if (memory == NULL) {
		fprintf(stderr, "%s: not enought memory.\n", __func__);
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	fprintf(stderr, "%s: Memory allocated: %zu.\n",
		__func__, memory_counter);
#endif

	program = memory;
}

void 
instruction_new(instruction new)
{
	PIP_DEBUG("instruction_counter=%d, memory_counter=%zu",
			instruction_counter, memory_counter);
	
	if ((size_t)instruction_counter == memory_counter)
		_more_memory_needed();

	program[instruction_counter++] = new;
}

gint
instruction_get_pc(void)
{
	return instruction_counter - 1;
}

void
instruction_set_jump(gint from, gint where)
{
	if (from < 0 || (size_t)from >= memory_counter)
		g_error("Jump from a nonexistent instruction memory location.");
	program[from].ival = where;
}

/* 
** Functions to store a token in the memory 
*/
int token_store(token command)
{
	instruction inst = {0};
	
	inst.command = command;
	inst.argument = NULL;
	inst.tag_attributes = NULL;
	
	instruction_new(inst);
	return TRUE;
}

int 
token_store_attr(token command, 
		tag_attr *attributes)
{
	gint             count;
	gint             index;
	instruction      inst = {0};

	/* Thunor: I've added this to convert underscores within tag
	 * attributes to hyphens so that we can deal exclusively in
	 * hyphenated strings throughout the remainder of this program */
	for (count = 0; count < attributes->n; count++) {
#ifdef DEBUG
		fprintf(stderr, "%s: Before: pairs[%i].name=%s  pairs[%i].value=%s\n",
			__func__, count, attributes->pairs[count].name,
			count, attributes->pairs[count].value);
#endif
		index = 0;
		while (attributes->pairs[count].name[index]) {
			if (attributes->pairs[count].name[index] == '_') {
				attributes->pairs[count].name[index] = '-';
			}
			index++;
		}
#ifdef DEBUG
		fprintf(stderr, "%s:  After: pairs[%i].name=%s  pairs[%i].value=%s\n",
			__func__, count, attributes->pairs[count].name,
			count, attributes->pairs[count].value);
#endif
	}

	inst.command = command;
	inst.argument = NULL;
	inst.tag_attributes = attributes;
	instruction_new(inst);

	return TRUE;
}

int 
token_store_with_argument_attr(
		token command, 
		const char *argument, 
		tag_attr *attributes)
{
	instruction       inst = {0};
	gint              sub_attribute;
	gint              count, index;

	PIP_DEBUG("Start argument='%s' attributes: %p", argument, attributes);

	/* Thunor: I've added this to convert underscores to hyphens within
	 * signal names declared by the user so that we can deal exclusively
	 * in hyphenated strings throughout the remainder of this program */
	for (count = 0; count < attributes->n; count++) {
		if (g_ascii_strcasecmp(attributes->pairs[count].name, "signal") == 0) {
#ifdef DEBUG
			fprintf(stderr, "%s: Before: pairs[%i].name=%s  pairs[%i].value=%s\n",
				__func__, count, attributes->pairs[count].name,
				count, attributes->pairs[count].value);
#endif
			index = 0;
			while (attributes->pairs[count].value[index]) {
				if (attributes->pairs[count].value[index] == '_')
					attributes->pairs[count].value[index] = '-';
				index++;
			}
#ifdef DEBUG
			fprintf(stderr, "%s:  After: pairs[%i].name=%s  pairs[%i].value=%s\n",
				__func__, count, attributes->pairs[count].name,
				count, attributes->pairs[count].value);
#endif
		}
	}

	inst.tag_attributes = attributes;

	if (argument == NULL)
		argument = "";

	sub_attribute = (command & SUB_ATTRIBUTE) >> 24;
	switch (sub_attribute) {
	case 0:
		/* 
		 ** No sub_attribute, it is just a normal text.
		 */
		inst.argument = g_strdup(argument);
		break;
	case 1:
		/*
		 ** The text argument is a shell command.
		 */
		inst.argument = g_strdup_printf("Command:%s", argument);
		break;
	case 2:
		/* 
		 ** The text argument is a file name.
		 */
		inst.argument = g_strdup_printf("File:%s", argument);
		break;
	case 3:
		/* 
		 ** The text argument is a widget name which must be
		 ** cleared.
		 */
		inst.argument = g_strdup_printf("Clear:%s", argument);
		break;
	case 4:
		/*
		**
		*/
		inst.argument = g_strdup_printf("Append:%s", argument);
		break;
	case 10:
		/* 
		 ** The text argument is widget name which must be filled with
		 ** a file name from a file selection dialog.
		 */
		inst.argument = g_strdup_printf("Fileselect:%s", argument);
		break;
	case 11:
		/* 
		 ** The text argument is widget name and we have to remove the
		 ** selected element from it.
		 */
		inst.argument = g_strdup_printf("Removeselected:%s", argument);
		break;
	case 12:
		/* 
		 ** The text argument is widget name and we have to remove the
		 ** selected element from it.
		 */
		inst.argument = g_strdup_printf("Refresh:%s", argument);
		break;
	case 13:
		/* 
		 ** The text argument is widget name and we have to
		 ** remove the selected element from it.
		 **  
		 */
		inst.argument = g_strdup_printf("Launch:%s", argument);
		break;
	default:
		fprintf(stderr, "%s(): Warning: Unknown sub-attribute %d",
			__func__, sub_attribute);
		fflush(stderr);
		inst.argument = g_strdup(argument);
	}
	inst.command = command;
	instruction_new(inst);
	return (0);
	
}

int token_store_with_argument(
		token command, 
	const char *argument)
{
	tag_attr *attributes = new_tag_attributeset(
			"text",
			(gchar *)argument);
	
	return token_store_with_argument_attr(
			command, 
			argument,
			attributes);
}
