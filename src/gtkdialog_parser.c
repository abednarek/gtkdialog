/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         gtkdialog_parse
#define yylex           gtkdialog_lex
#define yyerror         gtkdialog_error
#define yydebug         gtkdialog_debug
#define yynerrs         gtkdialog_nerrs
#define yylval          gtkdialog_lval
#define yychar          gtkdialog_char

/* First part of user prologue.  */
#line 1 "gtkdialog_parser.y"

/*
 * gtkdialog_parser.y: A simple grammar for the XML-like language we use.
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
** $Id: parser.y,v 1.5 2004/11/25 21:16:57 root Exp root $
** $Log: parser.y,v $
** Revision 1.5  2004/11/25 21:16:57  root
** *** empty log message ***
**
** Revision 1.4  2004/11/25 21:15:21  root
**   o No, the grammar still has problems.
**
** Revision 1.2  2004/11/25 19:53:03  pipas
**   o New object: tag attributes.
**
** Revision 1.1  2004/11/19 22:10:08  pipas
** Initial revision
**
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "gtkdialog.h"
#include "config.h"
#include "automaton.h"
#include "attributes.h"
#include "gtkdialog_parser.h"
#include "tag_attributes.h"

int linenumber = 1;
extern gchar *Token;
extern gboolean option_no_warning;
extern gboolean option_print_ir;

//
// Function declarations
//
int yywarning(char *c);
void yyerror_simple(char *c);

static inline void
start_up(void)
{
	if (!option_print_ir) {
		run_program();
		return;
	} else {
		print_program();
		exit(EXIT_SUCCESS);
	}
}

static void
token_store_owned_argument(token command, gchar *argument)
{
	token_store_with_argument(command, argument);
	g_free(argument);
}

static void
token_store_owned_argument_attr(token command, gchar *argument,
	tag_attr *attributes)
{
	token_store_with_argument_attr(command, argument, attributes);
	g_free(argument);
}

static tag_attr *
new_tag_attributeset_owned(gchar *name, gchar *value)
{
	tag_attr *attributes = new_tag_attributeset(name, value);

	g_free(name);
	g_free(value);
	return attributes;
}

static tag_attr *
add_tag_attribute_owned(tag_attr *attributes, gchar *name, gchar *value)
{
	add_tag_attribute(attributes, name, value);
	g_free(name);
	g_free(value);
	return attributes;
}


#line 190 "gtkdialog_parser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "gtkdialog_parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_WINDOW = 3,                     /* WINDOW  */
  YYSYMBOL_PART_WINDOW = 4,                /* PART_WINDOW  */
  YYSYMBOL_EWINDOW = 5,                    /* EWINDOW  */
  YYSYMBOL_VBOX = 6,                       /* VBOX  */
  YYSYMBOL_PART_VBOX = 7,                  /* PART_VBOX  */
  YYSYMBOL_EVBOX = 8,                      /* EVBOX  */
  YYSYMBOL_HBOX = 9,                       /* HBOX  */
  YYSYMBOL_PART_HBOX = 10,                 /* PART_HBOX  */
  YYSYMBOL_EHBOX = 11,                     /* EHBOX  */
  YYSYMBOL_NOTEBOOK = 12,                  /* NOTEBOOK  */
  YYSYMBOL_ENOTEBOOK = 13,                 /* ENOTEBOOK  */
  YYSYMBOL_PART_NOTEBOOK = 14,             /* PART_NOTEBOOK  */
  YYSYMBOL_FRAME = 15,                     /* FRAME  */
  YYSYMBOL_TAG_ATTR_NAME = 16,             /* TAG_ATTR_NAME  */
  YYSYMBOL_EFRAME = 17,                    /* EFRAME  */
  YYSYMBOL_ENTRY = 18,                     /* ENTRY  */
  YYSYMBOL_EENTRY = 19,                    /* EENTRY  */
  YYSYMBOL_PART_ENTRY = 20,                /* PART_ENTRY  */
  YYSYMBOL_MENUBAR = 21,                   /* MENUBAR  */
  YYSYMBOL_PART_MENUBAR = 22,              /* PART_MENUBAR  */
  YYSYMBOL_EMENUBAR = 23,                  /* EMENUBAR  */
  YYSYMBOL_MENU = 24,                      /* MENU  */
  YYSYMBOL_PART_MENU = 25,                 /* PART_MENU  */
  YYSYMBOL_EMENU = 26,                     /* EMENU  */
  YYSYMBOL_POPUPMENU = 27,                 /* POPUPMENU  */
  YYSYMBOL_PART_POPUPMENU = 28,            /* PART_POPUPMENU  */
  YYSYMBOL_EPOPUPMENU = 29,                /* EPOPUPMENU  */
  YYSYMBOL_MENUITEM = 30,                  /* MENUITEM  */
  YYSYMBOL_PART_MENUITEM = 31,             /* PART_MENUITEM  */
  YYSYMBOL_EMENUITEM = 32,                 /* EMENUITEM  */
  YYSYMBOL_MENUITEMSEPARATOR = 33,         /* MENUITEMSEPARATOR  */
  YYSYMBOL_EMENUITEMSEPARATOR = 34,        /* EMENUITEMSEPARATOR  */
  YYSYMBOL_EDIT = 35,                      /* EDIT  */
  YYSYMBOL_PART_EDIT = 36,                 /* PART_EDIT  */
  YYSYMBOL_EEDIT = 37,                     /* EEDIT  */
  YYSYMBOL_TREE = 38,                      /* TREE  */
  YYSYMBOL_PART_TREE = 39,                 /* PART_TREE  */
  YYSYMBOL_ETREE = 40,                     /* ETREE  */
  YYSYMBOL_CHOOSER = 41,                   /* CHOOSER  */
  YYSYMBOL_PART_CHOOSER = 42,              /* PART_CHOOSER  */
  YYSYMBOL_ECHOOSER = 43,                  /* ECHOOSER  */
  YYSYMBOL_LABEL = 44,                     /* LABEL  */
  YYSYMBOL_ELABEL = 45,                    /* ELABEL  */
  YYSYMBOL_ITEM = 46,                      /* ITEM  */
  YYSYMBOL_EITEM = 47,                     /* EITEM  */
  YYSYMBOL_PART_ITEM = 48,                 /* PART_ITEM  */
  YYSYMBOL_BUTTON = 49,                    /* BUTTON  */
  YYSYMBOL_PART_BUTTON = 50,               /* PART_BUTTON  */
  YYSYMBOL_EBUTTON = 51,                   /* EBUTTON  */
  YYSYMBOL_BUTTONOK = 52,                  /* BUTTONOK  */
  YYSYMBOL_BUTTONCANCEL = 53,              /* BUTTONCANCEL  */
  YYSYMBOL_BUTTONHELP = 54,                /* BUTTONHELP  */
  YYSYMBOL_BUTTONYES = 55,                 /* BUTTONYES  */
  YYSYMBOL_BUTTONNO = 56,                  /* BUTTONNO  */
  YYSYMBOL_CHECKBOX = 57,                  /* CHECKBOX  */
  YYSYMBOL_ECHECKBOX = 58,                 /* ECHECKBOX  */
  YYSYMBOL_PART_CHECKBOX = 59,             /* PART_CHECKBOX  */
  YYSYMBOL_RADIO = 60,                     /* RADIO  */
  YYSYMBOL_ERADIO = 61,                    /* ERADIO  */
  YYSYMBOL_PART_RADIO = 62,                /* PART_RADIO  */
  YYSYMBOL_PROGRESSBAR = 63,               /* PROGRESSBAR  */
  YYSYMBOL_EPROGRESSBAR = 64,              /* EPROGRESSBAR  */
  YYSYMBOL_PART_PROGRESSBAR = 65,          /* PART_PROGRESSBAR  */
  YYSYMBOL_LIST = 66,                      /* LIST  */
  YYSYMBOL_PART_LIST = 67,                 /* PART_LIST  */
  YYSYMBOL_ELIST = 68,                     /* ELIST  */
  YYSYMBOL_TABLE = 69,                     /* TABLE  */
  YYSYMBOL_ETABLE = 70,                    /* ETABLE  */
  YYSYMBOL_PART_TABLE = 71,                /* PART_TABLE  */
  YYSYMBOL_COMBOBOX = 72,                  /* COMBOBOX  */
  YYSYMBOL_PART_COMBOBOX = 73,             /* PART_COMBOBOX  */
  YYSYMBOL_ECOMBOBOX = 74,                 /* ECOMBOBOX  */
  YYSYMBOL_GVIM = 75,                      /* GVIM  */
  YYSYMBOL_EGVIM = 76,                     /* EGVIM  */
  YYSYMBOL_TEXT = 77,                      /* TEXT  */
  YYSYMBOL_PART_TEXT = 78,                 /* PART_TEXT  */
  YYSYMBOL_ETEXT = 79,                     /* ETEXT  */
  YYSYMBOL_PIXMAP = 80,                    /* PIXMAP  */
  YYSYMBOL_PART_PIXMAP = 81,               /* PART_PIXMAP  */
  YYSYMBOL_EPIXMAP = 82,                   /* EPIXMAP  */
  YYSYMBOL_DEFAULT = 83,                   /* DEFAULT  */
  YYSYMBOL_EDEFAULT = 84,                  /* EDEFAULT  */
  YYSYMBOL_SENSITIVE = 85,                 /* SENSITIVE  */
  YYSYMBOL_ESENSITIVE = 86,                /* ESENSITIVE  */
  YYSYMBOL_VARIABLE = 87,                  /* VARIABLE  */
  YYSYMBOL_PART_VARIABLE = 88,             /* PART_VARIABLE  */
  YYSYMBOL_EVARIABLE = 89,                 /* EVARIABLE  */
  YYSYMBOL_WIDTH = 90,                     /* WIDTH  */
  YYSYMBOL_EWIDTH = 91,                    /* EWIDTH  */
  YYSYMBOL_HEIGHT = 92,                    /* HEIGHT  */
  YYSYMBOL_EHEIGHT = 93,                   /* EHEIGHT  */
  YYSYMBOL_INPUT = 94,                     /* INPUT  */
  YYSYMBOL_INPUTFILE = 95,                 /* INPUTFILE  */
  YYSYMBOL_EINPUT = 96,                    /* EINPUT  */
  YYSYMBOL_PART_INPUT = 97,                /* PART_INPUT  */
  YYSYMBOL_PART_INPUTFILE = 98,            /* PART_INPUTFILE  */
  YYSYMBOL_OUTPUT = 99,                    /* OUTPUT  */
  YYSYMBOL_OUTPUTFILE = 100,               /* OUTPUTFILE  */
  YYSYMBOL_EOUTPUT = 101,                  /* EOUTPUT  */
  YYSYMBOL_ACTION = 102,                   /* ACTION  */
  YYSYMBOL_EACTION = 103,                  /* EACTION  */
  YYSYMBOL_PART_ACTION = 104,              /* PART_ACTION  */
  YYSYMBOL_COMM = 105,                     /* COMM  */
  YYSYMBOL_ENDCOMM = 106,                  /* ENDCOMM  */
  YYSYMBOL_IF = 107,                       /* IF  */
  YYSYMBOL_ENDIF = 108,                    /* ENDIF  */
  YYSYMBOL_WHILE = 109,                    /* WHILE  */
  YYSYMBOL_EWHILE = 110,                   /* EWHILE  */
  YYSYMBOL_SHOW_WIDGETS = 111,             /* SHOW_WIDGETS  */
  YYSYMBOL_EMB_VARIABLE = 112,             /* EMB_VARIABLE  */
  YYSYMBOL_EMB_NUMBER = 113,               /* EMB_NUMBER  */
  YYSYMBOL_END_OF_FILE = 114,              /* END_OF_FILE  */
  YYSYMBOL_NUMBER = 115,                   /* NUMBER  */
  YYSYMBOL_STRING = 116,                   /* STRING  */
  YYSYMBOL_117_ = 117,                     /* '='  */
  YYSYMBOL_118_ = 118,                     /* '-'  */
  YYSYMBOL_119_ = 119,                     /* '+'  */
  YYSYMBOL_120_ = 120,                     /* '*'  */
  YYSYMBOL_121_ = 121,                     /* '/'  */
  YYSYMBOL_UMINUS = 122,                   /* UMINUS  */
  YYSYMBOL_HSEPARATOR = 123,               /* HSEPARATOR  */
  YYSYMBOL_PART_HSEPARATOR = 124,          /* PART_HSEPARATOR  */
  YYSYMBOL_EHSEPARATOR = 125,              /* EHSEPARATOR  */
  YYSYMBOL_VSEPARATOR = 126,               /* VSEPARATOR  */
  YYSYMBOL_PART_VSEPARATOR = 127,          /* PART_VSEPARATOR  */
  YYSYMBOL_EVSEPARATOR = 128,              /* EVSEPARATOR  */
  YYSYMBOL_COMBOBOXTEXT = 129,             /* COMBOBOXTEXT  */
  YYSYMBOL_PART_COMBOBOXTEXT = 130,        /* PART_COMBOBOXTEXT  */
  YYSYMBOL_ECOMBOBOXTEXT = 131,            /* ECOMBOBOXTEXT  */
  YYSYMBOL_COMBOBOXENTRY = 132,            /* COMBOBOXENTRY  */
  YYSYMBOL_PART_COMBOBOXENTRY = 133,       /* PART_COMBOBOXENTRY  */
  YYSYMBOL_ECOMBOBOXENTRY = 134,           /* ECOMBOBOXENTRY  */
  YYSYMBOL_HSCALE = 135,                   /* HSCALE  */
  YYSYMBOL_PART_HSCALE = 136,              /* PART_HSCALE  */
  YYSYMBOL_EHSCALE = 137,                  /* EHSCALE  */
  YYSYMBOL_VSCALE = 138,                   /* VSCALE  */
  YYSYMBOL_PART_VSCALE = 139,              /* PART_VSCALE  */
  YYSYMBOL_EVSCALE = 140,                  /* EVSCALE  */
  YYSYMBOL_SPINBUTTON = 141,               /* SPINBUTTON  */
  YYSYMBOL_PART_SPINBUTTON = 142,          /* PART_SPINBUTTON  */
  YYSYMBOL_ESPINBUTTON = 143,              /* ESPINBUTTON  */
  YYSYMBOL_TIMER = 144,                    /* TIMER  */
  YYSYMBOL_PART_TIMER = 145,               /* PART_TIMER  */
  YYSYMBOL_ETIMER = 146,                   /* ETIMER  */
  YYSYMBOL_TOGGLEBUTTON = 147,             /* TOGGLEBUTTON  */
  YYSYMBOL_PART_TOGGLEBUTTON = 148,        /* PART_TOGGLEBUTTON  */
  YYSYMBOL_ETOGGLEBUTTON = 149,            /* ETOGGLEBUTTON  */
  YYSYMBOL_STATUSBAR = 150,                /* STATUSBAR  */
  YYSYMBOL_PART_STATUSBAR = 151,           /* PART_STATUSBAR  */
  YYSYMBOL_ESTATUSBAR = 152,               /* ESTATUSBAR  */
  YYSYMBOL_COLORBUTTON = 153,              /* COLORBUTTON  */
  YYSYMBOL_PART_COLORBUTTON = 154,         /* PART_COLORBUTTON  */
  YYSYMBOL_ECOLORBUTTON = 155,             /* ECOLORBUTTON  */
  YYSYMBOL_FONTBUTTON = 156,               /* FONTBUTTON  */
  YYSYMBOL_PART_FONTBUTTON = 157,          /* PART_FONTBUTTON  */
  YYSYMBOL_EFONTBUTTON = 158,              /* EFONTBUTTON  */
  YYSYMBOL_TERMINAL = 159,                 /* TERMINAL  */
  YYSYMBOL_PART_TERMINAL = 160,            /* PART_TERMINAL  */
  YYSYMBOL_ETERMINAL = 161,                /* ETERMINAL  */
  YYSYMBOL_EVENTBOX = 162,                 /* EVENTBOX  */
  YYSYMBOL_PART_EVENTBOX = 163,            /* PART_EVENTBOX  */
  YYSYMBOL_EEVENTBOX = 164,                /* EEVENTBOX  */
  YYSYMBOL_EXPANDER = 165,                 /* EXPANDER  */
  YYSYMBOL_PART_EXPANDER = 166,            /* PART_EXPANDER  */
  YYSYMBOL_EEXPANDER = 167,                /* EEXPANDER  */
  YYSYMBOL_HPANED = 168,                   /* HPANED  */
  YYSYMBOL_PART_HPANED = 169,              /* PART_HPANED  */
  YYSYMBOL_EHPANED = 170,                  /* EHPANED  */
  YYSYMBOL_VPANED = 171,                   /* VPANED  */
  YYSYMBOL_PART_VPANED = 172,              /* PART_VPANED  */
  YYSYMBOL_EVPANED = 173,                  /* EVPANED  */
  YYSYMBOL_ALIGNMENT = 174,                /* ALIGNMENT  */
  YYSYMBOL_PART_ALIGNMENT = 175,           /* PART_ALIGNMENT  */
  YYSYMBOL_EALIGNMENT = 176,               /* EALIGNMENT  */
  YYSYMBOL_HBUTTONBOX = 177,               /* HBUTTONBOX  */
  YYSYMBOL_PART_HBUTTONBOX = 178,          /* PART_HBUTTONBOX  */
  YYSYMBOL_EHBUTTONBOX = 179,              /* EHBUTTONBOX  */
  YYSYMBOL_VBUTTONBOX = 180,               /* VBUTTONBOX  */
  YYSYMBOL_PART_VBUTTONBOX = 181,          /* PART_VBUTTONBOX  */
  YYSYMBOL_EVBUTTONBOX = 182,              /* EVBUTTONBOX  */
  YYSYMBOL_TOOLBAR = 183,                  /* TOOLBAR  */
  YYSYMBOL_PART_TOOLBAR = 184,             /* PART_TOOLBAR  */
  YYSYMBOL_ETOOLBAR = 185,                 /* ETOOLBAR  */
  YYSYMBOL_TOOLBUTTON = 186,               /* TOOLBUTTON  */
  YYSYMBOL_PART_TOOLBUTTON = 187,          /* PART_TOOLBUTTON  */
  YYSYMBOL_ETOOLBUTTON = 188,              /* ETOOLBUTTON  */
  YYSYMBOL_TOGGLETOOLBUTTON = 189,         /* TOGGLETOOLBUTTON  */
  YYSYMBOL_PART_TOGGLETOOLBUTTON = 190,    /* PART_TOGGLETOOLBUTTON  */
  YYSYMBOL_ETOGGLETOOLBUTTON = 191,        /* ETOGGLETOOLBUTTON  */
  YYSYMBOL_RADIOTOOLBUTTON = 192,          /* RADIOTOOLBUTTON  */
  YYSYMBOL_PART_RADIOTOOLBUTTON = 193,     /* PART_RADIOTOOLBUTTON  */
  YYSYMBOL_ERADIOTOOLBUTTON = 194,         /* ERADIOTOOLBUTTON  */
  YYSYMBOL_MENUTOOLBUTTON = 195,           /* MENUTOOLBUTTON  */
  YYSYMBOL_PART_MENUTOOLBUTTON = 196,      /* PART_MENUTOOLBUTTON  */
  YYSYMBOL_EMENUTOOLBUTTON = 197,          /* EMENUTOOLBUTTON  */
  YYSYMBOL_SEPARATORTOOLITEM = 198,        /* SEPARATORTOOLITEM  */
  YYSYMBOL_PART_SEPARATORTOOLITEM = 199,   /* PART_SEPARATORTOOLITEM  */
  YYSYMBOL_ESEPARATORTOOLITEM = 200,       /* ESEPARATORTOOLITEM  */
  YYSYMBOL_CALENDAR = 201,                 /* CALENDAR  */
  YYSYMBOL_PART_CALENDAR = 202,            /* PART_CALENDAR  */
  YYSYMBOL_ECALENDAR = 203,                /* ECALENDAR  */
  YYSYMBOL_LINKBUTTON = 204,               /* LINKBUTTON  */
  YYSYMBOL_PART_LINKBUTTON = 205,          /* PART_LINKBUTTON  */
  YYSYMBOL_ELINKBUTTON = 206,              /* ELINKBUTTON  */
  YYSYMBOL_SPINNER = 207,                  /* SPINNER  */
  YYSYMBOL_PART_SPINNER = 208,             /* PART_SPINNER  */
  YYSYMBOL_ESPINNER = 209,                 /* ESPINNER  */
  YYSYMBOL_GRID = 210,                     /* GRID  */
  YYSYMBOL_PART_GRID = 211,                /* PART_GRID  */
  YYSYMBOL_EGRID = 212,                    /* EGRID  */
  YYSYMBOL_FILECHOOSERBUTTON = 213,        /* FILECHOOSERBUTTON  */
  YYSYMBOL_PART_FILECHOOSERBUTTON = 214,   /* PART_FILECHOOSERBUTTON  */
  YYSYMBOL_EFILECHOOSERBUTTON = 215,       /* EFILECHOOSERBUTTON  */
  YYSYMBOL_INFOBAR = 216,                  /* INFOBAR  */
  YYSYMBOL_PART_INFOBAR = 217,             /* PART_INFOBAR  */
  YYSYMBOL_EINFOBAR = 218,                 /* EINFOBAR  */
  YYSYMBOL_ASSISTANT = 219,                /* ASSISTANT  */
  YYSYMBOL_PART_ASSISTANT = 220,           /* PART_ASSISTANT  */
  YYSYMBOL_EASSISTANT = 221,               /* EASSISTANT  */
  YYSYMBOL_ICONVIEW = 222,                 /* ICONVIEW  */
  YYSYMBOL_PART_ICONVIEW = 223,            /* PART_ICONVIEW  */
  YYSYMBOL_EICONVIEW = 224,                /* EICONVIEW  */
  YYSYMBOL_SCALEBUTTON = 225,              /* SCALEBUTTON  */
  YYSYMBOL_PART_SCALEBUTTON = 226,         /* PART_SCALEBUTTON  */
  YYSYMBOL_ESCALEBUTTON = 227,             /* ESCALEBUTTON  */
  YYSYMBOL_VOLUMEBUTTON = 228,             /* VOLUMEBUTTON  */
  YYSYMBOL_PART_VOLUMEBUTTON = 229,        /* PART_VOLUMEBUTTON  */
  YYSYMBOL_EVOLUMEBUTTON = 230,            /* EVOLUMEBUTTON  */
  YYSYMBOL_ASPECTFRAME = 231,              /* ASPECTFRAME  */
  YYSYMBOL_PART_ASPECTFRAME = 232,         /* PART_ASPECTFRAME  */
  YYSYMBOL_EASPECTFRAME = 233,             /* EASPECTFRAME  */
  YYSYMBOL_MESSAGEDIALOG = 234,            /* MESSAGEDIALOG  */
  YYSYMBOL_PART_MESSAGEDIALOG = 235,       /* PART_MESSAGEDIALOG  */
  YYSYMBOL_EMESSAGEDIALOG = 236,           /* EMESSAGEDIALOG  */
  YYSYMBOL_DIALOG = 237,                   /* DIALOG  */
  YYSYMBOL_PART_DIALOG = 238,              /* PART_DIALOG  */
  YYSYMBOL_EDIALOG = 239,                  /* EDIALOG  */
  YYSYMBOL_ABOUTDIALOG = 240,              /* ABOUTDIALOG  */
  YYSYMBOL_PART_ABOUTDIALOG = 241,         /* PART_ABOUTDIALOG  */
  YYSYMBOL_EABOUTDIALOG = 242,             /* EABOUTDIALOG  */
  YYSYMBOL_RECENTCHOOSER = 243,            /* RECENTCHOOSER  */
  YYSYMBOL_PART_RECENTCHOOSER = 244,       /* PART_RECENTCHOOSER  */
  YYSYMBOL_ERECENTCHOOSER = 245,           /* ERECENTCHOOSER  */
  YYSYMBOL_RECENTCHOOSERMENU = 246,        /* RECENTCHOOSERMENU  */
  YYSYMBOL_PART_RECENTCHOOSERMENU = 247,   /* PART_RECENTCHOOSERMENU  */
  YYSYMBOL_ERECENTCHOOSERMENU = 248,       /* ERECENTCHOOSERMENU  */
  YYSYMBOL_STATUSICON = 249,               /* STATUSICON  */
  YYSYMBOL_PART_STATUSICON = 250,          /* PART_STATUSICON  */
  YYSYMBOL_ESTATUSICON = 251,              /* ESTATUSICON  */
  YYSYMBOL_ACCELLABEL = 252,               /* ACCELLABEL  */
  YYSYMBOL_PART_ACCELLABEL = 253,          /* PART_ACCELLABEL  */
  YYSYMBOL_EACCELLABEL = 254,              /* EACCELLABEL  */
  YYSYMBOL_ARROW = 255,                    /* ARROW  */
  YYSYMBOL_PART_ARROW = 256,               /* PART_ARROW  */
  YYSYMBOL_EARROW = 257,                   /* EARROW  */
  YYSYMBOL_HSV = 258,                      /* HSV  */
  YYSYMBOL_PART_HSV = 259,                 /* PART_HSV  */
  YYSYMBOL_EHSV = 260,                     /* EHSV  */
  YYSYMBOL_TOOLITEM = 261,                 /* TOOLITEM  */
  YYSYMBOL_PART_TOOLITEM = 262,            /* PART_TOOLITEM  */
  YYSYMBOL_ETOOLITEM = 263,                /* ETOOLITEM  */
  YYSYMBOL_FIXED = 264,                    /* FIXED  */
  YYSYMBOL_PART_FIXED = 265,               /* PART_FIXED  */
  YYSYMBOL_EFIXED = 266,                   /* EFIXED  */
  YYSYMBOL_LAYOUT = 267,                   /* LAYOUT  */
  YYSYMBOL_PART_LAYOUT = 268,              /* PART_LAYOUT  */
  YYSYMBOL_ELAYOUT = 269,                  /* ELAYOUT  */
  YYSYMBOL_SCROLLEDWINDOW = 270,           /* SCROLLEDWINDOW  */
  YYSYMBOL_PART_SCROLLEDWINDOW = 271,      /* PART_SCROLLEDWINDOW  */
  YYSYMBOL_ESCROLLEDWINDOW = 272,          /* ESCROLLEDWINDOW  */
  YYSYMBOL_HANDLEBOX = 273,                /* HANDLEBOX  */
  YYSYMBOL_PART_HANDLEBOX = 274,           /* PART_HANDLEBOX  */
  YYSYMBOL_EHANDLEBOX = 275,               /* EHANDLEBOX  */
  YYSYMBOL_TOOLPALETTE = 276,              /* TOOLPALETTE  */
  YYSYMBOL_PART_TOOLPALETTE = 277,         /* PART_TOOLPALETTE  */
  YYSYMBOL_ETOOLPALETTE = 278,             /* ETOOLPALETTE  */
  YYSYMBOL_TOOLITEMGROUP = 279,            /* TOOLITEMGROUP  */
  YYSYMBOL_PART_TOOLITEMGROUP = 280,       /* PART_TOOLITEMGROUP  */
  YYSYMBOL_ETOOLITEMGROUP = 281,           /* ETOOLITEMGROUP  */
  YYSYMBOL_CELLVIEW = 282,                 /* CELLVIEW  */
  YYSYMBOL_PART_CELLVIEW = 283,            /* PART_CELLVIEW  */
  YYSYMBOL_ECELLVIEW = 284,                /* ECELLVIEW  */
  YYSYMBOL_DRAWINGAREA = 285,              /* DRAWINGAREA  */
  YYSYMBOL_PART_DRAWINGAREA = 286,         /* PART_DRAWINGAREA  */
  YYSYMBOL_EDRAWINGAREA = 287,             /* EDRAWINGAREA  */
  YYSYMBOL_PART_OUTPUTFILE = 288,          /* PART_OUTPUTFILE  */
  YYSYMBOL_SOCKET = 289,                   /* SOCKET  */
  YYSYMBOL_PART_SOCKET = 290,              /* PART_SOCKET  */
  YYSYMBOL_ESOCKET = 291,                  /* ESOCKET  */
  YYSYMBOL_HSCROLLBAR = 292,               /* HSCROLLBAR  */
  YYSYMBOL_PART_HSCROLLBAR = 293,          /* PART_HSCROLLBAR  */
  YYSYMBOL_EHSCROLLBAR = 294,              /* EHSCROLLBAR  */
  YYSYMBOL_VSCROLLBAR = 295,               /* VSCROLLBAR  */
  YYSYMBOL_PART_VSCROLLBAR = 296,          /* PART_VSCROLLBAR  */
  YYSYMBOL_EVSCROLLBAR = 297,              /* EVSCROLLBAR  */
  YYSYMBOL_VIEWPORT = 298,                 /* VIEWPORT  */
  YYSYMBOL_PART_VIEWPORT = 299,            /* PART_VIEWPORT  */
  YYSYMBOL_EVIEWPORT = 300,                /* EVIEWPORT  */
  YYSYMBOL_COLORSELECTION = 301,           /* COLORSELECTION  */
  YYSYMBOL_PART_COLORSELECTION = 302,      /* PART_COLORSELECTION  */
  YYSYMBOL_ECOLORSELECTION = 303,          /* ECOLORSELECTION  */
  YYSYMBOL_FONTSELECTION = 304,            /* FONTSELECTION  */
  YYSYMBOL_PART_FONTSELECTION = 305,       /* PART_FONTSELECTION  */
  YYSYMBOL_EFONTSELECTION = 306,           /* EFONTSELECTION  */
  YYSYMBOL_HRULER = 307,                   /* HRULER  */
  YYSYMBOL_PART_HRULER = 308,              /* PART_HRULER  */
  YYSYMBOL_EHRULER = 309,                  /* EHRULER  */
  YYSYMBOL_VRULER = 310,                   /* VRULER  */
  YYSYMBOL_PART_VRULER = 311,              /* PART_VRULER  */
  YYSYMBOL_EVRULER = 312,                  /* EVRULER  */
  YYSYMBOL_PLUG = 313,                     /* PLUG  */
  YYSYMBOL_PART_PLUG = 314,                /* PART_PLUG  */
  YYSYMBOL_EPLUG = 315,                    /* EPLUG  */
  YYSYMBOL_316_ = 316,                     /* '>'  */
  YYSYMBOL_317_ = 317,                     /* ':'  */
  YYSYMBOL_318_ = 318,                     /* '!'  */
  YYSYMBOL_YYACCEPT = 319,                 /* $accept  */
  YYSYMBOL_window = 320,                   /* window  */
  YYSYMBOL_radio_group_scope = 321,        /* radio_group_scope  */
  YYSYMBOL_wlist = 322,                    /* wlist  */
  YYSYMBOL_widget = 323,                   /* widget  */
  YYSYMBOL_entry = 324,                    /* entry  */
  YYSYMBOL_edit = 325,                     /* edit  */
  YYSYMBOL_tree = 326,                     /* tree  */
  YYSYMBOL_chooser = 327,                  /* chooser  */
  YYSYMBOL_text = 328,                     /* text  */
  YYSYMBOL_button = 329,                   /* button  */
  YYSYMBOL_checkbox = 330,                 /* checkbox  */
  YYSYMBOL_radiobutton = 331,              /* radiobutton  */
  YYSYMBOL_progressbar = 332,              /* progressbar  */
  YYSYMBOL_list = 333,                     /* list  */
  YYSYMBOL_table = 334,                    /* table  */
  YYSYMBOL_combobox = 335,                 /* combobox  */
  YYSYMBOL_gvim = 336,                     /* gvim  */
  YYSYMBOL_pixmap = 337,                   /* pixmap  */
  YYSYMBOL_calendar = 338,                 /* calendar  */
  YYSYMBOL_toolbar = 339,                  /* toolbar  */
  YYSYMBOL_toolpalette = 340,              /* toolpalette  */
  YYSYMBOL_toolpalettewlist = 341,         /* toolpalettewlist  */
  YYSYMBOL_toolitemgroup = 342,            /* toolitemgroup  */
  YYSYMBOL_toolbarwlist = 343,             /* toolbarwlist  */
  YYSYMBOL_toolitem = 344,                 /* toolitem  */
  YYSYMBOL_toolbutton = 345,               /* toolbutton  */
  YYSYMBOL_toggletoolbutton = 346,         /* toggletoolbutton  */
  YYSYMBOL_radiotoolbutton = 347,          /* radiotoolbutton  */
  YYSYMBOL_menutoolbutton = 348,           /* menutoolbutton  */
  YYSYMBOL_separatortoolitem = 349,        /* separatortoolitem  */
  YYSYMBOL_menubar = 350,                  /* menubar  */
  YYSYMBOL_menuwlist = 351,                /* menuwlist  */
  YYSYMBOL_menu = 352,                     /* menu  */
  YYSYMBOL_menuitem = 353,                 /* menuitem  */
  YYSYMBOL_menuitemseparator = 354,        /* menuitemseparator  */
  YYSYMBOL_recentchoosermenu = 355,        /* recentchoosermenu  */
  YYSYMBOL_hseparator = 356,               /* hseparator  */
  YYSYMBOL_vseparator = 357,               /* vseparator  */
  YYSYMBOL_comboboxtext = 358,             /* comboboxtext  */
  YYSYMBOL_comboboxentry = 359,            /* comboboxentry  */
  YYSYMBOL_hscale = 360,                   /* hscale  */
  YYSYMBOL_vscale = 361,                   /* vscale  */
  YYSYMBOL_spinbutton = 362,               /* spinbutton  */
  YYSYMBOL_timer = 363,                    /* timer  */
  YYSYMBOL_togglebutton = 364,             /* togglebutton  */
  YYSYMBOL_statusbar = 365,                /* statusbar  */
  YYSYMBOL_statusicon = 366,               /* statusicon  */
  YYSYMBOL_accellabel = 367,               /* accellabel  */
  YYSYMBOL_arrow = 368,                    /* arrow  */
  YYSYMBOL_hsv = 369,                      /* hsv  */
  YYSYMBOL_hruler = 370,                   /* hruler  */
  YYSYMBOL_vruler = 371,                   /* vruler  */
  YYSYMBOL_colorbutton = 372,              /* colorbutton  */
  YYSYMBOL_fontbutton = 373,               /* fontbutton  */
  YYSYMBOL_terminal = 374,                 /* terminal  */
  YYSYMBOL_attr = 375,                     /* attr  */
  YYSYMBOL_label = 376,                    /* label  */
  YYSYMBOL_sensitive = 377,                /* sensitive  */
  YYSYMBOL_defaultvalue = 378,             /* defaultvalue  */
  YYSYMBOL_width = 379,                    /* width  */
  YYSYMBOL_height = 380,                   /* height  */
  YYSYMBOL_input = 381,                    /* input  */
  YYSYMBOL_output = 382,                   /* output  */
  YYSYMBOL_variable = 383,                 /* variable  */
  YYSYMBOL_action = 384,                   /* action  */
  YYSYMBOL_item = 385,                     /* item  */
  YYSYMBOL_tagattr = 386,                  /* tagattr  */
  YYSYMBOL_imperative = 387,               /* imperative  */
  YYSYMBOL_assignment = 388,               /* assignment  */
  YYSYMBOL_expression = 389,               /* expression  */
  YYSYMBOL_if = 390,                       /* if  */
  YYSYMBOL_then = 391,                     /* then  */
  YYSYMBOL_endif = 392,                    /* endif  */
  YYSYMBOL_while = 393,                    /* while  */
  YYSYMBOL_ewhile = 394,                   /* ewhile  */
  YYSYMBOL_do = 395,                       /* do  */
  YYSYMBOL_linkbutton = 396,               /* linkbutton  */
  YYSYMBOL_spinner = 397,                  /* spinner  */
  YYSYMBOL_filechooserbutton = 398,        /* filechooserbutton  */
  YYSYMBOL_iconview = 399,                 /* iconview  */
  YYSYMBOL_cellview = 400,                 /* cellview  */
  YYSYMBOL_drawingarea = 401,              /* drawingarea  */
  YYSYMBOL_scalebutton = 402,              /* scalebutton  */
  YYSYMBOL_volumebutton = 403,             /* volumebutton  */
  YYSYMBOL_recentchooser = 404,            /* recentchooser  */
  YYSYMBOL_socket = 405,                   /* socket  */
  YYSYMBOL_hscrollbar = 406,               /* hscrollbar  */
  YYSYMBOL_vscrollbar = 407                /* vscrollbar  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  33
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   11120

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  319
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  368
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1301

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   565


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   318,     2,     2,     2,     2,     2,     2,
       2,     2,   120,   119,     2,   118,     2,   121,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   317,     2,
       2,   117,   316,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   284,   285,   286,   287,   288,   289,
     290,   291,   292,   293,   294,   295,   296,   297,   298,   299,
     300,   301,   302,   303,   304,   305,   306,   307,   308,   309,
     310,   311,   312,   313,   314,   315
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   251,   251,   256,   261,   266,   271,   276,   280,   284,
     289,   294,   298,   302,   305,   310,   315,   320,   328,   334,
     335,   338,   339,   340,   344,   349,   353,   358,   362,   367,
     371,   376,   380,   385,   389,   394,   398,   403,   407,   412,
     416,   421,   425,   430,   434,   439,   443,   448,   452,   457,
     461,   466,   470,   475,   479,   484,   488,   493,   497,   502,
     506,   511,   515,   520,   524,   529,   533,   538,   542,   547,
     551,   556,   560,   565,   569,   574,   578,   583,   587,   592,
     596,   601,   605,   610,   614,   619,   623,   628,   632,   637,
     641,   646,   650,   655,   659,   664,   669,   678,   679,   680,
     681,   682,   683,   684,   685,   686,   687,   688,   689,   690,
     691,   692,   693,   694,   695,   696,   697,   698,   699,   700,
     701,   702,   703,   704,   705,   708,   711,   712,   715,   718,
     719,   720,   721,   722,   723,   724,   725,   726,   727,   728,
     729,   730,   731,   732,   733,   734,   735,   736,   737,   738,
     742,   745,   748,   753,   756,   759,   765,   768,   771,   777,
     780,   783,   789,   792,   795,   799,   800,   803,   804,   805,
     806,   807,   811,   814,   817,   823,   826,   829,   835,   838,
     841,   847,   850,   853,   859,   862,   865,   871,   874,   880,
     884,   885,   891,   894,   907,   910,   914,   921,   924,   928,
     935,   936,   942,   945,   949,   956,   957,   958,   959,   960,
     961,   962,   965,   968,   971,   974,   977,   983,   986,   990,
     997,  1000,  1006,  1009,  1015,  1018,  1024,  1027,  1031,  1038,
    1041,  1047,  1050,  1053,  1057,  1060,  1067,  1068,  1069,  1070,
    1074,  1077,  1081,  1086,  1090,  1098,  1101,  1105,  1108,  1115,
    1118,  1125,  1128,  1132,  1135,  1142,  1145,  1151,  1154,  1160,
    1163,  1169,  1172,  1178,  1181,  1187,  1190,  1196,  1199,  1205,
    1208,  1214,  1217,  1223,  1226,  1232,  1235,  1241,  1244,  1250,
    1253,  1259,  1262,  1268,  1271,  1277,  1280,  1286,  1289,  1295,
    1298,  1304,  1307,  1313,  1314,  1315,  1316,  1317,  1318,  1319,
    1320,  1321,  1322,  1323,  1327,  1332,  1337,  1342,  1347,  1352,
    1355,  1358,  1361,  1364,  1370,  1374,  1377,  1383,  1386,  1392,
    1395,  1401,  1404,  1407,  1413,  1416,  1422,  1423,  1426,  1429,
    1436,  1442,  1445,  1448,  1451,  1454,  1457,  1460,  1463,  1469,
    1473,  1480,  1487,  1491,  1499,  1506,  1509,  1515,  1518,  1524,
    1527,  1533,  1536,  1542,  1545,  1551,  1554,  1560,  1563,  1569,
    1572,  1578,  1581,  1587,  1590,  1596,  1599,  1605,  1608
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "WINDOW",
  "PART_WINDOW", "EWINDOW", "VBOX", "PART_VBOX", "EVBOX", "HBOX",
  "PART_HBOX", "EHBOX", "NOTEBOOK", "ENOTEBOOK", "PART_NOTEBOOK", "FRAME",
  "TAG_ATTR_NAME", "EFRAME", "ENTRY", "EENTRY", "PART_ENTRY", "MENUBAR",
  "PART_MENUBAR", "EMENUBAR", "MENU", "PART_MENU", "EMENU", "POPUPMENU",
  "PART_POPUPMENU", "EPOPUPMENU", "MENUITEM", "PART_MENUITEM", "EMENUITEM",
  "MENUITEMSEPARATOR", "EMENUITEMSEPARATOR", "EDIT", "PART_EDIT", "EEDIT",
  "TREE", "PART_TREE", "ETREE", "CHOOSER", "PART_CHOOSER", "ECHOOSER",
  "LABEL", "ELABEL", "ITEM", "EITEM", "PART_ITEM", "BUTTON", "PART_BUTTON",
  "EBUTTON", "BUTTONOK", "BUTTONCANCEL", "BUTTONHELP", "BUTTONYES",
  "BUTTONNO", "CHECKBOX", "ECHECKBOX", "PART_CHECKBOX", "RADIO", "ERADIO",
  "PART_RADIO", "PROGRESSBAR", "EPROGRESSBAR", "PART_PROGRESSBAR", "LIST",
  "PART_LIST", "ELIST", "TABLE", "ETABLE", "PART_TABLE", "COMBOBOX",
  "PART_COMBOBOX", "ECOMBOBOX", "GVIM", "EGVIM", "TEXT", "PART_TEXT",
  "ETEXT", "PIXMAP", "PART_PIXMAP", "EPIXMAP", "DEFAULT", "EDEFAULT",
  "SENSITIVE", "ESENSITIVE", "VARIABLE", "PART_VARIABLE", "EVARIABLE",
  "WIDTH", "EWIDTH", "HEIGHT", "EHEIGHT", "INPUT", "INPUTFILE", "EINPUT",
  "PART_INPUT", "PART_INPUTFILE", "OUTPUT", "OUTPUTFILE", "EOUTPUT",
  "ACTION", "EACTION", "PART_ACTION", "COMM", "ENDCOMM", "IF", "ENDIF",
  "WHILE", "EWHILE", "SHOW_WIDGETS", "EMB_VARIABLE", "EMB_NUMBER",
  "END_OF_FILE", "NUMBER", "STRING", "'='", "'-'", "'+'", "'*'", "'/'",
  "UMINUS", "HSEPARATOR", "PART_HSEPARATOR", "EHSEPARATOR", "VSEPARATOR",
  "PART_VSEPARATOR", "EVSEPARATOR", "COMBOBOXTEXT", "PART_COMBOBOXTEXT",
  "ECOMBOBOXTEXT", "COMBOBOXENTRY", "PART_COMBOBOXENTRY", "ECOMBOBOXENTRY",
  "HSCALE", "PART_HSCALE", "EHSCALE", "VSCALE", "PART_VSCALE", "EVSCALE",
  "SPINBUTTON", "PART_SPINBUTTON", "ESPINBUTTON", "TIMER", "PART_TIMER",
  "ETIMER", "TOGGLEBUTTON", "PART_TOGGLEBUTTON", "ETOGGLEBUTTON",
  "STATUSBAR", "PART_STATUSBAR", "ESTATUSBAR", "COLORBUTTON",
  "PART_COLORBUTTON", "ECOLORBUTTON", "FONTBUTTON", "PART_FONTBUTTON",
  "EFONTBUTTON", "TERMINAL", "PART_TERMINAL", "ETERMINAL", "EVENTBOX",
  "PART_EVENTBOX", "EEVENTBOX", "EXPANDER", "PART_EXPANDER", "EEXPANDER",
  "HPANED", "PART_HPANED", "EHPANED", "VPANED", "PART_VPANED", "EVPANED",
  "ALIGNMENT", "PART_ALIGNMENT", "EALIGNMENT", "HBUTTONBOX",
  "PART_HBUTTONBOX", "EHBUTTONBOX", "VBUTTONBOX", "PART_VBUTTONBOX",
  "EVBUTTONBOX", "TOOLBAR", "PART_TOOLBAR", "ETOOLBAR", "TOOLBUTTON",
  "PART_TOOLBUTTON", "ETOOLBUTTON", "TOGGLETOOLBUTTON",
  "PART_TOGGLETOOLBUTTON", "ETOGGLETOOLBUTTON", "RADIOTOOLBUTTON",
  "PART_RADIOTOOLBUTTON", "ERADIOTOOLBUTTON", "MENUTOOLBUTTON",
  "PART_MENUTOOLBUTTON", "EMENUTOOLBUTTON", "SEPARATORTOOLITEM",
  "PART_SEPARATORTOOLITEM", "ESEPARATORTOOLITEM", "CALENDAR",
  "PART_CALENDAR", "ECALENDAR", "LINKBUTTON", "PART_LINKBUTTON",
  "ELINKBUTTON", "SPINNER", "PART_SPINNER", "ESPINNER", "GRID",
  "PART_GRID", "EGRID", "FILECHOOSERBUTTON", "PART_FILECHOOSERBUTTON",
  "EFILECHOOSERBUTTON", "INFOBAR", "PART_INFOBAR", "EINFOBAR", "ASSISTANT",
  "PART_ASSISTANT", "EASSISTANT", "ICONVIEW", "PART_ICONVIEW", "EICONVIEW",
  "SCALEBUTTON", "PART_SCALEBUTTON", "ESCALEBUTTON", "VOLUMEBUTTON",
  "PART_VOLUMEBUTTON", "EVOLUMEBUTTON", "ASPECTFRAME", "PART_ASPECTFRAME",
  "EASPECTFRAME", "MESSAGEDIALOG", "PART_MESSAGEDIALOG", "EMESSAGEDIALOG",
  "DIALOG", "PART_DIALOG", "EDIALOG", "ABOUTDIALOG", "PART_ABOUTDIALOG",
  "EABOUTDIALOG", "RECENTCHOOSER", "PART_RECENTCHOOSER", "ERECENTCHOOSER",
  "RECENTCHOOSERMENU", "PART_RECENTCHOOSERMENU", "ERECENTCHOOSERMENU",
  "STATUSICON", "PART_STATUSICON", "ESTATUSICON", "ACCELLABEL",
  "PART_ACCELLABEL", "EACCELLABEL", "ARROW", "PART_ARROW", "EARROW", "HSV",
  "PART_HSV", "EHSV", "TOOLITEM", "PART_TOOLITEM", "ETOOLITEM", "FIXED",
  "PART_FIXED", "EFIXED", "LAYOUT", "PART_LAYOUT", "ELAYOUT",
  "SCROLLEDWINDOW", "PART_SCROLLEDWINDOW", "ESCROLLEDWINDOW", "HANDLEBOX",
  "PART_HANDLEBOX", "EHANDLEBOX", "TOOLPALETTE", "PART_TOOLPALETTE",
  "ETOOLPALETTE", "TOOLITEMGROUP", "PART_TOOLITEMGROUP", "ETOOLITEMGROUP",
  "CELLVIEW", "PART_CELLVIEW", "ECELLVIEW", "DRAWINGAREA",
  "PART_DRAWINGAREA", "EDRAWINGAREA", "PART_OUTPUTFILE", "SOCKET",
  "PART_SOCKET", "ESOCKET", "HSCROLLBAR", "PART_HSCROLLBAR", "EHSCROLLBAR",
  "VSCROLLBAR", "PART_VSCROLLBAR", "EVSCROLLBAR", "VIEWPORT",
  "PART_VIEWPORT", "EVIEWPORT", "COLORSELECTION", "PART_COLORSELECTION",
  "ECOLORSELECTION", "FONTSELECTION", "PART_FONTSELECTION",
  "EFONTSELECTION", "HRULER", "PART_HRULER", "EHRULER", "VRULER",
  "PART_VRULER", "EVRULER", "PLUG", "PART_PLUG", "EPLUG", "'>'", "':'",
  "'!'", "$accept", "window", "radio_group_scope", "wlist", "widget",
  "entry", "edit", "tree", "chooser", "text", "button", "checkbox",
  "radiobutton", "progressbar", "list", "table", "combobox", "gvim",
  "pixmap", "calendar", "toolbar", "toolpalette", "toolpalettewlist",
  "toolitemgroup", "toolbarwlist", "toolitem", "toolbutton",
  "toggletoolbutton", "radiotoolbutton", "menutoolbutton",
  "separatortoolitem", "menubar", "menuwlist", "menu", "menuitem",
  "menuitemseparator", "recentchoosermenu", "hseparator", "vseparator",
  "comboboxtext", "comboboxentry", "hscale", "vscale", "spinbutton",
  "timer", "togglebutton", "statusbar", "statusicon", "accellabel",
  "arrow", "hsv", "hruler", "vruler", "colorbutton", "fontbutton",
  "terminal", "attr", "label", "sensitive", "defaultvalue", "width",
  "height", "input", "output", "variable", "action", "item", "tagattr",
  "imperative", "assignment", "expression", "if", "then", "endif", "while",
  "ewhile", "do", "linkbutton", "spinner", "filechooserbutton", "iconview",
  "cellview", "drawingarea", "scalebutton", "volumebutton",
  "recentchooser", "socket", "hscrollbar", "vscrollbar", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-803)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-237)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      15,  -803,    -3,   -24,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,    77, 10832,  3248,   -96,   -10,
    -803,   513,    -8,  3248,    -6,  5407,    -5,  3248,     7,  5425,
       9,  3248,    10,  -803,   -85,   158,    -3,   -20,   -15,    13,
      -3,   112,   114,   116,   120,    -3,    -3,   128,   130,   131,
      -3,    -3,  3248,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
    -803,    -3,   261,    -3,   110,    -3,  -803,    -3,    98,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,  -803,  -803,
    -803,  -803,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,
    -803,    -3,  -803,    -3,  -803,  -803,    -3,  -803,    -3,   108,
    -803,  -803,  -803,   -21,    -3,    75,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,   -62,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -134,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  3554,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,   531,   190,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,   106,   106,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,   132,   134,  -803,   531,
    -803,  -803,  3554,  -803,  -803,  -803,  3554,  -803,  -803,  -803,
    3554,  -803,   162,  -803,   180,    11,   154,   156,   148,    14,
     140,   146,   174,   191,    16,    17,   176,   187,   186,    18,
      19,  3554,  3248,    20,  3248,    21,  3248,    22,  3248,  3823,
      23,  -803,   543,    24,  -803,   513,    28,  4418,    29,  -803,
    4726,    30,  4874,    31,  5039,    32,  5486,    33,  5504,  5531,
    5561,  5588,  5606,  5179,    34,  5197,    35,  5259,    36,  5284,
      37,  5322,    38,  5633,    39,  5671,  5350,    40,  5696,    41,
     -84,   -76,  -803,    42,  -803,    43,  5716,    44,  5741,    45,
    5798,    46,  5816,    48,  5878,    49,  5898,    50,  5943,    51,
    5963,    54,  6008,    55,  6026,    57,  6053,    59,  3248,    70,
    3248,    72,  3248,    74,  3248,    76,  3248,    78,  3248,    83,
    3248,    86,  -803,   783,    87,  6121,    89,  6145,    90,  6178,
      91,  3248,    93,  6223,    95,  3248,    97,  6245,    99,  6308,
     101,  6328,   103,  3248,   105,  6373,   109,  6391,   111,  6453,
     115,  6476,   117,  6538,   118,  6558,   119,  3248,   121,  3248,
     122,  3248,   123,  3248,   124,  -803,  -133,   125,  6583,   126,
    6603,   127,  5057,   129,  4892,   133,  4748,   135,  3248,   137,
    4306,   138,  4078,   139,  3904,   141,  3859,   142,  -803,    -3,
    -803,    -3,  -803,    -3,  -803,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,    -3,  -803,
      -3,  -803,    -3,  -803,    -3,  -803,   782,  -803,  -803,    -3,
    -803,    -3,   207,  -803,    -3,   513,    -3,  -803,  -803,   104,
     144,  -803,   152,  3248,  3247,   513,  6648,  3248,  6668,  6713,
    3248,  6737,  2353,  3248,  -803,  -803,   160,  -803,  -803,  -803,
     188,  -803,  -803,  -803,  -803,   193,   113,  -803,  -803,  -803,
     196,   198,  3554,  -803,  3554,  -803,  3554,  -803,  3554,  -803,
    -803,  -803,  3949,   513,   531,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,   199,   189,   192,
     194,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,  -803,    -3,  -803,
      -3,  -803,    -3,    60,    -3,  -803,    -3,   -18,    -3,   783,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  3554,  -803,  -803,  -803,  3554,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  3554,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,   -38,    -3,  -133,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  3554,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  3248,   143,  3248,   145,  3248,   147,  3248,  3248,
     149,  3248,   150,  3248,   151,  3248,   153,  3248,   155,  3248,
     157,  3248,   159,  3248,   161,  3248,   163,  3248,   164,  3248,
     165,  3248,   166,  3248,   167,  3248,   168,  3248,   169,  -803,
     513,   170,  4436,   171,  -803,  6811,   172,   543,   173,   106,
     106,   106,   106,   106,  -803,   201,  -803,  -803,  3554,  -803,
     531,  -803,  3554,  -803,  -803,  3554,  -803,  -803,  3554,   211,
     182,   197,  -803,   204,   216,   219,  1199,  3248,  1745,  3248,
    1895,  3248,  2427,  3841,  -803,   543,  4153,   513,  4516,  4807,
    4977,  5120,  6829,  6874,  6919,  6939,  6984,  7010,  7041,  7086,
    7111,   106,  -803,  -803,  -803,  7131,  7156,  7176,  7233,  7251,
    7308,  7333,  7401,  7425,  7484,  7510,  7541,  3248,  7600,  3248,
    7624,  3248,  7662,  3248,  7686,  3248,  7749,  3248,  7769,  3248,
    7832,   175,  7893,   177,  7911,   179,  -803,   513,   181,  7929,
     183,  -803,  3248,   184,  -803,  -803,  -803,  -803,  -803,  -803,
    7956,   783,  7988,  8019,  8045,  8078,  3248,  8110,  8139,  3248,
    8175,  8201,  8273,  8291,  3248,  8348,  8370,  8393,  8431,  8454,
    8488,  8511,  3248,  8576,  3248,  8599,  3248,  8662,  3248,  -803,
     783,   185,  -803,  8719,  -133,  8737,  8782,  5102,  4954,  4771,
    4498,  3248,  4324,  4171,  3922,  3886,  3554,  -803,  3554,  -803,
    3554,  -803,  3554,  3554,  -803,  3554,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,   531,  -803,  -803,  -803,  -803,  -803,  4006,
     513,   178,   -92,   -92,   -64,   -64,  3248,   106,  3248,   907,
    4400,  8805,  8864,  2449,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,  -803,  4033,  -803,
     531,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,    96,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,
    -803,  -803,  -803,  -803,  -803,  -803,   531,  -803,  -803,  -803,
    3554,  -803,  -803,   783,  -803,  -803,  -803,  -803,  3554,  -803,
    -803,  3554,  -803,  -803,  -803,  -803,  3554,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  3554,  -803,  3554,  -803,  3554,  -803,
    3554,   783,  -803,  -803,  -133,  -803,  -803,  -803,  -803,  -803,
    -803,  3554,  -803,  -803,  -803,  -803,  1222,  3248,  1773,  3248,
    2272,  3248,  2941,  8882,  3248,  8945,  3248,  8990,  3248,  9019,
    3248,  9051,  3248,  9081,  3248,  9110,  3248,  9155,  3248,  9182,
    3248,  9218,  3248,  9243,  3248,  9288,  3248,  9314,  3248,  9377,
    3248,  4534,  3248,  4198,   513,  4610,  9400,  -803,   543,  2636,
     178,  2942,  -803,  -803,  -803,  -803,  -803,  1338,  1807,  2317,
    -803,  4261,  9445,  9468,  9490,  9559,  9579,  9597,  9624,  9692,
    9715,  9737,  9794,   513,  9826,  9853,  3248,  9871,  9898,  9960,
    9986, 10045, 10063, 10108, 10126, 10183,   783, 10201,  4628,  -803,
    3554,  -803,  3554,  -803,  3554,  -803,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,  -803,  3554,
    -803,  3554,  -803,  3554,  -803,  3554,  -803,   531,  -803,  -803,
    4051,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
     531,  -803,  -803,  3554,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,   783,  -803,  -803,  1520,  1830,  2335, 10258,
   10291, 10321, 10398, 10421, 10455, 10478, 10516, 10541, 10579, 10602,
   10647, 10670, 10692,  4646,  4288,  -803, 10749, 10787, 10810,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     293,    18,     0,    18,     0,    18,     0,   293,     0,    18,
       0,   293,     0,    18,     0,     0,    18,     0,     0,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   296,   295,   294,   297,   298,   299,   300,
     301,   302,   303,    18,     0,    18,     0,    18,     0,    18,
     293,     0,     0,     0,    18,     0,   293,     0,     0,   293,
       0,   293,     0,   293,     0,   293,     0,   293,   293,   293,
     293,   293,   293,     0,   293,     0,   293,     0,   293,     0,
     293,     0,   293,     0,   293,   293,     0,   293,     0,     0,
     339,   342,   327,     0,     0,     0,     0,   293,     0,   293,
       0,   293,     0,   293,     0,   293,     0,   293,     0,   293,
       0,   293,     0,   293,     0,   293,     0,   293,     0,    18,
       0,    18,     0,    18,     0,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,   293,     0,   293,     0,   293,
       0,    18,     0,   293,     0,    18,     0,   293,     0,   293,
       0,   293,     0,    18,     0,   293,     0,   293,     0,   293,
       0,   293,     0,   293,     0,   293,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    18,     0,   293,     0,   293,
       0,   293,     0,   293,     0,   293,     0,    18,     0,   293,
       0,   293,     0,   293,     0,   293,     0,   293,    19,    98,
      99,   100,   101,    97,   102,   103,   104,   105,   106,   107,
     108,   110,   109,   132,   130,   131,   112,     0,   236,   237,
     238,   239,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   142,   143,   144,   145,   146,   147,   123,   126,
     129,    21,     0,     0,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   111,   148,   149,     0,     0,    18,   293,
     236,    18,   293,    18,     7,   293,   293,    18,    11,   293,
     293,    18,     0,   322,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     2,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   231,   293,     0,   240,     0,     0,     0,     0,   249,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   255,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   194,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   197,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      18,     0,    18,     0,    18,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    20,     0,    22,    18,     0,
     293,     0,     0,   293,     0,     0,     0,   331,   332,     0,
       0,   324,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   304,   321,     0,   306,   305,   317,
       0,   307,   308,   309,   311,     0,     0,   314,   315,   319,
       0,     0,   293,    18,   293,    18,   293,    18,   293,   152,
     150,   293,     0,     0,   293,    18,   245,   293,   155,   153,
     293,   158,   156,   293,   161,   159,   293,   165,   293,   167,
     168,   169,   171,   170,   174,   172,   293,   177,   175,   293,
     180,   178,   293,   183,   181,   293,   186,   184,   293,   187,
     293,   189,   164,   162,   293,   190,   293,     0,     0,     0,
       0,   259,   293,   261,   293,   263,   293,   265,   293,   267,
     293,   269,   293,   271,   293,   273,   293,   287,   293,   289,
     293,   291,   293,   293,    18,   293,    18,   293,    18,   293,
      18,   293,    18,   293,    18,   293,    18,   293,     0,   293,
       0,   293,     0,    18,     0,   293,     0,    18,     0,   293,
     206,   205,   207,   208,   209,   210,    18,   192,   293,   345,
     293,   347,   293,   293,    18,   349,   293,   293,    18,   351,
     293,   357,   293,   359,   293,   293,    18,   361,   293,   251,
     293,   275,   293,   277,   293,   279,   293,   281,   293,   293,
      18,   293,    18,   293,    18,   293,    18,    18,     0,   293,
     200,    18,   353,   293,   355,   293,   363,   293,   365,   293,
     367,   293,   293,    18,   124,   293,   127,   293,   283,   293,
     285,   293,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     3,
       0,     0,     0,     0,   250,     0,     0,   293,     0,     0,
       0,     0,     0,     0,   340,     0,   344,   325,   293,    14,
     293,     5,   293,     8,     9,   293,    12,    16,   293,     0,
       0,     0,   313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,   293,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   326,   256,   258,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   226,     0,     0,     0,
       0,   217,     0,     0,   212,   211,   213,   214,   215,   216,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   202,
       0,     0,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,    18,   293,    18,
     293,    18,   293,   293,    18,   293,    18,   293,    18,   293,
      18,   293,    18,   293,    18,   293,    18,   293,    18,   293,
      18,   293,    18,   293,    18,   293,    18,   293,    18,   293,
      18,   293,    18,   293,    18,   246,   293,   252,   293,     0,
       0,   337,   334,   333,   335,   336,     0,     0,     0,     0,
       0,     0,     0,     0,   323,   318,   310,   312,   320,   316,
      55,   293,    59,   293,    91,   293,    95,   151,     0,   241,
     293,   247,   154,   157,   160,   166,   173,   176,   179,   182,
     185,   188,   163,   191,   330,   260,   262,   264,   266,   268,
     270,   272,   274,   288,   290,   292,    79,   293,    83,   293,
      71,   293,    75,   293,    23,   293,    63,   293,    67,   293,
     220,   293,   222,   293,   224,   293,   293,    18,   229,   293,
     293,    18,   195,   293,   193,   346,   348,    31,   293,   350,
      87,   293,   352,   358,   360,    27,   293,   362,   253,   276,
     278,   280,   282,    35,   293,    39,   293,    43,   293,    51,
     293,   293,    18,   198,   293,   354,   356,   364,   366,   368,
      47,   293,   125,   128,   284,   286,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   233,   293,     0,
     338,     0,     4,    15,     6,    10,    17,     0,     0,     0,
     234,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
     293,    60,   293,    92,   293,    96,    80,   293,    84,   293,
      72,   293,    76,   293,    24,   293,    64,   293,    68,   293,
      32,   293,    88,   293,    28,   293,    36,   293,    40,   293,
      44,   293,    52,   293,    48,   293,   242,   293,   248,   254,
       0,   341,   328,   343,   329,    57,    61,    93,   243,    81,
      85,    73,    77,    25,    65,    69,   221,   223,   225,   227,
     293,   230,   218,   293,   196,    33,    89,    29,    37,    41,
      45,    53,   203,   293,   199,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,    58,
      62,    94,    82,    86,    74,    78,    26,    66,    70,    34,
      90,    30,    38,    42,    46,    54,    50,   244,   228,   219,
     204
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -803,  -803,  1076,   -11,   463,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -655,  -706,  -802,  -658,  -657,  -652,  -645,  -637,
    -635,  -803,   -12,    -4,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,    -7,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  2098,  1018,  -803,
    -248,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,
    -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803,  -803
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    15,    17,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   235,   709,   710,   659,   660,   661,   662,   663,   664,
     665,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,    16,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    19,   261,   361,
     519,   262,   976,  1222,   263,  1224,   978,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,   874,   875,   912,    29,    20,   277,   876,   277,   279,
     277,   277,   282,    18,   877,   520,   286,   280,     1,     2,
     290,   276,   878,   277,   879,   277,   277,   277,   782,   783,
     277,   292,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   311,     3,     4,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   319,   277,   277,   277,   277,   322,   327,
     277,   277,   330,   277,   332,   277,   334,    33,   336,  1053,
     338,   339,   340,   341,   342,   343,   277,   345,   277,   347,
     277,   349,   277,   351,   277,   353,   296,   355,   356,   277,
     358,   297,   277,   277,   362,   277,   277,   277,  1081,   277,
     366,   277,   368,   277,   370,   277,   372,   277,   374,   277,
     376,   277,   378,   402,   380,   277,   382,   277,   384,   298,
     386,   277,   329,   277,   277,   277,   324,   277,   277,   277,
     277,   277,   277,   277,   445,   277,   707,   708,   405,   277,
     407,   277,   409,   277,   277,   277,   413,   277,   277,   277,
     417,   277,   419,   277,   421,   277,   277,   277,   425,   277,
     427,   277,   429,   277,   431,   277,   433,   277,   435,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     448,   277,   450,   277,   452,   277,   454,   277,   456,   277,
     277,   277,   460,   364,   462,   293,   464,   534,   466,   802,
     506,   515,   516,   779,   780,   781,   782,   783,   517,   518,
     360,   779,   780,   781,   782,   783,   785,   535,   300,   803,
     301,   541,   302,   607,     5,     6,   303,   539,   537,   542,
     608,   774,   538,   909,   306,   871,   307,   308,   521,     7,
       8,   522,     9,    10,   785,    11,    12,   866,   984,  1084,
       0,   779,   780,   781,   782,   783,     0,     0,   787,     0,
     543,   985,   524,     0,   294,   526,   799,   547,   528,   529,
       0,     0,   531,   532,   321,    74,    75,   544,   548,   549,
       0,    76,    77,   986,    78,   832,   780,   781,   782,   783,
     987,   552,     0,   554,   800,   556,   278,   558,   281,   801,
     283,   285,   804,   564,   805,   562,   831,   833,   977,   988,
     989,   280,   834,   287,     0,   289,   291,   536,    13,    14,
     540,     0,   545,   546,   550,   551,   553,   555,   557,   561,
     563,     0,     0,     0,   565,   567,   570,   573,   576,   578,
     586,   589,   592,   595,   598,   600,   604,   606,   609,   610,
     612,   614,   616,     0,   618,   620,   622,   624,     0,     0,
     626,   628,     0,   630,  1253,   632,     0,   633,   912,   635,
       0,   637,     0,   639,     0,   641,   634,   643,   636,   645,
     638,     0,   640,     0,   642,   874,   875,     0,     0,   644,
     673,   876,   646,   666,   677,   668,   670,   672,   877,   674,
       0,   676,   685,   678,   785,   680,   878,   682,   879,   684,
     784,   686,   785,   874,   875,   688,   699,   690,   701,   876,
     703,   692,   705,   694,   696,   698,   877,   700,   702,   704,
     706,   711,   713,   715,   878,   717,   879,   722,     0,   719,
       0,   721,     0,   723,   725,   727,     0,   729,   731,   927,
     786,   929,   785,   931,     0,   934,   936,   938,     0,   940,
       0,   942,     0,   944,     0,   946,     0,   948,     0,   950,
     952,   954,   956,   958,   960,   962,   964,   966,   968,   970,
       0,  1041,     0,  1043,     0,  1045,   785,  1047,     0,  1049,
    1051,  1082,     0,   772,     0,     0,   775,   177,   178,     0,
       0,   777,   788,   790,     0,     0,   792,     0,     0,   795,
       0,   280,   798,     0,     0,     0,     0,     0,     0,     0,
       0,   971,   972,   973,   974,   975,     0,    74,    75,     0,
       0,     0,     0,    76,    77,   806,    78,   808,     0,   810,
       0,   812,     0,     0,   813,   508,   509,   816,     0,   815,
     818,   510,   511,   819,   512,     0,   820,  -236,  -236,   821,
       0,   822,     0,  -236,  -236,     0,  -236,     0,     0,   823,
       0,     0,   824,  1014,     0,   825,     0,     0,   826,     0,
       0,   827,     0,   828,     0,   874,   875,   829,     0,   830,
       0,   876,     0,     0,     0,   835,     0,   836,   877,   837,
       0,   838,     0,   839,     0,   840,   878,   841,   879,   842,
       0,   843,     0,   844,     0,   845,   846,     0,   848,     0,
     850,     0,   852,     0,   854,     0,   856,     0,   858,     0,
     860,     0,   862,     0,   864,     0,     0,     0,   869,     0,
       0,     0,   880,     0,     0,     0,     0,     0,     0,     0,
       0,   882,     0,   883,     0,   884,   885,     0,     0,   887,
     888,     0,     0,   890,     0,   891,     0,   892,   893,     0,
     505,   895,     0,   896,     0,   897,     0,   898,     0,   899,
       0,   900,   901,     0,   903,     0,   905,     0,   907,     0,
       0,     0,   913,     0,     0,     0,   915,     0,   916,     0,
     917,     0,   918,     0,   919,   920,     0,     0,   922,     0,
     923,   926,   924,   928,   925,   930,     0,   932,   933,  1140,
     935,     0,   937,     0,   939,     0,   941,     0,   943,     0,
     945,     0,   947,     0,   949,   505,   951,     0,   953,   505,
     955,     0,   957,   505,   959,     0,   961,     0,   963,   177,
     178,     0,     0,     0,     0,     0,   280,     0,     0,     0,
     969,     0,     0,     0,   505,     0,     0,   513,   514,     0,
       0,   979,     0,   980,     0,   981,     0,   769,   982,  -236,
    -236,   983,     0,     0,     0,     0,   991,     0,   993,     0,
     995,     0,     0,     0,     0,  1000,     0,     0,   998,     0,
       0,     0,     0,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,  1027,     0,  1029,     0,
    1031,     0,  1033,     0,  1035,     0,  1037,     0,  1039,     0,
       0,     0,     0,     0,     0,  1046,     0,     0,     0,     0,
       0,  1050,     0,   280,     0,    37,     0,    38,     0,    39,
      40,     0,    41,     0,    42,  1058,    43,    44,  1061,    45,
      46,    47,    48,  1066,    49,     0,    50,     0,     0,     0,
       0,  1074,     0,  1076,     0,  1078,     0,  1080,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1091,     0,  1142,     0,     0,     0,     0,     0,     0,  1096,
       0,  1098,     0,  1100,     0,  1102,  1103,     0,  1105,     0,
    1107,     0,  1109,     0,  1111,     0,  1113,     0,  1115,     0,
    1117,     0,  1119,     0,  1121,     0,  1123,     0,  1125,     0,
    1127,    34,  1129,    35,  1131,    36,  1133,     0,     0,  1135,
       0,  1136,     0,     0,     0,  1139,  1138,  1141,     0,   647,
     648,     0,   649,   650,     0,   651,   652,     0,   653,   654,
       0,   655,   656,     0,  1147,     0,  1148,     0,  1149,     0,
      37,     0,    38,  1151,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,     0,   505,     0,   505,     0,   505,
    1152,   505,  1153,     0,  1154,     0,  1155,     0,  1156,     0,
    1157,     0,  1158,     0,  1159,     0,  1160,     0,  1161,  1162,
       0,     0,  1164,  1165,   657,   658,  1167,     0,     0,     0,
       0,  1168,     0,     0,  1169,     0,     0,     0,     0,  1170,
       0,     0,     0,     0,     0,     0,     0,  1171,     0,  1172,
      51,  1173,     0,  1174,  1175,     0,     0,  1177,     0,    21,
       0,    23,     0,     0,  1178,    27,  1180,     0,  1182,    31,
    1184,     0,    52,  1187,     0,  1189,   505,  1191,   505,  1193,
     505,  1195,   505,  1197,   505,  1199,   505,  1201,   505,  1203,
       0,  1205,     0,  1207,     0,  1209,     0,  1211,     0,  1213,
       0,  1215,  1217,     0,     0,     0,     0,     0,     0,     0,
     280,  1220,     0,     0,     0,     0,   505,     0,     0,   312,
     505,   314,     0,   316,     0,   318,     0,     0,   505,     0,
     325,  1240,     0,     0,     0,  1243,     0,     0,     0,   280,
       0,     0,   505,     0,   505,     0,   505,     0,   505,     0,
       0,     0,     0,  1256,     0,  1257,     0,  1258,     0,     0,
    1259,     0,  1260,     0,  1261,   505,  1262,     0,  1263,     0,
    1264,     0,  1265,     0,  1266,    51,  1267,     0,  1268,     0,
    1269,     0,  1270,     0,  1271,     0,  1272,   990,  1273,     0,
    1274,     0,     0,     0,     0,   388,     0,   390,     0,   392,
       0,   394,     0,   396,     0,   398,     0,   400,     0,   403,
    1179,     0,     0,  1276,     0,   507,  1277,   411,     0,     0,
       0,   415,     0,    34,     0,    35,  1278,    36,     0,   423,
       0,   505,     0,     0,     0,   505,     0,     0,   505,     0,
       0,   505,     0,   437,     0,   439,    34,   441,    35,   443,
      36,   446,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,   458,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
     507,    49,     0,    50,   507,    37,     0,    38,   507,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,   507,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1225,     0,     0,     0,
       0,     0,     0,     0,   523,     0,     0,   525,     0,   527,
       0,     0,     0,   530,     0,     0,     0,   533,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,   505,
       0,   505,     0,   505,     0,   505,   505,     0,   505,     0,
     505,     0,   505,     0,   505,     0,   505,     0,   505,     0,
     505,     0,   505,     0,   505,     0,   505,     0,   505,     0,
     505,    37,   505,    38,   505,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   505,     0,   505,     0,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
     505,     0,   505,     0,   505,     0,   505,     0,   505,     0,
     505,     0,   505,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,   505,     0,     0,     0,     0,     0,     0,
       0,   505,     0,     0,   505,     0,     0,     0,  1279,   505,
       0,     0,     0,     0,     0,     0,     0,   505,     0,   505,
       0,   505,     0,   505,   732,     0,   734,     0,   736,     0,
     738,   739,     0,   741,   505,   743,     0,   745,     0,   747,
       0,   749,     0,   751,    34,   753,    35,   755,    36,   757,
     507,   759,   507,   761,   507,   763,   507,   765,     0,   767,
       0,     0,     0,     0,   770,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   505,    37,   505,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,    51,     0,     0,   807,
       0,   809,     0,   811,     0,     0,     0,     0,     0,     0,
       0,   817,     0,   505,     0,   505,     0,   505,     0,     0,
     505,   507,   505,   507,   505,   507,   505,   507,   505,   507,
     505,   507,   505,   507,   505,     0,   505,     0,   505,     0,
     505,     0,   505,     0,   505,     0,   505,     0,   505,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   507,     0,     0,     0,   507,     0,     0,     0,     0,
       0,     0,     0,   507,     0,     0,   505,     0,     0,     0,
     847,     0,   849,     0,   851,     0,   853,   507,   855,   507,
     857,   507,   859,   507,     0,     0,     0,     0,     0,   867,
       0,     0,     0,   872,     0,     0,     0,     0,     0,     0,
     507,     0,   881,     0,     0,     0,     0,     0,     0,     0,
     886,     0,     0,     0,   889,     0,   992,     0,     0,     0,
       0,     0,   894,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   902,     0,   904,     0,
     906,     0,   908,   910,  1181,     0,     0,   914,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,   921,
       0,     0,     0,     0,     0,     0,   507,     0,    51,     0,
     507,     0,     0,   507,     0,     0,   507,    34,  1226,    35,
       0,    36,     0,     0,     0,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,  1280,    45,    46,    47,    48,     0,    49,     0,    50,
       0,    34,     0,    35,     0,    36,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,    34,    49,    35,    50,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,   994,    49,
       0,    50,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,     0,     0,    34,
       0,    35,     0,    36,   507,     0,   507,     0,   507,     0,
     507,   507,     0,   507,     0,   507,     0,   507,     0,   507,
       0,   507,     0,   507,     0,   507,     0,   507,     0,   507,
       0,   507,     0,   507,     0,   507,     0,   507,    37,   507,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,     0,     0,  1097,     0,  1099,     0,  1101,     0,   507,
    1104,   507,  1106,   507,  1108,     0,  1110,     0,  1112,     0,
    1114,     0,  1116,     0,  1118,     0,  1120,     0,  1122,     0,
    1124,     0,  1126,    51,  1128,     0,  1130,     0,  1132,     0,
    1134,     0,     0,     0,     0,   507,     0,   507,     0,   507,
       0,   507,     0,   507,     0,   507,     0,   507,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,   507,     0,
       0,     0,     0,     0,     0,     0,   507,     0,     0,   507,
       0,     0,     0,     0,   507,     0,     0,     0,     0,     0,
       0,     0,   507,     0,   507,    51,   507,     0,   507,     0,
       0,     0,    22,     0,    24,     0,    26,     0,    28,   507,
      30,     0,    32,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,  1163,     0,     0,     0,  1166,     0,     0,
       0,     0,     0,     0,   295,     0,     0,     0,   299,     0,
       0,     0,     0,   304,   305,     0,     0,     0,   309,   310,
       0,     0,     0,     0,     0,     0,     0,   507,  1176,   507,
       0,     0,   313,     0,   315,     0,   317,     0,     0,   320,
       0,   323,     0,   326,     0,   328,     0,     0,   331,     0,
     333,     0,   335,    51,   337,     0,     0,     0,     0,     0,
       0,   344,     0,   346,     0,   348,     0,   350,   507,   352,
     507,   354,   507,     0,   357,   507,   359,   507,     0,   507,
       0,   507,   363,   507,   365,   507,   367,   507,   369,   507,
     371,   507,   373,   507,   375,   507,   377,   507,   379,   507,
     381,   507,   383,   507,   385,     0,   387,     0,   389,     0,
     391,     0,   393,     0,   395,     0,   397,     0,   399,     0,
     401,     0,   404,     0,   406,     0,   408,     0,   410,     0,
     412,   507,   414,     0,   416,     0,   418,     0,   420,     0,
     422,     0,   424,     0,   426,     0,   428,     0,   430,     0,
     432,     0,   434,     0,   436,  1183,   438,     0,   440,     0,
     442,     0,   444,     0,   447,     0,   449,     0,   451,     0,
     453,     0,   455,     0,   457,     0,   459,     0,   461,     0,
     463,     0,   465,     0,   467,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1227,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1281,     0,
       0,     0,     0,     0,     0,    37,     0,    38,     0,    39,
      40,    34,    41,    35,    42,    36,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,    35,
      37,    36,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,    37,    49,
      38,    50,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,    37,    49,    38,    50,
      39,    40,     0,    41,   996,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,   733,     0,   735,
       0,   737,     0,     0,   740,     0,   742,     0,   744,     0,
     746,     0,   748,     0,   750,     0,   752,     0,   754,     0,
     756,     0,   758,     0,   760,     0,   762,     0,   764,     0,
     766,     0,   768,     0,     0,    51,     0,   771,     0,   773,
       0,     0,   776,     0,   778,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,   468,   469,     0,   470,   471,     0,   472,     0,
     473,   474,     0,     0,    70,     0,    71,    72,    73,     0,
      74,    75,     0,     0,     0,     0,    76,    77,   797,    78,
       0,    79,    80,     0,    81,    82,     0,    83,    84,     0,
       0,     0,     0,     0,     0,    85,    86,     0,    87,    88,
      89,    90,    91,    92,     0,    93,    94,     0,    95,    96,
       0,    97,    98,    99,     0,   100,     0,   101,   102,   103,
       0,   104,     0,   105,   106,    51,   107,   108,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,   109,     0,   110,  1221,   111,   861,   112,   863,     0,
     865,     0,   868,     0,   870,     0,   873,     0,     0,   113,
     114,     0,   115,   116,  1146,   117,   118,     0,   119,   120,
       0,   121,   122,     0,   123,   124,     0,   125,   126,     0,
     127,   128,     0,   129,   130,     0,   131,   132,     0,   133,
     134,     0,   135,   136,     0,   137,   138,     0,   475,   476,
       0,   477,   478,     0,   479,   480,   911,   481,   482,     0,
     483,   484,     0,   485,   486,     0,   487,   488,     0,   153,
     154,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   156,     0,
     157,   158,     0,   159,   160,     0,   489,   490,     0,   163,
     164,     0,   491,   492,     0,     0,     0,     0,   167,   168,
       0,   169,   170,     0,   171,   172,     0,   493,   494,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   175,
     176,     0,   177,   178,     0,   179,   180,     0,   181,   182,
       0,   183,   184,     0,   185,   186,     0,     0,     0,     0,
     495,   496,     0,   497,   498,     0,   499,   500,     0,   501,
     502,     0,   195,   196,     0,     0,     0,     0,   197,   198,
       0,   199,   200,     0,     0,   201,   202,     0,   203,   204,
       0,   205,   206,     0,   503,   504,     0,   209,   210,     0,
     211,   212,     0,   213,   214,     0,   215,   216,   468,   469,
       0,   470,   471,     0,   472,     0,   473,   474,  1185,     0,
      70,     0,    71,    72,    73,     0,    74,    75,     0,     0,
       0,     0,    76,    77,     0,    78,     0,    79,    80,     0,
      81,    82,     0,    83,    84,    34,     0,    35,     0,    36,
       0,    85,    86,     0,    87,    88,    89,    90,    91,    92,
       0,    93,    94,     0,    95,    96,     0,    97,    98,    99,
       0,   100,     0,   101,   102,   103,     0,   104,     0,   105,
     106,     0,   107,   108,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,   109,     0,   110,
       0,   111,  1223,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   113,   114,     0,   115,   116,
       0,   117,   118,     0,   119,   120,     0,   121,   122,     0,
     123,   124,     0,   125,   126,     0,   127,   128,     0,   129,
     130,     0,   131,   132,     0,   133,   134,     0,   135,   136,
       0,   137,   138,     0,   475,   476,     0,   477,   478,     0,
     479,   480,     0,   481,   482,     0,   483,   484,     0,   485,
     486,     0,   487,   488,     0,   153,   154,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   155,   156,     0,   157,   158,     0,   159,
     160,     0,   489,   490,     0,   163,   164,     0,   491,   492,
       0,     0,     0,     0,   167,   168,     0,   169,   170,     0,
     171,   172,     0,   493,   494,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   176,     0,   177,   178,
       0,   179,   180,     0,   181,   182,     0,   183,   184,     0,
     185,   186,     0,     0,     0,     0,   495,   496,     0,   497,
     498,     0,   499,   500,     0,   501,   502,     0,   195,   196,
       0,     0,     0,     0,   197,   198,     0,   199,   200,    51,
       0,   201,   202,     0,   203,   204,     0,   205,   206,     0,
     503,   504,     0,   209,   210,     0,   211,   212,     0,   213,
     214,     0,   215,   216,    63,    64,     0,    65,    66,     0,
      67,     0,    68,    69,     0,     0,    70,     0,    71,    72,
      73,     0,    74,    75,     0,     0,   789,     0,    76,    77,
       0,    78,     0,    79,    80,     0,    81,    82,     0,    83,
      84,    34,     0,    35,     0,    36,     0,    85,    86,     0,
      87,    88,    89,    90,    91,    92,     0,    93,    94,     0,
      95,    96,     0,    97,    98,    99,     0,   100,     0,   101,
     102,   103,     0,   104,     0,   105,   106,     0,   107,   108,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,   109,     0,   110,     0,   111,     0,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   113,   114,     0,   115,   116,     0,   117,   118,     0,
     119,   120,     0,   121,   122,     0,   123,   124,     0,   125,
     126,     0,   127,   128,     0,   129,   130,     0,   131,   132,
       0,   133,   134,     0,   135,   136,     0,   137,   138,     0,
     139,   140,     0,   141,   142,     0,   143,   144,     0,   145,
     146,     0,   147,   148,     0,   149,   150,     0,   151,   152,
       0,   153,   154,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   155,
     156,     0,   157,   158,     0,   159,   160,     0,   161,   162,
       0,   163,   164,     0,   165,   166,     0,     0,     0,     0,
     167,   168,     0,   169,   170,     0,   171,   172,     0,   173,
     174,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,   176,     0,   177,   178,     0,   179,   180,     0,
     181,   182,     0,   183,   184,     0,   185,   186,     0,     0,
       0,     0,   187,   188,     0,   189,   190,     0,   191,   192,
       0,   193,   194,     0,   195,   196,     0,     0,     0,     0,
     197,   198,     0,   199,   200,    51,     0,   201,   202,     0,
     203,   204,     0,   205,   206,     0,   207,   208,     0,   209,
     210,     0,   211,   212,     0,   213,   214,     0,   215,   216,
     468,   469,     0,   470,   471,     0,   472,     0,   473,   474,
       0,     0,    70,     0,    71,    72,    73,     0,    74,    75,
       0,     0,     0,     0,    76,    77,     0,    78,     0,    79,
      80,     0,    81,    82,     0,    83,    84,     0,     0,     0,
       0,     0,     0,    85,    86,     0,    87,    88,    89,    90,
      91,    92,     0,    93,    94,     0,    95,    96,     0,    97,
      98,    99,     0,   100,     0,   101,   102,   103,     0,   104,
       0,   105,   106,     0,   107,   108,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   109,
       0,   110,     0,   111,     0,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   113,   114,     0,
     115,   116,     0,   117,   118,     0,   119,   120,     0,   121,
     122,     0,   123,   124,     0,   125,   126,     0,   127,   128,
       0,   129,   130,     0,   131,   132,     0,   133,   134,     0,
     135,   136,     0,   137,   138,     0,   475,   476,     0,   477,
     478,     0,   479,   480,     0,   481,   482,     0,   483,   484,
       0,   485,   486,     0,   487,   488,     0,   153,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   156,     0,   157,   158,
       0,   159,   160,     0,   489,   490,     0,   163,   164,     0,
     491,   492,     0,     0,     0,     0,   167,   168,     0,   169,
     170,     0,   171,   172,     0,   493,   494,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,   176,     0,
     177,   178,     0,   179,   180,     0,   181,   182,     0,   183,
     184,     0,   185,   186,     0,     0,     0,     0,   495,   496,
       0,   497,   498,     0,   499,   500,     0,   501,   502,     0,
     195,   196,     0,     0,     0,     0,   197,   198,     0,   199,
     200,   559,   560,   201,   202,     0,   203,   204,     0,   205,
     206,     0,   503,   504,     0,   209,   210,     0,   211,   212,
     997,   213,   214,     0,   215,   216,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    35,    37,    36,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,    37,    49,    38,    50,    39,    40,    34,    41,
      35,    42,    36,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,    34,     0,    35,    37,
      36,    38,   814,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    37,    49,    38,
      50,    39,    40,    34,    41,    35,    42,    36,    43,    44,
       0,    45,    46,    47,    48,    37,    49,    38,    50,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,  1137,
       0,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      34,    49,    35,    50,    36,     0,  1150,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1275,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,    51,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,    51,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,    51,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,   730,    43,    44,    51,    45,    46,    47,    48,   999,
      49,     0,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,    34,  1095,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,   728,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,  1216,     0,     0,     0,     0,     0,
       0,  1094,     0,     0,     0,     0,    37,    51,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,  1228,    41,     0,
      42,     0,    43,    44,    51,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,  1297,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,    51,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,    51,     0,    34,     0,
      35,    37,    36,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,   726,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    37,    49,    38,
      50,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,  1143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,    34,     0,    35,     0,    36,     0,
     566,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,    34,     0,    35,     0,    36,     0,   965,     0,
       0,     0,     0,     0,     0,     0,     0,  1093,     0,     0,
      34,     0,    35,    37,    36,    38,    51,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,    37,    49,    38,    50,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,    34,     0,    35,     0,    36,     0,  1001,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,    34,     0,
      35,    37,    36,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,    51,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,   724,
      43,    44,    51,    45,    46,    47,    48,    37,    49,    38,
      50,    39,    40,     0,    41,     0,    42,  1092,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,     0,  1218,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
      34,     0,    35,    37,    36,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,    51,    45,    46,    47,
      48,    37,    49,    38,    50,    39,    40,     0,    41,     0,
      42,     0,    43,    44,    51,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   568,     0,   569,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,  1090,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,    51,    45,    46,    47,    48,     0,    49,     0,
      50,    37,     0,    38,  1214,    39,    40,     0,    41,     0,
      42,     0,    43,    44,  1002,    45,    46,    47,    48,     0,
      49,    34,    50,    35,    37,    36,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,    38,     0,    39,    40,     0,    41,    51,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,   571,     0,   572,     0,    51,     0,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,  1255,     0,
       0,     0,     0,     0,    51,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,  1296,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    37,    49,    38,    50,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,  1003,     0,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    37,     0,    38,
       0,    39,    40,     0,    41,   720,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,    51,
      37,     0,    38,     0,    39,    40,     0,    41,  1089,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
     574,    50,   575,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      37,    49,    38,    50,    39,    40,    34,    41,    35,    42,
      36,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,    51,  1004,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,    37,   718,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,    37,    49,    38,    50,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,    34,    50,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,   584,   585,     0,     0,
       0,    34,    51,    35,     0,    36,     0,     0,  1088,     0,
       0,     0,     0,     0,     0,     0,     0,   587,   588,     0,
       0,     0,    37,     0,    38,    51,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      37,    49,    38,    50,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   590,   591,     0,     0,     0,    51,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    38,    51,    39,    40,   716,    41,
     593,    42,   594,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,    34,    37,    35,    38,
      36,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
      51,   596,   597,  1087,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,    37,     0,    38,    51,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,   602,     0,   603,
       0,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,    34,    49,    35,    50,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,    37,    49,
      38,    50,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
      34,     0,    35,     0,    36,     0,     0,   577,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,    34,     0,
      35,     0,    36,     0,     0,   579,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,    38,    51,    39,    40,    34,    41,    35,    42,    36,
      43,    44,   580,    45,    46,    47,    48,    37,    49,    38,
      50,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    34,    49,    35,    50,    36,
      51,     0,   581,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,    51,   582,
       0,     0,     0,   284,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,   583,    45,    46,
      47,    48,     0,    49,     0,    50,     0,   288,     0,     0,
       0,    37,     0,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,    51,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,   599,    49,     0,
      50,     0,     0,    51,     0,    34,    37,    35,    38,    36,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
      34,     0,    35,     0,    36,     0,     0,   601,     0,     0,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,     0,    49,    51,    50,     0,     0,   605,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,    51,    45,    46,    47,    48,     0,    49,    37,
      50,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,    51,
      50,     0,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,   611,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   613,    51,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,    51,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,    51,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,   615,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   617,     0,     0,    51,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,    37,    50,    38,    51,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,    51,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   619,     0,     0,     0,     0,    37,     0,    38,    51,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,   621,    49,    37,    50,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,    37,   623,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,    51,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,   625,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,     0,   627,     0,    34,    51,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   629,     0,    51,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,   631,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,    37,     0,
      38,    51,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,    51,    42,     0,    43,    44,     0,
      45,    46,    47,    48,   667,    49,     0,    50,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,    51,    45,    46,    47,    48,     0,    49,     0,    50,
       0,   669,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   671,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,    51,
      49,    37,    50,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,    51,     0,    34,     0,    35,   675,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,    51,    43,    44,   679,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,   681,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,   683,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,   687,     0,
       0,    37,     0,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,   689,
      49,    37,    50,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,     0,
      49,    51,    50,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,    51,
      45,    46,    47,    48,     0,    49,    37,    50,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,   691,    49,     0,    50,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     693,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,    51,    43,    44,     0,    45,    46,    47,    48,     0,
      49,    37,    50,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,    51,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,     0,     0,     0,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   695,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,   697,     0,
      37,     0,    38,     0,    39,    40,    51,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,   712,     0,   791,
       0,    51,     0,    34,     0,    35,     0,    36,     0,     0,
    1005,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     714,    51,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,   793,    43,    44,     0,    45,    46,
      47,    48,    37,    49,    38,    50,    39,    40,    34,    41,
      35,    42,    36,    43,    44,     0,    45,    46,    47,    48,
       0,    49,  1006,    50,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   794,     0,     0,     0,    51,    37,     0,    38,
       0,    39,    40,    34,    41,    35,    42,    36,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,   796,
    1007,     0,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    37,  1008,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,    37,    50,    38,    51,    39,    40,    34,    41,
      35,    42,    36,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,     0,     0,     0,     0,
       0,     0,  1009,     0,    34,     0,    35,     0,    36,   967,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
    1010,    45,    46,    47,    48,    34,    49,    35,    50,    36,
       0,     0,     0,    37,     0,    38,     0,    39,    40,    51,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,  1011,     0,    51,     0,     0,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,    51,     0,     0,  1012,     0,     0,     0,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,  1013,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,    51,    45,    46,
      47,    48,     0,    49,    37,    50,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,    51,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,    37,
      50,    38,  1015,    39,    40,     0,    41,     0,    42,     0,
      43,    44,    51,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,     0,     0,     0,     0,     0,     0,     0,
    1016,     0,     0,     0,     0,    34,     0,    35,    51,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1017,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,    51,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1018,    51,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    38,  1019,    39,    40,     0,    41,    51,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,    37,     0,    38,    51,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,     0,     0,    51,    34,     0,    35,     0,    36,
       0,     0,     0,     0,  1020,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,  1021,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,    51,    45,    46,    47,    48,     0,    49,    34,    50,
      35,     0,    36,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1022,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
    1023,    45,    46,    47,    48,    34,    49,    35,    50,    36,
       0,     0,     0,    37,     0,    38,    51,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,     0,     0,     0,
       0,    51,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,  1024,    49,    34,    50,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
      35,  1025,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,    38,     0,    39,    40,    51,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,  1026,    34,    37,    35,    38,
      36,    39,    40,    51,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,  1028,     0,    37,
       0,    38,    51,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,    34,  1030,    35,     0,    36,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,    37,     0,    38,  1032,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,    37,    50,    38,     0,    39,    40,     0,    41,
       0,    42,  1034,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,    37,     0,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,  1036,    45,
      46,    47,    48,     0,    49,     0,    50,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
      51,  1038,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    51,    35,    37,    36,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,    37,    49,    38,    50,    39,    40,     0,    41,
    1040,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,    34,    50,    35,     0,    36,    51,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    51,    49,     0,
      50,     0,     0,    34,     0,    35,     0,    36,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,  1042,    45,    46,    47,    48,    34,
      49,    35,    50,    36,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    38,  1044,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      51,    49,    34,    50,    35,     0,    36,     0,    37,  1048,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,  1052,    45,    46,    47,    48,     0,    49,     0,    50,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,    51,    50,    34,     0,    35,     0,    36,     0,     0,
       0,  1054,     0,    37,     0,    38,     0,    39,    40,    51,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,    51,     0,    34,
       0,    35,    37,    36,    38,  1055,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,    51,    34,     0,    35,     0,    36,
       0,     0,     0,     0,  1056,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,    51,    49,     0,    50,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
    1057,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,  1059,     0,     0,     0,     0,
       0,     0,     0,    51,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,  1060,    38,     0,
      39,    40,     0,    41,     0,    42,    51,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,    51,  1062,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,    51,  1063,     0,
       0,    37,     0,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,    51,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,    34,    37,    35,    38,    36,
      39,    40,     0,    41,     0,    42,     0,    43,    44,    51,
      45,    46,    47,    48,     0,    49,     0,    50,    34,     0,
      35,     0,    36,  1064,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,  1065,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    34,    49,    35,    50,    36,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,    51,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,  1067,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,  1068,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,    34,  1069,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,    51,    37,     0,    38,  1070,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,    34,     0,    35,     0,
      36,  1071,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,    37,     0,    38,  1072,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,    34,    49,    35,    50,    36,     0,     0,
       0,     0,     0,     0,     0,     0,    51,  1073,     0,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      37,    49,    38,    50,    39,    40,    34,    41,    35,    42,
      36,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,     0,  1075,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    37,     0,    38,     0,    39,
      40,  1077,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,    51,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,    34,    50,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,  1079,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
      51,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    37,    49,    38,    50,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,  1083,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1085,     0,     0,     0,    51,  1144,     0,    37,     0,
      38,     0,    39,    40,    34,    41,    35,    42,    36,    43,
      44,     0,    45,    46,    47,    48,  1186,    49,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    35,     0,    36,     0,  1086,
      51,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,    51,    50,    34,     0,    35,     0,    36,
       0,     0,    37,  1145,    38,     0,    39,    40,     0,    41,
       0,    42,  1188,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    51,    49,    34,    50,    35,     0,    36,     0,
    1190,     0,     0,     0,    37,     0,    38,     0,    39,    40,
      51,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
       0,     0,  1192,    37,     0,    38,     0,    39,    40,    34,
      41,    35,    42,    36,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    34,  1194,    35,     0,
      36,     0,     0,    51,     0,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
    1196,     0,    34,     0,    35,    37,    36,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,    51,    45,
      46,    47,    48,     0,    49,     0,    50,    34,     0,    35,
       0,    36,  1198,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,    51,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,    37,     0,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,    51,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
      35,     0,    36,     0,     0,     0,     0,  1200,     0,    51,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,     0,    37,    51,    38,
    1202,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    34,     0,    35,     0,    36,     0,
       0,  1204,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
      51,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,    37,     0,    38,     0,    39,    40,    34,
      41,    35,    42,    36,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,    51,     0,     0,  1206,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      38,    51,    39,    40,    34,    41,    35,    42,    36,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,    37,     0,    38,     0,    39,    40,  1208,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,    37,     0,    38,    51,    39,    40,     0,
      41,     0,    42,     0,    43,    44,  1210,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,     0,     0,     0,
       0,     0,    51,    34,     0,    35,     0,    36,     0,  1229,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,  1230,     0,     0,     0,     0,
       0,    34,    37,    35,    38,    36,    39,    40,  1219,    41,
       0,    42,  1212,    43,    44,     0,    45,    46,    47,    48,
    1231,    49,    37,    50,    38,    51,    39,    40,    34,    41,
      35,    42,    36,    43,    44,     0,    45,    46,    47,    48,
      37,    49,    38,    50,    39,    40,     0,    41,    51,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,     0,     0,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,     0,  1232,    51,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1233,    51,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,  1234,    38,    51,    39,
      40,    34,    41,    35,    42,    36,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,    37,     0,
      38,     0,    39,    40,     0,    41,  1235,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,    34,    49,
      35,    50,    36,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
      34,     0,    35,     0,    36,     0,     0,    37,     0,    38,
    1236,    39,    40,     0,    41,    51,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,    34,    50,    35,
       0,    36,     0,     0,     0,     0,  1237,     0,     0,    37,
       0,    38,    51,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,  1238,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
      51,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,  1239,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,    51,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,  1241,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,  1244,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,    51,    45,    46,    47,    48,     0,    49,    34,
      50,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,    35,
    1245,    36,     0,     0,    51,     0,  1242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,    51,    45,    46,    47,    48,    37,    49,    38,    50,
      39,    40,    34,    41,    35,    42,    36,    43,    44,    51,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,  1246,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,  1247,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,    51,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,    51,    42,     0,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,     0,     0,
       0,  1248,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1249,    51,     0,    34,     0,    35,     0,    36,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,    51,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
    1250,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,    51,     0,     0,     0,
       0,  1251,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,    51,    43,    44,     0,    45,    46,
      47,    48,  1282,    49,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1283,     0,
       0,     0,     0,     0,  1252,    34,     0,    35,     0,    36,
       0,    51,     0,     0,     0,     0,     0,     0,     0,  1254,
       0,    37,     0,    38,     0,    39,    40,     0,    41,    51,
      42,  1284,    43,    44,     0,    45,    46,    47,    48,    34,
      49,    35,    50,    36,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,    51,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
      34,    37,    35,    38,    36,    39,    40,     0,    41,     0,
      42,  1285,    43,    44,     0,    45,    46,    47,    48,    51,
      49,     0,    50,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,  1286,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,    51,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,    34,    37,    35,    38,    36,    39,    40,
       0,    41,     0,    42,  1287,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1288,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,    37,    51,    38,     0,    39,
      40,    34,    41,    35,    42,    36,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,    51,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1289,     0,
      37,     0,    38,     0,    39,    40,    34,    41,    35,    42,
      36,    43,    44,    51,    45,    46,    47,    48,     0,    49,
       0,    50,     0,    37,     0,    38,     0,    39,    40,  1290,
      41,     0,    42,     0,    43,    44,    51,    45,    46,    47,
      48,     0,    49,     0,    50,    37,     0,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,    34,    49,    35,    50,    36,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,  1291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,    34,    37,    35,    38,    36,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,    51,  1292,     0,
      37,     0,    38,     0,    39,    40,    34,    41,    35,    42,
      36,    43,    44,     0,    45,    46,    47,    48,     0,    49,
      51,    50,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,    37,  1293,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,    51,    50,     0,     0,     0,
       0,     0,  1294,     0,     0,     0,  1298,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,  1295,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1299,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1300,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51
};

static const yytype_int16 yycheck[] =
{
       7,   659,   659,   709,    11,    29,    16,   659,    16,    21,
      16,    16,    23,    16,   659,   263,    27,    21,     3,     4,
      31,   117,   659,    16,   659,    16,    16,    16,   120,   121,
      16,   116,    16,    16,    16,    16,    16,    16,    16,    16,
      16,    52,    27,    28,    16,    16,    16,    16,    16,    16,
      16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
      16,    16,    16,    70,    16,    16,    16,    16,    72,    76,
      16,    16,    79,    16,    81,    16,    83,     0,    85,   881,
      87,    88,    89,    90,    91,    92,    16,    94,    16,    96,
      16,    98,    16,   100,    16,   102,   116,   104,   105,    16,
     107,   116,    16,    16,   125,    16,    16,    16,   910,    16,
     117,    16,   119,    16,   121,    16,   123,    16,   125,    16,
     127,    16,   129,   185,   131,    16,   133,    16,   135,   116,
     137,    16,    34,    16,    16,    16,    26,    16,    16,    16,
      16,    16,    16,    16,   278,    16,   279,   280,   155,    16,
     157,    16,   159,    16,    16,    16,   163,    16,    16,    16,
     167,    16,   169,    16,   171,    16,    16,    16,   175,    16,
     177,    16,   179,    16,   181,    16,   183,    16,   185,    16,
      16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
     197,    16,   199,    16,   201,    16,   203,    16,   205,    16,
      16,    16,   209,   128,   211,    47,   213,    45,   215,    96,
     217,    21,    22,   117,   118,   119,   120,   121,   112,   113,
     112,   117,   118,   119,   120,   121,   318,    47,   116,   116,
     116,    91,   116,   317,   219,   220,   116,    89,    84,    93,
     316,    34,    86,   281,   116,   263,   116,   116,   116,   234,
     235,   117,   237,   238,   318,   240,   241,   197,    47,   914,
      -1,   117,   118,   119,   120,   121,    -1,    -1,   116,    -1,
      96,    89,   279,    -1,   116,   282,   116,   101,   285,   286,
      -1,    -1,   289,   290,    23,    24,    25,    96,   101,   103,
      -1,    30,    31,    96,    33,   106,   118,   119,   120,   121,
      96,   312,    -1,   314,   116,   316,   316,   318,   316,   116,
     316,   316,   116,   325,   116,   322,   117,   125,   117,   103,
     101,   325,   128,   316,    -1,   316,   316,   316,   313,   314,
     316,    -1,   316,   316,   316,   316,   316,   316,   316,   316,
     316,    -1,    -1,    -1,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,    -1,   316,   316,   316,   316,    -1,    -1,
     316,   316,    -1,   316,  1176,   316,    -1,   388,  1084,   390,
      -1,   392,    -1,   394,    -1,   396,   316,   398,   316,   400,
     316,    -1,   316,    -1,   316,  1053,  1053,    -1,    -1,   316,
     411,  1053,   316,   316,   415,   316,   316,   316,  1053,   316,
      -1,   316,   423,   316,   318,   316,  1053,   316,  1053,   316,
     316,   316,   318,  1081,  1081,   316,   437,   316,   439,  1081,
     441,   316,   443,   316,   316,   316,  1081,   316,   316,   316,
     316,   316,   316,   316,  1081,   316,  1081,   458,    -1,   316,
      -1,   316,    -1,   316,   316,   316,    -1,   316,   316,   316,
     316,   316,   318,   316,    -1,   316,   316,   316,    -1,   316,
      -1,   316,    -1,   316,    -1,   316,    -1,   316,    -1,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
      -1,   316,    -1,   316,    -1,   316,   318,   316,    -1,   316,
     316,   316,    -1,   510,    -1,    -1,   513,   246,   247,    -1,
      -1,   515,   523,   525,    -1,    -1,   527,    -1,    -1,   530,
      -1,   525,   533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   779,   780,   781,   782,   783,    -1,    24,    25,    -1,
      -1,    -1,    -1,    30,    31,   552,    33,   554,    -1,   556,
      -1,   558,    -1,    -1,   561,    24,    25,   564,    -1,   563,
     567,    30,    31,   570,    33,    -1,   573,    24,    25,   576,
      -1,   578,    -1,    30,    31,    -1,    33,    -1,    -1,   586,
      -1,    -1,   589,   831,    -1,   592,    -1,    -1,   595,    -1,
      -1,   598,    -1,   600,    -1,  1253,  1253,   604,    -1,   606,
      -1,  1253,    -1,    -1,    -1,   612,    -1,   614,  1253,   616,
      -1,   618,    -1,   620,    -1,   622,  1253,   624,  1253,   626,
      -1,   628,    -1,   630,    -1,   632,   633,    -1,   635,    -1,
     637,    -1,   639,    -1,   641,    -1,   643,    -1,   645,    -1,
     647,    -1,   649,    -1,   651,    -1,    -1,    -1,   655,    -1,
      -1,    -1,   659,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   668,    -1,   670,    -1,   672,   673,    -1,    -1,   676,
     677,    -1,    -1,   680,    -1,   682,    -1,   684,   685,    -1,
     217,   688,    -1,   690,    -1,   692,    -1,   694,    -1,   696,
      -1,   698,   699,    -1,   701,    -1,   703,    -1,   705,    -1,
      -1,    -1,   709,    -1,    -1,    -1,   713,    -1,   715,    -1,
     717,    -1,   719,    -1,   721,   722,    -1,    -1,   725,    -1,
     727,   732,   729,   734,   731,   736,    -1,   738,   739,   977,
     741,    -1,   743,    -1,   745,    -1,   747,    -1,   749,    -1,
     751,    -1,   753,    -1,   755,   282,   757,    -1,   759,   286,
     761,    -1,   763,   290,   765,    -1,   767,    -1,   770,   246,
     247,    -1,    -1,    -1,    -1,    -1,   770,    -1,    -1,    -1,
     777,    -1,    -1,    -1,   311,    -1,    -1,   246,   247,    -1,
      -1,   788,    -1,   790,    -1,   792,    -1,     5,   795,   246,
     247,   798,    -1,    -1,    -1,    -1,   807,    -1,   809,    -1,
     811,    -1,    -1,    -1,    -1,   817,    -1,    -1,   815,    -1,
      -1,    -1,    -1,   817,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,   847,    -1,   849,    -1,
     851,    -1,   853,    -1,   855,    -1,   857,    -1,   859,    -1,
      -1,    -1,    -1,    -1,    -1,   867,    -1,    -1,    -1,    -1,
      -1,   872,    -1,   867,    -1,    83,    -1,    85,    -1,    87,
      88,    -1,    90,    -1,    92,   886,    94,    95,   889,    97,
      98,    99,   100,   894,   102,    -1,   104,    -1,    -1,    -1,
      -1,   902,    -1,   904,    -1,   906,    -1,   908,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     921,    -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,   926,
      -1,   928,    -1,   930,    -1,   932,   933,    -1,   935,    -1,
     937,    -1,   939,    -1,   941,    -1,   943,    -1,   945,    -1,
     947,    -1,   949,    -1,   951,    -1,   953,    -1,   955,    -1,
     957,    44,   959,    46,   961,    48,   963,    -1,    -1,   966,
      -1,   968,    -1,    -1,    -1,   976,   970,   978,    -1,   186,
     187,    -1,   189,   190,    -1,   192,   193,    -1,   195,   196,
      -1,   198,   199,    -1,   991,    -1,   993,    -1,   995,    -1,
      83,    -1,    85,  1000,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,    -1,   552,    -1,   554,    -1,   556,
    1027,   558,  1029,    -1,  1031,    -1,  1033,    -1,  1035,    -1,
    1037,    -1,  1039,    -1,  1041,    -1,  1043,    -1,  1045,  1046,
      -1,    -1,  1049,  1050,   261,   262,  1053,    -1,    -1,    -1,
      -1,  1058,    -1,    -1,  1061,    -1,    -1,    -1,    -1,  1066,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1074,    -1,  1076,
     288,  1078,    -1,  1080,  1081,    -1,    -1,  1084,    -1,     3,
      -1,     5,    -1,    -1,  1091,     9,  1097,    -1,  1099,    13,
    1101,    -1,    16,  1104,    -1,  1106,   633,  1108,   635,  1110,
     637,  1112,   639,  1114,   641,  1116,   643,  1118,   645,  1120,
      -1,  1122,    -1,  1124,    -1,  1126,    -1,  1128,    -1,  1130,
      -1,  1132,  1134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1134,  1138,    -1,    -1,    -1,    -1,   673,    -1,    -1,    63,
     677,    65,    -1,    67,    -1,    69,    -1,    -1,   685,    -1,
      74,  1163,    -1,    -1,    -1,  1166,    -1,    -1,    -1,  1163,
      -1,    -1,   699,    -1,   701,    -1,   703,    -1,   705,    -1,
      -1,    -1,    -1,  1180,    -1,  1182,    -1,  1184,    -1,    -1,
    1187,    -1,  1189,    -1,  1191,   722,  1193,    -1,  1195,    -1,
    1197,    -1,  1199,    -1,  1201,   288,  1203,    -1,  1205,    -1,
    1207,    -1,  1209,    -1,  1211,    -1,  1213,     8,  1215,    -1,
    1217,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,   143,
      -1,   145,    -1,   147,    -1,   149,    -1,   151,    -1,   153,
       8,    -1,    -1,  1240,    -1,   217,  1243,   161,    -1,    -1,
      -1,   165,    -1,    44,    -1,    46,  1253,    48,    -1,   173,
      -1,   788,    -1,    -1,    -1,   792,    -1,    -1,   795,    -1,
      -1,   798,    -1,   187,    -1,   189,    44,   191,    46,   193,
      48,   195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,   207,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
     282,   102,    -1,   104,   286,    83,    -1,    85,   290,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,   311,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,   281,    -1,   283,
      -1,    -1,    -1,   287,    -1,    -1,    -1,   291,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,   926,
      -1,   928,    -1,   930,    -1,   932,   933,    -1,   935,    -1,
     937,    -1,   939,    -1,   941,    -1,   943,    -1,   945,    -1,
     947,    -1,   949,    -1,   951,    -1,   953,    -1,   955,    -1,
     957,    83,   959,    85,   961,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   991,    -1,   993,    -1,   995,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
    1027,    -1,  1029,    -1,  1031,    -1,  1033,    -1,  1035,    -1,
    1037,    -1,  1039,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,  1050,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1058,    -1,    -1,  1061,    -1,    -1,    -1,     8,  1066,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1074,    -1,  1076,
      -1,  1078,    -1,  1080,   468,    -1,   470,    -1,   472,    -1,
     474,   475,    -1,   477,  1091,   479,    -1,   481,    -1,   483,
      -1,   485,    -1,   487,    44,   489,    46,   491,    48,   493,
     552,   495,   554,   497,   556,   499,   558,   501,    -1,   503,
      -1,    -1,    -1,    -1,   508,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1139,    83,  1141,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,   288,    -1,    -1,   553,
      -1,   555,    -1,   557,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   565,    -1,  1180,    -1,  1182,    -1,  1184,    -1,    -1,
    1187,   633,  1189,   635,  1191,   637,  1193,   639,  1195,   641,
    1197,   643,  1199,   645,  1201,    -1,  1203,    -1,  1205,    -1,
    1207,    -1,  1209,    -1,  1211,    -1,  1213,    -1,  1215,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   673,    -1,    -1,    -1,   677,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   685,    -1,    -1,  1243,    -1,    -1,    -1,
     634,    -1,   636,    -1,   638,    -1,   640,   699,   642,   701,
     644,   703,   646,   705,    -1,    -1,    -1,    -1,    -1,   653,
      -1,    -1,    -1,   657,    -1,    -1,    -1,    -1,    -1,    -1,
     722,    -1,   666,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     674,    -1,    -1,    -1,   678,    -1,    11,    -1,    -1,    -1,
      -1,    -1,   686,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   700,    -1,   702,    -1,
     704,    -1,   706,   707,    11,    -1,    -1,   711,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,   723,
      -1,    -1,    -1,    -1,    -1,    -1,   788,    -1,   288,    -1,
     792,    -1,    -1,   795,    -1,    -1,   798,    44,    11,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    11,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    44,    -1,    46,    -1,    48,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    44,   102,    46,   104,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    13,   102,
      -1,   104,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,   926,    -1,   928,    -1,   930,    -1,
     932,   933,    -1,   935,    -1,   937,    -1,   939,    -1,   941,
      -1,   943,    -1,   945,    -1,   947,    -1,   949,    -1,   951,
      -1,   953,    -1,   955,    -1,   957,    -1,   959,    83,   961,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,   927,    -1,   929,    -1,   931,    -1,   991,
     934,   993,   936,   995,   938,    -1,   940,    -1,   942,    -1,
     944,    -1,   946,    -1,   948,    -1,   950,    -1,   952,    -1,
     954,    -1,   956,   288,   958,    -1,   960,    -1,   962,    -1,
     964,    -1,    -1,    -1,    -1,  1027,    -1,  1029,    -1,  1031,
      -1,  1033,    -1,  1035,    -1,  1037,    -1,  1039,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,  1050,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1058,    -1,    -1,  1061,
      -1,    -1,    -1,    -1,  1066,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1074,    -1,  1076,   288,  1078,    -1,  1080,    -1,
      -1,    -1,     4,    -1,     6,    -1,     8,    -1,    10,  1091,
      12,    -1,    14,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,  1047,    -1,    -1,    -1,  1051,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1139,  1082,  1141,
      -1,    -1,    64,    -1,    66,    -1,    68,    -1,    -1,    71,
      -1,    73,    -1,    75,    -1,    77,    -1,    -1,    80,    -1,
      82,    -1,    84,   288,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    -1,    95,    -1,    97,    -1,    99,  1180,   101,
    1182,   103,  1184,    -1,   106,  1187,   108,  1189,    -1,  1191,
      -1,  1193,   114,  1195,   116,  1197,   118,  1199,   120,  1201,
     122,  1203,   124,  1205,   126,  1207,   128,  1209,   130,  1211,
     132,  1213,   134,  1215,   136,    -1,   138,    -1,   140,    -1,
     142,    -1,   144,    -1,   146,    -1,   148,    -1,   150,    -1,
     152,    -1,   154,    -1,   156,    -1,   158,    -1,   160,    -1,
     162,  1243,   164,    -1,   166,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,   176,    -1,   178,    -1,   180,    -1,
     182,    -1,   184,    -1,   186,    13,   188,    -1,   190,    -1,
     192,    -1,   194,    -1,   196,    -1,   198,    -1,   200,    -1,
     202,    -1,   204,    -1,   206,    -1,   208,    -1,   210,    -1,
     212,    -1,   214,    -1,   216,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,
      88,    44,    90,    46,    92,    48,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      83,    48,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    83,   102,
      85,   104,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    83,   102,    85,   104,
      87,    88,    -1,    90,    17,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,   469,    -1,   471,
      -1,   473,    -1,    -1,   476,    -1,   478,    -1,   480,    -1,
     482,    -1,   484,    -1,   486,    -1,   488,    -1,   490,    -1,
     492,    -1,   494,    -1,   496,    -1,   498,    -1,   500,    -1,
     502,    -1,   504,    -1,    -1,   288,    -1,   509,    -1,   511,
      -1,    -1,   514,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,     6,     7,    -1,     9,    10,    -1,    12,    -1,
      14,    15,    -1,    -1,    18,    -1,    20,    21,    22,    -1,
      24,    25,    -1,    -1,    -1,    -1,    30,    31,   315,    33,
      -1,    35,    36,    -1,    38,    39,    -1,    41,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    49,    50,    -1,    52,    53,
      54,    55,    56,    57,    -1,    59,    60,    -1,    62,    63,
      -1,    65,    66,    67,    -1,    69,    -1,    71,    72,    73,
      -1,    75,    -1,    77,    78,   288,    80,    81,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,   105,    -1,   107,   108,   109,   648,   111,   650,    -1,
     652,    -1,   654,    -1,   656,    -1,   658,    -1,    -1,   123,
     124,    -1,   126,   127,   315,   129,   130,    -1,   132,   133,
      -1,   135,   136,    -1,   138,   139,    -1,   141,   142,    -1,
     144,   145,    -1,   147,   148,    -1,   150,   151,    -1,   153,
     154,    -1,   156,   157,    -1,   159,   160,    -1,   162,   163,
      -1,   165,   166,    -1,   168,   169,   708,   171,   172,    -1,
     174,   175,    -1,   177,   178,    -1,   180,   181,    -1,   183,
     184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,   202,    -1,
     204,   205,    -1,   207,   208,    -1,   210,   211,    -1,   213,
     214,    -1,   216,   217,    -1,    -1,    -1,    -1,   222,   223,
      -1,   225,   226,    -1,   228,   229,    -1,   231,   232,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,
     244,    -1,   246,   247,    -1,   249,   250,    -1,   252,   253,
      -1,   255,   256,    -1,   258,   259,    -1,    -1,    -1,    -1,
     264,   265,    -1,   267,   268,    -1,   270,   271,    -1,   273,
     274,    -1,   276,   277,    -1,    -1,    -1,    -1,   282,   283,
      -1,   285,   286,    -1,    -1,   289,   290,    -1,   292,   293,
      -1,   295,   296,    -1,   298,   299,    -1,   301,   302,    -1,
     304,   305,    -1,   307,   308,    -1,   310,   311,     6,     7,
      -1,     9,    10,    -1,    12,    -1,    14,    15,    17,    -1,
      18,    -1,    20,    21,    22,    -1,    24,    25,    -1,    -1,
      -1,    -1,    30,    31,    -1,    33,    -1,    35,    36,    -1,
      38,    39,    -1,    41,    42,    44,    -1,    46,    -1,    48,
      -1,    49,    50,    -1,    52,    53,    54,    55,    56,    57,
      -1,    59,    60,    -1,    62,    63,    -1,    65,    66,    67,
      -1,    69,    -1,    71,    72,    73,    -1,    75,    -1,    77,
      78,    -1,    80,    81,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,   105,    -1,   107,
      -1,   109,   110,   111,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,    -1,   126,   127,
      -1,   129,   130,    -1,   132,   133,    -1,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,    -1,   147,
     148,    -1,   150,   151,    -1,   153,   154,    -1,   156,   157,
      -1,   159,   160,    -1,   162,   163,    -1,   165,   166,    -1,
     168,   169,    -1,   171,   172,    -1,   174,   175,    -1,   177,
     178,    -1,   180,   181,    -1,   183,   184,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   201,   202,    -1,   204,   205,    -1,   207,
     208,    -1,   210,   211,    -1,   213,   214,    -1,   216,   217,
      -1,    -1,    -1,    -1,   222,   223,    -1,   225,   226,    -1,
     228,   229,    -1,   231,   232,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   243,   244,    -1,   246,   247,
      -1,   249,   250,    -1,   252,   253,    -1,   255,   256,    -1,
     258,   259,    -1,    -1,    -1,    -1,   264,   265,    -1,   267,
     268,    -1,   270,   271,    -1,   273,   274,    -1,   276,   277,
      -1,    -1,    -1,    -1,   282,   283,    -1,   285,   286,   288,
      -1,   289,   290,    -1,   292,   293,    -1,   295,   296,    -1,
     298,   299,    -1,   301,   302,    -1,   304,   305,    -1,   307,
     308,    -1,   310,   311,     6,     7,    -1,     9,    10,    -1,
      12,    -1,    14,    15,    -1,    -1,    18,    -1,    20,    21,
      22,    -1,    24,    25,    -1,    -1,    29,    -1,    30,    31,
      -1,    33,    -1,    35,    36,    -1,    38,    39,    -1,    41,
      42,    44,    -1,    46,    -1,    48,    -1,    49,    50,    -1,
      52,    53,    54,    55,    56,    57,    -1,    59,    60,    -1,
      62,    63,    -1,    65,    66,    67,    -1,    69,    -1,    71,
      72,    73,    -1,    75,    -1,    77,    78,    -1,    80,    81,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,   105,    -1,   107,    -1,   109,    -1,   111,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,    -1,   126,   127,    -1,   129,   130,    -1,
     132,   133,    -1,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,    -1,   147,   148,    -1,   150,   151,
      -1,   153,   154,    -1,   156,   157,    -1,   159,   160,    -1,
     162,   163,    -1,   165,   166,    -1,   168,   169,    -1,   171,
     172,    -1,   174,   175,    -1,   177,   178,    -1,   180,   181,
      -1,   183,   184,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   201,
     202,    -1,   204,   205,    -1,   207,   208,    -1,   210,   211,
      -1,   213,   214,    -1,   216,   217,    -1,    -1,    -1,    -1,
     222,   223,    -1,   225,   226,    -1,   228,   229,    -1,   231,
     232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   243,   244,    -1,   246,   247,    -1,   249,   250,    -1,
     252,   253,    -1,   255,   256,    -1,   258,   259,    -1,    -1,
      -1,    -1,   264,   265,    -1,   267,   268,    -1,   270,   271,
      -1,   273,   274,    -1,   276,   277,    -1,    -1,    -1,    -1,
     282,   283,    -1,   285,   286,   288,    -1,   289,   290,    -1,
     292,   293,    -1,   295,   296,    -1,   298,   299,    -1,   301,
     302,    -1,   304,   305,    -1,   307,   308,    -1,   310,   311,
       6,     7,    -1,     9,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    18,    -1,    20,    21,    22,    -1,    24,    25,
      -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    -1,    38,    39,    -1,    41,    42,    -1,    -1,    -1,
      -1,    -1,    -1,    49,    50,    -1,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    67,    -1,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    77,    78,    -1,    80,    81,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,
      -1,   107,    -1,   109,    -1,   111,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,
     126,   127,    -1,   129,   130,    -1,   132,   133,    -1,   135,
     136,    -1,   138,   139,    -1,   141,   142,    -1,   144,   145,
      -1,   147,   148,    -1,   150,   151,    -1,   153,   154,    -1,
     156,   157,    -1,   159,   160,    -1,   162,   163,    -1,   165,
     166,    -1,   168,   169,    -1,   171,   172,    -1,   174,   175,
      -1,   177,   178,    -1,   180,   181,    -1,   183,   184,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   201,   202,    -1,   204,   205,
      -1,   207,   208,    -1,   210,   211,    -1,   213,   214,    -1,
     216,   217,    -1,    -1,    -1,    -1,   222,   223,    -1,   225,
     226,    -1,   228,   229,    -1,   231,   232,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   243,   244,    -1,
     246,   247,    -1,   249,   250,    -1,   252,   253,    -1,   255,
     256,    -1,   258,   259,    -1,    -1,    -1,    -1,   264,   265,
      -1,   267,   268,    -1,   270,   271,    -1,   273,   274,    -1,
     276,   277,    -1,    -1,    -1,    -1,   282,   283,    -1,   285,
     286,    18,    19,   289,   290,    -1,   292,   293,    -1,   295,
     296,    -1,   298,   299,    -1,   301,   302,    -1,   304,   305,
      19,   307,   308,    -1,   310,   311,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    46,    83,    48,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    83,   102,    85,   104,    87,    88,    44,    90,
      46,    92,    48,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    44,    -1,    46,    83,
      48,    85,    23,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    83,   102,    85,
     104,    87,    88,    44,    90,    46,    92,    48,    94,    95,
      -1,    97,    98,    99,   100,    83,   102,    85,   104,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,    23,
      -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      44,   102,    46,   104,    48,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    23,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,   288,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,   288,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,   288,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,   312,    94,    95,   288,    97,    98,    99,   100,    26,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    44,   312,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,   309,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,   309,    -1,    -1,    -1,    -1,    83,   288,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    26,    90,    -1,
      92,    -1,    94,    95,   288,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,   288,    -1,    44,    -1,
      46,    83,    48,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   306,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    83,   102,    85,
     104,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   306,    -1,    -1,
      44,    -1,    46,    83,    48,    85,   288,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    83,   102,    85,   104,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    44,    -1,    46,    -1,    48,    -1,    32,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    44,    -1,
      46,    83,    48,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   288,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,   303,
      94,    95,   288,    97,    98,    99,   100,    83,   102,    85,
     104,    87,    88,    -1,    90,    -1,    92,   303,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      44,    -1,    46,    83,    48,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,   288,    97,    98,    99,
     100,    83,   102,    85,   104,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   288,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,   300,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,   288,    97,    98,    99,   100,    -1,   102,    -1,
     104,    83,    -1,    85,   300,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    37,    97,    98,    99,   100,    -1,
     102,    44,   104,    46,    83,    48,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,    -1,    87,    88,    -1,    90,   288,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    38,    -1,    40,    -1,   288,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,   300,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,   300,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    83,   102,    85,   104,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    40,    -1,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    83,    -1,    85,
      -1,    87,    88,    -1,    90,   297,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,   288,
      83,    -1,    85,    -1,    87,    88,    -1,    90,   297,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      41,   104,    43,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      83,   102,    85,   104,    87,    88,    44,    90,    46,    92,
      48,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,   288,    43,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    83,   294,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    83,   102,    85,   104,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    44,   104,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,
      -1,    44,   288,    46,    -1,    48,    -1,    -1,   294,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    -1,
      -1,    -1,    83,    -1,    85,   288,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      83,   102,    85,   104,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    64,    -1,    -1,    -1,   288,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,   288,    87,    88,   291,    90,
      66,    92,    68,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    44,    83,    46,    85,
      48,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
     288,    69,    70,   291,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    85,   288,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    77,    -1,    79,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    44,   102,    46,   104,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    83,   102,
      85,   104,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      44,    -1,    46,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    44,    -1,
      46,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    85,   288,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    51,    97,    98,    99,   100,    83,   102,    85,
     104,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    44,   102,    46,   104,    48,
     288,    -1,    51,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,   288,    51,
      -1,    -1,    -1,   236,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    51,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,   242,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,   288,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    74,   102,    -1,
     104,    -1,    -1,   288,    -1,    44,    83,    46,    85,    48,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,   288,   104,    -1,    -1,    82,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,   288,    97,    98,    99,   100,    -1,   102,    83,
     104,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,   288,
     104,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,   131,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,   288,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   288,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,   288,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   137,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,    -1,    -1,   288,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    83,   104,    85,   288,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,   288,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   143,    -1,    -1,    -1,    -1,    83,    -1,    85,   288,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,   146,   102,    83,   104,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    83,   149,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,   288,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,   152,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,   155,    -1,    44,   288,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,   288,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,   161,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,    83,    -1,
      85,   288,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,   288,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,   203,   102,    -1,   104,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,   288,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,   206,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   209,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,   288,
     102,    83,   104,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,   288,    -1,    44,    -1,    46,   215,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,   288,    94,    95,   224,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,   227,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,   230,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,   245,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,   248,
     102,    83,   104,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,   288,   104,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   288,
      97,    98,    99,   100,    -1,   102,    83,   104,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,   251,   102,    -1,   104,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     254,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,   288,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    83,   104,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,   288,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   257,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,   260,    -1,
      83,    -1,    85,    -1,    87,    88,   288,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,   221,
      -1,   288,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     287,   288,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,   236,    94,    95,    -1,    97,    98,
      99,   100,    83,   102,    85,   104,    87,    88,    44,    90,
      46,    92,    48,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    58,   104,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   239,    -1,    -1,    -1,   288,    83,    -1,    85,
      -1,    87,    88,    44,    90,    46,    92,    48,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,   242,
      61,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    83,    64,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    83,   104,    85,   288,    87,    88,    44,    90,
      46,    92,    48,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    44,    -1,    46,    -1,    48,   248,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      70,    97,    98,    99,   100,    44,   102,    46,   104,    48,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,   288,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    74,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,   288,    -1,    -1,    79,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    82,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,   288,    97,    98,
      99,   100,    -1,   102,    83,   104,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,   288,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    83,
     104,    85,   131,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,   288,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,    -1,    -1,    -1,    -1,    44,    -1,    46,   288,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   137,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   288,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   288,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,   143,    87,    88,    -1,    90,   288,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    83,    -1,    85,   288,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,   146,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   149,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,   288,    97,    98,    99,   100,    -1,   102,    44,   104,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   152,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
     155,    97,    98,    99,   100,    44,   102,    46,   104,    48,
      -1,    -1,    -1,    83,    -1,    85,   288,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,   158,   102,    44,   104,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      46,   161,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,   288,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,   164,    44,    83,    46,    85,
      48,    87,    88,   288,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,   167,    -1,    83,
      -1,    85,   288,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    44,   170,    46,    -1,    48,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    83,    -1,    85,   173,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    83,   104,    85,    -1,    87,    88,    -1,    90,
      -1,    92,   176,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    83,    -1,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,   179,    97,
      98,    99,   100,    -1,   102,    -1,   104,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,   182,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,   288,    46,    83,    48,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    83,   102,    85,   104,    87,    88,    -1,    90,
     188,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    44,   104,    46,    -1,    48,   288,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,   288,   102,    -1,
     104,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   191,    97,    98,    99,   100,    44,
     102,    46,   104,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,   194,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
     288,   102,    44,   104,    46,    -1,    48,    -1,    83,   200,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,   185,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,   288,   104,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,   203,    -1,    83,    -1,    85,    -1,    87,    88,   288,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,   288,    -1,    44,
      -1,    46,    83,    48,    85,   206,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,   288,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,   209,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,   288,   102,    -1,   104,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
     212,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,   215,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,   218,    85,    -1,
      87,    88,    -1,    90,    -1,    92,   288,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,   288,   224,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,   227,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,   288,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    44,    83,    46,    85,    48,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   288,
      97,    98,    99,   100,    -1,   102,    -1,   104,    44,    -1,
      46,    -1,    48,   230,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,   233,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    44,   102,    46,   104,    48,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,   288,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,   245,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,   248,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    44,   251,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,   288,    83,    -1,    85,   254,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,
      48,   257,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    83,    -1,    85,   260,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    44,   102,    46,   104,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,   266,    -1,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      83,   102,    85,   104,    87,    88,    44,    90,    46,    92,
      48,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,    -1,   269,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    83,    -1,    85,    -1,    87,
      88,   272,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,   288,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    44,   104,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,   275,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
     288,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    83,   102,    85,   104,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,   278,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   284,    -1,    -1,    -1,   288,   221,    -1,    83,    -1,
      85,    -1,    87,    88,    44,    90,    46,    92,    48,    94,
      95,    -1,    97,    98,    99,   100,   164,   102,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,   287,
     288,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,   288,   104,    44,    -1,    46,    -1,    48,
      -1,    -1,    83,   239,    85,    -1,    87,    88,    -1,    90,
      -1,    92,   167,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,   288,   102,    44,   104,    46,    -1,    48,    -1,
     170,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
     288,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,   173,    83,    -1,    85,    -1,    87,    88,    44,
      90,    46,    92,    48,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,   176,    46,    -1,
      48,    -1,    -1,   288,    -1,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
     179,    -1,    44,    -1,    46,    83,    48,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,   288,    97,
      98,    99,   100,    -1,   102,    -1,   104,    44,    -1,    46,
      -1,    48,   182,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,   288,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,   288,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,   212,    -1,   288,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    83,   288,    85,
     218,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    44,    -1,    46,    -1,    48,    -1,
      -1,   233,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
     288,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    83,    -1,    85,    -1,    87,    88,    44,
      90,    46,    92,    48,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,   288,    -1,    -1,   266,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      85,   288,    87,    88,    44,    90,    46,    92,    48,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    83,    -1,    85,    -1,    87,    88,   269,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    83,    -1,    85,   288,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,   272,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    44,    -1,    46,    -1,    48,    -1,   164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,    44,    83,    46,    85,    48,    87,    88,   248,    90,
      -1,    92,   275,    94,    95,    -1,    97,    98,    99,   100,
     170,   102,    83,   104,    85,   288,    87,    88,    44,    90,
      46,    92,    48,    94,    95,    -1,    97,    98,    99,   100,
      83,   102,    85,   104,    87,    88,    -1,    90,   288,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    -1,   173,   288,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,   288,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,   179,    85,   288,    87,
      88,    44,    90,    46,    92,    48,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,   182,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    44,   102,
      46,   104,    48,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    83,    -1,    85,
     188,    87,    88,    -1,    90,   288,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    44,   104,    46,
      -1,    48,    -1,    -1,    -1,    -1,   191,    -1,    -1,    83,
      -1,    85,   288,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,   194,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
     288,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,   197,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,   288,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,   200,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,   185,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,   288,    97,    98,    99,   100,    -1,   102,    44,
     104,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
     212,    48,    -1,    -1,   288,    -1,   263,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,   288,    97,    98,    99,   100,    83,   102,    85,   104,
      87,    88,    44,    90,    46,    92,    48,    94,    95,   288,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,   218,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,   233,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,   288,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,   288,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,    -1,    -1,
      -1,   266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   269,   288,    -1,    44,    -1,    46,    -1,    48,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,   288,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
     272,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,   288,    -1,    -1,    -1,
      -1,   275,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,   288,    94,    95,    -1,    97,    98,
      99,   100,   164,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,    -1,    -1,   281,    44,    -1,    46,    -1,    48,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,   288,
      92,   170,    94,    95,    -1,    97,    98,    99,   100,    44,
     102,    46,   104,    48,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,   288,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      44,    83,    46,    85,    48,    87,    88,    -1,    90,    -1,
      92,   173,    94,    95,    -1,    97,    98,    99,   100,   288,
     102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,   288,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    44,    83,    46,    85,    48,    87,    88,
      -1,    90,    -1,    92,   179,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     182,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    83,   288,    85,    -1,    87,
      88,    44,    90,    46,    92,    48,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,
      83,    -1,    85,    -1,    87,    88,    44,    90,    46,    92,
      48,    94,    95,   288,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    83,    -1,    85,    -1,    87,    88,   218,
      90,    -1,    92,    -1,    94,    95,   288,    97,    98,    99,
     100,    -1,   102,    -1,   104,    83,    -1,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    44,   102,    46,   104,    48,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   233,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    44,    83,    46,    85,    48,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,   266,    -1,
      83,    -1,    85,    -1,    87,    88,    44,    90,    46,    92,
      48,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
     288,   104,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    83,   269,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,   288,   104,    -1,    -1,    -1,
      -1,    -1,   272,    -1,    -1,    -1,   197,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   275,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   281,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     3,     4,    27,    28,   219,   220,   234,   235,   237,
     238,   240,   241,   313,   314,   320,   375,   321,    16,   386,
      29,   321,   386,   321,   386,   375,   386,   321,   386,   375,
     386,   321,   386,     0,    44,    46,    48,    83,    85,    87,
      88,    90,    92,    94,    95,    97,    98,    99,   100,   102,
     104,   288,   321,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,     6,     7,     9,    10,    12,    14,    15,
      18,    20,    21,    22,    24,    25,    30,    31,    33,    35,
      36,    38,    39,    41,    42,    49,    50,    52,    53,    54,
      55,    56,    57,    59,    60,    62,    63,    65,    66,    67,
      69,    71,    72,    73,    75,    77,    78,    80,    81,   105,
     107,   109,   111,   123,   124,   126,   127,   129,   130,   132,
     133,   135,   136,   138,   139,   141,   142,   144,   145,   147,
     148,   150,   151,   153,   154,   156,   157,   159,   160,   162,
     163,   165,   166,   168,   169,   171,   172,   174,   175,   177,
     178,   180,   181,   183,   184,   201,   202,   204,   205,   207,
     208,   210,   211,   213,   214,   216,   217,   222,   223,   225,
     226,   228,   229,   231,   232,   243,   244,   246,   247,   249,
     250,   252,   253,   255,   256,   258,   259,   264,   265,   267,
     268,   270,   271,   273,   274,   276,   277,   282,   283,   285,
     286,   289,   290,   292,   293,   295,   296,   298,   299,   301,
     302,   304,   305,   307,   308,   310,   311,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   387,   390,   393,   396,   397,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   117,    16,   316,   351,
     352,   316,   322,   316,   236,   316,   322,   316,   242,   316,
     322,   316,   116,    47,   116,   386,   116,   116,   116,   386,
     116,   116,   116,   116,   386,   386,   116,   116,   116,   386,
     386,   322,   321,   386,   321,   386,   321,   386,   321,   375,
     386,    23,   352,   386,    26,   321,   386,   375,   386,    34,
     375,   386,   375,   386,   375,   386,   375,   386,   375,   375,
     375,   375,   375,   375,   386,   375,   386,   375,   386,   375,
     386,   375,   386,   375,   386,   375,   375,   386,   375,   386,
     112,   388,   125,   386,   128,   386,   375,   386,   375,   386,
     375,   386,   375,   386,   375,   386,   375,   386,   375,   386,
     375,   386,   375,   386,   375,   386,   375,   386,   321,   386,
     321,   386,   321,   386,   321,   386,   321,   386,   321,   386,
     321,   386,   185,   321,   386,   375,   386,   375,   386,   375,
     386,   321,   386,   375,   386,   321,   386,   375,   386,   375,
     386,   375,   386,   321,   386,   375,   386,   375,   386,   375,
     386,   375,   386,   375,   386,   375,   386,   321,   386,   321,
     386,   321,   386,   321,   386,   278,   321,   386,   375,   386,
     375,   386,   375,   386,   375,   386,   375,   386,   321,   386,
     375,   386,   375,   386,   375,   386,   375,   386,     6,     7,
       9,    10,    12,    14,    15,   162,   163,   165,   166,   168,
     169,   171,   172,   174,   175,   177,   178,   180,   181,   210,
     211,   216,   217,   231,   232,   264,   265,   267,   268,   270,
     271,   273,   274,   298,   299,   323,   375,   387,    24,    25,
      30,    31,    33,   246,   247,    21,    22,   112,   113,   389,
     389,   116,   117,   321,   375,   321,   375,   321,   375,   375,
     321,   375,   375,   321,    45,    47,   316,    84,    86,    89,
     316,    91,    93,    96,    96,   316,   316,   101,   101,   103,
     316,   316,   322,   316,   322,   316,   322,   316,   322,    18,
      19,   316,   375,   316,   351,   316,    32,   316,    35,    37,
     316,    38,    40,   316,    41,    43,   316,    51,   316,    51,
      51,    51,    51,    51,    57,    58,   316,    60,    61,   316,
      63,    64,   316,    66,    68,   316,    69,    70,   316,    74,
     316,    76,    77,    79,   316,    82,   316,   317,   316,   316,
     316,   131,   316,   134,   316,   137,   316,   140,   316,   143,
     316,   146,   316,   149,   316,   152,   316,   155,   316,   158,
     316,   161,   316,   322,   316,   322,   316,   322,   316,   322,
     316,   322,   316,   322,   316,   322,   316,   186,   187,   189,
     190,   192,   193,   195,   196,   198,   199,   261,   262,   343,
     344,   345,   346,   347,   348,   349,   316,   203,   316,   206,
     316,   209,   316,   322,   316,   215,   316,   322,   316,   224,
     316,   227,   316,   230,   316,   322,   316,   245,   316,   248,
     316,   251,   316,   254,   316,   257,   316,   260,   316,   322,
     316,   322,   316,   322,   316,   322,   316,   279,   280,   341,
     342,   316,   284,   316,   287,   316,   291,   316,   294,   316,
     297,   316,   322,   316,   303,   316,   306,   316,   309,   316,
     312,   316,   321,   386,   321,   386,   321,   386,   321,   321,
     386,   321,   386,   321,   386,   321,   386,   321,   386,   321,
     386,   321,   386,   321,   386,   321,   386,   321,   386,   321,
     386,   321,   386,   321,   386,   321,   386,   321,   386,     5,
     321,   386,   375,   386,    34,   375,   386,   352,   386,   117,
     118,   119,   120,   121,   316,   318,   316,   116,   322,    29,
     351,   221,   322,   236,   239,   322,   242,   315,   322,   116,
     116,   116,    96,   116,   116,   116,   375,   321,   375,   321,
     375,   321,   375,   375,    23,   352,   375,   321,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   117,   106,   125,   128,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   321,   375,   321,
     375,   321,   375,   321,   375,   321,   375,   321,   375,   321,
     375,   386,   375,   386,   375,   386,   197,   321,   386,   375,
     386,   263,   321,   386,   344,   345,   346,   347,   348,   349,
     375,   321,   375,   375,   375,   375,   321,   375,   375,   321,
     375,   375,   375,   375,   321,   375,   375,   375,   375,   375,
     375,   375,   321,   375,   321,   375,   321,   375,   321,   281,
     321,   386,   342,   375,   321,   375,   375,   375,   375,   375,
     375,   321,   375,   375,   375,   375,   322,   316,   322,   316,
     322,   316,   322,   322,   316,   322,   316,   322,   316,   322,
     316,   322,   316,   322,   316,   322,   316,   322,   316,   322,
     316,   322,   316,   322,   316,   322,   316,   322,   316,   322,
     316,   322,   316,   351,   316,    32,   316,   248,   316,   375,
     316,   389,   389,   389,   389,   389,   391,   117,   395,   375,
     375,   375,   375,   375,    47,    89,    96,    96,   103,   101,
       8,   322,    11,   322,    13,   322,    17,    19,   375,    26,
     351,    32,    37,    40,    43,    51,    58,    61,    64,    68,
      70,    74,    79,    82,   389,   131,   134,   137,   140,   143,
     146,   149,   152,   155,   158,   161,   164,   322,   167,   322,
     170,   322,   173,   322,   176,   322,   179,   322,   182,   322,
     188,   316,   191,   316,   194,   316,   351,   316,   200,   316,
     322,   316,   185,   343,   203,   206,   209,   212,   322,   215,
     218,   322,   224,   227,   230,   233,   322,   245,   248,   251,
     254,   257,   260,   266,   322,   269,   322,   272,   322,   275,
     322,   343,   316,   278,   341,   284,   287,   291,   294,   297,
     300,   322,   303,   306,   309,   312,   375,   321,   375,   321,
     375,   321,   375,   375,   321,   375,   321,   375,   321,   375,
     321,   375,   321,   375,   321,   375,   321,   375,   321,   375,
     321,   375,   321,   375,   321,   375,   321,   375,   321,   375,
     321,   375,   321,   375,   321,   375,   375,    23,   352,   322,
     389,   322,     5,    29,   221,   239,   315,   375,   375,   375,
      23,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   321,   375,   375,   321,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   321,   375,   375,     8,
     322,    11,   322,    13,   322,    17,   164,   322,   167,   322,
     170,   322,   173,   322,   176,   322,   179,   322,   182,   322,
     212,   322,   218,   322,   233,   322,   266,   322,   269,   322,
     272,   322,   275,   322,   300,   322,    26,   351,    32,   248,
     375,   108,   392,   110,   394,     8,    11,    13,    26,   164,
     167,   170,   173,   176,   179,   182,   188,   191,   194,   197,
     351,   200,   263,   322,   185,   212,   218,   233,   266,   269,
     272,   275,   281,   343,   278,   300,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,    23,   375,   375,   375,     8,
      11,    13,   164,   167,   170,   173,   176,   179,   182,   212,
     218,   233,   266,   269,   272,   275,   300,    26,   197,   263,
     281
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   319,   320,   320,   320,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   320,   321,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     324,   324,   324,   325,   325,   325,   326,   326,   326,   327,
     327,   327,   328,   328,   328,   329,   329,   329,   329,   329,
     329,   329,   330,   330,   330,   331,   331,   331,   332,   332,
     332,   333,   333,   333,   334,   334,   334,   335,   335,   336,
     337,   337,   338,   338,   339,   339,   339,   340,   340,   340,
     341,   341,   342,   342,   342,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   344,   344,   344,
     345,   345,   346,   346,   347,   347,   348,   348,   348,   349,
     349,   350,   350,   350,   350,   350,   351,   351,   351,   351,
     352,   352,   352,   352,   352,   353,   353,   353,   353,   354,
     354,   355,   355,   355,   355,   356,   356,   357,   357,   358,
     358,   359,   359,   360,   360,   361,   361,   362,   362,   363,
     363,   364,   364,   365,   365,   366,   366,   367,   367,   368,
     368,   369,   369,   370,   370,   371,   371,   372,   372,   373,
     373,   374,   374,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   376,   377,   378,   379,   380,   381,
     381,   381,   381,   381,   382,   382,   382,   383,   383,   384,
     384,   385,   385,   385,   386,   386,   387,   387,   387,   387,
     388,   389,   389,   389,   389,   389,   389,   389,   389,   390,
     391,   392,   393,   394,   395,   396,   396,   397,   397,   398,
     398,   399,   399,   400,   400,   401,   401,   402,   402,   403,
     403,   404,   404,   405,   405,   406,   406,   407,   407
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     5,     7,     5,     7,     3,     5,     5,
       7,     3,     5,     2,     5,     7,     5,     7,     0,     1,
       2,     1,     2,     5,     6,     7,     8,     5,     6,     7,
       8,     5,     6,     7,     8,     5,     6,     7,     8,     5,
       6,     7,     8,     5,     6,     7,     8,     5,     6,     7,
       8,     5,     6,     7,     8,     5,     6,     7,     8,     5,
       6,     7,     8,     5,     6,     7,     8,     5,     6,     7,
       8,     5,     6,     7,     8,     5,     6,     7,     8,     5,
       6,     7,     8,     5,     6,     7,     8,     5,     6,     7,
       8,     5,     6,     7,     8,     5,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     1,     3,     5,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     5,     3,     3,     5,     3,     3,     5,     3,     3,
       5,     3,     3,     5,     3,     3,     5,     3,     3,     3,
       3,     3,     3,     5,     3,     3,     5,     3,     3,     5,
       3,     3,     5,     3,     3,     5,     3,     3,     5,     3,
       3,     5,     3,     5,     2,     5,     7,     2,     5,     7,
       1,     2,     2,     5,     7,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     2,     5,     7,
       3,     5,     3,     5,     3,     5,     2,     5,     7,     3,
       5,     2,     4,     5,     6,     7,     1,     1,     1,     1,
       2,     5,     6,     7,     8,     3,     4,     5,     6,     2,
       3,     3,     4,     5,     6,     2,     4,     2,     4,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     0,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       5,     3,     5,     4,     3,     3,     5,     3,     5,     3,
       5,     3,     2,     5,     3,     4,     4,     1,     6,     6,
       4,     1,     1,     3,     3,     3,     3,     3,     4,     1,
       0,     1,     1,     1,     0,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* window: attr radio_group_scope wlist  */
