/* -*- C -*- */
/*
 * Copyright (c) 2000-2012, 2013 S. Bhatnagar (bhatnagar dot sanjay at gmail dot com)
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/* $Id: shell.l,v 2.3 1999/06/18 09:57:28 sanjay Exp $ */
%{
/* #ifdef __cplusplus */
/*   extern "C" { */
/* #endif */
#include "shell.h"
#include "shell.tab.h"
#ifdef GNUREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
  using namespace std;
/*
   This piece is added only if commandline editing (via GNU Readline and
   History lib.) is opted for.  The size of the actual code increases 
   substantially.

   Also, currently, this will work ONLY with flex (GNU lex).  UNIX
   lex, does input char-by-char where as flex does it in blocks (which
   is more effecient).  Also, the macro names used by lex and flex are
   different (input(c) and YY_INPUT(...) resp.).  Ultimately, there will
   be another #define here for lex to define input() using GNU Readline
   calls instead of gets().
*/
//typedef yy_size_t cl_size_t;
typedef int cl_size_t;
extern char *sh_Prompt;
char *sh_line=0;
void (*CL_INPUT)(char *, int *, int)=NULL;
void inp_readline(char *,cl_size_t &,int);

#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) result=inp_readline(buf,max_size)
    //#define YY_INPUT(buf,result,max_size) (*CL_INPUT)(buf,&result,max_size)
static int inp_readline(char *buf,  size_t max_size)
{ 
  int  n,result; 
  static size_t index=0;   
  rl_instream=stdin; 
  rl_outstream=stderr; 
  if (index==0) 
    { 
      if (sh_line) free(sh_line);sh_line=0; 
      sh_line = readline(sh_Prompt);
      if (sh_line) 
	{
	  stripwhite(sh_line); 
	  if (history_search_prefix(sh_line, -1) < 0 || 
	      where_history() < history_length-1) 
	    if (sh_line&&strlen(sh_line)) add_history(sh_line);
	}
      else  {result = YY_NULL; buf[0]='\0';} 
    }
  if (sh_line) 
    { 
      n = max_size>strlen(sh_line)-index?strlen(sh_line)-index:max_size; 
      if (strlen(sh_line) <= index) 
	{ 
	  index = 0; 
	  free(sh_line); sh_line=0;
	  result=1; 
	  buf[0]='\n';
	} 
      else if (n==0) 
	{index =0; result =1;free(sh_line);sh_line=0;buf[0]='\n';} 
      else 
	{ 
	  size_t i,j=0;
	  for (i=index;i<n+index;i++)buf[j++]=sh_line[i]; 
	  buf[j]='\0';
	  index += n; 
	  result = n; 
	} 
    }
  return result;
}
  /*
{ \
  int  n; \
  static int index=0;   \
  rl_instream=stdin; \
  rl_outstream=stderr; \
  if (index==0) \
    { \
      if (sh_line) free(sh_line);sh_line=0; \
      sh_line = readline(sh_Prompt);\
      if (sh_line) \
	{\
	  stripwhite(sh_line); \
	  if (history_search_prefix(sh_line, -1) < 0 || \
	      where_history() < history_length-1) \
	    if (sh_line&&strlen(sh_line)) add_history(sh_line);\
	}\
      else  {result = YY_NULL; buf[0]='\0';} \
    }\
    if (sh_line) \
    { \
	n = max_size>strlen(sh_line)-index?strlen(sh_line)-index:max_size; \
	if (strlen(sh_line) <= index) \
	{ \
	    index = 0; \
	    free(sh_line); sh_line=0;\
	    result=1; \
	    buf[0]='\n';\
	} \
	else if (n==0) {index =0; result =1;free(sh_line);sh_line=0;buf[0]='\n';} \
        else { \
	    int i,j=0;\
	    for (i=index;i<n+index;i++)buf[j++]=sh_line[i]; \
	    buf[j]='\0';\
	    index += n; \
	    result = n; \
	} \
    } \
}
*/
#endif 

void LexReSet() { yyrestart( stdin );}

#undef ECHO
#define ECHO      /*fprintf(stderr,"\"%s\"\n",yytext);*/
#define ECHOS(s)  /* fprintf(stderr,"\"%s %s\"\n",s,yytext); */
/* #ifdef __cpluscplus */
/* 	     } */
/* #endif */
%}
VNAMES   (([a-zA-Z][a-zA-Z\-_0-9]*)|.)
DIGIT   [0-9]+
S       [\+\-]?
DOT     \.
RNUM    (({DIGIT})|({DIGIT}{DOT})|({DIGIT}{DOT}{DIGIT})|({DOT}{DIGIT}))
NUM     (({RNUM})|({DIGIT}))
%s  VARARG CMDARG ARG VARNAME
%option noyywrap
%%
\$                                   {/*Catch the dereferencing char.*/
                                       BEGIN(VARNAME);
                                       ECHO; 
                                       return '$';
                                     }
