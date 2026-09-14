/*
 * widget_menuitem.c: 
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
#include <errno.h>
#include "config.h"
#include "gtkdialog.h"
#include "attributes.h"
#include "automaton.h"
#include "widgets.h"
#include "signals.h"
#include "tag_attributes.h"
#include "widget_menuitem.h"

/* Defines */
//#define DEBUG_CONTENT
//#define DEBUG_TRANSITS

/* Local function prototypes, located at file bottom */
static void widget_menuitem_input_by_command(variable *var, char *command);
static void widget_menuitem_input_by_file(variable *var, char *filename);
static void widget_menuitem_input_by_items(variable *var);

#define CONTEXT_MENU_DATA "gtkdialog-context-menu"

typedef struct {
	GtkWidget *menu;
	gint window_id;
	gboolean destroyed;
} ContextMenuData;

typedef struct {
	GtkWidget *owner;
	GtkMenuPositionFunc position_func;
	gpointer position_data;
	guint button;
	guint32 event_time;
} PendingContextMenu;

typedef struct {
	GtkWidget *menu;
	gint window_id;
	gboolean terminate_program;
	gboolean exit_scheduled;
} PopupProgramData;

static PendingContextMenu pending_context_menu = {
	NULL, NULL, NULL, 0, 0
};

static void widget_context_menu_destroyed(GtkWidget *menu, gpointer data)
{
	ContextMenuData *context = data;

	(void)menu;
	context->destroyed = TRUE;
}

static void widget_menu_release_accel_groups(void)
{
	GList *element;

	for (element = accel_groups; element != NULL; element = element->next)
		g_object_unref(element->data);
	g_list_free(accel_groups);
	accel_groups = NULL;
}

static void widget_context_menu_data_destroy(gpointer data)
{
	ContextMenuData *context = data;

	variables_drop_by_window_id(NULL, context->window_id);
	if (!context->destroyed)
		gtk_widget_destroy(context->menu);
	g_object_unref(context->menu);
	g_free(context);
}

static gboolean widget_popup_program_exit(gpointer data)
{
	PopupProgramData *popup = data;

	if (popup->terminate_program) {
		print_variables(NULL);
		printf("EXIT=\"popupmenu\"\n");
		exit(EXIT_SUCCESS);
	}

	variables_drop_by_window_id(NULL, popup->window_id);
	gtk_widget_destroy(popup->menu);
	g_object_unref(popup->menu);
	g_free(popup);
	return FALSE;
}

static void widget_popup_program_deactivated(GtkWidget *menu, gpointer data)
{
	PopupProgramData *popup = data;

	(void)menu;
	if (!popup->exit_scheduled) {
		popup->exit_scheduled = TRUE;
		g_idle_add(widget_popup_program_exit, popup);
	}
}

static gboolean widget_menuitem_parse_accel_value(const gchar *value,
	guint minimum, guint *result, const gchar *attribute)
{
	const gchar *start;
	gchar *end;
	guint64 number;
	guint base;

	start = value;
	while (g_ascii_isspace(*start))
		++start;
	base = start[0] == '0' && (start[1] == 'x' || start[1] == 'X') ? 16 : 10;

	errno = 0;
	number = g_ascii_strtoull(start, &end, base);
	while (g_ascii_isspace(*end))
		++end;
	if (*start == '-' || errno != 0 || end == start || *end != '\0' ||
		number < minimum || number > G_MAXUINT) {
		gtkdialog_warning("Invalid menuitem %s value '%s'; accelerator ignored.",
			attribute, value);
		return FALSE;
	}

	*result = (guint)number;
	return TRUE;
}

/* Notes:
 * The following widgets are maintained here:
 * 
 * menu - a GtkMenu attached to a GtkMenuItem
 * menuitem  - a GtkMenuItem, GtkCheckMenuItem, GtkRadioMenuItem or GtkImageMenuItem
 * menuitemseparator - a GtkSeparatorMenuItem
 * 
 * Apart from GtkMenu which is the menu shell, the rest are all
 * descendants of GtkMenuItem and simply variations of the same thing.
 */

/***********************************************************************
 * Clear                                                               *
 ***********************************************************************/