#line 251 "gtkdialog_parser.y"
                                       {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_WINDOW);
		start_up();
	}
#line 4424 "gtkdialog_parser.c"
    break;

  case 3: /* window: WINDOW radio_group_scope wlist attr EWINDOW  */
#line 256 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_WINDOW);
		start_up();
	}
#line 4434 "gtkdialog_parser.c"
    break;

  case 4: /* window: PART_WINDOW tagattr '>' radio_group_scope wlist attr EWINDOW  */
#line 261 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_WINDOW, (yyvsp[-5].nvval));
		start_up();
	}
#line 4444 "gtkdialog_parser.c"
    break;

  case 5: /* window: ASSISTANT radio_group_scope wlist attr EASSISTANT  */
#line 266 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ASSISTANT);
		start_up();
	}
#line 4454 "gtkdialog_parser.c"
    break;

  case 6: /* window: PART_ASSISTANT tagattr '>' radio_group_scope wlist attr EASSISTANT  */
#line 271 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ASSISTANT, (yyvsp[-5].nvval));
		start_up();
	}
#line 4464 "gtkdialog_parser.c"
    break;

  case 7: /* window: MESSAGEDIALOG attr EMESSAGEDIALOG  */
#line 276 "gtkdialog_parser.y"
                                            {
		token_store(PUSH | WIDGET_MESSAGEDIALOG);
		start_up();
	}
