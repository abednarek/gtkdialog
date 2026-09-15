/*
 * variables.c:
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

#include "config.h"
#include "gtkdialog.h"
#include "variables.h"
#include "widgets.h"
#include "widget_accellabel.h"
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
#include "tag_attributes.h"
#if HAVE_SYS_INOTIFY_H
#include <sys/inotify.h>
#endif

extern gboolean option_no_warning;

/* Local function prototypes */
#ifdef DEBUG
void variables_print_one(variable *var);
#endif
variable *variables_new(const char *name);
gboolean variables_is_avail_by_name(const char *name);
int _tree_insert(variable *new, variable *actual);
static variable *_tree_find(const char *name, variable *actual);
static void _variables_initialize(variable *actual);
static void _variables_export(variable * actual);
static gboolean variable_name_is_shell_identifier(const gchar *name);
#ifdef DEBUG
void variables_print_debug(variable *actual);
#endif

typedef struct {
	gchar *name;
	GPtrArray *instances;
} VariableBucket;

static GTree *variables_by_name = NULL;
static GHashTable *variables_by_widget = NULL;

static void variable_bucket_free(gpointer data)
{
	VariableBucket *bucket = data;
	guint index;

	for (index = 0; index < bucket->instances->len; ++index) {
		variable *var = g_ptr_array_index(bucket->instances, index);

		g_free(var->Name);
		g_free(var);
	}
	g_ptr_array_free(bucket->instances, TRUE);
	g_free(bucket);
}

static gint variable_name_compare(gconstpointer left, gconstpointer right,
	gpointer data)
{
	(void)data;
	return strcmp(left, right);
}

static void variables_registry_ensure(void)
{
	if (variables_by_name == NULL)
		variables_by_name = g_tree_new_full(variable_name_compare, NULL,
			NULL, variable_bucket_free);
	if (variables_by_widget == NULL)
		variables_by_widget = g_hash_table_new(g_direct_hash, g_direct_equal);
}

void variables_free_all(void)
{
	if (variables_by_widget != NULL) {
		g_hash_table_destroy(variables_by_widget);
		variables_by_widget = NULL;
	}
	if (variables_by_name != NULL) {
		g_tree_destroy(variables_by_name);
		variables_by_name = NULL;
	}
}

static VariableBucket *variable_bucket_get(const gchar *name)
{
	variables_registry_ensure();
	return g_tree_lookup(variables_by_name, name);
}

static variable *variable_bucket_current(VariableBucket *bucket)
{
	gint index;
	variable *var;

	if (bucket == NULL || bucket->instances->len == 0)
		return NULL;

	/* Name-based actions retain the historical "last declaration wins"
	 * behaviour, but closing that widget now reveals the previous active
	 * declaration instead of leaving it orphaned. */
	for (index = (gint)bucket->instances->len - 1; index >= 0; --index) {
		var = g_ptr_array_index(bucket->instances, index);
		if (var->Widget != NULL)
			return var;
	}

	return g_ptr_array_index(bucket->instances, bucket->instances->len - 1);
}

static gboolean variable_bucket_has_active_widget(VariableBucket *bucket)
{
	guint index;

	for (index = 0; index < bucket->instances->len; ++index) {
		variable *var = g_ptr_array_index(bucket->instances, index);

		if (var->Widget != NULL)
			return TRUE;
	}
	return FALSE;
}

static variable *variable_new_instance(const gchar *name)
{
	variable *new;

	new = g_new0(variable, 1);
	new->Name = g_strdup(name);
	new->row = -1;
	new->column = -1;
	new->ncolumns = -1;

	return new;
}

static gboolean variable_name_is_shell_identifier(const gchar *name)
{
	const guchar *cursor = (const guchar *)name;

	if (cursor == NULL ||
		!(g_ascii_isalpha(*cursor) || *cursor == '_'))
		return FALSE;

	for (++cursor; *cursor != '\0'; ++cursor)
		if (!(g_ascii_isalnum(*cursor) || *cursor == '_'))
			return FALSE;

	return TRUE;
}

