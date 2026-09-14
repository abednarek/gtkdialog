/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_GTKDIALOG_GTKDIALOG_PARSER_H_INCLUDED
# define YY_GTKDIALOG_GTKDIALOG_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int gtkdialog_debug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    WINDOW = 258,                  /* WINDOW  */
    PART_WINDOW = 259,             /* PART_WINDOW  */
    EWINDOW = 260,                 /* EWINDOW  */
    VBOX = 261,                    /* VBOX  */
    PART_VBOX = 262,               /* PART_VBOX  */
    EVBOX = 263,                   /* EVBOX  */
    HBOX = 264,                    /* HBOX  */
    PART_HBOX = 265,               /* PART_HBOX  */
    EHBOX = 266,                   /* EHBOX  */
    NOTEBOOK = 267,                /* NOTEBOOK  */
    ENOTEBOOK = 268,               /* ENOTEBOOK  */
    PART_NOTEBOOK = 269,           /* PART_NOTEBOOK  */
    FRAME = 270,                   /* FRAME  */
    TAG_ATTR_NAME = 271,           /* TAG_ATTR_NAME  */
    EFRAME = 272,                  /* EFRAME  */
    ENTRY = 273,                   /* ENTRY  */
    EENTRY = 274,                  /* EENTRY  */
    PART_ENTRY = 275,              /* PART_ENTRY  */
    MENUBAR = 276,                 /* MENUBAR  */
    PART_MENUBAR = 277,            /* PART_MENUBAR  */
    EMENUBAR = 278,                /* EMENUBAR  */
    MENU = 279,                    /* MENU  */
    PART_MENU = 280,               /* PART_MENU  */
    EMENU = 281,                   /* EMENU  */
    POPUPMENU = 282,               /* POPUPMENU  */
    PART_POPUPMENU = 283,          /* PART_POPUPMENU  */
    EPOPUPMENU = 284,              /* EPOPUPMENU  */
    MENUITEM = 285,                /* MENUITEM  */
    PART_MENUITEM = 286,           /* PART_MENUITEM  */
    EMENUITEM = 287,               /* EMENUITEM  */
    MENUITEMSEPARATOR = 288,       /* MENUITEMSEPARATOR  */
    EMENUITEMSEPARATOR = 289,      /* EMENUITEMSEPARATOR  */
    EDIT = 290,                    /* EDIT  */
    PART_EDIT = 291,               /* PART_EDIT  */
    EEDIT = 292,                   /* EEDIT  */
    TREE = 293,                    /* TREE  */
    PART_TREE = 294,               /* PART_TREE  */
    ETREE = 295,                   /* ETREE  */
    CHOOSER = 296,                 /* CHOOSER  */
    PART_CHOOSER = 297,            /* PART_CHOOSER  */
    ECHOOSER = 298,                /* ECHOOSER  */
    LABEL = 299,                   /* LABEL  */
    ELABEL = 300,                  /* ELABEL  */
    ITEM = 301,                    /* ITEM  */
    EITEM = 302,                   /* EITEM  */
    PART_ITEM = 303,               /* PART_ITEM  */
    BUTTON = 304,                  /* BUTTON  */
    PART_BUTTON = 305,             /* PART_BUTTON  */
    EBUTTON = 306,                 /* EBUTTON  */
    BUTTONOK = 307,                /* BUTTONOK  */
    BUTTONCANCEL = 308,            /* BUTTONCANCEL  */
    BUTTONHELP = 309,              /* BUTTONHELP  */
    BUTTONYES = 310,               /* BUTTONYES  */
    BUTTONNO = 311,                /* BUTTONNO  */
    CHECKBOX = 312,                /* CHECKBOX  */
    ECHECKBOX = 313,               /* ECHECKBOX  */
    PART_CHECKBOX = 314,           /* PART_CHECKBOX  */
    RADIO = 315,                   /* RADIO  */
    ERADIO = 316,                  /* ERADIO  */
    PART_RADIO = 317,              /* PART_RADIO  */
    PROGRESSBAR = 318,             /* PROGRESSBAR  */
    EPROGRESSBAR = 319,            /* EPROGRESSBAR  */
    PART_PROGRESSBAR = 320,        /* PART_PROGRESSBAR  */
    LIST = 321,                    /* LIST  */
    PART_LIST = 322,               /* PART_LIST  */
    ELIST = 323,                   /* ELIST  */
    TABLE = 324,                   /* TABLE  */
    ETABLE = 325,                  /* ETABLE  */
    PART_TABLE = 326,              /* PART_TABLE  */
    COMBOBOX = 327,                /* COMBOBOX  */
    PART_COMBOBOX = 328,           /* PART_COMBOBOX  */
    ECOMBOBOX = 329,               /* ECOMBOBOX  */
    GVIM = 330,                    /* GVIM  */
    EGVIM = 331,                   /* EGVIM  */
    TEXT = 332,                    /* TEXT  */
    PART_TEXT = 333,               /* PART_TEXT  */
    ETEXT = 334,                   /* ETEXT  */
    PIXMAP = 335,                  /* PIXMAP  */
    PART_PIXMAP = 336,             /* PART_PIXMAP  */
    EPIXMAP = 337,                 /* EPIXMAP  */
    DEFAULT = 338,                 /* DEFAULT  */
    EDEFAULT = 339,                /* EDEFAULT  */
    SENSITIVE = 340,               /* SENSITIVE  */
    ESENSITIVE = 341,              /* ESENSITIVE  */
    VARIABLE = 342,                /* VARIABLE  */
    PART_VARIABLE = 343,           /* PART_VARIABLE  */
    EVARIABLE = 344,               /* EVARIABLE  */
    WIDTH = 345,                   /* WIDTH  */
    EWIDTH = 346,                  /* EWIDTH  */
    HEIGHT = 347,                  /* HEIGHT  */
    EHEIGHT = 348,                 /* EHEIGHT  */
    INPUT = 349,                   /* INPUT  */
    INPUTFILE = 350,               /* INPUTFILE  */
    EINPUT = 351,                  /* EINPUT  */
    PART_INPUT = 352,              /* PART_INPUT  */
    PART_INPUTFILE = 353,          /* PART_INPUTFILE  */
    OUTPUT = 354,                  /* OUTPUT  */
    OUTPUTFILE = 355,              /* OUTPUTFILE  */
    EOUTPUT = 356,                 /* EOUTPUT  */
    ACTION = 357,                  /* ACTION  */
    EACTION = 358,                 /* EACTION  */
    PART_ACTION = 359,             /* PART_ACTION  */
    COMM = 360,                    /* COMM  */
    ENDCOMM = 361,                 /* ENDCOMM  */
    IF = 362,                      /* IF  */
    ENDIF = 363,                   /* ENDIF  */
    WHILE = 364,                   /* WHILE  */
    EWHILE = 365,                  /* EWHILE  */
    SHOW_WIDGETS = 366,            /* SHOW_WIDGETS  */
    EMB_VARIABLE = 367,            /* EMB_VARIABLE  */
    EMB_NUMBER = 368,              /* EMB_NUMBER  */
    END_OF_FILE = 369,             /* END_OF_FILE  */
    NUMBER = 370,                  /* NUMBER  */
    STRING = 371,                  /* STRING  */
    UMINUS = 372,                  /* UMINUS  */
    HSEPARATOR = 373,              /* HSEPARATOR  */
    PART_HSEPARATOR = 374,         /* PART_HSEPARATOR  */
    EHSEPARATOR = 375,             /* EHSEPARATOR  */
    VSEPARATOR = 376,              /* VSEPARATOR  */
    PART_VSEPARATOR = 377,         /* PART_VSEPARATOR  */
    EVSEPARATOR = 378,             /* EVSEPARATOR  */
    COMBOBOXTEXT = 379,            /* COMBOBOXTEXT  */
    PART_COMBOBOXTEXT = 380,       /* PART_COMBOBOXTEXT  */
    ECOMBOBOXTEXT = 381,           /* ECOMBOBOXTEXT  */
    COMBOBOXENTRY = 382,           /* COMBOBOXENTRY  */
    PART_COMBOBOXENTRY = 383,      /* PART_COMBOBOXENTRY  */
    ECOMBOBOXENTRY = 384,          /* ECOMBOBOXENTRY  */
    HSCALE = 385,                  /* HSCALE  */
    PART_HSCALE = 386,             /* PART_HSCALE  */
    EHSCALE = 387,                 /* EHSCALE  */
    VSCALE = 388,                  /* VSCALE  */
    PART_VSCALE = 389,             /* PART_VSCALE  */
    EVSCALE = 390,                 /* EVSCALE  */
    SPINBUTTON = 391,              /* SPINBUTTON  */
    PART_SPINBUTTON = 392,         /* PART_SPINBUTTON  */
    ESPINBUTTON = 393,             /* ESPINBUTTON  */
    TIMER = 394,                   /* TIMER  */
    PART_TIMER = 395,              /* PART_TIMER  */
    ETIMER = 396,                  /* ETIMER  */
    TOGGLEBUTTON = 397,            /* TOGGLEBUTTON  */
    PART_TOGGLEBUTTON = 398,       /* PART_TOGGLEBUTTON  */
    ETOGGLEBUTTON = 399,           /* ETOGGLEBUTTON  */
    STATUSBAR = 400,               /* STATUSBAR  */
    PART_STATUSBAR = 401,          /* PART_STATUSBAR  */
    ESTATUSBAR = 402,              /* ESTATUSBAR  */
    COLORBUTTON = 403,             /* COLORBUTTON  */
    PART_COLORBUTTON = 404,        /* PART_COLORBUTTON  */
    ECOLORBUTTON = 405,            /* ECOLORBUTTON  */
    FONTBUTTON = 406,              /* FONTBUTTON  */
    PART_FONTBUTTON = 407,         /* PART_FONTBUTTON  */
    EFONTBUTTON = 408,             /* EFONTBUTTON  */
    TERMINAL = 409,                /* TERMINAL  */
    PART_TERMINAL = 410,           /* PART_TERMINAL  */
    ETERMINAL = 411,               /* ETERMINAL  */
    EVENTBOX = 412,                /* EVENTBOX  */
    PART_EVENTBOX = 413,           /* PART_EVENTBOX  */
    EEVENTBOX = 414,               /* EEVENTBOX  */
    EXPANDER = 415,                /* EXPANDER  */
    PART_EXPANDER = 416,           /* PART_EXPANDER  */
    EEXPANDER = 417,               /* EEXPANDER  */
    HPANED = 418,                  /* HPANED  */
    PART_HPANED = 419,             /* PART_HPANED  */
    EHPANED = 420,                 /* EHPANED  */
    VPANED = 421,                  /* VPANED  */
    PART_VPANED = 422,             /* PART_VPANED  */
    EVPANED = 423,                 /* EVPANED  */
    ALIGNMENT = 424,               /* ALIGNMENT  */
    PART_ALIGNMENT = 425,          /* PART_ALIGNMENT  */
    EALIGNMENT = 426,              /* EALIGNMENT  */
    HBUTTONBOX = 427,              /* HBUTTONBOX  */
    PART_HBUTTONBOX = 428,         /* PART_HBUTTONBOX  */
    EHBUTTONBOX = 429,             /* EHBUTTONBOX  */
    VBUTTONBOX = 430,              /* VBUTTONBOX  */
    PART_VBUTTONBOX = 431,         /* PART_VBUTTONBOX  */
    EVBUTTONBOX = 432,             /* EVBUTTONBOX  */
    TOOLBAR = 433,                 /* TOOLBAR  */
    PART_TOOLBAR = 434,            /* PART_TOOLBAR  */
    ETOOLBAR = 435,                /* ETOOLBAR  */
    TOOLBUTTON = 436,              /* TOOLBUTTON  */
    PART_TOOLBUTTON = 437,         /* PART_TOOLBUTTON  */
    ETOOLBUTTON = 438,             /* ETOOLBUTTON  */
    TOGGLETOOLBUTTON = 439,        /* TOGGLETOOLBUTTON  */
    PART_TOGGLETOOLBUTTON = 440,   /* PART_TOGGLETOOLBUTTON  */
    ETOGGLETOOLBUTTON = 441,       /* ETOGGLETOOLBUTTON  */
    RADIOTOOLBUTTON = 442,         /* RADIOTOOLBUTTON  */
    PART_RADIOTOOLBUTTON = 443,    /* PART_RADIOTOOLBUTTON  */
    ERADIOTOOLBUTTON = 444,        /* ERADIOTOOLBUTTON  */
    MENUTOOLBUTTON = 445,          /* MENUTOOLBUTTON  */
    PART_MENUTOOLBUTTON = 446,     /* PART_MENUTOOLBUTTON  */
    EMENUTOOLBUTTON = 447,         /* EMENUTOOLBUTTON  */
    SEPARATORTOOLITEM = 448,       /* SEPARATORTOOLITEM  */
    PART_SEPARATORTOOLITEM = 449,  /* PART_SEPARATORTOOLITEM  */
    ESEPARATORTOOLITEM = 450,      /* ESEPARATORTOOLITEM  */
    CALENDAR = 451,                /* CALENDAR  */
    PART_CALENDAR = 452,           /* PART_CALENDAR  */
    ECALENDAR = 453,               /* ECALENDAR  */
    LINKBUTTON = 454,              /* LINKBUTTON  */
    PART_LINKBUTTON = 455,         /* PART_LINKBUTTON  */
    ELINKBUTTON = 456,             /* ELINKBUTTON  */
    SPINNER = 457,                 /* SPINNER  */
    PART_SPINNER = 458,            /* PART_SPINNER  */
    ESPINNER = 459,                /* ESPINNER  */
    GRID = 460,                    /* GRID  */
    PART_GRID = 461,               /* PART_GRID  */
    EGRID = 462,                   /* EGRID  */
    FILECHOOSERBUTTON = 463,       /* FILECHOOSERBUTTON  */
    PART_FILECHOOSERBUTTON = 464,  /* PART_FILECHOOSERBUTTON  */
    EFILECHOOSERBUTTON = 465,      /* EFILECHOOSERBUTTON  */
    INFOBAR = 466,                 /* INFOBAR  */
    PART_INFOBAR = 467,            /* PART_INFOBAR  */
    EINFOBAR = 468,                /* EINFOBAR  */
    ASSISTANT = 469,               /* ASSISTANT  */
    PART_ASSISTANT = 470,          /* PART_ASSISTANT  */
    EASSISTANT = 471,              /* EASSISTANT  */
    ICONVIEW = 472,                /* ICONVIEW  */
    PART_ICONVIEW = 473,           /* PART_ICONVIEW  */
    EICONVIEW = 474,               /* EICONVIEW  */
    SCALEBUTTON = 475,             /* SCALEBUTTON  */
    PART_SCALEBUTTON = 476,        /* PART_SCALEBUTTON  */
    ESCALEBUTTON = 477,            /* ESCALEBUTTON  */
    VOLUMEBUTTON = 478,            /* VOLUMEBUTTON  */
    PART_VOLUMEBUTTON = 479,       /* PART_VOLUMEBUTTON  */
    EVOLUMEBUTTON = 480,           /* EVOLUMEBUTTON  */
    ASPECTFRAME = 481,             /* ASPECTFRAME  */
    PART_ASPECTFRAME = 482,        /* PART_ASPECTFRAME  */
    EASPECTFRAME = 483,            /* EASPECTFRAME  */
    MESSAGEDIALOG = 484,           /* MESSAGEDIALOG  */
    PART_MESSAGEDIALOG = 485,      /* PART_MESSAGEDIALOG  */
    EMESSAGEDIALOG = 486,          /* EMESSAGEDIALOG  */
    DIALOG = 487,                  /* DIALOG  */
    PART_DIALOG = 488,             /* PART_DIALOG  */
    EDIALOG = 489,                 /* EDIALOG  */
    ABOUTDIALOG = 490,             /* ABOUTDIALOG  */
    PART_ABOUTDIALOG = 491,        /* PART_ABOUTDIALOG  */
    EABOUTDIALOG = 492,            /* EABOUTDIALOG  */
    RECENTCHOOSER = 493,           /* RECENTCHOOSER  */
    PART_RECENTCHOOSER = 494,      /* PART_RECENTCHOOSER  */
    ERECENTCHOOSER = 495,          /* ERECENTCHOOSER  */
    RECENTCHOOSERMENU = 496,       /* RECENTCHOOSERMENU  */
    PART_RECENTCHOOSERMENU = 497,  /* PART_RECENTCHOOSERMENU  */
    ERECENTCHOOSERMENU = 498,      /* ERECENTCHOOSERMENU  */
    STATUSICON = 499,              /* STATUSICON  */
    PART_STATUSICON = 500,         /* PART_STATUSICON  */
    ESTATUSICON = 501,             /* ESTATUSICON  */
    ACCELLABEL = 502,              /* ACCELLABEL  */
    PART_ACCELLABEL = 503,         /* PART_ACCELLABEL  */
    EACCELLABEL = 504,             /* EACCELLABEL  */
    ARROW = 505,                   /* ARROW  */
    PART_ARROW = 506,              /* PART_ARROW  */
    EARROW = 507,                  /* EARROW  */
    HSV = 508,                     /* HSV  */
    PART_HSV = 509,                /* PART_HSV  */
    EHSV = 510,                    /* EHSV  */
    TOOLITEM = 511,                /* TOOLITEM  */
    PART_TOOLITEM = 512,           /* PART_TOOLITEM  */
    ETOOLITEM = 513,               /* ETOOLITEM  */
    FIXED = 514,                   /* FIXED  */
    PART_FIXED = 515,              /* PART_FIXED  */
    EFIXED = 516,                  /* EFIXED  */
    LAYOUT = 517,                  /* LAYOUT  */
    PART_LAYOUT = 518,             /* PART_LAYOUT  */
    ELAYOUT = 519,                 /* ELAYOUT  */
    SCROLLEDWINDOW = 520,          /* SCROLLEDWINDOW  */
    PART_SCROLLEDWINDOW = 521,     /* PART_SCROLLEDWINDOW  */
    ESCROLLEDWINDOW = 522,         /* ESCROLLEDWINDOW  */
    HANDLEBOX = 523,               /* HANDLEBOX  */
    PART_HANDLEBOX = 524,          /* PART_HANDLEBOX  */
    EHANDLEBOX = 525,              /* EHANDLEBOX  */
    TOOLPALETTE = 526,             /* TOOLPALETTE  */
    PART_TOOLPALETTE = 527,        /* PART_TOOLPALETTE  */
    ETOOLPALETTE = 528,            /* ETOOLPALETTE  */
    TOOLITEMGROUP = 529,           /* TOOLITEMGROUP  */
    PART_TOOLITEMGROUP = 530,      /* PART_TOOLITEMGROUP  */
    ETOOLITEMGROUP = 531,          /* ETOOLITEMGROUP  */
    CELLVIEW = 532,                /* CELLVIEW  */
    PART_CELLVIEW = 533,           /* PART_CELLVIEW  */
    ECELLVIEW = 534,               /* ECELLVIEW  */
    DRAWINGAREA = 535,             /* DRAWINGAREA  */
    PART_DRAWINGAREA = 536,        /* PART_DRAWINGAREA  */
    EDRAWINGAREA = 537,            /* EDRAWINGAREA  */
    PART_OUTPUTFILE = 538,         /* PART_OUTPUTFILE  */
    SOCKET = 539,                  /* SOCKET  */
    PART_SOCKET = 540,             /* PART_SOCKET  */
    ESOCKET = 541,                 /* ESOCKET  */
    HSCROLLBAR = 542,              /* HSCROLLBAR  */
    PART_HSCROLLBAR = 543,         /* PART_HSCROLLBAR  */
    EHSCROLLBAR = 544,             /* EHSCROLLBAR  */
    VSCROLLBAR = 545,              /* VSCROLLBAR  */
    PART_VSCROLLBAR = 546,         /* PART_VSCROLLBAR  */
    EVSCROLLBAR = 547,             /* EVSCROLLBAR  */
    VIEWPORT = 548,                /* VIEWPORT  */
    PART_VIEWPORT = 549,           /* PART_VIEWPORT  */
    EVIEWPORT = 550,               /* EVIEWPORT  */
    COLORSELECTION = 551,          /* COLORSELECTION  */
    PART_COLORSELECTION = 552,     /* PART_COLORSELECTION  */
    ECOLORSELECTION = 553,         /* ECOLORSELECTION  */
    FONTSELECTION = 554,           /* FONTSELECTION  */
    PART_FONTSELECTION = 555,      /* PART_FONTSELECTION  */
    EFONTSELECTION = 556,          /* EFONTSELECTION  */
    HRULER = 557,                  /* HRULER  */
    PART_HRULER = 558,             /* PART_HRULER  */
    EHRULER = 559,                 /* EHRULER  */
    VRULER = 560,                  /* VRULER  */
    PART_VRULER = 561,             /* PART_VRULER  */
    EVRULER = 562,                 /* EVRULER  */
    PLUG = 563,                    /* PLUG  */
    PART_PLUG = 564,               /* PART_PLUG  */
    EPLUG = 565                    /* EPLUG  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 113 "gtkdialog_parser.y"

  double     dval;
  char      *cval;
  GList     *lval;
  tag_attr *nvval;
  gint       ival;

#line 382 "gtkdialog_parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE gtkdialog_lval;


int gtkdialog_parse (void);


#endif /* !YY_GTKDIALOG_GTKDIALOG_PARSER_H_INCLUDED  */
