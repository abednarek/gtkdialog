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
  YYSYMBOL_IMAGEVIEW = 316,                /* IMAGEVIEW  */
  YYSYMBOL_PART_IMAGEVIEW = 317,           /* PART_IMAGEVIEW  */
  YYSYMBOL_EIMAGEVIEW = 318,               /* EIMAGEVIEW  */
  YYSYMBOL_CURVE = 319,                    /* CURVE  */
  YYSYMBOL_PART_CURVE = 320,               /* PART_CURVE  */
  YYSYMBOL_ECURVE = 321,                   /* ECURVE  */
  YYSYMBOL_322_ = 322,                     /* '>'  */
  YYSYMBOL_323_ = 323,                     /* ':'  */
  YYSYMBOL_324_ = 324,                     /* '!'  */
  YYSYMBOL_YYACCEPT = 325,                 /* $accept  */
  YYSYMBOL_window = 326,                   /* window  */
  YYSYMBOL_radio_group_scope = 327,        /* radio_group_scope  */
  YYSYMBOL_wlist = 328,                    /* wlist  */
  YYSYMBOL_widget = 329,                   /* widget  */
  YYSYMBOL_entry = 330,                    /* entry  */
  YYSYMBOL_edit = 331,                     /* edit  */
  YYSYMBOL_tree = 332,                     /* tree  */
  YYSYMBOL_chooser = 333,                  /* chooser  */
  YYSYMBOL_text = 334,                     /* text  */
  YYSYMBOL_button = 335,                   /* button  */
  YYSYMBOL_checkbox = 336,                 /* checkbox  */
  YYSYMBOL_radiobutton = 337,              /* radiobutton  */
  YYSYMBOL_progressbar = 338,              /* progressbar  */
  YYSYMBOL_list = 339,                     /* list  */
  YYSYMBOL_table = 340,                    /* table  */
  YYSYMBOL_combobox = 341,                 /* combobox  */
  YYSYMBOL_gvim = 342,                     /* gvim  */
  YYSYMBOL_pixmap = 343,                   /* pixmap  */
  YYSYMBOL_calendar = 344,                 /* calendar  */
  YYSYMBOL_toolbar = 345,                  /* toolbar  */
  YYSYMBOL_toolpalette = 346,              /* toolpalette  */
  YYSYMBOL_toolpalettewlist = 347,         /* toolpalettewlist  */
  YYSYMBOL_toolitemgroup = 348,            /* toolitemgroup  */
  YYSYMBOL_toolbarwlist = 349,             /* toolbarwlist  */
  YYSYMBOL_toolitem = 350,                 /* toolitem  */
  YYSYMBOL_toolbutton = 351,               /* toolbutton  */
  YYSYMBOL_toggletoolbutton = 352,         /* toggletoolbutton  */
  YYSYMBOL_radiotoolbutton = 353,          /* radiotoolbutton  */
  YYSYMBOL_menutoolbutton = 354,           /* menutoolbutton  */
  YYSYMBOL_separatortoolitem = 355,        /* separatortoolitem  */
  YYSYMBOL_menubar = 356,                  /* menubar  */
  YYSYMBOL_menuwlist = 357,                /* menuwlist  */
  YYSYMBOL_menu = 358,                     /* menu  */
  YYSYMBOL_menuitem = 359,                 /* menuitem  */
  YYSYMBOL_menuitemseparator = 360,        /* menuitemseparator  */
  YYSYMBOL_recentchoosermenu = 361,        /* recentchoosermenu  */
  YYSYMBOL_hseparator = 362,               /* hseparator  */
  YYSYMBOL_vseparator = 363,               /* vseparator  */
  YYSYMBOL_comboboxtext = 364,             /* comboboxtext  */
  YYSYMBOL_comboboxentry = 365,            /* comboboxentry  */
  YYSYMBOL_hscale = 366,                   /* hscale  */
  YYSYMBOL_vscale = 367,                   /* vscale  */
  YYSYMBOL_spinbutton = 368,               /* spinbutton  */
  YYSYMBOL_timer = 369,                    /* timer  */
  YYSYMBOL_togglebutton = 370,             /* togglebutton  */
  YYSYMBOL_statusbar = 371,                /* statusbar  */
  YYSYMBOL_statusicon = 372,               /* statusicon  */
  YYSYMBOL_accellabel = 373,               /* accellabel  */
  YYSYMBOL_arrow = 374,                    /* arrow  */
  YYSYMBOL_hsv = 375,                      /* hsv  */
  YYSYMBOL_curve = 376,                    /* curve  */
  YYSYMBOL_hruler = 377,                   /* hruler  */
  YYSYMBOL_vruler = 378,                   /* vruler  */
  YYSYMBOL_colorbutton = 379,              /* colorbutton  */
  YYSYMBOL_fontbutton = 380,               /* fontbutton  */
  YYSYMBOL_terminal = 381,                 /* terminal  */
  YYSYMBOL_attr = 382,                     /* attr  */
  YYSYMBOL_label = 383,                    /* label  */
  YYSYMBOL_sensitive = 384,                /* sensitive  */
  YYSYMBOL_defaultvalue = 385,             /* defaultvalue  */
  YYSYMBOL_width = 386,                    /* width  */
  YYSYMBOL_height = 387,                   /* height  */
  YYSYMBOL_input = 388,                    /* input  */
  YYSYMBOL_output = 389,                   /* output  */
  YYSYMBOL_variable = 390,                 /* variable  */
  YYSYMBOL_action = 391,                   /* action  */
  YYSYMBOL_item = 392,                     /* item  */
  YYSYMBOL_tagattr = 393,                  /* tagattr  */
  YYSYMBOL_imperative = 394,               /* imperative  */
  YYSYMBOL_assignment = 395,               /* assignment  */
  YYSYMBOL_expression = 396,               /* expression  */
  YYSYMBOL_if = 397,                       /* if  */
  YYSYMBOL_then = 398,                     /* then  */
  YYSYMBOL_endif = 399,                    /* endif  */
  YYSYMBOL_while = 400,                    /* while  */
  YYSYMBOL_ewhile = 401,                   /* ewhile  */
  YYSYMBOL_do = 402,                       /* do  */
  YYSYMBOL_linkbutton = 403,               /* linkbutton  */
  YYSYMBOL_spinner = 404,                  /* spinner  */
  YYSYMBOL_filechooserbutton = 405,        /* filechooserbutton  */
  YYSYMBOL_iconview = 406,                 /* iconview  */
  YYSYMBOL_cellview = 407,                 /* cellview  */
  YYSYMBOL_drawingarea = 408,              /* drawingarea  */
  YYSYMBOL_imageview = 409,                /* imageview  */
  YYSYMBOL_scalebutton = 410,              /* scalebutton  */
  YYSYMBOL_volumebutton = 411,             /* volumebutton  */
  YYSYMBOL_recentchooser = 412,            /* recentchooser  */
  YYSYMBOL_socket = 413,                   /* socket  */
  YYSYMBOL_hscrollbar = 414,               /* hscrollbar  */
  YYSYMBOL_vscrollbar = 415                /* vscrollbar  */
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
#define YYLAST   11295

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  325
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  91
/* YYNRULES -- Number of rules.  */
#define YYNRULES  374
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1319

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   571


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
       2,     2,     2,   324,     2,     2,     2,     2,     2,     2,
       2,     2,   120,   119,     2,   118,     2,   121,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   323,     2,
       2,   117,   322,     2,     2,     2,     2,     2,     2,     2,
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
     310,   311,   312,   313,   314,   315,   316,   317,   318,   319,
     320,   321
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   253,   253,   258,   263,   268,   273,   278,   282,   286,
     291,   296,   300,   304,   307,   312,   317,   322,   330,   336,
     337,   340,   341,   342,   346,   351,   355,   360,   364,   369,
     373,   378,   382,   387,   391,   396,   400,   405,   409,   414,
     418,   423,   427,   432,   436,   441,   445,   450,   454,   459,
     463,   468,   472,   477,   481,   486,   490,   495,   499,   504,
     508,   513,   517,   522,   526,   531,   535,   540,   544,   549,
     553,   558,   562,   567,   571,   576,   580,   585,   589,   594,
     598,   603,   607,   612,   616,   621,   625,   630,   634,   639,
     643,   648,   652,   657,   661,   666,   671,   680,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   705,   706,   707,   710,   713,   714,   717,   720,
     721,   722,   723,   724,   725,   726,   727,   728,   729,   730,
     731,   732,   733,   734,   735,   736,   737,   738,   739,   740,
     741,   742,   746,   749,   752,   757,   760,   763,   769,   772,
     775,   781,   784,   787,   793,   796,   799,   803,   804,   807,
     808,   809,   810,   811,   815,   818,   821,   827,   830,   833,
     839,   842,   845,   851,   854,   857,   863,   866,   869,   875,
     878,   884,   888,   889,   895,   898,   911,   914,   918,   925,
     928,   932,   939,   940,   946,   949,   953,   960,   961,   962,
     963,   964,   965,   966,   969,   972,   975,   978,   981,   987,
     990,   994,  1001,  1004,  1010,  1013,  1019,  1022,  1028,  1031,
    1035,  1042,  1045,  1051,  1054,  1057,  1061,  1064,  1071,  1072,
    1073,  1074,  1078,  1081,  1085,  1090,  1094,  1102,  1105,  1109,
    1112,  1119,  1122,  1129,  1132,  1136,  1139,  1146,  1149,  1155,
    1158,  1164,  1167,  1173,  1176,  1182,  1185,  1191,  1194,  1200,
    1203,  1209,  1212,  1218,  1221,  1227,  1230,  1236,  1239,  1245,
    1248,  1254,  1257,  1263,  1266,  1272,  1275,  1281,  1284,  1290,
    1293,  1299,  1302,  1308,  1311,  1317,  1320,  1326,  1327,  1328,
    1329,  1330,  1331,  1332,  1333,  1334,  1335,  1336,  1340,  1345,
    1350,  1355,  1360,  1365,  1368,  1371,  1374,  1377,  1383,  1387,
    1390,  1396,  1399,  1405,  1408,  1414,  1417,  1420,  1426,  1429,
    1435,  1436,  1439,  1442,  1449,  1455,  1458,  1461,  1464,  1467,
    1470,  1473,  1476,  1482,  1486,  1493,  1500,  1504,  1512,  1519,
    1522,  1528,  1531,  1537,  1540,  1546,  1549,  1555,  1558,  1564,
    1567,  1573,  1576,  1582,  1585,  1591,  1594,  1600,  1603,  1609,
    1612,  1618,  1621,  1627,  1630
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
  "PART_VRULER", "EVRULER", "PLUG", "PART_PLUG", "EPLUG", "IMAGEVIEW",
  "PART_IMAGEVIEW", "EIMAGEVIEW", "CURVE", "PART_CURVE", "ECURVE", "'>'",
  "':'", "'!'", "$accept", "window", "radio_group_scope", "wlist",
  "widget", "entry", "edit", "tree", "chooser", "text", "button",
  "checkbox", "radiobutton", "progressbar", "list", "table", "combobox",
  "gvim", "pixmap", "calendar", "toolbar", "toolpalette",
  "toolpalettewlist", "toolitemgroup", "toolbarwlist", "toolitem",
  "toolbutton", "toggletoolbutton", "radiotoolbutton", "menutoolbutton",
  "separatortoolitem", "menubar", "menuwlist", "menu", "menuitem",
  "menuitemseparator", "recentchoosermenu", "hseparator", "vseparator",
  "comboboxtext", "comboboxentry", "hscale", "vscale", "spinbutton",
  "timer", "togglebutton", "statusbar", "statusicon", "accellabel",
  "arrow", "hsv", "curve", "hruler", "vruler", "colorbutton", "fontbutton",
  "terminal", "attr", "label", "sensitive", "defaultvalue", "width",
  "height", "input", "output", "variable", "action", "item", "tagattr",
  "imperative", "assignment", "expression", "if", "then", "endif", "while",
  "ewhile", "do", "linkbutton", "spinner", "filechooserbutton", "iconview",
  "cellview", "drawingarea", "imageview", "scalebutton", "volumebutton",
  "recentchooser", "socket", "hscrollbar", "vscrollbar", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-719)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-239)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       2,  -719,    -3,    73,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,    75, 11007,  3389,  -102,   -14,
    -719,   293,    -9,  3389,    -8,  5582,     3,  3389,     7,  5600,
       9,  3389,    10,  -719,   -95,   180,    -3,   -39,   -37,    89,
      -3,   100,   110,   112,   118,    -3,    -3,   128,   129,   130,
      -3,    -3,  3389,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
    -719,    -3,   268,    -3,   185,    -3,  -719,    -3,   108,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,  -719,  -719,
    -719,  -719,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,
    -719,    -3,  -719,    -3,  -719,  -719,    -3,  -719,    -3,    13,
    -719,  -719,  -719,    82,    -3,    81,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,   -41,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -133,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  3704,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,   523,    84,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,   106,   106,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,   140,    86,  -719,   523,  -719,  -719,  3704,  -719,
    -719,  -719,  3704,  -719,  -719,  -719,  3704,  -719,   178,  -719,
     182,    11,    62,   134,   126,    12,   176,   148,   136,   158,
      15,    16,   146,   154,   114,    17,    18,  3704,  3389,    19,
    3389,    20,  3389,    21,  3389,  3998,    22,  -719,   547,    24,
    -719,   293,    25,  4593,    26,  -719,  4901,    27,  5049,    28,
    5214,    29,  5661,    30,  5679,  5706,  5736,  5763,  5781,  5354,
      31,  5372,    32,  5434,    33,  5459,    34,  5497,    35,  5808,
      36,  5846,  5525,    37,  5871,    38,   -92,   -87,  -719,    39,
    -719,    40,  5891,    41,  5916,    42,  5973,    43,  5991,    44,
    6053,    45,  6073,    46,  6118,    48,  6138,    49,  6183,    50,
    6201,    51,  6228,    54,  3389,    55,  3389,    57,  3389,    70,
    3389,    72,  3389,    74,  3389,    76,  3389,    78,  -719,   986,
      80,  6296,    83,  6320,    85,  6353,    87,  3389,    88,  6398,
      91,  3389,    92,  6420,    93,  6483,    95,  6503,    97,  3389,
      99,  6548,   101,  6566,   103,  6628,   105,  6651,   107,  6713,
     111,  6733,   113,  3389,   115,  3389,   116,  3389,   119,  3389,
     120,  -719,  -146,   121,  6758,   122,  6778,   123,  5232,   124,
    5067,   125,  4923,   127,  3389,   131,  4481,   133,  4253,   135,
    4079,   137,  4034,   138,  1945,   139,  1390,   141,  -719,    -3,
    -719,    -3,  -719,    -3,  -719,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,    -3,  -719,
      -3,  -719,    -3,  -719,    -3,  -719,  1183,  -719,  -719,    -3,
    -719,    -3,   204,  -719,    -3,   293,    -3,  -719,  -719,   132,
     144,  -719,   174,  3389,  3703,   293,  6823,  3389,  6843,  6888,
    3389,  6912,  2557,  3389,  -719,  -719,   186,  -719,  -719,  -719,
     187,  -719,  -719,  -719,  -719,   188,   117,  -719,  -719,  -719,
     190,   194,  3704,  -719,  3704,  -719,  3704,  -719,  3704,  -719,
    -719,  -719,  4124,   293,   523,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,   162,   160,   195,
     200,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,  -719,    -3,  -719,
      -3,  -719,    -3,    60,    -3,  -719,    -3,   -15,    -3,   986,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  3704,  -719,  -719,  -719,  3704,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  3704,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,   -23,    -3,  -146,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  3704,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  3389,   142,  3389,   143,
    3389,   145,  3389,  3389,   147,  3389,   149,  3389,   150,  3389,
     151,  3389,   153,  3389,   155,  3389,   157,  3389,   159,  3389,
     161,  3389,   163,  3389,   164,  3389,   165,  3389,   166,  3389,
     167,  3389,   168,  -719,   293,   169,  4611,   170,  -719,  6986,
     171,   547,   172,   106,   106,   106,   106,   106,  -719,   205,
    -719,  -719,  3704,  -719,   523,  -719,  3704,  -719,  -719,  3704,
    -719,  -719,  3704,   212,   191,   198,  -719,   199,   197,   196,
     741,  3389,  1461,  3389,  1750,  3389,  2660,  4016,  -719,   547,
    4328,   293,  4691,  4982,  5152,  5295,  7004,  7049,  7094,  7114,
    7159,  7185,  7216,  7261,  7286,   106,  -719,  -719,  -719,  7306,
    7331,  7351,  7408,  7426,  7483,  7508,  7576,  7600,  7659,  7685,
    7716,  3389,  7775,  3389,  7799,  3389,  7837,  3389,  7861,  3389,
    7924,  3389,  7944,  3389,  8007,   173,  8068,   175,  8086,   177,
    -719,   293,   179,  8104,   181,  -719,  3389,   183,  -719,  -719,
    -719,  -719,  -719,  -719,  8131,   986,  8163,  8194,  8220,  8253,
    3389,  8285,  8314,  3389,  8350,  8376,  8448,  8466,  3389,  8523,
    8545,  8568,  8606,  8629,  8663,  8686,  3389,  8751,  3389,  8774,
    3389,  8837,  3389,  -719,   986,   184,  -719,  8894,  -146,  8912,
    8957,  5277,  5129,  4946,  4673,  3389,  4499,  4346,  4097,  4061,
    2535,  1535,  3704,  -719,  3704,  -719,  3704,  -719,  3704,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,  -719,   523,
    -719,  -719,  -719,  -719,  -719,  4181,   293,   156,   104,   104,
     -64,   -64,  3389,   106,  3389,  1281,  4575,  8980,  9039,  3073,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  -719,  4208,  -719,   523,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
     152,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,  -719,  -719,  -719,
    -719,  -719,   523,  -719,  -719,  -719,  3704,  -719,  -719,   986,
    -719,  -719,  -719,  -719,  3704,  -719,  -719,  3704,  -719,  -719,
    -719,  -719,  3704,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    3704,  -719,  3704,  -719,  3704,  -719,  3704,   986,  -719,  -719,
    -146,  -719,  -719,  -719,  -719,  -719,  -719,  3704,  -719,  -719,
    -719,  -719,  -719,  -719,  1211,  3389,  1603,  3389,  2013,  3389,
    3388,  9057,  3389,  9120,  3389,  9165,  3389,  9194,  3389,  9226,
    3389,  9256,  3389,  9285,  3389,  9330,  3389,  9357,  3389,  9393,
    3389,  9418,  3389,  9463,  3389,  9489,  3389,  9552,  3389,  4709,
    3389,  4373,   293,  4785,  9575,  -719,   547,  2759,   156,  3074,
    -719,  -719,  -719,  -719,  -719,  1304,  1625,  2440,  -719,  4436,
    9620,  9643,  9665,  9734,  9754,  9772,  9799,  9867,  9890,  9912,
    9969,   293, 10001, 10028,  3389, 10046, 10073, 10135, 10161, 10220,
   10238, 10283, 10301, 10358,   986, 10376,  4803,  -719,  3704,  -719,
    3704,  -719,  3704,  -719,  -719,  3704,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,  -719,  3704,
    -719,  3704,  -719,  3704,  -719,   523,  -719,  -719,  4226,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,   523,  -719,
    -719,  3704,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,   986,  -719,  -719,  1366,  1670,  2458, 10433, 10466, 10496,
   10573, 10596, 10630, 10653, 10691, 10716, 10754, 10777, 10822, 10845,
   10867,  4821,  4463,  -719, 10924, 10962, 10985,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
     297,    18,     0,    18,     0,    18,     0,   297,     0,    18,
       0,   297,     0,    18,     0,     0,    18,     0,     0,     0,
      13,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   300,   299,   298,   301,   302,   303,   304,
     305,   306,   307,    18,     0,    18,     0,    18,     0,    18,
     297,     0,     0,     0,    18,     0,   297,     0,     0,   297,
       0,   297,     0,   297,     0,   297,     0,   297,   297,   297,
     297,   297,   297,     0,   297,     0,   297,     0,   297,     0,
     297,     0,   297,     0,   297,   297,     0,   297,     0,     0,
     343,   346,   331,     0,     0,     0,     0,   297,     0,   297,
       0,   297,     0,   297,     0,   297,     0,   297,     0,   297,
       0,   297,     0,   297,     0,   297,     0,   297,     0,    18,
       0,    18,     0,    18,     0,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,   297,     0,   297,     0,   297,
       0,    18,     0,   297,     0,    18,     0,   297,     0,   297,
       0,   297,     0,    18,     0,   297,     0,   297,     0,   297,
       0,   297,     0,   297,     0,   297,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    18,     0,   297,     0,   297,
       0,   297,     0,   297,     0,   297,     0,    18,     0,   297,
       0,   297,     0,   297,     0,   297,     0,   297,     0,   297,
       0,   297,    19,    98,    99,   100,   101,    97,   102,   103,
     104,   105,   106,   107,   108,   110,   109,   132,   130,   131,
     112,     0,   238,   239,   240,   241,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   143,   144,   145,   146,
     147,   148,   149,   123,   126,   129,    21,     0,     0,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   111,
     150,   151,     0,     0,    18,   297,   238,    18,   297,    18,
       7,   297,   297,    18,    11,   297,   297,    18,     0,   326,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   233,   297,     0,
     242,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   257,     0,
     259,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   196,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   199,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
      18,     0,    18,     0,    18,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    18,     0,    18,     0,    18,
       0,    18,     0,    18,     0,    20,     0,    22,    18,     0,
     297,     0,     0,   297,     0,     0,     0,   335,   336,     0,
       0,   328,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   308,   325,     0,   310,   309,   321,
       0,   311,   312,   313,   315,     0,     0,   318,   319,   323,
       0,     0,   297,    18,   297,    18,   297,    18,   297,   154,
     152,   297,     0,     0,   297,    18,   247,   297,   157,   155,
     297,   160,   158,   297,   163,   161,   297,   167,   297,   169,
     170,   171,   173,   172,   176,   174,   297,   179,   177,   297,
     182,   180,   297,   185,   183,   297,   188,   186,   297,   189,
     297,   191,   166,   164,   297,   192,   297,     0,     0,     0,
       0,   261,   297,   263,   297,   265,   297,   267,   297,   269,
     297,   271,   297,   273,   297,   275,   297,   291,   297,   293,
     297,   295,   297,   297,    18,   297,    18,   297,    18,   297,
      18,   297,    18,   297,    18,   297,    18,   297,     0,   297,
       0,   297,     0,    18,     0,   297,     0,    18,     0,   297,
     208,   207,   209,   210,   211,   212,    18,   194,   297,   349,
     297,   351,   297,   297,    18,   353,   297,   297,    18,   355,
     297,   363,   297,   365,   297,   297,    18,   367,   297,   253,
     297,   277,   297,   279,   297,   281,   297,   283,   297,   297,
      18,   297,    18,   297,    18,   297,    18,    18,     0,   297,
     202,    18,   357,   297,   359,   297,   369,   297,   371,   297,
     373,   297,   297,    18,   124,   297,   127,   297,   287,   297,
     289,   297,   361,   297,   285,   297,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     0,     0,     0,     0,   252,     0,
       0,   297,     0,     0,     0,     0,     0,     0,   344,     0,
     348,   329,   297,    14,   297,     5,   297,     8,     9,   297,
      12,    16,   297,     0,     0,     0,   317,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   234,   297,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   330,   258,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     228,     0,     0,     0,     0,   219,     0,     0,   214,   213,
     215,   216,   217,   218,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   204,     0,     0,   203,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   297,    18,   297,    18,   297,    18,   297,   297,
      18,   297,    18,   297,    18,   297,    18,   297,    18,   297,
      18,   297,    18,   297,    18,   297,    18,   297,    18,   297,
      18,   297,    18,   297,    18,   297,    18,   297,    18,   297,
      18,   248,   297,   254,   297,     0,     0,   341,   338,   337,
     339,   340,     0,     0,     0,     0,     0,     0,     0,     0,
     327,   322,   314,   316,   324,   320,    55,   297,    59,   297,
      91,   297,    95,   153,     0,   243,   297,   249,   156,   159,
     162,   168,   175,   178,   181,   184,   187,   190,   165,   193,
     334,   262,   264,   266,   268,   270,   272,   274,   276,   292,
     294,   296,    79,   297,    83,   297,    71,   297,    75,   297,
      23,   297,    63,   297,    67,   297,   222,   297,   224,   297,
     226,   297,   297,    18,   231,   297,   297,    18,   197,   297,
     195,   350,   352,    31,   297,   354,    87,   297,   356,   364,
     366,    27,   297,   368,   255,   278,   280,   282,   284,    35,
     297,    39,   297,    43,   297,    51,   297,   297,    18,   200,
     297,   358,   360,   370,   372,   374,    47,   297,   125,   128,
     288,   290,   362,   286,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   235,   297,     0,   342,     0,
       4,    15,     6,    10,    17,     0,     0,     0,   236,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    56,   297,    60,
     297,    92,   297,    96,    80,   297,    84,   297,    72,   297,
      76,   297,    24,   297,    64,   297,    68,   297,    32,   297,
      88,   297,    28,   297,    36,   297,    40,   297,    44,   297,
      52,   297,    48,   297,   244,   297,   250,   256,     0,   345,
     332,   347,   333,    57,    61,    93,   245,    81,    85,    73,
      77,    25,    65,    69,   223,   225,   227,   229,   297,   232,
     220,   297,   198,    33,    89,    29,    37,    41,    45,    53,
     205,   297,   201,    49,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   237,     0,     0,     0,    58,    62,    94,
      82,    86,    74,    78,    26,    66,    70,    34,    90,    30,
      38,    42,    46,    54,    50,   246,   230,   221,   206
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -719,  -719,   854,   -13,   929,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -660,  -718,  -694,  -666,  -658,  -657,  -649,  -647,
    -645,  -719,   -12,    -4,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,    -7,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  2211,  1367,
    -719,  -252,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,  -719,
    -719
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    15,    17,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   719,   720,   669,   670,   671,   672,   673,   674,
     675,   240,   241,   242,   243,   244,   245,   246,   247,   248,
     249,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,    16,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    19,   266,
     367,   529,   267,   992,  1240,   268,  1242,   994,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     281
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,   926,   283,   888,    29,     1,     2,   283,   283,   285,
     288,   889,   890,    18,   292,   282,   530,   286,   296,   283,
     891,   298,   892,   283,   893,   283,   283,   283,   283,     3,
       4,   283,   283,   283,   283,   283,   283,   283,   283,   317,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     283,   283,   283,   325,   283,   283,   283,   283,   328,   333,
     283,   283,   336,   283,   338,    33,   340,   302,   342,   303,
     344,   345,   346,   347,   348,   349,   283,   351,   283,   353,
     283,   355,   283,   357,   283,   359,   283,   361,   362,   283,
     364,   283,    20,   283,   283,   525,   526,   283,   283,   283,
     372,   283,   374,   283,   376,   283,   378,   283,   380,   283,
     382,   283,   384,   283,   386,   366,   388,   283,   390,   283,
     392,   283,   283,   717,   718,   283,   283,   283,   283,   283,
     283,   283,   335,   283,   408,   451,   547,   283,   411,   283,
     413,   283,   415,   283,   283,   283,   419,   283,   283,   283,
     423,   283,   425,   283,   427,   283,   283,   283,   431,   283,
     433,   283,   435,   283,   437,   283,   439,   283,   441,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   283,   283,
     454,   283,   456,   283,   458,   283,   460,   283,   462,   283,
     283,  1069,   466,   532,   468,   304,   470,   368,   472,   370,
     474,   330,   476,   816,   516,   549,   306,   559,   527,   528,
     548,     5,     6,   544,   796,   797,   307,   299,   308,   545,
    1097,   617,   553,   817,   309,   618,     7,     8,   788,     9,
      10,   552,    11,    12,   312,   313,   314,   557,   885,   793,
     794,   795,   796,   797,   554,   558,   531,   880,   923,  1000,
     799,   793,   794,   795,   796,   797,   846,   551,  1100,   793,
     794,   795,   796,   797,   794,   795,   796,   797,   534,   845,
    1001,   536,     0,     0,   538,   539,     0,     0,   541,   542,
     801,   327,    74,    75,  1002,  1003,   300,  1005,    76,    77,
    1004,    78,   813,   814,   815,   562,   818,   564,   284,   566,
     819,   568,     0,   287,   289,    13,    14,    74,    75,   574,
     847,   572,   993,    76,    77,   291,    78,   286,   848,   293,
       0,   295,   297,   546,   550,     0,     0,   555,   556,   560,
     561,   563,   565,   567,   571,     0,   573,   575,   577,   580,
     583,   586,   588,   596,   599,   602,   605,   608,   610,   614,
     616,   619,   620,   622,   624,   626,   628,   630,   632,     0,
     634,   636,   638,   640,     0,     0,   642,   644,     0,   646,
       0,   643,   926,   645,     0,   647,     0,   649,     0,   651,
       0,   653,   648,   655,   650,     0,   652,     0,   654,     0,
     656,     0,   676,   888,   683,   678,     0,   680,   687,   682,
     684,   889,   890,   686,   688,   690,   695,   692,     0,   694,
     891,   696,   892,   698,   893,   700,     0,   702,   799,   704,
     709,   888,   711,   706,   713,   708,   715,   710,   712,   889,
     890,   714,   716,   721,   723,   725,   727,   729,   891,   731,
     892,   732,   893,   733,   798,   735,   799,   737,     0,   739,
     741,   743,     0,   745,   943,   945,   800,   947,   799,   950,
       0,   952,   954,   956,     0,   958,   799,   960,     0,   962,
     799,   964,     0,   966,     0,   968,   970,   972,   974,   976,
     978,   980,   982,   984,   986,  1057,     0,  1059,     0,  1061,
    1271,  1063,     0,  1065,     0,  1067,  1098,     0,     0,     0,
       0,     0,     0,   786,   177,   178,   789,     0,     0,     0,
     802,   791,     0,   804,   806,     0,     0,   809,     0,     0,
     812,   286,     0,     0,     0,     0,     0,     0,     0,   177,
     178,   987,   988,   989,   990,   991,     0,   518,   519,     0,
       0,     0,     0,   520,   521,   820,   522,   822,     0,   824,
       0,   826,     0,     0,   827,     0,     0,   830,     0,   829,
     832,  -238,  -238,   833,     0,     0,   834,  -238,  -238,   835,
    -238,   836,     0,     0,     0,     0,     0,     0,     0,   837,
       0,     0,   838,  1030,     0,   839,     0,     0,   840,     0,
       0,   841,     0,   842,     0,   888,     0,   843,     0,   844,
       0,     0,     0,   889,   890,   849,     0,   850,     0,   851,
       0,   852,   891,   853,   892,   854,   893,   855,     0,   856,
       0,   857,     0,   858,     0,   859,   860,     0,   862,     0,
     864,     0,   866,     0,   868,     0,   870,     0,   872,     0,
     874,     0,   876,     0,   878,     0,     0,     0,   883,     0,
       0,     0,   894,     0,     0,     0,     0,     0,     0,     0,
       0,   896,     0,   897,     0,   898,   899,     0,     0,   901,
     902,     0,     0,   904,     0,   905,     0,   906,   907,     0,
       0,   909,     0,   910,     0,   911,     0,   912,     0,   913,
       0,   914,   915,     0,   917,     0,   919,     0,   921,     0,
       0,     0,   927,     0,     0,     0,   929,     0,   930,     0,
     931,     0,   932,     0,   933,   934,     0,     0,   936,     0,
     937,     0,   938,   942,   939,   944,   940,   946,   941,   948,
     949,  1158,   951,     0,   953,     0,   955,     0,   957,  1006,
     959,     0,   961,     0,   963,     0,   965,     0,   967,     0,
     969,     0,   971,     0,   973,     0,   975,     0,   977,   523,
     524,     0,   979,     0,     0,     0,     0,     0,     0,     0,
     286,     0,     0,     0,   985,    34,     0,    35,     0,    36,
       0,     0,     0,  -238,  -238,   995,     0,   996,     0,   997,
       0,     0,   998,     0,     0,   999,     0,     0,  1007,     0,
    1009,     0,  1011,     0,     0,     0,     0,     0,     0,  1016,
       0,     0,  1014,     0,    37,     0,    38,   286,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,  1043,     0,
    1045,     0,  1047,     0,  1049,     0,  1051,    21,  1053,    23,
    1055,     0,     0,    27,     0,     0,     0,    31,     0,  1062,
      52,     0,     0,  1066,     0,     0,     0,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1074,     0,     0,
    1077,     0,     0,     0,     0,  1082,     0,     0,     0,     0,
       0,     0,     0,  1090,     0,  1092,     0,  1094,     0,  1096,
       0,     0,     0,     0,     0,     0,     0,   318,     0,   320,
       0,   322,  1107,   324,     0,     0,     0,     0,   331,     0,
       0,     0,     0,     0,     0,  1114,     0,  1116,     0,  1118,
       0,  1120,  1121,     0,  1123,     0,  1125,     0,  1127,     0,
    1129,     0,  1131,     0,  1133,     0,  1135,     0,  1137,     0,
    1139,     0,  1141,     0,  1143,     0,  1145,     0,  1147,     0,
    1149,     0,  1151,     0,     0,  1153,     0,  1154,     0,  1157,
       0,  1159,  1156,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,     0,   396,     0,   398,     0,   400,
    1165,   402,  1166,   404,  1167,   406,     0,   409,     0,  1169,
       0,     0,     0,     0,     0,   417,     0,     0,     0,   421,
       0,     0,     0,     0,     0,     0,     0,   429,     0,    51,
       0,     0,     0,     0,     0,     0,  1170,     0,  1171,     0,
    1172,   443,  1173,   445,  1174,   447,  1175,   449,  1176,   452,
    1177,     0,  1178,     0,  1179,  1180,     0,     0,  1182,  1183,
       0,   464,  1185,     0,     0,     0,     0,  1186,     0,     0,
    1187,     0,     0,     0,     0,  1188,     0,     0,     0,     0,
       0,     0,     0,  1189,     0,  1190,     0,  1191,     0,  1192,
    1193,     0,     0,  1195,     0,     0,     0,     0,     0,     0,
    1196,     0,  1198,     0,  1200,     0,  1202,     0,     0,  1205,
       0,  1207,     0,  1209,     0,  1211,     0,  1213,     0,  1215,
       0,  1217,     0,  1219,     0,  1221,     0,  1223,     0,  1225,
       0,  1227,     0,  1229,     0,  1231,     0,  1233,   533,     0,
    1235,   535,     0,   537,     0,     0,     0,   540,   286,  1238,
     515,   543,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1258,
       0,  1261,   657,   658,     0,   659,   660,   286,   661,   662,
       0,   663,   664,     0,   665,   666,     0,     0,   783,     0,
       0,  1274,     0,  1275,     0,  1276,     0,     0,  1277,     0,
    1278,     0,  1279,     0,  1280,     0,  1281,     0,  1282,     0,
    1283,     0,  1284,     0,  1285,     0,  1286,   515,  1287,  1197,
    1288,   515,  1289,     0,  1290,   515,  1291,    34,  1292,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   515,   667,   668,     0,
       0,  1294,     0,     0,  1295,    34,     0,    35,     0,    36,
       0,     0,     0,     0,  1296,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,  1160,    50,     0,     0,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,  1243,    49,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,   746,     0,   748,     0,   750,     0,   752,   753,
       0,   755,     0,   757,     0,   759,     0,   761,    34,   763,
      35,   765,    36,   767,     0,   769,     0,   771,     0,   773,
       0,   775,     0,   777,    37,   779,    38,   781,    39,    40,
       0,    41,   784,    42,  1297,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
      34,     0,    35,     0,    36,     0,     0,   821,     0,   823,
       0,   825,     0,     0,     0,     0,     0,     0,     0,   831,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,    51,  1008,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,   515,    49,   515,    50,   515,     0,   515,   861,    51,
     863,     0,   865,     0,   867,    34,   869,    35,   871,    36,
     873,     0,     0,     0,     0,     0,     0,   881,     0,     0,
       0,   886,     0,     0,     0,     0,     0,     0,     0,     0,
     895,     0,     0,     0,     0,     0,     0,     0,   900,     0,
       0,     0,   903,     0,    37,     0,    38,     0,    39,    40,
     908,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,   916,    50,   918,     0,   920,    51,
     922,   924,   515,     0,   515,   928,   515,     0,   515,    34,
     515,    35,   515,    36,   515,     0,     0,   935,   517,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   515,     0,  1199,     0,   515,     0,    37,     0,
      38,     0,    39,    40,   515,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,  1244,    49,   515,    50,
     515,     0,   515,     0,   515,     0,     0,    34,     0,    35,
       0,    36,     0,     0,    51,   517,     0,     0,     0,   517,
       0,   515,     0,   517,     0,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,    51,     0,
       0,  1298,     0,     0,   517,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,    37,     0,
      38,   744,    39,    40,    34,    41,    35,    42,    36,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,   515,     0,     0,     0,   515,     0,     0,   515,     0,
       0,   515,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,  1010,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,    35,  1115,    36,  1117,
       0,  1119,     0,     0,  1122,     0,  1124,     0,  1126,     0,
    1128,     0,  1130,     0,  1132,     0,  1134,     0,  1136,     0,
    1138,     0,  1140,    51,  1142,     0,  1144,     0,  1146,     0,
    1148,     0,  1150,    37,  1152,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,  1113,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   515,     0,   515,     0,   515,     0,   515,   515,     0,
     515,     0,   515,     0,   515,     0,   515,     0,   515,     0,
     515,    51,   515,     0,   515,     0,   515,     0,   515,     0,
     515,     0,   515,     0,   515,     0,   515,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,  1181,     0,     0,
       0,  1184,     0,     0,     0,     0,     0,     0,     0,   517,
       0,   517,     0,   517,     0,   517,   515,     0,   515,     0,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1194,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   515,     0,   515,     0,   515,     0,   515,     0,
     515,     0,   515,     0,   515,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,   515,     0,     0,     0,     0,
       0,     0,     0,   515,     0,     0,   515,     0,     0,     0,
     517,   515,   517,     0,   517,     0,   517,     0,   517,   515,
     517,   515,   517,   515,     0,   515,  1201,     0,    37,     0,
      38,     0,    39,    40,     0,    41,   515,    42,    51,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
     517,     0,     0,     0,   517,     0,     0,    34,     0,    35,
       0,    36,   517,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   517,     0,   517,     0,
     517,     0,   517,     0,     0,     0,   515,     0,   515,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,   517,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   515,     0,   515,
       0,   515,     0,     0,   515,     0,   515,     0,   515,     0,
     515,     0,   515,     0,   515,     0,   515,     0,   515,     0,
     515,     0,   515,     0,   515,     0,   515,     0,   515,     0,
     515,     0,   515,     0,     0,     0,     0,     0,     0,   517,
       0,     0,     0,   517,     0,     0,   517,     0,     0,   517,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     515,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,     0,    24,     0,    26,
       0,    28,     0,    30,     0,    32,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   301,     0,     0,
       0,   305,     0,     0,     0,     0,   310,   311,     0,     0,
       0,   315,   316,   742,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   319,     0,   321,     0,   323,
       0,     0,   326,     0,   329,     0,   332,     0,   334,     0,
       0,   337,     0,   339,     0,   341,     0,   343,     0,     0,
       0,    51,     0,     0,   350,     0,   352,     0,   354,   517,
     356,   517,   358,   517,   360,   517,   517,   363,   517,   365,
     517,     0,   517,     0,   517,   369,   517,   371,   517,   373,
     517,   375,   517,   377,   517,   379,   517,   381,   517,   383,
     517,   385,   517,   387,   517,   389,     0,   391,     0,   393,
       0,   395,     0,   397,     0,   399,     0,   401,     0,   403,
       0,   405,     0,   407,     0,   410,     0,   412,     0,   414,
       0,   416,     0,   418,   517,   420,   517,   422,   517,   424,
       0,   426,     0,   428,     0,   430,     0,   432,     0,   434,
       0,   436,     0,   438,     0,   440,     0,   442,     0,   444,
       0,   446,     0,   448,     0,   450,     0,   453,     0,   455,
     517,   457,   517,   459,   517,   461,   517,   463,   517,   465,
     517,   467,   517,   469,     0,   471,     0,   473,     0,   475,
       0,   477,     0,   517,     0,     0,     0,     0,     0,     0,
       0,   517,     0,     0,   517,     0,     0,     0,     0,   517,
       0,     0,     0,  1245,     0,     0,     0,   517,     0,   517,
       0,   517,     0,   517,     0,     0,     0,     0,     0,     0,
       0,  1299,     0,     0,   517,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,   517,    38,   517,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,    37,    49,    38,    50,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,   517,     0,   517,     0,   517,
       0,     0,   517,     0,   517,     0,   517,     0,   517,    34,
     517,    35,   517,    36,   517,     0,   517,     0,   517,     0,
     517,     0,   517,     0,   517,     0,   517,     0,   517,     0,
     517,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,   517,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1012,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     747,     0,   749,     0,   751,     0,     0,   754,     0,   756,
       0,   758,     0,   760,    34,   762,    35,   764,    36,   766,
       0,   768,     0,   770,     0,   772,     0,   774,     0,   776,
       0,   778,     0,   780,     0,   782,     0,     0,    51,     0,
     785,     0,   787,     0,     0,   790,     0,   792,     0,     0,
       0,     0,     0,    37,     0,    38,    51,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,   478,   479,     0,   480,   481,
       0,   482,     0,   483,   484,     0,     0,    70,     0,    71,
      72,    73,     0,    74,    75,     0,     0,     0,     0,    76,
      77,     0,    78,     0,    79,    80,     0,    81,    82,     0,
      83,    84,     0,     0,     0,     0,     0,     0,    85,    86,
       0,    87,    88,    89,    90,    91,    92,     0,    93,    94,
       0,    95,    96,    51,    97,    98,    99,     0,   100,     0,
     101,   102,   103,     0,   104,     0,   105,   106,     0,   107,
     108,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,  1112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   109,     0,   110,  1239,   111,   875,
     112,   877,   811,   879,     0,   882,     0,   884,     0,   887,
       0,     0,   113,   114,     0,   115,   116,     0,   117,   118,
       0,   119,   120,     0,   121,   122,     0,   123,   124,     0,
     125,   126,     0,   127,   128,     0,   129,   130,     0,   131,
     132,     0,   133,   134,     0,   135,   136,     0,   137,   138,
       0,   485,   486,     0,   487,   488,     0,   489,   490,   925,
     491,   492,     0,   493,   494,     0,   495,   496,     0,   497,
     498,     0,   153,   154,     0,     0,     0,     0,    51,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,   156,     0,   157,   158,     0,   159,   160,     0,   499,
     500,     0,   163,   164,     0,   501,   502,     0,     0,     0,
       0,   167,   168,     0,   169,   170,     0,   171,   172,     0,
     503,   504,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,   176,     0,   177,   178,     0,   179,   180,
       0,   181,   182,     0,   183,   184,     0,   185,   186,     0,
       0,     0,     0,   505,   506,     0,   507,   508,     0,   509,
     510,     0,   511,   512,     0,   195,   196,     0,     0,     0,
       0,   197,   198,     0,   199,   200,     0,     0,   201,   202,
       0,   203,   204,     0,   205,   206,     0,   513,   514,     0,
     209,   210,     0,   211,   212,     0,   213,   214,     0,   215,
     216,     0,     0,     0,     0,   217,   218,     0,   219,   220,
     478,   479,     0,   480,   481,     0,   482,     0,   483,   484,
       0,     0,    70,     0,    71,    72,    73,     0,    74,    75,
       0,     0,     0,     0,    76,    77,     0,    78,     0,    79,
      80,     0,    81,    82,     0,    83,    84,    34,     0,    35,
       0,    36,     0,    85,    86,     0,    87,    88,    89,    90,
      91,    92,     0,    93,    94,     0,    95,    96,     0,    97,
      98,    99,     0,   100,     0,   101,   102,   103,     0,   104,
       0,   105,   106,     0,   107,   108,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,   109,
       0,   110,     0,   111,  1241,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   113,   114,     0,
     115,   116,     0,   117,   118,     0,   119,   120,     0,   121,
     122,     0,   123,   124,     0,   125,   126,     0,   127,   128,
       0,   129,   130,     0,   131,   132,     0,   133,   134,     0,
     135,   136,     0,   137,   138,     0,   485,   486,     0,   487,
     488,     0,   489,   490,     0,   491,   492,     0,   493,   494,
       0,   495,   496,     0,   497,   498,     0,   153,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   156,     0,   157,   158,
       0,   159,   160,     0,   499,   500,     0,   163,   164,     0,
     501,   502,     0,     0,     0,     0,   167,   168,     0,   169,
     170,     0,   171,   172,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,   176,     0,
     177,   178,     0,   179,   180,     0,   181,   182,     0,   183,
     184,     0,   185,   186,     0,     0,     0,     0,   505,   506,
       0,   507,   508,     0,   509,   510,     0,   511,   512,     0,
     195,   196,     0,     0,     0,     0,   197,   198,     0,   199,
     200,    51,     0,   201,   202,     0,   203,   204,     0,   205,
     206,     0,   513,   514,     0,   209,   210,     0,   211,   212,
       0,   213,   214,     0,   215,   216,     0,     0,  1164,     0,
     217,   218,     0,   219,   220,    63,    64,     0,    65,    66,
       0,    67,     0,    68,    69,  1203,     0,    70,     0,    71,
      72,    73,     0,    74,    75,     0,     0,     0,     0,    76,
      77,     0,    78,     0,    79,    80,     0,    81,    82,     0,
      83,    84,    34,     0,    35,     0,    36,     0,    85,    86,
       0,    87,    88,    89,    90,    91,    92,     0,    93,    94,
       0,    95,    96,     0,    97,    98,    99,     0,   100,     0,
     101,   102,   103,     0,   104,     0,   105,   106,     0,   107,
     108,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,   109,     0,   110,     0,   111,     0,
     112,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,   114,     0,   115,   116,     0,   117,   118,
       0,   119,   120,     0,   121,   122,     0,   123,   124,     0,
     125,   126,     0,   127,   128,     0,   129,   130,     0,   131,
     132,     0,   133,   134,     0,   135,   136,     0,   137,   138,
       0,   139,   140,     0,   141,   142,     0,   143,   144,     0,
     145,   146,     0,   147,   148,     0,   149,   150,     0,   151,
     152,     0,   153,   154,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     155,   156,     0,   157,   158,     0,   159,   160,     0,   161,
     162,     0,   163,   164,     0,   165,   166,     0,     0,     0,
       0,   167,   168,     0,   169,   170,     0,   171,   172,     0,
     173,   174,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,   176,     0,   177,   178,     0,   179,   180,
       0,   181,   182,     0,   183,   184,     0,   185,   186,     0,
       0,     0,     0,   187,   188,     0,   189,   190,     0,   191,
     192,     0,   193,   194,     0,   195,   196,     0,     0,     0,
       0,   197,   198,     0,   199,   200,    51,     0,   201,   202,
       0,   203,   204,     0,   205,   206,     0,   207,   208,     0,
     209,   210,     0,   211,   212,     0,   213,   214,     0,   215,
     216,     0,     0,     0,     0,   217,   218,     0,   219,   220,
     478,   479,     0,   480,   481,     0,   482,     0,   483,   484,
       0,     0,    70,     0,    71,    72,    73,     0,    74,    75,
       0,     0,   803,     0,    76,    77,     0,    78,     0,    79,
      80,     0,    81,    82,     0,    83,    84,    34,     0,    35,
       0,    36,     0,    85,    86,     0,    87,    88,    89,    90,
      91,    92,     0,    93,    94,     0,    95,    96,     0,    97,
      98,    99,     0,   100,     0,   101,   102,   103,     0,   104,
       0,   105,   106,     0,   107,   108,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,   109,
       0,   110,     0,   111,     0,   112,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   113,   114,     0,
     115,   116,     0,   117,   118,     0,   119,   120,     0,   121,
     122,     0,   123,   124,     0,   125,   126,     0,   127,   128,
       0,   129,   130,     0,   131,   132,     0,   133,   134,     0,
     135,   136,     0,   137,   138,     0,   485,   486,     0,   487,
     488,     0,   489,   490,     0,   491,   492,     0,   493,   494,
       0,   495,   496,     0,   497,   498,     0,   153,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   155,   156,     0,   157,   158,
       0,   159,   160,     0,   499,   500,     0,   163,   164,     0,
     501,   502,     0,     0,     0,     0,   167,   168,     0,   169,
     170,     0,   171,   172,     0,   503,   504,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,   176,     0,
     177,   178,     0,   179,   180,     0,   181,   182,     0,   183,
     184,     0,   185,   186,     0,     0,     0,     0,   505,   506,
       0,   507,   508,     0,   509,   510,     0,   511,   512,     0,
     195,   196,     0,     0,     0,     0,   197,   198,     0,   199,
     200,    51,     0,   201,   202,     0,   203,   204,     0,   205,
     206,     0,   513,   514,     0,   209,   210,     0,   211,   212,
       0,   213,   214,     0,   215,   216,   569,   570,     0,     0,
     217,   218,     0,   219,   220,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1013,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
      35,    37,    36,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,    37,    49,    38,
      50,    39,    40,    34,    41,    35,    42,    36,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,    34,     0,    35,    37,    36,    38,   828,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    37,    49,    38,    50,    39,    40,    34,    41,
      35,    42,    36,    43,    44,     0,    45,    46,    47,    48,
      37,    49,    38,    50,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,  1155,     0,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    34,    49,    35,    50,    36,
       0,  1168,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1293,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,    51,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,    51,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,    51,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,   740,    43,    44,    51,
      45,    46,    47,    48,  1015,    49,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     0,    34,  1111,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,   738,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,  1234,
       0,     0,     0,     0,     0,     0,  1110,     0,     0,     0,
       0,    37,    51,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,  1246,    41,     0,    42,     0,    43,    44,    51,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,  1315,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,    51,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,    51,     0,    34,     0,    35,    37,    36,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,   736,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    37,    49,    38,    50,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,  1161,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,    34,
       0,    35,     0,    36,     0,   576,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,    34,     0,    35,
       0,    36,     0,   981,     0,     0,     0,     0,     0,     0,
       0,     0,  1109,     0,     0,    34,     0,    35,    37,    36,
      38,    51,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,    37,    49,    38,    50,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,    34,     0,    35,
       0,    36,     0,  1017,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,    34,     0,    35,    37,    36,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,    51,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,   734,    43,    44,    51,    45,    46,
      47,    48,    37,    49,    38,    50,    39,    40,     0,    41,
       0,    42,  1108,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,     0,  1236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,    34,     0,    35,    37,    36,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,    51,    45,    46,    47,    48,    37,    49,    38,    50,
      39,    40,     0,    41,     0,    42,     0,    43,    44,    51,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   578,     0,   579,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,  1106,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,    51,    45,    46,
      47,    48,     0,    49,     0,    50,    37,     0,    38,  1232,
      39,    40,     0,    41,     0,    42,     0,    43,    44,  1018,
      45,    46,    47,    48,     0,    49,    34,    50,    35,    37,
      36,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,    38,     0,    39,
      40,     0,    41,    51,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,   581,     0,   582,
       0,    51,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,  1273,     0,     0,     0,     0,     0,    51,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,  1314,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      37,    49,    38,    50,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,  1019,     0,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    37,     0,    38,     0,    39,    40,     0,    41,
     730,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,    51,    37,     0,    38,     0,    39,
      40,     0,    41,  1105,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,   584,    50,   585,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    37,    49,    38,    50,    39,
      40,    34,    41,    35,    42,    36,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,    51,  1020,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
      37,   728,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,    37,    49,
      38,    50,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,    34,    50,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,   594,   595,     0,     0,     0,    34,    51,    35,     0,
      36,     0,     0,  1104,     0,     0,     0,     0,     0,     0,
       0,     0,   597,   598,     0,     0,     0,    37,     0,    38,
      51,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    37,    49,    38,    50,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   600,   601,     0,
       0,     0,    51,    34,     0,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
      51,    39,    40,   726,    41,   603,    42,   604,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,    34,    37,    35,    38,    36,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,    51,   606,   607,  1103,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
      37,     0,    38,    51,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,   612,     0,   613,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,    34,    49,    35,    50,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,    37,     0,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,    37,    49,    38,    50,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,    34,     0,    35,     0,    36,
       0,     0,   587,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,    34,     0,    35,     0,    36,     0,     0,
     589,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    37,     0,    38,    51,    39,    40,
      34,    41,    35,    42,    36,    43,    44,   590,    45,    46,
      47,    48,    37,    49,    38,    50,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      34,    49,    35,    50,    36,    51,     0,   591,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,    51,   592,     0,     0,     0,   290,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,   593,    45,    46,    47,    48,     0,    49,     0,
      50,     0,   294,     0,     0,     0,    37,     0,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
      51,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,   609,    49,     0,    50,     0,     0,    51,     0,
      34,    37,    35,    38,    36,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,    34,     0,    35,     0,    36,
       0,     0,   611,     0,     0,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,     0,    49,    51,
      50,     0,     0,   615,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,    51,    45,    46,
      47,    48,     0,    49,    37,    50,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,    51,    50,     0,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,   621,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     623,    51,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,    51,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,    51,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
     625,     0,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   627,     0,     0,    51,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,    37,    50,    38,    51,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,    51,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   629,     0,     0,     0,
       0,    37,     0,    38,    51,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,   631,
      49,    37,    50,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,    37,   633,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,    51,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
     635,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,     0,     0,   637,     0,
      34,    51,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   639,
       0,    51,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,   641,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,    37,     0,    38,    51,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,    51,
      42,     0,    43,    44,     0,    45,    46,    47,    48,   677,
      49,     0,    50,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,    51,    45,    46,    47,
      48,     0,    49,     0,    50,     0,   679,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   681,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,    51,    49,    37,    50,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,    51,     0,
      34,     0,    35,   685,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,    51,    43,    44,   689,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
     691,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,   693,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,   697,     0,     0,    37,     0,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,   699,    49,    37,    50,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,     0,    49,    51,    50,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,    51,    45,    46,    47,    48,     0,
      49,    37,    50,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,   701,
      49,     0,    50,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   703,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,    51,    43,    44,     0,
      45,    46,    47,    48,     0,    49,    37,    50,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,    51,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,     0,     0,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     705,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,   707,     0,    37,     0,    38,     0,    39,
      40,    51,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,   722,     0,   805,     0,    51,     0,    34,     0,
      35,     0,    36,     0,     0,  1021,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   724,    51,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,   807,
      43,    44,     0,    45,    46,    47,    48,    37,    49,    38,
      50,    39,    40,    34,    41,    35,    42,    36,    43,    44,
       0,    45,    46,    47,    48,     0,    49,  1022,    50,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   808,     0,     0,
       0,    51,    37,     0,    38,     0,    39,    40,    34,    41,
      35,    42,    36,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,   810,  1023,     0,     0,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    37,  1024,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,    37,    50,    38,
      51,    39,    40,    34,    41,    35,    42,    36,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,     0,     0,     0,     0,     0,     0,  1025,     0,    34,
       0,    35,     0,    36,   983,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,  1026,    45,    46,    47,    48,
      34,    49,    35,    50,    36,     0,     0,     0,    37,     0,
      38,     0,    39,    40,    51,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
    1027,     0,    51,     0,     0,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,    51,     0,     0,
    1028,     0,     0,     0,    37,     0,    38,     0,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,  1029,    37,
       0,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,    51,    45,    46,    47,    48,     0,    49,    37,
      50,    38,     0,    39,    40,    34,    41,    35,    42,    36,
      43,    44,    51,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,    37,    50,    38,  1031,    39,    40,
       0,    41,     0,    42,     0,    43,    44,    51,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,     0,     0,
       0,     0,     0,     0,     0,  1032,     0,     0,     0,     0,
      34,     0,    35,    51,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1033,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,    51,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1034,    51,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,  1035,
      39,    40,     0,    41,    51,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,    37,     0,    38,    51,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,     0,     0,     0,    51,
      34,     0,    35,     0,    36,     0,     0,     0,     0,  1036,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,  1037,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,    51,    45,    46,    47,
      48,     0,    49,    34,    50,    35,     0,    36,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1038,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,  1039,    45,    46,    47,    48,
      34,    49,    35,    50,    36,     0,     0,     0,    37,     0,
      38,    51,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,     0,     0,     0,     0,     0,    51,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,  1040,    49,    34,
      50,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    35,  1041,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      38,     0,    39,    40,    51,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
    1042,    34,    37,    35,    38,    36,    39,    40,    51,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,  1044,     0,    37,     0,    38,    51,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,    34,  1046,
      35,     0,    36,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,    37,     0,    38,
    1048,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,    37,    50,    38,
       0,    39,    40,     0,    41,     0,    42,  1050,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,  1052,    45,    46,    47,    48,     0,    49,
       0,    50,    34,     0,    35,     0,    36,     0,     0,     0,
       0,     0,     0,     0,     0,    51,  1054,     0,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,    51,
      35,    37,    36,    38,     0,    39,    40,     0,    41,     0,
      42,     0,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,    34,    41,    35,    42,    36,
      43,    44,     0,    45,    46,    47,    48,    37,    49,    38,
      50,    39,    40,     0,    41,  1056,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,    34,    50,    35,
       0,    36,    51,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,    51,    49,     0,    50,     0,     0,    34,     0,
      35,     0,    36,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,  1058,
      45,    46,    47,    48,    34,    49,    35,    50,    36,     0,
       0,     0,     0,     0,     0,     0,     0,    37,     0,    38,
    1060,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    51,    49,    34,    50,    35,
       0,    36,     0,    37,  1064,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,  1068,    45,    46,    47,
      48,     0,    49,     0,    50,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,    51,    50,    34,     0,
      35,     0,    36,     0,     0,     0,  1070,     0,    37,     0,
      38,     0,    39,    40,    51,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,     0,    51,     0,    34,     0,    35,    37,    36,    38,
    1071,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,    51,
      34,     0,    35,     0,    36,     0,     0,     0,     0,  1072,
       0,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,    51,    49,     0,    50,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,  1073,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,     0,    36,     0,     0,     0,
    1075,     0,     0,     0,     0,     0,     0,     0,    51,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,  1076,    38,     0,    39,    40,     0,    41,     0,
      42,    51,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,    51,  1078,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,    51,  1079,     0,     0,    37,     0,    38,     0,
      39,    40,    34,    41,    35,    42,    36,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,    51,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
      34,    37,    35,    38,    36,    39,    40,     0,    41,     0,
      42,     0,    43,    44,    51,    45,    46,    47,    48,     0,
      49,     0,    50,    34,     0,    35,     0,    36,  1080,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,  1081,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      34,    49,    35,    50,    36,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,    51,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,  1083,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,  1084,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,    34,  1085,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,    51,    37,     0,    38,
    1086,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
       0,    34,     0,    35,     0,    36,  1087,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
      37,     0,    38,  1088,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,    34,    49,
      35,    50,    36,     0,     0,     0,     0,     0,     0,     0,
       0,    51,  1089,     0,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,    37,    49,    38,    50,    39,
      40,    34,    41,    35,    42,    36,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,     0,
    1091,     0,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
      37,     0,    38,     0,    39,    40,  1093,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,    51,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,    34,    50,    35,     0,    36,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,  1095,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    37,     0,    38,    51,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
      37,    49,    38,    50,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,  1099,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1101,     0,     0,     0,
      51,  1162,     0,    37,     0,    38,     0,    39,    40,    34,
      41,    35,    42,    36,    43,    44,     0,    45,    46,    47,
      48,  1204,    49,     0,    50,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
      35,     0,    36,     0,  1102,    51,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,    51,    50,
      34,     0,    35,     0,    36,     0,     0,    37,  1163,    38,
       0,    39,    40,     0,    41,     0,    42,  1206,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    51,    49,    34,
      50,    35,     0,    36,     0,  1208,     0,     0,     0,    37,
       0,    38,     0,    39,    40,    51,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,     0,     0,  1210,    37,     0,
      38,     0,    39,    40,    34,    41,    35,    42,    36,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,  1212,    35,     0,    36,     0,     0,    51,     0,
       0,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,  1214,     0,    34,     0,    35,
      37,    36,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,    51,    45,    46,    47,    48,     0,    49,
       0,    50,    34,     0,    35,     0,    36,  1216,     0,     0,
       0,     0,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,    51,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,    37,     0,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,    51,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,     0,    35,     0,    36,     0,     0,
       0,     0,  1218,     0,    51,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
       0,     0,    37,    51,    38,  1220,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,    34,     0,    35,     0,
      36,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,    34,
       0,    35,     0,    36,     0,     0,  1222,     0,     0,     0,
       0,     0,     0,     0,     0,    37,     0,    38,     0,    39,
      40,     0,    41,     0,    42,    51,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,    37,     0,
      38,     0,    39,    40,    34,    41,    35,    42,    36,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
       0,    51,     0,     0,  1224,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,    38,    51,    39,    40,    34,
      41,    35,    42,    36,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,     0,    37,     0,    38,     0,
      39,    40,  1226,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,    37,     0,
      38,    51,    39,    40,     0,    41,     0,    42,     0,    43,
      44,  1228,    45,    46,    47,    48,     0,    49,     0,    50,
       0,     0,     0,     0,     0,     0,     0,    51,    34,     0,
      35,     0,    36,     0,  1247,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    34,     0,
      35,     0,    36,     0,     0,     0,     0,     0,     0,     0,
    1248,     0,     0,     0,     0,     0,    34,    37,    35,    38,
      36,    39,    40,  1237,    41,     0,    42,  1230,    43,    44,
       0,    45,    46,    47,    48,  1249,    49,    37,    50,    38,
      51,    39,    40,    34,    41,    35,    42,    36,    43,    44,
       0,    45,    46,    47,    48,    37,    49,    38,    50,    39,
      40,     0,    41,    51,    42,     0,    43,    44,     0,    45,
      46,    47,    48,     0,    49,     0,    50,     0,     0,     0,
       0,     0,    37,     0,    38,     0,    39,    40,     0,    41,
       0,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,     0,    50,     0,     0,     0,  1250,    51,     0,
       0,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1251,    51,     0,     0,    34,     0,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,  1252,    38,    51,    39,    40,    34,    41,    35,    42,
      36,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,    37,     0,    38,     0,    39,    40,     0,
      41,  1253,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,    37,     0,    38,     0,    39,
      40,     0,    41,     0,    42,     0,    43,    44,     0,    45,
      46,    47,    48,    34,    49,    35,    50,    36,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,    34,     0,    35,     0,    36,
       0,     0,    37,     0,    38,  1254,    39,    40,     0,    41,
      51,    42,     0,    43,    44,     0,    45,    46,    47,    48,
       0,    49,    34,    50,    35,     0,    36,     0,     0,     0,
       0,  1255,     0,     0,    37,     0,    38,    51,    39,    40,
      34,    41,    35,    42,    36,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,  1256,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,    34,    41,    35,
      42,    36,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,     0,     0,     0,     0,    51,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,  1257,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,    51,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,  1259,     0,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,  1262,    45,    46,    47,    48,     0,    49,     0,    50,
       0,     0,     0,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,     0,    43,    44,    51,    45,    46,
      47,    48,     0,    49,    34,    50,    35,     0,    36,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,     0,    35,  1263,    36,     0,     0,    51,
       0,  1260,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,     0,    42,     0,    43,    44,    51,    45,    46,    47,
      48,    37,    49,    38,    50,    39,    40,    34,    41,    35,
      42,    36,    43,    44,    51,    45,    46,    47,    48,     0,
      49,     0,    50,     0,     0,    34,     0,    35,     0,    36,
       0,     0,     0,  1264,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,     0,    43,    44,     0,
      45,    46,    47,    48,    37,    49,    38,    50,    39,    40,
       0,    41,     0,    42,  1265,    43,    44,     0,    45,    46,
      47,    48,    34,    49,    35,    50,    36,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      34,     0,    35,    51,    36,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    38,     0,    39,    40,     0,    41,    51,
      42,     0,    43,    44,     0,    45,    46,    47,    48,    37,
      49,    38,    50,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,     0,     0,     0,  1266,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1267,    51,     0,
      34,     0,    35,     0,    36,     0,    37,     0,    38,     0,
      39,    40,     0,    41,     0,    42,    51,    43,    44,     0,
      45,    46,    47,    48,     0,    49,     0,    50,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,    37,
       0,    38,     0,    39,    40,  1268,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,    51,     0,     0,     0,     0,  1269,     0,     0,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,    51,
      43,    44,     0,    45,    46,    47,    48,  1300,    49,     0,
      50,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    34,     0,    35,
       0,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1301,     0,     0,     0,     0,     0,  1270,
      34,     0,    35,     0,    36,     0,    51,     0,     0,     0,
       0,     0,     0,     0,  1272,     0,    37,     0,    38,     0,
      39,    40,     0,    41,    51,    42,  1302,    43,    44,     0,
      45,    46,    47,    48,    34,    49,    35,    50,    36,    37,
       0,    38,     0,    39,    40,     0,    41,     0,    42,     0,
      43,    44,     0,    45,    46,    47,    48,    34,    49,    35,
      50,    36,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,    38,     0,    39,    40,     0,
      41,    51,    42,     0,    43,    44,     0,    45,    46,    47,
      48,     0,    49,     0,    50,    34,    37,    35,    38,    36,
      39,    40,     0,    41,     0,    42,  1303,    43,    44,     0,
      45,    46,    47,    48,    51,    49,     0,    50,     0,     0,
      34,     0,    35,     0,    36,     0,     0,     0,     0,     0,
       0,     0,  1304,     0,    37,     0,    38,     0,    39,    40,
       0,    41,     0,    42,    51,    43,    44,     0,    45,    46,
      47,    48,     0,    49,     0,    50,     0,     0,    34,    37,
      35,    38,    36,    39,    40,     0,    41,     0,    42,  1305,
      43,    44,     0,    45,    46,    47,    48,     0,    49,     0,
      50,    34,     0,    35,     0,    36,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1306,     0,    37,     0,    38,
       0,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,     0,
      37,    51,    38,     0,    39,    40,    34,    41,    35,    42,
      36,    43,    44,     0,    45,    46,    47,    48,     0,    49,
       0,    50,     0,     0,    51,     0,     0,     0,     0,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1307,     0,    37,     0,    38,     0,    39,
      40,    34,    41,    35,    42,    36,    43,    44,    51,    45,
      46,    47,    48,     0,    49,     0,    50,     0,    37,     0,
      38,     0,    39,    40,  1308,    41,     0,    42,     0,    43,
      44,    51,    45,    46,    47,    48,     0,    49,     0,    50,
      37,     0,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,    34,    49,
      35,    50,    36,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,     0,     0,  1309,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,    34,    37,    35,    38,
      36,    39,    40,     0,    41,     0,    42,     0,    43,    44,
       0,    45,    46,    47,    48,     0,    49,     0,    50,    34,
       0,    35,     0,    36,     0,     0,     0,     0,     0,     0,
       0,     0,    51,  1310,     0,    37,     0,    38,     0,    39,
      40,    34,    41,    35,    42,    36,    43,    44,     0,    45,
      46,    47,    48,     0,    49,    51,    50,     0,    37,     0,
      38,     0,    39,    40,     0,    41,     0,    42,     0,    43,
      44,     0,    45,    46,    47,    48,     0,    49,     0,    50,
      37,  1311,    38,     0,    39,    40,     0,    41,     0,    42,
       0,    43,    44,     0,    45,    46,    47,    48,     0,    49,
      51,    50,     0,     0,     0,     0,     0,  1312,     0,     0,
       0,  1316,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,  1313,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1318,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51
};

