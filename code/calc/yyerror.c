/*
 * Copyright (c) 2000-2006, 2007 S.Bhatnagar
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
/* $Id$ */
#include <stdio.h>
/*----------------------------------------------------------------------*/
int yyerror(char *s)
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