"["                                  {/*Catch the indexing chars. only for*/
                                      /*Arguments                         */
                                       ECHO;return yytext[0];
                                     }
"]"                                  {
                                       ECHO;return yytext[0];
                                     }
<ARG>{DIGIT}                         {/*Catch numbers*/
                                      ECHO;
                                      sscanf(yytext,"%lf",&sh_lval.Result);
				      return NUMBER;
				     }
<INITIAL,VARARG,ARG>[ \t]            {/*Eat traling BLANKS and TABS */
                                       ECHOS("EAT");
                                     }
<INITIAL>"\\".*                      { /* ESCAPE the internal symbols */
                                       BEGIN(ARG);
                                       ECHOS("INIT");
				       /*
                                       s0=AllocVSymb(1);
                                       s0->Name = (char *)
                                           malloc(strlen(&yytext[1])+1);
                                       strcpy(s0->Name,&yytext[1]);
				       sh_lval.symb = s0;
				       */
                                       sh_lval.String = yytext;
                                       return UNDEF;
                                     }
<VARNAME,INITIAL>{VNAMES}            {/*Catch internal commands and symbols.
                                        If non is detected, return UNDEF */
                                       BEGIN(ARG);
				       if ((sh_lval.symb=
					    SearchVSymb(yytext,sh_SymbTab))!=0)
					 {
					   ECHOS("VAR");
					   return VAR;
					 }
				       else if ((sh_lval.cmd=
						 SearchCSymb(yytext,sh_CmdTab))
						!=0)
					 {
					   ECHOS("CMD");
					   return COMMAND;
					 }
				       else 
					 {
					   ECHOS("UNDEF");
					   sh_lval.String = (char *)
					     malloc(strlen(yytext)+1);
					   strcpy(sh_lval.String,yytext);
					   return UNDEF;
					 }
				     }
<ARG>"="                             {/*Catch assignment syntax*/
                                      /*Eat initial whitespaces*/
                                      ECHO;
                                      BEGIN(VARARG); 
                                      /* char c; */
                                      /* while(((c=input()) == ' ') && */
				      /* 	     (c != EOF)); */
                                      /* unput(c); */
                                      return '=';
                                     }
<ARG>[^=]                            {/*Catch syntax for unsetting a var*/
                                      ECHOS("UNPUT");
                                      BEGIN(CMDARG); unput(yytext[0]);
                                     }
<CMDARG,VARARG>[$].*                 {
                                      int i=strlen(yytext)-1; 
                                      ECHOS("DREF");
                                      BEGIN(VARNAME);
                                      while(i>=1) unput(yytext[i--]);
                                      return yytext[0];
                                     }
<VARARG,CMDARG>.*                    {/*Catch STRINGS.  Anthing following*/
                                      /*command is a STRING              */
                                       int i=0;
                                       BEGIN(INITIAL);
                                       ECHOS("STR");
				       /*
                                       while((yytext[i]==' ')||
					     (yytext[i]=='\t')) i++;
				       */
				       sh_lval.String=(char *)
					 malloc(strlen(&yytext[i])+1);
				       
				       strcpy(sh_lval.String,&yytext[i]);
				       return STRING;
				     }
\n                                   {/*Catch a newline and set the state */
                                      /*to initial                        */
                                       ECHO;BEGIN(INITIAL);return '\n';
                                     }

<<EOF>>                              {return FINIS;}
%%




