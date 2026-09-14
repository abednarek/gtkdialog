%{
/*
 * gtkdialog_lexer.l:
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
** $Id: lexer.l,v 1.2 2004/11/20 20:01:25 root Exp root $
** $Log: lexer.l,v $
** Revision 1.2  2004/11/20 20:01:25  root
** - Log entries.
**
*/

#define YY_INPUT(buf,result,max_size) \
    { \
    int c = getnextchar(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }


#include <string.h>
#include <gtk/gtk.h>

#include "gtkdialog.h"
#include "attributes.h"
#include "gtkdialog_parser.h"

extern int linenumber;
gchar *Token;
static gboolean shell_content = FALSE;

void gtkdialog_lexer_reset(void)
{
	if (YY_CURRENT_BUFFER != NULL)
		yy_flush_buffer(YY_CURRENT_BUFFER);
	linenumber = 1;
	Token = "start of input";
	shell_content = FALSE;
	BEGIN(0);
}

%}

	/**************************************************************
	 * Thunor: Uncomment this to debug.
	 **************************************************************
	* %option debug */

%x EMBEDDED
%x ST_TAG_ATTR ST_QUOTED_TAGATTR
%x ST_STRING ST_QUOTED_STRING
%x COMMENT

%%

	#ifdef DEBUG
		g_message("%s(): Start", __func__);
	#endif

"<!--"                         { BEGIN(COMMENT); }
<COMMENT>"-->"                 { BEGIN(0); }
<COMMENT>\n                    { ++linenumber; }
<COMMENT>[^\n-]+               { /* Ignore comment contents. */ }
<COMMENT>.                     { /* Ignore '-' and other characters. */ }
<COMMENT><<EOF>>               {
					  Token = "end of file";
					  gtkdialog_error("Unterminated XML comment.");
					  BEGIN(0);
					  return 0;
					}

">"       				{ Token=">"; return(yytext[0]); 			}
<*>$[ \t\n]* 				{ Token="indent chars"; 				}

	/*
	 * Attributes.
	 */
<ST_TAG_ATTR>[-a-zA-Z0-9_]+ {
	#ifdef DEBUG 
	g_message(" -> TAG_ATTR_NAME: '%s'", yytext);
	#endif
	Token="tag attribute name";
	gtkdialog_lval.cval = g_strdup(yytext);
	return TAG_ATTR_NAME;
}


<ST_TAG_ATTR>= 				{
					  #ifdef DEBUG 
					  g_message(" -> spec char: '%s'", yytext);
					  #endif
					  Token="=";
					  return '=';
												}
<ST_TAG_ATTR>\> 			{
					  #ifdef DEBUG 
					  g_message(" -> spec char: '%s'", yytext);
					  #endif
					  Token=">";
					  BEGIN(0);
					  return '>';
												}

<ST_TAG_ATTR>\" 			{
					  Token = "double quote";
					  BEGIN(ST_QUOTED_TAGATTR);				}
					  
<ST_QUOTED_TAGATTR>\n			{
					  Token = "string";
					  ++linenumber;
					  yymore();						}

<ST_QUOTED_TAGATTR>[^\n\\"]+		{
					  Token = "string";
					  yymore();						}

<ST_QUOTED_TAGATTR>("\\\"")		{
					  Token = "string";
					  yymore();						}
					  
<ST_QUOTED_TAGATTR>\"			{
					  Token = yytext;
					  gtkdialog_lval.cval = g_strdup(yytext);
					  gtkdialog_lval.cval[strlen(gtkdialog_lval.cval) - 1] = '\0';
					  BEGIN(ST_TAG_ATTR);
					  return STRING;
					  							}






	/*
	 * The elements of the embedded imperative language.
	 */
\<show\/\> { 
	Token="<show/>"; 
	return SHOW_WIDGETS;
}

\<command[ ]+assignment[ ]*=[ ]*\"  {
	Token="<command>";    
	BEGIN(EMBEDDED);
	return COMM;
}

\<if[ ]+condition[ ]*=[ ]*\"  {
	Token="<if>";    
	BEGIN(EMBEDDED);
	return IF;           
}

\<while[ ]+condition[ ]*=[ ]*\"  {
	Token="<while>";    
	BEGIN(EMBEDDED);
	return WHILE;
}

<EMBEDDED>[-+*/<>=:!] {
	Token = "operator";
	return yytext[0];
}

<EMBEDDED>[a-zA-Z_][a-zA-Z0-9_]* {
	Token = "variable";
	gtkdialog_lval.cval = g_strdup(yytext);
	return EMB_VARIABLE;
}

<EMBEDDED>[0-9,.]+ {
	Token = "number";
	gtkdialog_lval.cval = g_strdup(yytext);
	return EMB_NUMBER;
}

<EMBEDDED>\" { 
	BEGIN(0);
}

"</command>" { 
	Token="</if>"; 
	return ENDCOMM;
}

"</if>" { 
	Token="</if>"; 
	return ENDIF;
}

