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
/* $Id: cldbggetSVal.c,v 2.0 1998/11/11 07:13:01 sanjay Exp $ */
#include <cllib.h>
#include <shell.h>

#ifdef __cplusplus
extern "C" {
#endif
/*------------------------------------------------------------------------
   Return the Nth value of Name as a string
------------------------------------------------------------------------*/
int dbgclgetSVal(const char *Name, char *val, int *n)
{
  Symbol *S;
  unsigned int N;
  
  if (*n < 0)
    S=SearchVSymb(Name,cl_SymbTab);
  else
    S=SearchQSymb(Name,(char *)"string");
  N = _ABS(*n);

  if (S!=NULL) 
    {
      S->Class = CL_DBGCLASS;
      if (N <= S->NVals) 
	{
	  std::string trimmed=trim(S->Val[N-1]);
	  int len=trimmed.size();
	  strncpy(val, trimmed.c_str(), len);
	  return len;
	  // char *buf,*c=NULL;
	  // buf = (char *)S->Val[N-1].c_str();
	  // while (*buf == ' ') buf++;
	  // strncpy(val,buf,strlen(buf)+1);
	  // if ((c=strstr(buf,"\\\""))) 
	  //   while (*c) *c = *(++c);
	  // return strlen(buf);
	}
      else return CL_FAIL;
    }
  else
    return CL_FAIL;
}
#ifdef __cplusplus
	   }
#endif
