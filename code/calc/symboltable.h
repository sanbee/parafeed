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
#if !defined(CALC_SYMBOLTABLE_H)
/* $Id */
#define CALC_SYMBOLTABLE_H
#include <values.h>
#include <math.h>
#include <calc.h>
/*
  Following is not defined with -ansi -pedantic options of gcc!
  */
extern double rint(double x); 
Calc_Symbol Builtins[] =
{ {"sin",   BUILTIN, 0, sin, 0, 0},
  {"cos",   BUILTIN, 0, cos, 0, 0},
  {"tan",   BUILTIN, 0, tan, 0, 0},
  {"asin",  BUILTIN, 0, asin,0, 0},
  {"acos",  BUILTIN, 0, acos,0, 0},
  {"atan",  BUILTIN, 0, atan,0, 0},
  {"atan2", BUILTIN2, 0, 0, atan2, 0},
  {"cosh",  BUILTIN, 0, cosh,0, 0},
  {"sinh",  BUILTIN, 0, sinh,0, 0},
  {"tanh",  BUILTIN, 0, tanh,0, 0},
  {"exp",   BUILTIN, 0, exp, 0, 0},
  {"ln",    BUILTIN, 0, log, 0, 0},
  {"log",   BUILTIN, 0, log, 0, 0},  /* Natural log */
  {"log10", BUILTIN, 0, log10,0,0},
  {"sqrt",  BUILTIN, 0, sqrt, 0,0},
  {"fabs",  BUILTIN, 0, fabs, 0,0},
  {"floor", BUILTIN, 0, floor,0,0},
  {"ceil",  BUILTIN, 0, ceil, 0,0},
  {"rint",  BUILTIN, 0, rint, 0,0}, /* Round off using IEEE rounding direction*/
  {"todeg", BUILTIN, 0, todeg, 0,0},
  {"todms", BUILTIN, 0, todms, 0,0},
  {"b2j"  , BUILTIN2, 0, 0, b2j,   0},
  {"j2b"  , BUILTIN2, 0, 0, j2b,   0},
  {0,0,0,0,}
};

Calc_Symbol Constants[] =
{ {"PI",   VAR, M_PI, 0,0},
  {"C",    VAR, 299792458.0,0,0},         /* Speed of light in meter/sec*/
  {"R2D",  VAR, 180.0/M_PI, 0,0},         /* Deg to Radians */
  {"D2R",  VAR, M_PI/180.0, 0,0},         /* Radians to Deg */
  {"H2R",  VAR, 15.0*M_PI/180.0, 0,0},    /* Hours to Radians */
  {"R2H",  VAR, 180.0/(15.0*M_PI), 0,0},  /* Radians to Hours */
  {"SOL2SID", VAR, (23.0+56.0/60.0+4.091/3600.0)/24.0, 0,0}, /* Solar to Sidreal sec. */
  {"SID2SOL", VAR, 24.0/(23.0+56.0/60.0+4.091/3600.0), 0, 0},
  {"kb",      VAR, 1.380470e-23, 0, 0}, /* Boltzman's constant (MKS) */
  {"PC2M",    VAR, 3.085678e+16, 0, 0}, /* Parsec to meters */
  {"AU2M",    VAR, 1.495979e+11, 0, 0}, /* AU to meters */
  {0,0,0,0,0}
};
#endif