#line 4473 "gtkdialog_parser.c"
    break;

  case 8: /* window: PART_MESSAGEDIALOG tagattr '>' attr EMESSAGEDIALOG  */
#line 280 "gtkdialog_parser.y"
                                                             {
		token_store_attr(PUSH | WIDGET_MESSAGEDIALOG, (yyvsp[-3].nvval));
		start_up();
	}
#line 4482 "gtkdialog_parser.c"
    break;

  case 9: /* window: DIALOG radio_group_scope wlist attr EDIALOG  */
#line 284 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_DIALOG);
		start_up();
	}
#line 4492 "gtkdialog_parser.c"
    break;

  case 10: /* window: PART_DIALOG tagattr '>' radio_group_scope wlist attr EDIALOG  */
#line 289 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_DIALOG, (yyvsp[-5].nvval));
		start_up();
	}
#line 4502 "gtkdialog_parser.c"
    break;

  case 11: /* window: ABOUTDIALOG attr EABOUTDIALOG  */
#line 294 "gtkdialog_parser.y"
                                        {
		token_store(PUSH | WIDGET_ABOUTDIALOG);
		start_up();
	}
#line 4511 "gtkdialog_parser.c"
    break;

  case 12: /* window: PART_ABOUTDIALOG tagattr '>' attr EABOUTDIALOG  */
