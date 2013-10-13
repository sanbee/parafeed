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
%{
/* $Id: calc.y,v 1.8 2000/02/22 15:08:15 sanjay Exp $ */
#include <calc.h>
#include <math.h>
#include <calc.h>
#include <stdlib.h>
double Result=0.0;

 int calc_error(char *); /* dummy decleration */
Calc_Symbol s2;
double (*UserFunc)(char *) = 0;
%}
%union {
  double val;
  Calc_Symbol *symb;
}

%token <val> NUMBER
%token <symb>   BUILTIN VAR UNDEF BUILTIN2
%type  <val> expr

%left '+' '-'
%left '*' '/' '%'
%left MINUS
%right '^' 
/*'**'*/
%%
list:   '\n'
        |  list '\n'
        |  expr '\n'             { Result= $1; return 0;        }
	;
expr:     NUMBER                 { $$ = $1;                     }
        | VAR                    { $$ = ($1->value);            }
        | BUILTIN '(' expr ')'   { $$ = (*($1->func1))($3);      }
        | BUILTIN2 '(' expr ','  expr ')'   { $$ = (*($1->func2))($3,$5);      }
        | expr '+' expr          { $$ = $1 + $3;                }
        | expr '-' expr          { $$ = $1 - $3;                }
        | expr '*' expr          { $$ = $1 * $3;                }
        | expr '/' expr          { $$ = $1 / $3;                }
        | expr '%' expr          { $$ = (int)$1 % (int)$3;      }
        | '-' expr %prec MINUS   { $$ = -$2;                    } /* -x   */
        | expr '^' expr          { $$ = pow($1,$3);             } /* x^y  */
        | expr '*' '*' expr      { $$ = pow($1,$4);             } /* x**y */
        | '(' expr ')'           { $$ = $2;                     }
        | UNDEF                  { if ($1->ufunc != NULL)
                                    {
				     $$ = (*($1->ufunc))($1->Name);
                                     if ($1->Name) free($1->Name);
                                     $1->Name = NULL;
				    }
				   else
				     {
				       yyerror("Undefined symbol");
                                       if ($1->Name) free($1->Name);
                                       $1->Name = NULL;
				       return -1;                   
				     }
				 }
        ;
%%
/* Set the user defined function to resolve "external" variables */
/* Currently handles only true variables - not functions         */
void calcufunc(double (*func)(char *))
{
  UserFunc = func;
}
#ifdef DEBUG
#include <ctst.c>
#endif
#include <lex.yy.c>
#include <yyerror.c>
