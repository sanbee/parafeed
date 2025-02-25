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
/* $Id: cldbggetIVal.c,v 2.0 1998/11/11 07:13:00 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>
#include <support.h>
#include <clparseVal.h>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as an integer
------------------------------------------------------------------------*/
int dbgclgetIVal(char *Name, int *val, int *n)
{
  Symbol *S;
  double d;
  int N;

HANDLE_EXCEPTIONS(

  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);  
  else
    S=SearchQSymb(Name,(char *)"int");

  if (S != NULL) S->Class=CL_DBGCLASS;

  if ((N=clparseVal(S,n,&d))!=CL_FAIL) *val = (int)d;

  return N;
)
}
#ifdef __cplusplus
	   }
#endif
#ifdef __cplusplus
int dbgclgetIValp(const string& Name, int& val, int& n)
{
  Symbol *S;
  double d;
  int N;
HANDLE_EXCEPTIONS(
		  if (n < 0)
		    S=SearchVSymb((char *)Name.c_str(),cl_SymbTab);  
		  else
		    S=SearchQSymb((char *)Name.c_str(),(char *)"int");
		  if (S != NULL) S->Class=CL_DBGCLASS;
		  setAutoIDefaults(S,val);
		  if ((N=clparseVal(S,&n,&d))>0) val = (int)d;
		  return N;
		  );
}
#endif