static const yytype_int16 yycheck[] =
{
       7,   719,    16,   669,    11,     3,     4,    16,    16,    21,
      23,   669,   669,    16,    27,   117,   268,    21,    31,    16,
     669,   116,   669,    16,   669,    16,    16,    16,    16,    27,
      28,    16,    16,    16,    16,    16,    16,    16,    16,    52,
      16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
      16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
      16,    16,    16,    70,    16,    16,    16,    16,    72,    76,
      16,    16,    79,    16,    81,     0,    83,   116,    85,   116,
      87,    88,    89,    90,    91,    92,    16,    94,    16,    96,
      16,    98,    16,   100,    16,   102,    16,   104,   105,    16,
     107,    16,    29,    16,    16,    21,    22,    16,    16,    16,
     117,    16,   119,    16,   121,    16,   123,    16,   125,    16,
     127,    16,   129,    16,   131,   112,   133,    16,   135,    16,
     137,    16,    16,   279,   280,    16,    16,    16,    16,    16,
      16,    16,    34,    16,   185,   278,    84,    16,   155,    16,
     157,    16,   159,    16,    16,    16,   163,    16,    16,    16,
     167,    16,   169,    16,   171,    16,    16,    16,   175,    16,
     177,    16,   179,    16,   181,    16,   183,    16,   185,    16,
      16,    16,    16,    16,    16,    16,    16,    16,    16,    16,
     197,    16,   199,    16,   201,    16,   203,    16,   205,    16,
      16,   895,   209,   117,   211,   116,   213,   125,   215,   128,
     217,    26,   219,    96,   221,    89,   116,   103,   112,   113,
      86,   219,   220,    45,   120,   121,   116,    47,   116,    47,
     924,   323,    96,   116,   116,   322,   234,   235,    34,   237,
     238,    93,   240,   241,   116,   116,   116,   101,   263,   117,
     118,   119,   120,   121,    96,   101,   116,   197,   281,    47,
     324,   117,   118,   119,   120,   121,   106,    91,   928,   117,
     118,   119,   120,   121,   118,   119,   120,   121,   285,   117,
      89,   288,    -1,    -1,   291,   292,    -1,    -1,   295,   296,
     116,    23,    24,    25,    96,    96,   116,   101,    30,    31,
     103,    33,   116,   116,   116,   318,   116,   320,   322,   322,
     116,   324,    -1,   322,   322,   313,   314,    24,    25,   331,
     125,   328,   117,    30,    31,   322,    33,   331,   128,   322,
      -1,   322,   322,   322,   322,    -1,    -1,   322,   322,   322,
     322,   322,   322,   322,   322,    -1,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,    -1,
     322,   322,   322,   322,    -1,    -1,   322,   322,    -1,   322,
      -1,   394,  1100,   396,    -1,   398,    -1,   400,    -1,   402,
      -1,   404,   322,   406,   322,    -1,   322,    -1,   322,    -1,
     322,    -1,   322,  1069,   417,   322,    -1,   322,   421,   322,
     322,  1069,  1069,   322,   322,   322,   429,   322,    -1,   322,
    1069,   322,  1069,   322,  1069,   322,    -1,   322,   324,   322,
     443,  1097,   445,   322,   447,   322,   449,   322,   322,  1097,
    1097,   322,   322,   322,   322,   322,   322,   322,  1097,   322,
    1097,   464,  1097,   322,   322,   322,   324,   322,    -1,   322,
     322,   322,    -1,   322,   322,   322,   322,   322,   324,   322,
      -1,   322,   322,   322,    -1,   322,   324,   322,    -1,   322,
     324,   322,    -1,   322,    -1,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,    -1,   322,    -1,   322,
    1194,   322,    -1,   322,    -1,   322,   322,    -1,    -1,    -1,
      -1,    -1,    -1,   520,   246,   247,   523,    -1,    -1,    -1,
     533,   525,    -1,   535,   537,    -1,    -1,   540,    -1,    -1,
     543,   535,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   246,
     247,   793,   794,   795,   796,   797,    -1,    24,    25,    -1,
      -1,    -1,    -1,    30,    31,   562,    33,   564,    -1,   566,
      -1,   568,    -1,    -1,   571,    -1,    -1,   574,    -1,   573,
     577,    24,    25,   580,    -1,    -1,   583,    30,    31,   586,
      33,   588,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   596,
      -1,    -1,   599,   845,    -1,   602,    -1,    -1,   605,    -1,
      -1,   608,    -1,   610,    -1,  1271,    -1,   614,    -1,   616,
      -1,    -1,    -1,  1271,  1271,   622,    -1,   624,    -1,   626,
      -1,   628,  1271,   630,  1271,   632,  1271,   634,    -1,   636,
      -1,   638,    -1,   640,    -1,   642,   643,    -1,   645,    -1,
     647,    -1,   649,    -1,   651,    -1,   653,    -1,   655,    -1,
     657,    -1,   659,    -1,   661,    -1,    -1,    -1,   665,    -1,
      -1,    -1,   669,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   678,    -1,   680,    -1,   682,   683,    -1,    -1,   686,
     687,    -1,    -1,   690,    -1,   692,    -1,   694,   695,    -1,
      -1,   698,    -1,   700,    -1,   702,    -1,   704,    -1,   706,
      -1,   708,   709,    -1,   711,    -1,   713,    -1,   715,    -1,
      -1,    -1,   719,    -1,    -1,    -1,   723,    -1,   725,    -1,
     727,    -1,   729,    -1,   731,   732,    -1,    -1,   735,    -1,
     737,    -1,   739,   746,   741,   748,   743,   750,   745,   752,
     753,   993,   755,    -1,   757,    -1,   759,    -1,   761,     8,
     763,    -1,   765,    -1,   767,    -1,   769,    -1,   771,    -1,
     773,    -1,   775,    -1,   777,    -1,   779,    -1,   781,   246,
     247,    -1,   784,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     784,    -1,    -1,    -1,   791,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,   246,   247,   802,    -1,   804,    -1,   806,
      -1,    -1,   809,    -1,    -1,   812,    -1,    -1,   821,    -1,
     823,    -1,   825,    -1,    -1,    -1,    -1,    -1,    -1,   831,
      -1,    -1,   829,    -1,    83,    -1,    85,   831,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,   861,    -1,
     863,    -1,   865,    -1,   867,    -1,   869,     3,   871,     5,
     873,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,   881,
      16,    -1,    -1,   886,    -1,    -1,    -1,   881,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   900,    -1,    -1,
     903,    -1,    -1,    -1,    -1,   908,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   916,    -1,   918,    -1,   920,    -1,   922,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    65,
      -1,    67,   935,    69,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,    -1,    -1,   942,    -1,   944,    -1,   946,
      -1,   948,   949,    -1,   951,    -1,   953,    -1,   955,    -1,
     957,    -1,   959,    -1,   961,    -1,   963,    -1,   965,    -1,
     967,    -1,   969,    -1,   971,    -1,   973,    -1,   975,    -1,
     977,    -1,   979,    -1,    -1,   982,    -1,   984,    -1,   992,
      -1,   994,   986,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,    -1,   141,    -1,   143,    -1,   145,
    1007,   147,  1009,   149,  1011,   151,    -1,   153,    -1,  1016,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,  1043,    -1,  1045,    -1,
    1047,   187,  1049,   189,  1051,   191,  1053,   193,  1055,   195,
    1057,    -1,  1059,    -1,  1061,  1062,    -1,    -1,  1065,  1066,
      -1,   207,  1069,    -1,    -1,    -1,    -1,  1074,    -1,    -1,
    1077,    -1,    -1,    -1,    -1,  1082,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1090,    -1,  1092,    -1,  1094,    -1,  1096,
    1097,    -1,    -1,  1100,    -1,    -1,    -1,    -1,    -1,    -1,
    1107,    -1,  1115,    -1,  1117,    -1,  1119,    -1,    -1,  1122,
      -1,  1124,    -1,  1126,    -1,  1128,    -1,  1130,    -1,  1132,
      -1,  1134,    -1,  1136,    -1,  1138,    -1,  1140,    -1,  1142,
      -1,  1144,    -1,  1146,    -1,  1148,    -1,  1150,   284,    -1,
    1152,   287,    -1,   289,    -1,    -1,    -1,   293,  1152,  1156,
     221,   297,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1181,
      -1,  1184,   186,   187,    -1,   189,   190,  1181,   192,   193,
      -1,   195,   196,    -1,   198,   199,    -1,    -1,     5,    -1,
      -1,  1198,    -1,  1200,    -1,  1202,    -1,    -1,  1205,    -1,
    1207,    -1,  1209,    -1,  1211,    -1,  1213,    -1,  1215,    -1,
    1217,    -1,  1219,    -1,  1221,    -1,  1223,   288,  1225,     8,
    1227,   292,  1229,    -1,  1231,   296,  1233,    44,  1235,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   317,   261,   262,    -1,
      -1,  1258,    -1,    -1,  1261,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,  1271,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,     5,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,     8,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,   478,    -1,   480,    -1,   482,    -1,   484,   485,
      -1,   487,    -1,   489,    -1,   491,    -1,   493,    44,   495,
      46,   497,    48,   499,    -1,   501,    -1,   503,    -1,   505,
      -1,   507,    -1,   509,    83,   511,    85,   513,    87,    88,
      -1,    90,   518,    92,     8,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,   563,    -1,   565,
      -1,   567,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   575,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,   288,    11,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,   562,   102,   564,   104,   566,    -1,   568,   644,   288,
     646,    -1,   648,    -1,   650,    44,   652,    46,   654,    48,
     656,    -1,    -1,    -1,    -1,    -1,    -1,   663,    -1,    -1,
      -1,   667,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     676,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   684,    -1,
      -1,    -1,   688,    -1,    83,    -1,    85,    -1,    87,    88,
     696,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,   710,   104,   712,    -1,   714,   288,
     716,   717,   643,    -1,   645,   721,   647,    -1,   649,    44,
     651,    46,   653,    48,   655,    -1,    -1,   733,   221,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   683,    -1,    11,    -1,   687,    -1,    83,    -1,
      85,    -1,    87,    88,   695,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    11,   102,   709,   104,
     711,    -1,   713,    -1,   715,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,   288,   288,    -1,    -1,    -1,   292,
      -1,   732,    -1,   296,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    11,    -1,    -1,   317,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    83,    -1,
      85,   321,    87,    88,    44,    90,    46,    92,    48,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,   802,    -1,    -1,    -1,   806,    -1,    -1,   809,    -1,
      -1,   812,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    13,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    46,   943,    48,   945,
      -1,   947,    -1,    -1,   950,    -1,   952,    -1,   954,    -1,
     956,    -1,   958,    -1,   960,    -1,   962,    -1,   964,    -1,
     966,    -1,   968,   288,   970,    -1,   972,    -1,   974,    -1,
     976,    -1,   978,    83,   980,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,   321,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   942,    -1,   944,    -1,   946,    -1,   948,   949,    -1,
     951,    -1,   953,    -1,   955,    -1,   957,    -1,   959,    -1,
     961,   288,   963,    -1,   965,    -1,   967,    -1,   969,    -1,
     971,    -1,   973,    -1,   975,    -1,   977,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,  1063,    -1,    -1,
      -1,  1067,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   562,
      -1,   564,    -1,   566,    -1,   568,  1007,    -1,  1009,    -1,
    1011,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1098,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1043,    -1,  1045,    -1,  1047,    -1,  1049,    -1,
    1051,    -1,  1053,    -1,  1055,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,  1066,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1074,    -1,    -1,  1077,    -1,    -1,    -1,
     643,  1082,   645,    -1,   647,    -1,   649,    -1,   651,  1090,
     653,  1092,   655,  1094,    -1,  1096,    13,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,  1107,    92,   288,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
     683,    -1,    -1,    -1,   687,    -1,    -1,    44,    -1,    46,
      -1,    48,   695,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   709,    -1,   711,    -1,
     713,    -1,   715,    -1,    -1,    -1,  1157,    -1,  1159,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,   732,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1198,    -1,  1200,
      -1,  1202,    -1,    -1,  1205,    -1,  1207,    -1,  1209,    -1,
    1211,    -1,  1213,    -1,  1215,    -1,  1217,    -1,  1219,    -1,
    1221,    -1,  1223,    -1,  1225,    -1,  1227,    -1,  1229,    -1,
    1231,    -1,  1233,    -1,    -1,    -1,    -1,    -1,    -1,   802,
      -1,    -1,    -1,   806,    -1,    -1,   809,    -1,    -1,   812,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1261,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     4,    -1,     6,    -1,     8,
      -1,    10,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    51,   318,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    -1,    68,
      -1,    -1,    71,    -1,    73,    -1,    75,    -1,    77,    -1,
      -1,    80,    -1,    82,    -1,    84,    -1,    86,    -1,    -1,
      -1,   288,    -1,    -1,    93,    -1,    95,    -1,    97,   942,
      99,   944,   101,   946,   103,   948,   949,   106,   951,   108,
     953,    -1,   955,    -1,   957,   114,   959,   116,   961,   118,
     963,   120,   965,   122,   967,   124,   969,   126,   971,   128,
     973,   130,   975,   132,   977,   134,    -1,   136,    -1,   138,
      -1,   140,    -1,   142,    -1,   144,    -1,   146,    -1,   148,
      -1,   150,    -1,   152,    -1,   154,    -1,   156,    -1,   158,
      -1,   160,    -1,   162,  1007,   164,  1009,   166,  1011,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,   176,    -1,   178,
      -1,   180,    -1,   182,    -1,   184,    -1,   186,    -1,   188,
      -1,   190,    -1,   192,    -1,   194,    -1,   196,    -1,   198,
    1043,   200,  1045,   202,  1047,   204,  1049,   206,  1051,   208,
    1053,   210,  1055,   212,    -1,   214,    -1,   216,    -1,   218,
      -1,   220,    -1,  1066,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1074,    -1,    -1,  1077,    -1,    -1,    -1,    -1,  1082,
      -1,    -1,    -1,    13,    -1,    -1,    -1,  1090,    -1,  1092,
      -1,  1094,    -1,  1096,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    13,    -1,    -1,  1107,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,  1157,    85,  1159,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    83,   102,    85,   104,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,  1198,    -1,  1200,    -1,  1202,
      -1,    -1,  1205,    -1,  1207,    -1,  1209,    -1,  1211,    44,
    1213,    46,  1215,    48,  1217,    -1,  1219,    -1,  1221,    -1,
    1223,    -1,  1225,    -1,  1227,    -1,  1229,    -1,  1231,    -1,
    1233,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,  1261,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     479,    -1,   481,    -1,   483,    -1,    -1,   486,    -1,   488,
      -1,   490,    -1,   492,    44,   494,    46,   496,    48,   498,
      -1,   500,    -1,   502,    -1,   504,    -1,   506,    -1,   508,
      -1,   510,    -1,   512,    -1,   514,    -1,    -1,   288,    -1,
     519,    -1,   521,    -1,    -1,   524,    -1,   526,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    85,   288,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,     6,     7,    -1,     9,    10,
      -1,    12,    -1,    14,    15,    -1,    -1,    18,    -1,    20,
      21,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    35,    36,    -1,    38,    39,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    -1,    49,    50,
      -1,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,   288,    65,    66,    67,    -1,    69,    -1,
      71,    72,    73,    -1,    75,    -1,    77,    78,    -1,    80,
      81,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   318,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   105,    -1,   107,   108,   109,   658,
     111,   660,   315,   662,    -1,   664,    -1,   666,    -1,   668,
      -1,    -1,   123,   124,    -1,   126,   127,    -1,   129,   130,
      -1,   132,   133,    -1,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,    -1,   147,   148,    -1,   150,
     151,    -1,   153,   154,    -1,   156,   157,    -1,   159,   160,
      -1,   162,   163,    -1,   165,   166,    -1,   168,   169,   718,
     171,   172,    -1,   174,   175,    -1,   177,   178,    -1,   180,
     181,    -1,   183,   184,    -1,    -1,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    -1,   204,   205,    -1,   207,   208,    -1,   210,
     211,    -1,   213,   214,    -1,   216,   217,    -1,    -1,    -1,
      -1,   222,   223,    -1,   225,   226,    -1,   228,   229,    -1,
     231,   232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   243,   244,    -1,   246,   247,    -1,   249,   250,
      -1,   252,   253,    -1,   255,   256,    -1,   258,   259,    -1,
      -1,    -1,    -1,   264,   265,    -1,   267,   268,    -1,   270,
     271,    -1,   273,   274,    -1,   276,   277,    -1,    -1,    -1,
      -1,   282,   283,    -1,   285,   286,    -1,    -1,   289,   290,
      -1,   292,   293,    -1,   295,   296,    -1,   298,   299,    -1,
     301,   302,    -1,   304,   305,    -1,   307,   308,    -1,   310,
     311,    -1,    -1,    -1,    -1,   316,   317,    -1,   319,   320,
       6,     7,    -1,     9,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    18,    -1,    20,    21,    22,    -1,    24,    25,
      -1,    -1,    -1,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    -1,    38,    39,    -1,    41,    42,    44,    -1,    46,
      -1,    48,    -1,    49,    50,    -1,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    67,    -1,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    77,    78,    -1,    80,    81,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,   105,
      -1,   107,    -1,   109,   110,   111,    -1,    -1,    -1,    -1,
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
     286,   288,    -1,   289,   290,    -1,   292,   293,    -1,   295,
     296,    -1,   298,   299,    -1,   301,   302,    -1,   304,   305,
      -1,   307,   308,    -1,   310,   311,    -1,    -1,   315,    -1,
     316,   317,    -1,   319,   320,     6,     7,    -1,     9,    10,
      -1,    12,    -1,    14,    15,    17,    -1,    18,    -1,    20,
      21,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    30,
      31,    -1,    33,    -1,    35,    36,    -1,    38,    39,    -1,
      41,    42,    44,    -1,    46,    -1,    48,    -1,    49,    50,
      -1,    52,    53,    54,    55,    56,    57,    -1,    59,    60,
      -1,    62,    63,    -1,    65,    66,    67,    -1,    69,    -1,
      71,    72,    73,    -1,    75,    -1,    77,    78,    -1,    80,
      81,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,   105,    -1,   107,    -1,   109,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,    -1,   126,   127,    -1,   129,   130,
      -1,   132,   133,    -1,   135,   136,    -1,   138,   139,    -1,
     141,   142,    -1,   144,   145,    -1,   147,   148,    -1,   150,
     151,    -1,   153,   154,    -1,   156,   157,    -1,   159,   160,
      -1,   162,   163,    -1,   165,   166,    -1,   168,   169,    -1,
     171,   172,    -1,   174,   175,    -1,   177,   178,    -1,   180,
     181,    -1,   183,   184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     201,   202,    -1,   204,   205,    -1,   207,   208,    -1,   210,
     211,    -1,   213,   214,    -1,   216,   217,    -1,    -1,    -1,
      -1,   222,   223,    -1,   225,   226,    -1,   228,   229,    -1,
     231,   232,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   243,   244,    -1,   246,   247,    -1,   249,   250,
      -1,   252,   253,    -1,   255,   256,    -1,   258,   259,    -1,
      -1,    -1,    -1,   264,   265,    -1,   267,   268,    -1,   270,
     271,    -1,   273,   274,    -1,   276,   277,    -1,    -1,    -1,
      -1,   282,   283,    -1,   285,   286,   288,    -1,   289,   290,
      -1,   292,   293,    -1,   295,   296,    -1,   298,   299,    -1,
     301,   302,    -1,   304,   305,    -1,   307,   308,    -1,   310,
     311,    -1,    -1,    -1,    -1,   316,   317,    -1,   319,   320,
       6,     7,    -1,     9,    10,    -1,    12,    -1,    14,    15,
      -1,    -1,    18,    -1,    20,    21,    22,    -1,    24,    25,
      -1,    -1,    29,    -1,    30,    31,    -1,    33,    -1,    35,
      36,    -1,    38,    39,    -1,    41,    42,    44,    -1,    46,
      -1,    48,    -1,    49,    50,    -1,    52,    53,    54,    55,
      56,    57,    -1,    59,    60,    -1,    62,    63,    -1,    65,
      66,    67,    -1,    69,    -1,    71,    72,    73,    -1,    75,
      -1,    77,    78,    -1,    80,    81,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,   105,
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
     286,   288,    -1,   289,   290,    -1,   292,   293,    -1,   295,
     296,    -1,   298,   299,    -1,   301,   302,    -1,   304,   305,
      -1,   307,   308,    -1,   310,   311,    18,    19,    -1,    -1,
     316,   317,    -1,   319,   320,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      46,    83,    48,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    83,   102,    85,
     104,    87,    88,    44,    90,    46,    92,    48,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    44,    -1,    46,    83,    48,    85,    23,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    83,   102,    85,   104,    87,    88,    44,    90,
      46,    92,    48,    94,    95,    -1,    97,    98,    99,   100,
      83,   102,    85,   104,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,    23,    -1,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    44,   102,    46,   104,    48,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,   288,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,   288,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,   288,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,   312,    94,    95,   288,
      97,    98,    99,   100,    26,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      -1,    -1,    44,   312,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,   309,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,   309,    -1,    -1,    -1,
      -1,    83,   288,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    26,    90,    -1,    92,    -1,    94,    95,   288,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    26,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,   288,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,   288,    -1,    44,    -1,    46,    83,    48,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   306,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    83,   102,    85,   104,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    29,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   306,    -1,    -1,    44,    -1,    46,    83,    48,
      85,   288,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    83,   102,    85,   104,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    44,    -1,    46,
      -1,    48,    -1,    32,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    44,    -1,    46,    83,    48,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   288,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,   303,    94,    95,   288,    97,    98,
      99,   100,    83,   102,    85,   104,    87,    88,    -1,    90,
      -1,    92,   303,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    44,    -1,    46,    83,    48,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,   288,    97,    98,    99,   100,    83,   102,    85,   104,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   288,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,   300,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,   288,    97,    98,
      99,   100,    -1,   102,    -1,   104,    83,    -1,    85,   300,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    37,
      97,    98,    99,   100,    -1,   102,    44,   104,    46,    83,
      48,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,
      88,    -1,    90,   288,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    38,    -1,    40,
      -1,   288,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,   300,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,   300,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      83,   102,    85,   104,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    40,    -1,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    83,    -1,    85,    -1,    87,    88,    -1,    90,
     297,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,   288,    83,    -1,    85,    -1,    87,
      88,    -1,    90,   297,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    41,   104,    43,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    83,   102,    85,   104,    87,
      88,    44,    90,    46,    92,    48,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,   288,    43,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      83,   294,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    83,   102,
      85,   104,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    44,   104,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    57,    58,    -1,    -1,    -1,    44,   288,    46,    -1,
      48,    -1,    -1,   294,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    83,    -1,    85,
     288,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    83,   102,    85,   104,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    -1,
      -1,    -1,   288,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
     288,    87,    88,   291,    90,    66,    92,    68,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    44,    83,    46,    85,    48,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,   288,    69,    70,   291,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,   288,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    77,    -1,    79,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    44,   102,    46,   104,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    83,   102,    85,   104,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    44,    -1,    46,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    44,    -1,    46,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    83,    -1,    85,   288,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    51,    97,    98,
      99,   100,    83,   102,    85,   104,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      44,   102,    46,   104,    48,   288,    -1,    51,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,   288,    51,    -1,    -1,    -1,   236,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    51,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,   242,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
     288,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    74,   102,    -1,   104,    -1,    -1,   288,    -1,
      44,    83,    46,    85,    48,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,   288,
     104,    -1,    -1,    82,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,   288,    97,    98,
      99,   100,    -1,   102,    83,   104,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,   288,   104,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,   131,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     134,   288,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   288,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,   288,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,    -1,   288,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    83,   104,    85,   288,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,   288,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,
      -1,    83,    -1,    85,   288,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,   146,
     102,    83,   104,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    83,   149,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,   288,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
     152,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,   155,    -1,
      44,   288,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,   288,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,   161,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,    83,    -1,    85,   288,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,   288,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,   203,
     102,    -1,   104,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,   288,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,   206,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   209,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,   288,   102,    83,   104,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,   288,    -1,
      44,    -1,    46,   215,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,   288,    94,    95,   224,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
     227,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,   230,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,   245,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,   248,   102,    83,   104,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,   288,   104,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   288,    97,    98,    99,   100,    -1,
     102,    83,   104,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,   251,
     102,    -1,   104,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   254,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,   288,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    83,   104,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,   288,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     257,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,   260,    -1,    83,    -1,    85,    -1,    87,
      88,   288,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   284,    -1,   221,    -1,   288,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   287,   288,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,   236,
      94,    95,    -1,    97,    98,    99,   100,    83,   102,    85,
     104,    87,    88,    44,    90,    46,    92,    48,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    58,   104,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   239,    -1,    -1,
      -1,   288,    83,    -1,    85,    -1,    87,    88,    44,    90,
      46,    92,    48,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,   242,    61,    -1,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    83,    64,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    83,   104,    85,
     288,    87,    88,    44,    90,    46,    92,    48,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    44,
      -1,    46,    -1,    48,   248,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    70,    97,    98,    99,   100,
      44,   102,    46,   104,    48,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,   288,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      74,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,   288,    -1,    -1,
      79,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    82,    83,
      -1,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,   288,    97,    98,    99,   100,    -1,   102,    83,
     104,    85,    -1,    87,    88,    44,    90,    46,    92,    48,
      94,    95,   288,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    83,   104,    85,   131,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,   288,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   134,    -1,    -1,    -1,    -1,
      44,    -1,    46,   288,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   288,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   288,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,   143,
      87,    88,    -1,    90,   288,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    83,    -1,    85,   288,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,   146,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,   288,    97,    98,    99,
     100,    -1,   102,    44,   104,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   152,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,   155,    97,    98,    99,   100,
      44,   102,    46,   104,    48,    -1,    -1,    -1,    83,    -1,
      85,   288,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,   158,   102,    44,
     104,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    46,   161,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,   288,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
     164,    44,    83,    46,    85,    48,    87,    88,   288,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,   167,    -1,    83,    -1,    85,   288,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    44,   170,
      46,    -1,    48,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    83,    -1,    85,
     173,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    83,   104,    85,
      -1,    87,    88,    -1,    90,    -1,    92,   176,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,   179,    97,    98,    99,   100,    -1,   102,
      -1,   104,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,   182,    -1,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,   288,
      46,    83,    48,    85,    -1,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    44,    90,    46,    92,    48,
      94,    95,    -1,    97,    98,    99,   100,    83,   102,    85,
     104,    87,    88,    -1,    90,   188,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    44,   104,    46,
      -1,    48,   288,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,   288,   102,    -1,   104,    -1,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,   191,
      97,    98,    99,   100,    44,   102,    46,   104,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,
     194,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,   288,   102,    44,   104,    46,
      -1,    48,    -1,    83,   200,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,   185,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,   288,   104,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,   203,    -1,    83,    -1,
      85,    -1,    87,    88,   288,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    -1,   288,    -1,    44,    -1,    46,    83,    48,    85,
     206,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,   288,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,   209,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,   288,   102,    -1,   104,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,   212,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,
     215,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,   218,    85,    -1,    87,    88,    -1,    90,    -1,
      92,   288,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,   288,   224,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,   227,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    44,    90,    46,    92,    48,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,   288,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      44,    83,    46,    85,    48,    87,    88,    -1,    90,    -1,
      92,    -1,    94,    95,   288,    97,    98,    99,   100,    -1,
     102,    -1,   104,    44,    -1,    46,    -1,    48,   230,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,   233,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      44,   102,    46,   104,    48,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,   288,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,   245,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,   248,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    44,   251,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,   288,    83,    -1,    85,
     254,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      -1,    44,    -1,    46,    -1,    48,   257,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,
      83,    -1,    85,   260,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    44,   102,
      46,   104,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,   266,    -1,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    83,   102,    85,   104,    87,
      88,    44,    90,    46,    92,    48,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,
     269,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      83,    -1,    85,    -1,    87,    88,   272,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,   288,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    44,   104,    46,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,   288,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      83,   102,    85,   104,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,
     288,   221,    -1,    83,    -1,    85,    -1,    87,    88,    44,
      90,    46,    92,    48,    94,    95,    -1,    97,    98,    99,
     100,   164,   102,    -1,   104,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      46,    -1,    48,    -1,   287,   288,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,   288,   104,
      44,    -1,    46,    -1,    48,    -1,    -1,    83,   239,    85,
      -1,    87,    88,    -1,    90,    -1,    92,   167,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,   288,   102,    44,
     104,    46,    -1,    48,    -1,   170,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,   288,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,   173,    83,    -1,
      85,    -1,    87,    88,    44,    90,    46,    92,    48,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,   176,    46,    -1,    48,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,   179,    -1,    44,    -1,    46,
      83,    48,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,   288,    97,    98,    99,   100,    -1,   102,
      -1,   104,    44,    -1,    46,    -1,    48,   182,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,   288,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,   288,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,    -1,
      -1,    -1,   212,    -1,   288,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      -1,    -1,    83,   288,    85,   218,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    44,
      -1,    46,    -1,    48,    -1,    -1,   233,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,
      88,    -1,    90,    -1,    92,   288,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    83,    -1,
      85,    -1,    87,    88,    44,    90,    46,    92,    48,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,   288,    -1,    -1,   266,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    85,   288,    87,    88,    44,
      90,    46,    92,    48,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    -1,    83,    -1,    85,    -1,
      87,    88,   269,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    83,    -1,
      85,   288,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,   272,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,    44,    -1,
      46,    -1,    48,    -1,   164,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,
      46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     167,    -1,    -1,    -1,    -1,    -1,    44,    83,    46,    85,
      48,    87,    88,   248,    90,    -1,    92,   275,    94,    95,
      -1,    97,    98,    99,   100,   170,   102,    83,   104,    85,
     288,    87,    88,    44,    90,    46,    92,    48,    94,    95,
      -1,    97,    98,    99,   100,    83,   102,    85,   104,    87,
      88,    -1,    90,   288,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,
      -1,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    -1,   104,    -1,    -1,    -1,   173,   288,    -1,
      -1,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   288,    -1,    -1,    44,    -1,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      83,   179,    85,   288,    87,    88,    44,    90,    46,    92,
      48,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,   182,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    83,    -1,    85,    -1,    87,
      88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,    97,
      98,    99,   100,    44,   102,    46,   104,    48,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    83,    -1,    85,   188,    87,    88,    -1,    90,
     288,    92,    -1,    94,    95,    -1,    97,    98,    99,   100,
      -1,   102,    44,   104,    46,    -1,    48,    -1,    -1,    -1,
      -1,   191,    -1,    -1,    83,    -1,    85,   288,    87,    88,
      44,    90,    46,    92,    48,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,   194,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    44,    90,    46,
      92,    48,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    -1,    -1,    -1,    -1,   288,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,   197,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,   288,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,   200,    -1,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,   185,    97,    98,    99,   100,    -1,   102,    -1,   104,
      -1,    -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,    -1,    94,    95,   288,    97,    98,
      99,   100,    -1,   102,    44,   104,    46,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    -1,    46,   212,    48,    -1,    -1,   288,
      -1,   263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,    -1,    92,    -1,    94,    95,   288,    97,    98,    99,
     100,    83,   102,    85,   104,    87,    88,    44,    90,    46,
      92,    48,    94,    95,   288,    97,    98,    99,   100,    -1,
     102,    -1,   104,    -1,    -1,    44,    -1,    46,    -1,    48,
      -1,    -1,    -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   288,    -1,    -1,    -1,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,    -1,    94,    95,    -1,
      97,    98,    99,   100,    83,   102,    85,   104,    87,    88,
      -1,    90,    -1,    92,   233,    94,    95,    -1,    97,    98,
      99,   100,    44,   102,    46,   104,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      44,    -1,    46,   288,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    83,    -1,    85,    -1,    87,    88,    -1,    90,   288,
      92,    -1,    94,    95,    -1,    97,    98,    99,   100,    83,
     102,    85,   104,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,    -1,    -1,    -1,   266,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   269,   288,    -1,
      44,    -1,    46,    -1,    48,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,    -1,    92,   288,    94,    95,    -1,
      97,    98,    99,   100,    -1,   102,    -1,   104,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,   272,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,   288,    -1,    -1,    -1,    -1,   275,    -1,    -1,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,   288,
      94,    95,    -1,    97,    98,    99,   100,   164,   102,    -1,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    44,    -1,    46,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   167,    -1,    -1,    -1,    -1,    -1,   281,
      44,    -1,    46,    -1,    48,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    83,    -1,    85,    -1,
      87,    88,    -1,    90,   288,    92,   170,    94,    95,    -1,
      97,    98,    99,   100,    44,   102,    46,   104,    48,    83,
      -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,
      94,    95,    -1,    97,    98,    99,   100,    44,   102,    46,
     104,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    83,    -1,    85,    -1,    87,    88,    -1,
      90,   288,    92,    -1,    94,    95,    -1,    97,    98,    99,
     100,    -1,   102,    -1,   104,    44,    83,    46,    85,    48,
      87,    88,    -1,    90,    -1,    92,   173,    94,    95,    -1,
      97,    98,    99,   100,   288,   102,    -1,   104,    -1,    -1,
      44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    83,    -1,    85,    -1,    87,    88,
      -1,    90,    -1,    92,   288,    94,    95,    -1,    97,    98,
      99,   100,    -1,   102,    -1,   104,    -1,    -1,    44,    83,
      46,    85,    48,    87,    88,    -1,    90,    -1,    92,   179,
      94,    95,    -1,    97,    98,    99,   100,    -1,   102,    -1,
     104,    44,    -1,    46,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   182,    -1,    83,    -1,    85,
      -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    -1,
      83,   288,    85,    -1,    87,    88,    44,    90,    46,    92,
      48,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
      -1,   104,    -1,    -1,   288,    -1,    -1,    -1,    -1,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   212,    -1,    83,    -1,    85,    -1,    87,
      88,    44,    90,    46,    92,    48,    94,    95,   288,    97,
      98,    99,   100,    -1,   102,    -1,   104,    -1,    83,    -1,
      85,    -1,    87,    88,   218,    90,    -1,    92,    -1,    94,
      95,   288,    97,    98,    99,   100,    -1,   102,    -1,   104,
      83,    -1,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    44,   102,
      46,   104,    48,    -1,    -1,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   233,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   288,    -1,    44,    83,    46,    85,
      48,    87,    88,    -1,    90,    -1,    92,    -1,    94,    95,
      -1,    97,    98,    99,   100,    -1,   102,    -1,   104,    44,
      -1,    46,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,   266,    -1,    83,    -1,    85,    -1,    87,
      88,    44,    90,    46,    92,    48,    94,    95,    -1,    97,
      98,    99,   100,    -1,   102,   288,   104,    -1,    83,    -1,
      85,    -1,    87,    88,    -1,    90,    -1,    92,    -1,    94,
      95,    -1,    97,    98,    99,   100,    -1,   102,    -1,   104,
      83,   269,    85,    -1,    87,    88,    -1,    90,    -1,    92,
      -1,    94,    95,    -1,    97,    98,    99,   100,    -1,   102,
     288,   104,    -1,    -1,    -1,    -1,    -1,   272,    -1,    -1,
      -1,   197,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   275,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   263,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   281,    -1,    -1,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,     3,     4,    27,    28,   219,   220,   234,   235,   237,
     238,   240,   241,   313,   314,   326,   382,   327,    16,   393,
      29,   327,   393,   327,   393,   382,   393,   327,   393,   382,
     393,   327,   393,     0,    44,    46,    48,    83,    85,    87,
      88,    90,    92,    94,    95,    97,    98,    99,   100,   102,
     104,   288,   327,   383,   384,   385,   386,   387,   388,   389,
     390,   391,   392,     6,     7,     9,    10,    12,    14,    15,
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
     302,   304,   305,   307,   308,   310,   311,   316,   317,   319,
     320,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   394,   397,   400,   403,
     404,   405,   406,   407,   408,   409,   410,   411,   412,   413,
     414,   415,   117,    16,   322,   357,   358,   322,   328,   322,
     236,   322,   328,   322,   242,   322,   328,   322,   116,    47,
     116,   393,   116,   116,   116,   393,   116,   116,   116,   116,
     393,   393,   116,   116,   116,   393,   393,   328,   327,   393,
     327,   393,   327,   393,   327,   382,   393,    23,   358,   393,
      26,   327,   393,   382,   393,    34,   382,   393,   382,   393,
     382,   393,   382,   393,   382,   382,   382,   382,   382,   382,
     393,   382,   393,   382,   393,   382,   393,   382,   393,   382,
     393,   382,   382,   393,   382,   393,   112,   395,   125,   393,
     128,   393,   382,   393,   382,   393,   382,   393,   382,   393,
     382,   393,   382,   393,   382,   393,   382,   393,   382,   393,
     382,   393,   382,   393,   327,   393,   327,   393,   327,   393,
     327,   393,   327,   393,   327,   393,   327,   393,   185,   327,
     393,   382,   393,   382,   393,   382,   393,   327,   393,   382,
     393,   327,   393,   382,   393,   382,   393,   382,   393,   327,
     393,   382,   393,   382,   393,   382,   393,   382,   393,   382,
     393,   382,   393,   327,   393,   327,   393,   327,   393,   327,
     393,   278,   327,   393,   382,   393,   382,   393,   382,   393,
     382,   393,   382,   393,   327,   393,   382,   393,   382,   393,
     382,   393,   382,   393,   382,   393,   382,   393,     6,     7,
       9,    10,    12,    14,    15,   162,   163,   165,   166,   168,
     169,   171,   172,   174,   175,   177,   178,   180,   181,   210,
     211,   216,   217,   231,   232,   264,   265,   267,   268,   270,
     271,   273,   274,   298,   299,   329,   382,   394,    24,    25,
      30,    31,    33,   246,   247,    21,    22,   112,   113,   396,
     396,   116,   117,   327,   382,   327,   382,   327,   382,   382,
     327,   382,   382,   327,    45,    47,   322,    84,    86,    89,
     322,    91,    93,    96,    96,   322,   322,   101,   101,   103,
     322,   322,   328,   322,   328,   322,   328,   322,   328,    18,
      19,   322,   382,   322,   357,   322,    32,   322,    35,    37,
     322,    38,    40,   322,    41,    43,   322,    51,   322,    51,
      51,    51,    51,    51,    57,    58,   322,    60,    61,   322,
      63,    64,   322,    66,    68,   322,    69,    70,   322,    74,
     322,    76,    77,    79,   322,    82,   322,   323,   322,   322,
     322,   131,   322,   134,   322,   137,   322,   140,   322,   143,
     322,   146,   322,   149,   322,   152,   322,   155,   322,   158,
     322,   161,   322,   328,   322,   328,   322,   328,   322,   328,
     322,   328,   322,   328,   322,   328,   322,   186,   187,   189,
     190,   192,   193,   195,   196,   198,   199,   261,   262,   349,
     350,   351,   352,   353,   354,   355,   322,   203,   322,   206,
     322,   209,   322,   328,   322,   215,   322,   328,   322,   224,
     322,   227,   322,   230,   322,   328,   322,   245,   322,   248,
     322,   251,   322,   254,   322,   257,   322,   260,   322,   328,
     322,   328,   322,   328,   322,   328,   322,   279,   280,   347,
     348,   322,   284,   322,   287,   322,   291,   322,   294,   322,
     297,   322,   328,   322,   303,   322,   306,   322,   309,   322,
     312,   322,   318,   322,   321,   322,   327,   393,   327,   393,
     327,   393,   327,   327,   393,   327,   393,   327,   393,   327,
     393,   327,   393,   327,   393,   327,   393,   327,   393,   327,
     393,   327,   393,   327,   393,   327,   393,   327,   393,   327,
     393,   327,   393,     5,   327,   393,   382,   393,    34,   382,
     393,   358,   393,   117,   118,   119,   120,   121,   322,   324,
     322,   116,   328,    29,   357,   221,   328,   236,   239,   328,
     242,   315,   328,   116,   116,   116,    96,   116,   116,   116,
     382,   327,   382,   327,   382,   327,   382,   382,    23,   358,
     382,   327,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   117,   106,   125,   128,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   327,   382,   327,   382,   327,   382,   327,   382,   327,
     382,   327,   382,   327,   382,   393,   382,   393,   382,   393,
     197,   327,   393,   382,   393,   263,   327,   393,   350,   351,
     352,   353,   354,   355,   382,   327,   382,   382,   382,   382,
     327,   382,   382,   327,   382,   382,   382,   382,   327,   382,
     382,   382,   382,   382,   382,   382,   327,   382,   327,   382,
     327,   382,   327,   281,   327,   393,   348,   382,   327,   382,
     382,   382,   382,   382,   382,   327,   382,   382,   382,   382,
     382,   382,   328,   322,   328,   322,   328,   322,   328,   328,
     322,   328,   322,   328,   322,   328,   322,   328,   322,   328,
     322,   328,   322,   328,   322,   328,   322,   328,   322,   328,
     322,   328,   322,   328,   322,   328,   322,   328,   322,   357,
     322,    32,   322,   248,   322,   382,   322,   396,   396,   396,
     396,   396,   398,   117,   402,   382,   382,   382,   382,   382,
      47,    89,    96,    96,   103,   101,     8,   328,    11,   328,
      13,   328,    17,    19,   382,    26,   357,    32,    37,    40,
      43,    51,    58,    61,    64,    68,    70,    74,    79,    82,
     396,   131,   134,   137,   140,   143,   146,   149,   152,   155,
     158,   161,   164,   328,   167,   328,   170,   328,   173,   328,
     176,   328,   179,   328,   182,   328,   188,   322,   191,   322,
     194,   322,   357,   322,   200,   322,   328,   322,   185,   349,
     203,   206,   209,   212,   328,   215,   218,   328,   224,   227,
     230,   233,   328,   245,   248,   251,   254,   257,   260,   266,
     328,   269,   328,   272,   328,   275,   328,   349,   322,   278,
     347,   284,   287,   291,   294,   297,   300,   328,   303,   306,
     309,   312,   318,   321,   382,   327,   382,   327,   382,   327,
     382,   382,   327,   382,   327,   382,   327,   382,   327,   382,
     327,   382,   327,   382,   327,   382,   327,   382,   327,   382,
     327,   382,   327,   382,   327,   382,   327,   382,   327,   382,
     327,   382,   327,   382,   382,    23,   358,   328,   396,   328,
       5,    29,   221,   239,   315,   382,   382,   382,    23,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   327,   382,   382,   327,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   327,   382,   382,     8,   328,    11,
     328,    13,   328,    17,   164,   328,   167,   328,   170,   328,
     173,   328,   176,   328,   179,   328,   182,   328,   212,   328,
     218,   328,   233,   328,   266,   328,   269,   328,   272,   328,
     275,   328,   300,   328,    26,   357,    32,   248,   382,   108,
     399,   110,   401,     8,    11,    13,    26,   164,   167,   170,
     173,   176,   179,   182,   188,   191,   194,   197,   357,   200,
     263,   328,   185,   212,   218,   233,   266,   269,   272,   275,
     281,   349,   278,   300,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,    23,   382,   382,   382,     8,    11,    13,
     164,   167,   170,   173,   176,   179,   182,   212,   218,   233,
     266,   269,   272,   275,   300,    26,   197,   263,   281
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   325,   326,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   327,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   330,   330,   330,   331,   331,   331,   332,   332,
     332,   333,   333,   333,   334,   334,   334,   335,   335,   335,
     335,   335,   335,   335,   336,   336,   336,   337,   337,   337,
     338,   338,   338,   339,   339,   339,   340,   340,   340,   341,
     341,   342,   343,   343,   344,   344,   345,   345,   345,   346,
     346,   346,   347,   347,   348,   348,   348,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   350,
     350,   350,   351,   351,   352,   352,   353,   353,   354,   354,
     354,   355,   355,   356,   356,   356,   356,   356,   357,   357,
     357,   357,   358,   358,   358,   358,   358,   359,   359,   359,
     359,   360,   360,   361,   361,   361,   361,   362,   362,   363,
     363,   364,   364,   365,   365,   366,   366,   367,   367,   368,
     368,   369,   369,   370,   370,   371,   371,   372,   372,   373,
     373,   374,   374,   375,   375,   376,   376,   377,   377,   378,
     378,   379,   379,   380,   380,   381,   381,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   383,   384,
     385,   386,   387,   388,   388,   388,   388,   388,   389,   389,
     389,   390,   390,   391,   391,   392,   392,   392,   393,   393,
     394,   394,   394,   394,   395,   396,   396,   396,   396,   396,
     396,   396,   396,   397,   398,   399,   400,   401,   402,   403,
     403,   404,   404,   405,   405,   406,   406,   407,   407,   408,
     408,   409,   409,   410,   410,   411,   411,   412,   412,   413,
     413,   414,   414,   415,   415
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
       1,     1,     3,     5,     3,     3,     5,     3,     3,     5,
       3,     3,     5,     3,     3,     5,     3,     3,     5,     3,
       3,     3,     3,     3,     3,     5,     3,     3,     5,     3,
       3,     5,     3,     3,     5,     3,     3,     5,     3,     3,
       5,     3,     3,     5,     3,     5,     2,     5,     7,     2,
       5,     7,     1,     2,     2,     5,     7,     1,     1,     1,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     2,
       5,     7,     3,     5,     3,     5,     3,     5,     2,     5,
       7,     3,     5,     2,     4,     5,     6,     7,     1,     1,
       1,     1,     2,     5,     6,     7,     8,     3,     4,     5,
       6,     2,     3,     3,     4,     5,     6,     2,     4,     2,
       4,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     0,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     5,     3,     5,     4,     3,     3,
       5,     3,     5,     3,     5,     3,     2,     5,     3,     4,
       4,     1,     6,     6,     4,     1,     1,     3,     3,     3,
       3,     3,     4,     1,     0,     1,     1,     1,     0,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5,     3,     5,     3,     5,     3,
       5,     3,     5,     3,     5
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
#line 253 "gtkdialog_parser.y"
                                       {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_WINDOW);
		start_up();
	}
