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
/* $Id */
#include <stdio.h>
#include <string.h>
#include <calc.h>
#include <y.tab.h>
#include <symboltable.h>
#include <math.h>

Calc_Symbol *calcget(char *t)
{
  int i;

  for (i=0; Builtins[i].Name; i++)
    if (strcmp(t,Builtins[i].Name)==0) 
      return &Builtins[i];

  for (i=0; Constants[i].Name; i++)
    if (strcmp(t,Constants[i].Name)==0) 
      return &Constants[i];

  return (Calc_Symbol *)0;
}

double todeg(double sec) { return sec/3600.0;}
double Internaltodms(double sec,int *Sign, int *D, int *M, double *S)
{
  float t;

  
  *Sign = (sec < 0)?-1:1;
  t = fabs(sec);
  *D = (int)(t/3600.0);
  t=t-*D*3600;
  *M = (int)(t/60.0);
  *S = t-*M*60.0;
  /*
  *D = (int)fabs(deg);
  t=deg-(int)deg;
  t=t*60.0;
  *M = (int)fabs(t);
  t = t-(int)t;
  *S = fabs(t*60.0);
  */

  return 1;
}
  
double todms(double sec)
{
  int Sign,D,M;double S;

  Internaltodms(sec,&Sign,&D,&M,&S);
  if (Sign==1) fprintf(stderr,"%d %d %f\n",D,M,S);
  else fprintf(stderr,"-%d %d %f\n",D,M,S);
  return 1;
}

#define D2R (M_PI/180.0)
#define ASEC 1.0;
/*  Using the approximate formulas from the Almanac (1997) */

double InternalFromJ2000(double T,int Sgn, double RA, double Dec)
{
  double CM=Sgn*D2R*((1.2812323 + 
		       0.0003879*T) + 
		       0.0000101*T*T)*T;
  double CN=Sgn*D2R*((0.5567530 + 
		       0.0001185*T) + 
		       0.0000116*T*T)*T;
  double JRA, JDec,RAMean, DecMean;
  int Sign,D,M; double S;


  RA *= 15*D2R/3600.0;
  Dec *= D2R/3600.0;

  RAMean = RA - 0.5*(CM+CN*sin(RA)*tan(Dec));
  DecMean= Dec -0.5*CN*cos(RAMean);

  JRA  = RA - CM - CN*sin(RAMean)*tan(DecMean);
  JDec = Dec - CN*cos(RAMean);


  Internaltodms(JRA/(15*D2R),&Sign,&D,&M,&S);
  if (Sign==1) fprintf(stderr,"%dh %dm %fs\t",D,M,S);
  else fprintf(stderr,"-%dh %dm %fs\t",D,M,S);
  
  Internaltodms(JDec/D2R,&Sign,&D,&M,&S); 
  if (Sign==1) fprintf(stderr,"%dd %d\' %f\"\n",D,M,S);
  else fprintf(stderr,"-%dd %d\' %f\"\n",D,M,S);

  return 1;
}

double b2j(double RA, double Dec){return InternalFromJ2000(0.5,-1,RA,Dec);}
double j2b(double RA, double Dec){return InternalFromJ2000(0.5, 1,RA,Dec);}
/*
main()
{
  Calc_Symbol *s;
  char str[16];
  float arg;

  scanf("%s",str);
  if ((s=calcget(str))==0)
    {
      fprintf(stderr,"Not found\n");
      exit (-1);
    }
  if (s->type == VAR) printf("%s %lf\n",s->Name,s->value);
  else                
    {
      printf("Arg");
      scanf("%f", &arg);printf("%s %lf\n", s->Name, s->func(arg));
    }
}
*/
