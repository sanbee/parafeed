%{
/* -*- C -*- */
/* $Id: lex.l,v 1.2 1998/09/18 09:30:12 sanjay Exp $ */
#include <stdio.h>
#include <string.h>
#include <calclib.h>
#include <calc.h>
/* 
   Redefine macros to take input from internal memory.  Ignore the warnings
   about redefinition of these macros while compiling 
*/
/*
#define yyin cl_in
#define yyout cl_out
#define yytext cl_text
*/
#undef input
#undef unput
#undef output
#undef ECHO
#define input() ((cl_index==strlen(line))?0:line[cl_index++])
#define unput(C) {line[cl_index==0?cl_index:--cl_index]=C;}
#define output(C) {fprintf(stderr,"%c",C);}
#define ECHON  {fprintf(stderr,"\"\\n\"\n");}
#define ECHO  /*{fprintf(stderr,"\"%s\"\n",yytext);}*/

char line[LINELEN];
int cl_index=0;

int calc(char *s, double *v)
{
  int i=0;
  cl_index=0;
  strncpy(inp,s,LINELEN);
  strncpy(line,s, LINELEN);
  strcat(line,"\n");
  if ((i=yyparse()) != 0)
    {
      ywarn(" syntax error ", (char *)0);
      return -1;
    }
  *v = Result;
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
DIGIT   [0-9]+
HEXD    0x[0-9a-fA-F]++
S       [\+\-]?
DOT     \.
NUM     (({DIGIT})|({DIGIT}{DOT})|({DIGIT}{DOT}{DIGIT})|({DOT}{DIGIT}))
CREAL   ({NUM}|({NUM}[Ee]{S}{DIGIT}))
FREAL   ({NUM}|({NUM}[Dd]{S}{DIGIT}))
HDEG    ({NUM}h)
HMIN    ({NUM}m)
HSEC    ({NUM}s)
HANG    (({HDEG}{HMIN}?)|({HDEG}{HSEC}?)|({HMIN}{HSEC}?)|{HSEC}|({HDEG}({HMIN}{HSEC}?)?))

DDEG    ({NUM}d)
DMIN    ({NUM}\')
DSEC    ({NUM}\")
DANG    (({DDEG}{DMIN}?)|({DDEG}{DSEC}?)|({DMIN}{DSEC}?)|{DSEC}|({DDEG}({DMIN}{DSEC}?)?))
%%
[ \t]                     {;}
\/\*                      {
                           int n=strlen(line);
                           while(cl_index < n)
                             if (!strncmp(&line[cl_index++],"*/",2)) 
                               {cl_index++;break;}
                          }
{HEXD}                    {
                            int i;
                            ECHO;
                            sscanf(yytext+2,"%x",&i);yylval.val = i;
                            return NUMBER;
                          }
{HANG}                    {/* Numer in the hour angle format */
                           float a[3];
                           split3(yytext,yyleng,"hms",a);
                           yylval.val = (a[0]*60.0+a[1])*60.0+a[2];
/*                           yylval.val = a[0]+(a[1]+a[2]/60.0)/60.0;*/
			   return NUMBER;
			  }
{DANG}                    {/* Number in the angular format */
                           float a[3];
                           split3(yytext,yyleng,"d\'\"",a);
                           yylval.val = (a[0]*60.0+a[1])*60.0+a[2];
/*                           yylval.val = a[0]+(a[1])+a[2]/60.0)/60.0;*/
			   return NUMBER;
			  }
{CREAL}                   {sscanf(yytext,"%lf",&yylval.val);return NUMBER;}
{FREAL}                   {
                           float m,e; char c;
			   ECHO;
			   sscanf(yytext, "%f%c%f", &m,&c,&e);
			   yylval.val = m*pow(10.0,e);
			   return NUMBER;
			  }
atan2                     { Calc_Symbol *s;   /* A patch for atan2...*/
			    ECHO;
                            if ((s=calcget(yytext))!=0)
                              { yylval.symb=s; return s->type;}
                          }
[a-zA-Z][a-zA-Z0-9]*      {
                            Calc_Symbol *s;
			    ECHO;
                            if ((s=calcget(yytext))!=0) 
                             {
                               yylval.symb=s;
                               return s->type;
                             }
                            else 
                              {
				s2.ufunc = UserFunc;
				s2.Name = (char *)malloc(strlen(yytext)+1);
				strcpy(s2.Name,yytext);
				yylval.symb = &s2;
				return UNDEF;
			      }
                          }
\n                        {ECHON;return '\n';}
.                         {ECHO;return yytext[0];}