#line 4476 "gtkdialog_parser.c"
    break;

  case 3: /* window: WINDOW radio_group_scope wlist attr EWINDOW  */
#line 258 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_WINDOW);
		start_up();
	}
#line 4486 "gtkdialog_parser.c"
    break;

  case 4: /* window: PART_WINDOW tagattr '>' radio_group_scope wlist attr EWINDOW  */
#line 263 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_WINDOW, (yyvsp[-5].nvval));
		start_up();
	}
#line 4496 "gtkdialog_parser.c"
    break;

  case 5: /* window: ASSISTANT radio_group_scope wlist attr EASSISTANT  */
#line 268 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ASSISTANT);
		start_up();
	}
#line 4506 "gtkdialog_parser.c"
    break;

  case 6: /* window: PART_ASSISTANT tagattr '>' radio_group_scope wlist attr EASSISTANT  */
#line 273 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ASSISTANT, (yyvsp[-5].nvval));
		start_up();
	}
#line 4516 "gtkdialog_parser.c"
    break;

  case 7: /* window: MESSAGEDIALOG attr EMESSAGEDIALOG  */
#line 278 "gtkdialog_parser.y"
                                            {
		token_store(PUSH | WIDGET_MESSAGEDIALOG);
		start_up();
	}
#line 4525 "gtkdialog_parser.c"
    break;

  case 8: /* window: PART_MESSAGEDIALOG tagattr '>' attr EMESSAGEDIALOG  */