#line 298 "gtkdialog_parser.y"
                                                         {
		token_store_attr(PUSH | WIDGET_ABOUTDIALOG, (yyvsp[-3].nvval));
		start_up();
	}
#line 4520 "gtkdialog_parser.c"
    break;

  case 13: /* window: POPUPMENU EPOPUPMENU  */
#line 302 "gtkdialog_parser.y"
                               {
		yyerror("The popupmenu widget requires at least one menuitem widget.");
	}
#line 4528 "gtkdialog_parser.c"
    break;

  case 14: /* window: POPUPMENU radio_group_scope menuwlist attr EPOPUPMENU  */
#line 305 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_POPUPMENU);
		start_up();
	}
#line 4538 "gtkdialog_parser.c"
    break;

  case 15: /* window: PART_POPUPMENU tagattr '>' radio_group_scope menuwlist attr EPOPUPMENU  */
#line 310 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_POPUPMENU, (yyvsp[-5].nvval));
		start_up();
	}
#line 4548 "gtkdialog_parser.c"
    break;

  case 16: /* window: PLUG radio_group_scope wlist attr EPLUG  */
#line 315 "gtkdialog_parser.y"
                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_PLUG);
		start_up();
	}
#line 4558 "gtkdialog_parser.c"
    break;

  case 17: /* window: PART_PLUG tagattr '>' radio_group_scope wlist attr EPLUG  */
