#if !defined(CALC_H)
/* $Id: calc.h,v 1.9 2000/02/22 15:08:12 sanjay Exp $ */
#define CALC_H
#include <math.h>

#define yyin cl_in
#define yyout cl_out

typedef struct {
  char *Name;
  int type;
  double value;
  double (*func1)(double);
  double (*func2)(double,double);
  double (*ufunc)(char *);
} Calc_Symbol;
int yylex();
int yyparse();
/* int yyerror(char *); */
int calc(char *, double *);

Calc_Symbol *calcget(char *);
double todeg(double);
double todms(double);
double b2j(double, double);
double j2b(double, double);
#endif