#line 282 "gtkdialog_parser.y"
                                                             {
		token_store_attr(PUSH | WIDGET_MESSAGEDIALOG, (yyvsp[-3].nvval));
		start_up();
	}
#line 4534 "gtkdialog_parser.c"
    break;

  case 9: /* window: DIALOG radio_group_scope wlist attr EDIALOG  */
#line 286 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_DIALOG);
		start_up();
	}
#line 4544 "gtkdialog_parser.c"
    break;

  case 10: /* window: PART_DIALOG tagattr '>' radio_group_scope wlist attr EDIALOG  */
#line 291 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_DIALOG, (yyvsp[-5].nvval));
		start_up();
	}
#line 4554 "gtkdialog_parser.c"
    break;

  case 11: /* window: ABOUTDIALOG attr EABOUTDIALOG  */
#line 296 "gtkdialog_parser.y"
                                        {
		token_store(PUSH | WIDGET_ABOUTDIALOG);
		start_up();
	}
#line 4563 "gtkdialog_parser.c"
    break;

  case 12: /* window: PART_ABOUTDIALOG tagattr '>' attr EABOUTDIALOG  */
#line 300 "gtkdialog_parser.y"
                                                         {
		token_store_attr(PUSH | WIDGET_ABOUTDIALOG, (yyvsp[-3].nvval));
		start_up();
	}
