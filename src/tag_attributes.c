/*
 * tag_attributes.c:
 * Gtkdialog - A small utility for fast and easy GUI building.
 * Copyright (C) 2003-2007  László Pere <pipas@linux.pte.hu>
 * Copyright (C) 2012       Thunor <thunorsif@hotmail.com>
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

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <errno.h>
#include <math.h>
#include <string.h>
#include "gtkdialog.h"
#include "attributes.h"
#include "tag_attributes.h"

typedef enum {
	PROPERTY_VALUE_OK,
	PROPERTY_VALUE_INVALID,
	PROPERTY_VALUE_UNSUPPORTED
} PropertyValueResult;

extern gboolean option_no_warning;

static gboolean parse_signed_value(const gchar *text, gint64 *value)
{
	gchar *end;

	errno = 0;
	*value = g_ascii_strtoll(text, &end, 0);
	while (g_ascii_isspace(*end))
		++end;

	return errno == 0 && end != text && *end == '\0';
}

static gboolean parse_unsigned_value(const gchar *text, guint64 *value)
{
	gchar *end;
	const gchar *start = text;

	while (g_ascii_isspace(*start))
		++start;
	if (*start == '-')
		return FALSE;

	errno = 0;
	*value = g_ascii_strtoull(text, &end, 0);
	while (g_ascii_isspace(*end))
		++end;

	return errno == 0 && end != text && *end == '\0';
}

static gboolean parse_double_value(const gchar *text, gdouble *value)
{
	gchar *end;

	errno = 0;
	*value = g_ascii_strtod(text, &end);
	while (g_ascii_isspace(*end))
		++end;

	return errno == 0 && end != text && *end == '\0' && isfinite(*value);
}

static gboolean parse_boolean_value(const gchar *text, gboolean *value)
{
	if (strcasecmp(text, "true") == 0 ||
		strcasecmp(text, "yes") == 0 || strcmp(text, "1") == 0) {
		*value = TRUE;
		return TRUE;
	}
	if (strcasecmp(text, "false") == 0 ||
		strcasecmp(text, "no") == 0 || strcmp(text, "0") == 0) {
		*value = FALSE;
		return TRUE;
	}

	return FALSE;
}

static const GEnumValue *find_enum_value(GEnumClass *enum_class,
	const gchar *text)
{
	const GEnumValue *enum_value;
	guint i;

	enum_value = g_enum_get_value_by_name(enum_class, text);
	if (enum_value == NULL)
		enum_value = g_enum_get_value_by_nick(enum_class, text);
	if (enum_value != NULL)
		return enum_value;

	for (i = 0; i < enum_class->n_values; ++i)
		if (g_ascii_strcasecmp(text,
				enum_class->values[i].value_name) == 0 ||
			g_ascii_strcasecmp(text,
				enum_class->values[i].value_nick) == 0)
			return enum_class->values + i;

	return NULL;
}

static gboolean parse_enum_value(GType type, const gchar *text, gint *value)
{
	GEnumClass *enum_class;
	const GEnumValue *enum_value;
	gchar *copy;
	gint64 signed_value;

	if (parse_signed_value(text, &signed_value)) {
		if (signed_value < G_MININT || signed_value > G_MAXINT)
			return FALSE;
		*value = (gint)signed_value;
		return TRUE;
	}

	copy = g_strdup(text);
	g_strstrip(copy);
	enum_class = G_ENUM_CLASS(g_type_class_ref(type));
	enum_value = find_enum_value(enum_class, copy);
	if (enum_value != NULL)
		*value = enum_value->value;
	g_type_class_unref(enum_class);
	g_free(copy);

	return enum_value != NULL;
}

static const GFlagsValue *find_flags_value(GFlagsClass *flags_class,
	const gchar *text)
{
	const GFlagsValue *flags_value;
	guint i;

	flags_value = g_flags_get_value_by_name(flags_class, text);
	if (flags_value == NULL)
		flags_value = g_flags_get_value_by_nick(flags_class, text);
	if (flags_value != NULL)
		return flags_value;

	for (i = 0; i < flags_class->n_values; ++i)
		if (g_ascii_strcasecmp(text,
				flags_class->values[i].value_name) == 0 ||
			g_ascii_strcasecmp(text,
				flags_class->values[i].value_nick) == 0)
			return flags_class->values + i;

	return NULL;
}

static gboolean parse_flags_value(GType type, const gchar *text, guint *value)
{
	GFlagsClass *flags_class;
	const GFlagsValue *flags_value;
	gchar **tokens;
	guint64 unsigned_value;
	guint result = 0;
	guint i;
	gboolean valid = TRUE;

	if (parse_unsigned_value(text, &unsigned_value)) {
		if (unsigned_value > G_MAXUINT)
			return FALSE;
		*value = (guint)unsigned_value;
		return TRUE;
	}

	tokens = g_strsplit(text, "|", -1);
	flags_class = G_FLAGS_CLASS(g_type_class_ref(type));
	for (i = 0; tokens[i] != NULL; ++i) {
		g_strstrip(tokens[i]);
		if (tokens[i][0] == '\0') {
			valid = FALSE;
			break;
		}
		flags_value = find_flags_value(flags_class, tokens[i]);
		if (flags_value == NULL) {
			valid = FALSE;
			break;
		}
		result |= flags_value->value;
	}
	g_type_class_unref(flags_class);
	g_strfreev(tokens);

	if (valid)
		*value = result;
	return valid;
}

static PropertyValueResult property_value_from_string(GParamSpec *paramspec,
	const gchar *text, GValue *value)
{
	GType value_type = G_TYPE_FUNDAMENTAL(paramspec->value_type);
	GdkColor color_value;
	gboolean boolean_value;
	gdouble double_value;
	gint enum_value;
	gint64 signed_value;
	guint flags_value;
	guint64 unsigned_value;

	g_value_init(value, paramspec->value_type);

	switch (value_type) {
		case G_TYPE_BOOLEAN:
			if (!parse_boolean_value(text, &boolean_value))
				goto invalid;
			g_value_set_boolean(value, boolean_value);
			break;
		case G_TYPE_INT:
			if (!parse_signed_value(text, &signed_value) ||
				signed_value < G_MININT || signed_value > G_MAXINT)
				goto invalid;
			g_value_set_int(value, (gint)signed_value);
			break;
		case G_TYPE_UINT:
			if (!parse_unsigned_value(text, &unsigned_value) ||
				unsigned_value > G_MAXUINT)
				goto invalid;
			g_value_set_uint(value, (guint)unsigned_value);
			break;
		case G_TYPE_FLOAT:
			if (!parse_double_value(text, &double_value) ||
				double_value < -G_MAXFLOAT || double_value > G_MAXFLOAT)
				goto invalid;
			g_value_set_float(value, (gfloat)double_value);
			break;
		case G_TYPE_DOUBLE:
			if (!parse_double_value(text, &double_value))
				goto invalid;
			g_value_set_double(value, double_value);
			break;
		case G_TYPE_CHAR:
			if (!parse_signed_value(text, &signed_value) ||
				signed_value < G_MININT8 || signed_value > G_MAXINT8)
				goto invalid;
			g_value_set_schar(value, (gint8)signed_value);
			break;
		case G_TYPE_UCHAR:
			if (!parse_unsigned_value(text, &unsigned_value) ||
				unsigned_value > G_MAXUINT8)
				goto invalid;
			g_value_set_uchar(value, (guchar)unsigned_value);
			break;
		case G_TYPE_LONG:
			if (!parse_signed_value(text, &signed_value) ||
				signed_value < G_MINLONG || signed_value > G_MAXLONG)
				goto invalid;
			g_value_set_long(value, (glong)signed_value);
			break;
		case G_TYPE_ULONG:
			if (!parse_unsigned_value(text, &unsigned_value) ||
				unsigned_value > G_MAXULONG)
				goto invalid;
			g_value_set_ulong(value, (gulong)unsigned_value);
			break;
		case G_TYPE_INT64:
			if (!parse_signed_value(text, &signed_value))
				goto invalid;
			g_value_set_int64(value, signed_value);
			break;
		case G_TYPE_UINT64:
			if (!parse_unsigned_value(text, &unsigned_value))
				goto invalid;
			g_value_set_uint64(value, unsigned_value);
			break;
		case G_TYPE_ENUM:
			if (!parse_enum_value(paramspec->value_type, text, &enum_value))
				goto invalid;
			g_value_set_enum(value, enum_value);
			break;
		case G_TYPE_FLAGS:
			if (!parse_flags_value(paramspec->value_type, text, &flags_value))
				goto invalid;
			g_value_set_flags(value, flags_value);
			break;
		case G_TYPE_STRING:
			g_value_set_string(value, text);
			break;
		case G_TYPE_BOXED:
			if (paramspec->value_type != GDK_TYPE_COLOR) {
				g_value_unset(value);
				return PROPERTY_VALUE_UNSUPPORTED;
			}
			if (!gdk_color_parse(text, &color_value))
				goto invalid;
			g_value_set_boxed(value, &color_value);
			break;
		default:
			g_value_unset(value);
			return PROPERTY_VALUE_UNSUPPORTED;
	}

	return PROPERTY_VALUE_OK;

invalid:
	g_value_unset(value);
	return PROPERTY_VALUE_INVALID;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

static gboolean try_set_property(GtkWidget *widget, namevalue  *nameval)
{
	GParamSpec       *paramspec;
	GValue            property_value = { 0, };
	PropertyValueResult value_result;

	g_assert(nameval != NULL && GTK_IS_WIDGET(widget));

#ifdef DEBUG
	g_message("%s(): Start on '%s' = '%s'", __func__, 
			nameval->name, nameval->value);
#endif

	/* Thunor: The visible property should not be modified here */
	if (strcmp(nameval->name, "visible") == 0) {
#ifdef DEBUG
		fprintf(stderr, "%s(): not applying visible property\n", __func__);
#endif
		return FALSE;
	}

	/*
	 * If the widget -- or its parents -- has not got this property
	 * we set this as data.
	 */
	paramspec = g_object_class_find_property(G_OBJECT_GET_CLASS(widget), 
			nameval->name);
	if (paramspec == NULL){
		g_object_set_data_full(G_OBJECT(widget), nameval->name,
				g_strdup(nameval->value), g_free);
		return FALSE;
	}
	
	if (!(paramspec->flags & G_PARAM_WRITABLE)) {
		if (!option_no_warning)
			gtkdialog_warning("%s(): Property '%s' is not writable.",
				__func__, nameval->name);
		return FALSE;
	}
	if (paramspec->flags & G_PARAM_CONSTRUCT_ONLY) {
		if (!option_no_warning)
			gtkdialog_warning("%s(): Property '%s' is construct-only.",
				__func__, nameval->name);
		return FALSE;
	}
	
	value_result = property_value_from_string(paramspec, nameval->value,
		&property_value);
	if (value_result == PROPERTY_VALUE_OK) {
		if (!g_param_value_validate(paramspec, &property_value)) {
			g_object_set_property(G_OBJECT(widget), nameval->name,
				&property_value);
			g_value_unset(&property_value);
			return TRUE;
		}
		g_value_unset(&property_value);
		if (!option_no_warning)
			gtkdialog_warning("%s(): Value '%s' is outside the valid range for "
				"property '%s'.", __func__, nameval->value, nameval->name);
		return FALSE;
	}

	if (!option_no_warning && value_result == PROPERTY_VALUE_INVALID)
		gtkdialog_warning("%s(): Invalid value '%s' for property '%s'.", __func__,
			nameval->value, nameval->name);
	else if (!option_no_warning)
		gtkdialog_warning("%s(): Unsupported property type '%s' for '%s'.",
			__func__, g_type_name(paramspec->value_type), nameval->name);
	return FALSE;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