gchar *variables_shell_escape(const gchar *value)
{
	const gchar *cursor;
	GString *escaped;

	if (value == NULL)
		return g_strdup("");

	escaped = g_string_sized_new(strlen(value));
	for (cursor = value; *cursor != '\0'; ++cursor) {
		if (*cursor == '\\' || *cursor == '"' ||
			*cursor == '$' || *cursor == '`')
			g_string_append_c(escaped, '\\');
		g_string_append_c(escaped, *cursor);
	}

	return g_string_free(escaped, FALSE);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

#ifdef DEBUG
void variables_print_one(variable *var)
{
	fprintf(stderr, "Name: %s\n", var->Name);
	fprintf(stderr, "  Widget: %p\n", var->Widget);
	fprintf(stderr, "  Type: %s\n", widgets_to_str(var->Type));
	fprintf(stderr, "  Window ID: %d\n", var->window_id);
	fflush(stderr);
}
#endif

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function will create a new variable */

variable *variables_new(const char *name)
{
	variable *new;

#ifdef DEBUG
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* 
	 ** If the variable exists we simply returns without making a 
	 ** a warning. 
	 */
	if (variables_is_avail_by_name(name))
		return (variables_get_by_name(name));

	new = variable_new_instance(name);
	_tree_insert(new, NULL);

#ifdef DEBUG
	fprintf(stderr, "%s(): Name=%s\n", __func__, new->Name);
#endif

#ifdef DEBUG
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return new;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function creates a variable mapping for a widget. Duplicate names
 * are intentionally supported because launched windows can reuse names.
 * Each live widget receives its own variable instance; name-based actions
 * address the most recently declared active instance. */

variable *variables_new_with_widget(AttributeSet *Attr,
	tag_attr *widget_tag_attr, GtkWidget *widget, int type)
{
	GList *element;
	gchar *default_name;
	char *name;
	variable *var;
	VariableBucket *bucket;
	gint index;
	int autonamed = FALSE;

#ifdef DEBUG
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	g_assert(Attr != NULL);
	g_assert(widget != NULL);

	/*
	 ** If the widget have no name we create a default one for it.
	 */
	if (!attributeset_is_avail(Attr, ATTR_VARIABLE)) {
		default_name = str_default_name(type);
		attributeset_insert(Attr,
				    ATTR_VARIABLE,
				    default_name);
		g_free(default_name);
		autonamed = TRUE;
	}

	name = attributeset_get_first(&element, Attr, ATTR_VARIABLE);

	bucket = variable_bucket_get(name);
	var = NULL;

	/* Reuse an inactive instance when a launched window is recreated. */
	if (bucket != NULL) {
		for (index = (gint)bucket->instances->len - 1; index >= 0; --index) {
			var = g_ptr_array_index(bucket->instances, index);
			if (var->Widget == NULL)
				break;
			var = NULL;
		}
	}

	if (var == NULL) {
		if (bucket == NULL)
			var = variables_new(name);
		else {
			var = variable_new_instance(name);
			g_ptr_array_add(bucket->instances, var);
		}
	} else if ((guint)index + 1 != bucket->instances->len) {
		/* Keep the most recently bound instance at the end, where name
		 * lookup expects it. */
		g_ptr_array_remove_index(bucket->instances, index);
		g_ptr_array_add(bucket->instances, var);
	}

	if (bucket == NULL) {
		bucket = variable_bucket_get(name);
		g_assert(bucket != NULL);
	}

	g_assert(var != NULL);

	var->Widget = widget;
	var->window_id = window_id;
	var->Type = type;
	var->Attributes = Attr;
	var->widget_tag_attr = widget_tag_attr;
	var->autonamed = autonamed;
	g_hash_table_insert(variables_by_widget, widget, var);
	/* Action sequences can destroy their originating widget's window before
	 * a following launch action checks the historical timer restriction. Keep
	 * the immutable type with the referenced GTK object after registry drop. */
	g_object_set_data(G_OBJECT(widget), "gtkdialog-widget-type",
		GINT_TO_POINTER(type));

#ifdef DEBUG
	fprintf(stderr, "%s(): Name=%s Widget=%p window_id=%i Type=%i\n",
		__func__, var->Name, var->Widget, var->window_id, var->Type);
#endif

#ifdef DEBUG
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return (var);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *variables_set_attributes(const char *name, AttributeSet *a)
{
	variable *var;
#ifdef DEBUG
	g_message("%s(): start '%s'", __func__, name);
#endif
	var = variables_get_by_name(name);
	if (var == NULL)
		var = variables_new(name);

	var->Attributes = a;
	return (var);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *variables_set_row_column(const char *name, int row, int column)
{
	variable *toset;
#ifdef DEBUG
	fprintf(stderr, "%s(): variable: %s\n", __func__, name);
	fflush(stderr);
#endif
	toset = _tree_find(name, NULL);
	if (toset == NULL)
		return (NULL);

	toset->row = row;
	toset->column = column;
	return (toset);
}

static variable *variables_get_imageview(const gchar *name,
	const gchar *function)
{
	variable *var = variables_get_by_name(name);

	if (var == NULL || var->Widget == NULL)
		return NULL;
	if (var->Type != WIDGET_IMAGEVIEW) {
		gtkdialog_warning("%s requires an imageview variable; '%s' is %s.",
			function, name, widgets_to_str(var->Type));
		return NULL;
	}
	return var;
}

static variable *variables_get_zoomable(const gchar *name,
	const gchar *function)
{
	variable *var = variables_get_by_name(name);

	if (var == NULL || var->Widget == NULL)
		return NULL;
	if (var->Type != WIDGET_IMAGEVIEW &&
		!(var->Type == WIDGET_DRAWINGAREA &&
		(widget_drawingarea_is_databox(var->Widget) ||
		widget_drawingarea_is_canvas(var->Widget)))) {
		gtkdialog_warning(
			"%s requires an imageview, GooCanvas or GtkDatabox drawingarea variable; '%s' is %s.",
			function, name, widgets_to_str(var->Type));
		return NULL;
	}
	return var;
}

void variables_imageview_fit(const char *name)
{
	variable *var = variables_get_imageview(name, "fit");

	if (var != NULL)
		widget_imageview_fit(var);
}

void variables_imageview_fit_height(const char *name)
{
	variable *var = variables_get_imageview(name, "fitheight");

	if (var != NULL)
		widget_imageview_fit_height(var);
}

void variables_imageview_fit_width(const char *name)
{
	variable *var = variables_get_imageview(name, "fitwidth");

	if (var != NULL)
		widget_imageview_fit_width(var);
}

void variables_imageview_zoom_in(const char *name)
{
	variable *var = variables_get_zoomable(name, "zoomin");

	if (var == NULL)
		return;
	if (var->Type == WIDGET_IMAGEVIEW)
		widget_imageview_zoom_in(var);
	else
		widget_drawingarea_zoom_in(var);
}

void variables_imageview_zoom_out(const char *name)
{
	variable *var = variables_get_zoomable(name, "zoomout");

	if (var == NULL)
		return;
	if (var->Type == WIDGET_IMAGEVIEW)
		widget_imageview_zoom_out(var);
	else
		widget_drawingarea_zoom_out(var);
}

void variables_imageview_zoom_reset(const char *name)
{
	variable *var = variables_get_zoomable(name, "zoomreset");

	if (var == NULL)
		return;
	if (var->Type == WIDGET_IMAGEVIEW)
		widget_imageview_zoom_reset(var);
	else
		widget_drawingarea_zoom_reset(var);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function will set the value of the given variable. This means
 * that it will set a widget's data or insert data to the widget as the
 * type of the widget makes clear */

variable *variables_set_value(const char *name, const char *value)
{
	variable         *toset;
	gchar            *string;

#ifdef DEBUG
	fprintf(stderr, "%s(): variable '%s'.\n", __func__,
		name != NULL ? name : "(null)");
	fflush(stderr);
#endif
	g_assert(name != NULL && value != NULL);
	
	toset = _tree_find(name, NULL);

	if (toset == NULL || toset->Widget == NULL)
		return (NULL);

	/* If the custom attribute "block-function-signals" is true
	 * then block signals whilst performing this function */
	if (toset->widget_tag_attr &&
		((string = get_tag_attribute(toset->widget_tag_attr, "block-function-signals"))) &&
		widget_attribute_is_true(string)) {
		GTKD_FUNCTION_SIGNALS_BLOCK;
	}

	switch (toset->Type) {
		case WIDGET_ABOUTDIALOG:
			widget_dialog_fileselect(toset, name, value);
			break;
		case WIDGET_ALIGNMENT:
			widget_alignment_fileselect(toset, name, value);
			break;
		case WIDGET_ASPECTFRAME:
			widget_aspectframe_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_ASSISTANT:
			widget_assistant_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOOLBAR:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
			widget_toolbar_fileselect(toset, name, value);
			break;
		case WIDGET_CALENDAR:
			widget_calendar_fileselect(toset, name, value);
			break;
		case WIDGET_ACCELLABEL:
			widget_accellabel_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_CELLVIEW:
			widget_cellview_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_HBUTTONBOX:
		case WIDGET_VBUTTONBOX:
			widget_buttonbox_fileselect(toset, name, value);
			break;
		case WIDGET_CANCELBUTTON:
		case WIDGET_HELPBUTTON:
		case WIDGET_NOBUTTON:
		case WIDGET_OKBUTTON:
		case WIDGET_YESBUTTON:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_BUTTON:
			widget_button_fileselect(toset, name, value);
			break;
		case WIDGET_CHECKBOX:
			widget_checkbox_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_CHOOSER:
			widget_chooser_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_COLORBUTTON:
			widget_colorbutton_fileselect(toset, name, value);
			break;
		case WIDGET_COLORSELECTION:
			widget_colorselection_fileselect(toset, name, value);
			break;
		case WIDGET_HSV:
			widget_hsv_fileselect(toset, name, value);
			break;
		case WIDGET_TASKLIST:
			widget_tasklist_fileselect(toset, name, value);
			break;
		case WIDGET_PAGER:
			widget_pager_fileselect(toset, name, value);
			break;
		case WIDGET_WINDOWSELECTOR:
			widget_windowselector_fileselect(toset, name, value);
			break;
		case WIDGET_PAGESETUPDIALOG:
			widget_pagesetupdialog_fileselect(toset, name, value);
			break;
		case WIDGET_PRINTDIALOG:
			widget_printdialog_fileselect(toset, name, value);
			break;
		case WIDGET_FILECHOOSERDIALOG:
			widget_filechooserdialog_fileselect(toset, name, value);
			break;
		case WIDGET_CURVE:
			widget_curve_fileselect(toset, name, value);
			break;
		case WIDGET_HRULER:
		case WIDGET_VRULER:
			widget_ruler_fileselect(toset, name, value);
			break;
		case WIDGET_DIALOG:
			widget_dialog_fileselect(toset, name, value);
			break;
		case WIDGET_DRAWINGAREA:
			widget_drawingarea_fileselect(toset, name, value);
			break;
		case WIDGET_SHEET:
			widget_sheet_fileselect(toset, name, value);
			break;
		case WIDGET_DOCK:
		case WIDGET_DOCKITEM:
			widget_dock_fileselect(toset, name, value);
			break;
		case WIDGET_IMAGEVIEW:
			widget_imageview_fileselect(toset, name, value);
			break;
		case WIDGET_COMBOBOX:
			widget_combobox_fileselect(toset, name, value);
			break;
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
			widget_comboboxtext_fileselect(toset, name, value);
			break;
		case WIDGET_EDIT:
			widget_edit_fileselect(toset, name, value);
			break;
		case WIDGET_ENTRY:
			widget_entry_fileselect(toset, name, value);
			break;
		case WIDGET_EVENTBOX:
			widget_eventbox_fileselect(toset, name, value);
			break;
		case WIDGET_EXPANDER:
			widget_expander_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_FILECHOOSERBUTTON:
			widget_filechooserbutton_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_FIXED:
			widget_fixed_fileselect(toset, name, value);
			break;
		case WIDGET_FONTBUTTON:
			widget_fontbutton_fileselect(toset, name, value);
			break;
		case WIDGET_FONTSELECTION:
			widget_fontselection_fileselect(toset, name, value);
			break;
		case WIDGET_FRAME:
			widget_frame_fileselect(toset, name, value);
			break;
		case WIDGET_GRID:
			widget_grid_fileselect(toset, name, value);
			break;
		case WIDGET_HBOX:
			widget_hbox_fileselect(toset, name, value);
			break;
		case WIDGET_HPANED:
		case WIDGET_VPANED:
			widget_paned_fileselect(toset, name, value);
			break;
		case WIDGET_HSCALE:
		case WIDGET_VSCALE:
			widget_hscale_fileselect(toset, name, value);
			break;
		case WIDGET_HSCROLLBAR:
		case WIDGET_VSCROLLBAR:
			widget_scrollbar_fileselect(toset, name, value);
			break;
		case WIDGET_HSEPARATOR:
		case WIDGET_VSEPARATOR:
			widget_hseparator_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_ICONVIEW:
			widget_iconview_fileselect(toset, name, value);
			break;
#endif
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_INFOBAR:
			widget_infobar_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_LAYOUT:
			widget_layout_fileselect(toset, name, value);
			break;
		case WIDGET_LIST:
			widget_list_fileselect(toset, name, value);
			break;
		case WIDGET_MESSAGEDIALOG:
			widget_messagedialog_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_LINKBUTTON:
			widget_linkbutton_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_MENUBAR:
		case WIDGET_POPUPMENU:
			widget_menubar_fileselect(toset, name, value);
			break;
		case WIDGET_MENUITEMSEPARATOR:
		case WIDGET_MENUITEM:
		case WIDGET_MENU:
			widget_menuitem_fileselect(toset, name, value);
			break;
		case WIDGET_NOTEBOOK:
			widget_notebook_fileselect(toset, name, value);
			break;
		case WIDGET_PIXMAP:
			widget_pixmap_fileselect(toset, name, value);
			break;
		case WIDGET_PROGRESSBAR:
			widget_progressbar_fileselect(toset, name, value);
			break;
		case WIDGET_RADIOBUTTON:
			widget_radiobutton_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_RECENTCHOOSER:
			widget_recentchooser_fileselect(toset, name, value);
			break;
#endif
#if GTK_CHECK_VERSION(2,12,0)
		case WIDGET_SCALEBUTTON:
		case WIDGET_VOLUMEBUTTON:
			widget_scalebutton_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_SCROLLEDWINDOW:
			widget_scrolledwindow_fileselect(toset, name, value);
			break;
		case WIDGET_VIEWPORT:
			widget_viewport_fileselect(toset, name, value);
			break;
		case WIDGET_HANDLEBOX:
			widget_handlebox_fileselect(toset, name, value);
			break;
		case WIDGET_TOOLPALETTE:
		case WIDGET_TOOLITEMGROUP:
			widget_toolpalette_fileselect(toset, name, value);
			break;
		case WIDGET_SPINBUTTON:
			widget_spinbutton_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,20,0)
		case WIDGET_SPINNER:
			widget_spinner_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_STATUSBAR:
			widget_statusbar_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_STATUSICON:
			widget_statusicon_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_TABLE:
			widget_table_fileselect(toset, name, value);
			break;
		case WIDGET_TERMINAL:
			widget_terminal_fileselect(toset, name, value);
			break;
		case WIDGET_TEXT:
			widget_text_fileselect(toset, name, value);
			break;
		case WIDGET_TIMER:
			widget_timer_fileselect(toset, name, value);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_TREE:
			widget_tree_fileselect(toset, name, value);
			break;
#endif
		case WIDGET_VBOX:
			widget_vbox_fileselect(toset, name, value);
			break;
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			widget_window_fileselect(toset, name, value);
			break;
		default:
			yywarning("Set-value not implemented for this widget.");
	}

	GTKD_FUNCTION_SIGNALS_RESET;

	return (toset);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *variables_save(const char *name)
{
	variable *var;
	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);

	if (var->Widget == NULL)
		return (NULL);

	switch (var->Type) {
		case WIDGET_ABOUTDIALOG:
			widget_dialog_save(var);
			break;
		case WIDGET_ALIGNMENT:
			widget_alignment_save(var);
			break;
		case WIDGET_ASPECTFRAME:
			widget_aspectframe_save(var);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_ASSISTANT:
			widget_assistant_save(var);
			break;
#endif
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOOLBAR:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
			widget_toolbar_save(var);
			break;
		case WIDGET_CALENDAR:
			widget_calendar_save(var);
			break;
		case WIDGET_ACCELLABEL:
			widget_accellabel_save(var);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_CELLVIEW:
			widget_cellview_save(var);
			break;
#endif
		case WIDGET_HBUTTONBOX:
		case WIDGET_VBUTTONBOX:
			widget_buttonbox_save(var);
			break;
		case WIDGET_CANCELBUTTON:
		case WIDGET_HELPBUTTON:
		case WIDGET_NOBUTTON:
		case WIDGET_OKBUTTON:
		case WIDGET_YESBUTTON:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_BUTTON:
			widget_button_save(var);
			break;
		case WIDGET_CHECKBOX:
			widget_checkbox_save(var);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_CHOOSER:
			widget_chooser_save(var);
			break;
#endif
		case WIDGET_COLORBUTTON:
			widget_colorbutton_save(var);
			break;
		case WIDGET_COLORSELECTION:
			widget_colorselection_save(var);
			break;
		case WIDGET_HSV:
			widget_hsv_save(var);
			break;
		case WIDGET_TASKLIST:
			widget_tasklist_save(var);
			break;
		case WIDGET_PAGER:
			widget_pager_save(var);
			break;
		case WIDGET_WINDOWSELECTOR:
			widget_windowselector_save(var);
			break;
		case WIDGET_OFFSCREENWINDOW:
			widget_offscreenwindow_save(var);
			break;
		case WIDGET_PAGESETUPDIALOG:
			widget_pagesetupdialog_save(var);
			break;
		case WIDGET_PRINTDIALOG:
			widget_printdialog_save(var);
			break;
		case WIDGET_FILECHOOSERDIALOG:
			widget_filechooserdialog_save(var);
			break;
		case WIDGET_CURVE:
			widget_curve_save(var);
			break;
		case WIDGET_HRULER:
		case WIDGET_VRULER:
			widget_ruler_save(var);
			break;
		case WIDGET_DIALOG:
			widget_dialog_save(var);
			break;
		case WIDGET_DRAWINGAREA:
			widget_drawingarea_save(var);
			break;
		case WIDGET_SHEET:
			widget_sheet_save(var);
			break;
		case WIDGET_DOCK:
		case WIDGET_DOCKITEM:
			widget_dock_save(var);
			break;
		case WIDGET_IMAGEVIEW:
			widget_imageview_save(var);
			break;
		case WIDGET_COMBOBOX:
			widget_combobox_save(var);
			break;
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
			widget_comboboxtext_save(var);
			break;
		case WIDGET_EDIT:
			widget_edit_save(var);
			break;
		case WIDGET_ENTRY:
			widget_entry_save(var);
			break;
		case WIDGET_EVENTBOX:
			widget_eventbox_save(var);
			break;
		case WIDGET_EXPANDER:
			widget_expander_save(var);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_FILECHOOSERBUTTON:
			widget_filechooserbutton_save(var);
			break;
#endif
		case WIDGET_FIXED:
			widget_fixed_save(var);
			break;
		case WIDGET_FONTBUTTON:
			widget_fontbutton_save(var);
			break;
		case WIDGET_FONTSELECTION:
			widget_fontselection_save(var);
			break;
		case WIDGET_FRAME:
			widget_frame_save(var);
			break;
		case WIDGET_GRID:
			widget_grid_save(var);
			break;
		case WIDGET_HBOX:
			widget_hbox_save(var);
			break;
		case WIDGET_HPANED:
		case WIDGET_VPANED:
			widget_paned_save(var);
			break;
		case WIDGET_HSCALE:
		case WIDGET_VSCALE:
			widget_hscale_save(var);
			break;
		case WIDGET_HSCROLLBAR:
		case WIDGET_VSCROLLBAR:
			widget_scrollbar_save(var);
			break;
		case WIDGET_HSEPARATOR:
		case WIDGET_VSEPARATOR:
			widget_hseparator_save(var);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_ICONVIEW:
			widget_iconview_save(var);
			break;
#endif
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_INFOBAR:
			widget_infobar_save(var);
			break;
#endif
		case WIDGET_LAYOUT:
			widget_layout_save(var);
			break;
		case WIDGET_LIST:
			widget_list_save(var);
			break;
		case WIDGET_MESSAGEDIALOG:
			widget_messagedialog_save(var);
			break;
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_LINKBUTTON:
			widget_linkbutton_save(var);
			break;
#endif
		case WIDGET_MENUBAR:
		case WIDGET_POPUPMENU:
			widget_menubar_save(var);
			break;
		case WIDGET_MENUITEMSEPARATOR:
		case WIDGET_MENUITEM:
		case WIDGET_MENU:
			widget_menuitem_save(var);
			break;
		case WIDGET_NOTEBOOK:
			widget_notebook_save(var);
			break;
		case WIDGET_PIXMAP:
			widget_pixmap_save(var);
			break;
		case WIDGET_PROGRESSBAR:
			widget_progressbar_save(var);
			break;
		case WIDGET_RADIOBUTTON:
			widget_radiobutton_save(var);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_RECENTCHOOSER:
			widget_recentchooser_save(var);
			break;
#endif
#if GTK_CHECK_VERSION(2,12,0)
		case WIDGET_SCALEBUTTON:
		case WIDGET_VOLUMEBUTTON:
			widget_scalebutton_save(var);
			break;
#endif
		case WIDGET_SCROLLEDWINDOW:
			widget_scrolledwindow_save(var);
			break;
		case WIDGET_VIEWPORT:
			widget_viewport_save(var);
			break;
		case WIDGET_HANDLEBOX:
			widget_handlebox_save(var);
			break;
		case WIDGET_TOOLPALETTE:
		case WIDGET_TOOLITEMGROUP:
			widget_toolpalette_save(var);
			break;
		case WIDGET_SPINBUTTON:
			widget_spinbutton_save(var);
			break;
#if GTK_CHECK_VERSION(2,20,0)
		case WIDGET_SPINNER:
			widget_spinner_save(var);
			break;
#endif
		case WIDGET_STATUSBAR:
			widget_statusbar_save(var);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_STATUSICON:
			widget_statusicon_save(var);
			break;
#endif
		case WIDGET_TABLE:
			widget_table_save(var);
			break;
		case WIDGET_TERMINAL:
			widget_terminal_save(var);
			break;
		case WIDGET_TEXT:
			widget_text_save(var);
			break;
		case WIDGET_TIMER:
			widget_timer_save(var);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_TREE:
			widget_tree_save(var);
			break;
#endif
		case WIDGET_VBOX:
			widget_vbox_save(var);
			break;
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			widget_window_save(var);
			break;
		default:
			yywarning("Save not implemented for this widget.");
	}
	return (var);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function will actualize the variable by reinitializing widget */

variable *variables_refresh(const char *name)
{
	variable         *var;

	g_assert(name != NULL);

#ifdef DEBUG
	g_message("%s(%s)", __func__, name);
#endif

	var = _tree_find(name, NULL);
	return variables_refresh_widget(var);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *variables_refresh_widget(variable *registry_var)
{
	variable          refresh_var;
	variable         *var = registry_var;
	gboolean          snapshot_held = FALSE;
	gchar            *string;
	gint              initialised = FALSE;

	if (var == NULL || var->Widget == NULL)
		return NULL;

	g_assert(var->Attributes != NULL);

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(
			g_object_get_data(G_OBJECT(var->Widget), "_initialised"));

	/* A later refresh can emit a normal widget signal whose action closes this
	 * window. Hold the widget and its attached descriptions until the refresh
	 * returns, and use a snapshot because closing clears the registry entry.
	 * Initial setup must retain the real record because timers and monitors
	 * store its address for their future callbacks. */
	if (initialised) {
		refresh_var = *var;
		g_object_ref(refresh_var.Widget);
		var = &refresh_var;
		snapshot_held = TRUE;
	}

	/* If the custom attribute "block-function-signals" is true
	 * then block signals whilst performing this function */
	if (var->widget_tag_attr &&
		((string = get_tag_attribute(var->widget_tag_attr, "block-function-signals"))) &&
		widget_attribute_is_true(string)) {
		GTKD_FUNCTION_SIGNALS_BLOCK;
	}

	switch (var->Type) {
		case WIDGET_ABOUTDIALOG:
			widget_dialog_refresh(var);
			break;
		case WIDGET_ALIGNMENT:
			widget_alignment_refresh(var);
			break;
		case WIDGET_ASPECTFRAME:
			widget_aspectframe_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_ASSISTANT:
			widget_assistant_refresh(var);
			break;
#endif
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOOLBAR:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
			widget_toolbar_refresh(var);
			break;
		case WIDGET_CALENDAR:
			widget_calendar_refresh(var);
			break;
		case WIDGET_ACCELLABEL:
			widget_accellabel_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_CELLVIEW:
			widget_cellview_refresh(var);
			break;
#endif
		case WIDGET_HBUTTONBOX:
		case WIDGET_VBUTTONBOX:
			widget_buttonbox_refresh(var);
			break;
		case WIDGET_CANCELBUTTON:
		case WIDGET_HELPBUTTON:
		case WIDGET_NOBUTTON:
		case WIDGET_OKBUTTON:
		case WIDGET_YESBUTTON:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_BUTTON:
			widget_button_refresh(var);
			break;
		case WIDGET_CHECKBOX:
			widget_checkbox_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_CHOOSER:
			widget_chooser_refresh(var);
			break;
#endif
		case WIDGET_COLORBUTTON:
			widget_colorbutton_refresh(var);
			break;
		case WIDGET_COLORSELECTION:
			widget_colorselection_refresh(var);
			break;
		case WIDGET_HSV:
			widget_hsv_refresh(var);
			break;
		case WIDGET_TASKLIST:
			widget_tasklist_refresh(var);
			break;
		case WIDGET_PAGER:
			widget_pager_refresh(var);
			break;
		case WIDGET_WINDOWSELECTOR:
			widget_windowselector_refresh(var);
			break;
		case WIDGET_OFFSCREENWINDOW:
			widget_offscreenwindow_refresh(var);
			break;
		case WIDGET_PAGESETUPDIALOG:
			widget_pagesetupdialog_refresh(var);
			break;
		case WIDGET_PRINTDIALOG:
			widget_printdialog_refresh(var);
			break;
		case WIDGET_FILECHOOSERDIALOG:
			widget_filechooserdialog_refresh(var);
			break;
		case WIDGET_CURVE:
			widget_curve_refresh(var);
			break;
		case WIDGET_HRULER:
		case WIDGET_VRULER:
			widget_ruler_refresh(var);
			break;
		case WIDGET_DIALOG:
			widget_dialog_refresh(var);
			break;
		case WIDGET_DRAWINGAREA:
			widget_drawingarea_refresh(var);
			break;
		case WIDGET_SHEET:
			widget_sheet_refresh(var);
			break;
		case WIDGET_DOCK:
		case WIDGET_DOCKITEM:
			widget_dock_refresh(var);
			break;
		case WIDGET_IMAGEVIEW:
			widget_imageview_refresh(var);
			break;
		case WIDGET_COMBOBOX:
			widget_combobox_refresh(var);
			break;
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
			widget_comboboxtext_refresh(var);
			break;
		case WIDGET_EDIT:
			widget_edit_refresh(var);
			break;
		case WIDGET_ENTRY:
			widget_entry_refresh(var);
			break;
		case WIDGET_EVENTBOX:
			widget_eventbox_refresh(var);
			break;
		case WIDGET_EXPANDER:
			widget_expander_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_FILECHOOSERBUTTON:
			widget_filechooserbutton_refresh(var);
			break;
#endif
		case WIDGET_FIXED:
			widget_fixed_refresh(var);
			break;
		case WIDGET_FONTBUTTON:
			widget_fontbutton_refresh(var);
			break;
		case WIDGET_FONTSELECTION:
			widget_fontselection_refresh(var);
			break;
		case WIDGET_FRAME:
			widget_frame_refresh(var);
			break;
		case WIDGET_GRID:
			widget_grid_refresh(var);
			break;
		case WIDGET_HBOX:
			widget_hbox_refresh(var);
			break;
		case WIDGET_HPANED:
		case WIDGET_VPANED:
			widget_paned_refresh(var);
			break;
		case WIDGET_HSCALE:
		case WIDGET_VSCALE:
			widget_hscale_refresh(var);
			break;
		case WIDGET_HSCROLLBAR:
		case WIDGET_VSCROLLBAR:
			widget_scrollbar_refresh(var);
			break;
		case WIDGET_HSEPARATOR:
		case WIDGET_VSEPARATOR:
			widget_hseparator_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_ICONVIEW:
			widget_iconview_refresh(var);
			break;
#endif
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_INFOBAR:
			widget_infobar_refresh(var);
			break;
#endif
		case WIDGET_LAYOUT:
			widget_layout_refresh(var);
			break;
		case WIDGET_LIST:
			widget_list_refresh(var);
			break;
		case WIDGET_MESSAGEDIALOG:
			widget_messagedialog_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_LINKBUTTON:
			widget_linkbutton_refresh(var);
			break;
#endif
		case WIDGET_MENUBAR:
		case WIDGET_POPUPMENU:
			widget_menubar_refresh(var);
			break;
		case WIDGET_MENUITEMSEPARATOR:
		case WIDGET_MENUITEM:
		case WIDGET_MENU:
			widget_menuitem_refresh(var);
			break;
		case WIDGET_NOTEBOOK:
			widget_notebook_refresh(var);
			break;
		case WIDGET_PIXMAP:
			widget_pixmap_refresh(var);
			break;
		case WIDGET_PROGRESSBAR:
			widget_progressbar_refresh(var);
			break;
		case WIDGET_RADIOBUTTON:
			widget_radiobutton_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_RECENTCHOOSER:
			widget_recentchooser_refresh(var);
			break;
#endif
#if GTK_CHECK_VERSION(2,12,0)
		case WIDGET_SCALEBUTTON:
		case WIDGET_VOLUMEBUTTON:
			widget_scalebutton_refresh(var);
			break;
#endif
		case WIDGET_SCROLLEDWINDOW:
			widget_scrolledwindow_refresh(var);
			break;
		case WIDGET_VIEWPORT:
			widget_viewport_refresh(var);
			break;
		case WIDGET_HANDLEBOX:
			widget_handlebox_refresh(var);
			break;
		case WIDGET_TOOLPALETTE:
		case WIDGET_TOOLITEMGROUP:
			widget_toolpalette_refresh(var);
			break;
		case WIDGET_SPINBUTTON:
			widget_spinbutton_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,20,0)
		case WIDGET_SPINNER:
			widget_spinner_refresh(var);
			break;
#endif
		case WIDGET_STATUSBAR:
			widget_statusbar_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_STATUSICON:
			widget_statusicon_refresh(var);
			break;
#endif
		case WIDGET_TABLE:
			widget_table_refresh(var);
			break;
		case WIDGET_TERMINAL:
			widget_terminal_refresh(var);
			break;
		case WIDGET_TEXT:
			widget_text_refresh(var);
			break;
		case WIDGET_TIMER:
			widget_timer_refresh(var);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_TREE:
			widget_tree_refresh(var);
			break;
#endif
		case WIDGET_VBOX:
			widget_vbox_refresh(var);
			break;
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			widget_window_refresh(var);
			break;
		default:
			if (initialised)
				yywarning("Refresh not implemented for this widget.");
			break;
	}

	if (!initialised) {
		/* Store "initialised" as a piece of widget data to record that
		 * this widget has been through this function at start-up */
		g_object_set_data(G_OBJECT(var->Widget), "_initialised",
			GINT_TO_POINTER(TRUE));
	}

	GTKD_FUNCTION_SIGNALS_RESET;
	if (snapshot_held)
		g_object_unref(var->Widget);

#ifdef DEBUG
	g_message("%s(): end", __func__);
#endif

	return registry_var;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *variables_enable(const char *name)
{
	variable *var;
#ifdef DEBUG
	fprintf(stderr, "%s(): %s\n", __func__, name);
	fflush(stderr);
#endif
	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);
	if (var->Widget == NULL)
		return (NULL);

	gtk_widget_set_sensitive(var->Widget, TRUE);
	return (var);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *variables_disable(const char *name)
{
	variable *var;
#ifdef DEBUG
	fprintf(stderr, "%s(): %s\n", __func__, name);
	fflush(stderr);
#endif
	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);
	if (var->Widget == NULL)
		return (NULL);

	gtk_widget_set_sensitive(var->Widget, FALSE);
	return (var);
}

/***********************************************************************
 * Variables show                                                      *
 ***********************************************************************/

variable *variables_show(const char *name)
{
	GtkWidget        *grandparent = NULL;
	GtkWidget        *parent = NULL;
	variable         *var;

#ifdef DEBUG
	fprintf(stderr, "%s(): %s\n", __func__, name);
	fflush(stderr);
#endif

	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);
	if (var->Widget == NULL)
		return (NULL);

	/* The widget could be inside a scrolled window or inside a viewport
	 * inside a scrolled window so we need to show all of the widgets */
	parent = gtk_widget_get_parent(var->Widget);
	if (parent) grandparent = gtk_widget_get_parent(parent);

	if (!(parent && ((GTK_IS_SCROLLED_WINDOW(parent)) ||
		(GTK_IS_VIEWPORT(parent))))) parent = NULL;

	if (!(parent && (GTK_IS_VIEWPORT(parent)) && grandparent &&
		GTK_IS_SCROLLED_WINDOW(grandparent))) grandparent = NULL;

	if (grandparent)
		gtk_widget_show(grandparent);

	if (parent)
		gtk_widget_show(parent);

	if (var->Type == WIDGET_BUTTON) {
		/* Button widgets can have up to four added child widgets */
		gtk_widget_show_all(var->Widget);
	} else if (var->Type == WIDGET_FRAME) {
		/* Frame widgets have an added child widget */
		gtk_widget_show(gtk_bin_get_child(GTK_BIN(var->Widget)));
		gtk_widget_show(var->Widget);
	} else {
		gtk_widget_show(var->Widget);
	}

	return (var);
}

/***********************************************************************
 * Variables hide                                                      *
 ***********************************************************************/

variable *variables_hide(const char *name)
{
	GtkWidget        *grandparent = NULL;
	GtkWidget        *parent = NULL;
	variable         *var;

#ifdef DEBUG
	fprintf(stderr, "%s(): %s\n", __func__, name);
	fflush(stderr);
#endif

	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);
	if (var->Widget == NULL)
		return (NULL);

	/* The widget could be inside a scrolled window or inside a viewport
	 * inside a scrolled window so we need to hide all of the widgets */
	parent = gtk_widget_get_parent(var->Widget);
	if (parent) grandparent = gtk_widget_get_parent(parent);

	if (!(parent && ((GTK_IS_SCROLLED_WINDOW(parent)) ||
		(GTK_IS_VIEWPORT(parent))))) parent = NULL;

	if (!(parent && (GTK_IS_VIEWPORT(parent)) && grandparent &&
		GTK_IS_SCROLLED_WINDOW(grandparent))) grandparent = NULL;

#ifdef DEBUG
	fprintf(stderr, "%s(): Hiding widget\n", __func__);
#endif
	gtk_widget_hide(var->Widget);

	if (parent) {
#ifdef DEBUG
		fprintf(stderr, "%s(): Hiding parent\n", __func__);
#endif
		gtk_widget_hide(parent);
	}

	if (grandparent) {
#ifdef DEBUG
		fprintf(stderr, "%s(): Hiding grandparent\n", __func__);
#endif
		gtk_widget_hide(grandparent);
	}
	return (var);
}

/***********************************************************************
 * Variables activate                                                  *
 ***********************************************************************/

variable *variables_activate(const char *name)
{
	gboolean  retval;
	variable *var;

#ifdef DEBUG
	fprintf(stderr, "%s(): %s\n", __func__, name);
	fflush(stderr);
#endif

	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);
	if (var->Widget == NULL)
		return (NULL);

	if (GTK_IS_RADIO_BUTTON(var->Widget)) {
		if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(var->Widget)))
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(var->Widget), TRUE);
		retval = TRUE;
	} else if (GTK_IS_TOGGLE_BUTTON(var->Widget)) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(var->Widget),
			!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(var->Widget)));
		retval = TRUE;
	} else if (widget_statusicon_is_proxy(var->Widget)) {
		retval = widget_statusicon_activate(var->Widget);
	}