#line 4572 "gtkdialog_parser.c"
    break;

  case 13: /* window: POPUPMENU EPOPUPMENU  */
#line 304 "gtkdialog_parser.y"
                               {
		yyerror("The popupmenu widget requires at least one menuitem widget.");
	}
#line 4580 "gtkdialog_parser.c"
    break;

  case 14: /* window: POPUPMENU radio_group_scope menuwlist attr EPOPUPMENU  */
#line 307 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_POPUPMENU);
		start_up();
	}
#line 4590 "gtkdialog_parser.c"
    break;

  case 15: /* window: PART_POPUPMENU tagattr '>' radio_group_scope menuwlist attr EPOPUPMENU  */
#line 312 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_POPUPMENU, (yyvsp[-5].nvval));
		start_up();
	}
#line 4600 "gtkdialog_parser.c"
    break;

  case 16: /* window: PLUG radio_group_scope wlist attr EPLUG  */
#line 317 "gtkdialog_parser.y"
                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_PLUG);
		start_up();
	}
#line 4610 "gtkdialog_parser.c"
    break;

  case 17: /* window: PART_PLUG tagattr '>' radio_group_scope wlist attr EPLUG  */
#line 322 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_PLUG, (yyvsp[-5].nvval));
		start_up();
	}
#line 4620 "gtkdialog_parser.c"
    break;

  case 18: /* radio_group_scope: %empty  */
#line 330 "gtkdialog_parser.y"
    { token_store(RGROUP_PUSH); }
#line 4626 "gtkdialog_parser.c"
    break;

  case 20: /* wlist: wlist widget  */
#line 337 "gtkdialog_parser.y"
                       {
		token_store(SUM);
	}
#line 4634 "gtkdialog_parser.c"
    break;

  case 23: /* wlist: ALIGNMENT radio_group_scope wlist attr EALIGNMENT  */
#line 342 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ALIGNMENT);
	}
#line 4643 "gtkdialog_parser.c"
    break;

  case 24: /* wlist: wlist ALIGNMENT radio_group_scope wlist attr EALIGNMENT  */
#line 346 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ALIGNMENT);
		token_store(SUM);
	}