"</while>" { 
	Token="</while>"; 
	return EWHILE;
}


	/*
	 * The widget tags.
	 */
\<window\> { 
	Token="<window>"; 
	return(WINDOW);
}

\<window[ ]+ { 
	Token="<window>"; 
	BEGIN(ST_TAG_ATTR);
	return(PART_WINDOW);  		
}

\<\/window\> { 
	Token="</window>"; 
	return(EWINDOW);
}

\<assistant\> {
	Token="<assistant>";
	return(ASSISTANT);
}

\<assistant[ ]+ {
	Token="<assistant>";
	BEGIN(ST_TAG_ATTR);
	return(PART_ASSISTANT);
}

\<\/assistant\> {
	Token="</assistant>";
	return(EASSISTANT);
}

\<messagedialog\> {
	Token="<messagedialog>";
	return(MESSAGEDIALOG);
}

\<messagedialog[ ]+ {
	Token="<messagedialog>";
	BEGIN(ST_TAG_ATTR);
	return(PART_MESSAGEDIALOG);
}

\<\/messagedialog\> {
	Token="</messagedialog>";
	return(EMESSAGEDIALOG);
}

\<dialog\> {
	Token="<dialog>";
	return(DIALOG);
}

\<dialog[ ]+ {
	Token="<dialog>";
	BEGIN(ST_TAG_ATTR);
	return(PART_DIALOG);
}

\<\/dialog\> {
	Token="</dialog>";
	return(EDIALOG);
}

\<aboutdialog\> {
	Token="<aboutdialog>";
	return(ABOUTDIALOG);
}

\<aboutdialog[ ]+ {
	Token="<aboutdialog>";
	BEGIN(ST_TAG_ATTR);
	return(PART_ABOUTDIALOG);
}

\<\/aboutdialog\> {
	Token="</aboutdialog>";
	return(EABOUTDIALOG);
}

\<aspectframe\> {
	Token="<aspectframe>";
	return(ASPECTFRAME);
}

\<aspectframe[ ]+ {
	Token="<aspectframe>";
	BEGIN(ST_TAG_ATTR);
	return(PART_ASPECTFRAME);
}

\<\/aspectframe\> {
	Token="</aspectframe>";
	return(EASPECTFRAME);
}

\<iconview\> {
	Token="<iconview>";
	return(ICONVIEW);
}

\<iconview[ ]+ {
	Token="<iconview>";
	BEGIN(ST_TAG_ATTR);
	return(PART_ICONVIEW);
}

\<\/iconview\> {
	Token="</iconview>";
	return(EICONVIEW);
}

\<cellview\> {
	Token="<cellview>";
	return(CELLVIEW);
}

\<cellview[ ]+ {
	Token="<cellview>";
	BEGIN(ST_TAG_ATTR);
	return(PART_CELLVIEW);
}

\<\/cellview\> {
	Token="</cellview>";
	return(ECELLVIEW);
}

\<drawingarea\> {
	Token="<drawingarea>";
	return(DRAWINGAREA);
}

\<drawingarea[ ]+ {
	Token="<drawingarea>";
	BEGIN(ST_TAG_ATTR);
	return(PART_DRAWINGAREA);
}

\<\/drawingarea\> {
	Token="</drawingarea>";
	return(EDRAWINGAREA);
}

\<scalebutton\> {
	Token="<scalebutton>";
	return(SCALEBUTTON);
}

\<scalebutton[ ]+ {
	Token="<scalebutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_SCALEBUTTON);
}

\<\/scalebutton\> {
	Token="</scalebutton>";
	return(ESCALEBUTTON);
}

\<volumebutton\> {
	Token="<volumebutton>";
	return(VOLUMEBUTTON);
}

\<volumebutton[ ]+ {
	Token="<volumebutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_VOLUMEBUTTON);
}

\<\/volumebutton\> {
	Token="</volumebutton>";
	return(EVOLUMEBUTTON);
}


\<notebook\> { 
	Token="<notebook>"; 
	return(NOTEBOOK);
}

\<notebook[ ]+ { 
	Token="<notebook>";
	BEGIN(ST_TAG_ATTR);
	return(PART_NOTEBOOK);
}

\<\/notebook\> { 
	Token="</notebook>"; 
	return(ENOTEBOOK);
}


\<vbox\> { 
	Token="<vbox>"; 
	return(VBOX);
}

\<vbox[ ]+ { 
	Token="<vbox>"; 
	BEGIN(ST_TAG_ATTR);
	return(PART_VBOX);
}

\<\/vbox\> { 
	Token="</vbox>"; 
	return(EVBOX);
}


\<hbox\> { 
	Token="<hbox>"; 
	return(HBOX);
}

\<hbox[ ]+ { 
	Token="<hbox>";
	BEGIN(ST_TAG_ATTR);
	return(PART_HBOX);
}

\<\/hbox\> { 
	Token="</hbox>"; 
	return(EHBOX);
}


\<hbuttonbox\> {
	Token="<hbuttonbox>";
	return(HBUTTONBOX);
}

