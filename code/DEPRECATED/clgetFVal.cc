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
/* $Id: clgetFVal.c,v 2.1 1999/03/23 15:01:49 sanjay Exp $ */
#include <shell.h>
#include <cllib.h>
#include <string.h>
#include <support.h>
#include <clgetBaseCode.h>
#include <clgetValp.h>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as an float
------------------------------------------------------------------------*/
int clgetFVal(char *Name, float *val, int *n)
{
  Symbol *S;
  int N;
  double d;

HANDLE_EXCEPTIONS(  
  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);
  else
    S=SearchQSymb(Name,(char *)"float");
  if ((N=clparseVal(S,n,&d))>0) *val=(float)d;
  if (S!=NULL) SETBIT(S->Attributes,CL_FLOATTYPE);
  return N;
)
}   

#ifdef __cplusplus
	   }
#endif
#ifdef __cplusplus

int clgetFValp(const string& Name, float &val, int &n)
{
  HANDLE_EXCEPTIONS(
		    SMap empty;
		    return clgetValp(Name,val,n,empty);
		   );
}
int clgetFValp(const string& Name, float& val, int& n, SMap &smap)
{
  HANDLE_EXCEPTIONS(
		    return clgetValp(Name,val,n,smap);
		   );
}

#endif   

