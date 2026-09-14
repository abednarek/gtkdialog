/*
 * automaton.h: An automaton executing the program, creating widgets. 
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

#ifndef AUTOMATON_H
#define AUTOMATON_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>

#include "gtkdialog.h"
#include "stack.h"
#include "attributes.h"
#include "stringman.h"


typedef int token;

typedef struct _instruction {
	token      command;
	char      *argument;
	gint       ival;
	tag_attr  *tag_attributes;
} instruction;

typedef struct actioncommand {
  GtkWidget   *source_widget;
  char        *destination_name;
} actioncommand;

/*                                   -- Type of widget      */
/*                                 xxxxxxxx                 */
#define WIDGET_TYPE              0x00FF0000
#define WIDGET_TEXT              0x00010000
#define WIDGET_ENTRY             0x00020000
#define WIDGET_EDIT              0x00030000
#define WIDGET_BUTTON            0x00040000
#define WIDGET_CHECKBOX          0x00050000
#define WIDGET_RADIOBUTTON       0x00060000
#define WIDGET_LIST              0x00070000
#define WIDGET_TABLE             0x00080000
#define WIDGET_COMBOBOX          0x00090000
#define WIDGET_OKBUTTON          0x000A0000
#define WIDGET_CANCELBUTTON      0x000B0000
#define WIDGET_HELPBUTTON        0x000C0000
#define WIDGET_NOBUTTON          0x000D0000
#define WIDGET_YESBUTTON         0x000E0000
#define WIDGET_SCROLLEDW         0x000F0000
#define WIDGET_VBOX              0x00100000
#define WIDGET_HBOX              0x00200000
#define WIDGET_FRAME             0x00300000
#define WIDGET_NOTEBOOK          0x00310000
#define WIDGET_WINDOW            0x00400000
#define WIDGET_PIXMAP            0x00500000
#define WIDGET_MENUBAR           0x00600000
#define WIDGET_MENU              0x00700000
#define WIDGET_MENUITEM          0x00800000
#define WIDGET_MENUITEMSEPARATOR 0x00900000
#define WIDGET_GVIM              0x00A00000
#define WIDGET_TREE              0x00A10000
#define WIDGET_CHOOSER           0x00A20000
#define WIDGET_PROGRESSBAR       0x00A30000
#define WIDGET_HSEPARATOR        0x00A40000
#define WIDGET_VSEPARATOR        0x00A50000
#define WIDGET_COMBOBOXTEXT      0x00A60000
#define WIDGET_COMBOBOXENTRY     0x00A70000
#define WIDGET_HSCALE            0x00A80000
#define WIDGET_VSCALE            0x00A90000
#define WIDGET_SPINBUTTON        0x00AA0000
#define WIDGET_TIMER             0x00AB0000
#define WIDGET_TOGGLEBUTTON      0x00AC0000
#define WIDGET_STATUSBAR         0x00AD0000
#define WIDGET_COLORBUTTON       0x00AE0000
#define WIDGET_FONTBUTTON        0x00AF0000
#define WIDGET_TERMINAL          0x00B00000
#define WIDGET_EVENTBOX          0x00B10000
#define WIDGET_EXPANDER          0x00B20000
#define WIDGET_HPANED            0x00B30000
#define WIDGET_VPANED            0x00B40000
#define WIDGET_ALIGNMENT         0x00B50000
#define WIDGET_HBUTTONBOX        0x00B60000
#define WIDGET_VBUTTONBOX        0x00B70000
#define WIDGET_TOOLBAR           0x00B80000
#define WIDGET_TOOLBUTTON        0x00B90000
#define WIDGET_SEPARATORTOOLITEM 0x00BA0000
#define WIDGET_TOGGLETOOLBUTTON  0x00BB0000
#define WIDGET_RADIOTOOLBUTTON   0x00BC0000
#define WIDGET_MENUTOOLBUTTON    0x00BD0000
#define WIDGET_CALENDAR          0x00BE0000
#define WIDGET_LINKBUTTON        0x00BF0000
#define WIDGET_SPINNER           0x00C00000
#define WIDGET_GRID              0x00C10000
#define WIDGET_FILECHOOSERBUTTON 0x00C20000
#define WIDGET_INFOBAR           0x00C30000
#define WIDGET_ASSISTANT         0x00C40000
#define WIDGET_ICONVIEW          0x00C50000
#define WIDGET_SCALEBUTTON       0x00C60000
#define WIDGET_VOLUMEBUTTON      0x00C70000
#define WIDGET_ASPECTFRAME       0x00C80000
#define WIDGET_MESSAGEDIALOG     0x00C90000
#define WIDGET_DIALOG            0x00CA0000
#define WIDGET_ABOUTDIALOG       0x00CB0000
#define WIDGET_RECENTCHOOSER     0x00CC0000
#define WIDGET_TOOLITEM          0x00CD0000
#define WIDGET_FIXED             0x00CE0000
#define WIDGET_LAYOUT            0x00CF0000
#define WIDGET_SCROLLEDWINDOW    0x00D00000
#define WIDGET_HANDLEBOX         0x00D10000
#define WIDGET_TOOLPALETTE       0x00D20000
#define WIDGET_TOOLITEMGROUP     0x00D30000
#define WIDGET_CELLVIEW          0x00D40000
#define WIDGET_DRAWINGAREA       0x00D50000
#define WIDGET_POPUPMENU         0x00D60000
#define WIDGET_SOCKET            0x00D70000
#define WIDGET_HSCROLLBAR        0x00D80000
#define WIDGET_VSCROLLBAR        0x00D90000
#define WIDGET_VIEWPORT          0x00DA0000
#define WIDGET_COLORSELECTION    0x00DB0000
#define WIDGET_FONTSELECTION     0x00DC0000
#define WIDGET_RECENTCHOOSERMENU 0x00DD0000
#define WIDGET_STATUSICON        0x00DE0000
#define WIDGET_ACCELLABEL        0x00DF0000
#define WIDGET_ARROW             0x00E00000
#define WIDGET_HSV               0x00E10000
#define WIDGET_HRULER            0x00E20000
#define WIDGET_VRULER            0x00E30000
#define WIDGET_PLUG              0x00E40000