gint widget_set_tag_attributes(GtkWidget *widget, tag_attr *attr)
{
	gboolean	success;
	gint 		q;
	gint 		retval = 0;

	g_assert(GTK_IS_WIDGET(widget));
#ifdef DEBUG
	g_message("%s(): Start on attr = %p.", __func__, attr);
#endif
	if (attr == NULL)
		return -1;
	
	for (q = 0; q < attr->n; ++q) {
		/* Thunor: killed tag attributes have null names */
		if (attr->pairs[q].name[0] != 0) {
			success = try_set_property(widget, attr->pairs + q);
			if (success) ++retval;
		}
	}
	
	return retval;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

char *get_tag_attribute(tag_attr *attr, const char *name)
{
	int q;
	
	g_assert(attr != NULL && name != NULL);
#ifdef DEBUG
	g_message("%s(): searching for name = '%s' in %p", 
			__func__, name, attr);
#endif
	for (q = 0; q < attr->n; ++q) 
		if (strcmp(attr->pairs[q].name, name) == 0)
			return attr->pairs[q].value;

	return NULL;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

tag_attr *add_tag_attribute(tag_attr *attr, char *name, char *value)
{
	g_assert(attr != NULL);
	g_assert(name != NULL && value != NULL);
#ifdef DEBUG	
	g_message("%s(): name = '%s' value = '%s'", __func__, name, value);
#endif 
		
	if (name[0] != '\0' && name[strlen(name) - 1] == '=')
		name[strlen(name) - 1] = '\0';
	//
	// If the store is full, we enlarge its size.
	//
	if (attr->n == attr->nmax){
		attr->nmax += 32;
		attr->pairs = g_realloc(attr->pairs,
				attr->nmax * sizeof(namevalue));
	}
	/*
	 * What if this tagattr is already exists?
	 */
	attr->pairs[attr->n].name = g_strdup(name);
	attr->pairs[attr->n].value = g_strdup(value);
	++attr->n;
	return attr;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/*
 * Simple functions to handle attributesets, name/value pair list.
 */

tag_attr *new_tag_attributeset(char *name, char *value)
{
	tag_attr *New;

	g_assert(name != NULL);
	g_assert(value != NULL);
	
#ifdef DEBUG
	g_message("%s(): Name: '%s' Value: '%s'.", __func__, name, value);
#endif
	if (name[0] != '\0' && name[strlen(name) - 1] == '=')
		name[strlen(name) - 1] = '\0';
	
	New = g_malloc(sizeof(tag_attr));
	
	New->pairs = g_malloc(sizeof(namevalue) * 32);
	New->nmax = 32;
	New->n = 1;
	
	New->pairs[0].name = g_strdup(name);
	New->pairs[0].value = g_strdup(value);
	return New;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

tag_attr *tag_attributeset_copy(const tag_attr *attr)
{
	tag_attr *copy;
	int index;

	if (attr == NULL)
		return NULL;

	copy = g_malloc(sizeof(tag_attr));
	copy->pairs = g_malloc(sizeof(namevalue) * attr->nmax);
	copy->nmax = attr->nmax;
	copy->n = attr->n;

	for (index = 0; index < attr->n; ++index) {
		copy->pairs[index].name = g_strdup(attr->pairs[index].name);
		copy->pairs[index].value = g_strdup(attr->pairs[index].value);
	}

	return copy;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void tag_attributeset_free(tag_attr *attr)
{
	int index;

	if (attr == NULL)
		return;

	for (index = 0; index < attr->n; ++index) {
		g_free(attr->pairs[index].name);
		g_free(attr->pairs[index].value);
	}
	g_free(attr->pairs);
	g_free(attr);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* Thunor: The tag attribute utility functions are quite simple and
 * sparse and I require a way to nullify/erase/kill a tag attribute
 * that I don't want gtk applying on widget realization */

void kill_tag_attribute(tag_attr *attr, const char *name)
{
	int q;
	
	g_assert(attr != NULL && name != NULL);
#ifdef DEBUG
	g_message("%s(): searching for name = '%s' in %p", 
			__func__, name, attr);
#endif
	for (q = 0; q < attr->n; ++q) {
		if (strcmp(attr->pairs[q].name, name) == 0) {
			attr->pairs[q].name[0] = 0;
		}
	}

}
