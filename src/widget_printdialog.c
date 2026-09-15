/*
 * widget_printdialog.c: GTK2 Unix print dialog widget.
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
#if HAVE_GTK_UNIX_PRINT
#include <gtk/gtkunixprint.h>
#endif

#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widgets.h"
#include "widget_dialog.h"
#include "widget_printdialog.h"
#include "widget_printing.h"

#define PRINT_RESPONSE_DATA "gtkdialog-printdialog-response"
#define PAGE_SETUP_FILE_DATA "gtkdialog-printdialog-page-file"
#define PRINT_SETTINGS_FILE_DATA "gtkdialog-printdialog-settings-file"

extern gboolean option_centering;
extern gboolean have_geometry_xy;
extern gboolean have_geometry_dxdy;
extern gint geometry_dx;
extern gint geometry_dy;
extern gint geometry_x;
extern gint geometry_y;

static void widget_printdialog_store_response(GtkWidget *widget, gint response)
{
	g_object_set_data_full(G_OBJECT(widget), PRINT_RESPONSE_DATA,
		widget_dialog_response_name(response), g_free);
}

static gboolean widget_printdialog_set_response(GtkWidget *widget,
	const gchar *value, gboolean emit)
{
	gint response;

	if (!widget_dialog_parse_response(value, &response)) {
		gtkdialog_warning("Invalid print dialog response '%s'; ignored.",
			value != NULL ? value : "");
		return FALSE;
	}
	if (emit)
		gtk_dialog_response(GTK_DIALOG(widget), response);
	else
		widget_printdialog_store_response(widget, response);
	return TRUE;
}

#if HAVE_GTK_UNIX_PRINT && GTK_CHECK_VERSION(2,10,0)
static void widget_printdialog_response(GtkDialog *dialog, gint response,
	AttributeSet *attributes)
{
	widget_printdialog_store_response(GTK_WIDGET(dialog), response);
	widget_signal_executor(GTK_WIDGET(dialog), attributes, "response");
}

static GtkPrintCapabilities widget_printdialog_capabilities(
	const gchar *value)
{
	GtkPrintCapabilities capabilities = 0;
	gchar **names;
	guint index;

	if (value == NULL)
		return capabilities;
	names = g_strsplit_set(value, "|,; \t", -1);
	for (index = 0; names[index] != NULL; ++index) {
		const gchar *name = names[index];

		if (name[0] == '\0' || g_ascii_strcasecmp(name, "none") == 0)
			continue;
		if (g_ascii_strcasecmp(name, "all") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_PAGE_SET |
				GTK_PRINT_CAPABILITY_COPIES |
				GTK_PRINT_CAPABILITY_COLLATE |
				GTK_PRINT_CAPABILITY_REVERSE |
				GTK_PRINT_CAPABILITY_SCALE |
				GTK_PRINT_CAPABILITY_GENERATE_PDF |
				GTK_PRINT_CAPABILITY_GENERATE_PS |
				GTK_PRINT_CAPABILITY_PREVIEW |
				GTK_PRINT_CAPABILITY_NUMBER_UP |
				GTK_PRINT_CAPABILITY_NUMBER_UP_LAYOUT;
		} else if (g_ascii_strcasecmp(name, "page-set") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_PAGE_SET;
		} else if (g_ascii_strcasecmp(name, "copies") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_COPIES;
		} else if (g_ascii_strcasecmp(name, "collate") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_COLLATE;
		} else if (g_ascii_strcasecmp(name, "reverse") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_REVERSE;
		} else if (g_ascii_strcasecmp(name, "scale") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_SCALE;
		} else if (g_ascii_strcasecmp(name, "generate-pdf") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_GENERATE_PDF;
		} else if (g_ascii_strcasecmp(name, "generate-ps") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_GENERATE_PS;
		} else if (g_ascii_strcasecmp(name, "preview") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_PREVIEW;
		} else if (g_ascii_strcasecmp(name, "number-up") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_NUMBER_UP;
		} else if (g_ascii_strcasecmp(name, "number-up-layout") == 0) {
			capabilities |= GTK_PRINT_CAPABILITY_NUMBER_UP_LAYOUT;
		} else {
			gtkdialog_warning("Unknown print capability '%s'; ignored.", name);
		}
	}
	g_strfreev(names);
	return capabilities;
}

static void widget_printdialog_apply_attributes(GtkWidget *widget,
	tag_attr *attributes)
{
	gchar *value;

	if (attributes == NULL)
		return;
	value = get_tag_attribute(attributes, "current-page");
	if (value != NULL)
		gtk_print_unix_dialog_set_current_page(
			GTK_PRINT_UNIX_DIALOG(widget),
			widget_parse_bounded_integer(value, -1, G_MAXINT, -1,
				"printdialog current-page"));
	value = get_tag_attribute(attributes, "manual-capabilities");
	if (value != NULL)
		gtk_print_unix_dialog_set_manual_capabilities(
			GTK_PRINT_UNIX_DIALOG(widget),
			widget_printdialog_capabilities(value));
	value = get_tag_attribute(attributes, "support-selection");
	if (value != NULL)
		gtk_print_unix_dialog_set_support_selection(
			GTK_PRINT_UNIX_DIALOG(widget), widget_attribute_is_true(value));
	value = get_tag_attribute(attributes, "has-selection");
	if (value != NULL)
		gtk_print_unix_dialog_set_has_selection(
			GTK_PRINT_UNIX_DIALOG(widget), widget_attribute_is_true(value));
	value = get_tag_attribute(attributes, "embed-page-setup");
	if (value != NULL)
		gtk_print_unix_dialog_set_embed_page_setup(
			GTK_PRINT_UNIX_DIALOG(widget), widget_attribute_is_true(value));

	kill_tag_attribute(attributes, "current-page");
	kill_tag_attribute(attributes, "manual-capabilities");
	kill_tag_attribute(attributes, "support-selection");
	kill_tag_attribute(attributes, "has-selection");
	kill_tag_attribute(attributes, "embed-page-setup");
}

static void widget_printdialog_reload_files(GtkWidget *widget)
{
	GtkPageSetup *setup;
	GtkPrintSettings *settings;
	const gchar *filename;

	filename = g_object_get_data(G_OBJECT(widget), PAGE_SETUP_FILE_DATA);
	if (filename != NULL) {
		setup = widget_printing_load_page_setup(filename);
		if (setup != NULL) {
			gtk_print_unix_dialog_set_page_setup(
				GTK_PRINT_UNIX_DIALOG(widget), setup);
			g_object_unref(setup);
		}
	}
	filename = g_object_get_data(G_OBJECT(widget), PRINT_SETTINGS_FILE_DATA);
	if (filename != NULL) {
		settings = widget_printing_load_print_settings(filename);
		if (settings != NULL) {
			gtk_print_unix_dialog_set_settings(
				GTK_PRINT_UNIX_DIALOG(widget), settings);
			g_object_unref(settings);
		}
	}
}
#endif

GtkWidget *widget_printdialog_create(AttributeSet *attributes,
	tag_attr *tag_attributes, gint type)
{
#if HAVE_GTK_UNIX_PRINT && GTK_CHECK_VERSION(2,10,0)
	GtkPageSetup *setup;
	GtkPrintSettings *settings;
	GtkWidget *widget;
	gchar *filename;

	(void)attributes;
	(void)type;
	widget = gtk_print_unix_dialog_new(PACKAGE, NULL);
	g_object_set_data_full(G_OBJECT(widget), PRINT_RESPONSE_DATA,
		g_strdup(""), g_free);

	setup = gtk_page_setup_new();
	filename = tag_attributes != NULL
		? get_tag_attribute(tag_attributes, "page-setup-file") : NULL;
	if (filename != NULL) {
		GtkPageSetup *loaded = widget_printing_load_page_setup(filename);

		g_object_set_data_full(G_OBJECT(widget), PAGE_SETUP_FILE_DATA,
			g_strdup(filename), g_free);
		if (loaded != NULL) {
			g_object_unref(setup);
			setup = loaded;
		}
	}
	widget_printing_apply_page_attributes(setup, tag_attributes);
	gtk_print_unix_dialog_set_page_setup(GTK_PRINT_UNIX_DIALOG(widget), setup);
	g_object_unref(setup);

	settings = gtk_print_settings_new();
	filename = tag_attributes != NULL
		? get_tag_attribute(tag_attributes, "print-settings-file") : NULL;
	if (filename != NULL) {
		GtkPrintSettings *loaded;

		g_object_set_data_full(G_OBJECT(widget), PRINT_SETTINGS_FILE_DATA,
			g_strdup(filename), g_free);
		loaded = widget_printing_load_print_settings(filename);
		if (loaded != NULL) {
			g_object_unref(settings);
			settings = loaded;
		}
	}
	gtk_print_unix_dialog_set_settings(GTK_PRINT_UNIX_DIALOG(widget), settings);
	g_object_unref(settings);
	widget_printdialog_apply_attributes(widget, tag_attributes);
	if (tag_attributes != NULL) {
		kill_tag_attribute(tag_attributes, "page-setup-file");
		kill_tag_attribute(tag_attributes, "print-settings-file");
	}

	if (have_geometry_dxdy)
		gtk_widget_set_size_request(widget, geometry_dx, geometry_dy);
	if (have_geometry_xy)
		gtk_window_move(GTK_WINDOW(widget), geometry_x, geometry_y);
	if (option_centering)
		gtk_window_set_position(GTK_WINDOW(widget), GTK_WIN_POS_CENTER_ALWAYS);
	return widget;
#else
	(void)attributes;
	(void)tag_attributes;
	(void)type;
	g_error("The printdialog widget requires GTK2 Unix printing support.");
	return NULL;
#endif
}

gchar *widget_printdialog_envvar_construct(GtkWidget *widget)
{
	const gchar *response;

	response = g_object_get_data(G_OBJECT(widget), PRINT_RESPONSE_DATA);
	return g_strdup(response != NULL ? response : "");
}

void widget_printdialog_fileselect(variable *var, const char *name,
	const char *value)
{
	(void)name;
	widget_printdialog_set_response(var->Widget, value, TRUE);
}

static void widget_printdialog_input_by_command(variable *var,
	const gchar *command, gboolean emit)
{
	FILE *input;
	gchar *line;

	input = widget_opencommand(command);
	if (input == NULL) {
		gtkdialog_warning("Couldn't run print response input '%s'.", command);
		return;
	}
	line = widget_read_line(input);
	if (line != NULL) {
		widget_printdialog_set_response(var->Widget, line, emit);
		g_free(line);
	}
	widget_closecommand(input, command);
}

static void widget_printdialog_input_by_file(variable *var,
	const gchar *filename, gboolean emit)
{
	FILE *input;
	gchar *line;

	input = fopen(filename, "r");
	if (input == NULL) {
		gtkdialog_warning("Couldn't open print response input '%s'.", filename);
		return;
	}
	line = widget_read_line(input);
	if (line != NULL) {
		widget_printdialog_set_response(var->Widget, line, emit);
		g_free(line);
	}
	fclose(input);
}

void widget_printdialog_refresh(variable *var)
{
#if HAVE_GTK_UNIX_PRINT && GTK_CHECK_VERSION(2,10,0)
	GList *element;
	gchar *input;
	gboolean initialised;

	initialised = GPOINTER_TO_INT(g_object_get_data(
		G_OBJECT(var->Widget), "_initialised"));
	if (initialised)
		widget_printdialog_reload_files(var->Widget);
	input = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (input != NULL) {
		if (input_is_shell_command(input))
			widget_printdialog_input_by_command(var, input + 8, initialised);
		else if (strncasecmp(input, "file:", 5) == 0 && strlen(input) > 5) {
			if (!initialised)
				widget_file_monitor_try_create(var, input + 5);
			widget_printdialog_input_by_file(var, input + 5, initialised);
		}
		input = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}
	if (initialised)
		return;
	if (attributeset_is_avail(var->Attributes, ATTR_DEFAULT))
		widget_printdialog_set_response(var->Widget,
			attributeset_get_first(&element, var->Attributes, ATTR_DEFAULT),
			FALSE);
	if (attributeset_is_avail(var->Attributes, ATTR_LABEL))
		gtkdialog_warning("<label> is not used by printdialog.");
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		gtkdialog_warning("<item> is not used by printdialog.");
	if (attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "false") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "disabled") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "no") ||
		attributeset_cmp_left(var->Attributes, ATTR_SENSITIVE, "0"))
		gtk_widget_set_sensitive(var->Widget, FALSE);
	g_signal_connect(G_OBJECT(var->Widget), "response",
		G_CALLBACK(widget_printdialog_response), var->Attributes);
#else
	(void)var;
#endif
}

void widget_printdialog_save(variable *var)
{
#if HAVE_GTK_UNIX_PRINT && GTK_CHECK_VERSION(2,10,0)
	GError *error;
	GList *element;
	GtkPageSetup *setup;
	GtkPrintSettings *settings;
	GtkPrinter *printer;
	gchar *directive;
	gchar *page_setup_output;
	gchar *printer_output;
	gboolean found = FALSE;

	settings = gtk_print_unix_dialog_get_settings(
		GTK_PRINT_UNIX_DIALOG(var->Widget));
	setup = gtk_print_unix_dialog_get_page_setup(
		GTK_PRINT_UNIX_DIALOG(var->Widget));
	printer = gtk_print_unix_dialog_get_selected_printer(
		GTK_PRINT_UNIX_DIALOG(var->Widget));
	directive = attributeset_get_first(&element, var->Attributes, ATTR_OUTPUT);
	while (directive != NULL) {
		if (strncasecmp(directive, "file:", 5) == 0 && strlen(directive) > 5) {
			found = TRUE;
			error = NULL;
			page_setup_output = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_OUTPUT, "page-setup");
			printer_output = attributeset_get_this_tagattr(&element,
				var->Attributes, ATTR_OUTPUT, "printer");
			if (page_setup_output != NULL && printer_output != NULL &&
				widget_attribute_is_true(page_setup_output) &&
				widget_attribute_is_true(printer_output)) {
				gtkdialog_warning("Print output '%s' cannot be both page setup "
					"and printer name; ignored.", directive + 5);
			} else if (printer_output != NULL &&
				widget_attribute_is_true(printer_output)) {
				const gchar *name = printer != NULL
					? gtk_printer_get_name(printer) : "";

				if (!g_file_set_contents(directive + 5, name, -1, &error))
					gtkdialog_warning("Couldn't save printer name '%s': %s",
						directive + 5, error != NULL
							? error->message : "unknown error");
			} else if (page_setup_output != NULL &&
				widget_attribute_is_true(page_setup_output)) {
				if (setup == NULL) {
					gtkdialog_warning("No page setup is available to save.");
				} else if (!gtk_page_setup_to_file(setup, directive + 5,
					&error)) {
					gtkdialog_warning("Couldn't save page setup '%s': %s",
						directive + 5, error != NULL
							? error->message : "unknown error");
				}
			} else if (settings == NULL) {
				gtkdialog_warning("No print settings are available to save.");
			} else if (!widget_printing_save_print_settings(settings,
				directive + 5, &error)) {
				gtkdialog_warning("Couldn't save print settings '%s': %s",
					directive + 5, error != NULL
						? error->message : "unknown error");
			}
			if (error != NULL)
				g_error_free(error);
		}
		directive = attributeset_get_next(&element,
			var->Attributes, ATTR_OUTPUT);
	}
	if (settings != NULL)
		g_object_unref(settings);
	if (!found)
		gtkdialog_warning("No <output file> directive found for printdialog.");
#else
	(void)var;
#endif
}

void widget_printdialog_clear(variable *var)
{
#if HAVE_GTK_UNIX_PRINT && GTK_CHECK_VERSION(2,10,0)
	GtkPageSetup *setup;
	GtkPrintSettings *settings;

	setup = gtk_page_setup_new();
	gtk_print_unix_dialog_set_page_setup(
		GTK_PRINT_UNIX_DIALOG(var->Widget), setup);
	g_object_unref(setup);
	settings = gtk_print_settings_new();
	gtk_print_unix_dialog_set_settings(
		GTK_PRINT_UNIX_DIALOG(var->Widget), settings);
	g_object_unref(settings);
	gtk_print_unix_dialog_set_current_page(
		GTK_PRINT_UNIX_DIALOG(var->Widget), -1);
#endif
	g_object_set_data_full(G_OBJECT(var->Widget), PRINT_RESPONSE_DATA,
		g_strdup(""), g_free);
}

void widget_printdialog_removeselected(variable *var)
{
	widget_printdialog_clear(var);
}