\<hbuttonbox[ ]+ {
	Token="<hbuttonbox>";
	BEGIN(ST_TAG_ATTR);
	return(PART_HBUTTONBOX);
}

\<\/hbuttonbox\> {
	Token="</hbuttonbox>";
	return(EHBUTTONBOX);
}


\<vbuttonbox\> {
	Token="<vbuttonbox>";
	return(VBUTTONBOX);
}

\<vbuttonbox[ ]+ {
	Token="<vbuttonbox>";
	BEGIN(ST_TAG_ATTR);
	return(PART_VBUTTONBOX);
}

\<\/vbuttonbox\> {
	Token="</vbuttonbox>";
	return(EVBUTTONBOX);
}


\<toolbar\> {
	Token="<toolbar>";
	return(TOOLBAR);
}

\<toolbar[ ]+ {
	Token="<toolbar>";
	BEGIN(ST_TAG_ATTR);
	return(PART_TOOLBAR);
}

\<\/toolbar\> {
	Token="</toolbar>";
	return(ETOOLBAR);
}

\<toolpalette\> {
	Token="<toolpalette>";
	return(TOOLPALETTE);
}

\<toolpalette[ ]+ {
	Token="<toolpalette>";
	BEGIN(ST_TAG_ATTR);
	return(PART_TOOLPALETTE);
}

\<\/toolpalette\> {
	Token="</toolpalette>";
	return(ETOOLPALETTE);
}

\<toolitemgroup\> {
	Token="<toolitemgroup>";
	return(TOOLITEMGROUP);
}

\<toolitemgroup[ ]+ {
	Token="<toolitemgroup>";
	BEGIN(ST_TAG_ATTR);
	return(PART_TOOLITEMGROUP);
}

\<\/toolitemgroup\> {
	Token="</toolitemgroup>";
	return(ETOOLITEMGROUP);
}


\<toolbutton\> {
	Token="<toolbutton>";
	return(TOOLBUTTON);
}

\<toolbutton[ ]+ {
	Token="<toolbutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_TOOLBUTTON);
}

\<\/toolbutton\> {
	Token="</toolbutton>";
	return(ETOOLBUTTON);
}


\<toolitem\> {
	Token="<toolitem>";
	return(TOOLITEM);
}

\<toolitem[ ]+ {
	Token="<toolitem>";
	BEGIN(ST_TAG_ATTR);
	return(PART_TOOLITEM);
}

\<\/toolitem\> {
	Token="</toolitem>";
	return(ETOOLITEM);
}


\<toggletoolbutton\> {
	Token="<toggletoolbutton>";
	return(TOGGLETOOLBUTTON);
}

\<toggletoolbutton[ ]+ {
	Token="<toggletoolbutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_TOGGLETOOLBUTTON);
}

\<\/toggletoolbutton\> {
	Token="</toggletoolbutton>";
	return(ETOGGLETOOLBUTTON);
}


\<radiotoolbutton\> {
	Token="<radiotoolbutton>";
	return(RADIOTOOLBUTTON);
}

\<radiotoolbutton[ ]+ {
	Token="<radiotoolbutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_RADIOTOOLBUTTON);
}

\<\/radiotoolbutton\> {
	Token="</radiotoolbutton>";
	return(ERADIOTOOLBUTTON);
}


\<menutoolbutton\> {
	Token="<menutoolbutton>";
	return(MENUTOOLBUTTON);
}

\<menutoolbutton[ ]+ {
	Token="<menutoolbutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_MENUTOOLBUTTON);
}

\<\/menutoolbutton\> {
	Token="</menutoolbutton>";
	return(EMENUTOOLBUTTON);
}


\<separatortoolitem\> {
	Token="<separatortoolitem>";
	return(SEPARATORTOOLITEM);
}

\<separatortoolitem[ ]+ {
	Token="<separatortoolitem>";
	BEGIN(ST_TAG_ATTR);
	return(PART_SEPARATORTOOLITEM);
}

\<\/separatortoolitem\> {
	Token="</separatortoolitem>";
	return(ESEPARATORTOOLITEM);
}


\<calendar\> {
	Token="<calendar>";
	return(CALENDAR);
}

\<calendar[ ]+ {
	Token="<calendar>";
	BEGIN(ST_TAG_ATTR);
	return(PART_CALENDAR);
}

\<\/calendar\> {
	Token="</calendar>";
	return(ECALENDAR);
}


\<alignment\> {
	Token="<alignment>";
	return(ALIGNMENT);
}

\<alignment[ ]+ {
	Token="<alignment>";
	BEGIN(ST_TAG_ATTR);
	return(PART_ALIGNMENT);
}

\<\/alignment\> {
	Token="</alignment>";
	return(EALIGNMENT);
}


\<grid\> {
	Token="<grid>";
	return(GRID);
}

\<grid[ ]+ {
	Token="<grid>";
	BEGIN(ST_TAG_ATTR);
	return(PART_GRID);
}

\<\/grid\> {
	Token="</grid>";
	return(EGRID);
}

\<fixed\> {
	Token="<fixed>";
	return(FIXED);
}