void widget_menuitem_clear(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Clear not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Create                                                              *
 ***********************************************************************/

GtkWidget *widget_menu_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	GtkWidget        *menu;
	GtkWidget        *widget;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	menu = widget_menu_shell_create(attr);

	attributeset_set_if_unset(Attr, ATTR_LABEL, "menu");

	/* Thunor: A menu widget is in fact a menuitem widget with a submenu
	 * so it'll be created just like any other menuitem widget. It's
	 * possible to create image, checkbox or radiobutton menu widgets but
	 * their usefulness is questionable. In fact creating a menu widget
	 * closes any open radiobutton group so that makes that pointless */
	widget = widget_menuitem_create(Attr, attr, WIDGET_MENU);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget), menu);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

GtkWidget *widget_popupmenu_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	(void)Attr;
	(void)Type;
	return widget_menu_shell_create(attr);
}

gboolean widget_menu_popup_context(GtkWidget *owner,
	const gchar *program_name, guint button, guint32 event_time)
{
	return widget_menu_popup_context_positioned(owner, program_name,
		NULL, NULL, button, event_time);
}

gboolean widget_menu_popup_context_positioned(GtkWidget *owner,
	const gchar *program_name, GtkMenuPositionFunc position_func,
	gpointer position_data, guint button, guint32 event_time)
{
	ContextMenuData *context;
	extern int instruction_counter;

	g_return_val_if_fail(GTK_IS_WIDGET(owner), FALSE);
	if (program_name == NULL || *program_name == '\0')
		return FALSE;

	context = g_object_get_data(G_OBJECT(owner), CONTEXT_MENU_DATA);
	if (context != NULL && !context->destroyed) {
		gtk_menu_popup(GTK_MENU(context->menu), NULL, NULL, position_func,
			position_data, button, event_time);
		return TRUE;
	}

	if (instruction_counter != 0 || pending_context_menu.owner != NULL) {
		gtkdialog_warning("Can't open context menu '%s' while a program is "
			"being built.", program_name);
		return FALSE;
	}
	if (g_getenv(program_name) == NULL) {
		gtkdialog_warning("Context menu description '%s' is not present in "
			"the environment.", program_name);
		return FALSE;
	}

	pending_context_menu.owner = g_object_ref(owner);
	pending_context_menu.position_func = position_func;
	pending_context_menu.position_data = position_data;
	pending_context_menu.button = button;
	pending_context_menu.event_time = event_time;
	get_program_from_variable(program_name);
	variables_export_all();
	gtkdialog_parse();

	context = g_object_get_data(G_OBJECT(owner), CONTEXT_MENU_DATA);
	return context != NULL && !context->destroyed;
}

gboolean widget_menu_popup_finish(GtkWidget *root, gint popup_window_id)
{
	ContextMenuData *context;
	PopupProgramData *popup;
	GtkWidget *owner;

	if (pending_context_menu.owner != NULL) {
		owner = pending_context_menu.owner;
		if (!GTK_IS_MENU(root)) {
			gtkdialog_warning("A context menu program must have a "
				"<popupmenu> root.");
			variables_drop_by_window_id(NULL, popup_window_id);
			if (GTK_IS_WIDGET(root))
				gtk_widget_destroy(root);
			g_object_unref(owner);
			pending_context_menu.owner = NULL;
			pending_context_menu.position_func = NULL;
			pending_context_menu.position_data = NULL;
			return TRUE;
		}

		context = g_new0(ContextMenuData, 1);
		context->menu = g_object_ref_sink(root);
		context->window_id = popup_window_id;
		g_signal_connect(G_OBJECT(root), "destroy",
			G_CALLBACK(widget_context_menu_destroyed), context);
		gtk_menu_attach_to_widget(GTK_MENU(root), owner, NULL);
		g_object_set_data_full(G_OBJECT(owner), CONTEXT_MENU_DATA, context,
			widget_context_menu_data_destroy);
		widget_menu_release_accel_groups();
		gtk_menu_popup(GTK_MENU(root), NULL, NULL,
			pending_context_menu.position_func,
			pending_context_menu.position_data,
			pending_context_menu.button, pending_context_menu.event_time);
		g_object_unref(owner);
		pending_context_menu.owner = NULL;
		pending_context_menu.position_func = NULL;
		pending_context_menu.position_data = NULL;
		return TRUE;
	}

	if (!GTK_IS_MENU(root))
		return FALSE;

	popup = g_new0(PopupProgramData, 1);
	popup->menu = g_object_ref_sink(root);
	popup->window_id = popup_window_id;
	popup->terminate_program = gtk_main_level() == 0;
	widget_menu_release_accel_groups();
	g_signal_connect(G_OBJECT(root), "deactivate",
		G_CALLBACK(widget_popup_program_deactivated), popup);
	g_signal_connect(G_OBJECT(root), "destroy",
		G_CALLBACK(widget_popup_program_deactivated), popup);
	gtk_menu_popup(GTK_MENU(root), NULL, NULL, NULL, NULL, 0,
		gtk_get_current_event_time());
	return TRUE;
}

