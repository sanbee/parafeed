/* -*- C -*- */
%{
/* $Id: calc.y,v 1.8 2000/02/22 15:08:15 sanjay Exp $ */
#include <calc.h>
#include <math.h>
#include <calc.h>
#include <stdlib.h>
double Result=0.0;

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