#line 320 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_PLUG, (yyvsp[-5].nvval));
		start_up();
	}
#line 4568 "gtkdialog_parser.c"
    break;

  case 18: /* radio_group_scope: %empty  */
#line 328 "gtkdialog_parser.y"
    { token_store(RGROUP_PUSH); }
#line 4574 "gtkdialog_parser.c"
    break;

  case 20: /* wlist: wlist widget  */
#line 335 "gtkdialog_parser.y"
                       {
		token_store(SUM);
	}
#line 4582 "gtkdialog_parser.c"
    break;

  case 23: /* wlist: ALIGNMENT radio_group_scope wlist attr EALIGNMENT  */
#line 340 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ALIGNMENT);
	}
#line 4591 "gtkdialog_parser.c"
    break;

  case 24: /* wlist: wlist ALIGNMENT radio_group_scope wlist attr EALIGNMENT  */
#line 344 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ALIGNMENT);
		token_store(SUM);
	}
#line 4601 "gtkdialog_parser.c"
    break;

  case 25: /* wlist: PART_ALIGNMENT tagattr '>' radio_group_scope wlist attr EALIGNMENT  */
#line 349 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ALIGNMENT, (yyvsp[-5].nvval));
	}
#line 4610 "gtkdialog_parser.c"
    break;

  case 26: /* wlist: wlist PART_ALIGNMENT tagattr '>' radio_group_scope wlist attr EALIGNMENT  */