#line 4653 "gtkdialog_parser.c"
    break;

  case 25: /* wlist: PART_ALIGNMENT tagattr '>' radio_group_scope wlist attr EALIGNMENT  */
#line 351 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ALIGNMENT, (yyvsp[-5].nvval));
	}
#line 4662 "gtkdialog_parser.c"
    break;

  case 26: /* wlist: wlist PART_ALIGNMENT tagattr '>' radio_group_scope wlist attr EALIGNMENT  */
#line 355 "gtkdialog_parser.y"
                                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ALIGNMENT, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4672 "gtkdialog_parser.c"
    break;

  case 27: /* wlist: ASPECTFRAME radio_group_scope wlist attr EASPECTFRAME  */
#line 360 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ASPECTFRAME);
	}
#line 4681 "gtkdialog_parser.c"
    break;

  case 28: /* wlist: wlist ASPECTFRAME radio_group_scope wlist attr EASPECTFRAME  */
#line 364 "gtkdialog_parser.y"
                                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_ASPECTFRAME);
		token_store(SUM);
	}
#line 4691 "gtkdialog_parser.c"
    break;

  case 29: /* wlist: PART_ASPECTFRAME tagattr '>' radio_group_scope wlist attr EASPECTFRAME  */
#line 369 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ASPECTFRAME, (yyvsp[-5].nvval));
	}
#line 4700 "gtkdialog_parser.c"
    break;

  case 30: /* wlist: wlist PART_ASPECTFRAME tagattr '>' radio_group_scope wlist attr EASPECTFRAME  */
#line 373 "gtkdialog_parser.y"
                                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_ASPECTFRAME, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4710 "gtkdialog_parser.c"
    break;

  case 31: /* wlist: GRID radio_group_scope wlist attr EGRID  */
#line 378 "gtkdialog_parser.y"
                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_GRID);
	}
#line 4719 "gtkdialog_parser.c"
    break;

  case 32: /* wlist: wlist GRID radio_group_scope wlist attr EGRID  */
#line 382 "gtkdialog_parser.y"
                                                        {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_GRID);
		token_store(SUM);
	}
#line 4729 "gtkdialog_parser.c"
    break;

  case 33: /* wlist: PART_GRID tagattr '>' radio_group_scope wlist attr EGRID  */
#line 387 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_GRID, (yyvsp[-5].nvval));
	}
#line 4738 "gtkdialog_parser.c"
    break;

  case 34: /* wlist: wlist PART_GRID tagattr '>' radio_group_scope wlist attr EGRID  */
#line 391 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_GRID, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4748 "gtkdialog_parser.c"
    break;

  case 35: /* wlist: FIXED radio_group_scope wlist attr EFIXED  */
#line 396 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_FIXED);
	}
#line 4757 "gtkdialog_parser.c"
    break;

  case 36: /* wlist: wlist FIXED radio_group_scope wlist attr EFIXED  */
#line 400 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_FIXED);
		token_store(SUM);
	}
#line 4767 "gtkdialog_parser.c"
    break;

  case 37: /* wlist: PART_FIXED tagattr '>' radio_group_scope wlist attr EFIXED  */
#line 405 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_FIXED, (yyvsp[-5].nvval));
	}
#line 4776 "gtkdialog_parser.c"
    break;

  case 38: /* wlist: wlist PART_FIXED tagattr '>' radio_group_scope wlist attr EFIXED  */
#line 409 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_FIXED, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4786 "gtkdialog_parser.c"
    break;

  case 39: /* wlist: LAYOUT radio_group_scope wlist attr ELAYOUT  */
#line 414 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_LAYOUT);
	}
#line 4795 "gtkdialog_parser.c"
    break;

  case 40: /* wlist: wlist LAYOUT radio_group_scope wlist attr ELAYOUT  */
#line 418 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_LAYOUT);
		token_store(SUM);
	}
#line 4805 "gtkdialog_parser.c"
    break;

  case 41: /* wlist: PART_LAYOUT tagattr '>' radio_group_scope wlist attr ELAYOUT  */
#line 423 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_LAYOUT, (yyvsp[-5].nvval));
	}
#line 4814 "gtkdialog_parser.c"
    break;

  case 42: /* wlist: wlist PART_LAYOUT tagattr '>' radio_group_scope wlist attr ELAYOUT  */
#line 427 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_LAYOUT, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4824 "gtkdialog_parser.c"
    break;

  case 43: /* wlist: SCROLLEDWINDOW radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 432 "gtkdialog_parser.y"
                                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_SCROLLEDWINDOW);
	}
#line 4833 "gtkdialog_parser.c"
    break;

  case 44: /* wlist: wlist SCROLLEDWINDOW radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 436 "gtkdialog_parser.y"
                                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_SCROLLEDWINDOW);
		token_store(SUM);
	}
#line 4843 "gtkdialog_parser.c"
    break;

  case 45: /* wlist: PART_SCROLLEDWINDOW tagattr '>' radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 441 "gtkdialog_parser.y"
                                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_SCROLLEDWINDOW, (yyvsp[-5].nvval));
	}
#line 4852 "gtkdialog_parser.c"
    break;

  case 46: /* wlist: wlist PART_SCROLLEDWINDOW tagattr '>' radio_group_scope wlist attr ESCROLLEDWINDOW  */
#line 445 "gtkdialog_parser.y"
                                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_SCROLLEDWINDOW, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4862 "gtkdialog_parser.c"
    break;

  case 47: /* wlist: VIEWPORT radio_group_scope wlist attr EVIEWPORT  */
#line 450 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VIEWPORT);
	}
#line 4871 "gtkdialog_parser.c"
    break;

  case 48: /* wlist: wlist VIEWPORT radio_group_scope wlist attr EVIEWPORT  */
#line 454 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VIEWPORT);
		token_store(SUM);
	}
#line 4881 "gtkdialog_parser.c"
    break;

  case 49: /* wlist: PART_VIEWPORT tagattr '>' radio_group_scope wlist attr EVIEWPORT  */
#line 459 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VIEWPORT, (yyvsp[-5].nvval));
	}
#line 4890 "gtkdialog_parser.c"
    break;

  case 50: /* wlist: wlist PART_VIEWPORT tagattr '>' radio_group_scope wlist attr EVIEWPORT  */
#line 463 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VIEWPORT, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4900 "gtkdialog_parser.c"
    break;

  case 51: /* wlist: HANDLEBOX radio_group_scope wlist attr EHANDLEBOX  */
#line 468 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HANDLEBOX);
	}
#line 4909 "gtkdialog_parser.c"
    break;

  case 52: /* wlist: wlist HANDLEBOX radio_group_scope wlist attr EHANDLEBOX  */
#line 472 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HANDLEBOX);
		token_store(SUM);
	}
#line 4919 "gtkdialog_parser.c"
    break;

  case 53: /* wlist: PART_HANDLEBOX tagattr '>' radio_group_scope wlist attr EHANDLEBOX  */
#line 477 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HANDLEBOX, (yyvsp[-5].nvval));
	}
#line 4928 "gtkdialog_parser.c"
    break;

  case 54: /* wlist: wlist PART_HANDLEBOX tagattr '>' radio_group_scope wlist attr EHANDLEBOX  */
#line 481 "gtkdialog_parser.y"
                                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HANDLEBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4938 "gtkdialog_parser.c"
    break;

  case 55: /* wlist: VBOX radio_group_scope wlist attr EVBOX  */
#line 486 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBOX);
	}
#line 4947 "gtkdialog_parser.c"
    break;

  case 56: /* wlist: wlist VBOX radio_group_scope wlist attr EVBOX  */
#line 490 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBOX);
		token_store(SUM);
	}
#line 4957 "gtkdialog_parser.c"
    break;

  case 57: /* wlist: PART_VBOX tagattr '>' radio_group_scope wlist attr EVBOX  */
#line 495 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBOX, (yyvsp[-5].nvval));
	}
#line 4966 "gtkdialog_parser.c"
    break;

  case 58: /* wlist: wlist PART_VBOX tagattr '>' radio_group_scope wlist attr EVBOX  */
#line 499 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 4976 "gtkdialog_parser.c"
    break;

  case 59: /* wlist: HBOX radio_group_scope wlist attr EHBOX  */
#line 504 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBOX);
	}
#line 4985 "gtkdialog_parser.c"
    break;

  case 60: /* wlist: wlist HBOX radio_group_scope wlist attr EHBOX  */
#line 508 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBOX);
		token_store(SUM);
	}
#line 4995 "gtkdialog_parser.c"
    break;

  case 61: /* wlist: PART_HBOX tagattr '>' radio_group_scope wlist attr EHBOX  */
#line 513 "gtkdialog_parser.y"
                                                                   {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBOX, (yyvsp[-5].nvval));
	}
#line 5004 "gtkdialog_parser.c"
    break;

  case 62: /* wlist: wlist PART_HBOX tagattr '>' radio_group_scope wlist attr EHBOX  */
#line 517 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5014 "gtkdialog_parser.c"
    break;

  case 63: /* wlist: HBUTTONBOX radio_group_scope wlist attr EHBUTTONBOX  */
#line 522 "gtkdialog_parser.y"
                                                              {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBUTTONBOX);
	}
#line 5023 "gtkdialog_parser.c"
    break;

  case 64: /* wlist: wlist HBUTTONBOX radio_group_scope wlist attr EHBUTTONBOX  */
#line 526 "gtkdialog_parser.y"
                                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HBUTTONBOX);
		token_store(SUM);
	}
#line 5033 "gtkdialog_parser.c"
    break;

  case 65: /* wlist: PART_HBUTTONBOX tagattr '>' radio_group_scope wlist attr EHBUTTONBOX  */
#line 531 "gtkdialog_parser.y"
                                                                               {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBUTTONBOX, (yyvsp[-5].nvval));
	}
#line 5042 "gtkdialog_parser.c"
    break;

  case 66: /* wlist: wlist PART_HBUTTONBOX tagattr '>' radio_group_scope wlist attr EHBUTTONBOX  */
#line 535 "gtkdialog_parser.y"
                                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HBUTTONBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5052 "gtkdialog_parser.c"
    break;

  case 67: /* wlist: VBUTTONBOX radio_group_scope wlist attr EVBUTTONBOX  */
#line 540 "gtkdialog_parser.y"
                                                              {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBUTTONBOX);
	}
#line 5061 "gtkdialog_parser.c"
    break;

  case 68: /* wlist: wlist VBUTTONBOX radio_group_scope wlist attr EVBUTTONBOX  */
#line 544 "gtkdialog_parser.y"
                                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VBUTTONBOX);
		token_store(SUM);
	}
#line 5071 "gtkdialog_parser.c"
    break;

  case 69: /* wlist: PART_VBUTTONBOX tagattr '>' radio_group_scope wlist attr EVBUTTONBOX  */
#line 549 "gtkdialog_parser.y"
                                                                               {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBUTTONBOX, (yyvsp[-5].nvval));
	}
#line 5080 "gtkdialog_parser.c"
    break;

  case 70: /* wlist: wlist PART_VBUTTONBOX tagattr '>' radio_group_scope wlist attr EVBUTTONBOX  */
#line 553 "gtkdialog_parser.y"
                                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VBUTTONBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5090 "gtkdialog_parser.c"
    break;

  case 71: /* wlist: HPANED radio_group_scope wlist attr EHPANED  */
#line 558 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HPANED);
	}
#line 5099 "gtkdialog_parser.c"
    break;

  case 72: /* wlist: wlist HPANED radio_group_scope wlist attr EHPANED  */
#line 562 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_HPANED);
		token_store(SUM);
	}
#line 5109 "gtkdialog_parser.c"
    break;

  case 73: /* wlist: PART_HPANED tagattr '>' radio_group_scope wlist attr EHPANED  */
#line 567 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HPANED, (yyvsp[-5].nvval));
	}
#line 5118 "gtkdialog_parser.c"
    break;

  case 74: /* wlist: wlist PART_HPANED tagattr '>' radio_group_scope wlist attr EHPANED  */
#line 571 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_HPANED, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5128 "gtkdialog_parser.c"
    break;

  case 75: /* wlist: VPANED radio_group_scope wlist attr EVPANED  */
#line 576 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VPANED);
	}
#line 5137 "gtkdialog_parser.c"
    break;

  case 76: /* wlist: wlist VPANED radio_group_scope wlist attr EVPANED  */
#line 580 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_VPANED);
		token_store(SUM);
	}
#line 5147 "gtkdialog_parser.c"
    break;

  case 77: /* wlist: PART_VPANED tagattr '>' radio_group_scope wlist attr EVPANED  */
#line 585 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VPANED, (yyvsp[-5].nvval));
	}
#line 5156 "gtkdialog_parser.c"
    break;

  case 78: /* wlist: wlist PART_VPANED tagattr '>' radio_group_scope wlist attr EVPANED  */
#line 589 "gtkdialog_parser.y"
                                                                             {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_VPANED, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5166 "gtkdialog_parser.c"
    break;

  case 79: /* wlist: EVENTBOX radio_group_scope wlist attr EEVENTBOX  */
#line 594 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EVENTBOX);
	}
#line 5175 "gtkdialog_parser.c"
    break;

  case 80: /* wlist: wlist EVENTBOX radio_group_scope wlist attr EEVENTBOX  */
#line 598 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EVENTBOX);
		token_store(SUM);
	}
#line 5185 "gtkdialog_parser.c"
    break;

  case 81: /* wlist: PART_EVENTBOX tagattr '>' radio_group_scope wlist attr EEVENTBOX  */
#line 603 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EVENTBOX, (yyvsp[-5].nvval));
	}
#line 5194 "gtkdialog_parser.c"
    break;

  case 82: /* wlist: wlist PART_EVENTBOX tagattr '>' radio_group_scope wlist attr EEVENTBOX  */
#line 607 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EVENTBOX, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5204 "gtkdialog_parser.c"
    break;

  case 83: /* wlist: EXPANDER radio_group_scope wlist attr EEXPANDER  */
#line 612 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EXPANDER);
	}
#line 5213 "gtkdialog_parser.c"
    break;

  case 84: /* wlist: wlist EXPANDER radio_group_scope wlist attr EEXPANDER  */
#line 616 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_EXPANDER);
		token_store(SUM);
	}
#line 5223 "gtkdialog_parser.c"
    break;

  case 85: /* wlist: PART_EXPANDER tagattr '>' radio_group_scope wlist attr EEXPANDER  */
#line 621 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EXPANDER, (yyvsp[-5].nvval));
	}
#line 5232 "gtkdialog_parser.c"
    break;

  case 86: /* wlist: wlist PART_EXPANDER tagattr '>' radio_group_scope wlist attr EEXPANDER  */
#line 625 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_EXPANDER, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5242 "gtkdialog_parser.c"
    break;

  case 87: /* wlist: INFOBAR radio_group_scope wlist attr EINFOBAR  */
#line 630 "gtkdialog_parser.y"
                                                        {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_INFOBAR);
	}
#line 5251 "gtkdialog_parser.c"
    break;

  case 88: /* wlist: wlist INFOBAR radio_group_scope wlist attr EINFOBAR  */
#line 634 "gtkdialog_parser.y"
                                                              {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_INFOBAR);
		token_store(SUM);
	}
#line 5261 "gtkdialog_parser.c"
    break;

  case 89: /* wlist: PART_INFOBAR tagattr '>' radio_group_scope wlist attr EINFOBAR  */
#line 639 "gtkdialog_parser.y"
                                                                         {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_INFOBAR, (yyvsp[-5].nvval));
	}
#line 5270 "gtkdialog_parser.c"
    break;

  case 90: /* wlist: wlist PART_INFOBAR tagattr '>' radio_group_scope wlist attr EINFOBAR  */
#line 643 "gtkdialog_parser.y"
                                                                               {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_INFOBAR, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5280 "gtkdialog_parser.c"
    break;

  case 91: /* wlist: NOTEBOOK radio_group_scope wlist attr ENOTEBOOK  */
#line 648 "gtkdialog_parser.y"
                                                            {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_NOTEBOOK);
	}
#line 5289 "gtkdialog_parser.c"
    break;

  case 92: /* wlist: wlist NOTEBOOK radio_group_scope wlist attr ENOTEBOOK  */
#line 652 "gtkdialog_parser.y"
                                                                  {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_NOTEBOOK);
		token_store(SUM);
	}
#line 5299 "gtkdialog_parser.c"
    break;

  case 93: /* wlist: PART_NOTEBOOK tagattr '>' radio_group_scope wlist attr ENOTEBOOK  */
#line 657 "gtkdialog_parser.y"
                                                                           {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_NOTEBOOK, (yyvsp[-5].nvval));
	}
#line 5308 "gtkdialog_parser.c"
    break;

  case 94: /* wlist: wlist PART_NOTEBOOK tagattr '>' radio_group_scope wlist attr ENOTEBOOK  */
#line 661 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_NOTEBOOK, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 5318 "gtkdialog_parser.c"
    break;

  case 95: /* wlist: FRAME radio_group_scope wlist attr EFRAME  */
#line 666 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store_owned_argument(SET|ATTR_LABEL, (yyvsp[-4].cval));
		token_store(PUSH | WIDGET_FRAME);
	}
#line 5328 "gtkdialog_parser.c"
    break;

  case 96: /* wlist: wlist FRAME radio_group_scope wlist attr EFRAME  */
#line 671 "gtkdialog_parser.y"
                                                          {
		token_store(RGROUP_POP);
		token_store_owned_argument(SET|ATTR_LABEL, (yyvsp[-4].cval));
		token_store(PUSH | WIDGET_FRAME);
		token_store(SUM);
	}
#line 5339 "gtkdialog_parser.c"
    break;

  case 124: /* widget: COLORSELECTION attr ECOLORSELECTION  */
#line 707 "gtkdialog_parser.y"
                                        {
		token_store(PUSH | WIDGET_COLORSELECTION);
	}
#line 5347 "gtkdialog_parser.c"
    break;

  case 125: /* widget: PART_COLORSELECTION tagattr '>' attr ECOLORSELECTION  */
#line 710 "gtkdialog_parser.y"
                                                         {
		token_store_attr(PUSH | WIDGET_COLORSELECTION, (yyvsp[-3].nvval));
	}
#line 5355 "gtkdialog_parser.c"
    break;

  case 127: /* widget: FONTSELECTION attr EFONTSELECTION  */
#line 714 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_FONTSELECTION);
	}
#line 5363 "gtkdialog_parser.c"
    break;

  case 128: /* widget: PART_FONTSELECTION tagattr '>' attr EFONTSELECTION  */
#line 717 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_FONTSELECTION, (yyvsp[-3].nvval));
	}
#line 5371 "gtkdialog_parser.c"
    break;

  case 152: /* entry: ENTRY attr EENTRY  */
#line 746 "gtkdialog_parser.y"
                      {
                          token_store(PUSH | WIDGET_ENTRY);
			 }
#line 5379 "gtkdialog_parser.c"
    break;

  case 153: /* entry: PART_ENTRY tagattr '>' attr EENTRY  */
#line 749 "gtkdialog_parser.y"
                                       {
                token_store_attr(PUSH | WIDGET_ENTRY, (yyvsp[-3].nvval));
	}
#line 5387 "gtkdialog_parser.c"
    break;

  case 154: /* entry: ENTRY attr ENTRY  */
#line 752 "gtkdialog_parser.y"
                     {
                  yyerror("</entry> expected instead of <entry>.");}
#line 5394 "gtkdialog_parser.c"
    break;

  case 155: /* edit: EDIT attr EEDIT  */
#line 757 "gtkdialog_parser.y"
                     {
		token_store(PUSH | WIDGET_EDIT);
	}
#line 5402 "gtkdialog_parser.c"
    break;

  case 156: /* edit: PART_EDIT tagattr '>' attr EEDIT  */
#line 760 "gtkdialog_parser.y"
                                     {
		token_store_attr(PUSH | WIDGET_EDIT, (yyvsp[-3].nvval));
	}
#line 5410 "gtkdialog_parser.c"
    break;

  case 157: /* edit: EDIT attr EDIT  */
#line 763 "gtkdialog_parser.y"
                     {
		yyerror("</edit> expected instead of <edit>.");
	}
#line 5418 "gtkdialog_parser.c"
    break;

  case 158: /* tree: TREE attr ETREE  */
#line 769 "gtkdialog_parser.y"
                     {
		token_store(PUSH | WIDGET_TREE);
	}
#line 5426 "gtkdialog_parser.c"
    break;

  case 159: /* tree: PART_TREE tagattr '>' attr ETREE  */
#line 772 "gtkdialog_parser.y"
                                     {
		token_store_attr(PUSH | WIDGET_TREE, (yyvsp[-3].nvval));
	}
#line 5434 "gtkdialog_parser.c"
    break;

  case 160: /* tree: TREE attr TREE  */
#line 775 "gtkdialog_parser.y"
                   {
		yyerror("</tree> expected instead of <tree>.");
	}
#line 5442 "gtkdialog_parser.c"
    break;

  case 161: /* chooser: CHOOSER attr ECHOOSER  */
#line 781 "gtkdialog_parser.y"
                           {
		token_store(PUSH | WIDGET_CHOOSER);
	}
#line 5450 "gtkdialog_parser.c"
    break;

  case 162: /* chooser: PART_CHOOSER tagattr '>' attr ECHOOSER  */
#line 784 "gtkdialog_parser.y"
                                           {
		token_store_attr(PUSH | WIDGET_CHOOSER, (yyvsp[-3].nvval));
	}
#line 5458 "gtkdialog_parser.c"
    break;

  case 163: /* chooser: CHOOSER attr CHOOSER  */
#line 787 "gtkdialog_parser.y"
                         {
		yyerror("</chooser> expected instead of <chooser>.");
	}
#line 5466 "gtkdialog_parser.c"
    break;

  case 164: /* text: TEXT attr ETEXT  */
#line 793 "gtkdialog_parser.y"
                    {
		token_store(PUSH | WIDGET_TEXT);
	}
#line 5474 "gtkdialog_parser.c"
    break;

  case 165: /* text: PART_TEXT tagattr '>' attr ETEXT  */
#line 796 "gtkdialog_parser.y"
                                     {
                token_store_attr(PUSH | WIDGET_TEXT, (yyvsp[-3].nvval));
	}
#line 5482 "gtkdialog_parser.c"
    break;

  case 166: /* text: TEXT attr TEXT  */
#line 799 "gtkdialog_parser.y"
                    {yyerror("</text> expected instead of <text>.");}
#line 5488 "gtkdialog_parser.c"
    break;

  case 167: /* button: BUTTON attr EBUTTON  */
#line 803 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_BUTTON);  }
#line 5494 "gtkdialog_parser.c"
    break;

  case 168: /* button: PART_BUTTON tagattr '>' attr EBUTTON  */
#line 804 "gtkdialog_parser.y"
                                         {
                token_store_attr(PUSH | WIDGET_BUTTON, (yyvsp[-3].nvval));
	}
#line 5502 "gtkdialog_parser.c"
    break;

  case 169: /* button: BUTTONOK attr EBUTTON  */
#line 807 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_OKBUTTON);}
#line 5508 "gtkdialog_parser.c"
    break;

  case 170: /* button: BUTTONCANCEL attr EBUTTON  */
#line 808 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_CANCELBUTTON);}
#line 5514 "gtkdialog_parser.c"
    break;

  case 171: /* button: BUTTONHELP attr EBUTTON  */
#line 809 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_HELPBUTTON);}
#line 5520 "gtkdialog_parser.c"
    break;

  case 172: /* button: BUTTONNO attr EBUTTON  */
#line 810 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_NOBUTTON);}
#line 5526 "gtkdialog_parser.c"
    break;

  case 173: /* button: BUTTONYES attr EBUTTON  */
#line 811 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_YESBUTTON);}
#line 5532 "gtkdialog_parser.c"
    break;

  case 174: /* checkbox: CHECKBOX attr ECHECKBOX  */
#line 815 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_CHECKBOX);
	}
