%{/* -*- C -*- */
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
/* $Id: lex.l,v 1.15 2000/07/01 18:15:40 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <calc.h>
#ifdef DEBUG
#include <ctst.c>
#endif
#include <y.tab.h>
char Calc_line[LINELEN];
int Calc_index;

/*----------------------------------------------------------------------*/
    int calc_error(char *s)
    {
        /*  ywarn(s, (char *) 0);*/
        return 1;
    }
/*----------------------------------------------------------------------*/
    int ywarn(char *s, char *t)
    {
        int i,N=0;
        char *str = "###Error:";
        N += strlen(str)+strlen(s)+1;
        fprintf(stderr, "%s %s", str,s);
        if (t)
        {
            N += strlen(t)+1;
            fprintf(stderr, " %s", t);
        }
        N += 4;
        fprintf(stderr," in ");
        fprintf(stderr, "%s",Calc_line);
        N = N + Calc_index - yyleng;
        for (i=0;i<N; i++) fprintf(stderr," ");
        fprintf(stderr,"^\n");
        return 1;
    }

/*
   Redefine macros to take input from internal memory.  Ignore the warnings
   about redefinition of these macros while compiling 
*/
/*
#define yyin Calc_in
#define yyout Calc_out
#define yytext Calc_text
*/
int Calc_index=0;

#undef input
#undef unput
#undef output
#undef ECHO
#define INCR_INDEX  {Calc_index += yyleng;}
/* Used by FLEX.  But using yy_scan_string instead of this now in FLEX 
#define YY_INPUT(buf,result,max_size) \
  {                                   \
   result = (Calc_index==strlen(Calc_line)) ? YY_NULL : (buf[0]=Calc_line[Calc_index++],1 );\
   }
*/
/* Used by LEX.
#define YY_INPUT() ((Calc_index==strlen(Calc_line))?0:Calc_line[Calc_index++])
*/
#define unput(C) {Calc_line[Calc_index==0?Calc_index:--Calc_index]=C;}
#define output(C) {fprintf(stderr,"%c",C);}
#define ECHON /* {fprintf(stderr,"\"\\n\"\n");}*/
#define ECHO  /*{fprintf(stderr,"\"%s\"\n",yytext);}*/

/*yy_scan_buffer(Calc_line,strlen(Calc_line));*/

int calc(char *s, double *v)
{
  int i=0;
  yyleng=0;
  Calc_index=0;
/*  strncpy(inp,s,LINELEN);*/
  strncpy(Calc_line,s, LINELEN);
  strcat(Calc_line,"\n");
  yy_scan_string(Calc_line);
  if ((i=calc_parse()) != 0)
    {
      ywarn(" syntax error ", (char *)0);
      return -1;
    }
  *v = Result;
  yy_delete_buffer(YY_CURRENT_BUFFER);
  return i;
}
void split3(char *deg, int n, char *tok, float *d)
{
  int i=0,j=0;
  char *t=deg,val[16];

  d[0]=d[1]=d[2]=0.0;
  while(j < n)
    {
      while ((t[j] != tok[0]) &&(t[j] != tok[1]) && (t[j] != tok[2]))
	val[i++]=t[j++];
      val[i++]='\0';i=0;
      if      (t[j] == tok[0]) sscanf(val,"%f",&d[0]);
      else if (t[j] == tok[1]) sscanf(val,"%f",&d[1]);
      else if (t[j] == tok[2]) sscanf(val,"%f",&d[2]);
      j++;
    }
}

%}
%option noyywrap
DIGIT   [0-9]+
HEXD    0x[0-9a-fA-F]++
S       [\+\-]?
DOT     \.
PM      "+/-"
SPACE   [ ]++

NUM     ({DIGIT}({DOT})?({DIGIT})?)
CREAL   ({NUM}([Ee]{S}{DIGIT})?)
FREAL   ({NUM}([Dd]{S}{DIGIT})?)
RNUM    ({CREAL}|{FREAL})

HDEG    ({NUM}h)
HMIN    ({NUM}m)
HSEC    ({NUM}s)
HANG    ({HDEG}?({SPACE}?{HMIN})?({SPACE}?{HSEC})?)

DDEG    ({NUM}d)
DMIN    ({NUM}\')
DSEC    ({NUM}\")
DANG    ({DDEG}?({SPACE}?{DMIN})?({SPACE}?{DSEC})?)
%%
[ \t]                     {;}
\/\*                      {
                           int n=strlen(Calc_line);
                           while(Calc_index < n)
                             if (!strncmp(&Calc_line[Calc_index++],"*/",2)) 
                               {Calc_index++;break;}
                          }
{HEXD}                    {
                            int i;
                            ECHO;
                            sscanf(yytext+2,"%x",&i);calc_lval.val = i;
                            INCR_INDEX;
                            return NUMBER;
                          }
{HANG}                    {/* Numer in the hour angle format */
                           float a[3];
                           split3(yytext,yyleng,"hms",a);
                           calc_lval.val = (a[0]*60.0+a[1])*60.0+a[2];
/*                           calc_lval.val = a[0]+(a[1]+a[2]/60.0)/60.0;*/
                            INCR_INDEX;
			   return NUMBER;
			  }
{DANG}                    {/* Number in the angular format */
                           float a[3];
                           split3(yytext,yyleng,"d\'\"",a);
                           calc_lval.val = (a[0]*60.0+a[1])*60.0+a[2];
/*                           calc_lval.val = a[0]+(a[1])+a[2]/60.0)/60.0;*/
                            INCR_INDEX;
			   return NUMBER;
			  }
{CREAL}                   {
                            ECHO;
                            sscanf(yytext,"%lf",&calc_lval.val);
                            INCR_INDEX;
                            return NUMBER;
                          }
{FREAL}                   {
                           float m,e; char c;
			   ECHO;
			   sscanf(yytext, "%f%c%f", &m,&c,&e);
			   calc_lval.val = m*pow(10.0,e);
                            INCR_INDEX;
			   return NUMBER;
			  }
atan2                     { Calc_Symbol *s;   /* A patch for atan2...*/
			    ECHO;
                            INCR_INDEX;
                            if ((s=calcget(yytext))!=0)
                            { calc_lval.symb=s; return s->type;}
                          }
[a-zA-Z]+[a-zA-Z0-9]*      {
                            Calc_Symbol *s;
			    ECHO;
                            INCR_INDEX;
                            if ((s=calcget(yytext))!=0) 
                             {
                                 calc_lval.symb=s;
                               return s->type;
                             }
                            else
                              {
				s2.ufunc = UserFunc;
				s2.Name = (char *)malloc(strlen(yytext)+1);
				strcpy(s2.Name,yytext);
				calc_lval.symb = &s2;
				return UNDEF;
			      }
                          }
\n                        {ECHON;return '\n';}
.                         {ECHO;return yytext[0];}