#line 353 "gtkdialog_parser.y"
                                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ALIGNMENT, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4620 "gtkdialog_parser.c"
    break;

  case 27: /* wlist: ASPECTFRAME radio_group_scope wlist attr EASPECTFRAME  */
#line 358 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ASPECTFRAME);
	}
#line 4629 "gtkdialog_parser.c"
    break;

  case 28: /* wlist: wlist ASPECTFRAME radio_group_scope wlist attr EASPECTFRAME  */
#line 362 "gtkdialog_parser.y"
                                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ASPECTFRAME);
		token_store(SUM);
	}
#line 4639 "gtkdialog_parser.c"
    break;

  case 29: /* wlist: PART_ASPECTFRAME tagattr '>' radio_group_scope wlist attr EASPECTFRAME  */
#line 367 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ASPECTFRAME, (yyvsp[-5].nvval));
	}
#line 4648 "gtkdialog_parser.c"
    break;

  case 30: /* wlist: wlist PART_ASPECTFRAME tagattr '>' radio_group_scope wlist attr EASPECTFRAME  */
#line 371 "gtkdialog_parser.y"
                                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ASPECTFRAME, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4658 "gtkdialog_parser.c"
    break;

  case 31: /* wlist: GRID radio_group_scope wlist attr EGRID  */
#line 376 "gtkdialog_parser.y"
                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_GRID);
	}
#line 4667 "gtkdialog_parser.c"
    break;

  case 32: /* wlist: wlist GRID radio_group_scope wlist attr EGRID  */
#line 380 "gtkdialog_parser.y"
                                                        {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_GRID);
		token_store(SUM);
	}
#line 4677 "gtkdialog_parser.c"
    break;

  case 33: /* wlist: PART_GRID tagattr '>' radio_group_scope wlist attr EGRID  */
#line 385 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_GRID, (yyvsp[-5].nvval));
	}
#line 4686 "gtkdialog_parser.c"
    break;

  case 34: /* wlist: wlist PART_GRID tagattr '>' radio_group_scope wlist attr EGRID  */
#line 389 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_GRID, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4696 "gtkdialog_parser.c"
    break;

  case 35: /* wlist: FIXED radio_group_scope wlist attr EFIXED  */
#line 394 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_FIXED);
	}
#line 4705 "gtkdialog_parser.c"
    break;

  case 36: /* wlist: wlist FIXED radio_group_scope wlist attr EFIXED  */
#line 398 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_FIXED);
		token_store(SUM);
	}
#line 4715 "gtkdialog_parser.c"
    break;

  case 37: /* wlist: PART_FIXED tagattr '>' radio_group_scope wlist attr EFIXED  */
#line 403 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_FIXED, (yyvsp[-5].nvval));
	}
#line 4724 "gtkdialog_parser.c"
    break;

  case 38: /* wlist: wlist PART_FIXED tagattr '>' radio_group_scope wlist attr EFIXED  */
#line 407 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_FIXED, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4734 "gtkdialog_parser.c"
    break;

  case 39: /* wlist: LAYOUT radio_group_scope wlist attr ELAYOUT  */
#line 412 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_LAYOUT);
	}
#line 4743 "gtkdialog_parser.c"
    break;

  case 40: /* wlist: wlist LAYOUT radio_group_scope wlist attr ELAYOUT  */
#line 416 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_LAYOUT);
		token_store(SUM);
	}
#line 4753 "gtkdialog_parser.c"
    break;

  case 41: /* wlist: PART_LAYOUT tagattr '>' radio_group_scope wlist attr ELAYOUT  */
#line 421 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_LAYOUT, (yyvsp[-5].nvval));
	}
#line 4762 "gtkdialog_parser.c"
    break;

  case 42: /* wlist: wlist PART_LAYOUT tagattr '>' radio_group_scope wlist attr ELAYOUT  */
#line 425 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_LAYOUT, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4772 "gtkdialog_parser.c"
    break;

  case 43: /* wlist: SCROLLEDWINDOW radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 430 "gtkdialog_parser.y"
                                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_SCROLLEDWINDOW);
	}
#line 4781 "gtkdialog_parser.c"
    break;

  case 44: /* wlist: wlist SCROLLEDWINDOW radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 434 "gtkdialog_parser.y"
                                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_SCROLLEDWINDOW);
		token_store(SUM);
	}
#line 4791 "gtkdialog_parser.c"
    break;

  case 45: /* wlist: PART_SCROLLEDWINDOW tagattr '>' radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 439 "gtkdialog_parser.y"
                                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_SCROLLEDWINDOW, (yyvsp[-5].nvval));
	}
#line 4800 "gtkdialog_parser.c"
    break;

  case 46: /* wlist: wlist PART_SCROLLEDWINDOW tagattr '>' radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 443 "gtkdialog_parser.y"
                                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_SCROLLEDWINDOW, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4810 "gtkdialog_parser.c"
    break;

  case 47: /* wlist: VIEWPORT radio_group_scope wlist attr EVIEWPORT  */
#line 448 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VIEWPORT);
	}
#line 4819 "gtkdialog_parser.c"
    break;

  case 48: /* wlist: wlist VIEWPORT radio_group_scope wlist attr EVIEWPORT  */
#line 452 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VIEWPORT);
		token_store(SUM);
	}
#line 4829 "gtkdialog_parser.c"
    break;

  case 49: /* wlist: PART_VIEWPORT tagattr '>' radio_group_scope wlist attr EVIEWPORT  */
#line 457 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VIEWPORT, (yyvsp[-5].nvval));
	}
#line 4838 "gtkdialog_parser.c"
    break;

  case 50: /* wlist: wlist PART_VIEWPORT tagattr '>' radio_group_scope wlist attr EVIEWPORT  */
#line 461 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VIEWPORT, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4848 "gtkdialog_parser.c"
    break;

  case 51: /* wlist: HANDLEBOX radio_group_scope wlist attr EHANDLEBOX  */
#line 466 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HANDLEBOX);
	}
#line 4857 "gtkdialog_parser.c"
    break;

  case 52: /* wlist: wlist HANDLEBOX radio_group_scope wlist attr EHANDLEBOX  */
#line 470 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HANDLEBOX);
		token_store(SUM);
	}
#line 4867 "gtkdialog_parser.c"
    break;

  case 53: /* wlist: PART_HANDLEBOX tagattr '>' radio_group_scope wlist attr EHANDLEBOX  */
#line 475 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HANDLEBOX, (yyvsp[-5].nvval));
	}
#line 4876 "gtkdialog_parser.c"
    break;

  case 54: /* wlist: wlist PART_HANDLEBOX tagattr '>' radio_group_scope wlist attr EHANDLEBOX  */
#line 479 "gtkdialog_parser.y"
                                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HANDLEBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4886 "gtkdialog_parser.c"
    break;

  case 55: /* wlist: VBOX radio_group_scope wlist attr EVBOX  */
#line 484 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBOX);
	}
#line 4895 "gtkdialog_parser.c"
    break;

  case 56: /* wlist: wlist VBOX radio_group_scope wlist attr EVBOX  */
#line 488 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBOX);
		token_store(SUM);
	}
#line 4905 "gtkdialog_parser.c"
    break;

  case 57: /* wlist: PART_VBOX tagattr '>' radio_group_scope wlist attr EVBOX  */
#line 493 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBOX, (yyvsp[-5].nvval));
	}
#line 4914 "gtkdialog_parser.c"
    break;

  case 58: /* wlist: wlist PART_VBOX tagattr '>' radio_group_scope wlist attr EVBOX  */
#line 497 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4924 "gtkdialog_parser.c"
    break;

  case 59: /* wlist: HBOX radio_group_scope wlist attr EHBOX  */
#line 502 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBOX);
	}
#line 4933 "gtkdialog_parser.c"
    break;

  case 60: /* wlist: wlist HBOX radio_group_scope wlist attr EHBOX  */
#line 506 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBOX);
		token_store(SUM);
	}
#line 4943 "gtkdialog_parser.c"
    break;

  case 61: /* wlist: PART_HBOX tagattr '>' radio_group_scope wlist attr EHBOX  */
#line 511 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBOX, (yyvsp[-5].nvval));
	}
#line 4952 "gtkdialog_parser.c"
    break;

  case 62: /* wlist: wlist PART_HBOX tagattr '>' radio_group_scope wlist attr EHBOX  */
#line 515 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4962 "gtkdialog_parser.c"
    break;

  case 63: /* wlist: HBUTTONBOX radio_group_scope wlist attr EHBUTTONBOX  */
#line 520 "gtkdialog_parser.y"
                                                              {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBUTTONBOX);
	}
#line 4971 "gtkdialog_parser.c"
    break;

  case 64: /* wlist: wlist HBUTTONBOX radio_group_scope wlist attr EHBUTTONBOX  */
#line 524 "gtkdialog_parser.y"
                                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBUTTONBOX);
		token_store(SUM);
	}
#line 4981 "gtkdialog_parser.c"
    break;

  case 65: /* wlist: PART_HBUTTONBOX tagattr '>' radio_group_scope wlist attr EHBUTTONBOX  */
#line 529 "gtkdialog_parser.y"
                                                                               {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBUTTONBOX, (yyvsp[-5].nvval));
	}
#line 4990 "gtkdialog_parser.c"
    break;

  case 66: /* wlist: wlist PART_HBUTTONBOX tagattr '>' radio_group_scope wlist attr EHBUTTONBOX  */
#line 533 "gtkdialog_parser.y"
                                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBUTTONBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5000 "gtkdialog_parser.c"
    break;

  case 67: /* wlist: VBUTTONBOX radio_group_scope wlist attr EVBUTTONBOX  */
#line 538 "gtkdialog_parser.y"
                                                              {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBUTTONBOX);
	}
#line 5009 "gtkdialog_parser.c"
    break;

  case 68: /* wlist: wlist VBUTTONBOX radio_group_scope wlist attr EVBUTTONBOX  */
#line 542 "gtkdialog_parser.y"
                                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBUTTONBOX);
		token_store(SUM);
	}
#line 5019 "gtkdialog_parser.c"
    break;

  case 69: /* wlist: PART_VBUTTONBOX tagattr '>' radio_group_scope wlist attr EVBUTTONBOX  */
#line 547 "gtkdialog_parser.y"
                                                                               {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBUTTONBOX, (yyvsp[-5].nvval));
	}
#line 5028 "gtkdialog_parser.c"
    break;

  case 70: /* wlist: wlist PART_VBUTTONBOX tagattr '>' radio_group_scope wlist attr EVBUTTONBOX  */
#line 551 "gtkdialog_parser.y"
                                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBUTTONBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5038 "gtkdialog_parser.c"
    break;

  case 71: /* wlist: HPANED radio_group_scope wlist attr EHPANED  */
#line 556 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HPANED);
	}
#line 5047 "gtkdialog_parser.c"
    break;

  case 72: /* wlist: wlist HPANED radio_group_scope wlist attr EHPANED  */
#line 560 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HPANED);
		token_store(SUM);
	}
#line 5057 "gtkdialog_parser.c"
    break;

  case 73: /* wlist: PART_HPANED tagattr '>' radio_group_scope wlist attr EHPANED  */
#line 565 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HPANED, (yyvsp[-5].nvval));
	}
#line 5066 "gtkdialog_parser.c"
    break;

  case 74: /* wlist: wlist PART_HPANED tagattr '>' radio_group_scope wlist attr EHPANED  */
#line 569 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HPANED, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5076 "gtkdialog_parser.c"
    break;

  case 75: /* wlist: VPANED radio_group_scope wlist attr EVPANED  */
#line 574 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VPANED);
	}
#line 5085 "gtkdialog_parser.c"
    break;

  case 76: /* wlist: wlist VPANED radio_group_scope wlist attr EVPANED  */
#line 578 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VPANED);
		token_store(SUM);
	}
#line 5095 "gtkdialog_parser.c"
    break;

  case 77: /* wlist: PART_VPANED tagattr '>' radio_group_scope wlist attr EVPANED  */
#line 583 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VPANED, (yyvsp[-5].nvval));
	}
#line 5104 "gtkdialog_parser.c"
    break;

  case 78: /* wlist: wlist PART_VPANED tagattr '>' radio_group_scope wlist attr EVPANED  */
#line 587 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VPANED, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5114 "gtkdialog_parser.c"
    break;

  case 79: /* wlist: EVENTBOX radio_group_scope wlist attr EEVENTBOX  */
#line 592 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EVENTBOX);
	}
#line 5123 "gtkdialog_parser.c"
    break;

  case 80: /* wlist: wlist EVENTBOX radio_group_scope wlist attr EEVENTBOX  */
#line 596 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EVENTBOX);
		token_store(SUM);
	}
#line 5133 "gtkdialog_parser.c"
    break;

  case 81: /* wlist: PART_EVENTBOX tagattr '>' radio_group_scope wlist attr EEVENTBOX  */
#line 601 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EVENTBOX, (yyvsp[-5].nvval));
	}
#line 5142 "gtkdialog_parser.c"
    break;

  case 82: /* wlist: wlist PART_EVENTBOX tagattr '>' radio_group_scope wlist attr EEVENTBOX  */
#line 605 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EVENTBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5152 "gtkdialog_parser.c"
    break;

  case 83: /* wlist: EXPANDER radio_group_scope wlist attr EEXPANDER  */
#line 610 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EXPANDER);
	}