/*
 * Imperative stuff.
 */
#define VARIABLE_NAME         0x00010000
#define CONST_NUMBER          0x00020000
#define OP_ADD                0x000a0000
#define OP_SUBST              0x000b0000
#define OP_MULT               0x000c0000
#define OP_DIV                0x000d0000
#define REL_EQ                0x00100000
#define REL_NE                0x00200000



/*                                   - Widget subtype    */
/*                              xxxxxxxx                 */
#define WIDGET_SUBTYPE        0x00000F00
/*                                  - Command            */
/*                              xxxxxxxx                 */
#define COMMAND               0x0000F000
#define PUSH                  0x00001000
#define SUM                   0x00002000
#define SET                   0x00003000
#define SHOW		      0x00004000
#define IFNGOTO               0x00005000
#define IMPUSH                0x00006000
#define IMASSG                0x00007000
#define GOTO                  0x00008000
#define IMMERGE               0x00009000
#define RGROUP_PUSH           0x0000A000
#define RGROUP_POP            0x0000B000

/*                                    -- Attribute           */
/*                              xxxxxxxx                 */
#define ATTRIBUTE             0x000000ff
#define ATTR_LABEL            0x00000001
#define ATTR_DEFAULT          0x00000002
#define ATTR_VARIABLE         0x00000003
#define ATTR_HEIGHT           0x00000004
#define ATTR_WIDTH            0x00000005
#define ATTR_INPUT            0x00000006
#define ATTR_SENSITIVE        0x00000007
#define ATTR_ACTION           0x00000008
#define ATTR_ITEM             0x00000009
#define ATTR_OUTPUT           0x0000000A
#define ATTR_SOCKET           0x0000000B


/*                               - Sub attribute         */
/*                              xxxxxxxx                 */
#define SUB_ATTRIBUTE         0x0F000000
#define SUB_ATTR_SHELL        0x01000000
#define SUB_ATTR_FILE         0x02000000


/*************************************************************************
 * Public functions:                                                     *
 *                                                                       *
 *                                                                       *
 *************************************************************************/
int instruction_execute(instruction command);
void print_program(void);
void run_program(void);

void fileselection_made( GtkWidget *w, actioncommand *action );

int token_store(token command);
int token_store_attr(token command, tag_attr *attributes);
int token_store_with_argument_attr(token command, const char *argument, tag_attr *attributes);
int token_store_with_argument( token command, const char *argument);

gint instruction_get_pc(void);
void instruction_set_jump(gint from, gint where);

#endif
