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
    EPLUG = 565,                   /* EPLUG  */
    IMAGEVIEW = 566,               /* IMAGEVIEW  */
    PART_IMAGEVIEW = 567,          /* PART_IMAGEVIEW  */
    EIMAGEVIEW = 568,              /* EIMAGEVIEW  */
    CURVE = 569,                   /* CURVE  */
    PART_CURVE = 570,              /* PART_CURVE  */
    ECURVE = 571                   /* ECURVE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define WINDOW 258
#define PART_WINDOW 259
#define EWINDOW 260
#define VBOX 261
#define PART_VBOX 262
#define EVBOX 263
#define HBOX 264
#define PART_HBOX 265
#define EHBOX 266
#define NOTEBOOK 267
#define ENOTEBOOK 268
#define PART_NOTEBOOK 269
#define FRAME 270
#define TAG_ATTR_NAME 271
#define EFRAME 272
#define ENTRY 273
#define EENTRY 274
#define PART_ENTRY 275
#define MENUBAR 276
#define PART_MENUBAR 277
#define EMENUBAR 278
#define MENU 279
#define PART_MENU 280
#define EMENU 281
#define POPUPMENU 282
#define PART_POPUPMENU 283
#define EPOPUPMENU 284
#define MENUITEM 285
#define PART_MENUITEM 286
#define EMENUITEM 287
#define MENUITEMSEPARATOR 288
#define EMENUITEMSEPARATOR 289
#define EDIT 290
#define PART_EDIT 291
#define EEDIT 292
#define TREE 293
#define PART_TREE 294
#define ETREE 295
#define CHOOSER 296
#define PART_CHOOSER 297
#define ECHOOSER 298
#define LABEL 299
#define ELABEL 300
#define ITEM 301
#define EITEM 302
#define PART_ITEM 303
#define BUTTON 304
#define PART_BUTTON 305
#define EBUTTON 306
#define BUTTONOK 307
#define BUTTONCANCEL 308
#define BUTTONHELP 309
#define BUTTONYES 310
#define BUTTONNO 311
#define CHECKBOX 312
#define ECHECKBOX 313
#define PART_CHECKBOX 314
#define RADIO 315
#define ERADIO 316
#define PART_RADIO 317
#define PROGRESSBAR 318
#define EPROGRESSBAR 319
#define PART_PROGRESSBAR 320
#define LIST 321
#define PART_LIST 322
#define ELIST 323
#define TABLE 324
#define ETABLE 325
#define PART_TABLE 326
#define COMBOBOX 327
#define PART_COMBOBOX 328
#define ECOMBOBOX 329
#define GVIM 330
#define EGVIM 331
#define TEXT 332
#define PART_TEXT 333
#define ETEXT 334
#define PIXMAP 335
#define PART_PIXMAP 336
#define EPIXMAP 337
#define DEFAULT 338
#define EDEFAULT 339
#define SENSITIVE 340
#define ESENSITIVE 341
#define VARIABLE 342
#define PART_VARIABLE 343
#define EVARIABLE 344
#define WIDTH 345
#define EWIDTH 346
#define HEIGHT 347
#define EHEIGHT 348
#define INPUT 349
#define INPUTFILE 350
#define EINPUT 351
#define PART_INPUT 352
#define PART_INPUTFILE 353
#define OUTPUT 354
#define OUTPUTFILE 355
#define EOUTPUT 356
#define ACTION 357
#define EACTION 358
#define PART_ACTION 359
#define COMM 360
#define ENDCOMM 361
#define IF 362
#define ENDIF 363
#define WHILE 364
#define EWHILE 365
#define SHOW_WIDGETS 366
#define EMB_VARIABLE 367
#define EMB_NUMBER 368
#define END_OF_FILE 369
#define NUMBER 370
#define STRING 371
#define UMINUS 372
#define HSEPARATOR 373
#define PART_HSEPARATOR 374
#define EHSEPARATOR 375
#define VSEPARATOR 376
#define PART_VSEPARATOR 377
#define EVSEPARATOR 378
#define COMBOBOXTEXT 379
#define PART_COMBOBOXTEXT 380
#define ECOMBOBOXTEXT 381
#define COMBOBOXENTRY 382
#define PART_COMBOBOXENTRY 383
#define ECOMBOBOXENTRY 384
#define HSCALE 385
#define PART_HSCALE 386
#define EHSCALE 387
#define VSCALE 388
#define PART_VSCALE 389
#define EVSCALE 390
#define SPINBUTTON 391
#define PART_SPINBUTTON 392
#define ESPINBUTTON 393
#define TIMER 394
#define PART_TIMER 395
#define ETIMER 396
#define TOGGLEBUTTON 397
#define PART_TOGGLEBUTTON 398
#define ETOGGLEBUTTON 399
#define STATUSBAR 400
#define PART_STATUSBAR 401
#define ESTATUSBAR 402
#define COLORBUTTON 403
#define PART_COLORBUTTON 404
#define ECOLORBUTTON 405
#define FONTBUTTON 406
#define PART_FONTBUTTON 407
#define EFONTBUTTON 408
#define TERMINAL 409
#define PART_TERMINAL 410
#define ETERMINAL 411
#define EVENTBOX 412
#define PART_EVENTBOX 413
#define EEVENTBOX 414
#define EXPANDER 415
#define PART_EXPANDER 416
#define EEXPANDER 417
#define HPANED 418
#define PART_HPANED 419
#define EHPANED 420
#define VPANED 421
#define PART_VPANED 422
#define EVPANED 423
#define ALIGNMENT 424
#define PART_ALIGNMENT 425
#define EALIGNMENT 426
#define HBUTTONBOX 427
#define PART_HBUTTONBOX 428
#define EHBUTTONBOX 429
#define VBUTTONBOX 430
#define PART_VBUTTONBOX 431
#define EVBUTTONBOX 432
#define TOOLBAR 433
#define PART_TOOLBAR 434
#define ETOOLBAR 435
#define TOOLBUTTON 436
#define PART_TOOLBUTTON 437
#define ETOOLBUTTON 438
#define TOGGLETOOLBUTTON 439
#define PART_TOGGLETOOLBUTTON 440
#define ETOGGLETOOLBUTTON 441
#define RADIOTOOLBUTTON 442
#define PART_RADIOTOOLBUTTON 443
#define ERADIOTOOLBUTTON 444
#define MENUTOOLBUTTON 445
#define PART_MENUTOOLBUTTON 446
#define EMENUTOOLBUTTON 447
#define SEPARATORTOOLITEM 448
#define PART_SEPARATORTOOLITEM 449
#define ESEPARATORTOOLITEM 450
#define CALENDAR 451
#define PART_CALENDAR 452
#define ECALENDAR 453
#define LINKBUTTON 454
#define PART_LINKBUTTON 455
#define ELINKBUTTON 456
#define SPINNER 457
#define PART_SPINNER 458
#define ESPINNER 459
#define GRID 460
#define PART_GRID 461
#define EGRID 462
#define FILECHOOSERBUTTON 463
#define PART_FILECHOOSERBUTTON 464
#define EFILECHOOSERBUTTON 465
#define INFOBAR 466
#define PART_INFOBAR 467
#define EINFOBAR 468
#define ASSISTANT 469
#define PART_ASSISTANT 470
#define EASSISTANT 471
#define ICONVIEW 472
#define PART_ICONVIEW 473
#define EICONVIEW 474
#define SCALEBUTTON 475
#define PART_SCALEBUTTON 476
#define ESCALEBUTTON 477
#define VOLUMEBUTTON 478
#define PART_VOLUMEBUTTON 479
#define EVOLUMEBUTTON 480
#define ASPECTFRAME 481
#define PART_ASPECTFRAME 482
#define EASPECTFRAME 483
#define MESSAGEDIALOG 484
#define PART_MESSAGEDIALOG 485
#define EMESSAGEDIALOG 486
#define DIALOG 487
#define PART_DIALOG 488
#define EDIALOG 489
#define ABOUTDIALOG 490
#define PART_ABOUTDIALOG 491
#define EABOUTDIALOG 492
#define RECENTCHOOSER 493
#define PART_RECENTCHOOSER 494
#define ERECENTCHOOSER 495
#define RECENTCHOOSERMENU 496
#define PART_RECENTCHOOSERMENU 497
#define ERECENTCHOOSERMENU 498
#define STATUSICON 499
#define PART_STATUSICON 500
#define ESTATUSICON 501
#define ACCELLABEL 502
#define PART_ACCELLABEL 503
#define EACCELLABEL 504
#define ARROW 505
#define PART_ARROW 506
#define EARROW 507
#define HSV 508
#define PART_HSV 509
#define EHSV 510
#define TOOLITEM 511
#define PART_TOOLITEM 512
#define ETOOLITEM 513
#define FIXED 514
#define PART_FIXED 515
#define EFIXED 516
#define LAYOUT 517
#define PART_LAYOUT 518
#define ELAYOUT 519
#define SCROLLEDWINDOW 520
#define PART_SCROLLEDWINDOW 521
#define ESCROLLEDWINDOW 522
#define HANDLEBOX 523
#define PART_HANDLEBOX 524
#define EHANDLEBOX 525
#define TOOLPALETTE 526
#define PART_TOOLPALETTE 527
#define ETOOLPALETTE 528
#define TOOLITEMGROUP 529
#define PART_TOOLITEMGROUP 530
#define ETOOLITEMGROUP 531
#define CELLVIEW 532
#define PART_CELLVIEW 533
#define ECELLVIEW 534
#define DRAWINGAREA 535
#define PART_DRAWINGAREA 536
#define EDRAWINGAREA 537
#define PART_OUTPUTFILE 538
#define SOCKET 539
#define PART_SOCKET 540
#define ESOCKET 541
#define HSCROLLBAR 542
#define PART_HSCROLLBAR 543
#define EHSCROLLBAR 544
#define VSCROLLBAR 545
#define PART_VSCROLLBAR 546
#define EVSCROLLBAR 547
#define VIEWPORT 548
#define PART_VIEWPORT 549
#define EVIEWPORT 550
#define COLORSELECTION 551
#define PART_COLORSELECTION 552
#define ECOLORSELECTION 553
#define FONTSELECTION 554
#define PART_FONTSELECTION 555
#define EFONTSELECTION 556
#define HRULER 557
#define PART_HRULER 558
#define EHRULER 559
#define VRULER 560
#define PART_VRULER 561
#define EVRULER 562
#define PLUG 563
#define PART_PLUG 564
#define EPLUG 565
#define IMAGEVIEW 566
#define PART_IMAGEVIEW 567
#define EIMAGEVIEW 568
#define CURVE 569
#define PART_CURVE 570
#define ECURVE 571

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

#line 707 "gtkdialog_parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE gtkdialog_lval;


int gtkdialog_parse (void);


#endif /* !YY_GTKDIALOG_GTKDIALOG_PARSER_H_INCLUDED  */