GtkWidget *widget_menu_shell_create(tag_attr *attr)
{
	GtkAccelGroup *accel_group;
	GtkWidget *menu;
	stackelement children;
	gint n;

	menu = gtk_menu_new();

	/* Each menu needs an accelerator group.  The window is built last, so
	 * retain the groups temporarily and attach them in widget_window_create. */
	accel_group = gtk_accel_group_new();
	gtk_menu_set_accel_group(GTK_MENU(menu), accel_group);
	accel_groups = g_list_append(accel_groups, accel_group);

#ifdef DEBUG_CONTENT
	fprintf(stderr, "%s: Appending accel_group=%p to GList\n",
		__func__, accel_group);
#endif

	children = pop();
	for (n = 0; n < children.nwidgets; ++n)
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), children.widgets[n]);
	stackelement_clear(&children);

	widget_visibility_list_add(menu, attr);
	return menu;
}

GtkWidget *widget_menuitem_create(
	AttributeSet *Attr, tag_attr *attr, gint Type)
{
	#define           TYPE_MENUITEM 0
	#define           TYPE_MENUITEM_IMAGE_STOCK 1
	#define           TYPE_MENUITEM_IMAGE_ICON 2
	#define           TYPE_MENUITEM_IMAGE_FILE 3
	#define           TYPE_MENUITEM_CHECK 4
	#define           TYPE_MENUITEM_RADIO 5
	#define           TYPE_MENUITEM_SEPARATOR 6
	GdkPixbuf        *pixbuf;
	GError           *error = NULL;
	GList            *element;
	GtkIconTheme     *icon_theme;
	GtkWidget        *image;
	GtkWidget        *widget;
	gchar             accel_path[64];
	gchar            *active = "false";
	gchar            *accel_key_value, *accel_mods_value;
	gchar            *icon_name, *image_name;
	gchar            *label, *stock_id;
	gchar            *resolved_image;
	gchar            *value;
	gint              is_active;
	gint              menuitemtype = TYPE_MENUITEM;
	gint              width = -1, height = -1, size = 16;
	guint             accel_key = 0, accel_mods = 0, custom_accel = 0;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* We need to decode exactly what it is the user is trying to create
	 * and then make the right widget.
	 * 
	 * Originally as I couldn't use "stock", "icon" and "image" (image
	 * is a GTK+ property) I decided to make the meaningful names of
	 * "image-stock", "image-icon" and "image-file", but then I found that
	 * "stock[_id]" and "icon[_name]" were already being used within the
	 * tree widget (I missed that) so I'll document the latter but accept
	 * everything below (I'll have to now as they're likely being used) */
	if (Type == WIDGET_MENUITEMSEPARATOR) {
		menuitemtype = TYPE_MENUITEM_SEPARATOR;
	} else if (attr &&
		(stock_id = get_tag_attribute(attr, "label")) &&
		((value = get_tag_attribute(attr, "use-stock")) &&
		widget_attribute_is_true(value))) {
		menuitemtype = TYPE_MENUITEM_IMAGE_STOCK;
	} else if (attr &&
		((stock_id = get_tag_attribute(attr, "stock")) ||
		(stock_id = get_tag_attribute(attr, "stock-id")) ||
		(stock_id = get_tag_attribute(attr, "image-stock")))) {	/* I don't want to keep this temp temp */
		menuitemtype = TYPE_MENUITEM_IMAGE_STOCK;
	} else if (attr &&
		((icon_name = get_tag_attribute(attr, "icon")) ||
		(icon_name = get_tag_attribute(attr, "icon-name")) ||
		(icon_name = get_tag_attribute(attr, "image-icon")))) {	/* I don't want to keep this temp temp */
		menuitemtype = TYPE_MENUITEM_IMAGE_ICON;
	} else if (attr &&
		((image_name = get_tag_attribute(attr, "image-name")) ||
		(image_name = get_tag_attribute(attr, "image-file")))) {	/* I don't want to keep this temp temp */
		menuitemtype = TYPE_MENUITEM_IMAGE_FILE;
	} else if (attr &&
		(active = get_tag_attribute(attr, "checkbox"))) {
		menuitemtype = TYPE_MENUITEM_CHECK;
	} else if (attr &&
		(active = get_tag_attribute(attr, "radiobutton"))) {
		menuitemtype = TYPE_MENUITEM_RADIO;
	} else {
		menuitemtype = TYPE_MENUITEM;
	}

	/* Read declared directives */
	/* Only separator menuitems don't support this */
	if (menuitemtype != TYPE_MENUITEM_SEPARATOR) {
		/* Set a default label */
		attributeset_set_if_unset(Attr, ATTR_LABEL, "menuitem");
		label = attributeset_get_first(&element, Attr, ATTR_LABEL);
	} else {
		if (attributeset_is_avail(Attr, ATTR_LABEL)) {
			fprintf(stderr, "%s(): <label> not implemented for this widget.\n",
				__func__);
		}
	}
	/* Only image menuitems from theme or file support this */
	if (attributeset_is_avail(Attr, ATTR_HEIGHT)) {
		if (menuitemtype == TYPE_MENUITEM_IMAGE_ICON ||
			menuitemtype == TYPE_MENUITEM_IMAGE_FILE) {
			height = widget_parse_size(attributeset_get_first(&element, Attr,
				ATTR_HEIGHT), "menuitem height");
		} else {
			fprintf(stderr, "%s(): <height> not implemented for this widget.\n",
				__func__);
		}
	}
	/* Only image menuitems from theme or file support this */
	if (attributeset_is_avail(Attr, ATTR_WIDTH)) {
		if (menuitemtype == TYPE_MENUITEM_IMAGE_ICON ||
			menuitemtype == TYPE_MENUITEM_IMAGE_FILE) {
			width = widget_parse_size(attributeset_get_first(&element, Attr,
				ATTR_WIDTH), "menuitem width");
		} else {
			fprintf(stderr, "%s(): <width> not implemented for this widget.\n",
				__func__);
		}
	}

	/* Thunor: We can add an accelerator for this menuitem if both
	 * "accel-key" and "accel-mods" are valid custom tag attributes.
	 * Note that because these widgets are created and pushed when the
	 * end tags are detected, everything gets done in reverse! So here
	 * the menuitems are created with possibly an accelerator, then
	 * when the menu end tag is detected the menu and accelerator group
	 * are created and finally the menuitems are appended to the menu */

	/* Only separator menuitems don't support this */
	if (menuitemtype != TYPE_MENUITEM_SEPARATOR) {
		if (attr) {
			accel_key_value = get_tag_attribute(attr, "accel-key");
			accel_mods_value = get_tag_attribute(attr, "accel-mods");
			if (accel_key_value && accel_mods_value &&
				widget_menuitem_parse_accel_value(accel_key_value, 1,
					&accel_key, "accel-key") &&
				widget_menuitem_parse_accel_value(accel_mods_value, 0,
					&accel_mods, "accel-mods")) {
				/* Create a random accel-path (yeah, this is fine) */
				g_snprintf(accel_path, sizeof(accel_path), "<%i>/%i",
					rand(), rand());
				custom_accel = TRUE;
#ifdef DEBUG_CONTENT
				fprintf(stderr, "%s: accel-key=%u\n", __func__, accel_key);
				fprintf(stderr, "%s: accel-mods=%u\n", __func__, accel_mods);
				fprintf(stderr, "%s: accel-path=%s\n", __func__, accel_path);
#endif
			}
		}
	}

	/* Create the menuitem */
	switch (menuitemtype) {
		case TYPE_MENUITEM_SEPARATOR:
			widget = gtk_separator_menu_item_new();
			break;
		case TYPE_MENUITEM_IMAGE_STOCK:
			/* Do not install the stock accelerator automatically: existing
			 * dialogs may already use that key combination for another action. */
			widget = gtk_image_menu_item_new_from_stock(stock_id, NULL);
			break;
		case TYPE_MENUITEM_IMAGE_ICON:
			icon_theme = gtk_icon_theme_get_default();
			/* Use the height or width dimension to override the default size */
			if (height > -1) size = height;
			else if (width > -1) size = width;
			pixbuf = gtk_icon_theme_load_icon(icon_theme, icon_name,
				size, 0, &error);
			if (pixbuf) {
				image = gtk_image_new_from_pixbuf(pixbuf);
				/* pixbuf is no longer required and should be unreferenced */
				g_object_unref(pixbuf);
			} else {
				widget_report_icon_error(icon_name, error);
				/* pixbuf is null (file not found) so by using this
				 * function gtk will substitute a broken image icon */
				image = gtk_image_new_from_file("");
			}
			/* Create the GtkImageMenuItem using an image from the theme */
			widget = gtk_image_menu_item_new_with_label(label);
			gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), image);
			break;
		case TYPE_MENUITEM_IMAGE_FILE:
			resolved_image = find_pixmap(image_name);
			if (width == -1 && height == -1) {
				/* Handle unscaled images */
				image = gtk_image_new_from_file(resolved_image);
			} else {
				/* Handle scaled images */
				pixbuf = gdk_pixbuf_new_from_file_at_size(
					resolved_image, width, height, NULL);
				if (pixbuf) {
					image = gtk_image_new_from_pixbuf(pixbuf);
					/* pixbuf is no longer required and should be unreferenced */
					g_object_unref(pixbuf);
				} else {
					/* pixbuf is null (file not found) so by using this
					* function gtk will substitute a broken image icon */
					image = gtk_image_new_from_file("");
				}
			}
			g_free(resolved_image);
			/* Create the GtkImageMenuItem using an image from a file */
			widget = gtk_image_menu_item_new_with_label(label);
			gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(widget), image);
			break;
		case TYPE_MENUITEM_CHECK:
			/* Create the GtkCheckMenuItem */
			widget = gtk_check_menu_item_new_with_label(label);
			/* Get the active state */
			if (widget_attribute_is_true(active)) {
				is_active = 1;
			} else {
				is_active = 0;
			}
			/* Set the active state */
			gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), is_active);
			break;
		case TYPE_MENUITEM_RADIO:
			/* Create the GtkRadioMenuItem */
			if (lastradiowidget == NULL) {
				widget = gtk_radio_menu_item_new_with_label(NULL, label);
				lastradiowidget = widget;
			} else {
				widget = gtk_radio_menu_item_new_with_label_from_widget(
					GTK_RADIO_MENU_ITEM(lastradiowidget), label);
			}
			/* Get the active state */
			if (widget_attribute_is_true(active)) {
				is_active = 1;
			} else {
				is_active = 0;
			}
			/* Set the active state (yeah, it uses the same base class function) */
			gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), is_active);
			break;
		case TYPE_MENUITEM:
		default:
			/* Create the GtkMenuItem */
			widget = gtk_menu_item_new_with_label(label);
			break;
	}

	/* Are we setting an accelerator? */
	if (custom_accel) {
		/* Register and set the accelerator path on the menuitem */
		gtk_accel_map_add_entry(accel_path, accel_key, accel_mods);
		gtk_menu_item_set_accel_path(GTK_MENU_ITEM(widget), accel_path);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return widget;
}