\<fixed[ ]+ {
	Token="<fixed>";
	BEGIN(ST_TAG_ATTR);
	return(PART_FIXED);
}

\<\/fixed\> {
	Token="</fixed>";
	return(EFIXED);
}

\<layout\> {
	Token="<layout>";
	return(LAYOUT);
}

\<layout[ ]+ {
	Token="<layout>";
	BEGIN(ST_TAG_ATTR);
	return(PART_LAYOUT);
}

\<\/layout\> {
	Token="</layout>";
	return(ELAYOUT);
}

\<scrolledwindow\> {
	Token="<scrolledwindow>";
	return(SCROLLEDWINDOW);
}

\<scrolledwindow[ ]+ {
	Token="<scrolledwindow>";
	BEGIN(ST_TAG_ATTR);
	return(PART_SCROLLEDWINDOW);
}

\<\/scrolledwindow\> {
	Token="</scrolledwindow>";
	return(ESCROLLEDWINDOW);
}

\<viewport\> {
	Token="<viewport>";
	return(VIEWPORT);
}

\<viewport[ ]+ {
	Token="<viewport>";
	BEGIN(ST_TAG_ATTR);
	return(PART_VIEWPORT);
}

\<\/viewport\> {
	Token="</viewport>";
	return(EVIEWPORT);
}

\<handlebox\> {
	Token="<handlebox>";
	return(HANDLEBOX);
}

\<handlebox[ ]+ {
	Token="<handlebox>";
	BEGIN(ST_TAG_ATTR);
	return(PART_HANDLEBOX);
}

\<\/handlebox\> {
	Token="</handlebox>";
	return(EHANDLEBOX);
}


\<hpaned\> {
	Token="<hpaned>";
	return(HPANED);
}

\<hpaned[ ]+ {
	Token="<hpaned>";
	BEGIN(ST_TAG_ATTR);
	return(PART_HPANED);
}

\<\/hpaned\> {
	Token="</hpaned>";
	return(EHPANED);
}


\<vpaned\> {
	Token="<vpaned>";
	return(VPANED);
}

\<vpaned[ ]+ {
	Token="<vpaned>";
	BEGIN(ST_TAG_ATTR);
	return(PART_VPANED);
}

\<\/vpaned\> {
	Token="</vpaned>";
	return(EVPANED);
}


\<frame\> { 
	Token="<frame>"; 
	gtkdialog_lval.cval = g_strdup("");
	return(FRAME); 	
}

\<frame\ [^\>]+\> {
	Token="<frame>";
	gtkdialog_lval.cval = g_strdup(&yytext[7]);
	gtkdialog_lval.cval[strlen(gtkdialog_lval.cval)-1] = 0;
	return FRAME; 
}

\<\/frame\> { 
	Token="</frame>"; 
	return(EFRAME);
}


\<text\> { 
	Token="<text>"; 
	return(TEXT);
}

\<text[ ]+ { 
	Token="<text>"; 
	BEGIN(ST_TAG_ATTR);
	return(PART_TEXT);
}

\<\/text\> { 
	Token="</text>"; 
	return(ETEXT);
}


\<pixmap\> { 
	Token="<pixmap>"; 
	return(PIXMAP);
}

\<\/pixmap\> { 
	Token="</pixmap>"; 
	return(EPIXMAP); 
}

\<pixmap[ ]+ { 
	Token="<pixmap>"; 
	BEGIN(ST_TAG_ATTR);
	return(PART_PIXMAP);
}


\<entry\> { 
	Token="<entry>"; 
	return(ENTRY);
}

\<\/entry\> { 
	Token="</entry>"; 
	return(EENTRY);
}

\<entry[ ]+ { 
	Token="<entry>"; 
	BEGIN(ST_TAG_ATTR);
	return(PART_ENTRY);
}


\<edit\> { 
	Token="<edit>"; 
	return(EDIT);
}

\<edit[ ]+ { 
	Token="<edit>";
	BEGIN(ST_TAG_ATTR);
	return(PART_EDIT);
}

\<\/edit\> { 
	Token="</edit>"; 
	return(EEDIT);
}


\<tree\> { 
	Token="<tree>";  
	return  TREE;
}

\<tree[ ]+ { 
	Token="<tree>";  
	BEGIN(ST_TAG_ATTR);
	return PART_TREE;
}

\<\/tree\> {
	Token="</tree>"; 
	return ETREE;
}


\<chooser\> { 
	Token="<chooser>"; 
	return(CHOOSER);
}

\<\/chooser\> { 
	Token="</chooser>"; 
	return(ECHOOSER);
}

\<chooser[ ]+ { 
	Token="<chooser>";  
	BEGIN(ST_TAG_ATTR);
	return PART_CHOOSER;
}


\<button\> { 
	Token="<button>"; 
	return(BUTTON);
}

\<\/button\> { 
	Token="</button>"; 
	return(EBUTTON);
}

