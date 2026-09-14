/*
 * stack.c:
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

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "gtkdialog.h"
#include "stack.h"

#define INITIAL_STACK_CAPACITY 128

static stackelement *stack = NULL;
static size_t stack_capacity = 0;
static size_t sp = 0;

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void push_widget(GtkWidget * widget, int widgettype)
{
#ifdef DEBUG
	g_message("%s(): Start.", __func__);
#endif
	stackelement s = { 0 };

	s.nwidgets = 1;
	s.widget_capacity = 1;
	s.widgets = g_new(GtkWidget *, 1);
	s.widgettypes = g_new(int, 1);
	s.widgets[0] = widget;
	s.widgettypes[0] = widgettype;
	push(s);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void push(stackelement new)
{
#ifdef DEBUG
	static size_t spgreatest = 0;

	if (sp > spgreatest) {
		spgreatest = sp;
		g_message("%s: spgreatest=%zu", __func__, spgreatest);
	}

	g_message("%s: Start sp = %zu", __func__, sp);
#endif

	if (sp == stack_capacity) {
		stack_capacity = stack_capacity == 0
			? INITIAL_STACK_CAPACITY : stack_capacity * 2;
		stack = g_renew(stackelement, stack, stack_capacity);
	}
	stack[sp++] = new;

#ifdef DEBUG
	g_message("%s()): end sp = %zu", __func__, sp);
#endif
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

stackelement pop(void)
{
	stackelement result;

	if (sp == 0) 
		g_error("Stack underflow: Empty program or internal error.");
	
	sp--;
	result = stack[sp];
	memset(&stack[sp], 0, sizeof(stack[sp]));

#ifdef DEBUG
	fprintf(stderr, "%s: stack pointer: %zu\n", __func__, sp);
	fflush(stderr);
#endif

	return result;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void stackelement_append(stackelement *destination, stackelement *source)
{
	int required;
	int capacity;

	g_return_if_fail(destination != NULL);
	g_return_if_fail(source != NULL);
	if (source->nwidgets > G_MAXINT - destination->nwidgets)
		g_error("Too many widgets to represent in one container.");

	required = destination->nwidgets + source->nwidgets;
	if (required > destination->widget_capacity) {
		capacity = destination->widget_capacity > 0
			? destination->widget_capacity : 1;
		while (capacity < required) {
			if (capacity > G_MAXINT / 2) {
				capacity = required;
				break;
			}
			capacity *= 2;
		}
		destination->widgets = g_renew(
			GtkWidget *, destination->widgets, capacity);
		destination->widgettypes = g_renew(
			int, destination->widgettypes, capacity);
		destination->widget_capacity = capacity;
	}

	if (source->nwidgets > 0) {
		memcpy(destination->widgets + destination->nwidgets,
			source->widgets, source->nwidgets * sizeof(*source->widgets));
		memcpy(destination->widgettypes + destination->nwidgets,
			source->widgettypes,
			source->nwidgets * sizeof(*source->widgettypes));
		destination->nwidgets = required;
	}

	stackelement_clear(source);
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void stackelement_clear(stackelement *element)
{
	if (element == NULL)
		return;

	g_free(element->cval);
	g_free(element->widgets);
	g_free(element->widgettypes);
	element->cval = NULL;
	element->widgets = NULL;
	element->widgettypes = NULL;
	element->nwidgets = 0;
	element->widget_capacity = 0;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

size_t stack_depth(void)
{
	return sp;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/

void stack_merge_since(size_t depth)
{
	size_t destination;
	size_t source;

	if (depth > sp)
		g_error("Invalid imperative block stack depth.");
	if (depth == sp) {
		if (depth == 0) {
			stackelement empty = { 0 };

			push(empty);
		}
		return;
	}

	/* A block at the beginning of a widget list creates its first stack
	 * element. Otherwise append its result to the widget list that was
	 * already at the top when the block began. */
	destination = depth == 0 ? 0 : depth - 1;
	source = depth == 0 ? 1 : depth;
	for (; source < sp; ++source)
		stackelement_append(&stack[destination], &stack[source]);

	sp = destination + 1;
}

/***********************************************************************
 *                                                                     *
 ***********************************************************************/
/* This show command is much more complex than I thought */

void show_without_pop(GtkWidget *window)
{
	stackelement *current;
	g_assert(window != NULL);
	g_assert(sp != 0);

	current = stack + (sp - 1);
	g_message("sp = %zu nwidgets = %d", sp, current->nwidgets);
	gtk_container_add(GTK_CONTAINER(window), current->widgets[0]);
	gtk_widget_show_all(window);
}
