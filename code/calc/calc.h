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
#if !defined(CALC_H)
/* $Id: calc.h,v 1.9 2000/02/22 15:08:12 sanjay Exp $ */
#define CALC_H
#include <math.h>

//#define yyin cl_in
//#define yyout cl_out

typedef struct {
  char *Name;
  int type;
  double value;
  double (*func1)(double);
  double (*func2)(double,double);
  double (*ufunc)(char *);
} Calc_Symbol;
extern double Result;
extern Calc_Symbol s2;
extern double (*UserFunc)(char *);
int yylex();
int calc_error(char *s);
int ywarn(char *s, char *t);
int calc(char *, double *);

Calc_Symbol *calcget(char *);
double todeg(double);
double todms(double);
double b2j(double, double);
double j2b(double, double);
#endif
#define LINELEN 128
extern char Calc_line[LINELEN];
extern int Calc_index;