\<button[ ]+/([-a-zA-Z_]*=) { 
	//g_message("PART_BUTTON: '%s'", yytext);
	Token="<button>";  
	BEGIN(ST_TAG_ATTR);
	return PART_BUTTON;
}

\<button\ ok\>      			{ Token="<button>"; return(BUTTONOK);  			}
\<button\ cancel\>  			{ Token="<button>"; return(BUTTONCANCEL);  		}
\<button\ help\>    			{ Token="<button>"; return(BUTTONHELP);  		}
\<button\ yes\>     			{ Token="<button>"; return(BUTTONYES);  		}
\<button\ no\>      			{ Token="<button>"; return(BUTTONNO);  			}

\<checkbox\>				{ Token="<checkbox>"; return(CHECKBOX);       		}
\<\/checkbox\>  			{ Token="</checkbox>"; return(ECHECKBOX);     		}
\<checkbox[ ]+  			{ Token="<checkbox>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_CHECKBOX);  				}

\<radiobutton\>    			{ Token="<radiobutton>"; return(RADIO);      		}
\<\/radiobutton\>  			{ Token="</radiobutton>"; return(ERADIO);    		}
\<radiobutton[ ]+  			{ Token="<radiobutton>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_RADIO);  					}

\<progressbar\>    			{ Token="<progressbar>"; return(PROGRESSBAR);      	}
\<\/progressbar\>  			{ Token="</progressbar>"; return(EPROGRESSBAR);    	}
\<progressbar[ ]+  			{ Token="<progressbar>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_PROGRESSBAR);  				}


\<radio\>    				{ Token="<radio>"; return(RADIO);  			}
\<\/radio\>  				{ Token="</radio>"; return(ERADIO);  			}

\<gvim\>     				{ Token="<gvim>"; return(GVIM);  			}
\<\/gvim\>   				{ Token="</gvim>"; return(EGVIM);  			}

\<socket\>    				{ Token="<socket>"; return(SOCKET); }
\<socket[ ]+  				{ Token="<socket>";
					  BEGIN(ST_TAG_ATTR);
					  return(PART_SOCKET); }
\<\/socket\>  				{ Token="</socket>"; return(ESOCKET); }

\<combobox\>    			{ Token="<combobox>"; return(COMBOBOX);		}
\<combobox[ ]+  			{ Token="<combobox>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_COMBOBOX); 					}
\<\/combobox\>  			{ Token="</combobox>"; return(ECOMBOBOX);	}

\<list\>    				{ Token="<list>";  return(LIST);  			}
\<list[ ]+      			{ Token="<list>";  
					  BEGIN(ST_TAG_ATTR);
					  return(PART_LIST); 					}
\<\/list\>  				{ Token="</list>"; return(ELIST);  			}

\<table\>    				{ Token="<table>"; return(TABLE); }
\<table[ ]+    				{ Token="<table>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_TABLE); }
\<\/table\>  				{ Token="</table>"; return(ETABLE); }

\<menubar\>    				{ Token="<menubar>"; return(MENUBAR); }
\<menubar[ ]+    			{ Token="<menubar>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_MENUBAR); }
\<\/menubar\>  				{ Token="</menubar>"; return(EMENUBAR); }

\<menu\>    				{ Token="<menu>"; return(MENU); }
\<menu[ ]+    				{ Token="<menu>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_MENU); }
\<\/menu\>  				{ Token="</menu>"; return(EMENU); }

\<popupmenu\>    			{ Token="<popupmenu>"; return(POPUPMENU); }
\<popupmenu[ ]+    			{ Token="<popupmenu>";
					  BEGIN(ST_TAG_ATTR);
					  return(PART_POPUPMENU); }
\<\/popupmenu\>  			{ Token="</popupmenu>"; return(EPOPUPMENU); }

\<menuitem\>    			{ Token="<menuitem>"; return(MENUITEM); }
\<menuitem[ ]+  			{ Token="<menuitem>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_MENUITEM); }
\<\/menuitem\>  			{ Token="</menuitem>"; return(EMENUITEM); }

\<menuitemseparator\>    	{ Token="<menuitemseparator>"; return(MENUITEMSEPARATOR); }
\<\/menuitemseparator\>		{ Token="</menuitemseparator>"; return(EMENUITEMSEPARATOR); }

\<separator\>    			{ Token="<menuitemseparator>"; return(MENUITEMSEPARATOR); }
\<\/separator\>				{ Token="</menuitemseparator>"; return(EMENUITEMSEPARATOR); }

	/**************************************************************
	 * Thunor: Newly supported widgets.
	 **************************************************************/
\<hseparator\>    			{ Token="<hseparator>"; return(HSEPARATOR); }
\<hseparator[ ]+  			{ Token="<hseparator>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_HSEPARATOR); }
\<\/hseparator\>			{ Token="</hseparator>"; return(EHSEPARATOR); }

\<vseparator\>    			{ Token="<vseparator>"; return(VSEPARATOR); }
\<vseparator[ ]+  			{ Token="<vseparator>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_VSEPARATOR); }
\<\/vseparator\>			{ Token="</vseparator>"; return(EVSEPARATOR); }

\<comboboxtext\>   			{ Token="<comboboxtext>"; return(COMBOBOXTEXT); }
\<comboboxtext[ ]+  		{ Token="<comboboxtext>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_COMBOBOXTEXT); }
\<\/comboboxtext\>  		{ Token="</comboboxtext>"; return(ECOMBOBOXTEXT); }

\<comboboxentry\>   		{ Token="<comboboxentry>"; return(COMBOBOXENTRY); }
\<comboboxentry[ ]+  		{ Token="<comboboxentry>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_COMBOBOXENTRY); }
\<\/comboboxentry\>  		{ Token="</comboboxentry>"; return(ECOMBOBOXENTRY); }

\<hscale\>					{ Token="<hscale>"; return(HSCALE); }
\<hscale[ ]+				{ Token="<hscale>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_HSCALE); }
\<\/hscale\>				{ Token="</hscale>"; return(EHSCALE); }

\<vscale\>					{ Token="<vscale>"; return(VSCALE); }
\<vscale[ ]+				{ Token="<vscale>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_VSCALE); }
\<\/vscale\>				{ Token="</vscale>"; return(EVSCALE); }

\<hscrollbar\>				{ Token="<hscrollbar>"; return(HSCROLLBAR); }
\<hscrollbar[ ]+			{ Token="<hscrollbar>";
					  BEGIN(ST_TAG_ATTR);
					  return(PART_HSCROLLBAR); }
\<\/hscrollbar\>			{ Token="</hscrollbar>"; return(EHSCROLLBAR); }

\<vscrollbar\>				{ Token="<vscrollbar>"; return(VSCROLLBAR); }
\<vscrollbar[ ]+			{ Token="<vscrollbar>";
					  BEGIN(ST_TAG_ATTR);
					  return(PART_VSCROLLBAR); }
\<\/vscrollbar\>			{ Token="</vscrollbar>"; return(EVSCROLLBAR); }

\<spinbutton\>				{ Token="<spinbutton>"; return(SPINBUTTON); }
\<spinbutton[ ]+			{ Token="<spinbutton>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_SPINBUTTON); }
\<\/spinbutton\>			{ Token="</spinbutton>"; return(ESPINBUTTON); }

\<timer\>					{ Token="<timer>"; return(TIMER); }
\<timer[ ]+					{ Token="<timer>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_TIMER); }
\<\/timer\>					{ Token="</timer>"; return(ETIMER); }

\<togglebutton\>			{ Token="<togglebutton>"; return(TOGGLEBUTTON); }
\<togglebutton[ ]+			{ Token="<togglebutton>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_TOGGLEBUTTON); }
\<\/togglebutton\>			{ Token="</togglebutton>"; return(ETOGGLEBUTTON); }

\<statusbar\>				{ Token="<statusbar>"; return(STATUSBAR); }
\<statusbar[ ]+				{ Token="<statusbar>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_STATUSBAR); }
\<\/statusbar\>				{ Token="</statusbar>"; return(ESTATUSBAR); }

\<colorbutton\>				{ Token="<colorbutton>"; return(COLORBUTTON); }
\<colorbutton[ ]+			{ Token="<colorbutton>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_COLORBUTTON); }
\<\/colorbutton\>			{ Token="</colorbutton>"; return(ECOLORBUTTON); }

\<colorselection\>			{ Token="<colorselection>"; return(COLORSELECTION); }
\<colorselection[ ]+		{ Token="<colorselection>";
					  BEGIN(ST_TAG_ATTR);
					  return(PART_COLORSELECTION); }
\<\/colorselection\>		{ Token="</colorselection>"; return(ECOLORSELECTION); }

\<fontbutton\>				{ Token="<fontbutton>"; return(FONTBUTTON); }
\<fontbutton[ ]+			{ Token="<fontbutton>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_FONTBUTTON); }
\<\/fontbutton\>			{ Token="</fontbutton>"; return(EFONTBUTTON); }

\<fontselection\>			{ Token="<fontselection>"; return(FONTSELECTION); }
\<fontselection[ ]+		{ Token="<fontselection>";
					  BEGIN(ST_TAG_ATTR);
					  return(PART_FONTSELECTION); }
\<\/fontselection\>		{ Token="</fontselection>"; return(EFONTSELECTION); }

\<terminal\>				{ Token="<terminal>"; return(TERMINAL); }
\<terminal[ ]+				{ Token="<terminal>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_TERMINAL); }
\<\/terminal\>				{ Token="</terminal>"; return(ETERMINAL); }

\<eventbox\>				{ Token="<eventbox>"; return(EVENTBOX); }
\<eventbox[ ]+				{ Token="<eventbox>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_EVENTBOX); }
\<\/eventbox\>				{ Token="</eventbox>"; return(EEVENTBOX); }

\<expander\>				{ Token="<expander>"; return(EXPANDER); }
\<expander[ ]+				{ Token="<expander>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_EXPANDER); }
\<\/expander\>				{ Token="</expander>"; return(EEXPANDER); }



\<label\>    				{ Token="<label>"; return(LABEL);    			}
\<\/label\>  				{ Token="</label>"; return(ELABEL);  			}

\<item\>     				{ Token="<item>"; return(ITEM);      			}
\<\/item\>   				{ Token="</item>"; return(EITEM);    			}
\<item[ ]+   				{ Token="<item>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_ITEM); 					}

\<default\>      			{ Token="<default>"; return(DEFAULT);  			}
\<\/default\>    			{ Token="</default>"; return(EDEFAULT);  		}

\<visible\>      			{ Token="<sensitive>"; return(SENSITIVE);  		}
\<\/visible\>    			{ Token="</sensitive>"; return(ESENSITIVE);  	}

\<sensitive\>      			{ Token="<sensitive>"; return(SENSITIVE);  		}
\<\/sensitive\>    			{ Token="</sensitive>"; return(ESENSITIVE);  	}

\<height\>       			{ Token="<height>"; return(HEIGHT);  			}
\<\/height\>     			{ Token="</height>"; return(EHEIGHT);  			}

\<width\>        			{ Token="<width>"; return(WIDTH);   			}
\<\/width\>      			{ Token="</width>"; return(EWIDTH);   			}

\<input\>        			{ Token="<input>"; shell_content=TRUE;
					  return(INPUT);   					}
\<\/input\>      			{ Token="</input>"; shell_content=FALSE;
					  return(EINPUT);   					}
\<\input\ file\> 			{ Token="<input file>"; return(INPUTFILE);   		}
\<\input\ file[ ]+ 			{ Token="<input file>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_INPUTFILE);   				}
\<\input\ [ ]*   			{ Token="<input>"; 
					  shell_content=TRUE;
					  BEGIN(ST_TAG_ATTR);
					  return(PART_INPUT);   				}

\<output\>        			{ Token="<output>"; return(OUTPUT);   			}
\<\/output\>      			{ Token="</output>"; return(EOUTPUT);   		}
\<\output\ file\> 			{ Token="<output>"; return(OUTPUTFILE);   		}
\<\output\ file[ ]+ 			{ Token="<output>";
					  BEGIN(ST_TAG_ATTR);
					  return(PART_OUTPUTFILE);   			}

\<variable\>     			{ Token="<variable>"; return(VARIABLE);  		}
\<variable[ ]+   			{ Token="<variable>"; 
					  BEGIN(ST_TAG_ATTR);
					  return(PART_VARIABLE); 				}
\<\/variable\>   			{ Token="</variable>"; return(EVARIABLE);  		}

	/*
	** The definition of action tags.
	*/

\<action\>     				{ Token="<action>"; shell_content=TRUE;
					  return(ACTION);  					}
\<action[ ]+   				{ Token="<action>";
					  shell_content=TRUE;
					  BEGIN(ST_TAG_ATTR);
					  return(PART_ACTION); 					}
\<\/action\>     			{ Token="</action>"; shell_content=FALSE;
					  return(EACTION);  					}






\<linkbutton\> {
	Token="<linkbutton>";
	return(LINKBUTTON);
}

\<linkbutton[ ]+ {
	Token="<linkbutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_LINKBUTTON);
}

\<\/linkbutton\> {
	Token="</linkbutton>";
	return(ELINKBUTTON);
}


\<spinner\> {
	Token="<spinner>";
	return(SPINNER);
}

\<spinner[ ]+ {
	Token="<spinner>";
	BEGIN(ST_TAG_ATTR);
	return(PART_SPINNER);
}

\<\/spinner\> {
	Token="</spinner>";
	return(ESPINNER);
}


\<filechooserbutton\> {
	Token="<filechooserbutton>";
	return(FILECHOOSERBUTTON);
}

\<filechooserbutton[ ]+ {
	Token="<filechooserbutton>";
	BEGIN(ST_TAG_ATTR);
	return(PART_FILECHOOSERBUTTON);
}

\<\/filechooserbutton\> {
	Token="</filechooserbutton>";
	return(EFILECHOOSERBUTTON);
}


\<recentchooser\> {
	Token="<recentchooser>";
	return(RECENTCHOOSER);
}

\<recentchooser[ ]+ {
	Token="<recentchooser>";
	BEGIN(ST_TAG_ATTR);
	return(PART_RECENTCHOOSER);
}

\<\/recentchooser\> {
	Token="</recentchooser>";
	return(ERECENTCHOOSER);
}


\<recentchoosermenu\> {
	Token="<recentchoosermenu>";
	return(RECENTCHOOSERMENU);
}

\<recentchoosermenu[ ]+ {
	Token="<recentchoosermenu>";
	BEGIN(ST_TAG_ATTR);
	return(PART_RECENTCHOOSERMENU);
}

\<\/recentchoosermenu\> {
	Token="</recentchoosermenu>";
	return(ERECENTCHOOSERMENU);
}

\<statusicon\> {
	Token="<statusicon>";
	return(STATUSICON);
}

\<statusicon[ ]+ {
	Token="<statusicon>";
	BEGIN(ST_TAG_ATTR);
	return(PART_STATUSICON);
}

\<\/statusicon\> {
	Token="</statusicon>";
	return(ESTATUSICON);
}

\<accellabel\> {
	Token="<accellabel>";
	return(ACCELLABEL);
}

\<accellabel[ ]+ {
	Token="<accellabel>";
	BEGIN(ST_TAG_ATTR);
	return(PART_ACCELLABEL);
}

\<\/accellabel\> {
	Token="</accellabel>";
	return(EACCELLABEL);
}

\<arrow\> {
	Token="<arrow>";
	return(ARROW);
}

\<arrow[ ]+ {
	Token="<arrow>";
	BEGIN(ST_TAG_ATTR);
	return(PART_ARROW);
}

\<\/arrow\> {
	Token="</arrow>";
	return(EARROW);
}

\<hsv\> {
	Token="<hsv>";
	return(HSV);
}
\<hsv[ ]+ {
	Token="<hsv>";
	BEGIN(ST_TAG_ATTR);
	return(PART_HSV);
}
\<\/hsv\> {
	Token="</hsv>";
	return(EHSV);
}

\<hruler\> {
	Token="<hruler>";
	return(HRULER);
}
\<hruler[ ]+ {
	Token="<hruler>";
	BEGIN(ST_TAG_ATTR);
	return(PART_HRULER);
}
\<\/hruler\> {
	Token="</hruler>";
	return(EHRULER);
}

\<vruler\> {
	Token="<vruler>";
	return(VRULER);
}
\<vruler[ ]+ {
	Token="<vruler>";
	BEGIN(ST_TAG_ATTR);
	return(PART_VRULER);
}
\<\/vruler\> {
	Token="</vruler>";
	return(EVRULER);
}

\<plug\> {
	Token="<plug>";
	return(PLUG);
}
\<plug[ ]+ {
	Token="<plug>";
	BEGIN(ST_TAG_ATTR);
	return(PART_PLUG);
}
\<\/plug\> {
	Token="</plug>";
	return(EPLUG);
}


\<infobar\> {
	Token="<infobar>";
	return(INFOBAR);
}

\<infobar[ ]+ {
	Token="<infobar>";
	BEGIN(ST_TAG_ATTR);
	return(PART_INFOBAR);
}

\<\/infobar\> {
	Token="</infobar>";
	return(EINFOBAR);
}


\<\/[^> ]*\> { 
	Token=g_strdup(yytext); 
	gtkdialog_error("Unknown end tag.");  
}

	/* Matching "<!" here would prevent it from starting a comment. */
\<[^!> ]*\> {
	Token=g_strdup(yytext); 
	gtkdialog_error("Unknown tag.");      
}
					
					/*
					 * The quoted string. It can contain anything, except
					 * unescaped double quote.
					 */
\"	  				{
					  Token = "double quote";
					  BEGIN(ST_QUOTED_STRING);				}
					  
<ST_QUOTED_STRING>\n			{
					  ++linenumber;
					  yymore();						}

<ST_QUOTED_STRING>[^\n\\"]+		{
					  yymore();						}

<ST_QUOTED_STRING>"\\\""		{
					  yymore();						}
					  
<ST_QUOTED_STRING>\"			{
					  Token = yytext;
					  gtkdialog_lval.cval = g_strdup(yytext);
					  gtkdialog_lval.cval[strlen(gtkdialog_lval.cval) - 1] = '\0';
					  BEGIN(0);
					  return STRING;
					  							}
												
					/*
					 * An unquoted string can contain anything except 
					 * unescaped '<'.
					 */
[^ \n\t\>\<\"][^\n\<\\]*  		{
					  Token = "string";
					  BEGIN(ST_STRING);
					  yymore();
												}
<ST_STRING>[^\n\<\\]+  			{
					  Token = "string";
					  yymore();
												}


<ST_STRING>"\\<"			{
					  yymore();
												}

<ST_STRING>\\.				{
					  yymore();
												}

<ST_STRING>"</"/(action|input)">"	{
					  gtkdialog_lval.cval = g_strdup(yytext);
					  gtkdialog_lval.cval[strlen(gtkdialog_lval.cval) - 2] = '\0';
					  unput('/');
					  unput('<');
					  BEGIN(0);
					  return STRING;
										}

<ST_STRING>"<"				{
					  if (shell_content) {
						yymore();
					  } else {
					  gtkdialog_lval.cval = g_strdup(yytext);
					  gtkdialog_lval.cval[strlen(gtkdialog_lval.cval) - 1] = '\0';
					  unput('<');
					  BEGIN(0);
					  return STRING;
					  }
					  							}

	/* A shell redirection may be the first character of an action/input. */
"<"					{
					  if (shell_content) {
						Token = "string";
						BEGIN(ST_STRING);
						yymore();
					  }
										}
												

<*>\n 					{
					  ++linenumber;
												}
<*>[\t ] 					{
					  /* */
												}

<*>.                                       {
					  /* we do nothing, this is a dropped char. */
												}
%%
