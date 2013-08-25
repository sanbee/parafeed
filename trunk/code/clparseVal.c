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
/* $Id: clparseVal.c,v 2.0 1998/11/11 07:13:02 sanjay Exp sanjay $ */
#include <shell.h>
#include <cllib.h>
#include <cl.h>
//#include <string.h>
#include <string>
#include <vector>
#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Convert the n th value of symbol S into a double
------------------------------------------------------------------------*/

int clparseVal(Symbol *S, int *Which, double *d)
{
  unsigned int N = _ABS(*Which),n;

HANDLE_EXCEPTIONS(
  if (S != NULL)
    {
      if (N > S->NVals) return 0;
      if (ISSET(S->Attributes,CL_BOOLTYPE))
	{
	  string val(S->Val[N-1]);
	  if (clIsTrue(val)) *d=1;
	  if (clIsFalse(val)) *d=0;
	}
      else if ((n=calc(S->Val[N-1],d)))
	{
	  char msg[128];
	  sprintf(msg,"In conversion of %s[%d]=%s",S->Name,N-1,S->Val[N-1]);
	  clThrowUp(msg,"###Error",CL_FAIL);
	}
      return 1;
    }
  else return CL_FAIL;
)
}
#ifdef __cplusplus
	   }
#endif
