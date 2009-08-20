/*
 * Copyright (c) 2000-2008, 2009 S.Bhatnagar
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
/* $Id: cldbggetSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as a string
------------------------------------------------------------------------*/
int dbgclgetSVal(char *Name, char *val, int *n)
{
  Symbol *S;
  unsigned int N;
  char *buf,*c=NULL;
  
  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);
  else
    S=SearchQSymb(Name,"string");
  N = _ABS(*n);

  if (S!=NULL) 
    {
      S->Class = CL_DBGCLASS;
      if (N <= S->NVals) 
	{
	  buf = S->Val[N-1];
	  while (*buf == ' ') buf++;
	  strncpy(val,buf,strlen(buf)+1);
	  if ((c=strstr(buf,"\\\""))) 
	    while (*c) *c = *(++c);
	  return strlen(buf);
	}
      else return CL_FAIL;
    }
  else
    return CL_FAIL;
}
#ifdef __cplusplus
	   }
#endif
