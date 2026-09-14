/*
 * widget_table.c: 
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
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "stringman.h"
#include "tag_attributes.h"

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS

/* Local function prototypes, located at file bottom */
static void widget_table_input_by_command(variable *var, char *filename,
	gint command_or_file);
static void widget_table_input_by_file(variable *var, char *filename);
static void widget_table_input_by_items(variable *var);
static void widget_table_append_row(GtkWidget *widget, gchar **values);
gboolean widget_table_click_column_callback(GtkWidget *widget,
	gint column, variable *var);
gint widget_table_natcmp(GtkCList *clist, gconstpointer ptr1,
	gconstpointer ptr2);
gint widget_table_natcasecmp(GtkCList *clist, gconstpointer ptr1,
	gconstpointer ptr2);
static gint _widget_table_natcmp(GtkCList *clist, gconstpointer ptr1,
	gconstpointer ptr2, gint sensitive);

/* Notes: */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_table_clear(variable *var)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	gtk_clist_clear(GTK_CLIST(var->Widget));

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/
GtkWidget *widget_table_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GList            *element;
	GtkWidget        *widget;
	gchar            *value;
	gint              column;
	gint              sort_function;
	GtkSelectionMode  selectionmode;
	list_t           *sliced;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (attributeset_is_avail(Attr, ATTR_LABEL)) {
		sliced = linecutter(g_strdup(attributeset_get_first(
			&element, Attr, ATTR_LABEL)), '|');
		widget = gtk_clist_new_with_titles(sliced->n_lines,
			sliced->line);
		if (sliced) list_t_free(sliced);	/* Free linecutter memory */
	} else {
		widget = gtk_clist_new(1);	/* 1 column */
	}

	if (attr) {
		/* GtkCList rejects GTK_SELECTION_NONE even though it is in the enum. */
		if ((value = get_tag_attribute(attr, "selection-mode"))) {
			selectionmode = widget_parse_selection_mode(value, FALSE,
				"table selection-mode");
			gtk_clist_set_selection_mode(GTK_CLIST(widget), selectionmode);
			kill_tag_attribute(attr, "selection-mode");
		}
		/* Get sort-function (custom) */
		if ((value = get_tag_attribute(attr, "sort-function"))) {
			sort_function = widget_parse_bounded_integer(value, 0, 2, 0,
				"table sort-function");
			if (sort_function == 1) {
				gtk_clist_set_compare_func(GTK_CLIST(widget), widget_table_natcmp);
			} else if (sort_function == 2) {
				gtk_clist_set_compare_func(GTK_CLIST(widget), widget_table_natcasecmp);
			}
		}
		/* Get sort-type (auto-sort will require this preset) */
		if ((value = get_tag_attribute(attr, "sort-type"))) {
			gtk_clist_set_sort_type(GTK_CLIST(widget),
				widget_parse_sort_type(value, "table sort-type"));
		}
		/* Get sort-column (custom) */
		if ((value = get_tag_attribute(attr, "sort-column"))) {
			column = widget_parse_column_index(value,
				GTK_CLIST(widget)->columns, "table sort-column");
			gtk_clist_set_sort_column(GTK_CLIST(widget), column);
		}
		/* Get auto-sort (custom) */
		if ((value = get_tag_attribute(attr, "auto-sort")) &&
			widget_attribute_is_true(value)) {
			gtk_clist_set_auto_sort(GTK_CLIST(widget), TRUE);
		} else {
			gtk_clist_set_auto_sort(GTK_CLIST(widget), FALSE);
		}
		/* Get column-header-active (custom) */
		if ((value = get_tag_attribute(attr, "column-header-active"))) {
			sliced = linecutter(g_strdup(value), '|');
			for (column = 0; column < sliced->n_lines &&
				column < GTK_CLIST(widget)->columns; column++) {
				if (widget_attribute_is_true(sliced->line[column])) {
					gtk_clist_column_title_active(GTK_CLIST(widget), column);
				} else {
					gtk_clist_column_title_passive(GTK_CLIST(widget), column);
				}
			}
			if (sliced) list_t_free(sliced);	/* Free linecutter memory */
		}
		/* Get column-visible (custom) */
		if ((value = get_tag_attribute(attr, "column-visible"))) {
			sliced = linecutter(g_strdup(value), '|');
			for (column = 0; column < sliced->n_lines &&
				column < GTK_CLIST(widget)->columns; column++) {
				if (widget_attribute_is_true(sliced->line[column])) {
					gtk_clist_set_column_visibility(GTK_CLIST(widget),
						column, TRUE);
				} else {
					gtk_clist_set_column_visibility(GTK_CLIST(widget),
						column, FALSE);
				}
			}
			if (sliced) list_t_free(sliced);	/* Free linecutter memory */
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

gchar *widget_table_envvar_construct(GtkWidget *widget)
{
	GList            *selected_row;
	GString          *selected_values;
	gchar            *string;
	gchar            *value;
	gint              column = 0;
	gint              row;
	gint              selectionmode;
	gboolean          first_row;
	variable         *var = find_variable_by_widget(widget);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (var->widget_tag_attr) {
		/* Get exported-column */
		if ((value = get_tag_attribute(var->widget_tag_attr, "exported-column")))
			column = widget_parse_column_index(value,
				GTK_CLIST(widget)->columns, "table exported-column");
	}
	selectionmode = GTK_CLIST(widget)->selection_mode;

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): widget=%p selectionmode=%i column=%i\n",
		__func__, widget, selectionmode, column);
#endif

	if (selectionmode == GTK_SELECTION_NONE) {
		/* The API says that this is OK but a critical error gets dumped
		 * to the terminal and it doesn't actually prevent selection so
		 * it's not a usable selection mode */
		string = g_strdup("");	/* Nothing is selected */
	} else if (selectionmode == GTK_SELECTION_MULTIPLE) {
		selected_values = g_string_new(NULL);
		first_row = TRUE;
		for (selected_row = GTK_CLIST(widget)->selection;
			selected_row != NULL; selected_row = selected_row->next) {
			row = GPOINTER_TO_INT(selected_row->data);
			gtk_clist_get_text(GTK_CLIST(widget), row, column, &string);
			if (!first_row)
				g_string_append_c(selected_values, '\n');
			g_string_append(selected_values, string);
			first_row = FALSE;
		}
		string = g_string_free(selected_values, FALSE);
	} else {
		/* Default GTK_SELECTION_SINGLE and GTK_SELECTION_BROWSE.
		 * 
		 * There does not appear to be a way to get the selected item(s)
		 * outside of the select-row signal callback but I found this
		 * recommended technique from here:
		 * https://mail.gnome.org/archives/gtk-app-devel-list/1999-April/msg00033.html */
		if (g_list_length(GTK_CLIST(widget)->selection)) {
			row = GPOINTER_TO_INT(g_list_nth_data(
				GTK_CLIST(widget)->selection, 0));
			gtk_clist_get_text(GTK_CLIST(widget), row, column, &value);
			string = g_strdup(value);
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

void widget_table_fileselect(
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
void widget_table_refresh(variable *var)
{
	GList            *element;
	gchar            *act;
	gchar            *value;
	gint              freeze_thaw = FALSE;
	gint              initialised = FALSE;
	gint              selected_row;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(var->Widget), "_initialised"));

	if (var->widget_tag_attr) {
		/* Get freeze-thaw (custom) */
		if ((value = get_tag_attribute(var->widget_tag_attr, "freeze-thaw")) &&
			widget_attribute_is_true(value)) {
			freeze_thaw = TRUE;
			gtk_clist_freeze(GTK_CLIST(var->Widget));
		}
	}

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		if (input_is_shell_command(act))
			widget_table_input_by_command(var, act + 8, TRUE);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, act + 5);
			}
			widget_table_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_table_input_by_items(var);

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
		g_signal_connect(G_OBJECT(var->Widget), "select-row",
			G_CALLBACK(on_any_widget_select_row_event), (gpointer)var->Attributes);
		g_signal_connect(G_OBJECT(var->Widget), "click-column",
			G_CALLBACK(widget_table_click_column_callback), (gpointer)var);

	}

	/* Thaw a freeze? */
	if (freeze_thaw) gtk_clist_thaw(GTK_CLIST(var->Widget));

	if (var->widget_tag_attr) {
		/* Get selected-row (custom) */
		if ((value = get_tag_attribute(var->widget_tag_attr, "selected-row"))) {
			selected_row = widget_parse_nonnegative_integer(value, -1,
				"table selected-row");
			if (selected_row >= 0)
				gtk_clist_select_row(GTK_CLIST(var->Widget), selected_row, 0);
		}
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_table_removeselected(variable *var)
{
	GList            *selection;
	gint              row;
	gint              selectionmode;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	selectionmode = GTK_CLIST(var->Widget)->selection_mode;

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): widget=%p selectionmode=%i\n",
		__func__, var->Widget, selectionmode);