#if GTK_CHECK_VERSION(2,4,0)
	else if (GTK_IS_RADIO_TOOL_BUTTON(var->Widget)) {
		if (!gtk_toggle_tool_button_get_active(
			GTK_TOGGLE_TOOL_BUTTON(var->Widget)))
			gtk_toggle_tool_button_set_active(
				GTK_TOGGLE_TOOL_BUTTON(var->Widget), TRUE);
		retval = TRUE;
	} else if (GTK_IS_TOGGLE_TOOL_BUTTON(var->Widget)) {
		gtk_toggle_tool_button_set_active(
			GTK_TOGGLE_TOOL_BUTTON(var->Widget),
			!gtk_toggle_tool_button_get_active(
				GTK_TOGGLE_TOOL_BUTTON(var->Widget)));
		retval = TRUE;
	} else if (GTK_IS_TOOL_BUTTON(var->Widget)) {
		g_signal_emit_by_name(var->Widget, "clicked");
		retval = TRUE;
	} else
#endif
		retval = gtk_widget_activate(var->Widget);

	if (!retval)
		fprintf(stderr, "%s(): %s is not an activatable widget.\n", __func__, name);

	return (var);
}

/***********************************************************************
 * Variables grabfocus                                                 *
 ***********************************************************************/

variable *variables_grabfocus(const char *name)
{
	variable *var;

#ifdef DEBUG
	fprintf(stderr, "%s(): %s\n", __func__, name);
	fflush(stderr);
#endif

	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);
	if (var->Widget == NULL)
		return (NULL);

	gtk_widget_grab_focus(var->Widget);

	return (var);
}

