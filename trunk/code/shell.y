/* -*- C -*- */
%{
/*
 * Copyright (c) 2000-2008, 2009 S.Bhatnagar
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
/* $Id: shell.y,v 2.1 1999/04/19 03:41:07 sanjay Exp $ */
#include <stdio.h>
#include <clshelldefs.h>
#include <cl.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <shell.h>
#ifdef GNUREADLINE
#define PROMPT(s)   
#else
#define PROMPT(s)   fprintf(stderr,"%s>",s)
#endif


#define YYDEBUG 0


Symbol *sh_SymbTab;
CmdSymbol *sh_CmdTab;
char *sh_Prompt;
char *sh_sys_cmd=NULL;

 void FreeStr(char **dest) {if (*dest != NULL) free(*dest); *dest=NULL;}

 int CopyStr(char **dest, const char *source) 
   {
     if (!(*source) || 
	 (strlen(source) == 0)) return 1;

     if (*dest == NULL)
       *dest=(char *)calloc(strlen(source)+1,1);
     else *dest = (char *)realloc(*dest,strlen(*dest)+strlen(source)+1);
     if (*dest == NULL)
       {
	 yyerror((char *)"No memory left!");
	 return EOF;
       }
     else strcat(*dest,source);
     return 1;
   }

 int RangeOK(Symbol *s, unsigned int N)
   {
     if (N >= s->NVals)
       {
	 fprintf(stderr,"###Error: No. of vals=%d\n",s->NVals);
	 return 0;
       }
     return 1;
   }
%}
%union {
  double    Result;
  char      *String;
  Symbol    *symb;
  CmdSymbol *cmd;
}
%token <symb>   VAR 
%token <cmd>    COMMAND
%token <String> STRING 
%token <String> UNDEF 
%token <Result> NUMBER 
%token <Result> FINIS
%type  <String> expr externcmd
%type  <Result> stmt comd
%type  <symb>   asign
%%
/*--------------------------------------------------------------------------*/
list:     '\n'                   { PROMPT(sh_Prompt);return 1;}
/*        | list '\n'              { PROMPT(sh_Prompt);return 1;}*/
        | stmt '\n'              { PROMPT(sh_Prompt);return (int)$1;}
        | FINIS                  { exit(0);}
        ;
/*--------------------------------------------------------------------------*/
asign:     VAR '='                {SetVar($1->Name,NULL,sh_SymbTab,0);$$=$1;}

        |  VAR '=' expr           { 
                                    if ($3) SetVar($1->Name,$3,sh_SymbTab,0);
	                            $$=$1;
	                          }

        |  VAR '[' NUMBER ']' '=' expr 
                                  { 
	                            if (RangeOK($1,(int)$3))
				      SetVal($6,$1,(int)$3);
				    $$=$1;
				  }

        |  VAR '[' NUMBER ']' '=' '$' VAR 
                                  {
				    if (RangeOK($1,(int)$3))
				      {
					unsigned int i; char *s=NULL;
					for (i=0;i<$7->NVals;i++)
					  CopyStr(&s,$7->Val[i]);
					SetVal(s,$1,(int)$3);
					FreeStr(&s);
				      }
				    $$=$1;
				  }

        |  VAR '=' '$' VAR        { 
	                             unsigned int i;
				     for(i=0;i<$4->NVals;i++)
				       SetVal($4->Val[i],$1,i);
				     $$=$1;
                                  }
        ;
/*--------------------------------------------------------------------------*/
expr:     STRING                 {$$=$1;}

        | '$' VAR '[' NUMBER ']' {
                                   if ((unsigned int)$4>=$2->NVals)
	                           {fprintf(stderr,"###Error: No. of vals=%d\n"
					    ,$2->NVals);$$=NULL;} 
	                           else $$=$2->Val[(int)$4];
                                 }
        ;
/*--------------------------------------------------------------------------*/
externcmd:  UNDEF                   {CopyStr(&sh_sys_cmd,$1);free($1);}

           |  VAR                   {CopyStr(&sh_sys_cmd,$1->Name);}

           |  UNDEF '$' VAR         {
                                     unsigned int i;
	                             CopyStr(&sh_sys_cmd,$1);free($1);
	                             CopyStr(&sh_sys_cmd," ");
	                             for(i=0;i<$3->NVals;i++)
				       CopyStr(&sh_sys_cmd,$3->Val[i]);
                                    }

           |  UNDEF expr            { 
	                              CopyStr(&sh_sys_cmd,$1);free($1);
                                      CopyStr(&sh_sys_cmd," ");
	                              CopyStr(&sh_sys_cmd,$2);free($2);
                                    }
          ;
/*--------------------------------------------------------------------------*/
comd:    asign                   {$$=1;}

        | COMMAND                { if (($$=$1->func(NULL))==EOF) YYACCEPT;}

        | COMMAND expr           { 
                                   if (($$=$1->func($2))==EOF) 
	                             YYACCEPT;
                                 }

        | externcmd              { 
                                   system(sh_sys_cmd); 
	                           FreeStr(&sh_sys_cmd);
                                   $$=1;
				 }

        | UNDEF '=' expr         { 
                                   yyerror((char *)"undefined symbol");
				   /*FreeVSymb($1);free($3);*/
                                   $$=1;
				 }

        | COMMAND '='            { 
                                   yyerror((char *)"Illegal command syntax");
                                   $$=1;
                                 }
        ;
/*--------------------------------------------------------------------------*/
stmt:     '$' VAR                {PrintVals(stderr,$2);$$=1;}

        | expr                   {if ($1) fprintf(stderr,"%s\n",$1);$$=1; }

        | comd                   {}
        ;
/*--------------------------------------------------------------------------*/
%%