/***********************************************************************
 * Environment Variable Construct                                      *
 ***********************************************************************/

gchar *widget_menuitem_envvar_construct(GtkWidget *widget)
{
	gchar            *string;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Only checkbox and radiobutton menuitems support this */
	if (GTK_IS_CHECK_MENU_ITEM(widget)) {
		if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
			string = g_strdup("true");
		} else {
			string = g_strdup("false");
		}
	} else {
		string = g_strdup("");
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif

	return string;
}

/***********************************************************************
 * Fileselect                                                          *
 ***********************************************************************/

void widget_menuitem_fileselect(
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

void widget_menuitem_refresh(variable *var)
{
	GdkPixbuf        *pixbuf;
	GList            *element;
	GtkWidget        *image;
	gchar            *act;
	gchar            *resolved_image;
	gchar            *value, *image_name;
	gint              initialised = FALSE;
	gint              width = -1, height = -1;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Get initialised state of widget */
	if (g_object_get_data(G_OBJECT(var->Widget), "_initialised") != NULL)
		initialised = GPOINTER_TO_INT(g_object_get_data(
			G_OBJECT(var->Widget), "_initialised"));

	/* tag_attr="value"... */
	/* Only image menuitems from file support this.
	 * GTK+ destroys the original image when a new one is set */
	if (GTK_IS_IMAGE_MENU_ITEM(var->Widget)) {
		if (var->widget_tag_attr &&
			((image_name = get_tag_attribute(var->widget_tag_attr, "image-name")) ||
			(image_name = get_tag_attribute(var->widget_tag_attr, "image-file")))) {
			if (!initialised) {
				/* Check for file-monitor and create if requested */
				widget_file_monitor_try_create(var, image_name);
			}
			/* Don't refresh images on the first call otherwise they
			 * get created and then immediately refreshed at start-up */
			if (initialised) {
				resolved_image = find_pixmap(image_name);
				if (attributeset_is_avail(var->Attributes, ATTR_WIDTH))
					width = widget_parse_size(attributeset_get_first(&element,
						var->Attributes, ATTR_WIDTH), "menuitem width");
				if (attributeset_is_avail(var->Attributes, ATTR_HEIGHT))
					height = widget_parse_size(attributeset_get_first(&element,
						var->Attributes, ATTR_HEIGHT), "menuitem height");
				if (width == -1 && height == -1) {
					/* Handle unscaled images */
					image = gtk_image_new_from_file(resolved_image);
				} else {
					/* Handle scaled images */
					pixbuf = gdk_pixbuf_new_from_file_at_size(
						resolved_image, width, height, NULL);
					if (pixbuf) {
						image = gtk_image_new_from_pixbuf(pixbuf);
						/* pixbuf is no longer required and should be unreferenced */
						g_object_unref(pixbuf);
					} else {
						/* pixbuf is null (file not found) so by using this
						* function gtk will substitute a broken image icon */
						image = gtk_image_new_from_file("");
					}
				}
				gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(
					var->Widget), image);
				g_free(resolved_image);
			}
		}
	}

	/* The <input> tag... */
	act = attributeset_get_first(&element, var->Attributes, ATTR_INPUT);
	while (act) {
		if (input_is_shell_command(act))
			widget_menuitem_input_by_command(var, act + 8);
		/* input file stock = "File:", input file = "File:/path/to/file" */
		if (strncasecmp(act, "file:", 5) == 0 && strlen(act) > 5) {
			/* Only checkbox and radiobutton menuitems support this */
			if (GTK_IS_CHECK_MENU_ITEM(var->Widget)) {
				if (!initialised) {
					/* Check for file-monitor and create if requested */
					widget_file_monitor_try_create(var, act + 5);
				}
			}
			widget_menuitem_input_by_file(var, act + 5);
		}
		act = attributeset_get_next(&element, var->Attributes, ATTR_INPUT);
	}

	/* The <item> tags... */
	if (attributeset_is_avail(var->Attributes, ATTR_ITEM))
		widget_menuitem_input_by_items(var);

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
		/* The GTK "sensitive" property (if present) will be set later after
		 * widget realization, but this doesn't have any effect until after
		 * the menu has been opened by the user which results in accelerators
		 * being live up until that point. I don't know why that is -- it 
		 * looks as though GTK is initialising the menus on first opening as
		 * they render much quicker on subsequent openings -- but it can be
		 * dealt with by applying the property right here right now.
		 * Note that I'm applying this after any sensitive directive which
		 * would be the normal sequence of things.
		 * 
		 * [UPDATE]
		 * Menuitems are realized when the menu is opened which could be a
		 * problem since then tag attributes that are GTK properties will
		 * sit there waiting to be applied at some later time. Something is
		 * going to have to be done about the on_any_widget_realized method
		 * of setting tag attributes because it will be affecting other widgets
		 * such as the notebook including all of the widgets within that!
		 * When that's dealt with I can remove the following code temp temp */
		if (var->widget_tag_attr &&
			(value = get_tag_attribute(var->widget_tag_attr, "sensitive")) &&
			widget_attribute_is_false(value)) {
			gtk_widget_set_sensitive(var->Widget, FALSE);
		}

		/* Connect signals */
		/* Only checkbox and radiobutton menuitems support this */
		if (GTK_IS_CHECK_MENU_ITEM(var->Widget)) {
			g_signal_connect(G_OBJECT(var->Widget), "toggled",
				G_CALLBACK(on_any_widget_toggled_event), (gpointer)var->Attributes);
		}
		/* Only separator menuitems don't support this */
		if (!GTK_IS_SEPARATOR_MENU_ITEM(var->Widget)) {
			g_signal_connect(G_OBJECT(var->Widget), "activate",
				G_CALLBACK(on_any_widget_activate_event), (gpointer)var->Attributes);
		}

	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Removeselected                                                      *
 ***********************************************************************/

void widget_menuitem_removeselected(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): Removeselected not implemented for this widget.\n",
		__func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Save                                                                *
 ***********************************************************************/

void widget_menuitem_save(variable *var)
{
	FILE             *outfile;
	GList            *element;
	gchar            *act;
	gchar            *filename = NULL;
	gint              is_active;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Only checkbox and radiobutton menuitems support this */
	if (GTK_IS_CHECK_MENU_ITEM(var->Widget)) {

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

				is_active = gtk_check_menu_item_get_active(
					GTK_CHECK_MENU_ITEM(var->Widget));
				if (is_active) fprintf(outfile, "%s", "true");
				else fprintf(outfile, "%s", "false");

				widget_close_output(outfile, filename);
			} else {
				fprintf(stderr, "%s(): Couldn't open '%s' for writing.\n",
					__func__, filename);
			}
		} else {
			fprintf(stderr, "%s(): No <output file> directive found.\n",
				__func__);
		}

	} else {
		fprintf(stderr, "%s(): Save not implemented for this widget.\n",
			__func__);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Command                                                    *
 ***********************************************************************/

static void widget_menuitem_input_by_command(variable *var, char *command)
{
	FILE             *infile;
	gchar            *line;
	gint              is_active;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Only checkbox and radiobutton menuitems support this */
	if (GTK_IS_CHECK_MENU_ITEM(var->Widget)) {

		/* Opening pipe for reading... */
		if (infile = widget_opencommand(command)) {
			/* Just one line */
			if ((line = widget_read_line(infile))) {
				is_active = widget_parse_legacy_input_boolean(line);
				gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(
					var->Widget), is_active);
				g_free(line);
			}
			/* Close the file */
			widget_closecommand(infile, command);
		} else {
			fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
				__func__, command);
		}

	} else {
		fprintf(stderr, "%s(): <input> not implemented for this widget.\n",
			__func__);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by File                                                       *
 ***********************************************************************/

static void widget_menuitem_input_by_file(variable *var, char *filename)
{
	FILE             *infile;
	gchar            *line;
	gint              is_active;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	/* Only checkbox and radiobutton menuitems support this */
	if (GTK_IS_CHECK_MENU_ITEM(var->Widget)) {

		if (infile = fopen(filename, "r")) {
			/* Just one line */
			if ((line = widget_read_line(infile))) {
				is_active = widget_parse_legacy_input_boolean(line);
				gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(
					var->Widget), is_active);
				g_free(line);
			}
			/* Close the file */
			fclose(infile);
		} else {
			fprintf(stderr, "%s(): Couldn't open '%s' for reading.\n",
				__func__, filename);
		}

	} else {
		fprintf(stderr, "%s(): <input file> not implemented for this widget.\n",
			__func__);
	}

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}

/***********************************************************************
 * Input by Items                                                      *
 ***********************************************************************/

static void widget_menuitem_input_by_items(variable *var)
{
	gchar            *var1;
	gint              var2;

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Entering.\n", __func__);
#endif

	fprintf(stderr, "%s(): <item> not implemented for this widget.\n", __func__);

#ifdef DEBUG_TRANSITS
	fprintf(stderr, "%s(): Exiting.\n", __func__);
#endif
}
