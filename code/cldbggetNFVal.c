/*
 * Copyright (c) 2000-2012, 2013 S.Bhatnagar
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
/* $Id: cldbggetNFVal.c,v 2.0 1998/11/11 07:13:00 sanjay Exp $ */
#include <cllib.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return N values of Name as a integers
------------------------------------------------------------------------*/
int dbgclgetNFVal(char *Name, float *val, int *m)
{
  int i=1,n;
  double d;
  char tmp[8];
  Symbol *S;

HANDLE_EXCEPTIONS(
  sprintf(tmp,"float[%d]",*m);
  S = SearchQSymb(Name, tmp);
  i=1;
  if (S) S->Class = CL_DBGCLASS;
  while(i <= *m)
    if ((n=clparseVal(S,&i,&d))==CL_FAIL) return n;
    else if (n==0) break;
    else {val[i-1] = (float)d;i++;}

  return i-1;
)
}
#ifdef __cplusplus
	   }
#endif