#line 5540 "gtkdialog_parser.c"
    break;

  case 175: /* checkbox: PART_CHECKBOX tagattr '>' attr ECHECKBOX  */
#line 818 "gtkdialog_parser.y"
                                             {
                token_store_attr(PUSH | WIDGET_CHECKBOX, (yyvsp[-3].nvval));
	}
#line 5548 "gtkdialog_parser.c"
    break;

  case 176: /* checkbox: CHECKBOX attr CHECKBOX  */
#line 821 "gtkdialog_parser.y"
                            {
		yyerror("</checkbox> expected instead of <checkbox>.");
	}
#line 5556 "gtkdialog_parser.c"
    break;

  case 177: /* radiobutton: RADIO attr ERADIO  */
#line 827 "gtkdialog_parser.y"
                         {
		token_store(PUSH | WIDGET_RADIOBUTTON);
           }
#line 5564 "gtkdialog_parser.c"
    break;

  case 178: /* radiobutton: PART_RADIO tagattr '>' attr ERADIO  */
#line 830 "gtkdialog_parser.y"
                                       {
                token_store_attr(PUSH | WIDGET_RADIOBUTTON, (yyvsp[-3].nvval));
	   }
#line 5572 "gtkdialog_parser.c"
    break;

  case 179: /* radiobutton: RADIO attr RADIO  */
#line 833 "gtkdialog_parser.y"
                      {
		yyerror("</radiobutton> expected instead of <radiobutton>.");
           }
#line 5580 "gtkdialog_parser.c"
    break;

  case 180: /* progressbar: PROGRESSBAR attr EPROGRESSBAR  */
#line 839 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_PROGRESSBAR);
           }
#line 5588 "gtkdialog_parser.c"
    break;

  case 181: /* progressbar: PART_PROGRESSBAR tagattr '>' attr EPROGRESSBAR  */
#line 842 "gtkdialog_parser.y"
                                                   {
                token_store_attr(PUSH | WIDGET_PROGRESSBAR, (yyvsp[-3].nvval));
	   }
#line 5596 "gtkdialog_parser.c"
    break;

  case 182: /* progressbar: PROGRESSBAR attr PROGRESSBAR  */
#line 845 "gtkdialog_parser.y"
                                  {
		yyerror("</progressbar> expected instead of <progressbar>.");
           }
#line 5604 "gtkdialog_parser.c"
    break;

  case 183: /* list: LIST attr ELIST  */
#line 851 "gtkdialog_parser.y"
                    {
		token_store(PUSH | WIDGET_LIST);
	}
#line 5612 "gtkdialog_parser.c"
    break;

  case 184: /* list: PART_LIST tagattr '>' attr ELIST  */
#line 854 "gtkdialog_parser.y"
                                     {
		token_store_attr(PUSH | WIDGET_LIST, (yyvsp[-3].nvval));
	}
#line 5620 "gtkdialog_parser.c"
    break;

  case 185: /* list: LIST attr LIST  */
#line 857 "gtkdialog_parser.y"
                     {
		yyerror("</list> expected instead of <list>.");
	}
#line 5628 "gtkdialog_parser.c"
    break;

  case 186: /* table: TABLE attr ETABLE  */
#line 863 "gtkdialog_parser.y"
                      {
		token_store(PUSH | WIDGET_TABLE);
	}
#line 5636 "gtkdialog_parser.c"
    break;

  case 187: /* table: PART_TABLE tagattr '>' attr ETABLE  */
#line 866 "gtkdialog_parser.y"
                                       {
		token_store_attr(PUSH | WIDGET_TABLE, (yyvsp[-3].nvval));
	}
#line 5644 "gtkdialog_parser.c"
    break;

  case 188: /* table: TABLE attr TABLE  */
#line 869 "gtkdialog_parser.y"
                       {
		yyerror("</table> expected instead of <table>.");
	}
#line 5652 "gtkdialog_parser.c"
    break;

  case 189: /* combobox: COMBOBOX attr ECOMBOBOX  */
#line 875 "gtkdialog_parser.y"
                                             {
		token_store(PUSH | WIDGET_COMBOBOX);
	}
#line 5660 "gtkdialog_parser.c"
    break;

  case 190: /* combobox: PART_COMBOBOX tagattr '>' attr ECOMBOBOX  */
#line 878 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_COMBOBOX, (yyvsp[-3].nvval));
	}
#line 5668 "gtkdialog_parser.c"
    break;

  case 191: /* gvim: GVIM attr EGVIM  */
#line 884 "gtkdialog_parser.y"
                                {token_store(PUSH | WIDGET_GVIM);}
#line 5674 "gtkdialog_parser.c"
    break;

  case 192: /* pixmap: PIXMAP attr EPIXMAP  */
#line 888 "gtkdialog_parser.y"
                              {token_store(PUSH | WIDGET_PIXMAP);}
#line 5680 "gtkdialog_parser.c"
    break;

  case 193: /* pixmap: PART_PIXMAP tagattr '>' attr EPIXMAP  */
#line 889 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_PIXMAP, (yyvsp[-3].nvval));
	}
#line 5688 "gtkdialog_parser.c"
    break;

  case 194: /* calendar: CALENDAR attr ECALENDAR  */
#line 895 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_CALENDAR);
	}
#line 5696 "gtkdialog_parser.c"
    break;

  case 195: /* calendar: PART_CALENDAR tagattr '>' attr ECALENDAR  */
#line 898 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_CALENDAR, (yyvsp[-3].nvval));
	}
#line 5704 "gtkdialog_parser.c"
    break;

  case 196: /* toolbar: TOOLBAR ETOOLBAR  */
#line 911 "gtkdialog_parser.y"
                     {
		yyerror("The toolbar widget requires at least one tool item.");
	}
#line 5712 "gtkdialog_parser.c"
    break;

  case 197: /* toolbar: TOOLBAR radio_group_scope toolbarwlist attr ETOOLBAR  */
#line 914 "gtkdialog_parser.y"
                                                         {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLBAR);
	}
#line 5721 "gtkdialog_parser.c"
    break;

  case 198: /* toolbar: PART_TOOLBAR tagattr '>' radio_group_scope toolbarwlist attr ETOOLBAR  */
#line 918 "gtkdialog_parser.y"
                                                                          {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLBAR, (yyvsp[-5].nvval));
	}
#line 5730 "gtkdialog_parser.c"
    break;

  case 199: /* toolpalette: TOOLPALETTE ETOOLPALETTE  */
#line 925 "gtkdialog_parser.y"
                             {
		yyerror("The toolpalette widget requires at least one toolitemgroup.");
	}
#line 5738 "gtkdialog_parser.c"
    break;

  case 200: /* toolpalette: TOOLPALETTE radio_group_scope toolpalettewlist attr ETOOLPALETTE  */
#line 928 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLPALETTE);
	}
#line 5747 "gtkdialog_parser.c"
    break;

  case 201: /* toolpalette: PART_TOOLPALETTE tagattr '>' radio_group_scope toolpalettewlist attr ETOOLPALETTE  */
#line 932 "gtkdialog_parser.y"
                                                                                      {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLPALETTE, (yyvsp[-5].nvval));
	}
#line 5756 "gtkdialog_parser.c"
    break;

  case 203: /* toolpalettewlist: toolpalettewlist toolitemgroup  */
#line 940 "gtkdialog_parser.y"
                                   {
		token_store(SUM);
	}
#line 5764 "gtkdialog_parser.c"
    break;

  case 204: /* toolitemgroup: TOOLITEMGROUP ETOOLITEMGROUP  */
#line 946 "gtkdialog_parser.y"
                                 {
		yyerror("The toolitemgroup widget requires at least one tool item.");
	}
#line 5772 "gtkdialog_parser.c"
    break;

  case 205: /* toolitemgroup: TOOLITEMGROUP radio_group_scope toolbarwlist attr ETOOLITEMGROUP  */
#line 949 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLITEMGROUP);
	}
#line 5781 "gtkdialog_parser.c"
    break;

  case 206: /* toolitemgroup: PART_TOOLITEMGROUP tagattr '>' radio_group_scope toolbarwlist attr ETOOLITEMGROUP  */
#line 953 "gtkdialog_parser.y"
                                                                                      {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLITEMGROUP, (yyvsp[-5].nvval));
	}
#line 5790 "gtkdialog_parser.c"
    break;

  case 213: /* toolbarwlist: toolbarwlist toolbutton  */
#line 966 "gtkdialog_parser.y"
                            {
		token_store(SUM);
	}
#line 5798 "gtkdialog_parser.c"
    break;

  case 214: /* toolbarwlist: toolbarwlist toolitem  */
#line 969 "gtkdialog_parser.y"
                          {
		token_store(SUM);
	}
#line 5806 "gtkdialog_parser.c"
    break;

  case 215: /* toolbarwlist: toolbarwlist toggletoolbutton  */
#line 972 "gtkdialog_parser.y"
                                  {
		token_store(SUM);
	}
#line 5814 "gtkdialog_parser.c"
    break;

  case 216: /* toolbarwlist: toolbarwlist radiotoolbutton  */
#line 975 "gtkdialog_parser.y"
                                 {
		token_store(SUM);
	}
#line 5822 "gtkdialog_parser.c"
    break;

  case 217: /* toolbarwlist: toolbarwlist menutoolbutton  */
#line 978 "gtkdialog_parser.y"
                                {
		token_store(SUM);
	}
#line 5830 "gtkdialog_parser.c"
    break;

  case 218: /* toolbarwlist: toolbarwlist separatortoolitem  */
#line 981 "gtkdialog_parser.y"
                                   {
		token_store(SUM);
	}
#line 5838 "gtkdialog_parser.c"
    break;

  case 219: /* toolitem: TOOLITEM ETOOLITEM  */
#line 987 "gtkdialog_parser.y"
                       {
		yyerror("The toolitem widget requires exactly one child widget.");
	}
#line 5846 "gtkdialog_parser.c"
    break;

  case 220: /* toolitem: TOOLITEM radio_group_scope wlist attr ETOOLITEM  */
#line 990 "gtkdialog_parser.y"
                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_TOOLITEM);
	}
#line 5855 "gtkdialog_parser.c"
    break;

  case 221: /* toolitem: PART_TOOLITEM tagattr '>' radio_group_scope wlist attr ETOOLITEM  */
#line 994 "gtkdialog_parser.y"
                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_TOOLITEM, (yyvsp[-5].nvval));
	}
#line 5864 "gtkdialog_parser.c"
    break;

  case 222: /* toolbutton: TOOLBUTTON attr ETOOLBUTTON  */
#line 1001 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_TOOLBUTTON);
	}
#line 5872 "gtkdialog_parser.c"
    break;

  case 223: /* toolbutton: PART_TOOLBUTTON tagattr '>' attr ETOOLBUTTON  */
#line 1004 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_TOOLBUTTON, (yyvsp[-3].nvval));
	}
#line 5880 "gtkdialog_parser.c"
    break;

  case 224: /* toggletoolbutton: TOGGLETOOLBUTTON attr ETOGGLETOOLBUTTON  */
#line 1010 "gtkdialog_parser.y"
                                            {
		token_store(PUSH | WIDGET_TOGGLETOOLBUTTON);
	}
#line 5888 "gtkdialog_parser.c"
    break;

  case 225: /* toggletoolbutton: PART_TOGGLETOOLBUTTON tagattr '>' attr ETOGGLETOOLBUTTON  */
#line 1013 "gtkdialog_parser.y"
                                                             {
		token_store_attr(PUSH | WIDGET_TOGGLETOOLBUTTON, (yyvsp[-3].nvval));
	}
#line 5896 "gtkdialog_parser.c"
    break;

  case 226: /* radiotoolbutton: RADIOTOOLBUTTON attr ERADIOTOOLBUTTON  */
#line 1019 "gtkdialog_parser.y"
                                          {
		token_store(PUSH | WIDGET_RADIOTOOLBUTTON);
	}
#line 5904 "gtkdialog_parser.c"
    break;

  case 227: /* radiotoolbutton: PART_RADIOTOOLBUTTON tagattr '>' attr ERADIOTOOLBUTTON  */
#line 1022 "gtkdialog_parser.y"
                                                           {
		token_store_attr(PUSH | WIDGET_RADIOTOOLBUTTON, (yyvsp[-3].nvval));
	}
#line 5912 "gtkdialog_parser.c"
    break;

  case 228: /* menutoolbutton: MENUTOOLBUTTON EMENUTOOLBUTTON  */
#line 1028 "gtkdialog_parser.y"
                                   {
		yyerror("The menutoolbutton widget requires at least one menu item.");
	}
#line 5920 "gtkdialog_parser.c"
    break;

  case 229: /* menutoolbutton: MENUTOOLBUTTON radio_group_scope menuwlist attr EMENUTOOLBUTTON  */
#line 1031 "gtkdialog_parser.y"
                                                                    {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_MENUTOOLBUTTON);
	}
#line 5929 "gtkdialog_parser.c"
    break;

  case 230: /* menutoolbutton: PART_MENUTOOLBUTTON tagattr '>' radio_group_scope menuwlist attr EMENUTOOLBUTTON  */
#line 1035 "gtkdialog_parser.y"
                                                                                     {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_MENUTOOLBUTTON, (yyvsp[-5].nvval));
	}
#line 5938 "gtkdialog_parser.c"
    break;

  case 231: /* separatortoolitem: SEPARATORTOOLITEM attr ESEPARATORTOOLITEM  */
#line 1042 "gtkdialog_parser.y"
                                              {
		token_store(PUSH | WIDGET_SEPARATORTOOLITEM);
	}
#line 5946 "gtkdialog_parser.c"
    break;

  case 232: /* separatortoolitem: PART_SEPARATORTOOLITEM tagattr '>' attr ESEPARATORTOOLITEM  */
#line 1045 "gtkdialog_parser.y"
                                                               {
		token_store_attr(PUSH | WIDGET_SEPARATORTOOLITEM, (yyvsp[-3].nvval));
	}
#line 5954 "gtkdialog_parser.c"
    break;

  case 233: /* menubar: MENUBAR EMENUBAR  */
#line 1051 "gtkdialog_parser.y"
                     {
		yyerror("The menubar widget requires at least one menu widget.");
	}
#line 5962 "gtkdialog_parser.c"
    break;

  case 234: /* menubar: MENUBAR menu attr EMENUBAR  */
#line 1054 "gtkdialog_parser.y"
                               {
		token_store(PUSH | WIDGET_MENUBAR);
	}
#line 5970 "gtkdialog_parser.c"
    break;

  case 235: /* menubar: menu MENUBAR menu attr EMENUBAR  */
#line 1057 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_MENUBAR);
		token_store(SUM);
	}
#line 5979 "gtkdialog_parser.c"
    break;

  case 236: /* menubar: PART_MENUBAR tagattr '>' menu attr EMENUBAR  */
#line 1061 "gtkdialog_parser.y"
                                                {
		token_store_attr(PUSH | WIDGET_MENUBAR, (yyvsp[-4].nvval));
	}
#line 5987 "gtkdialog_parser.c"
    break;

  case 237: /* menubar: menu PART_MENUBAR tagattr '>' menu attr EMENUBAR  */
#line 1064 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_MENUBAR, (yyvsp[-4].nvval));
		token_store(SUM);
	}
#line 5996 "gtkdialog_parser.c"
    break;

  case 242: /* menu: MENU EMENU  */
#line 1078 "gtkdialog_parser.y"
               {
		yyerror("The menu widget requires at least one menuitem widget.");
	}
#line 6004 "gtkdialog_parser.c"
    break;

  case 243: /* menu: MENU radio_group_scope menuwlist attr EMENU  */
#line 1081 "gtkdialog_parser.y"
                                                      {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_MENU);
	}
#line 6013 "gtkdialog_parser.c"
    break;

  case 244: /* menu: menuwlist MENU radio_group_scope menuwlist attr EMENU  */
#line 1085 "gtkdialog_parser.y"
                                                                {
		token_store(RGROUP_POP);
		token_store(PUSH | WIDGET_MENU);
		token_store(SUM);
	}
#line 6023 "gtkdialog_parser.c"
    break;

  case 245: /* menu: PART_MENU tagattr '>' radio_group_scope menuwlist attr EMENU  */
#line 1090 "gtkdialog_parser.y"
                                                                       {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_MENU, (yyvsp[-5].nvval));
	}
#line 6032 "gtkdialog_parser.c"
    break;

  case 246: /* menu: menuwlist PART_MENU tagattr '>' radio_group_scope menuwlist attr EMENU  */
#line 1094 "gtkdialog_parser.y"
                                                                                 {
		token_store(RGROUP_POP);
		token_store_attr(PUSH | WIDGET_MENU, (yyvsp[-5].nvval));
		token_store(SUM);
	}
#line 6042 "gtkdialog_parser.c"
    break;

  case 247: /* menuitem: MENUITEM attr EMENUITEM  */
#line 1102 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_MENUITEM);
	}
#line 6050 "gtkdialog_parser.c"
    break;

  case 248: /* menuitem: menuwlist MENUITEM attr EMENUITEM  */
#line 1105 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_MENUITEM);
		token_store(SUM);
	}
#line 6059 "gtkdialog_parser.c"
    break;

  case 249: /* menuitem: PART_MENUITEM tagattr '>' attr EMENUITEM  */
#line 1109 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_MENUITEM, (yyvsp[-3].nvval));
	}
#line 6067 "gtkdialog_parser.c"
    break;

  case 250: /* menuitem: menuwlist PART_MENUITEM tagattr '>' attr EMENUITEM  */
#line 1112 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_MENUITEM, (yyvsp[-3].nvval));
		token_store(SUM);
	}
#line 6076 "gtkdialog_parser.c"
    break;

  case 251: /* menuitemseparator: MENUITEMSEPARATOR EMENUITEMSEPARATOR  */
#line 1119 "gtkdialog_parser.y"
                                         {
		token_store(PUSH | WIDGET_MENUITEMSEPARATOR);
	}
#line 6084 "gtkdialog_parser.c"
    break;

  case 252: /* menuitemseparator: menuwlist MENUITEMSEPARATOR EMENUITEMSEPARATOR  */
#line 1122 "gtkdialog_parser.y"
                                                   {
		token_store(PUSH | WIDGET_MENUITEMSEPARATOR);
		token_store(SUM);
	}
#line 6093 "gtkdialog_parser.c"
    break;

  case 253: /* recentchoosermenu: RECENTCHOOSERMENU attr ERECENTCHOOSERMENU  */
#line 1129 "gtkdialog_parser.y"
                                              {
		token_store(PUSH | WIDGET_RECENTCHOOSERMENU);
	}
#line 6101 "gtkdialog_parser.c"
    break;

  case 254: /* recentchoosermenu: menuwlist RECENTCHOOSERMENU attr ERECENTCHOOSERMENU  */
#line 1132 "gtkdialog_parser.y"
                                                        {
		token_store(PUSH | WIDGET_RECENTCHOOSERMENU);
		token_store(SUM);
	}
#line 6110 "gtkdialog_parser.c"
    break;

  case 255: /* recentchoosermenu: PART_RECENTCHOOSERMENU tagattr '>' attr ERECENTCHOOSERMENU  */
#line 1136 "gtkdialog_parser.y"
                                                               {
		token_store_attr(PUSH | WIDGET_RECENTCHOOSERMENU, (yyvsp[-3].nvval));
	}
#line 6118 "gtkdialog_parser.c"
    break;

  case 256: /* recentchoosermenu: menuwlist PART_RECENTCHOOSERMENU tagattr '>' attr ERECENTCHOOSERMENU  */
#line 1139 "gtkdialog_parser.y"
                                                                         {
		token_store_attr(PUSH | WIDGET_RECENTCHOOSERMENU, (yyvsp[-3].nvval));
		token_store(SUM);
	}
#line 6127 "gtkdialog_parser.c"
    break;

  case 257: /* hseparator: HSEPARATOR EHSEPARATOR  */
#line 1146 "gtkdialog_parser.y"
                           {
		token_store(PUSH | WIDGET_HSEPARATOR);
	}
#line 6135 "gtkdialog_parser.c"
    break;

  case 258: /* hseparator: PART_HSEPARATOR tagattr '>' EHSEPARATOR  */
#line 1149 "gtkdialog_parser.y"
                                            {
		token_store_attr(PUSH | WIDGET_HSEPARATOR, (yyvsp[-2].nvval));
	}
#line 6143 "gtkdialog_parser.c"
    break;

  case 259: /* vseparator: VSEPARATOR EVSEPARATOR  */
#line 1155 "gtkdialog_parser.y"
                           {
		token_store(PUSH | WIDGET_VSEPARATOR);
	}
#line 6151 "gtkdialog_parser.c"
    break;

  case 260: /* vseparator: PART_VSEPARATOR tagattr '>' EVSEPARATOR  */
#line 1158 "gtkdialog_parser.y"
                                            {
		token_store_attr(PUSH | WIDGET_VSEPARATOR, (yyvsp[-2].nvval));
	}
#line 6159 "gtkdialog_parser.c"
    break;

  case 261: /* comboboxtext: COMBOBOXTEXT attr ECOMBOBOXTEXT  */
#line 1164 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_COMBOBOXTEXT);
	}
#line 6167 "gtkdialog_parser.c"
    break;

  case 262: /* comboboxtext: PART_COMBOBOXTEXT tagattr '>' attr ECOMBOBOXTEXT  */
#line 1167 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_COMBOBOXTEXT, (yyvsp[-3].nvval));
	}
#line 6175 "gtkdialog_parser.c"
    break;

  case 263: /* comboboxentry: COMBOBOXENTRY attr ECOMBOBOXENTRY  */
#line 1173 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_COMBOBOXENTRY);
	}
#line 6183 "gtkdialog_parser.c"
    break;

  case 264: /* comboboxentry: PART_COMBOBOXENTRY tagattr '>' attr ECOMBOBOXENTRY  */
#line 1176 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_COMBOBOXENTRY, (yyvsp[-3].nvval));
	}
#line 6191 "gtkdialog_parser.c"
    break;

  case 265: /* hscale: HSCALE attr EHSCALE  */
#line 1182 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_HSCALE);
	}
#line 6199 "gtkdialog_parser.c"
    break;

  case 266: /* hscale: PART_HSCALE tagattr '>' attr EHSCALE  */
#line 1185 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_HSCALE, (yyvsp[-3].nvval));
	}
#line 6207 "gtkdialog_parser.c"
    break;

  case 267: /* vscale: VSCALE attr EVSCALE  */
#line 1191 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_VSCALE);
	}
#line 6215 "gtkdialog_parser.c"
    break;

  case 268: /* vscale: PART_VSCALE tagattr '>' attr EVSCALE  */
#line 1194 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_VSCALE, (yyvsp[-3].nvval));
	}
#line 6223 "gtkdialog_parser.c"
    break;

  case 269: /* spinbutton: SPINBUTTON attr ESPINBUTTON  */
#line 1200 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_SPINBUTTON);
	}
#line 6231 "gtkdialog_parser.c"
    break;

  case 270: /* spinbutton: PART_SPINBUTTON tagattr '>' attr ESPINBUTTON  */
#line 1203 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_SPINBUTTON, (yyvsp[-3].nvval));
	}
#line 6239 "gtkdialog_parser.c"
    break;

  case 271: /* timer: TIMER attr ETIMER  */
#line 1209 "gtkdialog_parser.y"
                      {
		token_store(PUSH | WIDGET_TIMER);
	}
#line 6247 "gtkdialog_parser.c"
    break;

  case 272: /* timer: PART_TIMER tagattr '>' attr ETIMER  */
#line 1212 "gtkdialog_parser.y"
                                       {
		token_store_attr(PUSH | WIDGET_TIMER, (yyvsp[-3].nvval));
	}
#line 6255 "gtkdialog_parser.c"
    break;

  case 273: /* togglebutton: TOGGLEBUTTON attr ETOGGLEBUTTON  */
#line 1218 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_TOGGLEBUTTON);
	}