#endif

	if (selectionmode == GTK_SELECTION_NONE) {
		/* Nothing to do */
	} else if (selectionmode == GTK_SELECTION_MULTIPLE) {
		/* Row removal updates the selection list synchronously. Always
		 * consume its head without repeatedly traversing the whole list. */
		selection = GTK_CLIST(var->Widget)->selection;
		while (selection != NULL) {
			row = GPOINTER_TO_INT(selection->data);
			gtk_clist_remove(GTK_CLIST(var->Widget), row);
			selection = GTK_CLIST(var->Widget)->selection;
		}
	} else {
		/* Default GTK_SELECTION_SINGLE and GTK_SELECTION_BROWSE.
		 * 
		 * There does not appear to be a way to get the selected item(s)
		 * outside of the select-row signal callback but I found this
		 * recommended technique from here:
		 * https://mail.gnome.org/archives/gtk-app-devel-list/1999-April/msg00033.html */
		selection = GTK_CLIST(var->Widget)->selection;
		if (selection != NULL) {
			row = GPOINTER_TO_INT(selection->data);
			gtk_clist_remove(GTK_CLIST(var->Widget), row);
		}
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_table_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	gchar            *act;
	gchar            *filename = NULL;
	gchar            *string;
	gint              column, columnmax;
	gint              retval;
	gint              row = 0;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We'll use the output file filename if available */
	act = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (act) {
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			filename = act + 5;
			break;
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_OUTPUT);
	}

	/* If we have a valid filename then open it and dump the
	 * widget's data to it */
	if (filename) {
		if ((outfile = fopen(filename, "w"))) {

			g_object_get(G_OBJECT(var->Widget), "n-columns", &columnmax, NULL);

			/* Where's the GtkCList row count? It's not such a problem as
			 * gtk_clist_get_text() returns 0 if it's not available at which
			 * point we'll stop */
			retval = gtk_clist_get_text(GTK_CLIST(var->Widget), row, 0, &string);
			while (retval) {
				if (row != 0)
					fputc('\n', outfile);
				for (column = 0; column < columnmax; column++) {

					/* Do not free string, it is not newly allocated */
					gtk_clist_get_text(GTK_CLIST(var->Widget), row, column, &string);
					if (column != 0)
						fputc('|', outfile);
					fputs(string ? string : "", outfile);
				}

				row++;
				retval = gtk_clist_get_text(GTK_CLIST(var->Widget), row, 0, &string);
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
 * Input by Command                                                    *
 ***********************************************************************/

static void widget_table_input_by_command(variable *var, char *filename,
	gint command_or_file)
{
	FILE             *infile;
	gchar            *line;
	list_t           *sliced;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	if (command_or_file) {
		infile = widget_opencommand(filename);
	} else {
		infile = fopen(filename, "r");
	}

	/* Opening pipe for reading... */
	if (infile) {
		while ((line = widget_read_line(infile)) != NULL) {
			sliced = linecutter(g_strdup(line), '|');
			widget_table_append_row(var->Widget, sliced->line);
			if (sliced) list_t_free(sliced);	/* Free linecutter memory */
			g_free(line);
		}
		/* Close the file */
		if (command_or_file)
			widget_closecommand(infile, filename);
		else
			fclose(infile);
	} else {
		fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n", __func__,
			filename);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by File                                                       *
 ***********************************************************************/

static void widget_table_input_by_file(variable *var, char *filename)
{
#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	widget_table_input_by_command(var, filename, FALSE);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Append Row                                                          *
 ***********************************************************************/

static void widget_table_append_row(GtkWidget *widget, gchar **values)
{
	gchar empty[] = "";
	gchar **row;
	guint value_count;
	gint column;
	gint column_count;

	column_count = GTK_CLIST(widget)->columns;
	value_count = g_strv_length(values);
	row = g_new(gchar *, column_count);
	for (column = 0; column < column_count; ++column)
		row[column] = column < (gint)value_count ? values[column] : empty;

	gtk_clist_append(GTK_CLIST(widget), row);
	g_free(row);
}

/***********************************************************************
 * Input by Items                                                      *
 ***********************************************************************/

static void widget_table_input_by_items(variable *var)
{
	GList            *element;
	gchar            *text;
	list_t           *sliced;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	g_assert(var->Attributes != NULL && var->Widget != NULL);

	text = attributeset_get_first(&element, var->Attributes, ATTR_ITEM);
	while (text != NULL) {
		sliced = linecutter(g_strdup(text), '|');
		widget_table_append_row(var->Widget, sliced->line);
		if (sliced) list_t_free(sliced);	/* Free linecutter memory */
		text = attributeset_get_next(&element, var->Attributes, ATTR_ITEM);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Click Column Callback                                               *
 ***********************************************************************/
/* Table Widget Behaviour
 * ----------------------
 * Assuming that the default sort direction is ascending:
 * 
 * Click column 0 and it'll sort ascending.
 * Click column 1 and it'll sort ascending.
 * Click column 1 and it'll sort descending having flipped.
 * Click column 0 and it'll sort descending.
 * Click column 1 and it'll sort descending.
 * 
 * Tree Widget Behaviour
 * ---------------------
 * Assuming that the default sort direction is ascending:
 * 
 * Click column 0 and it'll sort ascending.
 * Click column 1 and it'll sort ascending.
 * Click column 1 and it'll sort descending having flipped.
 * Click column 0 and it'll sort ascending.
 * Click column 1 and it'll sort ascending.
 * 
 * A descending column will not be in that same state when it is clicked
 * at a later time therefore tree defaults to ascending */

gboolean widget_table_click_column_callback(GtkWidget *widget,
	gint column, variable *var)
{
	gint              last_column = -1;
	gint              sort_type;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Set sort column to the clicked column */
	gtk_clist_set_sort_column(GTK_CLIST(widget), column);

	/* Get last recorded column if it exists */
	if (g_object_get_data(G_OBJECT(widget), "_last-column") != NULL) {
		last_column = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(widget), "_last-column"));
		last_column--;
	}

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): column=%i last-column=%i\n", __func__,
		column, last_column);
#endif

	/* If last recorded column matches column then flip sort direction */
	if (last_column == column) {
		/* Get sort-type (direction) and flip it */
		g_object_get(G_OBJECT(widget), "sort-type", &sort_type, NULL);
		sort_type = 1 - sort_type;
		gtk_clist_set_sort_type(GTK_CLIST(widget), sort_type);
	}

	/* Store "last-column" as a piece of widget data (recreated if exists) */
	/* Warning: Storing zero kills the piece of data so we have to
	 * maintain it with +1 on set, -1 on get */
	column++;
	g_object_set_data(G_OBJECT(widget), "_last-column", GINT_TO_POINTER(column));

	/* Sit back and be amazed */
	gtk_clist_sort(GTK_CLIST(widget));

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return TRUE;
}

/***********************************************************************
 * Natural Compare                                                     *
 ***********************************************************************/
/* I found a useful example at:
 * 
 *   https://mail.gnome.org/archives/gtk-list/2000-April/msg00234.html
 * 
 * which explained how to get at the text in the rows */

gint widget_table_natcmp(GtkCList *clist, gconstpointer ptr1,
	gconstpointer ptr2)
{
	return _widget_table_natcmp(clist, ptr1, ptr2, TRUE);
}

gint widget_table_natcasecmp(GtkCList *clist, gconstpointer ptr1,
	gconstpointer ptr2)
{
	return _widget_table_natcmp(clist, ptr1, ptr2, FALSE);
}

static gint _widget_table_natcmp(GtkCList *clist, gconstpointer ptr1,
	gconstpointer ptr2, gint sensitive)
{
	const GtkCListRow *row1 = (const GtkCListRow *) ptr1;
	const GtkCListRow *row2 = (const GtkCListRow *) ptr2;
	gchar            *r1 = NULL;
	gchar            *r2 = NULL;
	gint              retval;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	r1 = GTK_CELL_TEXT(row1->cell[clist->sort_column])->text;
	r2 = GTK_CELL_TEXT(row2->cell[clist->sort_column])->text;

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s(): r1=\"%s\" r2=\"%s\"\n", __func__, r1, r2);
#endif

	retval = strnatcmp(r1, r2, sensitive);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return retval;
}