#line 5161 "gtkdialog_parser.c"
    break;

  case 84: /* wlist: wlist EXPANDER radio_group_scope wlist attr EEXPANDER  */
#line 614 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EXPANDER);
		token_store(SUM);
	}
#line 5171 "gtkdialog_parser.c"
    break;

  case 85: /* wlist: PART_EXPANDER tagattr '>' radio_group_scope wlist attr EEXPANDER  */
#line 619 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EXPANDER, (yyvsp[-5].nvval));
	}
#line 5180 "gtkdialog_parser.c"
    break;

  case 86: /* wlist: wlist PART_EXPANDER tagattr '>' radio_group_scope wlist attr EEXPANDER  */
#line 623 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EXPANDER, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5190 "gtkdialog_parser.c"
    break;

  case 87: /* wlist: INFOBAR radio_group_scope wlist attr EINFOBAR  */
#line 628 "gtkdialog_parser.y"
                                                        {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_INFOBAR);
	}
#line 5199 "gtkdialog_parser.c"
    break;

  case 88: /* wlist: wlist INFOBAR radio_group_scope wlist attr EINFOBAR  */
#line 632 "gtkdialog_parser.y"
                                                              {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_INFOBAR);
		token_store(SUM);
	}
#line 5209 "gtkdialog_parser.c"
    break;

  case 89: /* wlist: PART_INFOBAR tagattr '>' radio_group_scope wlist attr EINFOBAR  */
#line 637 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_INFOBAR, (yyvsp[-5].nvval));
	}
#line 5218 "gtkdialog_parser.c"
    break;

  case 90: /* wlist: wlist PART_INFOBAR tagattr '>' radio_group_scope wlist attr EINFOBAR  */
#line 641 "gtkdialog_parser.y"
                                                                               {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_INFOBAR, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5228 "gtkdialog_parser.c"
    break;

  case 91: /* wlist: NOTEBOOK radio_group_scope wlist attr ENOTEBOOK  */
#line 646 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_NOTEBOOK);
	}
#line 5237 "gtkdialog_parser.c"
    break;

  case 92: /* wlist: wlist NOTEBOOK radio_group_scope wlist attr ENOTEBOOK  */
#line 650 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_NOTEBOOK);
		token_store(SUM);
	}
#line 5247 "gtkdialog_parser.c"
    break;

  case 93: /* wlist: PART_NOTEBOOK tagattr '>' radio_group_scope wlist attr ENOTEBOOK  */
#line 655 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_NOTEBOOK, (yyvsp[-5].nvval));
	}
#line 5256 "gtkdialog_parser.c"
    break;

  case 94: /* wlist: wlist PART_NOTEBOOK tagattr '>' radio_group_scope wlist attr ENOTEBOOK  */
#line 659 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_NOTEBOOK, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5266 "gtkdialog_parser.c"
    break;

  case 95: /* wlist: FRAME radio_group_scope wlist attr EFRAME  */
#line 664 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store_owned_argument(SET|ATTR_LABEL, (yyvsp[-4].cval));
		token_store(PUSH | WIDGET_FRAME);
	}
#line 5276 "gtkdialog_parser.c"
    break;

  case 96: /* wlist: wlist FRAME radio_group_scope wlist attr EFRAME  */
#line 669 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store_owned_argument(SET|ATTR_LABEL, (yyvsp[-4].cval));
		token_store(PUSH | WIDGET_FRAME);
		token_store(SUM);
	}
#line 5287 "gtkdialog_parser.c"
    break;

  case 124: /* widget: COLORSELECTION attr ECOLORSELECTION  */
#line 705 "gtkdialog_parser.y"
                                        {
		token_store(PUSH | WIDGET_COLORSELECTION);
	}
#line 5295 "gtkdialog_parser.c"
    break;

  case 125: /* widget: PART_COLORSELECTION tagattr '>' attr ECOLORSELECTION  */
#line 708 "gtkdialog_parser.y"
                                                         {
		token_store_attr(PUSH | WIDGET_COLORSELECTION, (yyvsp[-3].nvval));
	}
#line 5303 "gtkdialog_parser.c"
    break;

  case 127: /* widget: FONTSELECTION attr EFONTSELECTION  */
#line 712 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_FONTSELECTION);
	}
#line 5311 "gtkdialog_parser.c"
    break;

  case 128: /* widget: PART_FONTSELECTION tagattr '>' attr EFONTSELECTION  */
#line 715 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_FONTSELECTION, (yyvsp[-3].nvval));
	}
#line 5319 "gtkdialog_parser.c"
    break;

  case 150: /* entry: ENTRY attr EENTRY  */
#line 742 "gtkdialog_parser.y"
                      {
                          token_store(PUSH | WIDGET_ENTRY);
			 }
#line 5327 "gtkdialog_parser.c"
    break;

  case 151: /* entry: PART_ENTRY tagattr '>' attr EENTRY  */
#line 745 "gtkdialog_parser.y"
                                       {
                token_store_attr(PUSH | WIDGET_ENTRY, (yyvsp[-3].nvval));
	}
#line 5335 "gtkdialog_parser.c"
    break;

  case 152: /* entry: ENTRY attr ENTRY  */
#line 748 "gtkdialog_parser.y"
                     {
                  yyerror("</entry> expected instead of <entry>.");}
#line 5342 "gtkdialog_parser.c"
    break;

  case 153: /* edit: EDIT attr EEDIT  */
#line 753 "gtkdialog_parser.y"
                     {
		token_store(PUSH | WIDGET_EDIT);
	}
#line 5350 "gtkdialog_parser.c"
    break;

  case 154: /* edit: PART_EDIT tagattr '>' attr EEDIT  */
#line 756 "gtkdialog_parser.y"
                                     {
		token_store_attr(PUSH | WIDGET_EDIT, (yyvsp[-3].nvval));
	}
#line 5358 "gtkdialog_parser.c"
    break;

  case 155: /* edit: EDIT attr EDIT  */
#line 759 "gtkdialog_parser.y"
                     {
		yyerror("</edit> expected instead of <edit>.");
	}
#line 5366 "gtkdialog_parser.c"
    break;

  case 156: /* tree: TREE attr ETREE  */
#line 765 "gtkdialog_parser.y"
                     {
		token_store(PUSH | WIDGET_TREE);
	}
#line 5374 "gtkdialog_parser.c"
    break;

  case 157: /* tree: PART_TREE tagattr '>' attr ETREE  */
#line 768 "gtkdialog_parser.y"
                                     {
		token_store_attr(PUSH | WIDGET_TREE, (yyvsp[-3].nvval));
	}
#line 5382 "gtkdialog_parser.c"
    break;

  case 158: /* tree: TREE attr TREE  */
#line 771 "gtkdialog_parser.y"
                   {
		yyerror("</tree> expected instead of <tree>.");
	}
#line 5390 "gtkdialog_parser.c"
    break;

  case 159: /* chooser: CHOOSER attr ECHOOSER  */
#line 777 "gtkdialog_parser.y"
                           {
		token_store(PUSH | WIDGET_CHOOSER);
	}
#line 5398 "gtkdialog_parser.c"
    break;

  case 160: /* chooser: PART_CHOOSER tagattr '>' attr ECHOOSER  */
#line 780 "gtkdialog_parser.y"
                                           {
		token_store_attr(PUSH | WIDGET_CHOOSER, (yyvsp[-3].nvval));
	}
#line 5406 "gtkdialog_parser.c"
    break;

  case 161: /* chooser: CHOOSER attr CHOOSER  */
#line 783 "gtkdialog_parser.y"
                         {
		yyerror("</chooser> expected instead of <chooser>.");
	}
#line 5414 "gtkdialog_parser.c"
    break;

  case 162: /* text: TEXT attr ETEXT  */
#line 789 "gtkdialog_parser.y"
                    {
		token_store(PUSH | WIDGET_TEXT);
	}
#line 5422 "gtkdialog_parser.c"
    break;

  case 163: /* text: PART_TEXT tagattr '>' attr ETEXT  */
#line 792 "gtkdialog_parser.y"
                                     {
                token_store_attr(PUSH | WIDGET_TEXT, (yyvsp[-3].nvval));
	}
#line 5430 "gtkdialog_parser.c"
    break;

  case 164: /* text: TEXT attr TEXT  */
#line 795 "gtkdialog_parser.y"
                    {yyerror("</text> expected instead of <text>.");}
#line 5436 "gtkdialog_parser.c"
    break;

  case 165: /* button: BUTTON attr EBUTTON  */
#line 799 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_BUTTON);  }
#line 5442 "gtkdialog_parser.c"
    break;

  case 166: /* button: PART_BUTTON tagattr '>' attr EBUTTON  */
#line 800 "gtkdialog_parser.y"
                                         {
                token_store_attr(PUSH | WIDGET_BUTTON, (yyvsp[-3].nvval));
	}
#line 5450 "gtkdialog_parser.c"
    break;

  case 167: /* button: BUTTONOK attr EBUTTON  */
#line 803 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_OKBUTTON);}
#line 5456 "gtkdialog_parser.c"
    break;

  case 168: /* button: BUTTONCANCEL attr EBUTTON  */
#line 804 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_CANCELBUTTON);}
#line 5462 "gtkdialog_parser.c"
    break;

  case 169: /* button: BUTTONHELP attr EBUTTON  */
#line 805 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_HELPBUTTON);}
#line 5468 "gtkdialog_parser.c"
    break;

  case 170: /* button: BUTTONNO attr EBUTTON  */
#line 806 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_NOBUTTON);}
#line 5474 "gtkdialog_parser.c"
    break;

  case 171: /* button: BUTTONYES attr EBUTTON  */
#line 807 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_YESBUTTON);}
#line 5480 "gtkdialog_parser.c"
    break;

  case 172: /* checkbox: CHECKBOX attr ECHECKBOX  */
#line 811 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_CHECKBOX);
	}
#line 5488 "gtkdialog_parser.c"
    break;

  case 173: /* checkbox: PART_CHECKBOX tagattr '>' attr ECHECKBOX  */
#line 814 "gtkdialog_parser.y"
                                             {
                token_store_attr(PUSH | WIDGET_CHECKBOX, (yyvsp[-3].nvval));
	}
#line 5496 "gtkdialog_parser.c"
    break;

  case 174: /* checkbox: CHECKBOX attr CHECKBOX  */
#line 817 "gtkdialog_parser.y"
                            {
		yyerror("</checkbox> expected instead of <checkbox>.");
	}
#line 5504 "gtkdialog_parser.c"
    break;

  case 175: /* radiobutton: RADIO attr ERADIO  */
#line 823 "gtkdialog_parser.y"
                         {
		token_store(PUSH | WIDGET_RADIOBUTTON);
           }
#line 5512 "gtkdialog_parser.c"
    break;

  case 176: /* radiobutton: PART_RADIO tagattr '>' attr ERADIO  */
#line 826 "gtkdialog_parser.y"
                                       {
                token_store_attr(PUSH | WIDGET_RADIOBUTTON, (yyvsp[-3].nvval));
	   }
#line 5520 "gtkdialog_parser.c"
    break;

  case 177: /* radiobutton: RADIO attr RADIO  */
#line 829 "gtkdialog_parser.y"
                      {
		yyerror("</radiobutton> expected instead of <radiobutton>.");
           }
#line 5528 "gtkdialog_parser.c"
    break;

  case 178: /* progressbar: PROGRESSBAR attr EPROGRESSBAR  */
#line 835 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_PROGRESSBAR);
           }
#line 5536 "gtkdialog_parser.c"
    break;

  case 179: /* progressbar: PART_PROGRESSBAR tagattr '>' attr EPROGRESSBAR  */
#line 838 "gtkdialog_parser.y"
                                                   {
                token_store_attr(PUSH | WIDGET_PROGRESSBAR, (yyvsp[-3].nvval));
	   }
#line 5544 "gtkdialog_parser.c"
    break;

  case 180: /* progressbar: PROGRESSBAR attr PROGRESSBAR  */
#line 841 "gtkdialog_parser.y"
                                  {
		yyerror("</progressbar> expected instead of <progressbar>.");
           }
#line 5552 "gtkdialog_parser.c"
    break;

  case 181: /* list: LIST attr ELIST  */
#line 847 "gtkdialog_parser.y"
                    {
		token_store(PUSH | WIDGET_LIST);
	}
#line 5560 "gtkdialog_parser.c"
    break;

  case 182: /* list: PART_LIST tagattr '>' attr ELIST  */
#line 850 "gtkdialog_parser.y"
                                     {
		token_store_attr(PUSH | WIDGET_LIST, (yyvsp[-3].nvval));
	}
#line 5568 "gtkdialog_parser.c"
    break;

  case 183: /* list: LIST attr LIST  */
#line 853 "gtkdialog_parser.y"
                     {
		yyerror("</list> expected instead of <list>.");
	}
#line 5576 "gtkdialog_parser.c"
    break;

  case 184: /* table: TABLE attr ETABLE  */
#line 859 "gtkdialog_parser.y"
                      {
		token_store(PUSH | WIDGET_TABLE);
	}
#line 5584 "gtkdialog_parser.c"
    break;

  case 185: /* table: PART_TABLE tagattr '>' attr ETABLE  */
#line 862 "gtkdialog_parser.y"
                                       {
		token_store_attr(PUSH | WIDGET_TABLE, (yyvsp[-3].nvval));
	}
#line 5592 "gtkdialog_parser.c"
    break;

  case 186: /* table: TABLE attr TABLE  */
#line 865 "gtkdialog_parser.y"
                       {
		yyerror("</table> expected instead of <table>.");
	}
#line 5600 "gtkdialog_parser.c"
    break;

  case 187: /* combobox: COMBOBOX attr ECOMBOBOX  */
#line 871 "gtkdialog_parser.y"
                                             {
		token_store(PUSH | WIDGET_COMBOBOX);
	}
#line 5608 "gtkdialog_parser.c"
    break;

  case 188: /* combobox: PART_COMBOBOX tagattr '>' attr ECOMBOBOX  */
#line 874 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_COMBOBOX, (yyvsp[-3].nvval));
	}
#line 5616 "gtkdialog_parser.c"
    break;

  case 189: /* gvim: GVIM attr EGVIM  */
#line 880 "gtkdialog_parser.y"
                                {token_store(PUSH | WIDGET_GVIM);}
#line 5622 "gtkdialog_parser.c"
    break;

  case 190: /* pixmap: PIXMAP attr EPIXMAP  */
#line 884 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_PIXMAP);}
#line 5628 "gtkdialog_parser.c"
    break;

  case 191: /* pixmap: PART_PIXMAP tagattr '>' attr EPIXMAP  */
#line 885 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_PIXMAP, (yyvsp[-3].nvval));
	}
#line 5636 "gtkdialog_parser.c"
    break;

  case 192: /* calendar: CALENDAR attr ECALENDAR  */
#line 891 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_CALENDAR);
	}
#line 5644 "gtkdialog_parser.c"
    break;

  case 193: /* calendar: PART_CALENDAR tagattr '>' attr ECALENDAR  */
#line 894 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_CALENDAR, (yyvsp[-3].nvval));
	}
#line 5652 "gtkdialog_parser.c"
    break;

  case 194: /* toolbar: TOOLBAR ETOOLBAR  */
#line 907 "gtkdialog_parser.y"
                     {
		yyerror("The toolbar widget requires at least one tool item.");
	}
#line 5660 "gtkdialog_parser.c"
    break;

  case 195: /* toolbar: TOOLBAR radio_group_scope toolbarwlist attr ETOOLBAR  */
#line 910 "gtkdialog_parser.y"
                                                         {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLBAR);
	}
#line 5669 "gtkdialog_parser.c"
    break;

  case 196: /* toolbar: PART_TOOLBAR tagattr '>' radio_group_scope toolbarwlist attr ETOOLBAR  */
#line 914 "gtkdialog_parser.y"
                                                                          {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLBAR, (yyvsp[-5].nvval));
	}
#line 5678 "gtkdialog_parser.c"
    break;

  case 197: /* toolpalette: TOOLPALETTE ETOOLPALETTE  */
#line 921 "gtkdialog_parser.y"
                             {
		yyerror("The toolpalette widget requires at least one toolitemgroup.");
	}
#line 5686 "gtkdialog_parser.c"
    break;

  case 198: /* toolpalette: TOOLPALETTE radio_group_scope toolpalettewlist attr ETOOLPALETTE  */
#line 924 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLPALETTE);
	}
#line 5695 "gtkdialog_parser.c"
    break;

  case 199: /* toolpalette: PART_TOOLPALETTE tagattr '>' radio_group_scope toolpalettewlist attr ETOOLPALETTE  */
#line 928 "gtkdialog_parser.y"
                                                                                      {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLPALETTE, (yyvsp[-5].nvval));
	}
#line 5704 "gtkdialog_parser.c"
    break;

  case 201: /* toolpalettewlist: toolpalettewlist toolitemgroup  */
#line 936 "gtkdialog_parser.y"
                                   {
		token_store(SUM);
	}
#line 5712 "gtkdialog_parser.c"
    break;

  case 202: /* toolitemgroup: TOOLITEMGROUP ETOOLITEMGROUP  */
#line 942 "gtkdialog_parser.y"
                                 {
		yyerror("The toolitemgroup widget requires at least one tool item.");
	}
#line 5720 "gtkdialog_parser.c"
    break;

  case 203: /* toolitemgroup: TOOLITEMGROUP radio_group_scope toolbarwlist attr ETOOLITEMGROUP  */
#line 945 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLITEMGROUP);
	}
#line 5729 "gtkdialog_parser.c"
    break;

  case 204: /* toolitemgroup: PART_TOOLITEMGROUP tagattr '>' radio_group_scope toolbarwlist attr ETOOLITEMGROUP  */
#line 949 "gtkdialog_parser.y"
                                                                                      {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLITEMGROUP, (yyvsp[-5].nvval));
	}
#line 5738 "gtkdialog_parser.c"
    break;

  case 211: /* toolbarwlist: toolbarwlist toolbutton  */
#line 962 "gtkdialog_parser.y"
                            {
		token_store(SUM);
	}
#line 5746 "gtkdialog_parser.c"
    break;

  case 212: /* toolbarwlist: toolbarwlist toolitem  */
#line 965 "gtkdialog_parser.y"
                          {
		token_store(SUM);
	}
#line 5754 "gtkdialog_parser.c"
    break;

  case 213: /* toolbarwlist: toolbarwlist toggletoolbutton  */
#line 968 "gtkdialog_parser.y"
                                  {
		token_store(SUM);
	}
#line 5762 "gtkdialog_parser.c"
    break;

  case 214: /* toolbarwlist: toolbarwlist radiotoolbutton  */
#line 971 "gtkdialog_parser.y"
                                 {
		token_store(SUM);
	}
#line 5770 "gtkdialog_parser.c"
    break;

  case 215: /* toolbarwlist: toolbarwlist menutoolbutton  */
#line 974 "gtkdialog_parser.y"
                                {
		token_store(SUM);
	}
#line 5778 "gtkdialog_parser.c"
    break;

  case 216: /* toolbarwlist: toolbarwlist separatortoolitem  */
#line 977 "gtkdialog_parser.y"
                                   {
		token_store(SUM);
	}
#line 5786 "gtkdialog_parser.c"
    break;

  case 217: /* toolitem: TOOLITEM ETOOLITEM  */
#line 983 "gtkdialog_parser.y"
                       {
		yyerror("The toolitem widget requires exactly one child widget.");
	}
#line 5794 "gtkdialog_parser.c"
    break;

  case 218: /* toolitem: TOOLITEM radio_group_scope wlist attr ETOOLITEM  */
#line 986 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLITEM);
	}
#line 5803 "gtkdialog_parser.c"
    break;

  case 219: /* toolitem: PART_TOOLITEM tagattr '>' radio_group_scope wlist attr ETOOLITEM  */
#line 990 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLITEM, (yyvsp[-5].nvval));
	}
#line 5812 "gtkdialog_parser.c"
    break;

  case 220: /* toolbutton: TOOLBUTTON attr ETOOLBUTTON  */
#line 997 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_TOOLBUTTON);
	}
#line 5820 "gtkdialog_parser.c"
    break;

  case 221: /* toolbutton: PART_TOOLBUTTON tagattr '>' attr ETOOLBUTTON  */
#line 1000 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_TOOLBUTTON, (yyvsp[-3].nvval));
	}
#line 5828 "gtkdialog_parser.c"
    break;

  case 222: /* toggletoolbutton: TOGGLETOOLBUTTON attr ETOGGLETOOLBUTTON  */
#line 1006 "gtkdialog_parser.y"
                                            {
		token_store(PUSH | WIDGET_TOGGLETOOLBUTTON);
	}
#line 5836 "gtkdialog_parser.c"
    break;

  case 223: /* toggletoolbutton: PART_TOGGLETOOLBUTTON tagattr '>' attr ETOGGLETOOLBUTTON  */
#line 1009 "gtkdialog_parser.y"
                                                             {
		token_store_attr(PUSH | WIDGET_TOGGLETOOLBUTTON, (yyvsp[-3].nvval));
	}
#line 5844 "gtkdialog_parser.c"
    break;

  case 224: /* radiotoolbutton: RADIOTOOLBUTTON attr ERADIOTOOLBUTTON  */
#line 1015 "gtkdialog_parser.y"
                                          {
		token_store(PUSH | WIDGET_RADIOTOOLBUTTON);
	}
#line 5852 "gtkdialog_parser.c"
    break;

  case 225: /* radiotoolbutton: PART_RADIOTOOLBUTTON tagattr '>' attr ERADIOTOOLBUTTON  */
#line 1018 "gtkdialog_parser.y"
                                                           {
		token_store_attr(PUSH | WIDGET_RADIOTOOLBUTTON, (yyvsp[-3].nvval));
	}
#line 5860 "gtkdialog_parser.c"
    break;

  case 226: /* menutoolbutton: MENUTOOLBUTTON EMENUTOOLBUTTON  */
#line 1024 "gtkdialog_parser.y"
                                   {
		yyerror("The menutoolbutton widget requires at least one menu item.");
	}
#line 5868 "gtkdialog_parser.c"
    break;

  case 227: /* menutoolbutton: MENUTOOLBUTTON radio_group_scope menuwlist attr EMENUTOOLBUTTON  */
#line 1027 "gtkdialog_parser.y"
                                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_MENUTOOLBUTTON);
	}
#line 5877 "gtkdialog_parser.c"
    break;

  case 228: /* menutoolbutton: PART_MENUTOOLBUTTON tagattr '>' radio_group_scope menuwlist attr EMENUTOOLBUTTON  */
#line 1031 "gtkdialog_parser.y"
                                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_MENUTOOLBUTTON, (yyvsp[-5].nvval));
	}
#line 5886 "gtkdialog_parser.c"
    break;

  case 229: /* separatortoolitem: SEPARATORTOOLITEM attr ESEPARATORTOOLITEM  */
#line 1038 "gtkdialog_parser.y"
                                              {
		token_store(PUSH | WIDGET_SEPARATORTOOLITEM);
	}
#line 5894 "gtkdialog_parser.c"
    break;

  case 230: /* separatortoolitem: PART_SEPARATORTOOLITEM tagattr '>' attr ESEPARATORTOOLITEM  */
#line 1041 "gtkdialog_parser.y"
                                                               {
		token_store_attr(PUSH | WIDGET_SEPARATORTOOLITEM, (yyvsp[-3].nvval));
	}
#line 5902 "gtkdialog_parser.c"
    break;

  case 231: /* menubar: MENUBAR EMENUBAR  */
#line 1047 "gtkdialog_parser.y"
                     {
		yyerror("The menubar widget requires at least one menu widget.");
	}
#line 5910 "gtkdialog_parser.c"
    break;

  case 232: /* menubar: MENUBAR menu attr EMENUBAR  */
#line 1050 "gtkdialog_parser.y"
                               {
		token_store(PUSH | WIDGET_MENUBAR);
	}
#line 5918 "gtkdialog_parser.c"
    break;

  case 233: /* menubar: menu MENUBAR menu attr EMENUBAR  */
#line 1053 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_MENUBAR);
		token_store(SUM);
	}
#line 5927 "gtkdialog_parser.c"
    break;

  case 234: /* menubar: PART_MENUBAR tagattr '>' menu attr EMENUBAR  */
#line 1057 "gtkdialog_parser.y"
                                                {
		token_store_attr(PUSH | WIDGET_MENUBAR, (yyvsp[-4].nvval));
	}
#line 5935 "gtkdialog_parser.c"
    break;

  case 235: /* menubar: menu PART_MENUBAR tagattr '>' menu attr EMENUBAR  */
#line 1060 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_MENUBAR, (yyvsp[-4].nvval));
		token_store(SUM);
	}
#line 5944 "gtkdialog_parser.c"
    break;

  case 240: /* menu: MENU EMENU  */
#line 1074 "gtkdialog_parser.y"
               {
		yyerror("The menu widget requires at least one menuitem widget.");
	}
#line 5952 "gtkdialog_parser.c"
    break;

  case 241: /* menu: MENU radio_group_scope menuwlist attr EMENU  */
#line 1077 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_MENU);
	}
#line 5961 "gtkdialog_parser.c"
    break;

  case 242: /* menu: menuwlist MENU radio_group_scope menuwlist attr EMENU  */
#line 1081 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_MENU);
		token_store(SUM);
	}
#line 5971 "gtkdialog_parser.c"
    break;

  case 243: /* menu: PART_MENU tagattr '>' radio_group_scope menuwlist attr EMENU  */
#line 1086 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_MENU, (yyvsp[-5].nvval));
	}
#line 5980 "gtkdialog_parser.c"
    break;

  case 244: /* menu: menuwlist PART_MENU tagattr '>' radio_group_scope menuwlist attr EMENU  */
#line 1090 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_MENU, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5990 "gtkdialog_parser.c"
    break;

  case 245: /* menuitem: MENUITEM attr EMENUITEM  */
#line 1098 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_MENUITEM);
	}
#line 5998 "gtkdialog_parser.c"
    break;

  case 246: /* menuitem: menuwlist MENUITEM attr EMENUITEM  */
#line 1101 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_MENUITEM);
		token_store(SUM);
	}
#line 6007 "gtkdialog_parser.c"
    break;

  case 247: /* menuitem: PART_MENUITEM tagattr '>' attr EMENUITEM  */
#line 1105 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_MENUITEM, (yyvsp[-3].nvval));
	}
#line 6015 "gtkdialog_parser.c"
    break;

  case 248: /* menuitem: menuwlist PART_MENUITEM tagattr '>' attr EMENUITEM  */
#line 1108 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_MENUITEM, (yyvsp[-3].nvval));
		token_store(SUM);
	}
#line 6024 "gtkdialog_parser.c"
    break;

  case 249: /* menuitemseparator: MENUITEMSEPARATOR EMENUITEMSEPARATOR  */
#line 1115 "gtkdialog_parser.y"
                                         {
		token_store(PUSH | WIDGET_MENUITEMSEPARATOR);
	}
#line 6032 "gtkdialog_parser.c"
    break;

  case 250: /* menuitemseparator: menuwlist MENUITEMSEPARATOR EMENUITEMSEPARATOR  */
#line 1118 "gtkdialog_parser.y"
                                                   {
		token_store(PUSH | WIDGET_MENUITEMSEPARATOR);
		token_store(SUM);
	}
#line 6041 "gtkdialog_parser.c"
    break;

  case 251: /* recentchoosermenu: RECENTCHOOSERMENU attr ERECENTCHOOSERMENU  */
#line 1125 "gtkdialog_parser.y"
                                              {
		token_store(PUSH | WIDGET_RECENTCHOOSERMENU);
	}
