/* -*- C -*- */
%{
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
/* $Id: shell.y,v 2.1 1999/04/19 03:41:07 sanjay Exp $ */
#include <stdio.h>
#include <clshelldefs.h>
#include <cl.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <shell.h>
#include <sstream>
  
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
	 fprintf(stderr,"###Error: Range of index is [0 - %d]\n",s->NVals-1);
	 return 0;
       }
     return 1;
   }

 void catchHiddenSymbol(Symbol *s)
 {
   int v = CL_ALLOWHIDDEN; // Default: Allow accessing hidden
			   // variables for any operation
   char *valStr=NULL;
   if ((valStr = std::getenv("CL_ALLOWHIDDEN")) != NULL)
     {
       stringstream toT2(valStr);
       toT2 >> v;
     }
   //   v=cl_getenv("CL_ALLOWHIDDEN",1); // DOES NOT FIND THE TEMPLATE
   //   FUNCTION AT LINK TIME.  I DON'T KNOW WHY!

   if (v==CL_ALLOWHIDDEN) return; // Allow setting hidden variables, without warning even.
   
   if (s->Exposed!=1)
     {
       if (v==CL_ALLOWHIDDEN_WITHWARNING) // Allow, but give a warning
	 {
	   string msg="Accessing a hidden variable (named \'" + string(s->Name) + "\')."; 
	   clThrowUp(msg.c_str(),"###Error",CL_INFORMATIONAL);
	   //clThrowUp(msg.c_str(),"###Error",CL_SEVERE);
	 }
       else if (v==CL_STRICTLY_NOT_ALLOWHIDDEN) // Strictly not allowed.  Value is left unchanged and an exception is emitted.
	 {
	   string msg="Attempted access of a hidden variable (named \'" + string(s->Name) + "\').";
	   clError x(msg,string("###Error"),CL_INFORMATIONAL);
	   throw(x);
	 }
     }
 }
%}
//%language "c++"
%code requires {
#include <iostream>
#include <stdio.h>

/* If you insist :) */
//using namespace std;

}

%code provides {

  //void yyerror (char const*);
//int  yylex (YYSTYPE*, YYLTYPE*);
extern "C" int yyparse ();
}
%union yy_lval{
  double    Result;
  char      *String;
  Symbol    *symb;
  CmdSymbol *cmd;
/*   std::string tt; */
/* yy_lval(): tt() {}; */
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
asign:     VAR '='                {
                                    catchHiddenSymbol($1);
                                    SetVar($1->Name,NULL,sh_SymbTab,0,0,1);$$=$1;
                                  }

        |  VAR '=' expr           {
                                    catchHiddenSymbol($1);
                                    if ($3) SetVar($1->Name,$3,sh_SymbTab,0,0,1);
	                            $$=$1;
				    FreeStr(&$3);
	                          }

        |  VAR '[' NUMBER ']' '=' expr 
                                  { 
                                    catchHiddenSymbol($1);
	                            if (RangeOK($1,(int)$3))
				      SetVal($6,$1,(int)$3);
				    $$=$1;
				    FreeStr(&$6);
				  }

        |  VAR '[' NUMBER ']' '=' '$' VAR 
                                  {
                                    catchHiddenSymbol($1);
                                    catchHiddenSymbol($7);
				    if (RangeOK($1,(int)$3))
				      {
					unsigned int i; char *s=NULL;
					for (i=0;i<$7->NVals;i++)
					  CopyStr(&s,(char *)$7->Val[i].c_str());
					SetVal(s,$1,(int)$3);
					FreeStr(&s);
				      }
				    $$=$1;
				  }

        |  VAR '=' '$' VAR        { 
                                    catchHiddenSymbol($1);
                                    catchHiddenSymbol($4);
	                             unsigned int i;
				     for(i=0;i<$4->NVals;i++)
				       SetVal((char *)$4->Val[i].c_str(),$1,i);
				     $$=$1;
                                  }
        ;
/*--------------------------------------------------------------------------*/
expr:     STRING                 {$$=$1;}

        | '$' VAR '[' NUMBER ']' {
                                    catchHiddenSymbol($2);

                                   /* if ((unsigned int)$4>=$2->NVals) */
				   /*   { */
				   /*     fprintf(stderr,"###Error: Index should be in the range [0-%d]\n" */
				   /* 	    ,$2->NVals-1);$$=NULL; */
				   /*   }  */
	                           if (RangeOK($2, (int)$4)) $$=(char *)$2->Val[(int)$4].c_str();
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
				       CopyStr(&sh_sys_cmd,(char *)$3->Val[i].c_str());
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
				   FreeStr(&$1);
				   FreeStr(&$3);
                                   $$=1;
				 }

        | COMMAND '='            { 
                                   yyerror((char *)"Illegal command syntax");
                                   $$=1;
                                 }
        ;
/*--------------------------------------------------------------------------*/
stmt:     '$' VAR                {
                                   catchHiddenSymbol($2);

                                   PrintVals(stderr,$2,1);$$=1;
                                 }

        | expr                   {if ($1) fprintf(stderr,"%s\n",$1);$$=1; }

        | comd                   {}
        ;
/*--------------------------------------------------------------------------*/
%%