/***********************************************************************
 * Variables presentwindow                                             *
 ***********************************************************************/

variable *variables_presentwindow(const char *name)
{
	variable *var;

#ifdef DEBUG
	fprintf(stderr, "%s(): %s\n", __func__, name);
	fflush(stderr);
#endif

	var = _tree_find(name, NULL);
	if (var == NULL)
		return (NULL);
	if (var->Widget == NULL)
		return (NULL);
	if (var->Type == WIDGET_OFFSCREENWINDOW)
		return (var);

	gtk_window_present(GTK_WINDOW(var->Widget));

	return (var);
}

/***********************************************************************
 * Tree expansion                                                      *
 ***********************************************************************/

static variable *variables_set_tree_expanded(const char *name,
	gboolean expanded)
{
	variable *var;

	g_assert(name != NULL);
	var = _tree_find(name, NULL);
	if (var == NULL || var->Widget == NULL)
		return NULL;

#if GTK_CHECK_VERSION(2,4,0)
	if (var->Type != WIDGET_TREE || !GTK_IS_TREE_VIEW(var->Widget)) {
		gtkdialog_warning("%s is not a tree widget.", name);
		return var;
	}

	if (expanded)
		gtk_tree_view_expand_all(GTK_TREE_VIEW(var->Widget));
	else
		gtk_tree_view_collapse_all(GTK_TREE_VIEW(var->Widget));
#else
	(void)expanded;
#endif

	return var;
}