#line 6263 "gtkdialog_parser.c"
    break;

  case 274: /* togglebutton: PART_TOGGLEBUTTON tagattr '>' attr ETOGGLEBUTTON  */
#line 1221 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_TOGGLEBUTTON, (yyvsp[-3].nvval));
	}
#line 6271 "gtkdialog_parser.c"
    break;

  case 275: /* statusbar: STATUSBAR attr ESTATUSBAR  */
#line 1227 "gtkdialog_parser.y"
                              {
		token_store(PUSH | WIDGET_STATUSBAR);
	}
#line 6279 "gtkdialog_parser.c"
    break;

  case 276: /* statusbar: PART_STATUSBAR tagattr '>' attr ESTATUSBAR  */
#line 1230 "gtkdialog_parser.y"
                                               {
		token_store_attr(PUSH | WIDGET_STATUSBAR, (yyvsp[-3].nvval));
	}
#line 6287 "gtkdialog_parser.c"
    break;

  case 277: /* statusicon: STATUSICON attr ESTATUSICON  */
#line 1236 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_STATUSICON);
	}
#line 6295 "gtkdialog_parser.c"
    break;

  case 278: /* statusicon: PART_STATUSICON tagattr '>' attr ESTATUSICON  */
#line 1239 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_STATUSICON, (yyvsp[-3].nvval));
	}
#line 6303 "gtkdialog_parser.c"
    break;

  case 279: /* accellabel: ACCELLABEL attr EACCELLABEL  */
#line 1245 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_ACCELLABEL);
	}
#line 6311 "gtkdialog_parser.c"
    break;

  case 280: /* accellabel: PART_ACCELLABEL tagattr '>' attr EACCELLABEL  */
#line 1248 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_ACCELLABEL, (yyvsp[-3].nvval));
	}
#line 6319 "gtkdialog_parser.c"
    break;

  case 281: /* arrow: ARROW attr EARROW  */
#line 1254 "gtkdialog_parser.y"
                      {
		token_store(PUSH | WIDGET_ARROW);
	}
#line 6327 "gtkdialog_parser.c"
    break;

  case 282: /* arrow: PART_ARROW tagattr '>' attr EARROW  */
#line 1257 "gtkdialog_parser.y"
                                       {
		token_store_attr(PUSH | WIDGET_ARROW, (yyvsp[-3].nvval));
	}
#line 6335 "gtkdialog_parser.c"
    break;

  case 283: /* hsv: HSV attr EHSV  */
#line 1263 "gtkdialog_parser.y"
                  {
		token_store(PUSH | WIDGET_HSV);
	}
#line 6343 "gtkdialog_parser.c"
    break;

  case 284: /* hsv: PART_HSV tagattr '>' attr EHSV  */
#line 1266 "gtkdialog_parser.y"
                                   {
		token_store_attr(PUSH | WIDGET_HSV, (yyvsp[-3].nvval));
	}
#line 6351 "gtkdialog_parser.c"
    break;

  case 285: /* curve: CURVE attr ECURVE  */
#line 1272 "gtkdialog_parser.y"
                      {
		token_store(PUSH | WIDGET_CURVE);
	}
#line 6359 "gtkdialog_parser.c"
    break;

  case 286: /* curve: PART_CURVE tagattr '>' attr ECURVE  */
#line 1275 "gtkdialog_parser.y"
                                       {
		token_store_attr(PUSH | WIDGET_CURVE, (yyvsp[-3].nvval));
	}
#line 6367 "gtkdialog_parser.c"
    break;

  case 287: /* hruler: HRULER attr EHRULER  */
#line 1281 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_HRULER);
	}
#line 6375 "gtkdialog_parser.c"
    break;

  case 288: /* hruler: PART_HRULER tagattr '>' attr EHRULER  */
#line 1284 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_HRULER, (yyvsp[-3].nvval));
	}
#line 6383 "gtkdialog_parser.c"
    break;

  case 289: /* vruler: VRULER attr EVRULER  */
#line 1290 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_VRULER);
	}
#line 6391 "gtkdialog_parser.c"
    break;

  case 290: /* vruler: PART_VRULER tagattr '>' attr EVRULER  */
#line 1293 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_VRULER, (yyvsp[-3].nvval));
	}
#line 6399 "gtkdialog_parser.c"
    break;

  case 291: /* colorbutton: COLORBUTTON attr ECOLORBUTTON  */
#line 1299 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_COLORBUTTON);
	}
#line 6407 "gtkdialog_parser.c"
    break;

  case 292: /* colorbutton: PART_COLORBUTTON tagattr '>' attr ECOLORBUTTON  */
#line 1302 "gtkdialog_parser.y"
                                                   {
		token_store_attr(PUSH | WIDGET_COLORBUTTON, (yyvsp[-3].nvval));
	}
#line 6415 "gtkdialog_parser.c"
    break;

  case 293: /* fontbutton: FONTBUTTON attr EFONTBUTTON  */
#line 1308 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_FONTBUTTON);
	}
#line 6423 "gtkdialog_parser.c"
    break;

  case 294: /* fontbutton: PART_FONTBUTTON tagattr '>' attr EFONTBUTTON  */
#line 1311 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_FONTBUTTON, (yyvsp[-3].nvval));
	}
#line 6431 "gtkdialog_parser.c"
    break;

  case 295: /* terminal: TERMINAL attr ETERMINAL  */
#line 1317 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_TERMINAL);
	}
#line 6439 "gtkdialog_parser.c"
    break;

  case 296: /* terminal: PART_TERMINAL tagattr '>' attr ETERMINAL  */
#line 1320 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_TERMINAL, (yyvsp[-3].nvval));
	}
#line 6447 "gtkdialog_parser.c"
    break;

  case 308: /* label: LABEL STRING ELABEL  */
#line 1340 "gtkdialog_parser.y"
                                    {
		token_store_owned_argument(SET | ATTR_LABEL, (yyvsp[-1].cval));     }
#line 6454 "gtkdialog_parser.c"
    break;

  case 309: /* sensitive: SENSITIVE STRING ESENSITIVE  */
#line 1345 "gtkdialog_parser.y"
                                      {
     token_store_owned_argument(SET | ATTR_SENSITIVE, (yyvsp[-1].cval));  }
#line 6461 "gtkdialog_parser.c"
    break;

  case 310: /* defaultvalue: DEFAULT STRING EDEFAULT  */
#line 1350 "gtkdialog_parser.y"
                             {
     token_store_owned_argument(SET | ATTR_DEFAULT, (yyvsp[-1].cval));   }
#line 6468 "gtkdialog_parser.c"
    break;

  case 311: /* width: WIDTH STRING EWIDTH  */
#line 1355 "gtkdialog_parser.y"
                                    {
     token_store_owned_argument(SET | ATTR_WIDTH, (yyvsp[-1].cval));    }
#line 6475 "gtkdialog_parser.c"
    break;

  case 312: /* height: HEIGHT STRING EHEIGHT  */
#line 1360 "gtkdialog_parser.y"
                                    {
     token_store_owned_argument(SET | ATTR_HEIGHT, (yyvsp[-1].cval));   }
#line 6482 "gtkdialog_parser.c"
    break;

  case 313: /* input: INPUT STRING EINPUT  */
#line 1365 "gtkdialog_parser.y"
                           {
		token_store_owned_argument(SET|ATTR_INPUT|SUB_ATTR_SHELL, (yyvsp[-1].cval));
	}
#line 6490 "gtkdialog_parser.c"
    break;

  case 314: /* input: PART_INPUT tagattr '>' STRING EINPUT  */
#line 1368 "gtkdialog_parser.y"
                                         {
		token_store_owned_argument_attr(SET|ATTR_INPUT, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6498 "gtkdialog_parser.c"
    break;

  case 315: /* input: INPUTFILE STRING EINPUT  */
#line 1371 "gtkdialog_parser.y"
                             {
		token_store_owned_argument(SET|ATTR_INPUT|SUB_ATTR_FILE, (yyvsp[-1].cval));
	}
#line 6506 "gtkdialog_parser.c"
    break;

  case 316: /* input: PART_INPUTFILE tagattr '>' STRING EINPUT  */
#line 1374 "gtkdialog_parser.y"
                                             {
		token_store_owned_argument_attr(SET|ATTR_INPUT|SUB_ATTR_FILE, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6514 "gtkdialog_parser.c"
    break;

  case 317: /* input: PART_INPUTFILE tagattr '>' EINPUT  */
#line 1377 "gtkdialog_parser.y"
                                      {
		token_store_with_argument_attr(SET|ATTR_INPUT|SUB_ATTR_FILE, "", (yyvsp[-2].nvval));
	}
#line 6522 "gtkdialog_parser.c"
    break;

  case 318: /* output: OUTPUT STRING EOUTPUT  */
#line 1383 "gtkdialog_parser.y"
                          {
		fprintf(stderr, "<output>: Not implemented.\n");
		g_free((yyvsp[-1].cval));
	}
#line 6531 "gtkdialog_parser.c"
    break;

  case 319: /* output: OUTPUTFILE STRING EOUTPUT  */
#line 1387 "gtkdialog_parser.y"
                              {
		token_store_owned_argument(SET|ATTR_OUTPUT|SUB_ATTR_FILE, (yyvsp[-1].cval));
	}
#line 6539 "gtkdialog_parser.c"
    break;

  case 320: /* output: PART_OUTPUTFILE tagattr '>' STRING EOUTPUT  */
#line 1390 "gtkdialog_parser.y"
                                               {
		token_store_owned_argument_attr(SET|ATTR_OUTPUT|SUB_ATTR_FILE, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6547 "gtkdialog_parser.c"
    break;

  case 321: /* variable: VARIABLE STRING EVARIABLE  */
#line 1396 "gtkdialog_parser.y"
                              {
		token_store_owned_argument(SET | ATTR_VARIABLE, (yyvsp[-1].cval));
	}
#line 6555 "gtkdialog_parser.c"
    break;

  case 322: /* variable: PART_VARIABLE tagattr '>' STRING EVARIABLE  */
#line 1399 "gtkdialog_parser.y"
                                               {
		token_store_owned_argument_attr(SET | ATTR_VARIABLE, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6563 "gtkdialog_parser.c"
    break;

  case 323: /* action: ACTION STRING EACTION  */
#line 1405 "gtkdialog_parser.y"
                           {
		token_store_owned_argument(SET|ATTR_ACTION, (yyvsp[-1].cval));
	}
#line 6571 "gtkdialog_parser.c"
    break;

  case 324: /* action: PART_ACTION tagattr '>' STRING EACTION  */
#line 1408 "gtkdialog_parser.y"
                                           {
		token_store_owned_argument_attr(SET | ATTR_ACTION, (yyvsp[-1].cval), (yyvsp[-3].nvval));
	}
#line 6579 "gtkdialog_parser.c"
    break;

  case 325: /* item: ITEM STRING EITEM  */
#line 1414 "gtkdialog_parser.y"
                      {
		token_store_owned_argument(SET | ATTR_ITEM, (yyvsp[-1].cval));
	}
#line 6587 "gtkdialog_parser.c"
    break;

  case 326: /* item: ITEM EITEM  */
#line 1417 "gtkdialog_parser.y"
               {
		token_store_with_argument( SET | ATTR_ITEM, "");
	}
#line 6595 "gtkdialog_parser.c"
    break;

  case 327: /* item: PART_ITEM tagattr '>' STRING EITEM  */
#line 1420 "gtkdialog_parser.y"
                                       {
		token_store_owned_argument_attr(SET | ATTR_ITEM, (yyvsp[-1].cval), (yyvsp[-3].nvval));
                    }
#line 6603 "gtkdialog_parser.c"
    break;

  case 328: /* tagattr: TAG_ATTR_NAME '=' STRING  */
#line 1426 "gtkdialog_parser.y"
                             {
		(yyval.nvval) = new_tag_attributeset_owned((yyvsp[-2].cval), (yyvsp[0].cval));
	}
#line 6611 "gtkdialog_parser.c"
    break;

  case 329: /* tagattr: tagattr TAG_ATTR_NAME '=' STRING  */
#line 1429 "gtkdialog_parser.y"
                                     {
		(yyval.nvval) = add_tag_attribute_owned((yyvsp[-3].nvval), (yyvsp[-2].cval), (yyvsp[0].cval));
	}
#line 6619 "gtkdialog_parser.c"
    break;

  case 331: /* imperative: SHOW_WIDGETS  */
#line 1436 "gtkdialog_parser.y"
                 {
		token_store(SHOW);
	}
#line 6627 "gtkdialog_parser.c"
    break;

  case 332: /* imperative: if expression '>' then wlist endif  */
#line 1439 "gtkdialog_parser.y"
                                       {
		instruction_set_jump((yyvsp[-2].ival), (yyvsp[0].ival) + 1);
	}
#line 6635 "gtkdialog_parser.c"
    break;

  case 333: /* imperative: while expression '>' do wlist ewhile  */
#line 1442 "gtkdialog_parser.y"
                                         {
		instruction_set_jump((yyvsp[-2].ival), (yyvsp[0].ival) + 1);
		instruction_set_jump((yyvsp[0].ival), (yyvsp[-5].ival) + 1);
	}
#line 6644 "gtkdialog_parser.c"
    break;

  case 334: /* assignment: EMB_VARIABLE ':' '=' expression  */
#line 1449 "gtkdialog_parser.y"
                                    {
		token_store_owned_argument(IMASSG | VARIABLE_NAME, (yyvsp[-3].cval));
	}
#line 6652 "gtkdialog_parser.c"
    break;

  case 335: /* expression: EMB_VARIABLE  */
#line 1455 "gtkdialog_parser.y"
                 {
		token_store_owned_argument(IMPUSH | VARIABLE_NAME, (yyvsp[0].cval));
	}
#line 6660 "gtkdialog_parser.c"
    break;

  case 336: /* expression: EMB_NUMBER  */
#line 1458 "gtkdialog_parser.y"
               {
		token_store_owned_argument(IMPUSH | CONST_NUMBER, (yyvsp[0].cval));
	}
#line 6668 "gtkdialog_parser.c"
    break;

  case 337: /* expression: expression '+' expression  */
#line 1461 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_ADD);
	}
#line 6676 "gtkdialog_parser.c"
    break;

  case 338: /* expression: expression '-' expression  */
#line 1464 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_SUBST);
	}
#line 6684 "gtkdialog_parser.c"
    break;

  case 339: /* expression: expression '*' expression  */
#line 1467 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_MULT);
	}
#line 6692 "gtkdialog_parser.c"
    break;

  case 340: /* expression: expression '/' expression  */
#line 1470 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | OP_DIV);
	}
#line 6700 "gtkdialog_parser.c"
    break;

  case 341: /* expression: expression '=' expression  */
#line 1473 "gtkdialog_parser.y"
                              {
		token_store(IMPUSH | REL_EQ);
	}
#line 6708 "gtkdialog_parser.c"
    break;

  case 342: /* expression: expression '!' '=' expression  */
#line 1476 "gtkdialog_parser.y"
                                  {
		token_store(IMPUSH | REL_NE);
	}
#line 6716 "gtkdialog_parser.c"
    break;

  case 344: /* then: %empty  */
#line 1486 "gtkdialog_parser.y"
        {
		token_store(IFNGOTO);
		(yyval.ival) = instruction_get_pc();
	}
#line 6725 "gtkdialog_parser.c"
    break;

  case 345: /* endif: ENDIF  */
#line 1493 "gtkdialog_parser.y"
           {
		token_store(IMMERGE);
		(yyval.ival) = instruction_get_pc();
	}
#line 6734 "gtkdialog_parser.c"
    break;

  case 346: /* while: WHILE  */
#line 1500 "gtkdialog_parser.y"
          { (yyval.ival) = instruction_get_pc(); }
#line 6740 "gtkdialog_parser.c"
    break;

  case 347: /* ewhile: EWHILE  */
#line 1504 "gtkdialog_parser.y"
           {
		token_store(IMMERGE);
		token_store(GOTO);
		(yyval.ival) = instruction_get_pc();
	}
#line 6750 "gtkdialog_parser.c"
    break;

  case 348: /* do: %empty  */
#line 1512 "gtkdialog_parser.y"
    {
		token_store(IFNGOTO);
		(yyval.ival) = instruction_get_pc();
    }
#line 6759 "gtkdialog_parser.c"
    break;

  case 349: /* linkbutton: LINKBUTTON attr ELINKBUTTON  */
#line 1519 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_LINKBUTTON);
	}
#line 6767 "gtkdialog_parser.c"
    break;

  case 350: /* linkbutton: PART_LINKBUTTON tagattr '>' attr ELINKBUTTON  */
#line 1522 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_LINKBUTTON, (yyvsp[-3].nvval));
	}
#line 6775 "gtkdialog_parser.c"
    break;

  case 351: /* spinner: SPINNER attr ESPINNER  */
#line 1528 "gtkdialog_parser.y"
                          {
		token_store(PUSH | WIDGET_SPINNER);
	}
#line 6783 "gtkdialog_parser.c"
    break;

  case 352: /* spinner: PART_SPINNER tagattr '>' attr ESPINNER  */
#line 1531 "gtkdialog_parser.y"
                                           {
		token_store_attr(PUSH | WIDGET_SPINNER, (yyvsp[-3].nvval));
	}
#line 6791 "gtkdialog_parser.c"
    break;

  case 353: /* filechooserbutton: FILECHOOSERBUTTON attr EFILECHOOSERBUTTON  */
#line 1537 "gtkdialog_parser.y"
                                              {
		token_store(PUSH | WIDGET_FILECHOOSERBUTTON);
	}
#line 6799 "gtkdialog_parser.c"
    break;

  case 354: /* filechooserbutton: PART_FILECHOOSERBUTTON tagattr '>' attr EFILECHOOSERBUTTON  */
#line 1540 "gtkdialog_parser.y"
                                                               {
		token_store_attr(PUSH | WIDGET_FILECHOOSERBUTTON, (yyvsp[-3].nvval));
	}
#line 6807 "gtkdialog_parser.c"
    break;

  case 355: /* iconview: ICONVIEW attr EICONVIEW  */
#line 1546 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_ICONVIEW);
	}
#line 6815 "gtkdialog_parser.c"
    break;

  case 356: /* iconview: PART_ICONVIEW tagattr '>' attr EICONVIEW  */
#line 1549 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_ICONVIEW, (yyvsp[-3].nvval));
	}
#line 6823 "gtkdialog_parser.c"
    break;

  case 357: /* cellview: CELLVIEW attr ECELLVIEW  */
#line 1555 "gtkdialog_parser.y"
                            {
		token_store(PUSH | WIDGET_CELLVIEW);
	}
#line 6831 "gtkdialog_parser.c"
    break;

  case 358: /* cellview: PART_CELLVIEW tagattr '>' attr ECELLVIEW  */
#line 1558 "gtkdialog_parser.y"
                                             {
		token_store_attr(PUSH | WIDGET_CELLVIEW, (yyvsp[-3].nvval));
	}
#line 6839 "gtkdialog_parser.c"
    break;

  case 359: /* drawingarea: DRAWINGAREA attr EDRAWINGAREA  */
#line 1564 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_DRAWINGAREA);
	}
#line 6847 "gtkdialog_parser.c"
    break;

  case 360: /* drawingarea: PART_DRAWINGAREA tagattr '>' attr EDRAWINGAREA  */
#line 1567 "gtkdialog_parser.y"
                                                   {
		token_store_attr(PUSH | WIDGET_DRAWINGAREA, (yyvsp[-3].nvval));
	}
#line 6855 "gtkdialog_parser.c"
    break;

  case 361: /* imageview: IMAGEVIEW attr EIMAGEVIEW  */
#line 1573 "gtkdialog_parser.y"
                              {
		token_store(PUSH | WIDGET_IMAGEVIEW);
	}
#line 6863 "gtkdialog_parser.c"
    break;

  case 362: /* imageview: PART_IMAGEVIEW tagattr '>' attr EIMAGEVIEW  */
#line 1576 "gtkdialog_parser.y"
                                               {
		token_store_attr(PUSH | WIDGET_IMAGEVIEW, (yyvsp[-3].nvval));
	}
#line 6871 "gtkdialog_parser.c"
    break;

  case 363: /* scalebutton: SCALEBUTTON attr ESCALEBUTTON  */
#line 1582 "gtkdialog_parser.y"
                                  {
		token_store(PUSH | WIDGET_SCALEBUTTON);
	}
#line 6879 "gtkdialog_parser.c"
    break;

  case 364: /* scalebutton: PART_SCALEBUTTON tagattr '>' attr ESCALEBUTTON  */
#line 1585 "gtkdialog_parser.y"
                                                   {
		token_store_attr(PUSH | WIDGET_SCALEBUTTON, (yyvsp[-3].nvval));
	}
#line 6887 "gtkdialog_parser.c"
    break;

  case 365: /* volumebutton: VOLUMEBUTTON attr EVOLUMEBUTTON  */
#line 1591 "gtkdialog_parser.y"
                                    {
		token_store(PUSH | WIDGET_VOLUMEBUTTON);
	}
#line 6895 "gtkdialog_parser.c"
    break;

  case 366: /* volumebutton: PART_VOLUMEBUTTON tagattr '>' attr EVOLUMEBUTTON  */
#line 1594 "gtkdialog_parser.y"
                                                     {
		token_store_attr(PUSH | WIDGET_VOLUMEBUTTON, (yyvsp[-3].nvval));
	}
#line 6903 "gtkdialog_parser.c"
    break;

  case 367: /* recentchooser: RECENTCHOOSER attr ERECENTCHOOSER  */
#line 1600 "gtkdialog_parser.y"
                                      {
		token_store(PUSH | WIDGET_RECENTCHOOSER);
	}
#line 6911 "gtkdialog_parser.c"
    break;

  case 368: /* recentchooser: PART_RECENTCHOOSER tagattr '>' attr ERECENTCHOOSER  */
#line 1603 "gtkdialog_parser.y"
                                                       {
		token_store_attr(PUSH | WIDGET_RECENTCHOOSER, (yyvsp[-3].nvval));
	}
#line 6919 "gtkdialog_parser.c"
    break;

  case 369: /* socket: SOCKET attr ESOCKET  */
#line 1609 "gtkdialog_parser.y"
                        {
		token_store(PUSH | WIDGET_SOCKET);
	}
#line 6927 "gtkdialog_parser.c"
    break;

  case 370: /* socket: PART_SOCKET tagattr '>' attr ESOCKET  */
#line 1612 "gtkdialog_parser.y"
                                         {
		token_store_attr(PUSH | WIDGET_SOCKET, (yyvsp[-3].nvval));
	}
#line 6935 "gtkdialog_parser.c"
    break;

  case 371: /* hscrollbar: HSCROLLBAR attr EHSCROLLBAR  */
#line 1618 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_HSCROLLBAR);
	}
#line 6943 "gtkdialog_parser.c"
    break;

  case 372: /* hscrollbar: PART_HSCROLLBAR tagattr '>' attr EHSCROLLBAR  */
#line 1621 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_HSCROLLBAR, (yyvsp[-3].nvval));
	}
#line 6951 "gtkdialog_parser.c"
    break;

  case 373: /* vscrollbar: VSCROLLBAR attr EVSCROLLBAR  */
#line 1627 "gtkdialog_parser.y"
                                {
		token_store(PUSH | WIDGET_VSCROLLBAR);
	}
#line 6959 "gtkdialog_parser.c"
    break;

  case 374: /* vscrollbar: PART_VSCROLLBAR tagattr '>' attr EVSCROLLBAR  */
#line 1630 "gtkdialog_parser.y"
                                                 {
		token_store_attr(PUSH | WIDGET_VSCROLLBAR, (yyvsp[-3].nvval));
	}
#line 6967 "gtkdialog_parser.c"
    break;


#line 6971 "gtkdialog_parser.c"

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

#line 1635 "gtkdialog_parser.y"


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