#line 6049 "gtkdialog_parser.c"
    break;

  case 252: /* recentchoosermenu: menuwlist RECENTCHOOSERMENU attr ERECENTCHOOSERMENU  */
#line 1128 "gtkdialog_parser.y"
                                                        {
		token_store(PUSH | WIDGET_RECENTCHOOSERMENU);
		token_store(SUM);
	}
#line 6058 "gtkdialog_parser.c"
    break;

  case 253: /* recentchoosermenu: PART_RECENTCHOOSERMENU tagattr '>' attr ERECENTCHOOSERMENU  */
#line 1132 "gtkdialog_parser.y"
                                                               {
		token_store_attr(PUSH | WIDGET_RECENTCHOOSERMENU, (yyvsp[-3].nvval));
	}
#line 6066 "gtkdialog_parser.c"
    break;

  case 254: /* recentchoosermenu: menuwlist PART_RECENTCHOOSERMENU tagattr '>' attr ERECENTCHOOSERMENU  */
#line 1135 "gtkdialog_parser.y"
                                                                         {
		token_store_attr(PUSH | WIDGET_RECENTCHOOSERMENU, (yyvsp[-3].nvval));
		token_store(SUM);
	}
#line 6075 "gtkdialog_parser.c"
    break;

  case 255: /* hseparator: HSEPARATOR EHSEPARATOR  */
#line 1142 "gtkdialog_parser.y"
                           {
		token_store(PUSH | WIDGET_HSEPARATOR);
	}
#line 6083 "gtkdialog_parser.c"
    break;

  case 256: /* hseparator: PART_HSEPARATOR tagattr '>' EHSEPARATOR  */
#line 1145 "gtkdialog_parser.y"
                                            {
		token_store_attr(PUSH | WIDGET_HSEPARATOR, (yyvsp[-2].nvval));
	}
#line 6091 "gtkdialog_parser.c"
    break;

  case 257: /* vseparator: VSEPARATOR EVSEPARATOR  */
#line 1151 "gtkdialog_parser.y"
                           {
		token_store(PUSH | WIDGET_VSEPARATOR);
	}
#line 6099 "gtkdialog_parser.c"
    break;

  case 258: /* vseparator: PART_VSEPARATOR tagattr '>' EVSEPARATOR  */
#line 1154 "gtkdialog_parser.y"
                                            {
		token_store_attr(PUSH | WIDGET_VSEPARATOR, (yyvsp[-2].nvval));
	}
#line 6107 "gtkdialog_parser.c"
    break;

  case 259: /* comboboxtext: COMBOBOXTEXT attr ECOMBOBOXTEXT  */
#line 1160 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_COMBOBOXTEXT);
	}
#line 6115 "gtkdialog_parser.c"
    break;

  case 260: /* comboboxtext: PART_COMBOBOXTEXT tagattr '>' attr ECOMBOBOXTEXT  */
#line 1163 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_COMBOBOXTEXT, (yyvsp[-3].nvval));
	}
#line 6123 "gtkdialog_parser.c"
    break;

  case 261: /* comboboxentry: COMBOBOXENTRY attr ECOMBOBOXENTRY  */
#line 1169 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_COMBOBOXENTRY);
	}
#line 6131 "gtkdialog_parser.c"
    break;

  case 262: /* comboboxentry: PART_COMBOBOXENTRY tagattr '>' attr ECOMBOBOXENTRY  */
#line 1172 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_COMBOBOXENTRY, (yyvsp[-3].nvval));
	}
#line 6139 "gtkdialog_parser.c"
    break;

  case 263: /* hscale: HSCALE attr EHSCALE  */
#line 1178 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_HSCALE);
	}
#line 6147 "gtkdialog_parser.c"
    break;

  case 264: /* hscale: PART_HSCALE tagattr '>' attr EHSCALE  */
#line 1181 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_HSCALE, (yyvsp[-3].nvval));
	}
#line 6155 "gtkdialog_parser.c"
    break;

  case 265: /* vscale: VSCALE attr EVSCALE  */
#line 1187 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_VSCALE);
	}
#line 6163 "gtkdialog_parser.c"
    break;

  case 266: /* vscale: PART_VSCALE tagattr '>' attr EVSCALE  */
#line 1190 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_VSCALE, (yyvsp[-3].nvval));
	}
#line 6171 "gtkdialog_parser.c"
    break;

  case 267: /* spinbutton: SPINBUTTON attr ESPINBUTTON  */
#line 1196 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_SPINBUTTON);
	}
#line 6179 "gtkdialog_parser.c"
    break;

  case 268: /* spinbutton: PART_SPINBUTTON tagattr '>' attr ESPINBUTTON  */
#line 1199 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_SPINBUTTON, (yyvsp[-3].nvval));
	}
#line 6187 "gtkdialog_parser.c"
    break;

  case 269: /* timer: TIMER attr ETIMER  */
#line 1205 "gtkdialog_parser.y"
                      {
		token_store(PUSH | WIDGET_TIMER);
	}
#line 6195 "gtkdialog_parser.c"
    break;

  case 270: /* timer: PART_TIMER tagattr '>' attr ETIMER  */
#line 1208 "gtkdialog_parser.y"
                                       {
		token_store_attr(PUSH | WIDGET_TIMER, (yyvsp[-3].nvval));
	}
#line 6203 "gtkdialog_parser.c"
    break;

  case 271: /* togglebutton: TOGGLEBUTTON attr ETOGGLEBUTTON  */
#line 1214 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_TOGGLEBUTTON);
	}
#line 6211 "gtkdialog_parser.c"
    break;

  case 272: /* togglebutton: PART_TOGGLEBUTTON tagattr '>' attr ETOGGLEBUTTON  */
#line 1217 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_TOGGLEBUTTON, (yyvsp[-3].nvval));
	}
#line 6219 "gtkdialog_parser.c"
    break;

  case 273: /* statusbar: STATUSBAR attr ESTATUSBAR  */
#line 1223 "gtkdialog_parser.y"
                              {
		token_store(PUSH | WIDGET_STATUSBAR);
	}
#line 6227 "gtkdialog_parser.c"
    break;

  case 274: /* statusbar: PART_STATUSBAR tagattr '>' attr ESTATUSBAR  */
#line 1226 "gtkdialog_parser.y"
                                               {
		token_store_attr(PUSH | WIDGET_STATUSBAR, (yyvsp[-3].nvval));
	}
#line 6235 "gtkdialog_parser.c"
    break;

  case 275: /* statusicon: STATUSICON attr ESTATUSICON  */
#line 1232 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_STATUSICON);
	}
#line 6243 "gtkdialog_parser.c"
    break;

  case 276: /* statusicon: PART_STATUSICON tagattr '>' attr ESTATUSICON  */
#line 1235 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_STATUSICON, (yyvsp[-3].nvval));
	}
#line 6251 "gtkdialog_parser.c"
    break;

  case 277: /* accellabel: ACCELLABEL attr EACCELLABEL  */
#line 1241 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_ACCELLABEL);
	}
#line 6259 "gtkdialog_parser.c"
    break;

  case 278: /* accellabel: PART_ACCELLABEL tagattr '>' attr EACCELLABEL  */
#line 1244 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_ACCELLABEL, (yyvsp[-3].nvval));
	}
#line 6267 "gtkdialog_parser.c"
    break;

  case 279: /* arrow: ARROW attr EARROW  */
#line 1250 "gtkdialog_parser.y"
                      {
		token_store(PUSH | WIDGET_ARROW);
	}
#line 6275 "gtkdialog_parser.c"
    break;

  case 280: /* arrow: PART_ARROW tagattr '>' attr EARROW  */
#line 1253 "gtkdialog_parser.y"
                                       {
		token_store_attr(PUSH | WIDGET_ARROW, (yyvsp[-3].nvval));
	}
#line 6283 "gtkdialog_parser.c"
    break;

  case 281: /* hsv: HSV attr EHSV  */
#line 1259 "gtkdialog_parser.y"
                  {
		token_store(PUSH | WIDGET_HSV);
	}
#line 6291 "gtkdialog_parser.c"
    break;

  case 282: /* hsv: PART_HSV tagattr '>' attr EHSV  */
#line 1262 "gtkdialog_parser.y"
                                   {
		token_store_attr(PUSH | WIDGET_HSV, (yyvsp[-3].nvval));
	}
#line 6299 "gtkdialog_parser.c"
    break;

  case 283: /* hruler: HRULER attr EHRULER  */
#line 1268 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_HRULER);
	}
#line 6307 "gtkdialog_parser.c"
    break;

  case 284: /* hruler: PART_HRULER tagattr '>' attr EHRULER  */
#line 1271 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_HRULER, (yyvsp[-3].nvval));
	}
#line 6315 "gtkdialog_parser.c"
    break;

  case 285: /* vruler: VRULER attr EVRULER  */
#line 1277 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_VRULER);
	}
#line 6323 "gtkdialog_parser.c"
    break;

  case 286: /* vruler: PART_VRULER tagattr '>' attr EVRULER  */
#line 1280 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_VRULER, (yyvsp[-3].nvval));
	}
#line 6331 "gtkdialog_parser.c"
    break;

  case 287: /* colorbutton: COLORBUTTON attr ECOLORBUTTON  */
#line 1286 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_COLORBUTTON);
	}
#line 6339 "gtkdialog_parser.c"
    break;

  case 288: /* colorbutton: PART_COLORBUTTON tagattr '>' attr ECOLORBUTTON  */
#line 1289 "gtkdialog_parser.y"
                                                   {
		token_store_attr(PUSH | WIDGET_COLORBUTTON, (yyvsp[-3].nvval));
	}
#line 6347 "gtkdialog_parser.c"
    break;

  case 289: /* fontbutton: FONTBUTTON attr EFONTBUTTON  */
#line 1295 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_FONTBUTTON);
	}
#line 6355 "gtkdialog_parser.c"
    break;

  case 290: /* fontbutton: PART_FONTBUTTON tagattr '>' attr EFONTBUTTON  */
#line 1298 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_FONTBUTTON, (yyvsp[-3].nvval));
	}
#line 6363 "gtkdialog_parser.c"
    break;

  case 291: /* terminal: TERMINAL attr ETERMINAL  */
#line 1304 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_TERMINAL);
	}
#line 6371 "gtkdialog_parser.c"
    break;

  case 292: /* terminal: PART_TERMINAL tagattr '>' attr ETERMINAL  */
#line 1307 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_TERMINAL, (yyvsp[-3].nvval));
	}
#line 6379 "gtkdialog_parser.c"
    break;

  case 304: /* label: LABEL STRING ELABEL  */
#line 1327 "gtkdialog_parser.y"
                                    {
		token_store_owned_argument(SET | ATTR_LABEL, (yyvsp[-1].cval));     }
#line 6386 "gtkdialog_parser.c"
    break;

  case 305: /* sensitive: SENSITIVE STRING ESENSITIVE  */
#line 1332 "gtkdialog_parser.y"
                                      {
     token_store_owned_argument(SET | ATTR_SENSITIVE, (yyvsp[-1].cval));  }
#line 6393 "gtkdialog_parser.c"
    break;

  case 306: /* defaultvalue: DEFAULT STRING EDEFAULT  */
#line 1337 "gtkdialog_parser.y"
                             {
     token_store_owned_argument(SET | ATTR_DEFAULT, (yyvsp[-1].cval));   }
#line 6400 "gtkdialog_parser.c"
    break;

  case 307: /* width: WIDTH STRING EWIDTH  */
#line 1342 "gtkdialog_parser.y"
                                    {
     token_store_owned_argument(SET | ATTR_WIDTH, (yyvsp[-1].cval));    }
#line 6407 "gtkdialog_parser.c"
    break;

  case 308: /* height: HEIGHT STRING EHEIGHT  */
#line 1347 "gtkdialog_parser.y"
                                    {
     token_store_owned_argument(SET | ATTR_HEIGHT, (yyvsp[-1].cval));   }
#line 6414 "gtkdialog_parser.c"
    break;

  case 309: /* input: INPUT STRING EINPUT  */
#line 1352 "gtkdialog_parser.y"
                           {
		token_store_owned_argument(SET|ATTR_INPUT|SUB_ATTR_SHELL, (yyvsp[-1].cval));
	}
#line 6422 "gtkdialog_parser.c"
    break;

  case 310: /* input: PART_INPUT tagattr '>' STRING EINPUT  */
#line 1355 "gtkdialog_parser.y"
                                         {
		token_store_owned_argument_attr(SET|ATTR_INPUT, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6430 "gtkdialog_parser.c"
    break;

  case 311: /* input: INPUTFILE STRING EINPUT  */
#line 1358 "gtkdialog_parser.y"
                             {
		token_store_owned_argument(SET|ATTR_INPUT|SUB_ATTR_FILE, (yyvsp[-1].cval));
	}
#line 6438 "gtkdialog_parser.c"
    break;

  case 312: /* input: PART_INPUTFILE tagattr '>' STRING EINPUT  */
#line 1361 "gtkdialog_parser.y"
                                             {
		token_store_owned_argument_attr(SET|ATTR_INPUT|SUB_ATTR_FILE, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6446 "gtkdialog_parser.c"
    break;

  case 313: /* input: PART_INPUTFILE tagattr '>' EINPUT  */
#line 1364 "gtkdialog_parser.y"
                                      {
		token_store_with_argument_attr(SET|ATTR_INPUT|SUB_ATTR_FILE, "", (yyvsp[-2].nvval));
	}
#line 6454 "gtkdialog_parser.c"
    break;

  case 314: /* output: OUTPUT STRING EOUTPUT  */
#line 1370 "gtkdialog_parser.y"
                          {
		fprintf(stderr, "<output>: Not implemented.\n");
		g_free((yyvsp[-1].cval));
	}
#line 6463 "gtkdialog_parser.c"
    break;

  case 315: /* output: OUTPUTFILE STRING EOUTPUT  */
#line 1374 "gtkdialog_parser.y"
                              {
		token_store_owned_argument(SET|ATTR_OUTPUT|SUB_ATTR_FILE, (yyvsp[-1].cval));
	}
#line 6471 "gtkdialog_parser.c"
    break;

  case 316: /* output: PART_OUTPUTFILE tagattr '>' STRING EOUTPUT  */
#line 1377 "gtkdialog_parser.y"
                                               {
		token_store_owned_argument_attr(SET|ATTR_OUTPUT|SUB_ATTR_FILE, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6479 "gtkdialog_parser.c"
    break;

  case 317: /* variable: VARIABLE STRING EVARIABLE  */
#line 1383 "gtkdialog_parser.y"
                              {
		token_store_owned_argument(SET | ATTR_VARIABLE, (yyvsp[-1].cval));
	}
#line 6487 "gtkdialog_parser.c"
    break;

  case 318: /* variable: PART_VARIABLE tagattr '>' STRING EVARIABLE  */
#line 1386 "gtkdialog_parser.y"
                                               {
		token_store_owned_argument_attr(SET | ATTR_VARIABLE, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6495 "gtkdialog_parser.c"
    break;

  case 319: /* action: ACTION STRING EACTION  */
#line 1392 "gtkdialog_parser.y"
                           {
		token_store_owned_argument(SET|ATTR_ACTION, (yyvsp[-1].cval));
	}
#line 6503 "gtkdialog_parser.c"
    break;

  case 320: /* action: PART_ACTION tagattr '>' STRING EACTION  */
#line 1395 "gtkdialog_parser.y"
                                           {
		token_store_owned_argument_attr(SET | ATTR_ACTION, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6511 "gtkdialog_parser.c"
    break;

  case 321: /* item: ITEM STRING EITEM  */
#line 1401 "gtkdialog_parser.y"
                      {
		token_store_owned_argument(SET | ATTR_ITEM, (yyvsp[-1].cval));
	}
#line 6519 "gtkdialog_parser.c"
    break;

  case 322: /* item: ITEM EITEM  */
#line 1404 "gtkdialog_parser.y"
               {
		token_store_with_argument( SET | ATTR_ITEM, "");
	}
#line 6527 "gtkdialog_parser.c"
    break;

  case 323: /* item: PART_ITEM tagattr '>' STRING EITEM  */
#line 1407 "gtkdialog_parser.y"
                                       {
		token_store_owned_argument_attr(SET | ATTR_ITEM, (yyvsp[-1].cval), (yyvsp[-3].nvval));
                    }
#line 6535 "gtkdialog_parser.c"
    break;

  case 324: /* tagattr: TAG_ATTR_NAME '=' STRING  */
#line 1413 "gtkdialog_parser.y"
                             {
		(yyval.nvval) = new_tag_attributeset_owned((yyvsp[-2].cval), (yyvsp[0].cval));
	}
#line 6543 "gtkdialog_parser.c"
    break;

  case 325: /* tagattr: tagattr TAG_ATTR_NAME '=' STRING  */
#line 1416 "gtkdialog_parser.y"
                                     {
		(yyval.nvval) = add_tag_attribute_owned((yyvsp[-3].nvval), (yyvsp[-2].cval), (yyvsp[0].cval));
	}
#line 6551 "gtkdialog_parser.c"
    break;

  case 327: /* imperative: SHOW_WIDGETS  */
#line 1423 "gtkdialog_parser.y"
                 {
		token_store(SHOW);
	}
#line 6559 "gtkdialog_parser.c"
    break;

  case 328: /* imperative: if expression '>' then wlist endif  */
#line 1426 "gtkdialog_parser.y"
                                       {
		instruction_set_jump((yyvsp[-2].ival), (yyvsp[0].ival) + 1);
	}
#line 6567 "gtkdialog_parser.c"
    break;

  case 329: /* imperative: while expression '>' do wlist ewhile  */
#line 1429 "gtkdialog_parser.y"
                                         {
		instruction_set_jump((yyvsp[-2].ival), (yyvsp[0].ival) + 1);
		instruction_set_jump((yyvsp[0].ival), (yyvsp[-5].ival) + 1);
	}
#line 6576 "gtkdialog_parser.c"
    break;

  case 330: /* assignment: EMB_VARIABLE ':' '=' expression  */
#line 1436 "gtkdialog_parser.y"
                                    {
		token_store_owned_argument(IMASSG | VARIABLE_NAME, (yyvsp[-3].cval));
	}
#line 6584 "gtkdialog_parser.c"
    break;

  case 331: /* expression: EMB_VARIABLE  */
#line 1442 "gtkdialog_parser.y"
                 {
		token_store_owned_argument(IMPUSH | VARIABLE_NAME, (yyvsp[0].cval));
	}
#line 6592 "gtkdialog_parser.c"
    break;

  case 332: /* expression: EMB_NUMBER  */
#line 1445 "gtkdialog_parser.y"
               {
		token_store_owned_argument(IMPUSH | CONST_NUMBER, (yyvsp[0].cval));
	}
#line 6600 "gtkdialog_parser.c"
    break;

  case 333: /* expression: expression '+' expression  */
#line 1448 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_ADD);
	}
#line 6608 "gtkdialog_parser.c"
    break;

  case 334: /* expression: expression '-' expression  */
#line 1451 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_SUBST);
	}
#line 6616 "gtkdialog_parser.c"
    break;

  case 335: /* expression: expression '*' expression  */
#line 1454 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_MULT);
	}
#line 6624 "gtkdialog_parser.c"
    break;

  case 336: /* expression: expression '/' expression  */
#line 1457 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_DIV);
	}
#line 6632 "gtkdialog_parser.c"
    break;

  case 337: /* expression: expression '=' expression  */
#line 1460 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | REL_EQ);
	}
#line 6640 "gtkdialog_parser.c"
    break;

  case 338: /* expression: expression '!' '=' expression  */
#line 1463 "gtkdialog_parser.y"
                                  {
		token_store(IMPUSH | REL_NE);
	}
#line 6648 "gtkdialog_parser.c"
    break;

  case 340: /* then: %empty  */
#line 1473 "gtkdialog_parser.y"
        {
		token_store(IFNGOTO);
		(yyval.ival) = instruction_get_pc();
	}
#line 6657 "gtkdialog_parser.c"
    break;

  case 341: /* endif: ENDIF  */
#line 1480 "gtkdialog_parser.y"
           {
		token_store(IMMERGE);
		(yyval.ival) = instruction_get_pc();
	}
#line 6666 "gtkdialog_parser.c"
    break;

  case 342: /* while: WHILE  */
#line 1487 "gtkdialog_parser.y"
          { (yyval.ival) = instruction_get_pc(); }
#line 6672 "gtkdialog_parser.c"
    break;

  case 343: /* ewhile: EWHILE  */
#line 1491 "gtkdialog_parser.y"
           {
		token_store(IMMERGE);
		token_store(GOTO);
		(yyval.ival) = instruction_get_pc();
	}
#line 6682 "gtkdialog_parser.c"
    break;

  case 344: /* do: %empty  */
#line 1499 "gtkdialog_parser.y"
    {
		token_store(IFNGOTO);
		(yyval.ival) = instruction_get_pc();
    }
#line 6691 "gtkdialog_parser.c"
    break;

  case 345: /* linkbutton: LINKBUTTON attr ELINKBUTTON  */
#line 1506 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_LINKBUTTON);
	}
#line 6699 "gtkdialog_parser.c"
    break;

  case 346: /* linkbutton: PART_LINKBUTTON tagattr '>' attr ELINKBUTTON  */
#line 1509 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_LINKBUTTON, (yyvsp[-3].nvval));
	}
#line 6707 "gtkdialog_parser.c"
    break;

  case 347: /* spinner: SPINNER attr ESPINNER  */
#line 1515 "gtkdialog_parser.y"
                          {
		token_store(PUSH | WIDGET_SPINNER);
	}
#line 6715 "gtkdialog_parser.c"
    break;

  case 348: /* spinner: PART_SPINNER tagattr '>' attr ESPINNER  */
#line 1518 "gtkdialog_parser.y"
                                           {
		token_store_attr(PUSH | WIDGET_SPINNER, (yyvsp[-3].nvval));
	}
#line 6723 "gtkdialog_parser.c"
    break;

  case 349: /* filechooserbutton: FILECHOOSERBUTTON attr EFILECHOOSERBUTTON  */
#line 1524 "gtkdialog_parser.y"
                                              {
		token_store(PUSH | WIDGET_FILECHOOSERBUTTON);
	}
#line 6731 "gtkdialog_parser.c"
    break;

  case 350: /* filechooserbutton: PART_FILECHOOSERBUTTON tagattr '>' attr EFILECHOOSERBUTTON  */
#line 1527 "gtkdialog_parser.y"
                                                               {
		token_store_attr(PUSH | WIDGET_FILECHOOSERBUTTON, (yyvsp[-3].nvval));
	}
#line 6739 "gtkdialog_parser.c"
    break;

  case 351: /* iconview: ICONVIEW attr EICONVIEW  */
#line 1533 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_ICONVIEW);
	}
#line 6747 "gtkdialog_parser.c"
    break;

  case 352: /* iconview: PART_ICONVIEW tagattr '>' attr EICONVIEW  */
#line 1536 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_ICONVIEW, (yyvsp[-3].nvval));
	}
#line 6755 "gtkdialog_parser.c"
    break;

  case 353: /* cellview: CELLVIEW attr ECELLVIEW  */
#line 1542 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_CELLVIEW);
	}
#line 6763 "gtkdialog_parser.c"
    break;

  case 354: /* cellview: PART_CELLVIEW tagattr '>' attr ECELLVIEW  */
#line 1545 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_CELLVIEW, (yyvsp[-3].nvval));
	}
#line 6771 "gtkdialog_parser.c"
    break;

  case 355: /* drawingarea: DRAWINGAREA attr EDRAWINGAREA  */
#line 1551 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_DRAWINGAREA);
	}
#line 6779 "gtkdialog_parser.c"
    break;

  case 356: /* drawingarea: PART_DRAWINGAREA tagattr '>' attr EDRAWINGAREA  */
#line 1554 "gtkdialog_parser.y"
                                                   {
		token_store_attr(PUSH | WIDGET_DRAWINGAREA, (yyvsp[-3].nvval));
	}
#line 6787 "gtkdialog_parser.c"
    break;

  case 357: /* scalebutton: SCALEBUTTON attr ESCALEBUTTON  */
#line 1560 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_SCALEBUTTON);
	}
#line 6795 "gtkdialog_parser.c"
    break;

  case 358: /* scalebutton: PART_SCALEBUTTON tagattr '>' attr ESCALEBUTTON  */
#line 1563 "gtkdialog_parser.y"
                                                   {
		token_store_attr(PUSH | WIDGET_SCALEBUTTON, (yyvsp[-3].nvval));
	}
#line 6803 "gtkdialog_parser.c"
    break;

  case 359: /* volumebutton: VOLUMEBUTTON attr EVOLUMEBUTTON  */
#line 1569 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_VOLUMEBUTTON);
	}
#line 6811 "gtkdialog_parser.c"
    break;

  case 360: /* volumebutton: PART_VOLUMEBUTTON tagattr '>' attr EVOLUMEBUTTON  */
#line 1572 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_VOLUMEBUTTON, (yyvsp[-3].nvval));
	}
#line 6819 "gtkdialog_parser.c"
    break;

  case 361: /* recentchooser: RECENTCHOOSER attr ERECENTCHOOSER  */
#line 1578 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_RECENTCHOOSER);
	}
#line 6827 "gtkdialog_parser.c"
    break;

  case 362: /* recentchooser: PART_RECENTCHOOSER tagattr '>' attr ERECENTCHOOSER  */
#line 1581 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_RECENTCHOOSER, (yyvsp[-3].nvval));
	}
#line 6835 "gtkdialog_parser.c"
    break;

  case 363: /* socket: SOCKET attr ESOCKET  */
#line 1587 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_SOCKET);
	}
#line 6843 "gtkdialog_parser.c"
    break;

  case 364: /* socket: PART_SOCKET tagattr '>' attr ESOCKET  */
#line 1590 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_SOCKET, (yyvsp[-3].nvval));
	}
#line 6851 "gtkdialog_parser.c"
    break;

  case 365: /* hscrollbar: HSCROLLBAR attr EHSCROLLBAR  */
#line 1596 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_HSCROLLBAR);
	}
#line 6859 "gtkdialog_parser.c"
    break;

  case 366: /* hscrollbar: PART_HSCROLLBAR tagattr '>' attr EHSCROLLBAR  */
#line 1599 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_HSCROLLBAR, (yyvsp[-3].nvval));
	}
#line 6867 "gtkdialog_parser.c"
    break;

  case 367: /* vscrollbar: VSCROLLBAR attr EVSCROLLBAR  */
#line 1605 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_VSCROLLBAR);
	}
#line 6875 "gtkdialog_parser.c"
    break;

  case 368: /* vscrollbar: PART_VSCROLLBAR tagattr '>' attr EVSCROLLBAR  */
#line 1608 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_VSCROLLBAR, (yyvsp[-3].nvval));
	}
#line 6883 "gtkdialog_parser.c"
    break;


#line 6887 "gtkdialog_parser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1613 "gtkdialog_parser.y"


extern gboolean option_print_ir;

int gtkdialog_wrap(void)
{
	#ifdef DEBUG
	g_message("%s(): Start", __func__);
	#endif
	return 1;
}

int gtkdialog_error(char *c)
{
	g_error("%s: Error in line %d, near token '%s': %s\n",
		PACKAGE, linenumber, Token, c);
}

void yyerror_simple(char *c)
{
	g_error("%s: Error: %s", PACKAGE, c);
}

int yywarning(char *c){
	#ifdef DEBUG
		gtkdialog_warning("Warning: %s.", c);
	#endif
	if (!option_no_warning)
		gtkdialog_warning("%s: Warning: %s.", PACKAGE, c);
	return option_no_warning;
}