variable *variables_expand_all(const char *name)
{
	return variables_set_tree_expanded(name, TRUE);
}

variable *variables_collapse_all(const char *name)
{
	return variables_set_tree_expanded(name, FALSE);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gboolean variables_is_avail_by_name(const char *name)
{
	if (_tree_find(name, NULL) == NULL)
		return (FALSE);
	else
		return (TRUE);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *variables_get_by_name(const char *name)
{
	return _tree_find(name, NULL);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* Compatibility wrapper for the original internal tree helper. The
 * registry now stores names in GLib's balanced tree and keeps all widget
 * instances belonging to a name in declaration order. */

int _tree_insert(variable *new, variable *actual)
{
	VariableBucket *bucket;

	(void)actual;

	if (new == NULL) {
		fprintf(stderr, "%s(): Inserting a NULL element.\n", __func__);
		exit(EXIT_FAILURE);
	}

	bucket = variable_bucket_get(new->Name);
	if (bucket != NULL) {
		fprintf(stderr, "%s(): Inserting existing variable: '%s'.",
			__func__, new->Name);
		exit(EXIT_FAILURE);
	}

	bucket = g_new0(VariableBucket, 1);
	bucket->name = new->Name;
	bucket->instances = g_ptr_array_new();
	g_ptr_array_add(bucket->instances, new);
	g_tree_insert(variables_by_name, bucket->name, bucket);

	return 0;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function finds the variable by its name and returns the pointer
 * to it */

static variable *_tree_find(const char *name, variable *actual)
{
	(void)actual;
	return variable_bucket_current(variable_bucket_get(name));
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

static gboolean variables_count_bucket(gpointer key, gpointer value,
	gpointer data)
{
	VariableBucket *bucket = value;
	gint *count = data;
	guint index;

	(void)key;
	for (index = 0; index < bucket->instances->len; ++index) {
		variable *var = g_ptr_array_index(bucket->instances, index);
		if (var->Widget != NULL)
			++*count;
	}

	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gint variables_count_widgets(void) 
{
	gint count = 0;

	variables_registry_ensure();
	g_tree_foreach(variables_by_name, variables_count_bucket, &count);
	return count;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function will drop all the widgets with the given parent */

static void variable_drop_by_window_id(variable *actual, gint window_id)
{
	gint              index = 0;
	guint             timer_id;
#if HAVE_SYS_INOTIFY_H
	gchar             name[32];
#else
	GFileMonitor     *monitor;
	gchar             name[32];
#endif

#ifdef DEBUG
	GtkWidget *ancestor;
#endif

	if (actual == NULL || actual->Widget == NULL ||
		actual->window_id != window_id)
		return;

#ifdef DEBUG
	fprintf(stderr, "%s(): Name=%s Widget=%p window_id=%i Type=%i\n",
		__func__, actual->Name, actual->Widget, actual->window_id, actual->Type);
#endif

	g_assert(GTK_IS_WIDGET(actual->Widget));

	/* A timeout owns only the variable pointer, so destroying the widget does
	 * not remove it. Cancel it before this inactive variable instance can be
	 * reused by a subsequently launched window. */
	if (actual->Type == WIDGET_TIMER) {
		timer_id = GPOINTER_TO_UINT(g_object_get_data(
			G_OBJECT(actual->Widget), "_timer-id"));
		if (timer_id != 0) {
			g_source_remove(timer_id);
			g_object_set_data(G_OBJECT(actual->Widget), "_timer-id", NULL);
		}
	}

#ifdef DEBUG
	ancestor = gtk_widget_get_parent(actual->Widget);
	fprintf(stderr, "%s(): ancestor1=%p\n", __func__, ancestor);
	if (ancestor) ancestor = gtk_widget_get_parent(ancestor);
	fprintf(stderr, "%s(): ancestor2=%p\n", __func__, ancestor);
	if (ancestor) ancestor = gtk_widget_get_parent(ancestor);
	fprintf(stderr, "%s(): ancestor3=%p\n", __func__, ancestor);
	if (ancestor) ancestor = gtk_widget_get_parent(ancestor);
	fprintf(stderr, "%s(): ancestor4=%p\n", __func__, ancestor);
#endif

	/* Timer callbacks cancel themselves when they detect that var and
	 * var->Widget are NULL. Cancel any existing file monitors as well. */
#if HAVE_SYS_INOTIFY_H
	while (TRUE) {
		g_snprintf(name, sizeof(name), "_monitor%i", index);
		if (g_object_get_data(G_OBJECT(actual->Widget), name)) {
#ifdef DEBUG
			fprintf(stderr, "%s(): cancelling %s\n", __func__, name);
#endif
			/* Clearing the data invokes the monitor destroy function, which
			 * removes the GTK input source, watch and file descriptor. */
			g_object_set_data(G_OBJECT(actual->Widget), name, NULL);
		} else {
			break;
		}
		index++;
	}
#else
	while (TRUE) {
		g_snprintf(name, sizeof(name), "_monitor%i", index);
		monitor = g_object_get_data(G_OBJECT(actual->Widget), name);
		if (monitor) {
#ifdef DEBUG
			fprintf(stderr, "%s(): cancelling %s\n", __func__, name);
#endif
			g_file_monitor_cancel(monitor);
			/* Clearing the data releases the reference installed with the
			 * monitor and prevents a stale pointer on the widget. */
			g_object_set_data(G_OBJECT(actual->Widget), name, NULL);
		} else {
			break;
		}
		index++;
	}
#endif

	g_hash_table_remove(variables_by_widget, actual->Widget);
	actual->Widget = NULL;
	actual->Attributes = NULL;
	actual->widget_tag_attr = NULL;

#ifdef DEBUG
	fprintf(stderr, "%s(): Name=%s Widget=%p window_id=%i Type=%i\n",
		__func__, actual->Name, actual->Widget, actual->window_id, actual->Type);
#endif
}

typedef struct {
	gint window_id;
	GSList *inactive_names;
} VariableDropData;

static gboolean variables_remove_inactive_buckets_idle(gpointer data)
{
	GSList *element;
	GSList *names = data;

	for (element = names; element != NULL; element = element->next) {
		VariableBucket *bucket = variables_by_name == NULL ? NULL :
			g_tree_lookup(variables_by_name, element->data);

		/* A following synchronous action can relaunch the same name before
		 * this idle callback runs. Preserve the bucket if that happened. */
		if (bucket != NULL && !variable_bucket_has_active_widget(bucket))
			g_tree_remove(variables_by_name, element->data);
		g_free(element->data);
	}
	g_slist_free(names);
	return FALSE;
}

static gboolean variables_drop_bucket(gpointer key, gpointer value,
	gpointer data)
{
	VariableBucket *bucket = value;
	VariableDropData *drop_data = data;
	gboolean dropped = FALSE;
	guint index;

	for (index = 0; index < bucket->instances->len; ++index) {
		variable *var = g_ptr_array_index(bucket->instances, index);

		if (var->Widget != NULL && var->window_id == drop_data->window_id) {
			variable_drop_by_window_id(var, drop_data->window_id);
			dropped = TRUE;
		}
	}
	if (dropped && !variable_bucket_has_active_widget(bucket))
		drop_data->inactive_names = g_slist_prepend(
			drop_data->inactive_names, g_strdup(key));

	return FALSE;
}

void variables_drop_by_window_id(variable *actual, gint window_id)
{
	VariableDropData data = { window_id, NULL };

#ifdef DEBUG
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (actual != NULL) {
		if (actual->Widget != NULL && actual->window_id == window_id) {
			VariableBucket *bucket;

			variable_drop_by_window_id(actual, window_id);
			bucket = variable_bucket_get(actual->Name);
			if (!variable_bucket_has_active_widget(bucket))
				data.inactive_names = g_slist_prepend(
					data.inactive_names, g_strdup(actual->Name));
		}
	} else {
		variables_registry_ensure();
		g_tree_foreach(variables_by_name, variables_drop_bucket, &data);
	}
	if (data.inactive_names != NULL)
		g_idle_add(variables_remove_inactive_buckets_idle,
			data.inactive_names);

#ifdef DEBUG
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

variable *find_variable_by_widget(GtkWidget *widget)
{
	variables_registry_ensure();
	return g_hash_table_lookup(variables_by_widget, widget);
}

int variables_get_widget_type(GtkWidget *widget)
{
	variable *var;

	g_return_val_if_fail(GTK_IS_WIDGET(widget), 0);

	var = find_variable_by_widget(widget);
	if (var != NULL)
		return var->Type;

	return GPOINTER_TO_INT(g_object_get_data(G_OBJECT(widget),
		"gtkdialog-widget-type"));
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void variables_initialize_all(void)
{
	_variables_initialize(NULL);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
static gboolean variables_initialize_bucket(gpointer key, gpointer value,
	gpointer data)
{
	VariableBucket *bucket = value;
	guint index;

	(void)key;
	(void)data;
	for (index = 0; index < bucket->instances->len; ++index) {
		variable *var = g_ptr_array_index(bucket->instances, index);
		if (var->Widget != NULL)
			_variables_initialize(var);
	}

	return FALSE;
}

static void _variables_initialize(variable *actual)
{
	GList *element;
	char *socket_id;
	GError *spawn_error;
	gchar *gvim_argv[4];

#ifdef DEBUG
	fprintf(stderr, "%s: Start.\n", __func__);
	fflush(stderr);
#endif

	if (actual == NULL) {
		variables_registry_ensure();
		g_tree_foreach(variables_by_name, variables_initialize_bucket, NULL);
		return;
	}

	if (actual->Widget != NULL && actual->Type == WIDGET_GVIM) {
		gtk_widget_show(actual->Widget);
		socket_id = attributeset_get_first(&element, actual->Attributes,
			ATTR_SOCKET);
		if (socket_id != NULL) {
			spawn_error = NULL;
			gvim_argv[0] = "gvim";
			gvim_argv[1] = "--socketid";
			gvim_argv[2] = socket_id;
			gvim_argv[3] = NULL;
			if (!g_spawn_async(NULL, gvim_argv, NULL, G_SPAWN_SEARCH_PATH,
				NULL, NULL, NULL, &spawn_error)) {
				gtkdialog_warning("Couldn't start GVim: %s", spawn_error->message);
				g_error_free(spawn_error);
			}
		} else {
			yywarning("Socket ID is NULL\n");
		}
	}

}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function will export the variables as environment variables */

void variables_export_all(void)
{
	_variables_export(NULL);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
static gboolean variables_export_bucket(gpointer key, gpointer value,
	gpointer data)
{
	variable *var = variable_bucket_current(value);

	(void)key;
	(void)data;
	if (var != NULL)
		_variables_export(var);

	return FALSE;
}

static void _variables_export(variable *actual)
{
	GList            *element;
	gchar            *act;
	gchar            *value;
	gint              export = TRUE;

#ifdef DEBUG
	g_message("%s(%p)", __func__, actual);
#endif

	if (actual == NULL) {
		variables_registry_ensure();
		g_tree_foreach(variables_by_name, variables_export_bucket, NULL);
		return;
	}
	if (actual->Widget == NULL || actual->autonamed)
		return;

	/* A launched top-level dialog may use the required source variable as its
	 * own value. Keep the XML in the environment for a later launch;
	 * variables_print() still reports the dialog's current value. */
	if ((actual->Type == WIDGET_ABOUTDIALOG ||
		actual->Type == WIDGET_ASSISTANT ||
		actual->Type == WIDGET_DIALOG ||
		actual->Type == WIDGET_MESSAGEDIALOG ||
		actual->Type == WIDGET_PAGESETUPDIALOG ||
		actual->Type == WIDGET_PRINTDIALOG ||
		actual->Type == WIDGET_FILECHOOSERDIALOG ||
		actual->Type == WIDGET_POPUPMENU) &&
		strcmp(actual->Name, get_program_name()) == 0)
		return;

#ifdef DEBUG
	g_message("    actual->name = '%s'", actual->Name);
#endif

	/* Thunor: It's now possible to prevent the exporting of declared
	 * variables by using <variable export="false">VARNAME</variable> */
	if ((act = attributeset_get_first(&element, actual->Attributes,
		ATTR_VARIABLE)) &&
		(value = attributeset_get_this_tagattr(&element, actual->Attributes,
		ATTR_VARIABLE, "export")) &&
		((strcasecmp(value, "false") == 0) ||
		(strcasecmp(value, "no") == 0) ||
		(strcasecmp(value, "0") == 0))) {
		export = FALSE;
#ifdef DEBUG
		fprintf(stderr, "%s(): variable=%s export=%s\n",
			__func__, act, value);
#endif
	}

	/* Autonamed internal widgets are deliberately not exported. */
	if (export) {
		/* Export only the active value. Constructing an additional _ALL value
		 * on every signal made large list and table updates prohibitively slow. */
		value = widget_get_text_value(actual->Widget, actual->Type);
		if (value != NULL) {
			if (variable_name_is_shell_identifier(actual->Name))
				g_setenv(actual->Name, value, TRUE);
			else
				gtkdialog_warning("Not exporting invalid variable name '%s'.",
					actual->Name);
			g_free(value);
		}
	}

}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function is called when we want to send the variable's values
 * to the standard output */

static gboolean variables_print_bucket(gpointer key, gpointer value,
	gpointer data)
{
	variable *var = variable_bucket_current(value);

	(void)key;
	(void)data;
	if (var != NULL)
		print_variables(var);

	return FALSE;
}

void print_variables(variable *actual)
{
	GList            *element;
	gchar            *act;
	gchar            *value;
	gchar            *escaped;
	gint              export = TRUE;

	if (actual == NULL) {
		variables_registry_ensure();
		g_tree_foreach(variables_by_name, variables_print_bucket, NULL);
		fflush(stdout);
		return;
	}
	if (actual->Widget == NULL || actual->autonamed)
		return;

	/* Thunor: It's now possible to prevent the exporting of declared
	 * variables by using <variable export="false">VARNAME</variable> */
	if ((act = attributeset_get_first(&element, actual->Attributes,
		ATTR_VARIABLE)) &&
		(value = attributeset_get_this_tagattr(&element, actual->Attributes,
		ATTR_VARIABLE, "export")) &&
		((strcasecmp(value, "false") == 0) ||
		(strcasecmp(value, "no") == 0) ||
		(strcasecmp(value, "0") == 0))) {
		export = FALSE;
#ifdef DEBUG
		fprintf(stderr, "%s(): variable=%s export=%s\n",
			__func__, act, value);
#endif
	}

	if (export) {
		/* Print only the active value, matching the environment export path. */
		value = widget_get_text_value(actual->Widget, actual->Type);
		if (value == NULL)
			value = g_strdup("");
		if (variable_name_is_shell_identifier(actual->Name)) {
			escaped = variables_shell_escape(value);
			printf("%s=\"%s\"\n", actual->Name, escaped);
			g_free(escaped);
		} else {
			gtkdialog_warning("Not printing invalid variable name '%s'.",
				actual->Name);
		}
		g_free(value);
	}

	fflush(stdout);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function will clear the variable by deleting everything from
 * the widget */

variable *variables_clear(const char *name)
{
	variable         *toclear;
	gchar            *string;

#ifdef DEBUG
	fprintf(stderr, "%s(): variable: %s\n", __func__, name);
	fflush(stderr);
#endif

	toclear = _tree_find(name, NULL);
	if (toclear == NULL || toclear->Widget == NULL)
		return (NULL);

	/* If the custom attribute "block-function-signals" is true
	 * then block signals whilst performing this function */
	if (toclear->widget_tag_attr &&
		((string = get_tag_attribute(toclear->widget_tag_attr, "block-function-signals"))) &&
		widget_attribute_is_true(string)) {
		GTKD_FUNCTION_SIGNALS_BLOCK;
	}

	switch (toclear->Type) {
		case WIDGET_ABOUTDIALOG:
			widget_dialog_clear(toclear);
			break;
		case WIDGET_ALIGNMENT:
			widget_alignment_clear(toclear);
			break;
		case WIDGET_ASPECTFRAME:
			widget_aspectframe_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_ASSISTANT:
			widget_assistant_clear(toclear);
			break;
#endif
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOOLBAR:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
			widget_toolbar_clear(toclear);
			break;
		case WIDGET_CALENDAR:
			widget_calendar_clear(toclear);
			break;
		case WIDGET_ACCELLABEL:
			widget_accellabel_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_CELLVIEW:
			widget_cellview_clear(toclear);
			break;
#endif
		case WIDGET_HBUTTONBOX:
		case WIDGET_VBUTTONBOX:
			widget_buttonbox_clear(toclear);
			break;
		case WIDGET_CANCELBUTTON:
		case WIDGET_HELPBUTTON:
		case WIDGET_NOBUTTON:
		case WIDGET_OKBUTTON:
		case WIDGET_YESBUTTON:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_BUTTON:
			widget_button_clear(toclear);
			break;
		case WIDGET_CHECKBOX:
			widget_checkbox_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_CHOOSER:
			widget_chooser_clear(toclear);
			break;
#endif
		case WIDGET_COLORBUTTON:
			widget_colorbutton_clear(toclear);
			break;
		case WIDGET_COLORSELECTION:
			widget_colorselection_clear(toclear);
			break;
		case WIDGET_HSV:
			widget_hsv_clear(toclear);
			break;
		case WIDGET_TASKLIST:
			widget_tasklist_clear(toclear);
			break;
		case WIDGET_PAGER:
			widget_pager_clear(toclear);
			break;
		case WIDGET_WINDOWSELECTOR:
			widget_windowselector_clear(toclear);
			break;
		case WIDGET_OFFSCREENWINDOW:
			widget_offscreenwindow_clear(toclear);
			break;
		case WIDGET_PAGESETUPDIALOG:
			widget_pagesetupdialog_clear(toclear);
			break;
		case WIDGET_PRINTDIALOG:
			widget_printdialog_clear(toclear);
			break;
		case WIDGET_FILECHOOSERDIALOG:
			widget_filechooserdialog_clear(toclear);
			break;
		case WIDGET_CURVE:
			widget_curve_clear(toclear);
			break;
		case WIDGET_HRULER:
		case WIDGET_VRULER:
			widget_ruler_clear(toclear);
			break;
		case WIDGET_DIALOG:
			widget_dialog_clear(toclear);
			break;
		case WIDGET_DRAWINGAREA:
			widget_drawingarea_clear(toclear);
			break;
		case WIDGET_SHEET:
			widget_sheet_clear(toclear);
			break;
		case WIDGET_DOCK:
		case WIDGET_DOCKITEM:
			widget_dock_clear(toclear);
			break;
		case WIDGET_IMAGEVIEW:
			widget_imageview_clear(toclear);
			break;
		case WIDGET_COMBOBOX:
			widget_combobox_clear(toclear);
			break;
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
			widget_comboboxtext_clear(toclear);
			break;
		case WIDGET_EDIT:
			widget_edit_clear(toclear);
			break;
		case WIDGET_ENTRY:
			widget_entry_clear(toclear);
			break;
		case WIDGET_EVENTBOX:
			widget_eventbox_clear(toclear);
			break;
		case WIDGET_EXPANDER:
			widget_expander_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_FILECHOOSERBUTTON:
			widget_filechooserbutton_clear(toclear);
			break;
#endif
		case WIDGET_FIXED:
			widget_fixed_clear(toclear);
			break;
		case WIDGET_FONTBUTTON:
			widget_fontbutton_clear(toclear);
			break;
		case WIDGET_FONTSELECTION:
			widget_fontselection_clear(toclear);
			break;
		case WIDGET_FRAME:
			widget_frame_clear(toclear);
			break;
		case WIDGET_GRID:
			widget_grid_clear(toclear);
			break;
		case WIDGET_HBOX:
			widget_hbox_clear(toclear);
			break;
		case WIDGET_HPANED:
		case WIDGET_VPANED:
			widget_paned_clear(toclear);
			break;
		case WIDGET_HSCALE:
		case WIDGET_VSCALE:
			widget_hscale_clear(toclear);
			break;
		case WIDGET_HSCROLLBAR:
		case WIDGET_VSCROLLBAR:
			widget_scrollbar_clear(toclear);
			break;
		case WIDGET_HSEPARATOR:
		case WIDGET_VSEPARATOR:
			widget_hseparator_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_ICONVIEW:
			widget_iconview_clear(toclear);
			break;
#endif
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_INFOBAR:
			widget_infobar_clear(toclear);
			break;
#endif
		case WIDGET_LAYOUT:
			widget_layout_clear(toclear);
			break;
		case WIDGET_LIST:
			widget_list_clear(toclear);
			break;
		case WIDGET_MESSAGEDIALOG:
			widget_messagedialog_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_LINKBUTTON:
			widget_linkbutton_clear(toclear);
			break;
#endif
		case WIDGET_MENUBAR:
		case WIDGET_POPUPMENU:
			widget_menubar_clear(toclear);
			break;
		case WIDGET_MENUITEMSEPARATOR:
		case WIDGET_MENUITEM:
		case WIDGET_MENU:
			widget_menuitem_clear(toclear);
			break;
		case WIDGET_NOTEBOOK:
			widget_notebook_clear(toclear);
			break;
		case WIDGET_PIXMAP:
			widget_pixmap_clear(toclear);
			break;
		case WIDGET_PROGRESSBAR:
			widget_progressbar_clear(toclear);
			break;
		case WIDGET_RADIOBUTTON:
			widget_radiobutton_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_RECENTCHOOSER:
			widget_recentchooser_clear(toclear);
			break;
#endif
#if GTK_CHECK_VERSION(2,12,0)
		case WIDGET_SCALEBUTTON:
		case WIDGET_VOLUMEBUTTON:
			widget_scalebutton_clear(toclear);
			break;
#endif
		case WIDGET_SCROLLEDWINDOW:
			widget_scrolledwindow_clear(toclear);
			break;
		case WIDGET_VIEWPORT:
			widget_viewport_clear(toclear);
			break;
		case WIDGET_HANDLEBOX:
			widget_handlebox_clear(toclear);
			break;
		case WIDGET_TOOLPALETTE:
		case WIDGET_TOOLITEMGROUP:
			widget_toolpalette_clear(toclear);
			break;
		case WIDGET_SPINBUTTON:
			widget_spinbutton_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,20,0)
		case WIDGET_SPINNER:
			widget_spinner_clear(toclear);
			break;
#endif
		case WIDGET_STATUSBAR:
			widget_statusbar_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_STATUSICON:
			widget_statusicon_clear(toclear);
			break;
#endif
		case WIDGET_TABLE:
			widget_table_clear(toclear);
			break;
		case WIDGET_TERMINAL:
			widget_terminal_clear(toclear);
			break;
		case WIDGET_TEXT:
			widget_text_clear(toclear);
			break;
		case WIDGET_TIMER:
			widget_timer_clear(toclear);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_TREE:
			widget_tree_clear(toclear);
			break;
#endif
		case WIDGET_VBOX:
			widget_vbox_clear(toclear);
			break;
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			widget_window_clear(toclear);
			break;
		default:
			yywarning("Clear not implemented for this widget.");
	}

	GTKD_FUNCTION_SIGNALS_RESET;

	return (toclear);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This function will remove the selected item from the widget */

int remove_selected_variable(const char *name)
{
	variable         *toclear;
	gchar            *string;

	g_assert(name != NULL);

#ifdef DEBUG
	g_message("%s(): variable: %s", __func__, name);
#endif
	/*
	 * Searching for the given variable.
	 */
	toclear = _tree_find(name, NULL);
	if (toclear == NULL || toclear->Widget == NULL)
		return -1;

	/* If the custom attribute "block-function-signals" is true
	 * then block signals whilst performing this function */
	if (toclear->widget_tag_attr &&
		((string = get_tag_attribute(toclear->widget_tag_attr, "block-function-signals"))) &&
		widget_attribute_is_true(string)) {
		GTKD_FUNCTION_SIGNALS_BLOCK;
	}

	/*
	 * Removing the selected item or text range from the widget.
	 */
	switch (toclear->Type) {
		case WIDGET_ABOUTDIALOG:
			widget_dialog_removeselected(toclear);
			break;
		case WIDGET_ALIGNMENT:
			widget_alignment_removeselected(toclear);
			break;
		case WIDGET_ASPECTFRAME:
			widget_aspectframe_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_ASSISTANT:
			widget_assistant_removeselected(toclear);
			break;
#endif
		case WIDGET_SEPARATORTOOLITEM:
		case WIDGET_TOOLBAR:
		case WIDGET_TOOLBUTTON:
		case WIDGET_TOOLITEM:
		case WIDGET_TOGGLETOOLBUTTON:
		case WIDGET_RADIOTOOLBUTTON:
		case WIDGET_MENUTOOLBUTTON:
			widget_toolbar_removeselected(toclear);
			break;
		case WIDGET_CALENDAR:
			widget_calendar_removeselected(toclear);
			break;
		case WIDGET_HBUTTONBOX:
		case WIDGET_VBUTTONBOX:
			widget_buttonbox_removeselected(toclear);
			break;
		case WIDGET_CANCELBUTTON:
		case WIDGET_HELPBUTTON:
		case WIDGET_NOBUTTON:
		case WIDGET_OKBUTTON:
		case WIDGET_YESBUTTON:
		case WIDGET_TOGGLEBUTTON:
		case WIDGET_BUTTON:
			widget_button_removeselected(toclear);
			break;
		case WIDGET_CHECKBOX:
			widget_checkbox_removeselected(toclear);
			break;
		case WIDGET_COLORBUTTON:
			widget_colorbutton_removeselected(toclear);
			break;
		case WIDGET_COLORSELECTION:
			widget_colorselection_removeselected(toclear);
			break;
		case WIDGET_HSV:
			widget_hsv_removeselected(toclear);
			break;
		case WIDGET_TASKLIST:
			widget_tasklist_removeselected(toclear);
			break;
		case WIDGET_PAGER:
			widget_pager_removeselected(toclear);
			break;
		case WIDGET_WINDOWSELECTOR:
			widget_windowselector_removeselected(toclear);
			break;
		case WIDGET_OFFSCREENWINDOW:
			widget_offscreenwindow_removeselected(toclear);
			break;
		case WIDGET_PAGESETUPDIALOG:
			widget_pagesetupdialog_removeselected(toclear);
			break;
		case WIDGET_PRINTDIALOG:
			widget_printdialog_removeselected(toclear);
			break;
		case WIDGET_FILECHOOSERDIALOG:
			widget_filechooserdialog_removeselected(toclear);
			break;
		case WIDGET_CURVE:
			widget_curve_removeselected(toclear);
			break;
		case WIDGET_HRULER:
		case WIDGET_VRULER:
			widget_ruler_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_CHOOSER:
			widget_chooser_removeselected(toclear);
			break;
#endif
		case WIDGET_DIALOG:
			widget_dialog_removeselected(toclear);
			break;
		case WIDGET_COMBOBOX:
			widget_combobox_removeselected(toclear);
			break;
		case WIDGET_COMBOBOXENTRY:
		case WIDGET_COMBOBOXTEXT:
			widget_comboboxtext_removeselected(toclear);
			break;
		case WIDGET_EDIT:
			widget_edit_removeselected(toclear);
			break;
		case WIDGET_ACCELLABEL:
			widget_accellabel_removeselected(toclear);
			break;
		case WIDGET_ENTRY:
			widget_entry_removeselected(toclear);
			break;
		case WIDGET_EVENTBOX:
			widget_eventbox_removeselected(toclear);
			break;
		case WIDGET_EXPANDER:
			widget_expander_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_FILECHOOSERBUTTON:
			widget_filechooserbutton_removeselected(toclear);
			break;
#endif
		case WIDGET_FIXED:
			widget_fixed_removeselected(toclear);
			break;
		case WIDGET_FONTBUTTON:
			widget_fontbutton_removeselected(toclear);
			break;
		case WIDGET_FONTSELECTION:
			widget_fontselection_removeselected(toclear);
			break;
		case WIDGET_FRAME:
			widget_frame_removeselected(toclear);
			break;
		case WIDGET_GRID:
			widget_grid_removeselected(toclear);
			break;
		case WIDGET_HBOX:
			widget_hbox_removeselected(toclear);
			break;
		case WIDGET_HPANED:
		case WIDGET_VPANED:
			widget_paned_removeselected(toclear);
			break;
		case WIDGET_HSCALE:
		case WIDGET_VSCALE:
			widget_hscale_removeselected(toclear);
			break;
		case WIDGET_HSCROLLBAR:
		case WIDGET_VSCROLLBAR:
			widget_scrollbar_removeselected(toclear);
			break;
		case WIDGET_HSEPARATOR:
		case WIDGET_VSEPARATOR:
			widget_hseparator_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,6,0)
		case WIDGET_ICONVIEW:
			widget_iconview_removeselected(toclear);
			break;
#endif
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_INFOBAR:
			widget_infobar_removeselected(toclear);
			break;
#endif
		case WIDGET_LAYOUT:
			widget_layout_removeselected(toclear);
			break;
		case WIDGET_LIST:
			widget_list_removeselected(toclear);
			break;
		case WIDGET_MESSAGEDIALOG:
			widget_messagedialog_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,18,0)
		case WIDGET_LINKBUTTON:
			widget_linkbutton_removeselected(toclear);
			break;
#endif
		case WIDGET_MENUBAR:
		case WIDGET_POPUPMENU:
			widget_menubar_removeselected(toclear);
			break;
		case WIDGET_MENUITEMSEPARATOR:
		case WIDGET_MENUITEM:
		case WIDGET_MENU:
			widget_menuitem_removeselected(toclear);
			break;
		case WIDGET_NOTEBOOK:
			widget_notebook_removeselected(toclear);
			break;
		case WIDGET_PIXMAP:
			widget_pixmap_removeselected(toclear);
			break;
		case WIDGET_PROGRESSBAR:
			widget_progressbar_removeselected(toclear);
			break;
		case WIDGET_RADIOBUTTON:
			widget_radiobutton_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_RECENTCHOOSERMENU:
		case WIDGET_RECENTCHOOSER:
			widget_recentchooser_removeselected(toclear);
			break;
#endif
#if GTK_CHECK_VERSION(2,12,0)
		case WIDGET_SCALEBUTTON:
		case WIDGET_VOLUMEBUTTON:
			widget_scalebutton_removeselected(toclear);
			break;
#endif
		case WIDGET_SCROLLEDWINDOW:
			widget_scrolledwindow_removeselected(toclear);
			break;
		case WIDGET_VIEWPORT:
			widget_viewport_removeselected(toclear);
			break;
		case WIDGET_HANDLEBOX:
			widget_handlebox_removeselected(toclear);
			break;
		case WIDGET_TOOLPALETTE:
		case WIDGET_TOOLITEMGROUP:
			widget_toolpalette_removeselected(toclear);
			break;
		case WIDGET_SPINBUTTON:
			widget_spinbutton_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,20,0)
		case WIDGET_SPINNER:
			widget_spinner_removeselected(toclear);
			break;
#endif
		case WIDGET_STATUSBAR:
			widget_statusbar_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,10,0)
		case WIDGET_STATUSICON:
			widget_statusicon_removeselected(toclear);
			break;
#endif
		case WIDGET_TABLE:
			widget_table_removeselected(toclear);
			break;
		case WIDGET_TERMINAL:
			widget_terminal_removeselected(toclear);
			break;
		case WIDGET_TEXT:
			widget_text_removeselected(toclear);
			break;
		case WIDGET_TIMER:
			widget_timer_removeselected(toclear);
			break;
#if GTK_CHECK_VERSION(2,4,0)
		case WIDGET_TREE:
			widget_tree_removeselected(toclear);
			break;
#endif
		case WIDGET_VBOX:
			widget_vbox_removeselected(toclear);
			break;
		case WIDGET_PLUG:
		case WIDGET_WINDOW:
			widget_window_removeselected(toclear);
			break;
		default:
			yywarning("Delete not implemented for this widget.");
	}

	GTKD_FUNCTION_SIGNALS_RESET;

	return 0;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This is a debug function we use to print all variables to the 
 * standard error. It will print a lot of information */

#ifdef DEBUG
static gboolean variables_print_debug_bucket(gpointer key, gpointer value,
	gpointer data)
{
	VariableBucket *bucket = value;
	guint index;

	(void)key;
	(void)data;
	for (index = 0; index < bucket->instances->len; ++index)
		variables_print_one(g_ptr_array_index(bucket->instances, index));

	return FALSE;
}

void variables_print_debug(variable *actual)
{
	if (actual != NULL) {
		variables_print_one(actual);
		return;
	}

	variables_registry_ensure();
	g_tree_foreach(variables_by_name, variables_print_debug_bucket, NULL);
}
#endif
